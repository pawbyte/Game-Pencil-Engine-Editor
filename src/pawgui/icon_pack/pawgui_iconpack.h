/*
pawgui_iconpack.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the �Software�), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#ifndef PAWGUI_ICONPACK_H
#define PAWGUI_ICONPACK_H

#include "../base/pawgui_base.h"

namespace pawgui
{
    enum
    {
        icon_pack_single_image = 0,
        icon_pack_image_sheet = 1,
        icon_pack_font = 2,
        icon_pack_none = 3, //Leave it as none for now, unsure of other possible types
    };

    class widget_iconpack
    {
        protected:
            std::string last_texture_loaded;
            std::map<std::string, gpe::texture_base *> atlas_textures; //map of every icon_texture
            gpe::animation2d * iconpack_animation;
            int image_effect_required; //i.e sephia, grayscale, invert, etc
        public:
            widget_iconpack( int icon_pack_style, std::string icons_directory, std::string icons_file_type, int font_size = 24, bool effect_required = 0 );
            ~widget_iconpack();
            //i_name  is the icon name in map, but f_name is the actual file name( before the filetype, sheet position is between 0 and frames )
            bool add_icon( std::string i_name, std::string f_name_or_text, int sheet_position );
            bool icon_exists( std::string name );


            void clear_pack();
            void clear_sheet();

            std::string get_file_type();
            std::string get_file_directory();
            int get_sheet_size();
            int get_size();
            bool is_sheet_setup();

            void render_icon( std::string icon_name, int x, int y, int w, int h );
            void setup_sheet( int frame_count, int aw, int ah, int sofx, int sofy, int hPad, int vPad );
            void setup_sheet_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad );
    };
}
#endif //PAWGUI_SCROLLBARS_H
