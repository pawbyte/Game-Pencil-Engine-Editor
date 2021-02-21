/*
renderer_base.h
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

#ifndef renderer_base_h
#define renderer_base_h

#include "gpe_shapes.h"
#include <string>

namespace gpe
{
    const int blend_mode_add = 1;
    const int blend_mode_blend = 2;
    const int blend_mode_mod = 3;
    const int blend_mode_mul = 4;
    const int blend_mode_none = 5;

    //Our Renderer Wrapper base class
    class renderer_base
    {
        protected:
            bool previously_scaled;
            bool renderer_scaling;
            bool use_integer_scaling;
            float scale_percentage_x, scale_percentage_y;
            int scale_source_width, scale_source_height;

            int render_count;
            int render_id;
            //Whether the window is windowed or not
            bool cleared_this_frame;
            int last_rendered_width, last_rendered_height;
            int r_width, r_height;
            int last_screenshot_id;

            bool rendered_once;
            int render_blend_mode;
            std::string r_name;
            std::string r_type;
        public:
            bool resized;
            renderer_base();
            renderer_base(int rId, int widthStart, int heightStart );
            virtual ~renderer_base();


            virtual void clean_up();
            virtual void clear_renderer( bool windowIsMinimized );

            virtual bool disable_scaling();
            virtual bool enable_scaling();

            //Check if anything's wrong with the window
            bool error_check();

            int get_blend_mode();
            virtual std::string get_renderer_name();
            std::string get_renderer_type();

            int get_scale_width();
            int get_scale_height();

            bool get_scaled_previous();
            float get_scale_x();
            float get_scale_y();

            bool is_integer_scaling();
            bool is_scaled();

            //Some Shape Stuff
            virtual void render_horizontal_line(int y, int x1, int x2);
            virtual void render_horizontal_line_color( int y, int x1, int x2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
            virtual bool render_circle_color( int x, int y, int rad, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 );

            virtual void reset_input();
            virtual void reset_viewpoint();

            virtual void resize_renderer(int newW, int newH );

            virtual std::string save_screenshot(std::string file_location = "");
            virtual bool scale_renderer( int s_width, int s_height, bool scale_int );
            virtual bool scale_renderer_factor( float s_width, float s_height, bool scale_int );

            virtual bool screen_was_cleared();
            virtual void set_render_blend_mode( int blend_mode_new );
            virtual void set_viewpoint( gpe::shape_rect * newViewPoint = NULL);
            virtual void update_renderer( bool windowIsMinimized );
    };
    extern renderer_base * renderer_main;

}
#endif // renderer_base_H
