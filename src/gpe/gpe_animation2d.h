/*
gpe_animation2d.h
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

#ifndef gpe_animation2d_h
#define gpe_animation2d_h

#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "gpe_collisions.h"
#include "../other_libs\stg_ex.h"
#include "gpe_globals.h"
#include "gpe_branch.h"
#include "gpe_render_package.h"

namespace gpe
{
    enum class animation2d_collision_zone
    {
        colzone_top_left = 0,
        colzone_top_center = 1,
        colzone_top_right = 2,

        colzone_middle_left = 3,
        colzone_middle_center = 4,
        colzone_middle_right = 5,

        colzone_bottom_left = 6,
        colzone_bottom_center = 7,
        colzone_bottom_right = 8,
    };

    class animaton2d : public branch
    {
        private:
            texture_base * animation_texture;
            std::string file_name;
            //texture_base *animationFlipSurface;
            //texture_base *animationMirFlipSurface;
            int xoffset, yoffset, frameCount;
            int hPadding, vPadding;
            std::vector <shape_rect * > animation_frames;
            int x_middle, y_middle;
        public:
            int animation_id;
            int collision_radius;
            shape_rect * collision_box;
            animation2d_collision_zone collision_zone;
            animaton2d( render_package * r_package, std::string anim_name= "", std::string anim_filename = "", bool transparent_image=false);
            ~animaton2d();
            void change_name(std::string anim_name);
            bool copy_image_source(std::string directory_output_name);
            animaton2d * copy_self();
            void clean_up();
            void edit_collision_box(int cx, int cy, int cw, int ch);
            void edit_collision_circle(int cx, int cy, int cr);
            int get_frame_count();
            int get_width();
            int get_height();
            int get_hpadding();
            int get_vpadding();
            int get_texture_width();
            int get_texture_height();
            int get_frame_xoffset();
            int get_frame_yoffset();

            std::string get_file_name();
            std::string get_name();
            bool has_texture();
            void load_image( render_package * r_package,std::string anim_filename, bool transparent = true );
            void render(int sub_image_to_draw, int x_pos, int y_pos,  shape_rect * cam = NULL);
            void render_colored(int sub_image_to_draw, int x_pos, int y_pos, color * render_color, int alpha = 255, shape_rect * cam = NULL);
            void render_piece( int x_pos, int y_pos, shape_rect * rect_piece = NULL, shape_rect * cam = NULL);
            void render_piece_resized( int x_pos, int y_pos, int new_width, int new_height, shape_rect * rect_piece = NULL, shape_rect * cam = NULL);
            void render_resized(int sub_image_to_draw, int x_pos, int y_pos, float new_width, float new_height, shape_rect * cam = NULL);
            void render_rotated(int sub_image_to_draw, int x_pos, int y_pos, float new_angle, float x_scale, float y_scale, shape_rect * cam= NULL);
            void render_scaled(int sub_image_to_draw, int x_pos, int y_pos, float x_scale, float y_scale, shape_rect * cam = NULL);
            void render_special(int sub_image_to_draw, int x_pos, int y_pos, float x_scale, float y_scale,float new_angle = 0,color * render_color = NULL, int alpha = 255, shape_rect * cam= NULL);
            void reset_frames();
            void setup_animation( int frame_count, int aw, int ah, int sofx, int sofy, int hPad, int vPad );
            void setup_fullimg_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad );
    };
}

#endif //gpe_animation2d_h
