/*
gpe_runtime.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the ԓoftwareԩ, to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ԁS ISԬ WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "gpe_runtime.h"
#include "gpe_artist_base.h"
#include "internal_libs/semath.h"

namespace gpe
{
    runtime_master * game_runtime = nullptr;

    runtime_master::runtime_master()
    {
        loading_data = false;
        loop_started = false;

        state_current = nullptr;
        state_seeked_name = "";
        state_current_name = "";

        for(int iV=0; iV < max_cameras_allowed; iV++)
        {
            runtime_cameras[iV] = new scene_camera(0,0,screen_width,screen_height,iV);
        }
    }

    runtime_master::~runtime_master()
    {
        error_log->report("Removing program states...");
        program_state *  cState = nullptr;
        for( int i = (int)states_list.size() -1; i >=0; i-- )
        {
            cState = states_list[i];
            if( cState!=nullptr )
            {
                error_log->report("Removing State["+cState->get_state_name()+"]...");
                delete cState;
                cState = nullptr;
            }
        }
        states_list.clear();
        state_current = nullptr;

        error_log->report("Removing cameras...");
        scene_camera * tCamera = nullptr;
        for(int iV= max_cameras_allowed-1; iV >=0; iV-- )
        {
            tCamera = runtime_cameras[iV];
            if( tCamera!=nullptr)
            {
                delete tCamera;
                tCamera = nullptr;
            }
            runtime_cameras[iV] = nullptr;
        }
    }


    void runtime_master::camera_disable ( int cameraId)
    {
        if( cameraId >=0 && cameraId < max_cameras_allowed )
        {
            runtime_cameras[cameraId]->set_visible( false );
        }
    }

    void runtime_master::camera_enable ( int cameraId )
    {
        if( cameraId >=0 && cameraId < max_cameras_allowed )
        {
            runtime_cameras[cameraId]->update_view_space(0,0, screen_width, screen_height);
            runtime_cameras[cameraId]->update_screen_space(0,0,screen_width, screen_height);
            runtime_cameras[cameraId]->set_visible( true );
        }
    }

    //gameloop
    int runtime_master::game_loop()
    {
        int neededTime = 0;

        state_handle_changes();
        //If the gameloop starts off on exit, we return false
        if( stg_ex::string_lower( state_seeked_name )== "exit" )
        {
            return -4;
        }

        if( state_current == nullptr )
        {
            return -3; //no state set on first go around so we exit
        }

        uint64_t sTicks = 0;
        uint64_t eTicks = 0;
        error_log->report("Beginning gameloop...");
        error_log->report("Current state: "+ state_current->get_state_name() );
        //While we didnt't request gameloop to end
        while( stg_ex::string_lower( state_seeked_name ) != "exit" )
        {
            start_loop();

            //Change state if needed
            if( state_handle_changes() )
            {
                return true;
            }

            if( state_current != nullptr )
            {
                sTicks = time_keeper->get_ticks();
                state_current->process_input();
                eTicks = time_keeper->get_ticks();
                error_log->log_ms_action("state_current->process_input()",eTicks - sTicks,10 );
            }
            else
            {
                return true;
            }

            sTicks = time_keeper->get_ticks();
            state_current->apply_logic();
            eTicks = time_keeper->get_ticks();
            error_log->log_ms_action("state_current->apply_logic",eTicks - sTicks, 20);

            if( window_controller_main->is_minimized() == false )
            {
                sTicks = time_keeper->get_ticks();

                if( rph!=nullptr )
                {
                    rph->clear_render_packages();
                }
                //renderer_main->clear_renderer( false );

                if( gcanvas != nullptr )
                {
                    gcanvas->begin_frame();
                }

                if( state_current!=nullptr )
                {
                    state_current->render();
                }

                eTicks = time_keeper->get_ticks();
                error_log->log_ms_action("state_current->render",eTicks - sTicks,10 );

                //Updates the screen, calculates/caps fps and more.
                end_loop();
            }
            else
            {
                if(input!=nullptr)
                {
                    input->reset_temp_input();
                }
                if( time_keeper!=nullptr )
                {
                    time_keeper->reset_timer();
                }
            }
        }
        return true; //Otherwise return true that the gameloop ended properly
    }

    void runtime_master::start_loop()
    {
        time_keeper->start_timer();
        //gets user input
        uint64_t sTicks = time_keeper->get_ticks();
        input->handle_input(true, false );
        if(  window_controller_main->is_minimized() == false )
        {
            if( cursor_main_controller !=nullptr )
            {
                cursor_main_controller->process_cursors();
            }
            screen_width = window_controller_main->get_window_width();
            screen_height = window_controller_main->get_window_height();
        }
        loop_started  = true;
        uint64_t eTicks = time_keeper->get_ticks();
        error_log->log_ms_action("runtime_master::start_loop()",eTicks - sTicks,10 );
    }

    void runtime_master::end_loop(  )
    {
        loop_started = false;
        if(  window_controller_main->is_minimized() )
        {
            //error_log->report("End loop resetting timer...");
            //time_keeper->reset_timer();
            return;
        }

        if( rph!=nullptr )
        {
            rph->update_render_packages();
        }

        if( gcanvas != nullptr )
        {
            gcanvas->end_frame();
        }

        time_keeper->cap_fps( window_controller_main->is_minimized() );
        time_keeper->calculate_avg_fps( window_controller_main->is_minimized() );
        //}
    }

    void runtime_master::finish_loop()
    {
        end_loop();
    }


    void runtime_master::reset_loop()
    {
        start_loop();
    }

    bool runtime_master::state_add( program_state * newState )
    {
        if( newState!=nullptr )
        {
            if( state_find( newState->get_state_name() ) == nullptr )
            {
                states_list.push_back( newState );
                return true;
            }
        }
        return false;
    }

    program_state * runtime_master::state_find( std::string state_name )
    {
        program_state *  cState = nullptr;
        for( int i = (int)states_list.size() -1; i >=0; i-- )
        {
            cState = states_list[i];
            if( cState!=nullptr && cState->get_state_name() == state_name )
            {
                return cState;
            }
        }
        return nullptr;
    }

    bool runtime_master::state_handle_changes()
    {
        //If the state needs to be changed
        if( (int)state_seeked_name.size() > 0 && state_current_name != state_seeked_name )
        {
            if( state_current!=nullptr )
            {
                state_current->end_state();
            }
            state_current = nullptr;

            program_state * cState  = state_find( state_seeked_name);
            if( cState!=nullptr )
            {
                state_current = cState;
                state_current->start_state();
                state_current_name = state_seeked_name;
            }

            if( stg_ex::string_lower( state_seeked_name ) == "exit" )
            {
                return true;
            }
            else
            {
                state_seeked_name = "";
            }
        }
        return false;
    }

    void runtime_master::state_set( std::string newState )
    {
        state_seeked_name = newState;
    }

}
//GPE Render calls [ End ]
