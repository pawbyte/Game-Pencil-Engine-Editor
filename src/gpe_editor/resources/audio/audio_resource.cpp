/*
audio_resource.cpp
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

#include "audio_resource.h"
#include "../../editor/gpe_editor_settings.h"

audioResource::audioResource(pawgui::widget_resource_container * pFolder)
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
    musicVal = nullptr;
    soundVal = nullptr;
    audioEditorMainNote = new pawgui::widget_label_text ("MP3 file type not supported.","Please Note MP3 files are not supported or licensed with usage of the Game Pencil Engine.");
    audioGroupName = new pawgui::widget_input_text("","default=blank");
    audioGroupName->set_name("Audio Group");
    audioGroupName->set_label("Audio Group");
    audioType_buttonController = new pawgui::widget_radio_button_controller("Audio Type", true,1);
    audioType_buttonController->add_opton("Normal Audio");
    audioType_buttonController->add_opton("Voice");
    audioType_buttonController->add_opton("Background Music");
    if( saveResource_button!=nullptr)
    {
        audioType_buttonController->set_coords(-1,saveResource_button->get_ypos()+saveResource_button->get_height()+pawgui::padding_default);
    }
    openExternalEditor_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Use External Editor" );
    refreshResourceData_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png"," Refreshes this resource's media files");

    play_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Play" );
    preloadCheckBox = new pawgui::widget_checkbox("Preload Audio","Check to load audio at game open", true);
    volumeLabel = new pawgui::widget_label_text ("Volume","Default Volume Level");
    defaultVolume = new pawgui::widget_input_number("Range 0 to 100",true,0,200 );
    defaultVolume->set_string("100");
    defaultVolume->set_name("Default Volume:");
    defaultVolume->set_width(32);
    volumeSlider = new pawgui::widget_slide_xaxis(100,0,200);
    positionSlider = new pawgui::widget_slide_xaxis(100,0,200);
}

audioResource::~audioResource()
{
    if(audioEditorMainNote!=nullptr)
    {
        delete audioEditorMainNote;
        audioEditorMainNote = nullptr;
    }
    if(volumeLabel!=nullptr)
    {
        delete volumeLabel;
        volumeLabel = nullptr;
    }
    if(positionSlider!=nullptr)
    {
        delete positionSlider;
        positionSlider = nullptr;
    }
    if(volumeSlider!=nullptr)
    {
        delete volumeSlider;
        volumeSlider = nullptr;
    }

    if(musicVal!=nullptr)
    {
        delete musicVal;
        musicVal = nullptr;
        gpe::error_log->report("Removing Audio file...");
    }
    if(soundVal!=nullptr)
    {
        delete soundVal;
        soundVal = nullptr;
        gpe::error_log->report("Removing Audio file...");
    }

    if( audioType_buttonController!=nullptr)
    {
        delete audioType_buttonController;
        audioType_buttonController = nullptr;
    }
    if( defaultVolume!=nullptr)
    {
        delete defaultVolume;
        defaultVolume = nullptr;
    }
    if( audioGroupName!=nullptr)
    {
        delete audioGroupName;
        audioGroupName = nullptr;
    }
    if( audioGroupName!=nullptr)
    {
        delete audioGroupName;
        audioGroupName = nullptr;
    }
    if( play_button!=nullptr)
    {
        delete play_button;
        play_button = nullptr;
    }
    if( refreshResourceData_button!=nullptr)
    {
        delete refreshResourceData_button;
        refreshResourceData_button = nullptr;
    }
}


bool audioResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void audioResource::compile_cpp()
{

}

bool audioResource::copy_audio_source(std::string directory_output_name)
{
    std::string copyDestinationStr = "";
    bool copyErrorFound = false;
    for( int i = 0; i < gpe::sound_format_max ; i++)
    {
        if((int)audioFileName[i].size() > 0)
        {
            copyDestinationStr = directory_output_name+"/"+ stg_ex::get_short_filename(audioFileName[i],true);
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
        play_button->set_name("Play");
        play_button->descriptionText = "Push to Play Audio";
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
            pawgui::display_user_alert("[Alert - "+resource_name+"]","Unsupported file type added.");
        }
        if( loadableAudio )
        {
            if( soundVal!=nullptr)
            {
                soundVal->load( new_file_name );
            }
            else
            {
                soundVal = gpe::standard_sound_object->create_new( resource_name, new_file_name );
            }
            if( soundVal==nullptr)
            {
                pawgui::update_action_message("Failed to Load Audio file "+new_file_name+"." );
            }
        }
        if( isAudioFile)
        {
            copy_audio_source( stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/audio" );
        }
    }
    else
    {
        pawgui::display_user_alert("Unable to load audio","File type["+ stg_ex::get_file_ext(new_file_name)+"] not supported when loading ["+new_file_name+"].");
    }
}

void audioResource::load_resource(std::string file_path )
{

    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Processing Audio", resource_name );
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
                    equalPos = currLineToBeProcessed.find_first_of("=");
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
                    else if( key_string=="Audiofile_location")
                    {
                        load_audio( soughtDir+valstring );
                    }
                    else if( key_string=="Preload")
                    {
                        isPreloaded = stg_ex::string_to_bool(valstring);
                        preloadCheckBox->set_checked(isPreloaded );
                    }
                    else if( key_string=="AudioGroup")
                    {
                        audioGroupName->set_string(valstring);
                    }
                    else if( key_string=="AudioType")
                    {
                        audioType = stg_ex::string_to_int(valstring,0);
                        audioType_buttonController->set_selection(audioType);
                    }
                    else if( key_string=="DefaultVolume")
                    {
                        defaultVolume->set_number( stg_ex::string_to_int(valstring,100));
                    }
                    else
                    {
                        for(int i = 0; i < gpe::sound_format_max; i++)
                        {
                            if( key_string=="AudioFile["+ gpe::sound_type_names[i]+"]")
                            {
                                //to at least include if a file type is available in string
                                if( (int)valstring.size() > 3 )
                                {
                                    load_audio( soughtDir+valstring );
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
    if( play_button!=nullptr)
    {
        play_button->prerender_self( );
    }
    if( openExternalEditor_button!=nullptr)
    {
        openExternalEditor_button->prerender_self( );
    }
    if( preloadCheckBox!=nullptr)
    {
        preloadCheckBox->prerender_self( );
    }
    if( audioType_buttonController!=nullptr)
    {
        audioType_buttonController->prerender_self( );
    }

}

void audioResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if(cam!=nullptr && view_space!=nullptr )
    {
        int prevVolumeSliderVal = volumeSlider->get_value();
        int prevVolumeTypedVal = defaultVolume->get_held_number();

        if( panel_main_editor!=nullptr )
        {
            panel_main_editor->clear_panel();

            panel_main_editor->add_gui_element(audioEditorMainNote,true);
            panel_main_editor->add_gui_element(renameBox,true);
            panel_main_editor->add_gui_element(audioGroupName, true);

            panel_main_editor->add_gui_auto(refreshResourceData_button);
            panel_main_editor->add_gui_auto(loadResource_button);
            panel_main_editor->add_gui_auto(play_button);
            panel_main_editor->add_gui_auto(exportResource_button);
            panel_main_editor->add_gui_element(openExternalEditor_button,true);

            panel_main_editor->add_gui_element(audioType_buttonController,true);

            panel_main_editor->add_gui_element(volumeLabel, true);
            panel_main_editor->add_gui_element(volumeSlider, false);
            panel_main_editor->add_gui_element(defaultVolume,true);

            panel_main_editor->add_gui_auto(confirmResource_button);
            panel_main_editor->add_gui_auto(cancelResource_button);
            panel_main_editor->process_self( nullptr, nullptr );

            if( confirmResource_button!=nullptr && confirmResource_button->is_clicked() )
            {
                save_resource();
            }
            else if( cancelResource_button->is_clicked() )
            {
                resourcePostProcessed = false;
                load_resource();
            }
            if(audioType_buttonController!=nullptr)
            {
                audioType = audioType_buttonController->get_selected_id();
            }
            //plays or pauses the sound
            if( play_button!=nullptr)
            {
                if( play_button->is_clicked() )
                {
                    if( play_button->get_name()=="Play")
                    {
                        if( soundVal!=nullptr)
                        {
                            soundVal->play();
                            play_button->set_name("Stop");
                            play_button->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/stop.png" );
                            play_button->descriptionText = "Push to Stop Audio";
                        }
                    }
                    else if( play_button->get_name()=="Stop")
                    {
                        if( soundVal != nullptr )
                        {
                            soundVal->stop();
                        }
                        play_button->set_name("Play");
                        play_button->descriptionText = "Push to Play Audio";
                        play_button->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );

                    }
                    play_button->set_width(loadResource_button->get_width() );
                }
                else if( play_button->get_name()=="Stop")
                {
                    /*if( audio_is_play( )== false )
                    {
                        play_button->set_name("Play");
                        play_button->descriptionText = "Push to Play Audio";
                        play_button->change_texture( app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
                    }*/
                }
            }
            if( loadResource_button!=nullptr)
            {
                if( loadResource_button->is_clicked() )
                {
                    std::string new_file_name = pawgui::get_filename_open_from_popup("Load In an Audio File","Audio",pawgui::main_settings->fileOpenAudioDir);
                    if( (int)new_file_name.size() > 3)
                    {
                        load_audio(new_file_name);
                    }
                }
            }
            if( openExternalEditor_button!=nullptr)
            {
                if( openExternalEditor_button->is_clicked() )
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
                        pawgui::context_menu_open();
                        pawgui::main_context_menu->set_width(openExternalEditor_button->get_width() );
                        for( ii = 0; ii < gpe::sound_format_max; ii++)
                        {
                            if( (int)audioFileName[ii].size() > 0)
                            {
                                pawgui::main_context_menu->add_menu_option("Edit "+ gpe::sound_type_names[ii],ii,nullptr,-1,nullptr,true,true);
                            }
                        }
                        int menuSelection = pawgui::context_menu_process();
                        if( menuSelection >=0 && menuSelection < gpe::sound_format_max)
                        {
                            std::string fileToEdit = audioFileName[menuSelection];

                            if( main_editor_settings!=nullptr && main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]!=nullptr)
                            {
                                gpe::external_open_program(main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_AUD]->get_string(),fileToEdit, true );
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

            if( refreshResourceData_button!=nullptr )
            {
                if( refreshResourceData_button->is_clicked() )
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

        positionSlider->set_coords( view_space->w/2, view_space->h - 64 - pawgui::padding_default  );
        positionSlider->set_width( view_space->w/2 - 64 - pawgui::padding_default );
        positionSlider->set_height( 64 );
        if( panel_main_editor->hasArrowkeyControl== false && panel_main_editor->hasScrollControl == false )
        {
            if( get_mouse_coords(view_space,cam) )
            {
                positionSlider->process_self( view_space, cam );
            }
        }
    }
}

void audioResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if(cam!=nullptr && view_space!=nullptr)
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
        int required_height = current_max_types *( pawgui::default_line_height+pawgui::padding_default );

        if( view_space->h >= required_height )
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

                if( view_space->h > 720 )
                {
                    gpe::gfs->render_text( pawgui::padding_default,positionSlider->get_ypos() - 32 - i_sound_type*(pawgui::padding_default+pawgui::default_line_height), gpe::sound_type_names[i_sound_type]+" "+support_string,pawgui::theme_main->main_box_font_highlight_color, gpe::font_default, gpe::fa_left, gpe::fa_bottom);

                    if( audioFileName[i_sound_type].size()> 0)
                    {
                        if( gpe::sound_is_format_supported[i_sound_type] )
                        {
                            gpe::gfs->render_text( view_space->w-pawgui::padding_default*3,i_sound_type *(pawgui::padding_default+pawgui::default_line_height), stg_ex::get_local_from_global_file( audioFileName[i_sound_type]  )+ " " + gpe::sound_type_names[i_sound_type]+" attatched ", pawgui::theme_main->input_selected_color, gpe::font_default, gpe::fa_right, gpe::fa_top);
                        }
                        else
                        {
                            gpe::gfs->render_text( view_space->w-pawgui::padding_default*3,i_sound_type *(pawgui::padding_default+pawgui::default_line_height), stg_ex::get_local_from_global_file( audioFileName[i_sound_type] ) + " " + gpe::sound_type_names[i_sound_type]+" attatched ", pawgui::theme_main->input_error_Box_color, gpe::font_default, gpe::fa_right, gpe::fa_top);
                        }
                    }
                    else
                    {
                        gpe::gfs->render_text( view_space->w-pawgui::padding_default*3,i_sound_type*(pawgui::padding_default+pawgui::default_line_height), gpe::sound_type_names[i_sound_type]+" not attatched ",pawgui::theme_main->input_error_Box_color, gpe::font_default,gpe::fa_right,gpe::fa_top);
                    }
                }

            }
        }
        if( gpe::sound_is_working )
        {
            gpe::gfs->render_text( pawgui::padding_default,pawgui::padding_default, gpe::sound_system_name,pawgui::theme_main->input_color, gpe::font_default, gpe::fa_left, gpe::fa_top );
        }
        else
        {
            gpe::gfs->render_text( pawgui::padding_default,pawgui::padding_default, gpe::sound_system_name,pawgui::theme_main->input_error_Box_color, gpe::font_default, gpe::fa_left, gpe::fa_top );
        }

        std::string sound_string_length = "";
        if( soundVal != nullptr )
        {
            if( view_space->h > 720 )
            {
                gpe::gfs->render_text( view_space->w/2,view_space->h/2, "Sound size: "+stg_ex::int_to_string( soundVal->get_length() )+" ms",pawgui::theme_main->main_box_font_highlight_color, gpe::font_default, gpe::fa_right, gpe::fa_bottom);
                gpe::gfs->render_text( view_space->w/2,view_space->h/2+32, "Sound size: "+stg_ex::int_to_string( soundVal->get_length_seconds() )+" seconds",pawgui::theme_main->main_box_font_highlight_color, gpe::font_default, gpe::fa_right, gpe::fa_bottom);
                gpe::gfs->render_text( view_space->w/2,view_space->h/2+64, "Sound size: "+stg_ex::int_to_string( soundVal->get_length_minutes() )+" minutes",pawgui::theme_main->main_box_font_highlight_color, gpe::font_default, gpe::fa_right, gpe::fa_bottom);
                gpe::gfs->render_text( view_space->w/2,view_space->h/2+96, "Sound size: "+stg_ex::int_to_string( soundVal->get_length_hours() )+" hours",pawgui::theme_main->main_box_font_highlight_color, gpe::font_default, gpe::fa_right, gpe::fa_bottom);
                gpe::gfs->render_text( view_space->w/2,view_space->h/2+128, "Sound size: "+stg_ex::int_to_string( soundVal->get_length_days() )+" days",pawgui::theme_main->main_box_font_highlight_color, gpe::font_default, gpe::fa_right, gpe::fa_bottom);
            }


            if( soundVal->get_length() > 0)
            {
                if( soundVal->get_length_days() > 0 )
                {
                    sound_string_length = stg_ex::int_to_string( soundVal->get_length_days() )+
                    ":"+stg_ex::int_to_string( soundVal->get_length_hours() )+
                    ":"+stg_ex::int_to_string( soundVal->get_length_minutes() )+
                    ":"+stg_ex::int_to_string( soundVal->get_length_seconds() )+
                    ":"+stg_ex::int_to_string( soundVal->get_length_ms() );
                }
                else if( soundVal->get_length_hours() > 0)
                {
                    sound_string_length = stg_ex::int_to_string( soundVal->get_length_hours() )+":"+stg_ex::int_to_string( soundVal->get_length_minutes() )+":"+stg_ex::int_to_string( soundVal->get_length_seconds() )+":"+stg_ex::int_to_string( soundVal->get_length_ms() );
                }
                else
                {
                    sound_string_length = stg_ex::int_to_string( soundVal->get_length_minutes() )+":"+stg_ex::int_to_string( soundVal->get_length_seconds() )+":"+stg_ex::int_to_string( soundVal->get_length_ms() );
                }
            }
            else
            {
                sound_string_length = "0 ms";
            }
        }
        else
        {
            sound_string_length = "Empty";
        }
        gpe::gfs->render_text( pawgui::padding_default, view_space->h - pawgui::padding_default-32, sound_string_length ,pawgui::theme_main->main_box_font_highlight_color, gpe::font_default, gpe::fa_left, gpe::fa_bottom);
        positionSlider->render_self( view_space, cam );
    }
}

