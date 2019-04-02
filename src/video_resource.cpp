/*
video_resource.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

#include "video_resource.h"
#include "gpe_editor_settings.h"

std::string SUPPORTED_VIDEO_EXT[SUPPORTED_VIDEO_FORMAT_COUNT];

videoResource::videoResource(GPE_GeneralResourceContainer * pFolder)
{
    projectParentFolder = pFolder;
    resourceFileName = " ";
    resourceName = " ";
    for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
    {
        videoFileName[i] = "";
    }
    videoId = -1;
    videoType = 0;
    videoEditorMainNote = new GPE_Label_Text("Video playback in editor not supported.","Video playback in editor not supported.");

    videoGroupName = new GPE_TextInputBasic("","default=blank");
    videoGroupName->set_label("Video Group");

    defaultVolume = new GPE_TextInputNumber("100",true,0,100);
    defaultVolume->set_string("100");
    defaultVolume->set_label("Default Volume:");

    openExternalEditorButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens Audio In External Editor");
    refreshResourceDataButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes this resource's media files");
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
        *fileTarget << int_to_string (exportBuildGlobalId ) + ",";
        *fileTarget << "'"+html5SpShName + "',";
        for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
        {
            if( (int)videoFileName[i].size() > 3)
            {
                *fileTarget << "'resources/videos/"+getShortFileName( videoFileName[i] )+"',";
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
            copyDestinationStr = outDirectoryName+"/"+ getShortFileName(videoFileName[i],true);
            GPE_Report(videoFileName[i]+" attempted to copy to "+copyDestinationStr);
            if( copy_file(videoFileName[i],copyDestinationStr )==false)
            {
                copyErrorFound = true;
            }
        }
    }
    return copyErrorFound;
}

void videoResource::load_video(std::string newFileName)
{
    if( (int) newFileName.size()>0 )
    {
        bool isvideoFile = true;
        std::string newFileExt = get_file_ext(newFileName);

        if( newFileExt=="mp4")
        {
            videoFileName[mp4FileName] = newFileName;
        }
        else if( newFileExt=="ogg")
        {
            videoFileName[oggVideoFileName] = newFileName;
        }
        else if( newFileExt=="webm")
        {
            videoFileName[webmFileName] = newFileName;
        }
        else
        {
            display_user_alert("[Alert - "+resourceName+"]","Unsupported file type added.");
        }
        if( isvideoFile)
        {
            copy_video_source( fileToDir(parentProjectName)+"/gpe_project/resources/videos" );
        }
    }
}

void videoResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed == false  || file_exists(alternatePath) )
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Video",resourceName );
        }

        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/videos/";
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

        GPE_Report("Loading video - "+newFileIn);
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
            double foundFileVersion = 0;
            std::string fFontFile = "";
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
                                defaultVolume->set_number(string_to_int(valString,100));
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
                    GPE_Report("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                }
            }

            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->increment_and_update( "Video resource processed",resourceName );
            }
        }
        else
        {
            if( GPE_LOADER != NULL )
            {
                GPE_LOADER->increment_and_update( "Video resource processingg error occurred",resourceName );
            }
        }
    }
}

void videoResource::prerender_self()
{
    standardEditableGameResource::prerender_self();
}

void videoResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
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
            std::string newFileName = GPE_GetOpenFileName("Load In an video File","Video",MAIN_GUI_SETTINGS->fileOpenVideoDir);
            load_video(newFileName);
        }
        else if( confirmResourceButton!=NULL && confirmResourceButton->is_clicked() )
        {
            save_resource();
        }
        else if( cancelResourceButton->is_clicked() )
        {
            resourcePostProcessed = false;
            preprocess_self();
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
                MAIN_CONTEXT_MENU->set_width(openExternalEditorButton->get_width() );
                for( ii = 0; ii < SUPPORTED_VIDEO_FORMAT_COUNT; ii++)
                {
                    if( (int)videoFileName[ii].size() > 0)
                    {
                        MAIN_CONTEXT_MENU->add_menu_option("Edit "+SUPPORTED_VIDEO_EXT[ii],ii);
                    }
                }
                int menuSelection = GPE_Get_Context_Result();
                if( menuSelection >=0 && menuSelection < SUPPORTED_VIDEO_FORMAT_COUNT)
                {
                    std::string fileToEdit = videoFileName[menuSelection];

                    if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]!=NULL)
                    {
                        GPE_OpenProgram(MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_VID]->get_string(),fileToEdit, true );
                    }
                    else
                    {
                        GPE_OpenURL(fileToEdit);
                    }
                    /*
                    fileToEdit = "\"C:/Program Files (x86)/Audacity/audacity.exe\" \""+fileToEdit+"\"";
                    GPE_OpenURL(fileToEdit);*/
                    appendToFile(get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+fileToEdit+"]...");
                }
            }
        }
        else if( refreshResourceDataButton!=NULL &&  refreshResourceDataButton->is_clicked() )
        {
            resourcePostProcessed = false;
            preprocess_self();
        }
    }
}

void videoResource::render_self(GPE_Rect * viewedSpace,GPE_Rect *cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && viewedSpace!=NULL && forceRedraw )
    {
        for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
        {
            if( videoFileName[i].size()> 0)
            {
                gfs->render_text( viewedSpace->w-GENERAL_GPE_PADDING*3,GENERAL_GPE_PADDING+GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_VIDEO_EXT[i]+" is attatched",GPE_MAIN_THEME->Main_Box_Font_Highlight_Color,GPE_DEFAULT_FONT,FA_RIGHT,FA_TOP);
            }
            else
            {
                gfs->render_text( viewedSpace->w-GENERAL_GPE_PADDING*3,GENERAL_GPE_PADDING+GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_VIDEO_EXT[i]+" not attatched",GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT,FA_RIGHT,FA_TOP);
            }
        }
        gfs->render_text( viewedSpace->w/2, viewedSpace->h-32, "HTML5 Feature. Currently not supported in C++/Native runtime",GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT, FA_CENTER, FA_BOTTOM, 255);
    }
    //
}

void videoResource::save_resource(std::string alternatePath, int backupId)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Game video", resourceName );
    }

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
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/videos/";
        newFileOut = soughtDir + resourceName+".gpf";
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
                resFileLocation = getShortFileName (videoFileName[i],true );
                newSaveDataFile << "videoFile["+SUPPORTED_VIDEO_EXT[i]+"]="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                {
                    resFileCopySrc = fileToDir(parentProjectName)+"/gpe_project/resources/video/"+resFileLocation;
                    resFileCopyDest = soughtDir+resFileLocation;
                    if( file_exists(resFileCopyDest) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]Video File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] Video file? This action is irreversible!")==DISPLAY_QUERY_YES)
                        {
                            copy_file(resFileCopySrc,resFileCopyDest);
                        }
                        */
                    }
                    else
                    {
                        copy_file(resFileCopySrc,resFileCopyDest);
                    }
                }
            }
        }

        if( defaultVolume!=NULL)
        {
            newSaveDataFile << "DefaultVolume="+int_to_string(defaultVolume->get_held_number() )+"\n";
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
            GPE_LOADER->increment_and_update( "Video resource saved",resourceName );
        }
        return;
    }

    GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->increment_and_update( "Video resource unable to save",resourceName );
    }
}

bool videoResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "video=" << resourceName << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}
