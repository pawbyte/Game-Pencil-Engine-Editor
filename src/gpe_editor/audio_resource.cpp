/*
audio_resource.cpp
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

#include "audio_resource.h"
#include "gpe_editor_settings.h"

audioResource::audioResource(GPE_GeneralResourceContainer * pFolder)
{
    projectParentFolder = pFolder;
    resourceFileName = " ";
    resource_name = " ";
    for( int i = 0; i < 4; i++)
    {
        audioFileName[i] = "";
    }
    audioId = -1;
    audioType = 0;
    musicVal = NULL;
    soundVal = NULL;
    audioEditorMainNote = new GPE_Label_Text("MP3 file type not supported.","Please Note MP3 files are not supported or licensed with usage of the Game Pencil Engine.");
    audioGroupName = new gpe_text_widget_string("","default=blank");
    audioGroupName->set_name("Audio Group");
    audioGroupName->set_label("Audio Group");
    audioTypeButtonController = new GPE_RadioButtonControllerBasic("Audio Type", true,1);
    audioTypeButtonController->add_opton("Normal Audio");
    audioTypeButtonController->add_opton("Voice");
    audioTypeButtonController->add_opton("Background Music");
    if( saveResourceButton!=NULL)
    {
        audioTypeButtonController->set_coords(-1,saveResourceButton->get_ypos()+saveResourceButton->get_height()+GENERAL_GPE_GUI_PADDING);
    }
    openExternalEditorButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Use External Editor" );
    refreshResourceDataButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png"," Refreshes this resource's media files");

    playButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Play" );
    preloadCheckBox = new GPE_CheckBoxBasic("Preload Audio","Check to load audio at game open", true);
    volumeLabel = new GPE_Label_Text("Volume","Default Volume Level");
    defaultVolume = new gpe_text_widget_number("Range 0 to 100",true,0,200 );
    defaultVolume->set_string("100");
    defaultVolume->set_name("Default Volume:");
    defaultVolume->set_width(32);
    volumeSlider = new GPE_Slider_XAxis(100,0,200);
}

audioResource::~audioResource()
{
    if(audioEditorMainNote!=NULL)
    {
        delete audioEditorMainNote;
        audioEditorMainNote = NULL;
    }
    if(volumeLabel!=NULL)
    {
        delete volumeLabel;
        volumeLabel = NULL;
    }
    if(volumeSlider!=NULL)
    {
        delete volumeSlider;
        volumeSlider = NULL;
    }

    if(musicVal!=NULL)
    {
        delete musicVal;
        musicVal = NULL;
        gpe::error_log->report("Removing Audio file...");
    }
    if(soundVal!=NULL)
    {
        delete soundVal;
        soundVal = NULL;
        gpe::error_log->report("Removing Audio file...");
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
        *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) + ",";
        *fileTarget << "'"+html5SpShName + "',";
        for( int i = 0; i < gpe::sound_format_max; i++)
        {
            if( (int)audioFileName[i].size() > 3)
            {
                *fileTarget << "'resources/audio/"+ stg_ex::get_short_filename( audioFileName[i] )+"',";
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

bool audioResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void audioResource::compile_cpp()
{

}

bool audioResource::copy_audio_source(std::string outDirectoryName)
{
    std::string copyDestinationStr = "";
    bool copyErrorFound = false;
    for( int i = 0; i < gpe::sound_format_max ; i++)
    {
        if((int)audioFileName[i].size() > 0)
        {
            copyDestinationStr = outDirectoryName+"/"+ stg_ex::get_short_filename(audioFileName[i],true);
            if( sff_ex::file_copy(audioFileName[i],copyDestinationStr )==false)
            {
                copyErrorFound = true;
            }
        }
    }
    return copyErrorFound;
}

bool audioResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

bool audioResource::is_build_ready()
{
    recentErrorMessage = "";
    return true;
}
void audioResource::load_audio(std::string new_file_name)
{
    if( stg_ex::file_is_audio(new_file_name) )
    {
        bool loadableAudio = false;
        bool isAudioFile = true;
        playButton->set_name("Play");
        playButton->descriptionText = "Push to Play Audio";
        std::string newFileExt = stg_ex::get_file_ext(new_file_name);
        if( newFileExt=="aac")
        {
            audioFileName[gpe::sound_format_acc] = new_file_name;
            loadableAudio = true;
        }
        else if( newFileExt=="mp3")
        {
            audioFileName[ gpe::sound_format_mp3] = new_file_name;
            loadableAudio = true;
        }
        else if( newFileExt=="ogg")
        {
            audioFileName[ gpe::sound_format_ogg ] = new_file_name;
            loadableAudio = true;
        }
        else if( newFileExt=="wav")
        {
            audioFileName[ gpe::sound_format_wav] = new_file_name;
            loadableAudio = true;
        }
        else
        {
            isAudioFile = false;
            display_user_alert("[Alert - "+resource_name+"]","Unsupported file type added.");
        }
        if( loadableAudio )
        {
            if( soundVal!=NULL)
            {
                soundVal->load( new_file_name );
            }
            else
            {
                soundVal = gpe::standard_sound_object->create_new( resource_name, new_file_name );
            }
            if( soundVal==NULL)
            {
                update_action_message("Failed to Load Audio file "+new_file_name+"." );
            }
        }
        if( isAudioFile)
        {
            copy_audio_source( stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/audio" );
        }
    }
    else
    {
        display_user_alert("Unable to load audio","File type["+ stg_ex::get_file_ext(new_file_name)+"] not supported when loading ["+new_file_name+"].");
    }
}

void audioResource::load_resource(std::string file_path )
{

    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Processing Audio", resource_name );
    }
    std::string otherColContainerName = "";

    std::string newFileIn ="";
    std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/audio/";
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

    gpe::error_log->report("Loading audio resource - "+newFileIn);
    if( gameResourceFileIn.fail() )
    {
        gameResourceFileIn.close();
        return;
    }
    //makes sure the file is open
    if ( !gameResourceFileIn.is_open())
    {
        gameResourceFileIn.close();
        return;
    }
    int equalPos = 0;
    std::string firstChar="";
    std::string section="";
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
                    equalPos = currLineToBeProcessed.find_first_of("=");
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
                    else if( keyString=="AudioFileLocation")
                    {
                        load_audio( soughtDir+valString );
                    }
                    else if( keyString=="Preload")
                    {
                        isPreloaded = stg_ex::string_to_bool(valString);
                        preloadCheckBox->set_checked(isPreloaded );
                    }
                    else if( keyString=="AudioGroup")
                    {
                        audioGroupName->set_string(valString);
                    }
                    else if( keyString=="AudioType")
                    {
                        audioType = stg_ex::string_to_int(valString,0);
                        audioTypeButtonController->set_selection(audioType);
                    }
                    else if( keyString=="DefaultVolume")
                    {
                        defaultVolume->set_number( stg_ex::string_to_int(valString,100));
                    }
                    else
                    {
                        for(int i = 0; i < gpe::sound_format_max; i++)
                        {
                            if( keyString=="AudioFile["+ gpe::sound_type_names[i]+"]")
                            {
                                //to at least include if a file type is available in string
                                if( (int)valString.size() > 3 )
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
            gpe::error_log->report("Invalid FoundFileVersion ="+ stg_ex::float_to_string(foundFileVersion)+".");
        }
    }
    gpe::error_log->report("Audio resource loaded...");
}

void audioResource::prerender_self( )
{
    standardEditableGameResource::prerender_self( );
    if( playButton!=NULL)
    {
        playButton->prerender_self( );
    }
    if( openExternalEditorButton!=NULL)
    {
        openExternalEditorButton->prerender_self( );
    }
    if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self( );
    }
    if( audioTypeButtonController!=NULL)
    {
        audioTypeButtonController->prerender_self( );
    }

}

void audioResource::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if(cam!=NULL && viewedSpace!=NULL )
    {
        int prevVolumeSliderVal = volumeSlider->get_value();
        int prevVolumeTypedVal = defaultVolume->get_held_number();

        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            PANEL_GENERAL_EDITOR->clear_panel();

            PANEL_GENERAL_EDITOR->add_gui_element(audioEditorMainNote,true);
            PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);
            PANEL_GENERAL_EDITOR->add_gui_element(audioGroupName, true);

            PANEL_GENERAL_EDITOR->add_gui_auto(refreshResourceDataButton);
            PANEL_GENERAL_EDITOR->add_gui_auto(loadResourceButton);
            PANEL_GENERAL_EDITOR->add_gui_auto(playButton);
            PANEL_GENERAL_EDITOR->add_gui_auto(exportResourceButton);
            PANEL_GENERAL_EDITOR->add_gui_element(openExternalEditorButton,true);

            PANEL_GENERAL_EDITOR->add_gui_element(audioTypeButtonController,true);

            PANEL_GENERAL_EDITOR->add_gui_element(volumeLabel, true);
            PANEL_GENERAL_EDITOR->add_gui_element(volumeSlider, false);
            PANEL_GENERAL_EDITOR->add_gui_element(defaultVolume,true);

            PANEL_GENERAL_EDITOR->add_gui_auto(confirmResourceButton);
            PANEL_GENERAL_EDITOR->add_gui_auto(cancelResourceButton);
            PANEL_GENERAL_EDITOR->process_self( NULL, NULL );

            if( confirmResourceButton!=NULL && confirmResourceButton->is_clicked() )
            {
                save_resource();
            }
            else if( cancelResourceButton->is_clicked() )
            {
                resourcePostProcessed = false;
                load_resource();
            }
            if(audioTypeButtonController!=NULL)
            {
                audioType = audioTypeButtonController->get_selected_id();
            }
            //plays or pauses the sound
            if( playButton!=NULL)
            {
                if( playButton->is_clicked() )
                {
                    if( playButton->get_name()=="Play")
                    {
                        if( soundVal!=NULL)
                        {
                            soundVal->play();
                            playButton->set_name("Stop");
                            playButton->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/stop.png" );
                            playButton->descriptionText = "Push to Stop Audio";
                        }
                    }
                    else if( playButton->get_name()=="Stop")
                    {
                        if( soundVal != NULL )
                        {
                            soundVal->stop();
                        }
                        playButton->set_name("Play");
                        playButton->descriptionText = "Push to Play Audio";
                        playButton->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );

                    }
                    playButton->set_width(loadResourceButton->get_width() );
                }
                else if( playButton->get_name()=="Stop")
                {
                    /*if( audio_is_play( )== false )
                    {
                        playButton->set_name("Play");
                        playButton->descriptionText = "Push to Play Audio";
                        playButton->change_texture( app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
                    }*/
                }
            }
            if( loadResourceButton!=NULL)
            {
                if( loadResourceButton->is_clicked() )
                {
                    std::string new_file_name = GPE_GetOpenFileName("Load In an Audio File","Audio",main_GUI_SETTINGS->fileOpenAudioDir);
                    if( (int)new_file_name.size() > 3)
                    {
                        load_audio(new_file_name);
                    }
                }
            }
            if( openExternalEditorButton!=NULL)
            {
                if( openExternalEditorButton->is_clicked() )
                {
                    bool hasFileToOpen = false;
                    int ii = 0;
                    for( ii = 0; ii < gpe::sound_format_max; ii++)
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
                        main_CONTEXT_MENU->set_width(openExternalEditorButton->get_width() );
                        for( ii = 0; ii < gpe::sound_format_max; ii++)
                        {
                            if( (int)audioFileName[ii].size() > 0)
                            {
                                main_CONTEXT_MENU->add_menu_option("Edit "+ gpe::sound_type_names[ii],ii,NULL,-1,NULL,true,true);
                            }
                        }
                        int menuSelection = GPE_Get_Context_Result();
                        if( menuSelection >=0 && menuSelection < gpe::sound_format_max)
                        {
                            std::string fileToEdit = audioFileName[menuSelection];

                            if( main_EDITOR_SETTINGS!=NULL && main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]!=NULL)
                            {
                                gpe::external_open_program(main_EDITOR_SETTINGS->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->get_string(),fileToEdit, true );
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
            }

            if( refreshResourceDataButton!=NULL )
            {
                if( refreshResourceDataButton->is_clicked() )
                {
                    resourcePostProcessed = false;
                    load_resource();
                }
            }
            if( defaultVolume->get_held_number() != prevVolumeTypedVal )
            {
                volumeSlider->set_value(  defaultVolume->get_held_number()  );
            }
            else if( volumeSlider->get_value() != prevVolumeSliderVal  )
            {
                defaultVolume->set_number(  volumeSlider->get_value()   );
            }
        }
    }
}

