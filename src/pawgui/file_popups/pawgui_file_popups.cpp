/*
pawgui_file_popups.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/
#include "pawgui_file_popups.h"

namespace pawgui
{
    std::string  get_filename_open_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory)
    {
        return get_filename_plain_from_popup( prompt, allowedFileTypes,previousDirectory,false);
    }

    std::string get_filename_save_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory )
    {
        return get_filename_plain_from_popup( prompt, allowedFileTypes,previousDirectory,true);
    }

    std::string  get_directory_name_from_popup( const std::string & prompt, std::string &previousDirectory, bool isSaving )
    {
        return get_filename_plain_from_popup(prompt,"",previousDirectory,isSaving, true);
    }

    std::string get_filename_plain_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory,bool isSaving, bool file_is_dirSearch )
    {
        //resource_dragged = nullptr;
        gpe::file_object * currentFile = nullptr;
        gpe::file_directory_class * currentDirectory = new gpe::file_directory_class();

        gpe::cursor_main_controller->cursor_change("arrow");
        gpe::error_log->report("[Opening file] Plainfile Function... using <"+previousDirectory+"> as previous.");
        main_overlay_system->take_frozen_screenshot();
        std::string returnFile = "";
        std::string currentDirectoryInView = "";
        std::string nextDirectoryToView = "";
        std::string soughtReturnFilename = "";
        std::string userHomePath = "";
        std::string  osFileFilterstring = "All types(*.*)\0*.*\0";
        int filesAndDirectoryPassedFilterCount = 16;
        int maxCharsToView = 16;
        bool fileMatchesFilter = false;
        bool mouseIsInBrowserBox = false;
        float imagePreview_scaleSize = 1;
        char* homeDir;
        std::string iDirLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        homeDir = getenv("%UserProfile%");
        if( homeDir!=nullptr)
        {
            userHomePath = currentDirectoryInView = homeDir;
        }
        else
        {
            homeDir = getenv("home");
            if( homeDir!=nullptr)
            {
                userHomePath = currentDirectoryInView = homeDir;
            }
            else
            {
                homeDir = getenv("HOME");
                if( homeDir!=nullptr)
                {
                    userHomePath = currentDirectoryInView = homeDir;
                }
                else
                {
                    homeDir = getenv("homepath");
                    if( homeDir!=nullptr)
                    {
                        userHomePath = currentDirectoryInView = homeDir;
                    }
                    else
                    {
                        userHomePath = currentDirectoryInView = gpe::app_directory_name;
                    }
                }
            }
        }

        for( int iLetter = 0; iLetter < (int)iDirLetters.size(); iLetter++)
        {
            if( sff_ex::path_exists(iDirLetters.substr(iLetter,1)+":"+userHomePath) )
            {
                userHomePath = iDirLetters.substr(iLetter,1)+":"+userHomePath;
                break;
            }
            else
            {
                gpe::error_log->report(iDirLetters.substr(iLetter,1)+":"+userHomePath+" is not a directory.");
            }
        }
        userHomePath = stg_ex::string_replace_all(userHomePath,"\\","/");
        currentDirectoryInView = userHomePath;
        if( sff_ex::path_exists(previousDirectory) )
        {
            currentDirectoryInView = previousDirectory.c_str();
        }
        if( (int)currentDirectoryInView.size()>0 && currentDirectory!=nullptr)
        {
            main_overlay_system->take_frozen_screenshot( );
            int font_sizeH = 12;
            int font_sizeW = 12;
            font_textinput->get_metrics("A",&font_sizeW,&font_sizeH);
            if( font_sizeW<=0)
            {
                font_sizeW = 12;
            }
            gpe::texture_base * previewedImageTexture = nullptr;
            widget_panel_list * shortCutGList = new widget_panel_list();

            std::string shorterCurrentDirectoryInView = currentDirectoryInView;

            std::vector<std::string > fileFilterTypes;
            if( currentDirectoryInView.size() > 32)
            {
                shorterCurrentDirectoryInView = currentDirectoryInView.substr(0,32);
            }
            gpe::input->reset_all_input();

            int iFile = 0;
            int filesProcessedCount = 0;
            int iDirectory = 0;
            bool enterMyComputerMode = false;
            bool exitOperation = false;
            if( currentDirectory->open_directory_sorted(currentDirectoryInView )== false )
            {
                exitOperation = true;
            }
            int TEXTBOX_FONT_SIZE_WIDTH = 12;
            int TEXTBOX_FONT_SIZE_HEIGHT = 12;

            int DRIVER_FONT_SIZE_WIDTH = 24;
            int DRIVER_FONT_SIZE_HEIGHT = 24;
            if( font_textinput!=nullptr)
            {
                font_textinput->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
            }
            if( font_textinput!=nullptr)
            {
                font_textinput->get_metrics("A",&DRIVER_FONT_SIZE_WIDTH,&DRIVER_FONT_SIZE_HEIGHT);
            }
            std::string returnVal = "";
            std::string fileToClick = "";
            std::string currentFileExt = "";
            std::string fileRenderName[3];
            fileRenderName[0] = "";
            fileRenderName[1] = "";
            fileRenderName[2] = "";
            std::vector <std::string> mountedDriversList;
            widget_dropdown_menu * fileTypeDropDown = new widget_dropdown_menu( "All types(*.*)",false);
            osFileFilterstring = gpe::parse_file_types(allowedFileTypes,fileFilterTypes);
            std::string tempFileType = "";
            for( int fDT = 0; fDT < (int)fileFilterTypes.size(); fDT++)
            {
                tempFileType  = fileFilterTypes.at(fDT);
                fileTypeDropDown->add_menu_option(tempFileType,tempFileType );
            }
            fileFilterTypes.clear();

            widget_button_label * yes_button = nullptr;
            if( isSaving)
            {
                yes_button= new widget_button_label( "Save","");
            }
            else
            {
                yes_button = new widget_button_label( "Open","");
            }

            gpe::animation2d * mainFilesanimation = nullptr;
            gpe::animation2d * mainMenuanimation = nullptr;


            widget_button_iconbar * fileBrowserModeBar = new widget_button_iconbar( 32,true);
            fileBrowserModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/table.png","Thumbnailed View",1,false);
            fileBrowserModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/list-alt.png","Tiled View",1,false);
            fileBrowserModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/list.png","Detailed View",1,false);
            int pastFileBrowserMode = 0;
            widget_button_push * mycomputer_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/archive.png","Computer","My Computer");
            widget_button_push * home_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/home.png","Home","View Home Disk Drives");
            widget_button_push * desktop_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/desktop.png","Desktop","Opens the Desktop folder");
            widget_button_push * documents_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/suitcase.png","Documents","Opens the Documents folder");
            widget_button_push * downloads_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/download.png","Downloads","Opens the Downloads folder");
            widget_button_push * pictures_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/image.png","Pictures","Opens the Pictures folder");
            widget_button_push * music_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/music.png","Music","Opens the Music folder");
            widget_button_push * video_buttons = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/film.png","Videos","Opens the Videos folder");

            widget_button_push * back_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/backward.png","Up Dir...","Traverses up one directory");
            widget_button_push * appDirectory_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/pencil.png","GPE Folder","Opens the drive to GPE IDE");
            widget_button_push * examplesDirectory_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/leanpub.png","Examples","Examples of Key and simple features");
            //widget_button_push * tutorialsDirectory_button = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/institution.png","Tutorials","Tutorials on using GPE");

            widget_button_label * cancel_button = new widget_button_label( "Cancel","");
            widget_input_text * newstringBox = new widget_input_text("");
            widget_input_text * directoryViewBox = new widget_input_text("Directory");
            directoryViewBox->set_string("Home");
            widget_button_icon * refresh_button = new widget_button_icon( "resources/gfx/iconpacks/fontawesome/refresh.png","Refresh");

            widget_scrollbar_yaxis * fileDirYScroll = new widget_scrollbar_yaxis();
            int maxFilesInColumn = 0;
            int maxFilesInRows = 0;
            int maxContentInView = 0;
            int iFileXPos = 0;
            int iDirectoryXPos = 0;
            int fileselectedNumber = -1;
            int fileHoverNumber = -1;
            int fileSubImageToDraw = 0;
            int colTextWidth = 128;
            int fileRowHeight = 128;
            int colTextCharacters = colTextWidth/font_sizeW;
            if( file_is_dirSearch)
            {
                newstringBox->set_label("Directory Name:");
            }
            else
            {
                newstringBox->set_label("File Name:");
            }
            newstringBox->switch_inuse(true);
            int buttonsWidth = yes_button->get_width()+cancel_button->get_width()+padding_default*2;

            gpe::shape_rect widget_box;
            widget_box.x = 0;
            widget_box.y = 0;
            widget_box.w = 512;
            widget_box.h = 416;

            gpe::shape_rect fileBrowserBox;
            fileBrowserBox.x = 160;
            fileBrowserBox.y = 64;
            fileBrowserBox.w = 512-32;
            fileBrowserBox.h = widget_box.h -384;

            gpe::shape_rect filePreviewBox;
            filePreviewBox.x = gpe::screen_width-(widget_box.x+widget_box.w);
            filePreviewBox.y = 64;
            filePreviewBox.w = gpe::screen_width-filePreviewBox.x;
            filePreviewBox.h = widget_box.h -384;

            gpe::shape_rect fileShortcutTab;
            fileShortcutTab.x = 0;
            fileShortcutTab.y = 64;
            fileShortcutTab.w = 144;
            fileShortcutTab.h = widget_box.h -384;

            gpe::shape_rect currFileRect;
            currFileRect.x = 0;
            currFileRect.y = 0;
            currFileRect.w = default_icon_width;

            int iContentCol = 0;
            int jContentRow = 0;

            gpe::renderer_main->reset_viewpoint();
            main_overlay_system->render_frozen_screenshot( );
            gpe::error_log->report("Attempting to open file selection menu...");
            //The lovely file selector loop
            nextDirectoryToView = currentDirectoryInView;
    #ifdef _WIN32
    #include "windows.h"
            if( !file_is_dirSearch)
            {
                const int BUf_size = 1024;
                char buffer[BUf_size] = {0};
                OPENFILENAME ofns = {0};
                ofns.hInstance = nullptr;
                ofns.hwndOwner = nullptr;
                ofns.lStructSize = sizeof( ofns );
                ofns.lpstrFile = buffer;
                ofns.nMaxFile = BUf_size;
                ofns.lpstrInitialDir  = currentDirectoryInView.c_str();
                ofns.lpstrTitle = prompt.c_str();
                ofns.lpstrFilter =  osFileFilterstring.c_str();

                ofns.nFilterIndex = 1;
                ofns.Flags =  OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
                bool emptyFileReturned = false;
                if( isSaving)
                {
                    if ( !GetSaveFileName( & ofns ) )
                    {
                        emptyFileReturned = true;
                    }
                }
                else if ( !GetOpenFileName( & ofns ) )
                {
                    emptyFileReturned = true;
                }
                returnFile = buffer;
                if( emptyFileReturned)
                {
                    returnFile = "";
                }
                exitOperation = true;
            }
    #endif

            gpe::texture_base * currentFIleTexture = nullptr;
            gpe::texture_base * textRepFolder = rsm_gui->texture_add("folderOpen", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder-open.png");
            gpe::texture_base * textRepHarddrive = rsm_gui->texture_add("harddriveIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/hdd-o.png");
            gpe::texture_base * textRepAudio = rsm_gui->texture_add("audioIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-audio-o.png");
            gpe::texture_base * textRepCode = rsm_gui->texture_add("codeIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-code-o.png");
            gpe::texture_base * textRepExe = rsm_gui->texture_add("paperPlane", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/paper-plane-o.png");
            gpe::texture_base * textRepFont = rsm_gui->texture_add("fontIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/font.png");
            gpe::texture_base * textRepGPE = rsm_gui->texture_add("gpeIconSquare", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/pencil-square.png");
            gpe::texture_base * textRepImage = rsm_gui->texture_add("fileBrowserImageFileIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-image-o.png");
            gpe::texture_base * textRepOther = rsm_gui->texture_add("fileBrowserFileIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png");
            gpe::texture_base * textRepPDF = rsm_gui->texture_add("fileBrowserPDFIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-pdf-o.png");
            gpe::texture_base * textRepText = rsm_gui->texture_add("fileBrowserTxtIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-text-o.png");
            gpe::texture_base * textRepVideo = rsm_gui->texture_add("fileBrowserVideoIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-video-o.png");
            gpe::texture_base * textRepZip = rsm_gui->texture_add("fileBrowserZipIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-zip-o.png");
            gpe::color * fileIconColor = nullptr;
            while(exitOperation==false)
            {
                gpe::cursor_main_controller->cursor_change("arrow");
                gpe::renderer_main->reset_viewpoint();
                if( !gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,
                                  fileBrowserBox.x,fileBrowserBox.y,
                                  fileBrowserBox.x+fileBrowserBox.w,
                                  fileBrowserBox.y+fileBrowserBox.h) )
                {
                    fileHoverNumber = -1;
                }

                ///
                if( (int)nextDirectoryToView.size() > 0 && sff_ex::path_exists(nextDirectoryToView) )
                {
                    gpe::error_log->report("Attempting to open ["+nextDirectoryToView+"] directory...");
                    if( sff_ex::path_exists(nextDirectoryToView)==false)
                    {
                        nextDirectoryToView+="\\";
                    }
                    if( sff_ex::path_exists(nextDirectoryToView) )
                    {
                        gpe::error_log->report("Opening... ["+nextDirectoryToView+"] directory...");

                        currentDirectoryInView = nextDirectoryToView;
                        directoryViewBox->set_string(currentDirectoryInView);
                        shorterCurrentDirectoryInView = currentDirectoryInView;
                        if( currentDirectoryInView.size() > 32)
                        {
                            shorterCurrentDirectoryInView = currentDirectoryInView.substr(0,32);
                        }
                        nextDirectoryToView = "";
                        fileHoverNumber = -1;
                        fileselectedNumber = -1;
                        enterMyComputerMode= false;
                        iFile = 0;
                        iDirectory = 0;
                        iFileXPos = 0;
                        iDirectoryXPos = 0;
                        fileDirYScroll->fullRect.x = fileDirYScroll->contextRect.x = 0;
                        fileDirYScroll->fullRect.y = fileDirYScroll->contextRect.y = 0;
                        fileDirYScroll->fullRect.w = fileDirYScroll->contextRect.w = 0;
                        fileDirYScroll->fullRect.h = fileDirYScroll->contextRect.h = 16;
                        fileDirYScroll->process_self();
                        //previewedImageTexture->change_texture(nullptr);

                        gpe::parse_file_types(fileTypeDropDown->get_selected_name(),fileFilterTypes);
                        //Processes files to view
                        filesAndDirectoryPassedFilterCount = 0;

                        if( currentDirectory->open_directory_sorted( currentDirectoryInView ) == false )
                        {
                            exitOperation = true;
                        }
                        else
                        {
                            currentDirectory->filter_directory(file_is_dirSearch, fileFilterTypes );
                            filesAndDirectoryPassedFilterCount = currentDirectory->get_count();
                            /*
                            for(iFile = 0; iFile < currentDirectory->get_count(); iFile++)
                            {
                                currentFile = currentDirectory->get_file(iFile);
                                if( currentFile!=nullptr)
                                {
                                    fileToClick = currentFile->get_name();
                                    if( currentFile->is_directory() )
                                    {
                                        filesAndDirectoryPassedFilterCount++;
                                    }
                                    else if( !file_is_dirSearch &&  sff_ex::file_passes_filter(currentFile->get_type(),fileFilterTypes) )
                                    {
                                        filesAndDirectoryPassedFilterCount++;
                                    }
                                }
                            }
                            */
                            gpe::error_log->report("<"+currentDirectoryInView+"> has ["+ stg_ex::int_to_string(filesAndDirectoryPassedFilterCount)+"/"+ stg_ex::int_to_string(currentDirectory->get_count() )+"] amount of files/directories...");
                            fileDirYScroll->fullRect.h = fileDirYScroll->contextRect.h =  filesAndDirectoryPassedFilterCount;
                        }
                    }
                    else
                    {
                        nextDirectoryToView = "";
                    }
                }

                ///
                //Start the frame timer
                gpe::game_runtime->start_loop();

                widget_box.x = 0;
                widget_box.y = 0;
                if( gpe::screen_width > 1024)
                {
                    widget_box.w = gpe::screen_width*2/3;
                }
                else
                {
                    widget_box.w = gpe::screen_width-128;
                }

                //if( gpe::screen_height >= 600)
                {
                    widget_box.h = gpe::screen_height*4/5;
                }/*
                else
                {
                    widget_box.h = gpe::screen_height;
                }*/

                fileBrowserBox.w = widget_box.w-16-fileBrowserBox.x;
                fileBrowserBox.h = widget_box.h - 192;

                filePreviewBox.x = widget_box.x+widget_box.w;
                filePreviewBox.y = fileBrowserBox.y;
                filePreviewBox.w = gpe::screen_width-filePreviewBox.x-(padding_default*2);
                filePreviewBox.h = fileBrowserBox.h;

                fileShortcutTab.h = widget_box.h - fileShortcutTab.y;
                fileBrowserModeBar->process_self();
                if( fileBrowserModeBar->is_clicked() || pastFileBrowserMode!=fileBrowserModeBar->get_tab_pos() )
                {
                    iDirectoryXPos = iFileXPos = 0;
                    pastFileBrowserMode=fileBrowserModeBar->get_tab_pos();
                }

                if( enterMyComputerMode ==true)
                {
                    maxFilesInRows = 1;
                    colTextWidth = fileBrowserBox.w-padding_default-default_thumbnal_size;
                }
                else
                {
                    if( fileBrowserBox.w >TEXTBOX_FONT_SIZE_WIDTH*60 )
                    {
                        maxFilesInRows = fileBrowserBox.w / (TEXTBOX_FONT_SIZE_WIDTH*60);
                        colTextWidth = (fileBrowserBox.w-maxFilesInColumn*(padding_default+default_icon_width) )/maxFilesInRows;
                    }
                    else
                    {
                        maxFilesInRows = 1;
                        colTextWidth = fileBrowserBox.w-padding_default;
                    }
                }

                if( enterMyComputerMode ==true)
                {
                    maxFilesInColumn = fileBrowserBox.h / (default_thumbnal_size+padding_default);
                    maxFilesInRows = 1;
                    currFileRect.w =colTextWidth = fileBrowserBox.w-32;
                    currFileRect.h = default_thumbnal_size;
                }
                else
                {
                    maxCharsToView = (fileBrowserBox.w/TEXTBOX_FONT_SIZE_WIDTH)-20;
                    fileRowHeight = default_icon_width;
                    if( fileBrowserModeBar->get_tab_pos()==0 )
                    {
                        currFileRect.w = colTextWidth = (default_thumbnal_size+padding_default*4);
                        maxFilesInRows = fileBrowserBox.w/currFileRect.w;
                        currFileRect.h = fileRowHeight = default_thumbnal_size+TEXTBOX_FONT_SIZE_HEIGHT*3+padding_default;
                        maxFilesInColumn = std::ceil( (float)fileBrowserBox.h/(float)currFileRect.h );
                        maxContentInView = (maxFilesInColumn)*maxFilesInRows;
                    }
                    else if( fileBrowserModeBar->get_tab_pos()==1 )
                    {
                        currFileRect.w = colTextWidth = 40*TEXTBOX_FONT_SIZE_WIDTH+default_icon_width;
                        maxFilesInRows = fileBrowserBox.w/currFileRect.w;
                        currFileRect.h = default_icon_width+padding_default;
                        maxFilesInColumn = fileBrowserBox.h/currFileRect.h;
                        maxContentInView = (maxFilesInColumn)*maxFilesInRows;
                    }
                    else if( fileBrowserModeBar->get_tab_pos()==2 )
                    {
                        currFileRect.w =colTextWidth = fileBrowserBox.w-32;
                        currFileRect.h = default_icon_width+padding_default;
                        maxFilesInRows = 1;
                        maxFilesInColumn = fileBrowserBox.h/currFileRect.h;
                        maxContentInView = (maxFilesInColumn)*maxFilesInRows;
                    }
                }


                colTextCharacters = colTextWidth/font_sizeW;
                shorterCurrentDirectoryInView = currentDirectoryInView;

                if( (int)currentDirectoryInView.size() > maxCharsToView)
                {
                    shorterCurrentDirectoryInView = "/..."+currentDirectoryInView.substr(currentDirectoryInView.size()-maxCharsToView);
                }

                refresh_button->set_coords(padding_default,widget_box.y+32);
                newstringBox->set_coords( fileBrowserBox.x+padding_default,fileBrowserBox.y+fileBrowserBox.h+padding_default);
                newstringBox->set_width(fileBrowserBox.w-128 );

                fileTypeDropDown->set_coords( fileBrowserBox.x+padding_default,newstringBox->get_y2()+padding_default);
                fileTypeDropDown->set_width(fileBrowserBox.w-128 );


                directoryViewBox->set_coords( widget_box.x+padding_default+32,widget_box.y+32);
                directoryViewBox->set_width(widget_box.w - 192);

                fileBrowserModeBar->set_coords(directoryViewBox->get_x2(),directoryViewBox->get_ypos() );
                fileBrowserModeBar->set_width(128);

                back_button->set_width(128);
                mycomputer_button->set_width(128);
                home_button->set_width(128);
                desktop_button->set_width(128);
                documents_button->set_width(128);
                downloads_button->set_width(128);
                pictures_button->set_width(128);
                music_button->set_width(128);
                video_buttons->set_width(128);
                appDirectory_button->set_width(128);
                examplesDirectory_button->set_width(128);
                //tutorialsDirectory_button->set_width(128);

                yes_button->set_coords(  widget_box.x+widget_box.w-buttonsWidth,widget_box.h-48);
                cancel_button->set_coords( widget_box.x+yes_button->get_x2()+padding_default,widget_box.h-48);

                if( shortCutGList!=nullptr)
                {
                    shortCutGList->clear_list();
                    shortCutGList->barXMargin = padding_default;
                    shortCutGList->barYMargin = 0;
                    shortCutGList->barXPadding = 0;
                    shortCutGList->barYPadding = padding_default;

                    shortCutGList->set_coords(fileShortcutTab.x,fileShortcutTab.y);
                    shortCutGList->set_width(fileShortcutTab.w);
                    shortCutGList->set_height(fileShortcutTab.h);
                    /*
                    Commmented out temporarily in 1.13
                    //if( gpe::system_found_os== gpe::system_os_linux)
                    {
                        if( main_editor_settings!=nullptr)
                        {
                            shortCutGList->add_gui_element(main_editor_settings->showHiddenFilesInBrowser,true);
                        }
                    }*/
                    shortCutGList->add_gui_element(back_button,true);
                    shortCutGList->add_gui_element(mycomputer_button,true);
                    shortCutGList->add_gui_element(home_button,true);
                    shortCutGList->add_gui_element(desktop_button,true);
                    shortCutGList->add_gui_element(documents_button,true);
                    shortCutGList->add_gui_element(downloads_button,true);
                    shortCutGList->add_gui_element(pictures_button,true);
                    shortCutGList->add_gui_element(music_button,true);
                    shortCutGList->add_gui_element(video_buttons,true);
                    shortCutGList->add_gui_element(appDirectory_button,true);
                    shortCutGList->add_gui_element(examplesDirectory_button,true);
                    //shortCutGList->add_gui_element(tutorialsDirectory_button,true);
                    //shortCutGList->add_gui_element(music_button);
                    shortCutGList->process_self();
                }

                refresh_button->process_self();
                yes_button->process_self();

                cancel_button->process_self();

                newstringBox->process_self();
                if( !file_is_dirSearch)
                {
                    fileTypeDropDown->process_self();
                }
                if( fileTypeDropDown->just_activated() )
                {
                    nextDirectoryToView = currentDirectoryInView;
                }
                directoryViewBox->process_self();

                if( directoryViewBox->is_inuse() ==false)
                {
                    if( directoryViewBox->get_string() !=currentDirectoryInView )
                    {
                        if( sff_ex::path_exists(directoryViewBox->get_string() ) )
                        {
                            nextDirectoryToView  = directoryViewBox->get_string();
                        }
                    }
                }

                if( back_button->is_clicked() )
                {
                    nextDirectoryToView = stg_ex::get_path_from_file(currentDirectoryInView);
                    if( nextDirectoryToView.size()<=1 )
                    {
                        nextDirectoryToView = "";
                    }
                }

                if( newstringBox->is_inuse()==false && directoryViewBox->is_inuse()==false )
                {
                    if( gpe::input->check_kb_released(kb_backspace) )
                    {
                        nextDirectoryToView = stg_ex::get_path_from_file(currentDirectoryInView);
                        if( nextDirectoryToView.size()<=1 )
                        {
                            nextDirectoryToView = "";
                        }
                    }
                }
                if( home_button->is_clicked() )
                {
                    nextDirectoryToView = userHomePath;
                }

                if( desktop_button->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Desktop";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/desktop";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyDesktop";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Desktop";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( downloads_button->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Downloads";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/downloads";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyDownloads";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Downloads";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( pictures_button->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Pictures";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/pictures";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyPictures";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Pictures";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( music_button->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Music";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/music";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyMusic";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Music";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( video_buttons->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Videos";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/videos";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyVideos";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Videos";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( enterMyComputerMode==false && refresh_button->is_clicked() )
                {
                    nextDirectoryToView = currentDirectoryInView;
                }

                if( mycomputer_button->is_clicked() || ( enterMyComputerMode==true && refresh_button->is_clicked()) )
                {
                    mountedDriversList.clear();
                    nextDirectoryToView = "/proc/mounts";
                    if( sff_ex::file_exists( nextDirectoryToView) && gpe::system_found_os==gpe::system_os_linux )
                    {
                        enterMyComputerMode = true;
                    }
                    else if( gpe::system_found_os == gpe::system_os_windows)
                    {
                        enterMyComputerMode = true;
    #ifdef _WIN32

                        DWORD dwSize = MAX_PATH;
                        char szLogicalDrives[MAX_PATH] = {0};
                        DWORD dwResult = GetLogicalDriveStrings(dwSize,szLogicalDrives);

                        if (dwResult > 0 && dwResult <= MAX_PATH)
                        {
                            char* szSingleDrive = szLogicalDrives;
                            while(*szSingleDrive)
                            {
                                mountedDriversList.push_back(szSingleDrive);

                                // get the next drive
                                szSingleDrive += strlen(szSingleDrive) + 1;
                            }
                        }
    #endif
                    }
                    nextDirectoryToView = "";
                    directoryViewBox->set_string("My Computer");
                }

                if( documents_button->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Documents";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/documents";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyDocuments";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Documents";
                            }
                        }
                    }
                }

                if( appDirectory_button->is_clicked() )
                {
                    nextDirectoryToView = stg_ex::get_path_from_file( gpe::app_directory_name);
                }

                if( examplesDirectory_button->is_clicked() )
                {
                    nextDirectoryToView = gpe::app_directory_name+"examples";
                }

                /*if( tutorialsDirectory_button->is_clicked() )
                {
                    nextDirectoryToView = gpe::app_directory_name+"tutorials";
                }*/

                if( gpe::input->check_kb_released(kb_esc) || cancel_button->is_clicked() )
                {
                    exitOperation = true;
                    returnVal = "";
                }
                else if( gpe::input->check_kb_released(kb_enter) || yes_button->is_clicked() )
                {
                    if( newstringBox!=nullptr)
                    {
                        soughtReturnFilename = currentDirectoryInView+"/"+newstringBox->get_string();
                        if( file_is_dirSearch )
                        {
                            if( sff_ex::path_exists(soughtReturnFilename) )
                            {
                                exitOperation = true;
                                returnFile = soughtReturnFilename;
                            }
                            else
                            {
                                soughtReturnFilename = currentDirectoryInView;
                                if( sff_ex::path_exists(soughtReturnFilename) )
                                {
                                    exitOperation = true;
                                    returnFile = soughtReturnFilename;
                                }
                            }
                        }
                        else if( isSaving && (int)newstringBox->get_string().size()>0 )
                        {
                            if( sff_ex::file_exists(soughtReturnFilename) )
                            {
                                if( display_prompt_message("File already exists","Do you wish to overwrite this file?")==display_query_yes )
                                {
                                    exitOperation = true;
                                    returnFile = soughtReturnFilename;
                                }
                            }
                            else
                            {
                                exitOperation = true;
                                returnFile = soughtReturnFilename;
                            }
                        }
                        else if( sff_ex::file_exists(soughtReturnFilename) )
                        {
                            exitOperation = true;
                            returnFile = soughtReturnFilename;
                        }
                    }
                }

                if( enterMyComputerMode)
                {
                    if( (int)mountedDriversList.size() > maxContentInView)
                    {

                        fileDirYScroll->set_coords( fileBrowserBox.x+fileBrowserBox.w-fileDirYScroll->get_width(), fileBrowserBox.y );
                        fileDirYScroll->set_height( fileBrowserBox.h );

                        fileDirYScroll->fullRect.x = 0;
                        fileDirYScroll->fullRect.y = 0;
                        fileDirYScroll->fullRect.w = 0;
                        fileDirYScroll->fullRect.h = (int)mountedDriversList.size();
                        //if( directoryViewBox->is_inuse()==false && newstringBox->is_inuse()==false && shortCutGList->hasScrollControl==false)
                        //{
                        if( gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&fileBrowserBox ) )
                        {
                            if( gpe::input->mouse_scrolling_up || gpe::input->check_kb_down(kb_page_up))
                            {
                                iDirectoryXPos-=maxContentInView/2;
                            }
                            else if( gpe::input->mouse_scrolling_down || gpe::input->check_kb_down(kb_page_down) )
                            {
                                iDirectoryXPos+=maxContentInView/2;
                            }
                        }
                        fileDirYScroll->contextRect.x = 0;
                        fileDirYScroll->contextRect.y = iDirectoryXPos;
                        fileDirYScroll->contextRect.w = 0;
                        fileDirYScroll->contextRect.h = maxContentInView;

                        fileDirYScroll->process_self();
                        if( fileDirYScroll->has_moved() )
                        {
                            iDirectoryXPos = ceil( ( (float)mountedDriversList.size() ) * ( (float)fileDirYScroll->scrollYPos/(float)fileDirYScroll->get_height() ) );
                        }
                    }
                    else
                    {
                        fileDirYScroll->contextRect.x =  fileDirYScroll->fullRect.x = 0;
                        fileDirYScroll->contextRect.y =  fileDirYScroll->fullRect.y = 0;
                        fileDirYScroll->contextRect.w =  fileDirYScroll->fullRect.w = 32;
                        fileDirYScroll->contextRect.h =  fileDirYScroll->fullRect.h = (int)mountedDriversList.size();
                        fileDirYScroll->process_self();
                    }
                }
                else
                {
                    if( filesAndDirectoryPassedFilterCount > maxContentInView)
                    {
                        fileDirYScroll->set_coords( fileBrowserBox.x+fileBrowserBox.w-fileDirYScroll->get_width(), fileBrowserBox.y );
                        fileDirYScroll->set_height( fileBrowserBox.h );

                        fileDirYScroll->fullRect.x = 0;
                        fileDirYScroll->fullRect.y = 0;
                        fileDirYScroll->fullRect.w = 0;
                        fileDirYScroll->fullRect.h = filesAndDirectoryPassedFilterCount;

                        /*if( maxContentInView >8)
                        {

                        }
                        scrollAmount = */
                        //if( directoryViewBox->is_inuse()==false && newstringBox->is_inuse()==false && shortCutGList->hasScrollControl==false)
                        if( gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&fileBrowserBox ) )
                        {

                            if( gpe::input->mouse_scrolling_up)
                            {
                                iFileXPos-=maxFilesInRows;
                            }
                            else if( gpe::input->mouse_scrolling_down)
                            {
                                iFileXPos+=maxFilesInRows;
                            }
                        }
                        if( directoryViewBox->is_inuse()==false && newstringBox->is_inuse()==false && shortCutGList->hasScrollControl==false )
                        {
                            if( gpe::input->check_kb_down(kb_up) )
                            {
                                iFileXPos-=maxFilesInRows;
                            }
                            else if( gpe::input->check_kb_down(kb_down) )
                            {
                                iFileXPos+=maxFilesInRows;
                            }
                        }

                        fileDirYScroll->contextRect.x = 0;
                        fileDirYScroll->contextRect.y = iFileXPos;
                        fileDirYScroll->contextRect.w = 0;
                        fileDirYScroll->contextRect.h = maxContentInView;
                        fileDirYScroll->process_self();

                        if( fileDirYScroll->has_moved() )
                        {
                            iFileXPos = ceil( ( (float)filesAndDirectoryPassedFilterCount ) * ( (float)fileDirYScroll->scrollYPos/(float)fileDirYScroll->get_height() ) );
                        }
                    }
                    else
                    {
                        fileDirYScroll->contextRect.x =  fileDirYScroll->fullRect.x = 0;
                        fileDirYScroll->contextRect.y =  fileDirYScroll->fullRect.y = 0;
                        fileDirYScroll->contextRect.w =  fileDirYScroll->fullRect.w = 32;
                        fileDirYScroll->contextRect.h =  fileDirYScroll->fullRect.h = filesAndDirectoryPassedFilterCount;
                        fileDirYScroll->process_self();
                    }
                }


                if( !gpe::window_controller_main->is_resized()  )
                {
                    gpe::renderer_main->clear_renderer( gpe::window_controller_main->is_minimized() );
                    //if( !gpe::input->window_input_received )
                    {
                        main_overlay_system->render_frozen_screenshot( );
                        //state_current->render();
                        //gpe::renderer_main->update_renderer();
                    }
                    //Update screen
                    gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->program_color_header,false);

                    gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,pawgui::theme_main->popup_box_highlight_color,false);

                    gpe::gfs->render_text( widget_box.x+widget_box.w/2,widget_box.y+16,prompt,pawgui::theme_main->popup_box_highlight_font_color,gpe::font_default,gpe::fa_center,gpe::fa_middle);
                    if( enterMyComputerMode)
                    {
                        //render_text( widget_box.x+32,widget_box.y+48,"My Computer",pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_middle);
                    }
                    else
                    {
                        //render_text( widget_box.x+32,widget_box.y+48,"Viewing "+shorterCurrentDirectoryInView,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_middle);
                    }

                    gpe::gcanvas->render_rect( &fileShortcutTab,pawgui::theme_main->text_box_color,false);
                    gpe::gcanvas->render_rect( &fileShortcutTab,pawgui::theme_main->text_box_highlight_color,true);

                    gpe::gcanvas->render_rect( &fileBrowserBox,pawgui::theme_main->text_box_color,false);
                    gpe::gcanvas->render_rect( &fileBrowserBox,pawgui::theme_main->text_box_highlight_color,true);

                    iContentCol = 0;
                    jContentRow = 0;
                    gpe::renderer_main->set_viewpoint( &fileBrowserBox);
                    mouseIsInBrowserBox = gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&fileBrowserBox);

                    if( enterMyComputerMode==true)
                    {
                        //display my computer directory
                        for (iDirectory = iDirectoryXPos; iDirectory < (int)mountedDriversList.size() && iDirectory < iDirectoryXPos+maxContentInView; iDirectory++)
                        {
                            currFileRect.x = fileBrowserBox.x+padding_default;
                            currFileRect.y = fileBrowserBox.y+jContentRow*(default_thumbnal_size+padding_default);

                            if( mouseIsInBrowserBox && gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&currFileRect) )
                            {
                                fileHoverNumber = iDirectory;
                                if( gpe::input->check_mouse_double_clicked( mb_left) )
                                {
                                    nextDirectoryToView = mountedDriversList[iDirectory];
                                    fileHoverNumber = -1;
                                    fileselectedNumber = -1;
                                }
                                else if( gpe::input->check_mouse_released( mb_left))
                                {
                                    fileselectedNumber = iDirectory;
                                    fileHoverNumber = -1;
                                }
                            }

                            currFileRect.x = padding_default;
                            currFileRect.y = jContentRow*(default_thumbnal_size+padding_default);

                            if( fileselectedNumber ==iDirectory)
                            {
                                gpe::gcanvas->render_rect( &currFileRect,pawgui::theme_main->main_box_font_highlight_color,false);
                            }
                            else if( fileHoverNumber ==iDirectory)
                            {
                                gpe::gcanvas->render_rect( &currFileRect,pawgui::theme_main->main_border_color,false);
                            }
                            if( textRepHarddrive!=nullptr )
                            {
                                textRepHarddrive->render_tex_resized( currFileRect.x,currFileRect.y,default_thumbnal_size,default_thumbnal_size,nullptr, pawgui::theme_main->folder_color );
                            }
                            gpe::gfs->render_text( currFileRect.x+default_thumbnal_size+padding_default,currFileRect.y+default_thumbnal_size/2,mountedDriversList[iDirectory],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_left,gpe::fa_middle);

                            jContentRow += 1;
                        }
                    }
                    else
                    {
                        filesProcessedCount = 0;
                        for(iFile = iFileXPos; filesProcessedCount < maxContentInView && iFile < currentDirectory->get_count(); iFile++)
                        {
                            currentFile = currentDirectory->get_file(iFile);
                            if( currentFile!=nullptr)
                            {
                                fileToClick = currentFile->get_name();
                                if( currentFile->is_directory() )
                                {
                                    fileMatchesFilter = true;
                                }
                                else if( !file_is_dirSearch )
                                {
                                    currentFileExt = currentFile->get_type();
                                    fileMatchesFilter = gpe::file_passes_filter(currentFileExt,fileFilterTypes);
                                }
                                else
                                {
                                    fileMatchesFilter = false;
                                }
                                if( fileMatchesFilter)
                                {
                                    currFileRect.x = fileBrowserBox.x+iContentCol*(currFileRect.w);
                                    currFileRect.y = fileBrowserBox.y+jContentRow*(currFileRect.h);
                                    if( mouseIsInBrowserBox && gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&currFileRect) )
                                    {
                                        fileHoverNumber = iFile;
                                        if( gpe::input->check_mouse_double_clicked( mb_left) )
                                        {
                                            if( currentFile->is_directory() )
                                            {
                                                nextDirectoryToView = currentDirectoryInView+"/"+fileToClick;
                                            }
                                            else
                                            {
                                                soughtReturnFilename = currentDirectoryInView+"/"+fileToClick;
                                                fileMatchesFilter = gpe::file_passes_filter(stg_ex::get_file_ext(fileToClick),fileFilterTypes);
                                                if( fileMatchesFilter)
                                                {
                                                    if( isSaving)
                                                    {
                                                        if( sff_ex::file_exists(soughtReturnFilename) )
                                                        {
                                                            if( display_prompt_message("File already exists","Do you wish to overwrite this file?")==display_query_yes )
                                                            {
                                                                exitOperation = true;
                                                                returnFile = soughtReturnFilename;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            exitOperation = true;
                                                            returnFile = soughtReturnFilename;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if( sff_ex::file_exists(soughtReturnFilename) )
                                                        {
                                                            exitOperation = true;
                                                            returnFile = soughtReturnFilename;
                                                        }
                                                    }
                                                }
                                            }
                                            fileHoverNumber = -1;
                                            fileselectedNumber = -1;
                                        }
                                        else if( gpe::input->check_mouse_released( mb_left))
                                        {
                                            fileselectedNumber = iFile;
                                            fileHoverNumber = -1;
                                            if( !currentFile->is_directory() )
                                            {
                                                newstringBox->set_string(fileToClick);
                                                previewedImageTexture = rsm_gui->texture_add_filename( currentDirectoryInView+"/"+fileToClick );
                                                if( previewedImageTexture!=nullptr )
                                                {
                                                    if( fileMatchesFilter)
                                                    {
                                                        if( currentFileExt=="bmp" || currentFileExt=="gif" || currentFileExt=="jpg" || currentFileExt=="jpeg"  || currentFileExt=="png" )
                                                        {
                                                            if( previewedImageTexture->get_width() < 1 || previewedImageTexture->get_height() < 1 || previewedImageTexture->get_width() > 4096 || previewedImageTexture->get_height() > 4096)
                                                            {
                                                                    //previewedImageTexture->change_texture(nullptr);
                                                            }
                                                        }
                                                        else
                                                        {
                                                            //previewedImageTexture->change_texture(nullptr);
                                                        }
                                                    }
                                                }
                                                else if(previewedImageTexture!=nullptr)
                                                {
                                                    //previewedImageTexture->change_texture(nullptr);
                                                }
                                            }
                                            else if( file_is_dirSearch)
                                            {
                                                newstringBox->set_string(fileToClick);
                                            }
                                        }
                                    }
                                    fileRenderName[0] = "";
                                    fileRenderName[1] = "";
                                    fileRenderName[2] = "";
                                    //Renders file
                                    currFileRect.x = iContentCol*(currFileRect.w);
                                    currFileRect.y = jContentRow*(currFileRect.h);
                                    if( fileselectedNumber ==iFile)
                                    {
                                        gpe::gcanvas->render_rect( &currFileRect,pawgui::theme_main->button_box_selected_color,false);
                                    }
                                    else if( fileHoverNumber ==iFile)
                                    {
                                        gpe::gcanvas->render_rect( &currFileRect,pawgui::theme_main->button_box_highlight_color,false);
                                    }
                                    currentFIleTexture = nullptr;
                                    if( currentFile->is_directory() )
                                    {
                                        fileSubImageToDraw = 1;
                                        //mainFilesanimation
                                        if( fileselectedNumber ==iFile)
                                        {
                                            fileIconColor = pawgui::theme_main->folder_color_highlight;
                                        }
                                        else
                                        {
                                            fileIconColor = pawgui::theme_main->folder_color;
                                        }
                                        currentFIleTexture = textRepFolder;
                                    }
                                    else
                                    {
                                        fileToClick = currentFile->get_name();
                                        currentFileExt =currentFile->get_type();
                                        fileMatchesFilter = gpe::file_passes_filter(currentFileExt,fileFilterTypes);

                                        fileSubImageToDraw = 0;

                                        if( currentFileExt=="txt")
                                        {
                                            currentFIleTexture = textRepText;
                                        }
                                        else if( currentFileExt=="eot" || currentFileExt=="ttf" || currentFileExt=="woff")
                                        {
                                            currentFIleTexture = textRepFont;
                                        }
                                        else if( currentFileExt=="bmp" || currentFileExt=="gif" || currentFileExt=="jpg" || currentFileExt=="jpeg"  || currentFileExt=="png" )
                                        {
                                            currentFIleTexture = textRepImage;
                                        }
                                        else if( currentFileExt=="aac" || currentFileExt=="mp3" || currentFileExt=="ogg")
                                        {
                                            currentFIleTexture = textRepAudio;
                                        }
                                        else if( currentFileExt=="avi" || currentFileExt=="mp4" || currentFileExt=="webm")
                                        {
                                            currentFIleTexture = textRepVideo;
                                        }
                                        else if( currentFileExt=="c" || currentFileExt=="css" || currentFileExt=="h" || currentFileExt=="html" || currentFileExt=="js"  || currentFileExt=="php")
                                        {
                                            currentFIleTexture = textRepCode;
                                        }
                                        else if( currentFileExt=="gppf")
                                        {
                                            currentFIleTexture = textRepGPE;
                                        }
                                        else if( currentFileExt=="exe")
                                        {
                                            currentFIleTexture = textRepExe;
                                        }
                                        else
                                        {
                                            currentFIleTexture = textRepOther;
                                        }

                                        if( fileselectedNumber ==iFile)
                                        {
                                            fileIconColor = pawgui::theme_main->main_box_font_highlight_color;
                                        }
                                        else
                                        {
                                            fileIconColor  = pawgui::theme_main->main_box_font_color;
                                        }
                                    }

                                    fileRenderName[0] = "";
                                    fileRenderName[1] = "";
                                    fileRenderName[2] = "";
                                    if( (int)fileToClick.size() >=colTextCharacters)
                                    {
                                        fileRenderName[0] = stg_ex::get_substring(fileToClick,0,colTextCharacters);
                                        fileRenderName[1] = stg_ex::get_substring(fileToClick,colTextCharacters,colTextCharacters);

                                        if( (int)fileToClick.size() >colTextCharacters*3)
                                        {
                                            fileRenderName[2] = stg_ex::get_substring(fileToClick,colTextCharacters*2,colTextCharacters-3)+"...";
                                        }
                                        else
                                        {
                                            fileRenderName[2] = stg_ex::get_substring(fileToClick,colTextCharacters*2,colTextCharacters);
                                        }
                                        fileToClick = fileRenderName[2];
                                    }
                                    else
                                    {
                                        fileRenderName[0] = fileToClick;
                                    }
                                    //Renders Thumbnails
                                    if( fileBrowserModeBar->get_tab_pos()==0 )
                                    {
                                        if( currentFile->thumbnail_texture!=nullptr )
                                        {
                                            currentFile->thumbnail_texture->render_tex_resized(
                                                                    currFileRect.x+(currFileRect.w-default_thumbnal_size)/2,currFileRect.y,default_thumbnal_size,default_thumbnal_size);
                                        }
                                        else if( currentFIleTexture!=nullptr)
                                        {
                                            currentFIleTexture->render_tex_resized( currFileRect.x+(currFileRect.w-default_thumbnal_size)/2,currFileRect.y,default_thumbnal_size,default_thumbnal_size,nullptr, fileIconColor );
                                        }
                                        gpe::gfs->render_text( currFileRect.x+currFileRect.w/2,currFileRect.y+default_thumbnal_size,fileRenderName[0],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_center,gpe::fa_top);
                                        gpe::gfs->render_text( currFileRect.x+currFileRect.w/2,currFileRect.y+default_thumbnal_size+TEXTBOX_FONT_SIZE_HEIGHT,fileRenderName[1],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_center,gpe::fa_top);
                                        gpe::gfs->render_text( currFileRect.x+currFileRect.w/2,currFileRect.y+default_thumbnal_size+TEXTBOX_FONT_SIZE_HEIGHT*2,fileRenderName[2],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_center,gpe::fa_top);
                                    }
                                    else
                                    {
                                        //Renders listed icon
                                        if( currentFIleTexture!=nullptr)
                                        {
                                            currentFIleTexture->render_tex_resized(
                                                                    currFileRect.x,currFileRect.y+(currFileRect.h-default_icon_width)/2,
                                                                    default_icon_width,default_icon_width,nullptr, fileIconColor );
                                        }
                                        gpe::gfs->render_text( currFileRect.x+default_icon_width,currFileRect.y+currFileRect.h/2,fileRenderName[0],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_left,gpe::fa_middle );
                                    }

                                    filesProcessedCount++;
                                    iContentCol++;
                                    if( iContentCol >= maxFilesInRows || fileBrowserModeBar->get_tab_pos()==2 )
                                    {
                                        iContentCol = 0;
                                        jContentRow += 1;
                                    }
                                }
                            }
                            //tinydir_next(&dir);
                        }
                    }
                    gpe::renderer_main->reset_viewpoint();
                    if( filesAndDirectoryPassedFilterCount > maxContentInView)
                    {
                        fileDirYScroll->render_self( );
                    }
                    /*
                    appDirectory_button->render_self( );
                    back_button->render_self( );
                    desktop_button->render_self( );
                    documents_button->render_self( );
                    downloads_button->render_self( );
                    home_button->render_self( );
                    music_button->render_self( );
                    mycomputer_button->render_self( );*/
                    shortCutGList->render_self( );

                    refresh_button->render_self( );

                    yes_button->render_self( );
                    cancel_button->render_self( );
                    directoryViewBox->render_self( nullptr, nullptr);
                    fileBrowserModeBar->render_self( nullptr, nullptr);
                    newstringBox->render_self( nullptr, nullptr);
                    if( !file_is_dirSearch)
                    {
                        fileTypeDropDown->render_self( nullptr, nullptr);
                    }

                    if( previewedImageTexture!=nullptr && enterMyComputerMode==false && filePreviewBox.w!=0 && filePreviewBox.h!=0)
                    {
                        if( filePreviewBox.w > previewedImageTexture->get_width()&& filePreviewBox.h> previewedImageTexture->get_height() )
                        {
                            previewedImageTexture->render_tex( filePreviewBox.x,filePreviewBox.y);
                        }
                        else
                        {
                            imagePreview_scaleSize = std::max((float)previewedImageTexture->get_width()/ (float)(filePreviewBox.w), (float)previewedImageTexture->get_height()/(float)(filePreviewBox.h) );
                            if( imagePreview_scaleSize > 1)
                            {
                                previewedImageTexture->render_tex_resized( filePreviewBox.x,filePreviewBox.y,(float)previewedImageTexture->get_width()/imagePreview_scaleSize,(float)previewedImageTexture->get_height()/imagePreview_scaleSize);
                            }
                            else
                            {
                                previewedImageTexture->render_tex_resized( filePreviewBox.x,filePreviewBox.y,previewedImageTexture->get_width()*imagePreview_scaleSize,previewedImageTexture->get_height()*imagePreview_scaleSize);
                            }
                        }
                    }
                    gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->main_border_color,true);
                    gpe::game_runtime->end_loop(  );
                }
            }

            //Delete buttons and such
            //tinydir_close(&dir);
            if( currentDirectory!=nullptr)
            {
                delete currentDirectory;
                currentDirectory = nullptr;
            }

            if( refresh_button!=nullptr)
            {
                delete refresh_button;
                refresh_button = nullptr;
            }
            if( appDirectory_button!=nullptr)
            {
                delete appDirectory_button;
                appDirectory_button = nullptr;
            }
            if( examplesDirectory_button!=nullptr)
            {
                delete examplesDirectory_button;
                examplesDirectory_button = nullptr;
            }
            /*if( tutorialsDirectory_button!=nullptr)
            {
                delete tutorialsDirectory_button;
                tutorialsDirectory_button = nullptr;
            }*/
            if( back_button!=nullptr)
            {
                delete back_button;
                back_button = nullptr;
            }
            if( desktop_button!=nullptr)
            {
                delete desktop_button;
                desktop_button = nullptr;
            }
            if( documents_button!=nullptr)
            {
                delete documents_button;
                documents_button = nullptr;
            }
            if( downloads_button!=nullptr)
            {
                delete downloads_button;
                downloads_button = nullptr;
            }
            if( pictures_button!=nullptr)
            {
                delete pictures_button;
                pictures_button = nullptr;
            }
            if( music_button!=nullptr)
            {
                delete music_button;
                music_button = nullptr;
            }
            if( home_button!=nullptr)
            {
                delete home_button;
                home_button = nullptr;
            }
            if( mycomputer_button!=nullptr)
            {
                delete mycomputer_button;
                mycomputer_button = nullptr;
            }
            if( yes_button!=nullptr)
            {
                delete yes_button;
                yes_button = nullptr;
            }
            if( cancel_button!=nullptr)
            {
                delete cancel_button;
                cancel_button = nullptr;
            }
            if( newstringBox!=nullptr)
            {
                delete newstringBox;
                newstringBox = nullptr;
            }
            if( fileTypeDropDown!=nullptr)
            {
                delete fileTypeDropDown;
                fileTypeDropDown = nullptr;
            }
            if( directoryViewBox!=nullptr)
            {
                delete directoryViewBox;
                directoryViewBox = nullptr;
            }
            if( fileBrowserModeBar!=nullptr)
            {
                delete fileBrowserModeBar;
                fileBrowserModeBar = nullptr;
            }
            if( fileDirYScroll!=nullptr)
            {
                delete fileDirYScroll;
                fileDirYScroll = nullptr;
            }
            if( shortCutGList!=nullptr)
            {
                delete shortCutGList;
                shortCutGList = nullptr;
            }
            gpe::input->reset_all_input();
            previousDirectory = currentDirectoryInView;
            return returnFile;
        }
        gpe::input->reset_all_input();
        return returnFile;
    }
}

