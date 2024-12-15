/*
gpe_window_controller_sdl2_h.h
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

#ifndef gpe_window_sdl2_h
#define gpe_window_sdl2_h

//The headers

#include "../gpe/gpe_globals.h"
#include "../gpe/gpe_shared_resources.h"
#include "../gpe/gpe_window_controller_base.h"
#include "thirdparty/sdl2_surface_ex.h"

namespace gpe
{
    //Our Window SDL Wrapper
    class window_controller_sdl2: public window_controller_base
    {
        private:
            SDL_Window * local_sdl2_window;
            Uint32 window_flags;
            SDL_Surface * window_icon;
        public:
            //SDL_SysWMinfo * get_sys_info();
            window_controller_sdl2(std::string windowTitle,int wWidth, int wHeight,bool showBorder, bool fullScreen, bool maximized, bool isResizable );
            ~window_controller_sdl2();

            bool disable_scaling() override;
            bool enable_scaling() override;
            virtual bool disable_window_resize() override;
            virtual bool enable_window_resize() override;

            SDL_Window * get_sdl2_window();

            //Handle window events
            bool hide_window() override;
            bool minimize_window() override;
            bool maximize_window() override;

            void process_event( input_event_container * event_holder ) override;

             //SDL_SysWMinfo * get_info();
            void reset_input() override;
            void resize_window() override;

            bool scale_window( int s_width, int s_height , bool scale_int ) override;
            bool scale_window_factor( float s_width, float s_height, bool scale_int ) override;

            void set_renderer( renderer_base * new_renderer, bool remove_current );
            void set_vysnc( bool vs_on ) override;
            void set_window_position( int new_x, int new_y ) override;
            bool set_window_size( int n_width, int n_height ) override;
            //std::string save_screenshot(std::string file_location = "");
            void set_window_title(std::string new_title) override;
            bool show_window() override;
            void start_loop() override;

            //Turn fullscreen on/off
            void toggle_fullscreen() override;

            bool window_changed();
    };

    extern window_controller_sdl2 * window_controller_main_sdl2;

    bool init_sdl2_window_system();
    void quit_sdl2_window_system();
}
#endif //gpe_window_sdl2_h
