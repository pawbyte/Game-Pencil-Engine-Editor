/*
gpe_input_sdl2.h
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

-Game Pencil Engine  - SDL2 Module <https://gamepencil.net/sdl2-module/>


*/

#ifndef gpe_input_sdl2_h
#define gpe_input_sdl2_h

#include "SDL.h"
#include "SDL_version.h"
#include "SDL_video.h"
#include "../gpe/gpe_input_base.h"
#include "../gpe/gpe_render_package_handler.h"
#include "../gpe/gpe_shared_resources.h"
#include "../gpe/gpe_timer_base.h"
#include <string>

namespace gpe
{
    class gamepad_sdl2: public gamepad_base
    {
        public:
            SDL_Joystick  * assigned_sdl2_controller;
            int joy_sdl2_id;

            gamepad_sdl2();
            ~gamepad_sdl2();
            void handle_input();
            void pure_reset();
            void reset_gamepad();
            void reset_temp_input();
            void setup_default_mapping( bool map_buttons = true , bool mapAxis = true );
    };

    /**
     * class input_manager_sdl2
     *
     * Handles keyboard, gamepad and mouse states
     */

    class input_manager_sdl2: public input_manager_base
    {
        private:
            SDL_Event sdl2_input_event;
            gamepad_sdl2 * game_pads_sdl2[gp_max_devices];

        public:
            input_manager_sdl2();
            ~input_manager_sdl2();

            bool clipboard_empty();
            bool clipboard_set( std::string new_clipboard_string);
            std::string clipboard_string();

            void convert_event_input();
            void key_bind_qwerty();
            void key_bind_load();
            void key_bind_save();

            //void handle_modifers( SDLMod mod );
            bool gamepad_detect_all();
            bool gamepad_disconnect( int gamepad_id);
            bool gamepad_setup(int gamepad_id );
            bool load_input_settings(std::string file_path = "");
            void handle_input(bool dump_event = false, bool is_embedded=false );
            //void reset_all_input();
            //void reset_temp_input();
    };

    bool init_sdl2_input_system();
    void quit_sdl2_input_system();
}

#endif //gpe_input_sdl2_h