void audioResource::save_resource(std::string file_path, int backupId)
{
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Saving Audio", resource_name );
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
            std::string resfile_location = "";
            std::string resFileCopySrc;
            std::string resFileCopyDest;
            for(int i = 0; i < gpe::sound_format_max; i++)
            {
                if( (int)audioFileName[i].size() > 3)
                {
                    resfile_location = stg_ex::get_short_filename (audioFileName[i],true );
                    newSaveDataFile << "AudioFile["+ gpe::sound_type_names[i]+"]="+resfile_location+"\n";
                    if( (int)resfile_location.size() > 0 && usingAltSaveSource )
                    {
                        resFileCopySrc = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/audio/"+resfile_location;
                        resFileCopyDest = soughtDir+resfile_location;
                        if( sff_ex::file_exists(resFileCopyDest) )
                        {
                            /*
                            if( pawgui::display_prompt_message("[WARNING]Audio File Already exists?","Are you sure you will like to overwrite your ["+resfile_location+"] audio file? This action is irreversible!")==pawgui::display_query_yes)
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
            if( preloadCheckBox!=nullptr)
            {
                newSaveDataFile << "Preload="+ stg_ex::int_to_string(preloadCheckBox->is_clicked() )+"\n";
            }
            if( defaultVolume!=nullptr)
            {
                newSaveDataFile << "DefaultVolume="+ stg_ex::int_to_string(defaultVolume->get_held_number() )+"\n";
            }
            if( audioGroupName!=nullptr)
            {
                newSaveDataFile << "AudioGroup="+audioGroupName->get_string()+"\n";

            }
            if( audioType_buttonController!=nullptr)
            {
                newSaveDataFile << "AudioType="+ stg_ex::int_to_string(audioType_buttonController->get_selected_id() )+"\n";
            }

            newSaveDataFile.close();
            if( !usingAltSaveSource)
            {
                isModified = false;
            }
        }
        else
        {
            main_editor_log->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        main_editor_log->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

bool audioResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "audio=" << resource_name << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}
