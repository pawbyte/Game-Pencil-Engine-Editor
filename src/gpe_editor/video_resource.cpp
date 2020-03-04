/*
video_resource.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

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
#include "gpe_editor_settings.h"

std::string SUPPORTED_VIDEO_EXT[SUPPORTED_VIDEO_FORMAT_COUNT];

videoResource::videoResource(GPE_GeneralResourceContainer * pFolder)
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
    videoEditorMainNote = new GPE_Label_Text("Video playback in editor not supported.","Video playback in editor not supported.");

    videoGroupName = new gpe_text_widget_string("","default=blank");
    videoGroupName->set_label("Video Group");

    defaultVolume = new gpe_text_widget_number("100",true,0,100);
    defaultVolume->set_string("100");
    defaultVolume->set_label("Default Volume:");

    openExternalEditorButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Audio In External Editor");
    refreshResourceDataButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes this resource's media files");
}

videoResource::~videoResource()
{
    if( videoEditorMainNote!=NULL)
    {
        delete videoEditorMainNote;
        videoEditorMainNote = NULL;
    }
    if( defaultVolume!=NULL)
    {
        delete defaultVolume;
        defaultVolume = NULL;
    }
    if( videoGroupName!=NULL)
    {
        delete videoGroupName;
        videoGroupName = NULL;
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
}

bool videoResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string nestedTabsStrObjFunc = generate_tabs( leftTabAmount +1 );
        std::string html5SpShName = get_name();

        *fileTarget << nestedTabsStr << "var " + html5SpShName + " =  GPE.rsm.add_video(";
        *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) + ",";
        *fileTarget << "'"+html5SpShName + "',";
        for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
        {
            if( (int)videoFileName[i].size() > 3)
            {
                *fileTarget << "'resources/videos/"+stg_ex::get_short_filename( videoFileName[i] )+"',";
            }
            else
            {
                *fileTarget << "'',";
            }
        }
        if( videoGroupName!=NULL)
        {
            *fileTarget << "'"+videoGroupName->get_string()+"',";
        }
        else
        {
            *fileTarget << "'',";
        }
        if( defaultVolume!=NULL)
        {
            *fileTarget << defaultVolume->get_held_number() << "";
        }
        else
        {
            *fileTarget << "100";
        }
        *fileTarget << "); \n";

        return true;
    }
    return false;
}


bool videoResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void videoResource::compile_cpp()
{

}

bool videoResource::copy_video_source(std::string outDirectoryName)
{
    std::string copyDestinationStr = "";
    bool copyErrorFound = false;
    for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT ; i++)
    {
        if((int)videoFileName[i].size() > 0)
        {
            copyDestinationStr = outDirectoryName+"/"+ stg_ex::get_short_filename(videoFileName[i],true);
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
            display_user_alert("[Alert - "+resource_name+"]","Unsupported file type added.");
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
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Video",resource_name );
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
            std::string keyString="";
            std::string valString="";
            std::string subValString="";
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
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( keyString=="Version")
                                {
                                    foundFileVersion = stg_ex::string_to_float(valString);
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
                            keyString = currLineToBeProcessed.substr(0,equalPos);
                            valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                            if( keyString=="ResourceName")
                            {
                                renameBox->set_string(valString);
                            }
                            else if( keyString=="videoFileLocation")
                            {
                                load_video( soughtDir+valString );
                            }
                            else if( keyString=="VideoGroup")
                            {
                                videoGroupName->set_string(valString);
                            }
                            else if( keyString=="DefaultVolume")
                            {
                                defaultVolume->set_number( stg_ex::string_to_int(valString,100));
                            }
                            else
                            {
                                for(int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
                                {
                                    if( keyString=="videoFile["+SUPPORTED_VIDEO_EXT[i]+"]")
                                    {
                                        if( (int)valString.size() > 3)
                                        {
                                            load_video( soughtDir+valString );
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

            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->increment_and_update( "Video resource processed",resource_name );
            }
        }
        else
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->increment_and_update( "Video resource processingg error occurred",resource_name );
            }
        }
    }
}

void videoResource::prerender_self()
{
    standardEditableGameResource::prerender_self();
}

void videoResource::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if(cam!=NULL && viewedSpace!=NULL && PANEL_GENERAL_EDITOR!=NULL)
    {
        PANEL_GENERAL_EDITOR->clear_panel();
        PANEL_GENERAL_EDITOR->add_gui_element(videoEditorMainNote,true);
        PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);

        PANEL_GENERAL_EDITOR->add_gui_element(refreshResourceDataButton,false );
        PANEL_GENERAL_EDITOR->add_gui_element(loadResourceButton,false );
        PANEL_GENERAL_EDITOR->add_gui_element(openExternalEditorButton,true);

        PANEL_GENERAL_EDITOR->add_gui_element(videoGroupName, true);
        PANEL_GENERAL_EDITOR->add_gui_element(defaultVolume,true);
        PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);
        PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
        //PANEL_GENERAL_EDITOR->set_maxed_out_width();
        PANEL_GENERAL_EDITOR->process_self(NULL, NULL);


        if( loadResourceButton!=NULL && loadResourceButton->is_clicked() )
        {
            std::string new_file_name = GPE_GetOpenFileName("Load In an video File","Video",main_GUI_SETTINGS->fileOpenVideoDir);
            load_video(new_file_name);
        }
        else if( confirmResourceButton!=NULL && confirmResourceButton->is_clicked() )
        {
            save_resource();
        }
        else if( cancelResourceButton->is_clicked() )
        {
            resourcePostProcessed = false;
            load_resource();
        }
        else if( openExternalEditorButton!=NULL &&  openExternalEditorButton->is_clicked() )
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
                GPE_open_context_menu(-1,-1,256);
                main_CONTEXT_MENU->set_width(openExternalEditorButton->get_width() );
                for( ii = 0; ii < SUPPORTED_VIDEO_FORMAT_COUNT; ii++)
                {
                    if( (int)videoFileName[ii].size() > 0)
                    {
                        main_CONTEXT_MENU->add_menu_option("Edit "+SUPPORTED_VIDEO_EXT[ii],ii);
                    }
                }
                int menuSelection = GPE_Get_Context_Result();
                if( menuSelection >=0 && menuSelection < SUPPORTED_VIDEO_FORMAT_COUNT)
                {
                    std::string fileToEdit = videoFileName[menuSelection];

                    if( main_EDITOR_SETTINGS!=NULL && main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]!=NULL)
                    {
                        gpe::external_open_program(main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->get_string(),fileToEdit, true );
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
        else if( refreshResourceDataButton!=NULL &&  refreshResourceDataButton->is_clicked() )
        {
            resourcePostProcessed = false;
            load_resource();
        }
    }
}

void videoResource::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
        {
            if( videoFileName[i].size()> 0)
            {
                gpe::gfs->render_text( viewedSpace->w-GENERAL_GPE_GUI_PADDING*3,GENERAL_GPE_GUI_PADDING+GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_VIDEO_EXT[i]+" is attatched",theme_main->main_box_font_highlight_color,font_default,gpe::fa_right,gpe::fa_top);
            }
            else
            {
                gpe::gfs->render_text( viewedSpace->w-GENERAL_GPE_GUI_PADDING*3,GENERAL_GPE_GUI_PADDING+GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_VIDEO_EXT[i]+" not attatched",theme_main->main_box_font_color,font_default,gpe::fa_right,gpe::fa_top);
            }
        }
        gpe::gfs->render_text( viewedSpace->w/2, viewedSpace->h-32, "HTML5 Feature. Currently not supported in C++/Native runtime",theme_main->main_box_font_color,font_default, gpe::fa_center, gpe::fa_bottom, 255);
    }
    //
}

void videoResource::save_resource(std::string file_path, int backupId)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Game video", resource_name );
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

        std::string resFileLocation = "";
        std::string resFileCopySrc;
        std::string resFileCopyDest;

        for(int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
        {
            if( (int)videoFileName[i].size() > 3)
            {
                resFileLocation = stg_ex::get_short_filename (videoFileName[i],true );
                newSaveDataFile << "videoFile["+SUPPORTED_VIDEO_EXT[i]+"]="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                {
                    resFileCopySrc = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/video/"+resFileLocation;
                    resFileCopyDest = soughtDir+resFileLocation;
                    if( sff_ex::file_exists(resFileCopyDest) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]Video File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] Video file? This action is irreversible!")==DISPLAY_QUERY_YES)
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

        if( defaultVolume!=NULL)
        {
            newSaveDataFile << "DefaultVolume="+ stg_ex::int_to_string(defaultVolume->get_held_number() )+"\n";
        }

        if( videoGroupName!=NULL)
        {
            newSaveDataFile << "VideoGroup="+videoGroupName->get_string()+"\n";

        }

        newSaveDataFile.close();
        if( !usingAltSaveSource)
        {
            isModified = false;
        }
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->increment_and_update( "Video resource saved",resource_name );
        }
        return;
    }

    GPE_main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->increment_and_update( "Video resource unable to save",resource_name );
    }
}

bool videoResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "video=" << resource_name << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}
