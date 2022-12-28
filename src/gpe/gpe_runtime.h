/*
gpe_runtime.h
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

#ifndef gpe_runtime_h
#define gpe_runtime_h

#include "gpe_common_includes.h"
#include "gpe_cursor_base.h"
#include "gpe_shared_resources.h"
#include "gpe_globals.h"
#include "gpe_program_state.h"
#include "gpe_camera.h"
#include "gpe_render_package.h"
#include "gpe_spatial_partition_controller.h"
#include "gpe_timer_base.h"

namespace gpe
{
    //The Master class that keeps the world going
    class runtime_master
    {
    private:
        bool loop_started;
        program_state * state_current;
        std::vector < program_state * > states_list;
        std::string state_current_name;
        std::string state_seeked_name;
        scene_camera * runtime_cameras[max_cameras_allowed];
    public:
        bool loading_data;
        runtime_master();
        ~runtime_master();

        //Camera functions
        void camera_disable ( int cameraId);
        void camera_enable ( int cameraId);

        //Loop related functions
        int game_loop();

        void start_loop();
        void reset_loop();

        void end_loop( );
        void finish_loop();

        bool state_add( program_state * new_state );
        bool state_handle_changes();
        program_state * state_find(  std::string name );
        void state_set( std::string name );
    };

    extern runtime_master * game_runtime;
}

#endif //gpe_runtime_h
