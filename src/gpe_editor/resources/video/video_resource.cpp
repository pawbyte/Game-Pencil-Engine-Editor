/*
video_resource.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "video_resource.h"
#include "../../editor/gpe_editor_settings.h"

std::string SUPPORTED_VIDEO_EXT[SUPPORTED_VIDEO_FORMAT_COUNT];

videoResource::videoResource(pawgui::widget_resource_container * pFolder)
{
    projectParentFolder = pFolder;
    resourceFileName = " ";
    resource_name = " ";
    for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
    {
        videoFileName[i] = "";
    }
    videoId = -1;
    videoType = 0;
    videoEditorMainNote = new pawgui::widget_label_text ("Video playback in editor not supported.","Video playback in editor not supported.");

    videoGroupName = new pawgui::widget_input_text("","default=blank");
    videoGroupName->set_label("Video Group");

    defaultVolume = new pawgui::widget_input_number("100",true,0,100);
    defaultVolume->set_string("100");
    defaultVolume->set_label("Default Volume:");

    openExternalEditor_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Audio In External Editor");
    refreshResourceData_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes this resource's media files");
}

videoResource::~videoResource()
{
    if( videoEditorMainNote!=nullptr)
    {
        delete videoEditorMainNote;
        videoEditorMainNote = nullptr;
    }
    if( defaultVolume!=nullptr)
    {
        delete defaultVolume;
        defaultVolume = nullptr;
    }
    if( videoGroupName!=nullptr)
    {
        delete videoGroupName;
        videoGroupName = nullptr;
    }
    if( openExternalEditor_button!=nullptr)
    {
        delete openExternalEditor_button;
        openExternalEditor_button = nullptr;
    }
    if( refreshResourceData_button!=nullptr)
    {
        delete refreshResourceData_button;
        refreshResourceData_button = nullptr;
    }
}

bool videoResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void videoResource::compile_cpp()
{

}

bool videoResource::copy_video_source(std::string directory_output_name)
{
    std::string copyDestinationStr = "";
    bool copyErrorFound = false;
    for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT ; i++)
    {
        if((int)videoFileName[i].size() > 0)
        {
            copyDestinationStr = directory_output_name+"/"+ stg_ex::get_short_filename(videoFileName[i],true);
            gpe::error_log->report(videoFileName[i]+" attempted to copy to "+copyDestinationStr);
            if( sff_ex::file_copy(videoFileName[i],copyDestinationStr )==false)
            {
                copyErrorFound = true;
            }
        }
    }
    return copyErrorFound;
}

bool videoResource::include_local_files( std::string pBuildDir , int buildType )
{
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt", get_name() +"...");
    return true;
}

bool videoResource::is_build_ready()
{
    recentErrorMessage = "";
    return true;
}

void videoResource::load_video(std::string new_file_name)
{
    if( (int) new_file_name.size()>0 )
    {
        bool isvideoFile = true;
        std::string newFileExt = stg_ex::get_file_ext(new_file_name);

        if( newFileExt=="mp4")
        {
            videoFileName[mp4FileName] = new_file_name;
        }
        else if( newFileExt=="ogg")
        {
            videoFileName[oggVideoFileName] = new_file_name;
        }
        else if( newFileExt=="webm")
        {
            videoFileName[webmFileName] = new_file_name;
        }
        else
        {
            pawgui::display_user_alert("[Alert - "+resource_name+"]","Unsupported file type added.");
        }
        if( isvideoFile)
        {
            copy_video_source( stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/videos" );
        }
    }
}

void videoResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed == false  || sff_ex::file_exists(file_path) )
    {
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->update_submessages( "Processing Video",resource_name );
        }

        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/videos/";
        if( sff_ex::file_exists(file_path) )
        {
            newFileIn = file_path;
            soughtDir = stg_ex::get_path_from_file(newFileIn);
        }
        else
        {
            newFileIn = soughtDir + resource_name+".gpf";
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        gpe::error_log->report("Loading video - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() && gameResourceFileIn.is_open())
        {
            int equalPos = 0;
            std::string firstChar="";
            std::string key_string="";
            std::string valstring="";
            std::string subValstring="";
            std::string currLine="";
            std::string currLineToBeProcessed;
            float foundFileVersion = 0;
            std::string fFontFile = "";
            while ( gameResourceFileIn.good() )
            {
                getline (gameResourceFileIn,currLine); //gets the next line of the file
                currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);
                if( foundFileVersion <=0)
                {
                    //Empty Line skipping is only allowed at the top of the file
                    if(!currLineToBeProcessed.empty() )
                    {
                        //Comment skipping is only allowed at the top of the file
                        if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                key_string = currLineToBeProcessed.substr(0,equalPos);
                                valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( key_string=="Version")
                                {
                                    foundFileVersion = stg_ex::string_to_float(valstring);
                                }
                            }
                        }
                    }
                }
                else if( foundFileVersion <= 2)
                {
                    //Begin processing the file.
                    if(!currLineToBeProcessed.empty() )
                    {
                        equalPos=currLineToBeProcessed.find_first_of("=");
                        if(equalPos!=(int)std::string::npos)
                        {
                            //if the equalPos is present, then parse on through and carryon
                            key_string = currLineToBeProcessed.substr(0,equalPos);
                            valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                            if( key_string=="ResourceName")
                            {
                                renameBox->set_string(valstring);
                            }
                            else if( key_string=="videofile_location")
                            {
                                load_video( soughtDir+valstring );
                            }
                            else if( key_string=="VideoGroup")
                            {
                                videoGroupName->set_string(valstring);
                            }
                            else if( key_string=="DefaultVolume")
                            {
                                defaultVolume->set_number( stg_ex::string_to_int(valstring,100));
                            }
                            else
                            {
                                for(int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
                                {
                                    if( key_string=="videoFile["+SUPPORTED_VIDEO_EXT[i]+"]")
                                    {
                                        if( (int)valstring.size() > 3)
                                        {
                                            load_video( soughtDir+valstring );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    gpe::error_log->report("Invalid FoundFileVersion ="+ stg_ex::float_to_string(foundFileVersion)+".");
                }
            }

            if( main_gpe_splash_page != nullptr )
            {
                main_gpe_splash_page->increment_and_update( "Video resource processed",resource_name );
            }
        }
        else
        {
            if( main_gpe_splash_page != nullptr )
            {
                main_gpe_splash_page->increment_and_update( "Video resource processingg error occurred",resource_name );
            }
        }
    }
}

void videoResource::prerender_self()
{
    standardEditableGameResource::prerender_self();
}

void videoResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if(cam!=nullptr && view_space!=nullptr && panel_main_editor!=nullptr)
    {
        panel_main_editor->clear_panel();
        panel_main_editor->add_gui_element(videoEditorMainNote,true);
        panel_main_editor->add_gui_element(renameBox,true);

        panel_main_editor->add_gui_element(refreshResourceData_button,false );
        panel_main_editor->add_gui_element(loadResource_button,false );
        panel_main_editor->add_gui_element(openExternalEditor_button,true);

        panel_main_editor->add_gui_element(videoGroupName, true);
        panel_main_editor->add_gui_element(defaultVolume,true);
        panel_main_editor->add_gui_element(confirmResource_button,true);
        panel_main_editor->add_gui_element(cancelResource_button,true);
        //panel_main_editor->set_maxed_out_width();
        panel_main_editor->process_self(nullptr, nullptr);


        if( loadResource_button!=nullptr && loadResource_button->is_clicked() )
        {
            std::string new_file_name = pawgui::get_filename_open_from_popup("Load In an video File","Video",pawgui::main_settings->fileOpenVideoDir);
            load_video(new_file_name);
        }
        else if( confirmResource_button!=nullptr && confirmResource_button->is_clicked() )
        {
            save_resource();
        }
        else if( cancelResource_button->is_clicked() )
        {
            resourcePostProcessed = false;
            load_resource();
        }
        else if( openExternalEditor_button!=nullptr &&  openExternalEditor_button->is_clicked() )
        {
            bool hasFileToOpen = false;
            int ii = 0;
            for( ii = 0; ii < SUPPORTED_VIDEO_FORMAT_COUNT; ii++)
            {
                if( (int)videoFileName[ii].size() > 0)
                {
                    hasFileToOpen = true;
                    break;
                }
            }
            if( hasFileToOpen )
            {
                pawgui::context_menu_open(-1,-1,256);
                pawgui::main_context_menu->set_width(openExternalEditor_button->get_width() );
                for( ii = 0; ii < SUPPORTED_VIDEO_FORMAT_COUNT; ii++)
                {
                    if( (int)videoFileName[ii].size() > 0)
                    {
                        pawgui::main_context_menu->add_menu_option("Edit "+SUPPORTED_VIDEO_EXT[ii],ii);
                    }
                }
                int menuSelection = pawgui::context_menu_process();
                if( menuSelection >=0 && menuSelection < SUPPORTED_VIDEO_FORMAT_COUNT)
                {
                    std::string fileToEdit = videoFileName[menuSelection];

                    if( main_editor_settings!=nullptr && main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]!=nullptr)
                    {
                        gpe::external_open_program(main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->get_string(),fileToEdit, true );
                    }
                    else
                    {
                        gpe::external_open_url(fileToEdit);
                    }
                    /*
                    fileToEdit = "\"C:/Program Files (x86)/Audacity/audacity.exe\" \""+fileToEdit+"\"";
                    external_open_url(fileToEdit);*/
                    sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+fileToEdit+"]...");
                }
            }
        }
        else if( refreshResourceData_button!=nullptr &&  refreshResourceData_button->is_clicked() )
        {
            resourcePostProcessed = false;
            load_resource();
        }
    }
}

void videoResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( cam!=nullptr && view_space!=nullptr)
    {
        for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
        {
            if( videoFileName[i].size()> 0)
            {
                gpe::gfs->render_text( view_space->w-pawgui::padding_default*3,pawgui::padding_default+pawgui::default_line_height*i,SUPPORTED_VIDEO_EXT[i]+" is attatched",pawgui::theme_main->main_box_font_highlight_color,gpe::font_default,gpe::fa_right,gpe::fa_top);
            }
            else
            {
                gpe::gfs->render_text( view_space->w-pawgui::padding_default*3,pawgui::padding_default+pawgui::default_line_height*i,SUPPORTED_VIDEO_EXT[i]+" not attatched",pawgui::theme_main->main_box_font_color,gpe::font_default,gpe::fa_right,gpe::fa_top);
            }
        }
        gpe::gfs->render_text( view_space->w/2, view_space->h-32, "HTML5 Feature. Currently not supported in C++/Native runtime",pawgui::theme_main->main_box_font_color,gpe::font_default, gpe::fa_center, gpe::fa_bottom, 255);
    }
    //
}

void videoResource::save_resource(std::string file_path, int backupId)
{
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Saving Game video", resource_name );
    }

    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = stg_ex::get_path_from_file(file_path);
    if( sff_ex::path_exists(soughtDir) )
    {
        newFileOut = file_path;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/videos/";
        newFileOut = soughtDir + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() && newSaveDataFile.is_open() )
    {
        write_header_on_file(&newSaveDataFile);

        std::string resfile_location = "";
        std::string resFileCopySrc;
        std::string resFileCopyDest;

        for(int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
        {
            if( (int)videoFileName[i].size() > 3)
            {
                resfile_location = stg_ex::get_short_filename (videoFileName[i],true );
                newSaveDataFile << "videoFile["+SUPPORTED_VIDEO_EXT[i]+"]="+resfile_location+"\n";
                if( (int)resfile_location.size() > 0 && usingAltSaveSource )
                {
                    resFileCopySrc = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/video/"+resfile_location;
                    resFileCopyDest = soughtDir+resfile_location;
                    if( sff_ex::file_exists(resFileCopyDest) )
                    {
                        /*
                        if( pawgui::display_prompt_message("[WARNING]Video File Already exists?","Are you sure you will like to overwrite your ["+resfile_location+"] Video file? This action is irreversible!")==pawgui::display_query_yes)
                        {
                            sff_ex::file_copy(resFileCopySrc,resFileCopyDest);
                        }
                        */
                    }
                    else
                    {
                        sff_ex::file_copy(resFileCopySrc,resFileCopyDest);
                    }
                }
            }
        }

        if( defaultVolume!=nullptr)
        {
            newSaveDataFile << "DefaultVolume="+ stg_ex::int_to_string(defaultVolume->get_held_number() )+"\n";
        }

        if( videoGroupName!=nullptr)
        {
            newSaveDataFile << "VideoGroup="+videoGroupName->get_string()+"\n";

        }

        newSaveDataFile.close();
        if( !usingAltSaveSource)
        {
            isModified = false;
        }
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->increment_and_update( "Video resource saved",resource_name );
        }
        return;
    }

    main_editor_log->log_general_error("Unable to save file ["+newFileOut+"]");
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->increment_and_update( "Video resource unable to save",resource_name );
    }
}

bool videoResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "video=" << resource_name << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}
