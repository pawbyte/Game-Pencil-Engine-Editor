/*
gpe_renderer_sdl2.h
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

#ifndef gpe_renderer_sdl2_h
#define gpe_renderer_sdl2_h

//The headers

#include <ctime>
#include "../gpe/gpe_common_includes.h"
#include "../gpe/gpe_shapes.h"
#include "../gpe/gpe_renderer_base.h"
#include "gpe_sdl2_constants.h"
#include "thirdparty/sdl2_surface_ex.h"

namespace gpe
{
    //Our Renderer SDL Wrapper
    class renderer_system_sdl2: public renderer_base
    {
        private:
            //SDL_SysWMinfo sdl2_sys;
            SDL_Renderer * sdlRenderer;
            SDL_Texture * renderTexture;

        public:
            SDL_RendererFlip bothFlip;
            SDL_RendererFlip horiFlip;
            SDL_RendererFlip vertFlip;
            SDL_Point  defaultPoint;
            //SDL_SysWMinfo * get_sys_info();
            renderer_system_sdl2(int rId, int wWidth, int wHeight, SDL_Window * window_ptr );
            ~renderer_system_sdl2();

            void clear_renderer( bool windowIsMinimized );

            bool disable_scaling();
            bool enable_scaling();

            //Check if anything's wrong with the window
            bool error_check();

            int get_blend_mode();
            std::string get_renderer_name();
            std::string get_renderer_type();
            SDL_Renderer * get_sdl2_renderer();

            //SDL_SysWMinfo * get_info();

            //Some Shape Stuff
            void render_horizontal_line(int y, int x1, int x2);
            void render_horizontal_line_color( int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
            bool render_circle_color( Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255 );

            void reset_input();
            void reset_viewpoint();
            void resize_renderer(int newW, int newH );

            std::string save_screenshot(std::string file_location = "");
            bool scale_renderer( int s_width, int s_height, bool scale_int );
            bool scale_renderer_factor( float s_width, float s_height, bool scale_int );

            bool screen_was_cleared();
            void set_render_blend_mode( int blend_mode_new );
            void set_viewpoint( shape_rect * newViewPoint = NULL);
            void set_vysnc( bool vs_on );

            void set_window_title(std::string new_title);
            void update_renderer( bool windowIsMinimized);
    };

    extern renderer_system_sdl2 * renderer_main_sdl2;
}
#endif // GPE_RENDERER_sdl2_h
