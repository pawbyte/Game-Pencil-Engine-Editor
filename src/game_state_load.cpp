/*
game_state_load.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

#include "game_state_load.h"

game_loader::game_loader( std::string s_name )
{
    if( (int)s_name.size() > 0 )
    {
        state_name = s_name;
    }
    else
    {
        state_name = "game_loader";
    }
    show_load_percentage = true;
    resources_all_loaded = false;

    loading_began = false;

    load_title_current = "Loading Game Settings";
    load_message_current = "Please wait";
    load_subtitle_current = "...";
    //For now images take up the main amount of resources.
    //This will either be a full even split or actually dependent on amount of files per category

    c_ticks = 0;
    c_ticks_max = 1000;



    update_load_count = 0;
    update_load_max = 20;

    load_category_current = 0;
    load_subcategory_current = 0;
    loading_began = false;

    std::string mainGuiFontLocation = gpe::app_directory_name+"resources/fonts/dejavu_sans_mono/DejaVuSansMono.ttf";
    if( gpe::gfs!=NULL )
    {
        gpe::error_log->report("Loading default font...");
        gpe::font_default = gpe::gfs->open_font(mainGuiFontLocation,16,false, "font_default");
    }
    else
    {
        gpe::error_log->report("gfs is NULL...");
    }

    color_load_bar = gpe::c_lime->duplicate_color();
    color_load_bar_outline = gpe::c_gray->duplicate_color();
    color_load_text = gpe::c_ltgray->duplicate_color();

    text_load_position = 0;
    load_bar_position = 0;
    padding_text = 0;
    text_halign = gpe::fa_right;
    text_valign = gpe::fa_bottom;
    load_value_current = 0;
    load_value_seeked = 0;

    seeked_categories.push_back( "textures" );
    seeked_categories.push_back( "tilesheets" );
    seeked_categories.push_back( "audio" );
    seeked_categories.push_back( "particles" );
    seeked_categories.push_back( "lights" );

    error_occurred = false;
    if( gpe::settings != NULL )
    {
        gpe::key_pair * cCategorty  = NULL;
        for( int i = 0; i < (int)seeked_categories.size(); i++ )
        {
            cCategorty = gpe::settings->gameResourcesInstructions->find_option_named( seeked_categories[i] );
            if( cCategorty !=NULL )
            {
                load_title_current = cCategorty->key_string;
                load_value_seeked += (int)cCategorty->sub_options.size();
            }
            else
            {
                gpe::error_log->report("Error: Engine _settings group ["+seeked_categories[i]+"] is currently NULL!" );
            }
        }
    }
    else
    {
        gpe::error_log->report("Error: Global variable [settings] is currently NULL!" );
        error_message = "settings not defined!";
        error_occurred = true;
    }
}

game_loader::~game_loader()
{
    clean_up();
}

void game_loader::apply_logic()
{
    if( gpe::settings == NULL )
    {
        error_occurred = true;
        error_message = "settings not defined!";
    }
    else
    {
        gpe::key_pair * mPair = NULL;
        gpe::key_pair * cPair = NULL;
        int j = 0;
        int maxResourceTypeSize = (int)gpe::settings->gameResourcesInstructions->sub_options.size();
        for( int i = load_category_current; i < maxResourceTypeSize; i++ )
        {
            mPair = gpe::settings->gameResourcesInstructions->sub_options[i];
            load_title_current = "Loading " + mPair->key_string + "s";
            if( mPair !=NULL )
            {
                process_keypair( mPair );
            }
        }
    }

    if( load_value_seeked >= load_value_seeked )
    {
        resources_all_loaded = true;
    }

    if( resources_all_loaded )
    {
        if( gpe::input->any_key_pressed() ||  gpe::font_default == NULL )
        {
            gpe::game_runtime->state_set( state_next_name);
        }
    }

    if( gpe::window_controller_main->window_closed || gpe::input->exit_requested  )
    {
        gpe::game_runtime->state_set( "exit" );
    }
}

void game_loader::clean_up()
{

}

void game_loader::end_state()
{

}

bool game_loader::keypair_is_seeked( gpe::key_pair * pairToProcess )
{
    if( pairToProcess!=NULL )
    {
        for( int i = (int)seeked_categories.size(); i >=0; i--)
        {
            if( pairToProcess->key_string == seeked_categories[i] )
            {
                return true;
            }
        }
    }
    return false;
}

void game_loader::process_input()
{
    if( gpe::window_controller_main->window_closed || gpe::input->exit_requested  )
    {
        gpe::game_runtime->state_set( "exit" );
    }

    if( gpe::input->check_kb_released( kb_f) )
    {
        gpe::error_log->report("Default font type: "+ gpe::font_default->get_font_type() );
    }
}

void game_loader::process_keypair( gpe::key_pair * pairToProcess )
{
    if( pairToProcess ==NULL )
    {
        return;
    }

}

void game_loader::render()
{
    gpe::program_state::render();

    gpe::gcanvas->render_rectangle( 0, 0, gpe::screen_width, gpe::screen_height, gpe::c_blgray, false, 255 );
    gpe::gcanvas->render_rectangle( 0, gpe::screen_height - 64, gpe::screen_width, gpe::screen_height - 32, color_load_bar, false, 255 );
    gpe::gcanvas->render_rectangle( 0, gpe::screen_height - 64, gpe::screen_width, gpe::screen_height - 32, color_load_bar_outline, true, 255 );

    if( gpe::font_default!=NULL )
    {
        if( resources_all_loaded )
        {
            gpe::gfs->render_text(gpe::screen_width/2, gpe::screen_height/2, "Resources all loaded, Press ANYKEY to START", color_load_text, gpe::font_default,  gpe::fa_center, gpe::fa_middle, 255 );
            return;
        }
        gpe::gfs->render_text(gpe::screen_width/2, gpe::screen_height/2, load_title_current, color_load_text, gpe::font_default,  gpe::fa_center, gpe::fa_middle, 255 );
        gpe::gfs->render_text(gpe::screen_width/2, gpe::screen_height/2 + ( padding_text + gpe::font_default->get_mono_height() ) *2, load_subtitle_current, color_load_text, gpe::font_default,  gpe::fa_center, gpe::fa_middle, 255 );
        gpe::gfs->render_text(gpe::screen_width/2, gpe::screen_height/2 + ( padding_text + gpe::font_default->get_mono_height() )* 4, load_message_current, color_load_text, gpe::font_default,  gpe::fa_center, gpe::fa_middle, 255 );

        if( show_load_percentage )
        {
            float loadPercentage = 0;
            if( load_value_seeked != 0 )
            {
                loadPercentage = load_value_current /load_value_seeked;
                loadPercentage = 100.f * loadPercentage;
            }
            gpe::gfs->render_text( gpe::screen_width/2, gpe::screen_height - 48, stg_ex::int_to_string( loadPercentage )+ "%", color_load_text, gpe::font_default,  gpe::fa_center, gpe::fa_middle, 255 );
        }
    }
    else
    {
        gpe::error_log->report("Default font = null");
    }
}

void game_loader::start_state()
{

}

