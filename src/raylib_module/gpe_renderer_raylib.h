/*
gpe_renderer_raylib.h
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

#ifndef gpe_renderer_raylib_h
#define gpe_renderer_raylib_h

//The headers

#include "../gpe/gpe_camera.h"
#include "../gpe/gpe_common_includes.h"
#include "../gpe/gpe_cursor_base.h"
#include "../gpe/gpe_shapes.h"
#include "../gpe/gpe_renderer_base.h"
#include "../gpe/gpe_timer_base.h"

#include "config.h"
#include "raylib.h"
#include "rlgl.h"


#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

namespace gpe
{
    //Our Renderer raylib wrapper
    class renderer_system_raylib: public renderer_base
    {
        protected:
            Camera2D raylib_2d_cameras[ max_cameras_allowed ];
            Camera raylib_3d_cameras[ max_cameras_allowed ];
            Color render_current_color;
            bool in_scissor_mode;
            RenderTexture2D  scissor_mode_target;
            Rectangle scissor_mode_rectangle_source;
            Rectangle scissor_mode_rectangle_dest;
            Vector2 scissor_mode_position;
        public:
            Vector2 scissor_mode_offset;
            //raylib_SysWMinfo * get_sys_info();
            renderer_system_raylib(int rId, int wWidth, int wHeight );
            ~renderer_system_raylib();

            bool begin_mode_2d();
            bool begin_mode_25d();
            bool begin_mode_3d();
            bool begin_mode_vr();

            void clear_renderer( bool windowIsMinimized = false );

            bool disable_scaling();
            bool enable_scaling();

            bool end_mode_2d();
            bool end_mode_25d();
            bool end_mode_3d();
            bool end_mode_vr();

            //Check if anything's wrong with the window
            bool error_check();

            int get_blend_mode();
            std::string get_renderer_name();
            std::string get_renderer_type();

            //raylib_SysWMinfo * get_info();

            //Some Shape Stuff
            void render_horizontal_line(int y, int x1, int x2);
            void render_horizontal_line_color( int y, int x1, int x2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
            bool render_circle_color( int16_t x, int16_t y, int16_t rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 );

            void reset_input();
            void reset_viewpoint();
            void resize_renderer(int newW, int newH );

            std::string save_screenshot(std::string file_location = "");
            bool scale_renderer( int s_width, int s_height, bool scale_int );
            bool scale_renderer_factor( float s_width, float s_height, bool scale_int );

            bool screen_was_cleared();
            void set_render_blend_mode( int blend_mode_new );
            void set_viewpoint( shape_rect * newViewPoint = nullptr);
            void set_window_title(std::string new_title);
            void update_renderer( bool windowIsMinimized);
    };

    extern renderer_system_raylib * renderer_main_raylib;
}

#endif // GPE_RENDERER_raylib_H
