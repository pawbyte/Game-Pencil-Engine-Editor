/*
game_state_splash.cpp
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

#include "game_state_splash.h"

splash_logo::splash_logo( std::string name , std::string logo_file_location, std::string t_text, std::string b_text )
{
    logo_name = name;
    always_display_logo = false;
    always_display_text = false;
    current_splash_state = splash_logo_state::state_enter;
    logo_texture = NULL;
    text_top = "";
    text_bottom = "";
    text_top_color = new gpe::color("splash_top", 228, 228, 228, 255 );
    text_bottom_color = new gpe::color("splash_bottom", 228, 228, 228, 255 );
    audio_effect_display = NULL;
    audio_effect_enter = NULL;
    audio_effect_exit = NULL;
    ms_displayed = 2000;
    ms_displayed_counter= 0;
    ms_entered = 500;
    ms_entered_counter = 0;
    ms_exit = 500;
    ms_entered_counter = 0;
}

splash_logo::~splash_logo()
{
    if( text_top_color != NULL )
    {
        delete text_top_color;
        text_top_color = NULL;
    }

    if( text_bottom_color != NULL )
    {
        delete text_bottom_color;
        text_bottom_color = NULL;
    }

    if( audio_effect_enter != NULL )
    {
        delete audio_effect_enter;
        audio_effect_enter = NULL;
    }

    if( audio_effect_display != NULL )
    {
        delete audio_effect_display;
        audio_effect_display = NULL;
    }

    if( audio_effect_exit != NULL )
    {
        delete audio_effect_exit;
        audio_effect_exit = NULL;
    }
}

std::string splash_logo::get_name()
{
    return logo_name;
}

void splash_logo::process_self(float delta_time )
{

}

void splash_logo::render()
{
    switch( current_splash_state )
    {
        case splash_logo_state::state_enter:

        break;

        case splash_logo_state::state_display:

        break;

        case splash_logo_state::state_exit :

        break;

        default:

        break;

    }
}

splash_screen::splash_screen( std::string s_name )
{
    if( (int)s_name.size() > 0 )
    {
        state_name = s_name;
    }
    else
    {
        state_name = "gpe_splash_screen";
    }
    current_position = 0;
}

splash_screen::~splash_screen()
{

}

void splash_screen::apply_logic()
{
    splash_logo * current_logo = NULL;
    int splash_logos_count = (int)logos.size();

    if( current_position >=0 && current_position < splash_logos_count )
    {
        current_logo = logos[current_position];

        switch( current_logo->current_splash_state )
        {
            case splash_logo_state::state_enter:
                current_logo->ms_entered_counter+=  gpe::time_keeper->get_delta_ticks();
                if( current_logo->ms_entered_counter >= current_logo->ms_entered )
                {
                    current_logo->ms_entered_counter = 0;
                    current_logo->current_splash_state = splash_logo_state::state_display;
                }
            break;

            case splash_logo_state::state_display:
                current_logo->ms_displayed_counter+=  gpe::time_keeper->get_delta_ticks();
                if( current_logo->ms_displayed_counter >= current_logo->ms_displayed )
                {
                    current_logo->ms_displayed_counter = 0;
                    current_logo->current_splash_state = splash_logo_state::state_exit;
                }
            break;

            case splash_logo_state::state_exit:
                current_logo->ms_exit_counter+=  gpe::time_keeper->get_delta_ticks();
                if( current_logo->ms_exit_counter >= current_logo->ms_entered )
                {
                    current_logo->ms_exit_counter = 0;
                    current_logo->current_splash_state = splash_logo_state::state_enter;
                    current_position++;
                }

            break;

            default:
                current_position++;
            break;
        }
    }
    else
    {
        gpe::game_runtime->state_set( state_next_name );
    }

    if( gpe::window_controller_main->window_closed || gpe::input->exit_requested  )
    {
        gpe::game_runtime->state_set( "exit" );
    }
}

void splash_screen::clean_up()
{
}


void splash_screen::end_state()
{
    current_position = 0;
}

void splash_screen::process_input()
{

}

void splash_screen::remove_all_logos( )
{

}

void splash_screen::remove_splash_logo( std::string logo_name )
{

}

void splash_screen::render()
{
    gpe::gcanvas->render_rectangle( 0, 0, gpe::screen_width, gpe::screen_height, gpe::c_black, false, 255 );

    splash_logo * current_logo = NULL;
    int splash_logos_count = (int)logos.size();

    if( current_position >=0 && current_position < splash_logos_count )
    {

    }
    gpe::gfs->render_fps( gpe::font_default, gpe::c_white, gpe::screen_width, 32 );
}

void splash_screen::start_state()
{
    gpe::window_controller_main->set_window_title("...");
    current_position = 0;
}

