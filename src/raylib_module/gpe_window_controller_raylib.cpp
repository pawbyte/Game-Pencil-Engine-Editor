/*
gpe_window_controller_raylib.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_window_controller_raylib.h"


namespace gpe
{
    window_controller_raylib * window_controller_main_raylib = nullptr;

    bool init_raylib_window_system()
    {
        error_log->report("Starting raylib_module window system...");
        if( window_controller_main != nullptr )
        {
            delete window_controller_main;
            window_controller_main = nullptr;
        }
        window_controller_main_raylib = new window_controller_raylib( settings->programTitle,settings->defaultWindowWidth, settings->defaultWindowHeight,  settings->showWindowBorder, settings->startFullScreen, settings->startMaximized, settings->mainWindowIsResizable );
        window_controller_main = window_controller_main_raylib;
        error_log->report("Window Controller swapped from base to raylib class...");
        return true;
    }

    void quit_raylib_window_system()
    {
        if( window_controller_main_raylib != nullptr )
        {
            delete window_controller_main_raylib;
            window_controller_main_raylib = nullptr;
        }
        window_controller_main = nullptr;
    }

    window_controller_raylib::window_controller_raylib(std::string windowTitle, int wWidth, int wHeight,bool showBorder, bool fullScreen, bool maximized, bool isResizable )
    {
        if( window_base_renderer!=nullptr )
        {
            delete window_base_renderer;
        }
        window_base_renderer = nullptr;
        window_id = 0;
        window_closed = false;
        window_has_mouse= false;
        window_has_focus = false;
        window_scaling = false;

        if( wWidth < 0 )
        {
            wWidth = 320;
        }

        if( wHeight < 0 )
        {
            wHeight = 240;
        }
        window_width = wWidth;
        window_height = wHeight;
        resized = false;
        minimized = false;

        //Set up the window and render area
        InitWindow(window_width, window_height, windowTitle.c_str() );
        int current_window_monitor =  0; //GetCurrentMonitor();
        position_x = GetMonitorWidth( current_window_monitor )/2;
        position_y = GetMonitorHeight( current_window_monitor )/2;

        if( isResizable )
        {
           SetWindowState(FLAG_WINDOW_RESIZABLE);
        }

        if( !showBorder)
        {
            SetWindowState( FLAG_WINDOW_UNDECORATED );
        }

        if(fullScreen)
        {
            SetWindowState( FLAG_FULLSCREEN_MODE );
        }
        else if( maximized )
        {
            SetWindowState( FLAG_WINDOW_MAXIMIZED );
        }

        window_icon = LoadImage("icon.png");
        SetWindowIcon( window_icon );

        SetWindowMinSize( settings->minWindowWidth, settings->minWindowHeight );

        //Set window flag
        windowed = true;
        window_id = 0;

        window_width = GetScreenWidth();
        window_height = GetScreenHeight();
        SetWindowPosition( position_x - window_width/2, position_y - window_height/2);

    }

    window_controller_raylib::~window_controller_raylib()
    {
        UnloadImage( window_icon ); //Unloads the window icon if one was found
        CloseWindow(); // Not much to do here, but close the window
    }

    bool window_controller_raylib::disable_scaling()
    {
        window_scaling = false;
        return window_scaling;
    }

    bool window_controller_raylib::enable_scaling()
    {
        return true;
    }

    bool window_controller_raylib::hide_window()
    {
        minimized  = true;
        if( IsWindowHidden() )
        {
            return false;
        }
        SetWindowState( FLAG_WINDOW_HIDDEN );
        return true;
    }

    void window_controller_raylib::process_event( input_event_container * event_holder )
    {
        resized = false;
        minimized = false;

        if( event_holder == nullptr )
        {
            error_log->report("Event holder = null!");
            return;
        }

        if( event_holder->window_id != window_id )
        {
            return;
        }

        /*
        switch( event_holder->event_type )
        {
            //Get new dimensions and repaint on window size change
            case input_event_type::window_resized_event:
                if( minimized)
                {
                    //raylib_RestoreWindow(gpeWindow);
                    raylib_SetWindowSize(local_raylib_window,window_width,window_height);
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
                //raylib_SetWindowFullscreen( gpeWindow, window_flags);
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
                    raylib_RestoreWindow(local_raylib_window);
                    raylib_MaximizeWindow(local_raylib_window);
                    raylib_GetWindowSize(local_raylib_window,&window_width,&window_height);
                    //raylib_RaiseWindow(local_raylib_window);
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
                raylib_GetWindowSize(local_raylib_window,&window_width,&window_height);
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

        */
    }


    void window_controller_raylib::reset_input()
    {
        window_controller_base::reset_input();
        resized = false;
        window_closed = false;


        if(  window_base_renderer!=nullptr )
        {
            window_base_renderer->reset_input();
        }
    }

    void window_controller_raylib::resize_window()
    {
        resized = true;
        if( window_base_renderer!=nullptr )
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

    bool window_controller_raylib::scale_window( int s_width, int s_height , bool scale_int )
    {
        if( window_base_renderer !=nullptr )
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

    bool window_controller_raylib::scale_window_factor( float s_width, float s_height, bool scale_int )
    {
        if( window_base_renderer !=nullptr )
        {
            window_scaling = window_base_renderer->scale_renderer_factor(s_width, s_height, scale_int );
            return window_scaling;
        }
        return false;
    }

    void window_controller_raylib::set_renderer( renderer_base * new_renderer, bool remove_current )
    {
        if( remove_current && window_base_renderer!=nullptr )
        {
            delete window_base_renderer;
            window_base_renderer = nullptr;
        }
        if( new_renderer == nullptr )
        {
            error_log->report(" new_renderer = null in [void window_controller_raylib::set_renderer()] function!");
            return;
        }
        window_base_renderer = new_renderer;
        new_renderer->resize_renderer( window_width, window_height );
    }

    void window_controller_raylib::set_window_position( int new_x, int new_y )
    {
        int current_monitor = 0; //GetCurrentMonitor();
        if( current_monitor >=0 )
        {
            if( new_x < 0 )
            {
                new_x = GetMonitorWidth( current_monitor ) / 2;
            }

            if( new_y < 0 )
            {
                new_y = GetMonitorHeight(current_monitor ) / 2;
            }
            SetWindowPosition(  new_x - GetScreenWidth()/2, new_y  - GetScreenHeight()/2 );
        }
    }

    bool window_controller_raylib::set_window_size( int n_width, int n_height )
    {
        if( n_width > 0 && n_height > 0 )
        {
            SetWindowSize(  n_width, n_height );

            window_width = n_width;
            window_height = n_height;
            resize_window();
            return true;

            if( window_base_renderer !=nullptr )
            {
                window_base_renderer->resize_renderer(n_width, n_height );
                window_width = gpe::screen_width = n_width;
                window_height = gpe::screen_height = n_height;
            }
            return true;
        }
        return false;
    }

    void window_controller_raylib::set_window_title(std::string new_title)
    {
        SetWindowTitle( new_title.c_str() );
    }

    bool window_controller_raylib::show_window()
    {
        RestoreWindow(  );
        minimized  = false;
        return true;
    }


    void window_controller_raylib::start_loop()
    {
        resized = false;

        int past_width = window_width;
        int past_height = window_height;
        window_width = GetScreenWidth();
        window_height = GetScreenHeight();

        if( past_width != window_width || past_height != window_height )
        {
            resized = true;
            resize_window();
        }
        else if( IsWindowResized() )
        {
            resized = true;
            resize_window();
        }
        else
        {
            resized = false;
        }
        window_has_mouse = IsWindowFocused();
        window_has_focus = IsWindowFocused();
        window_closed = WindowShouldClose();
        minimized = IsWindowMinimized();
        windowed = !minimized;
    }

    void window_controller_raylib::toggle_fullscreen()
    {
        if( windowed )
        {
            windowed = false;

        }
        else
        {
            windowed = true;
        }
        ToggleFullscreen();
        window_width = GetScreenWidth();
        window_height = GetScreenHeight();
        gpe::screen_width = window_width;
        gpe::screen_height = window_height;
        resized = true;
        gpe::error_log->report("Window Size: "+ stg_ex::int_to_string(window_width)+","+ stg_ex::int_to_string(window_height) );
    }

    bool window_controller_raylib::window_changed()
    {
        return resized;
    }
}
