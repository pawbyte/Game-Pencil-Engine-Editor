/*
gpe_font_base.h
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

#ifndef GPE_FONTS_H
#define GPE_FONTS_H

#include "gpe_common_includes.h"
#include "gpe_shared_resources.h"
#include "gpe_globals.h"
#include "gpe_branch.h"
#include "internal_libs/stg_ex.h"

namespace gpe
{
    class font_pair_base
    {
        protected:
            std::string str;
            int text_width, text_height;
        public:
            int alpha_last_rendered;
            font_pair_base();
            ~font_pair_base();
            bool texture_matches(const std::string& str_check);
            int get_width();
            int get_height();
    };

    class font_base
    {
        protected:
            int last_used_halign;
            int last_used_valign;
            int font_id;
            std::string font_file_location;
            std::string font_family_name;
            std::string font_system_type;
            int font_size;
            bool font_is_monospaced;
            int mono_width;
            int mono_height;
            int number_width;
            int number_height;
        public:
            std::string font_nickname;
            font_base();
            virtual ~font_base();
            virtual void clear_cache();
            virtual font_base * create_new(const std::string& file_loc, int f_size, bool make_monospaced = false, const std::string& f_nickname = "", int id_number =-1);
            virtual font_pair_base * find_character_texture( const std::string& id_number);
            virtual font_pair_base * find_texture( const std::string& text_to_render);
            virtual void get_metrics(const std::string& text_to_render, int * width_value, int *height_value);
            virtual void get_numbered_metrics(const std::string& text_to_render, int * width_value, int *height_value);
            virtual void get_wrapped_string_metrics( const std::string& str_in, int line_width, int linePadding, int * width_value, int *height_value);
            int get_mono_height();
            int get_mono_width();
            std::string get_nickname();
            std::string get_family_name();
            std::string get_font_type();
            virtual int get_cache_count();
            int get_font_id();
            int get_font_size();
            bool is_monospaced();

            virtual void render_bitmapped_text( int x_pos, int y_pos, const std::string& number_to_render, color * text_color, int alignment_h=gpe::fa_top,int alignment_v=gpe::fa_top,int render_alpha = 255);
            virtual void render_text( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top,int render_alpha = 255);
            virtual void render_text_boxed( int x_pos, int y_pos, const std::string& text_to_render, color * text_color,color * boxColor,int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top,int render_alpha = 255);
            virtual void render_text_clipped( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_width = -1, int render_height = -1,int render_alpha = 255);
            virtual void render_text_scaled( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, float text_scale, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_alpha = 255);
            virtual void render_text_resized( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_width = -1, int render_height = -1,int render_alpha = 255);
            virtual void render_text_rotated( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, float textAngle = 0,int render_alpha = 255);
            virtual bool render_text_special( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, float render_angle = 0, float render_scale = 1.f, int render_alpha = 255);
    };

    extern font_base * font_default;

    class font_system_controller
    {
        private:
            int newid_numberer;
            int fontTotalCount;
            font_base * lastFontUsed;
            std::vector < font_base * > loadedFonts;
        public:
            module_information *  version_info;
            font_base * font_template;
            font_system_controller();
            ~font_system_controller();
            //Font  Handling Functions [ BEGIN ]
            font_base *  copy_font( font_base * parent_font, const std::string& font_nickname , int dynamic_id );
            font_base *  open_font( const std::string& fontLocation, int font_size = 12, bool make_monospaced = false, const std::string& f_nickname = "", int dynamic_id = -1 );
            void close_font(int fontId );
            void close_font(font_base * fontIn);
            void clear_font_cache();
            int get_font_cache_size(bool recordCache = false);
            int get_font_count();
            //Font Handling Function [ END ]

            //Font Rendering [ BEGIN ]
            //Auto-translates
            bool render_bitmap_text( int x_pos, int y_pos, const std::string& text_to_render, color * text_color,font_base * text_font=nullptr, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_alpha = 255);
            void render_fps( font_base * text_font,color * color = nullptr , int x_pos = 0, int y_pos = 0, int alignment_h = gpe::fa_right, int alignment_v = gpe::fa_top );
            bool render_text( int x_pos, int y_pos, const std::string& text_to_render, color * text_color,font_base * text_font=nullptr, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_alpha = 255);
            bool render_text_boxed( int x_pos, int y_pos, const std::string& text_to_render, color * text_color,color * boxColor, font_base * text_font=nullptr, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_alpha = 255);
            bool render_text_clipped( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, font_base * text_font=nullptr, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_width = -1, int render_height = -1, int render_alpha = 255);
            bool render_text_resized( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, font_base * text_font=nullptr, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_width = -1, int render_height = -1, int render_alpha = 255);
            bool render_text_rotated( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, font_base * text_font, float textAngle = 0, int render_alpha  = 255);
            bool render_text_scaled( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, float scaleValue, font_base * text_font=nullptr,  int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_alpha = 255);
            bool render_text_special( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, font_base * text_font=nullptr, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, float render_angle = 0, float render_scale = 1.f, int render_alpha = 255);
            //Doesnt translate ( useful for textfields and code/text editors)
            bool render_only_text( int x_pos, int y_pos, const std::string& text_to_render, color * text_color,font_base * text_font=nullptr, int alignment_h=gpe::fa_left,int alignment_v=gpe::fa_top, int render_alpha = 255);
            //Font Rendering [ END ]
    };

    extern font_system_controller * gfs;

    bool init_font_system();
    void quit_font_system();
}

#endif // GPE_FONTS_H
