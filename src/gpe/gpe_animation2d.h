/*
gpe_animation2d.h
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

#ifndef gpe_animation2d_h
#define gpe_animation2d_h

#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "gpe_collisions.h"
#include "gpe_globals.h"
#include "gpe_branch.h"
#include "gpe_renderer_base.h"
#include "gpe_texture_base.h"
#include "internal_libs/stg_ex.h"

namespace gpe
{
    enum
    {
        dir_top_left = 0,
        dir_top_center = 1,
        dir_top_right = 2,

        dir_middle_left = 3,
        dir_middle_center = 4,
        dir_middle_right = 5,

        dir_bottom_left = 6,
        dir_bottom_center = 7,
        dir_bottom_right = 8,
    };

    class animation2d_frame_data
    {
        public:
            shape_rect * collision_box;
            int collision_radius;
            shape_rect * frame_box;
            int frame_message_id ;

            animation2d_frame_data();
            ~animation2d_frame_data();
    };

    class animation2d : public branch
    {
        protected:
            int animation_alignment;
            float animation_offset_x, animation_offset_y;
            texture_base * animation_texture;
            std::string file_name;
            bool texture_is_transparent;
            //texture_base *animationFlipSurface;
            //texture_base *animationMirFlipSurface;
            int xoffset, yoffset, frameCount;
            int hPadding, vPadding;
            int x_middle, y_middle;
            float tex_width, tex_height;
            uint8_t ck_r, ck_g, ck_b;


        public:
            int animation_id;
            std::vector <animation2d_frame_data *  > frame_data;

            animation2d();
            animation2d( std::string anim_name, std::string anim_filename, bool transparent_image=false , uint8_t colorkeyR = 255, uint8_t colorkeyG = 0, uint8_t colorkeyB = 255);
            ~animation2d();
            void change_name(std::string anim_name);
            virtual bool copy_image_source(std::string directory_output_name);
            virtual animation2d * copy_self();
            virtual void clean_up();
            void edit_collision_box(int frame_id, int cx, int cy, int cw, int ch); //int 1st parameter frame_id added for 1.6 LTS and newer
            void edit_collision_circle(int frame_id, int cx, int cy, int cr); //int 1st parameter frame_id added for 1.6 LTS and newer
            virtual animation2d * create_new( const std::string& anim_name= "", const std::string& anim_filename = "", bool transparent_image=false, uint8_t colorkeyR = 255, uint8_t colorkeyG = 0, uint8_t colorkeyB = 255  );

            //Getters, should remain the same, but for mos backends, if a change is needed we can make it virtual
            int get_alignment();
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
            int get_xoffset();
            int get_yoffset();
            bool has_texture();

            virtual void load_image( const std::string& anim_filename, bool transparent = true, uint8_t colorkeyR = 255, uint8_t colorkeyG = 0, uint8_t colorkeyB = 255  );

            void render(int sub_image_to_draw, int x_pos, int y_pos,  shape_rect * cam = nullptr);
            void render_colored(int sub_image_to_draw, int x_pos, int y_pos, color * render_color, int alpha = 255, shape_rect * cam = nullptr);
            void render_piece( int sub_image_to_draw,int x_pos, int y_pos, shape_rect * rect_piece = nullptr, shape_rect * cam = nullptr);
            void render_piece_resized( int sub_image_to_draw,int x_pos, int y_pos, int new_width, int new_height, shape_rect * rect_piece = nullptr, shape_rect * cam = nullptr);

            //So far is the only function that needs to be virtual in case implementation differs too greatly per backend
            virtual bool render_quad( int sub_image_to_draw, shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha = 255, shape_rect * cam = nullptr  );

            void render_resized(int sub_image_to_draw, int x_pos, int y_pos, float new_width, float new_height, shape_rect * cam = nullptr);
            void render_rotated(int sub_image_to_draw, int x_pos, int y_pos, float new_angle, float x_scale, float y_scale, shape_rect * cam= nullptr );
            void render_scaled(int sub_image_to_draw, int x_pos, int y_pos, float x_scale, float y_scale, shape_rect * cam = nullptr);
            void render_special(int sub_image_to_draw, int x_pos, int y_pos, float x_scale, float y_scale,float new_angle = 0,color * render_color = nullptr, int alpha = 255, shape_rect * cam= nullptr);

            virtual void reset_frames();
            virtual void set_alignment( int animAlign );
            virtual void setup_animation( int frame_count, int aw, int ah, int sofx, int sofy, int hPad, int vPad,int animAlign =dir_top_left  );
            virtual void setup_fullimg_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad,int animAlign= dir_top_left  );
    };
}

#endif //gpe_animation2d_h
