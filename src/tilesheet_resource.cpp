/*
tilesheet_resource.cpp
This file is part of:
GAME PENCI ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "gpe_editor.h"

tilesheetResource::tilesheetResource(GPE_ResourceContainer * pFolder)
{
    projectParentFolder = pFolder;
    editorMode = 0;
    tilesheetInEditor = NULL;
    tsPreviewer = new tilesheetPreviewer();
    preloadCheckBox = new GPE_CheckBoxBasic("Preload tilesheet","Check to load tilesheet at game open",GENERAL_GPE_PADDING,256,true);
    isPreloaded = true;
    if(saveResourceButton!=NULL)
    {
        //saveResourceButton->disable_self();
    }
    int i = 0;
    for( i =0; i < TILESHEET_DATA_FIELD_COUNT; i++)
    {
        tsDataFields[i] = new GPE_TextInputNumber("0",true,0,2048);
        tsDataFields[i] ->set_label(tsDataLabels[i]);
    }
    transformResourceButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/magic.png","Transform IMG","Transform the Image",-1);
    tilesheetDimensionsStr = new GPE_Label_Text("0x0px");

    openExternalEditorButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/rocket.png","Use External Editor","Opens Tilesheet Image In External Editor");
    refreshResourceDataButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/refresh.png","Refresh","Refreshes the loaded tilesheet image");
    labelInfoMaxTextureSize = new GPE_Label_Text("Max Image Size: 4096 X 4096px","Max Image Size: 4096 X 4096px");
}

tilesheetResource::~tilesheetResource()
{
    if( labelInfoMaxTextureSize!=NULL)
    {
        delete labelInfoMaxTextureSize;
        labelInfoMaxTextureSize = NULL;
    }
    if( openExternalEditorButton!=NULL)
    {
        delete openExternalEditorButton;
        openExternalEditorButton = NULL;
    }
    if( refreshResourceDataButton!=NULL)
    {
        delete refreshResourceDataButton;
        refreshResourceDataButton = NULL;
    }
    if( tilesheetInEditor!=NULL)
    {
        delete tilesheetInEditor;
        tilesheetInEditor = NULL;
    }
    if( preloadCheckBox!=NULL)
    {
        delete preloadCheckBox;
        preloadCheckBox = NULL;
    }
    if( tilesheetDimensionsStr!=NULL)
    {
        delete tilesheetDimensionsStr;
        tilesheetDimensionsStr = NULL;
    }
    for( int i =TILESHEET_DATA_FIELD_COUNT-1; i >=0; i--)
    {
        if( tsDataFields[i]!=NULL)
        {
            delete tsDataFields[i];
            tsDataFields[i] = NULL;
        }
    }
    if( transformResourceButton!=NULL)
    {
        delete transformResourceButton;
        transformResourceButton = NULL;
    }
    if( tsPreviewer!=NULL)
    {
        delete tsPreviewer;
        tsPreviewer = NULL;
    }
}

bool tilesheetResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string html5TSName = get_name();
        if( tilesheetInEditor!=NULL)
        {
            *fileTarget << nestedTabsStr << "var " << html5TSName << " =  GPE.rsm.add_tilesheet(";
            *fileTarget << int_to_string (html5BuildGlobalId ) +",";
            *fileTarget << "'resources/tilesheets/"+getShortFileName (tilesheetInEditor->fileNameLocation,true ) +"',";
            *fileTarget << int_to_string (tilesheetInEditor->tsWidth ) +",";
            *fileTarget << int_to_string (tilesheetInEditor->tsHeight ) +",";
            *fileTarget << int_to_string (tilesheetInEditor->tsXOff ) +",";
            *fileTarget << int_to_string (tilesheetInEditor->tsYOff ) +",";
            *fileTarget << int_to_string (tilesheetInEditor->tsXPadding ) +",";
            *fileTarget << int_to_string (tilesheetInEditor->tsYPadding );
            *fileTarget << "); \n";
        }
        else
        {
            *fileTarget << nestedTabsStr << "var " << html5TSName << " = -1;\n";
        }
    }
    return false;
}

void tilesheetResource::load_image(std::string newFileName)
{
    if( file_is_image(newFileName) )
    {
        GPE_Texture * tempTexture =  new GPE_Texture();
        tempTexture->load_new_texture(MAIN_RENDERER, newFileName, -1, true);
        if( tempTexture->get_width()>0 )
        {
            if(tilesheetInEditor==NULL)
            {
                tilesheetInEditor = new GPE_Tilesheet();
            }
            if(tilesheetInEditor->tsImage!=NULL)
            {
               delete tilesheetInEditor->tsImage;
                tilesheetInEditor->tsImage = NULL;
            }
            if( tilesheetInEditor!=NULL)
            {
                tilesheetInEditor->tsImage = tempTexture;
                if(tilesheetInEditor->tsImage!=NULL)
                {
                    tilesheetInEditor->tsImage->load_new_texture(MAIN_RENDERER, newFileName, -1, false);
                    if( tilesheetInEditor->tsImage->get_width()>0 )
                    {
                        if( tilesheetInEditor->tsImage->get_width() <1 || tilesheetInEditor->tsImage->get_width()>4096 || tilesheetInEditor->tsImage->get_height() <1 || tilesheetInEditor->tsImage->get_height()>4096)
                        {
                            display_user_alert("Unable to load image","Editor Error: Unable to load ["+newFileName+"] please check file and make sure it is between 1x1 and 4096x4096 pixels and is a valid image");
                            if( tilesheetInEditor->tsImage!=NULL)
                            {
                                delete tilesheetInEditor->tsImage;
                                tilesheetInEditor->tsImage = NULL;
                            }
                        }
                        else
                        {
                            //successfully added new image.
                            tilesheetInEditor->tsRects.clear();
                            tilesheetInEditor->fileNameLocation = newFileName;
                            SDL_Rect newBox = {0,0,tilesheetInEditor->tsImage->get_width(),tilesheetInEditor->tsImage->get_height() };
                            tilesheetInEditor->tsRects.push_back(newBox);
                            tsDataFields[0]->set_string( int_to_string(tempTexture->get_width()) );
                            tsDataFields[1]->set_string( int_to_string(tempTexture->get_height()) );
                            tsDataFields[2]->set_string("0");
                            tsDataFields[3]->set_string("0");
                            tsDataFields[4]->set_string("0");
                            tsDataFields[5]->set_string("0");
                            tilesheetInEditor->tsImage->copy_image_source( fileToDir(parentProjectName)+"/gpe_project/resources/tilesheets");
                            process_data_fields();
                            tilesheetDimensionsStr->set_name(int_to_string(tilesheetInEditor->tsImage->get_width() )+" x "+int_to_string(tilesheetInEditor->tsImage->get_height() )+"px" );
                        }
                    }
                }
            }
        }
        else
        {
            display_user_alert("Tilesheet Edit Log","Unable to load image" );
        }
    }
    else
    {
        display_user_alert("Unable to load image","File type["+get_file_ext(newFileName)+"] not supported when loading ["+newFileName+"].");
    }
}

void tilesheetResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        displayMessageTitle = "Processing Tilesheet";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();

        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/tilesheets/";
        if( file_exists(alternatePath) )
        {
            newFileIn = alternatePath;
            soughtDir = get_path_from_file(newFileIn);
        }
        else
        {
            newFileIn = soughtDir + resourceName+".gpf";
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        //record_error("Loading Tilesheet - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
        {
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string section="";
                std::string cur_layer="";
                std::string data_format="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                std::string tsDataStr = "";
                float foundFileVersion = 0;
                int i = 0;
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = trim_left_inplace(currLine);
                    currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

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
                                    keyString = currLineToBeProcessed.substr(0,equalPos);
                                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( keyString=="Version")
                                    {
                                        foundFileVersion = string_to_double(valString);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion < 2)
                    {
                        //Begin processing the file.
                        if(!currLineToBeProcessed.empty() )
                        {
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( keyString=="ResourceName")
                                {
                                    renameBox->set_string(valString);
                                }
                                else if( keyString=="ImageLocation")
                                {
                                    if( valString!="NULL")
                                    {
                                        load_image( soughtDir+valString );
                                    }
                                    tilesheetDimensionsStr->set_name(int_to_string(tilesheetInEditor->tsImage->get_width() )+" x "+int_to_string(tilesheetInEditor->tsImage->get_height() )+"px" );
                                }
                                else if( keyString=="Preload")
                                {
                                    preloadCheckBox->set_clicked(is_bool(valString) );
                                }
                                else
                                {
                                    for( i = 0; i <TILESHEET_DATA_FIELD_COUNT ; i++)
                                    {
                                        tsDataStr = "TilesheetData["+int_to_string(i)+"]";
                                        if( keyString==tsDataStr)
                                        {
                                            tsDataFields[i]->set_string(valString);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        record_error("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
    }
    process_data_fields();
}

void tilesheetResource::process_data_fields()
{
    ///constantly updates the tilesheet boxes
    bool allInputsAreValid = true;
    if( tilesheetInEditor!=NULL)
    {
        for(  int i =0; i < TILESHEET_DATA_FIELD_COUNT; i++)
        {
            if( tsDataFields[i]!=NULL)
            {
                if( !tsDataFields[i]->is_valid() )
                {
                    allInputsAreValid = false;
                }
            }
            else
            {
                allInputsAreValid = false;
            }
        }
        if( allInputsAreValid)
        {
            tilesheetInEditor->tsWidth = tsDataFields[0]->get_held_number();
            tilesheetInEditor->tsHeight = tsDataFields[1]->get_held_number();
            tilesheetInEditor->tsXOff = tsDataFields[2]->get_held_number();
            tilesheetInEditor->tsYOff = tsDataFields[3]->get_held_number();
            tilesheetInEditor->tsXPadding = tsDataFields[4]->get_held_number();
            tilesheetInEditor->tsYPadding = tsDataFields[5]->get_held_number();
            tilesheetInEditor->organize_tilesheet();
        }
    }
}

void tilesheetResource::prerender_self(GPE_Renderer * cRender)
{
    standardEditableGameResource::prerender_self(cRender);
    if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self(cRender);
    }
}

void tilesheetResource::process_self(SDL_Rect *viewedSpace,SDL_Rect *cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    int i;
    if( cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL )
    {
        editorPaneList->set_coords(0,0);
        editorPaneList->set_width(256);
        editorPaneList->set_height(viewedSpace->h);
        editorPaneList->barXPadding = GENERAL_GPE_PADDING;
        editorPaneList->barXMargin = 0;

        editorPaneList->clear_list();
        editorPaneList->add_gui_element(renameBox,true);
        editorPaneList->add_gui_element(refreshResourceDataButton,true);
        editorPaneList->add_gui_element(loadResourceButton,true);
        editorPaneList->add_gui_element(labelInfoMaxTextureSize,true);

        //editorPaneList->add_gui_element(saveResourceButton,true);
        editorPaneList->add_gui_element(transformResourceButton,true);
        editorPaneList->add_gui_element(tilesheetDimensionsStr,true);

        SDL_Rect tilesheetPreviewCam;
        tilesheetPreviewCam.x = viewedSpace->x+editorPaneList->get_x2pos()+GENERAL_GPE_PADDING*2;
        tilesheetPreviewCam.y = viewedSpace->y;
        tilesheetPreviewCam.w = viewedSpace->x+viewedSpace->w-tilesheetPreviewCam.x - GENERAL_GPE_PADDING;
        tilesheetPreviewCam.h = abs(viewedSpace->h-GENERAL_GPE_PADDING+GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING- tilesheetPreviewCam.y);

        //processes the tile data fields
        for(  i =0; i < TILESHEET_DATA_FIELD_COUNT; i++)
        {
            if( tsDataFields[i]!=NULL)
            {
                editorPaneList->add_gui_element(tsDataFields[i],true);
            }
        }
        editorPaneList->add_gui_element(preloadCheckBox,true);
        editorPaneList->add_gui_element( openExternalEditorButton,true);
        editorPaneList->add_gui_element(confirmResourceButton,true);

        editorPaneList->add_gui_element(cancelResourceButton,true);
        editorPaneList->set_maxed_out_width();
        if( !tsPreviewer->hasScrollControl )
        {
            editorPaneList->process_self(viewedSpace,cam);
        }
        else
        {
            editorPaneList->hasScrollControl = false;
            editorPaneList->hasArrowkeyControl = false;
        }
        process_data_fields();
        if( tsPreviewer!=NULL && tilesheetInEditor!=NULL)
        {
            tsPreviewer->set_coords(editorPaneList->get_x2pos()+GENERAL_GPE_PADDING,0);
            tsPreviewer->set_width(viewedSpace->w-tsPreviewer->get_xpos() );
            tsPreviewer->set_height(viewedSpace->h-tsPreviewer->get_ypos() );
            tsPreviewer->tileSheetToPreview = tilesheetInEditor;
            if( !editorPaneList->hasScrollControl)
            {
                tsPreviewer->process_self(viewedSpace,cam);
            }
            else
            {
                tsPreviewer->hasScrollControl = false;
                tsPreviewer->hasArrowkeyControl = false;
            }
        }
        if( loadResourceButton!=NULL)
        {
            if( loadResourceButton->is_clicked() )
            {
                std::string newFileName = GPE_GetOpenFileName("Load Your tilesheet Image","Image",GPE_MAIN_GUI->fileOpenTilesheetDir);
                if( (int)newFileName.size() > 3)
                {
                    load_image(newFileName);
                }
            }
        }
        if( refreshResourceDataButton!=NULL && tilesheetInEditor!=NULL )
        {
            if( refreshResourceDataButton->is_clicked() )
            {
                resourcePostProcessed = false;
                preprocess_self();
            }
        }

        if(renameBox!=NULL)
        {
            if( renameBox->get_string()!=resourceName)
            {
                isModified = true;
            }
        }

        if( confirmResourceButton!=NULL)
        {
            //actual saving of the tilesheet onto the engine and files
            if( confirmResourceButton->is_clicked() )
            {
                isPreloaded = preloadCheckBox->is_clicked();
            }
        }

        if( transformResourceButton!=NULL && tilesheetInEditor!=NULL)
        {
            if( transformResourceButton->is_clicked() && tilesheetInEditor->tsImage!=NULL)
            {
                if( tilesheetInEditor->tsImage->get_width() > 0 && tilesheetInEditor->tsImage->get_height() > 0)
                {
                    GPE_open_context_menu();
                    MAIN_CONTEXT_MENU->set_width(256);
                    MAIN_CONTEXT_MENU->add_menu_option("Erase BG Color",0);
                    MAIN_CONTEXT_MENU->add_menu_option("Invert Colors",1);
                    MAIN_CONTEXT_MENU->add_menu_option("Make GrayScale",2);
                    MAIN_CONTEXT_MENU->add_menu_option("Exit",10);
                    int menuSelection = get_popupmenu_result();
                    if( menuSelection>=0 && menuSelection <=3)
                    {
                        SDL_Surface * oTempSurface = load_surface_image(tilesheetInEditor->tsImage->get_filename() );
                        SDL_Surface *nTempSurface = NULL;
                        if( oTempSurface!=NULL)
                        {
                            if( menuSelection==0)
                            {
                                GPE_Color * foundBGColor = GPE_Get_Color_PopUp("Image Background Color To Remove",c_fuchsia);
                                if( foundBGColor!=NULL)
                                {
                                    if( display_get_prompt("Are you sure you want to erase this Color from this image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                    {
                                        record_error("Modifying image at: "+tilesheetInEditor->tsImage->get_filename()+".");
                                        nTempSurface=surface_remove_color(oTempSurface,foundBGColor->get_sdl_color() );
                                        delete foundBGColor;
                                        foundBGColor = NULL;
                                    }
                                }

                            }
                            else if( menuSelection==1 )
                            {
                                if( display_get_prompt("Are you sure you want to invert your image's colors?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                {
                                    nTempSurface=surface_invert(oTempSurface);
                                }
                            }
                            else if( menuSelection==2 )
                            {
                                if( display_get_prompt("Are you sure you want to grayscale your image?","This action is irreversible and will change your image's format to a .png file!")==DISPLAY_QUERY_YES)
                                {
                                    nTempSurface=surface_grayscale(oTempSurface);
                                }
                            }
                            if( nTempSurface!=NULL)
                            {
                                remove( tilesheetInEditor->fileNameLocation.c_str() );
                                std::string newImageName = get_file_noext(tilesheetInEditor->tsImage->get_filename())+".png";
                                IMG_SavePNG(nTempSurface,newImageName.c_str() );
                                tilesheetInEditor->fileNameLocation = newImageName;
                                tilesheetInEditor->tsImage->load_new_texture(MAIN_RENDERER,newImageName);
                                SDL_FreeSurface(nTempSurface);
                                nTempSurface = NULL;
                            }
                            SDL_FreeSurface(oTempSurface);
                            oTempSurface = NULL;
                        }
                    }
                }
            }
        }

        if( openExternalEditorButton!=NULL && tilesheetInEditor!=NULL)
        {
            if( openExternalEditorButton->is_clicked() )
            {
                if( file_exists(tilesheetInEditor->fileNameLocation ) )
                {
                    if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=NULL)
                    {
                        GPE_OpenProgram(MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),tilesheetInEditor->fileNameLocation, true  );
                    }
                    else
                    {
                        GPE_OpenURL(tilesheetInEditor->fileNameLocation );
                    }
                    appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+tilesheetInEditor->fileNameLocation+"]...");
                }
            }
        }
    }
}

void tilesheetResource::render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace,SDL_Rect *cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    bool texturePreviewIsRendered = false;
    if( cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL )
    {
        if( forceRedraw)
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
        }
        SDL_Rect tilesheetPreviewCam = {0,0,0,0};
        tilesheetPreviewCam.x = viewedSpace->x+editorPaneList->get_x2pos();
        tilesheetPreviewCam.y = viewedSpace->y;
        tilesheetPreviewCam.w = viewedSpace->x+viewedSpace->w-tilesheetPreviewCam.x - GENERAL_GPE_PADDING;
        tilesheetPreviewCam.h = abs(viewedSpace->y+viewedSpace->h-GENERAL_GPE_PADDING+(GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING)*2- tilesheetPreviewCam.y);
        if( forceRedraw)
        {
            if( tsPreviewer!=NULL)
            {
                tsPreviewer->render_self(cRender,viewedSpace,cam,forceRedraw);
            }
            //render_rect(cRender,&tilesheetPreviewCam,GPE_MAIN_TEMPLATE->Button_Box_Color,true);

            if( tilesheetInEditor!=NULL)
            {
                render_new_text(cRender,tilesheetPreviewCam.x,tilesheetPreviewCam.y+tilesheetPreviewCam.h+GENERAL_GPE_PADDING,"Number of Tiles:   "+int_to_string((int)tilesheetInEditor->tsRects.size()),GPE_MAIN_TEMPLATE->Text_Box_Color,DEFAULT_FONT,FA_LEFT,FA_TOP );
            }
        }
        SDL_RenderSetViewport( cRender->get_renderer(), NULL );
        SDL_RenderSetViewport( cRender->get_renderer(), viewedSpace );

        if(editorPaneList!=NULL)
        {
            editorPaneList->render_self(cRender, viewedSpace, cam, forceRedraw);
        }
    }
}

void tilesheetResource::save_resource(std::string alternatePath, int backupId)
{
    displayMessageTitle = "Saving Game Tilesheet";
    displayMessageSubtitle = resourceName;
    displayMessageString = "...";
    display_user_messaage();
    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = get_path_from_file(alternatePath);
    if( path_exists(soughtDir) )
    {
        newFileOut = alternatePath;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/tilesheets/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    Game Pencil Engine Project Game Tilesheet DataFile \n";
            newSaveDataFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newSaveDataFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newSaveDataFile << "#    If you have any problems reading this file please report it to debug@pawbyte.com . \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            newSaveDataFile << "ResourceName=" << resourceName << "\n";

            if( tilesheetInEditor!=NULL)
            {
                std::string resFileLocation = getShortFileName(tilesheetInEditor->fileNameLocation,true);
                newSaveDataFile << "ImageLocation="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                {
                    std::string resFileCopySrc = fileToDir(parentProjectName)+"/gpe_project/resources/tilesheets/"+resFileLocation;
                    std::string resFileCopyDest = soughtDir+resFileLocation;
                    if( file_exists(resFileCopyDest) )
                    {
                        if( display_get_prompt("[WARNING]Tilesheet Image Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] Tilesheet File? This action is irreversible!")==DISPLAY_QUERY_YES)
                        {
                            copy_file(resFileCopySrc,resFileCopyDest);
                        }
                    }
                    else
                    {
                        copy_file(resFileCopySrc,resFileCopyDest);
                    }
                }
            }
            if(preloadCheckBox!=NULL)
            {
                newSaveDataFile << "Preload=" << preloadCheckBox->is_clicked() << "\n";
            }
            else
            {
                newSaveDataFile << "Preload=1\n";
            }
            for( int i = 0; i < TILESHEET_DATA_FIELD_COUNT; i++)
            {
                newSaveDataFile << "TilesheetData[" << int_to_string(i)<< "]=";

                if( tsDataFields[i]!=NULL)
                {
                    newSaveDataFile << int_to_string( tsDataFields[i]->get_held_number() )<<"\n";
                }
                else
                {
                    newSaveDataFile << "0\n";
                }
            }
            newSaveDataFile.close();
            if( !usingAltSaveSource)
            {
                isModified = false;
            }
        }
        else
        {
            GPE_Main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void tilesheetResource::update_box(int newX, int newY, int newW, int newH)
{
    if( newX!=-1)
    {
        encapBox.x = newX;
    }
    if(newY!=-1)
    {
        encapBox.y = newY;
    }

    if( newW!=-1)
    {
        encapBox.w = newW;
    }
    if(newH!=-1)
    {
        encapBox.h = newH;
    }
}

bool tilesheetResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Tilesheet=" << resourceName << "," << get_global_rid() << ",";

            *fileTarget << "\n";
            return true;
        }
    }
    //save_resource();
    return false;
}

