/*
audio_resource.cpp
This file is part of:
GAME PENCIL ENGINE
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

#include "gpe_project_resources.h"

audioResource::audioResource(GPE_ResourceContainer * pFolder)
{
    projectParentFolder = pFolder;
    resourceFileName = " ";
    resourceName = " ";
    for( int i = 0; i < 4; i++)
    {
        audioFileName[i] = "";
    }
    audioId = -1;
    audioType = 0;
    musicVal = NULL;
    soundVal = NULL;
    audioEditorMainNote = new GPE_Label_Text("MP3 file type not supported.","Please Note MP3 files are not supported or licensed with usage of the Game Pencil Engine.");
    audioGroupName = new GPE_TextInputBasic("","default=blank");
    audioGroupName->set_name("Audio Group");
    audioGroupName->set_label("Audio Group");
    audioTypeButtonController = new GPE_RadioButtonControllerBasic("Audio Type",GENERAL_GPE_PADDING,192,true,1);
    audioTypeButtonController->add_opton("Normal Audio");
    audioTypeButtonController->add_opton("Voice");
    audioTypeButtonController->add_opton("Background Music");
    if( saveResourceButton!=NULL)
    {
        audioTypeButtonController->set_coords(-1,saveResourceButton->get_ypos()+saveResourceButton->get_height()+GENERAL_GPE_PADDING);
    }
    openExternalEditorButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/rocket.png","Use External Editor","Opens Audio In External Editor");
    openExternalEditorButton->set_width(loadResourceButton->get_width() );
    refreshResourceDataButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/refresh.png","Refresh","Refreshes this resource's media files");

    playButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png","Play","Play Audio");
    playButton->set_width(loadResourceButton->get_width() );
    preloadCheckBox = new GPE_CheckBoxBasic("Preload Audio","Check to load audio at game open",GENERAL_GPE_PADDING,304,true);
    defaultVolume = new GPE_TextInputNumber("Range 0 to 100",true,0,100);
    defaultVolume->set_string("100");
    defaultVolume->set_name("Default Volume:");
    defaultVolume->set_label("Default Volume:");
}

audioResource::~audioResource()
{
    if(audioEditorMainNote!=NULL)
    {
        delete audioEditorMainNote;
        audioEditorMainNote = NULL;
    }
    if(musicVal!=NULL)
    {
        Mix_FreeMusic(musicVal);
        musicVal = NULL;
        record_error("Removing Audio file...");
    }
    if(soundVal!=NULL)
    {
        Mix_FreeChunk(soundVal);
        soundVal = NULL;
        record_error("Removing Audio file...");
    }

    if( audioTypeButtonController!=NULL)
    {
        delete audioTypeButtonController;
        audioTypeButtonController = NULL;
    }
    if( defaultVolume!=NULL)
    {
        delete defaultVolume;
        defaultVolume = NULL;
    }
    if( audioGroupName!=NULL)
    {
        delete audioGroupName;
        audioGroupName = NULL;
    }
    if( audioGroupName!=NULL)
    {
        delete audioGroupName;
        audioGroupName = NULL;
    }
    if( playButton!=NULL)
    {
        delete playButton;
        playButton = NULL;
    }
    if( refreshResourceDataButton!=NULL)
    {
        delete refreshResourceDataButton;
        refreshResourceDataButton = NULL;
    }
}

bool audioResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string nestedTabsStrObjFunc = generate_tabs( leftTabAmount +1 );
        std::string html5SpShName = get_name();

        *fileTarget << nestedTabsStr << "var " + html5SpShName + " =  GPE.rsm.add_audio(";
        *fileTarget << int_to_string (html5BuildGlobalId ) + ",";
        *fileTarget << "'"+html5SpShName + "',";
        for( int i = 0;i < SUPPORTED_AUDIO_FORMAT_COUNT; i++)
        {
            if( (int)audioFileName[i].size() > 3)
            {
                *fileTarget << "'resources/audio/"+getShortFileName( audioFileName[i] )+"',";
            }
            else
            {
                *fileTarget << "'',";
            }
        }
        if( audioTypeButtonController!=NULL)
        {
            *fileTarget << audioTypeButtonController->get_selected_id() << ",";
        }
        else
        {
            *fileTarget << "0,";
        }
        if( audioGroupName!=NULL)
        {
            *fileTarget << "'"+audioGroupName->get_string()+"',";
        }
        else
        {
            *fileTarget << "'',";
        }
        if( preloadCheckBox!=NULL)
        {
            *fileTarget << preloadCheckBox->is_clicked() << ",";
        }
        else
        {
            *fileTarget << "0,";
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

bool audioResource::copy_audio_source(std::string outDirectoryName)
{
    std::string copyDestinationStr = "";
    bool copyErrorFound = false;
    for( int i = 0; i < SUPPORTED_AUDIO_FORMAT_COUNT ; i++)
    {
        if((int)audioFileName[i].size() > 0)
        {
            copyDestinationStr = outDirectoryName+"/"+ getShortFileName(audioFileName[i],true);
            if( copy_file(audioFileName[i],copyDestinationStr )==false)
            {
                copyErrorFound = true;
            }
        }
    }
    return copyErrorFound;
}

void audioResource::load_audio(std::string newFileName)
{
    if( file_is_audio(newFileName) )
    {
        bool loadableAudio = false;
        bool isAudioFile = true;
        playButton->set_name("Play");
        playButton->descriptionText = "Push to Play Audio";
        std::string newFileExt = get_file_ext(newFileName);
        if( newFileExt=="aac")
        {
            audioFileName[aacFileName] = newFileName;
        }
        else if( newFileExt=="mp3")
        {
            audioFileName[mp3FileName] = newFileName;
        }
        else if( newFileExt=="ogg")
        {
            audioFileName[oggFileName] = newFileName;
            loadableAudio = true;
        }
        else if( newFileExt=="wav")
        {
            audioFileName[wavFileName] = newFileName;
            loadableAudio = true;
        }
        else
        {
            isAudioFile = false;
            display_user_alert("[Alert - "+resourceName+"]","Unsupported file type added.");
        }
        if( loadableAudio )
        {
            if( soundVal!=NULL)
            {
                Mix_FreeChunk(soundVal);
                soundVal = NULL;
            }
            soundVal = Mix_LoadWAV(newFileName.c_str() );
            if( soundVal==NULL)
            {
                update_action_message("Failed to Load Audio file "+newFileName+"." );
            }
        }
        if( isAudioFile)
        {
            copy_audio_source( fileToDir(parentProjectName)+"/gpe_project/resources/audio" );
        }
    }
    else
    {
        display_user_alert("Unable to load audio","File type["+get_file_ext(newFileName)+"] not supported when loading ["+newFileName+"].");
    }
}

void audioResource::preprocess_self(std::string alternatePath )
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        displayMessageTitle = "Processing Audio";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();
        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/audio/";
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

        record_error("Loading audio - "+newFileIn);
        if( !gameResourceFileIn.fail() )
        //If the level file could be loaded
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
                                equalPos = currLineToBeProcessed.find_first_of("=");
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
                                else if( keyString=="AudioFileLocation")
                                {
                                    load_audio( soughtDir+valString );
                                }
                                else if( keyString=="Preload")
                                {
                                    isPreloaded = is_bool(valString);
                                    preloadCheckBox->set_clicked(isPreloaded );
                                }
                                else if( keyString=="AudioGroup")
                                {
                                    audioGroupName->set_string(valString);
                                }
                                else if( keyString=="AudioType")
                                {
                                    audioType = string_to_int(valString,0);
                                    audioTypeButtonController->set_selection(audioType);
                                }
                                else if( keyString=="DefaultVolume")
                                {
                                    defaultVolume->set_number(string_to_int(valString,100));
                                }
                                else
                                {
                                    for(int i = 0; i < SUPPORTED_AUDIO_FORMAT_COUNT; i++)
                                    {
                                        if( keyString=="AudioFile["+SUPPORTED_AUDIO_EXT[i]+"]")
                                        {
                                            if( (int)valString.size() > 3)
                                            {
                                                load_audio( soughtDir+valString );
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

void audioResource::prerender_self(GPE_Renderer * cRender)
{
    standardEditableGameResource::prerender_self(cRender);
    if( playButton!=NULL)
    {
        playButton->prerender_self(cRender);
    }
    if( openExternalEditorButton!=NULL)
    {
        openExternalEditorButton->prerender_self(cRender);
    }
    if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self(cRender);
    }
    if( audioTypeButtonController!=NULL)
    {
        audioTypeButtonController->prerender_self(cRender);
    }

}

void audioResource::process_self(GPE_Rect * viewedSpace,GPE_Rect * cam )
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

        editorPaneList->add_gui_element(audioEditorMainNote,true);
        editorPaneList->add_gui_element(renameBox,true);
        editorPaneList->add_gui_element(refreshResourceDataButton,true);
        editorPaneList->add_gui_element(loadResourceButton,true);
        editorPaneList->add_gui_element(playButton,true);
        editorPaneList->add_gui_element(audioTypeButtonController,true);
        editorPaneList->add_gui_element(openExternalEditorButton,true);
        editorPaneList->add_gui_element(audioGroupName, true);
        editorPaneList->add_gui_element(defaultVolume,true);
        editorPaneList->add_gui_element(confirmResourceButton,true);
        editorPaneList->add_gui_element(cancelResourceButton,true);
        editorPaneList->add_gui_element(exportResourceButton,true);
        editorPaneList->set_maxed_out_width();
        editorPaneList->process_self(viewedSpace,cam);
        if(audioTypeButtonController!=NULL)
        {
            audioType = audioTypeButtonController->get_selected_id();
        }
        //plays or pauses the sound
        if( playButton!=NULL)
        {
            if( playButton->is_clicked() )
            {
                Mix_HaltChannel(-1);
                if( playButton->get_name()=="Play")
                {
                    if( soundVal!=NULL)
                    {
                        Mix_PlayChannel( -1, soundVal, 0 );
                        update_action_message("Playing Audio" );
                        playButton->set_name("Stop");
                        playButton->change_texture( rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/stop.png") );
                        playButton->descriptionText = "Push to Stop Audio";
                    }
                }
                else if( playButton->get_name()=="Stop")
                {
                    playButton->set_name("Play");
                    playButton->change_texture( rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png") );
                    playButton->descriptionText = "Push to Play Audio";playButton->change_texture( rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/play.png") );

                }
                playButton->set_width(loadResourceButton->get_width() );
            }
        }
        if( loadResourceButton!=NULL)
        {
            if( loadResourceButton->is_clicked() )
            {
                std::string newFileName = GPE_GetOpenFileName("Load In an Audio File","Audio",MAIN_GUI_SETTINGS->fileOpenAudioDir);
                if( (int)newFileName.size() > 3)
                {
                    load_audio(newFileName);
                }
            }
        }
        if( openExternalEditorButton!=NULL)
        {
            if( openExternalEditorButton->is_clicked() )
            {
                bool hasFileToOpen = false;
                int ii = 0;
                for( ii = 0; ii < SUPPORTED_AUDIO_FORMAT_COUNT; ii++)
                {
                    if( (int)audioFileName[ii].size() > 0)
                    {
                        hasFileToOpen = true;
                        break;
                    }
                }
                if( hasFileToOpen )
                {
                    GPE_open_context_menu();
                    MAIN_CONTEXT_MENU->set_width(openExternalEditorButton->get_width() );
                    for( ii = 0; ii < SUPPORTED_AUDIO_FORMAT_COUNT; ii++)
                    {
                        if( (int)audioFileName[ii].size() > 0)
                        {
                            MAIN_CONTEXT_MENU->add_menu_option("Edit "+SUPPORTED_AUDIO_EXT[ii],ii,NULL,-1,NULL,true,true);
                        }
                    }
                    int menuSelection = get_popupmenu_result();
                    if( menuSelection >=0 && menuSelection < SUPPORTED_AUDIO_FORMAT_COUNT)
                    {
                        std::string fileToEdit = audioFileName[menuSelection];

                        if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]!=NULL)
                        {
                            GPE_OpenProgram(MAIN_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->get_string(),fileToEdit, true );
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

void audioResource::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace,GPE_Rect *cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL)
    {
        if( forceRedraw)
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
            for( int i = 0; i < SUPPORTED_AUDIO_FORMAT_COUNT; i++)
            {
                if( audioFileName[i].size()> 0)
                {
                    render_new_text(cRender,viewedSpace->w-GENERAL_GPE_PADDING*3,GENERAL_GPE_PADDING+GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_AUDIO_EXT[i]+" attatched",GPE_MAIN_TEMPLATE->Main_Box_Font_Highlight_Color,DEFAULT_FONT,FA_RIGHT,FA_TOP);
                }
                else
                {
                    render_new_text(cRender,viewedSpace->w-GENERAL_GPE_PADDING*3,GENERAL_GPE_PADDING+GPE_AVERAGE_LINE_HEIGHT*i,SUPPORTED_AUDIO_EXT[i]+" not attatched",GPE_MAIN_TEMPLATE->Main_Box_Font_Color,DEFAULT_FONT,FA_RIGHT,FA_TOP);
                }
            }
        }
        editorPaneList->render_self(cRender,viewedSpace,cam, forceRedraw);
    }
    //
}

void audioResource::save_resource(std::string alternatePath, int backupId)
{
    displayMessageTitle = "Saving Game Audio";
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
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/audio/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the audio file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    Game Pencil Engine Project Game Audio DataFile \n";
            newSaveDataFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newSaveDataFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newSaveDataFile << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#     \n";
            newSaveDataFile << "#    --------------------------------------------------  # \n";
            newSaveDataFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            newSaveDataFile << "ResourceName=" << resourceName << "\n";

            std::string resFileLocation = "";
            std::string resFileCopySrc;
            std::string resFileCopyDest;
            for(int i = 0; i < SUPPORTED_AUDIO_FORMAT_COUNT; i++)
            {
                if( (int)audioFileName[i].size() > 3)
                {
                    resFileLocation = getShortFileName (audioFileName[i],true );
                    newSaveDataFile << "AudioFile["+SUPPORTED_AUDIO_EXT[i]+"]="+resFileLocation+"\n";
                    if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                    {
                        resFileCopySrc = fileToDir(parentProjectName)+"/gpe_project/resources/audio/"+resFileLocation;
                        resFileCopyDest = soughtDir+resFileLocation;
                        if( file_exists(resFileCopyDest) )
                        {
                            if( display_get_prompt("[WARNING]Audio File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] audio file? This action is irreversible!")==DISPLAY_QUERY_YES)
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
            if( preloadCheckBox!=NULL)
            {
                newSaveDataFile << "Preload="+int_to_string(preloadCheckBox->is_clicked() )+"\n";
            }
            if( defaultVolume!=NULL)
            {
                newSaveDataFile << "DefaultVolume="+int_to_string(defaultVolume->get_held_number() )+"\n";
            }
            if( audioGroupName!=NULL)
            {
                newSaveDataFile << "AudioGroup="+audioGroupName->get_string()+"\n";

            }
            if( audioTypeButtonController!=NULL)
            {
                newSaveDataFile << "AudioType="+int_to_string(audioTypeButtonController->get_selected_id() )+"\n";
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

bool audioResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "audio=" << resourceName << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}
