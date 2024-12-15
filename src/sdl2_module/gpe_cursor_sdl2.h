/*
gpe_cursor_sdl2.h
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

#ifndef gpe_cursor_sdl2_h
#define gpe_cursor_sdl2_h

#include <string>
#include <map>

#include "SDL.h"
#include "thirdparty/sdl2_surface_ex.h"
#include "../gpe/gpe_cursor_base.h"
#include "../gpe/gpe_error_logger.h"
#include "../gpe/internal_libs/sff_ex.h"
#include "../gpe/internal_libs/stg_ex.h"

namespace gpe
{
    class cursor_controller_sdl2: public gpe::cursor_controller_base
    {
        protected:
            SDL_Cursor * cursor_sdl2;
           std::map <std::string, SDL_Cursor *> cursor_sdl2_map;
        public:
            cursor_controller_sdl2( int window_id = -1 );
            virtual ~cursor_controller_sdl2();
            //Cursor logic
            void cursor_change( std::string new_cursor );
            void cursor_change_system( int system_cursor_id );
            void cursor_clear_dynamic();
            bool cursor_contains( std::string cursor_name );
            bool cursor_create_from_image(std::string f_name );
            int cursor_map_size();
            std::string cursor_system_name( int cId );
            void name_default_cursors();
            void process_cursors();
    };

    extern cursor_controller_sdl2 * cursor_main_sdl2_controller;

    bool init_sdl2_cursor_system();
    void quit_sdl2_cursor_system();
}

#endif //gpe_cursor_sdl2_h
