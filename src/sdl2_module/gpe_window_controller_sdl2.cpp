/*
gpe_window_controller_sdl2s.cpp
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

//The headers

#include "gpe_window_controller_sdl2.h"


namespace gpe
{
    window_controller_sdl2 * window_controller_main_sdl2 = NULL;

    bool init_sdl2_window_system()
    {
        error_log->report("Starting SDL2 Window system...");
        if( window_controller_main != NULL )
        {
            delete window_controller_main;
            window_controller_main = NULL;
        }
        window_controller_main_sdl2 = new window_controller_sdl2( settings->programTitle,settings->defaultWindowWidth, settings->defaultWindowHeight,  settings->showWindowBorder, settings->startFullScreen, settings->startMaximized, settings->mainWindowIsResizable );
        window_controller_main = window_controller_main_sdl2;
        error_log->report("Window Controller swapped from base to sdl class...");
        return true;
    }

    void quit_sdl2_window_system()
    {
        if( window_controller_main_sdl2 != NULL )
        {
            delete window_controller_main_sdl2;
            window_controller_main_sdl2 = NULL;
        }
        window_controller_main = NULL;
    }

    window_controller_sdl2::window_controller_sdl2(std::string windowTitle, int wWidth, int wHeight,bool showBorder, bool fullScreen, bool maximized, bool isResizable )
    {
        window_base_renderer = NULL;
        window_id = -1;
        window_closed = false;
        window_has_mouse= false;
        window_has_focus = false;
        window_width = wWidth;
        window_height = wHeight;
        resized = false;
        minimized = false;
        position_x = 0;
        position_y = 0;
        //Set up the window and render area

        window_flags = SDL_WINDOW_SHOWN;
        if( isResizable )
        {
            window_flags = window_flags | SDL_WINDOW_RESIZABLE;
        }

        if( !showBorder)
        {
            window_flags = window_flags | SDL_WINDOW_BORDERLESS;
        }
        if(fullScreen)
        {
            window_flags = window_flags | SDL_WINDOW_FULLSCREEN;
        }
        else if( maximized )
        {
            window_flags = window_flags | SDL_WINDOW_MAXIMIZED;
        }

        local_sdl2_window = SDL_CreateWindow(windowTitle.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,wWidth,wHeight,window_flags  );
        std::string window_iconFName = "icon.png";
        window_icon = sdl2_surface_ex::load_surface_image( window_iconFName.c_str() );
        SDL_SetWindowIcon( local_sdl2_window, window_icon);
        SDL_SetWindowMinimumSize( local_sdl2_window, settings->minWindowWidth, settings->minWindowHeight );

        SDL_SetWindowResizable( local_sdl2_window, SDL_FALSE );
        window_resizing_enabled = false;
        //Set window flag
        windowed = true;
        window_id = SDL_GetWindowID( local_sdl2_window );

        if( local_sdl2_window!=NULL)
        {
            SDL_GetWindowPosition(local_sdl2_window,&position_x, &position_y);
            SDL_GetWindowSize( local_sdl2_window, &window_width, &window_height );
        }
        else
        {
            window_error_occurred = false;
            return;
        }
    }

    window_controller_sdl2::~window_controller_sdl2()
    {

    }

    bool window_controller_sdl2::disable_scaling()
    {
        window_scaling = window_base_renderer->disable_scaling();
        if( !window_scaling )
        {
            gpe::screen_width = window_width;
            gpe::screen_height = window_height;
        }

        return true;
    }

    bool window_controller_sdl2::enable_scaling()
    {

        return true;
    }

     bool window_controller_sdl2::disable_window_resize()
    {
        if( local_sdl2_window == NULL )
        {
            return false;
        }
        SDL_SetWindowResizable( local_sdl2_window, SDL_FALSE );
        window_resizing_enabled = false;
        return true;
    }

    bool window_controller_sdl2::enable_window_resize()
    {
        if( local_sdl2_window == NULL )
        {
            return false;
        }

        SDL_SetWindowBordered( local_sdl2_window, SDL_TRUE );
        SDL_SetWindowResizable( local_sdl2_window, SDL_TRUE );
        window_resizing_enabled = true;
        return true;
    }

    SDL_Window * window_controller_sdl2::get_sdl2_window()
    {
        return local_sdl2_window;
    }

    bool window_controller_sdl2::hide_window()
    {
        if( local_sdl2_window == NULL )
        {
            return false;
        }
        SDL_HideWindow( local_sdl2_window );
        return true;
    }

    bool window_controller_sdl2::minimize_window()
    {
        if( local_sdl2_window == NULL )
        {
            return false;
        }
        SDL_MinimizeWindow( local_sdl2_window );
        return true;
    }

    bool window_controller_sdl2::maximize_window()
    {
        if( local_sdl2_window == NULL )
        {
            return false;
        }
        SDL_MaximizeWindow( local_sdl2_window );
        return true;
    }

    void window_controller_sdl2::process_event( input_event_container * event_holder )
    {
        resized = false;

        if( event_holder == NULL )
        {
            error_log->report("Event holder = null!");
            return;
        }

        if( event_holder->window_id != window_id )
        {
            return;
        }

        switch( event_holder->event_type )
        {
            //Get new dimensions and repaint on window size change
            case input_event_type::window_resized_event:
                if( minimized)
                {
                    //SDL_RestoreWindow(gpeWindow);
                    SDL_SetWindowSize(local_sdl2_window,window_width,window_height);
                    minimized = false;
                    gpe::error_log->report("Window unminimized");
                }
                else
                {
                    if( event_holder->window_resize_w >0 && event_holder->window_resize_h > 0)
                    {
                        window_width = event_holder->window_resize_w;
                        window_height = event_holder->window_resize_h;
                        minimized = false;
                        resize_window();
                        gpe::error_log->report("Window resized to ["+ stg_ex::int_to_string(window_width)+"X "+ stg_ex::int_to_string(window_height)+"]." );
                    }
                    else
                    {
                        window_width = event_holder->window_resize_w;
                        window_height = event_holder->window_resize_h;
                        resize_window();
                        gpe::error_log->report("Window resized with improper data");
                    }
                }
                window_base_renderer->update_renderer( minimized );
                resized= true;
            break;


            case input_event_type::window_minimized_event:
                //SDL_SetWindowFullscreen( gpeWindow, window_flags);
                windowed = true;
                minimized = true;
                resized = true;
                window_width = 0;
                window_height = 0;
                resized = true;
                gpe::error_log->report("Window minimized");
            break;

            case input_event_type::window_entered_event:
                window_has_mouse= true;
                minimized = false;
            break;

            case input_event_type::window_leave_event:
                window_has_mouse = false;
                minimized = false;
            break;

            case input_event_type::window_focus_gained_event:
                window_has_focus = true;
                minimized = false;
            break;

            case input_event_type::window_focus_lost_event:
                window_has_focus= false;
                minimized = false;
            break;

            case input_event_type::window_exposed_event:
                if( minimized )
                {
                    minimized = false;
                    resized = false;
                    SDL_RestoreWindow(local_sdl2_window);
                    SDL_MaximizeWindow(local_sdl2_window);
                    SDL_GetWindowSize(local_sdl2_window,&window_width,&window_height);
                    //SDL_RaiseWindow(local_sdl2_window);
                    //resize_window();
                    gpe::error_log->report("Window unminimized from being exposed!");
                }
                else
                {
                    //gpe::error_log->report("Window exposed!!!!");
                }
                window_base_renderer->update_renderer( minimized );
            break;

            case input_event_type::window_hidden_event:
                gpe::error_log->report("Window hidden!!!!");
                minimized = true;
            break;

            case input_event_type::window_closed_event:
                window_closed = true;
            break;

            case input_event_type::window_shown_event:
                minimized = false;
                SDL_GetWindowSize(local_sdl2_window,&window_width,&window_height);
                if( minimized)
                {
                    minimized = false;
                    gpe::error_log->report("Window restored and unminimized");
                }
                else
                {
                    gpe::error_log->report("Window restored.");
                }
                resized = true;
                window_base_renderer->update_renderer( minimized );
            break;

            case input_event_type::window_maximized_event:
                minimized = false;
            break;

            default:
                //gpe::error_log->report("Default Window event happened."+ stg_ex::int_to_string(e.window.event ) );
                minimized =false;
            break;
        }
    }


    void window_controller_sdl2::reset_input()
    {
        //window_base_renderer::reset_input();
        resized = false;
        window_closed = false;
        if( local_sdl2_window!=NULL && window_base_renderer!=NULL)
        {
            SDL_GetWindowSize( local_sdl2_window, &window_width, &window_height );
            window_base_renderer->reset_input();
        }
    }

    void window_controller_sdl2::resize_window()
    {
        if( window_base_renderer!=NULL )
        {
            window_base_renderer->resize_renderer( window_width, window_height );
        }
        if( !window_scaling )
        {
            gpe::screen_width = window_width;
            gpe::screen_height = window_height;
        }
        minimized = false;
    }

    bool window_controller_sdl2::scale_window( int s_width, int s_height , bool scale_int )
    {
        if( window_base_renderer !=NULL )
        {
            window_scaling = window_base_renderer->scale_renderer(s_width, s_height, scale_int );

            if( window_scaling )
            {
                gpe::screen_width = s_width;
                gpe::screen_height = s_height;
                previously_scaled = true;
            }
            else
            {
                gpe::screen_width = window_width;
                gpe::screen_height = window_height;
            }
            return window_scaling;
        }
        return false;
    }

    bool window_controller_sdl2::scale_window_factor( float s_width, float s_height, bool scale_int )
    {
        if( window_base_renderer !=NULL )
        {
            window_scaling = window_base_renderer->scale_renderer_factor(s_width, s_height, scale_int );
            return window_scaling;
        }
        return false;
    }

    void window_controller_sdl2::set_renderer( renderer_base * new_renderer, bool remove_current )
    {
        if( remove_current && window_base_renderer!=NULL )
        {
            delete window_base_renderer;
            window_base_renderer = NULL;
        }
        if( new_renderer == NULL )
        {
            error_log->report(" new_renderer = null in [void window_controller_sdl2::set_renderer()] function!");
            return;
        }
        window_base_renderer = new_renderer;
        new_renderer->resize_renderer( window_width, window_height );
    }

    void window_controller_sdl2::set_vysnc( bool vs_on )
    {
        vsync_is_on = vs_on;

        if( vsync_is_on )
        {
            SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
        }
        else
        {
            SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
        }
    }

    void window_controller_sdl2::set_window_position( int new_x, int new_y )
    {
        if( local_sdl2_window != NULL )
        {
            if( new_x < 0 )
            {
                new_x = SDL_WINDOWPOS_CENTERED;
            }

            if( new_y < 0 )
            {
                new_y = SDL_WINDOWPOS_CENTERED;
            }
            SDL_SetWindowPosition( local_sdl2_window, new_x, new_y );
        }
    }

    bool window_controller_sdl2::set_window_size( int n_width, int n_height )
    {
        if( n_width > 0 && n_height > 0 )
        {
            if( local_sdl2_window != NULL )
            {
                SDL_SetWindowSize( local_sdl2_window, n_width, n_height );
            }

            window_width = n_width;
            window_height = n_height;

            if( window_base_renderer !=NULL )
            {
                window_base_renderer->resize_renderer(n_width, n_height );
                window_width = gpe::screen_width = n_width;
                window_height = gpe::screen_height = n_height;
                return true;
            }
            return true;
        }
        return false;
    }

    void window_controller_sdl2::set_window_title(std::string new_title)
    {
        SDL_SetWindowTitle(local_sdl2_window, new_title.c_str() );
    }

    bool window_controller_sdl2::show_window()
    {
        if( local_sdl2_window == NULL )
        {
            return false;
        }
        SDL_ShowWindow( local_sdl2_window );
        return true;
    }


    void window_controller_sdl2::start_loop()
    {
        resized = false;
    }

    void window_controller_sdl2::toggle_fullscreen()
    {
        if( windowed )
        {
            SDL_SetWindowFullscreen( local_sdl2_window, SDL_WINDOW_FULLSCREEN_DESKTOP   );
            windowed = false;

        }
        else
        {
            SDL_SetWindowFullscreen( local_sdl2_window, window_flags);
            windowed = true;
        }
        SDL_GetWindowSize(local_sdl2_window,&window_width,&window_height);
        gpe::screen_width = window_width;
        gpe::screen_height = window_height;
        resized = true;
        gpe::error_log->report("Window Size: "+ stg_ex::int_to_string(window_width)+","+ stg_ex::int_to_string(window_height) );
        gpe::error_log->report("Window Flags: "+ stg_ex::int_to_string(window_flags) );
    }

    bool window_controller_sdl2::window_changed()
    {
        return resized;
    }
}