void audioResource::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if(cam!=NULL && viewedSpace!=NULL)
    {
        std::string support_string = "";

        int current_max_types = 0;
        int i_sound_type = 0;
        for(  i_sound_type = 0; i_sound_type < gpe::sound_format_max; i_sound_type++)
        {
            if( (int)gpe::sound_type_names[i_sound_type].size() > 0 )
            {
                current_max_types++;
            }
        }
        int required_height = current_max_types *( GPE_AVERAGE_LINE_HEIGHT+GENERAL_GPE_GUI_PADDING );

        if( viewedSpace->h >= required_height )
        {
            for( i_sound_type= 0; i_sound_type < current_max_types; i_sound_type++)
            {
                if( gpe::sound_is_format_supported[i_sound_type] )
                {
                    support_string = "Supported with current sound system";
                }
                else
                {
                    support_string = "Not supported with current sound system";
                }

                gpe::gfs->render_text( GENERAL_GPE_GUI_PADDING,viewedSpace->h - i_sound_type*(GENERAL_GPE_GUI_PADDING+GPE_AVERAGE_LINE_HEIGHT), gpe::sound_type_names[i_sound_type]+" "+support_string,theme_main->main_box_font_highlight_color,font_default, gpe::fa_left, gpe::fa_bottom);

                if( audioFileName[i_sound_type].size()> 0)
                {
                    gpe::gfs->render_text( viewedSpace->w-GENERAL_GPE_GUI_PADDING*3,i_sound_type *(GENERAL_GPE_GUI_PADDING+GPE_AVERAGE_LINE_HEIGHT), gpe::sound_type_names[i_sound_type]+" attatched ",theme_main->main_box_font_highlight_color,font_default, gpe::fa_right, gpe::fa_top);
                }
                else
                {
                    gpe::gfs->render_text( viewedSpace->w-GENERAL_GPE_GUI_PADDING*3,i_sound_type*(GENERAL_GPE_GUI_PADDING+GPE_AVERAGE_LINE_HEIGHT), gpe::sound_type_names[i_sound_type]+" not attatched ",theme_main->main_box_font_color,font_default,gpe::fa_right,gpe::fa_top);
                }
            }
        }
        gpe::gfs->render_text( viewedSpace->w - GENERAL_GPE_GUI_PADDING,viewedSpace->h - GENERAL_GPE_GUI_PADDING, gpe::sound_system_name,theme_main->main_box_font_highlight_color,font_default, gpe::fa_right, gpe::fa_bottom);
    }
}

