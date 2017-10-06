/*
video_resource.cpp
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

videoResource::videoResource(GPE_ResourceContainer * pFolder)
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

    openExternalEditorButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/rocket.png","Use External Editor","Opens Audio In External Editor");
    refreshResourceDataButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/refresh.png","Refresh","Refreshes this resource's media files");
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
        *fileTarget << int_to_string (html5BuildGlobalId ) + ",";
        *fileTarget << "'"+html5SpShName + "',";
        for( int i = 0;i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
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

bool videoResource::copy_video_source(std::string outDirectoryName)
{
    std::string copyDestinationStr = "";
    bool copyErrorFound = false;
    for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT ; i++)
    {
        if((int)videoFileName[i].size() > 0)
        {
            copyDestinationStr = outDirectoryName+"/"+ getShortFileName(videoFileName[i],true);
            record_error(videoFileName[i]+" attempted to copy to "+copyDestinationStr);
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
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        displayMessageTitle = "Processing Video";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();
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

        record_error("Loading video - "+newFileIn);
        //If the level file could be loaded
        if( gameResourceFileIn != NULL )
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
                float foundFileVersion = 0;
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
                        record_error("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
    }
}

void videoResource::prerender_self(GPE_Renderer * cRender)
{
    standardEditableGameResource::prerender_self(cRender);
}

void videoResource::process_self(SDL_Rect *viewedSpace,SDL_Rect *cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL)
    {
        editorPaneList->set_coords(0,0);
        editorPaneList->set_width(viewedSpace->w);
        editorPaneList->set_height(viewedSpace->h);
        editorPaneList->barXPadding = GENERAL_GPE_PADDING;
        editorPaneList->barXMargin = GENERAL_GPE_PADDING;

        editorPaneList->clear_list();
        editorPaneList->add_gui_element(videoEditorMainNote,true);
        editorPaneList->add_gui_element(renameBox,true);

        editorPaneList->add_gui_element(refreshResourceDataButton,true);
        editorPaneList->add_gui_element(loadResourceButton,true);
        editorPaneList->add_gui_element(videoGroupName, true);
        editorPaneList->add_gui_element(defaultVolume,true);
        editorPaneList->add_gui_element(openExternalEditorButton,true);
        editorPaneList->add_gui_element(confirmResourceButton,true);
        editorPaneList->add_gui_element(cancelResourceButton,true);
        editorPaneList->set_maxed_out_width();
        editorPaneList->process_self(viewedSpace,cam);
        if( loadResourceButton!=NULL)
        {
            if( loadResourceButton->is_clicked() )
            {
                std::string newFileName = GPE_GetOpenFileName("Load In an video File","Video",GPE_MAIN_GUI->fileOpenVideoDir);
                load_video(newFileName);
            }
        }

        if( openExternalEditorButton!=NULL)
        {
            if( openExternalEditorButton->is_clicked() )
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
                    GPE_open_context_menu();
                    MAIN_CONTEXT_MENU->set_width(openExternalEditorButton->get_width() );
                    for( ii = 0; ii < SUPPORTED_VIDEO_FORMAT_COUNT; ii++)
                    {
                        if( (int)videoFileName[ii].size() > 0)
                        {
                            MAIN_CONTEXT_MENU->add_menu_option("Edit "+SUPPORTED_VIDEO_EXT[ii],ii);
                        }
                    }
                    int menuSelection = get_popupmenu_result();
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
        }

        if( refreshResourceDataButton!=NULL )
        {
            if( refreshResourceDataButton->is_clicked() )
            {
                resourcePostProcessed = false;
                preprocess_self();
            }
        }

        defaultVolume->process_self(viewedSpace,cam);
    }
}

void videoResource::render_self(GPE_Renderer * cRender,SDL_Rect *viewedSpace,SDL_Rect *cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL)
    {
        if( forceRedraw )
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
            for( int i = 0; i < SUPPORTED_VIDEO_FORMAT_COUNT; i++)
            {
                if( videoFileName[i].size()> 0)
                {
                    render_new_text(cRender,viewedSpace->w-GENERAL_GPE_PADDING*3,GENERAL_GPE_PADDING+GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_VIDEO_EXT[i]+" is attatched",GPE_MAIN_TEMPLATE->Main_Box_Font_Highlight_Color,DEFAULT_FONT,FA_RIGHT,FA_TOP);
                }
                else
                {
                    render_new_text(cRender,viewedSpace->w-GENERAL_GPE_PADDING*3,GENERAL_GPE_PADDING+GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_VIDEO_EXT[i]+" not attatched",GPE_MAIN_TEMPLATE->Main_Box_Font_Color,DEFAULT_FONT,FA_RIGHT,FA_TOP);
                }
            }
        }

        editorPaneList->render_self(cRender,viewedSpace,cam, forceRedraw);
    }
    //
}

void videoResource::save_resource(std::string alternatePath, int backupId)
{
    displayMessageTitle = "Saving Game video";
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
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/videos/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( newSaveDataFile != NULL )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    Game Pencil Engine Project Game Video DataFile \n";
            newSaveDataFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newSaveDataFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newSaveDataFile << "#    If you have any problems reading this file please report it to debug@pawbyte.com . \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            newSaveDataFile << "ResourceName=" << resourceName << "\n";

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
                            if( display_get_prompt("[WARNING]Video File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] Video file? This action is irreversible!")==DISPLAY_QUERY_YES)
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