void audioResource::save_resource(std::string file_path, int backupId)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Audio", resource_name );
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
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/audio/";
        newFileOut = soughtDir + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the audio file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            std::string resFileLocation = "";
            std::string resFileCopySrc;
            std::string resFileCopyDest;
            for(int i = 0; i < gpe::sound_format_max; i++)
            {
                if( (int)audioFileName[i].size() > 3)
                {
                    resFileLocation = stg_ex::get_short_filename (audioFileName[i],true );
                    newSaveDataFile << "AudioFile["+ gpe::sound_type_names[i]+"]="+resFileLocation+"\n";
                    if( (int)resFileLocation.size() > 0 && usingAltSaveSource )
                    {
                        resFileCopySrc = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/audio/"+resFileLocation;
                        resFileCopyDest = soughtDir+resFileLocation;
                        if( sff_ex::file_exists(resFileCopyDest) )
                        {
                            /*
                            if( GPE_Display_Basic_Prompt("[WARNING]Audio File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] audio file? This action is irreversible!")==DISPLAY_QUERY_YES)
                            {
                                file_copy(resFileCopySrc,resFileCopyDest);
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
            if( preloadCheckBox!=NULL)
            {
                newSaveDataFile << "Preload="+ stg_ex::int_to_string(preloadCheckBox->is_clicked() )+"\n";
            }
            if( defaultVolume!=NULL)
            {
                newSaveDataFile << "DefaultVolume="+ stg_ex::int_to_string(defaultVolume->get_held_number() )+"\n";
            }
            if( audioGroupName!=NULL)
            {
                newSaveDataFile << "AudioGroup="+audioGroupName->get_string()+"\n";

            }
            if( audioTypeButtonController!=NULL)
            {
                newSaveDataFile << "AudioType="+ stg_ex::int_to_string(audioTypeButtonController->get_selected_id() )+"\n";
            }

            newSaveDataFile.close();
            if( !usingAltSaveSource)
            {
                isModified = false;
            }
        }
        else
        {
            GPE_main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

bool audioResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "audio=" << resource_name << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}
