/*
gpe_font_base.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

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
#include "../other_libs/stg_ex.h"

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
            bool texture_matches(std::string strCheck);
            int get_width();
            int get_height();
    };

    const int GPE_MonoNumberPairCount = 15;
    class font_base
    {
        protected:
            int last_used_halign;
            int last_used_valign;
            int customFontId;
            std::string fontFileLocation;
            std::string fontFamilyName;
            std::string font_system_type;
            int fontSize;
            bool fontIsMonoSpaced;
            int monoWidth;
            int monoHeight;
            int numberWidth;
            int numberHeight;
        public:
            std::string fontNickName;
            font_base();
            virtual ~font_base();
            virtual void clear_cache();
            virtual font_base * create_new(std::string fFileLocation, int fSize, bool isMonospaced = false, const std::string fNickName = "", int fontIdNumb =-1);
            virtual void get_metrics(std::string textToRender, int * wVal, int *hVal);
            virtual void get_numbered_metrics(std::string textToRender, int * wVal, int *hVal);
            virtual void get_wrapped_string_metrics( const std::string strIn, int lineWidth, int linePadding, int * wVal, int *hVal);
            int get_mono_height();
            int get_mono_width();
            std::string get_family_name();
            std::string get_font_type();
            virtual int get_cache_count();
            int get_font_id();
            virtual font_pair_base * find_character_texture( const std::string numbId);
            virtual font_pair_base * find_texture( const std::string textToRender);
            virtual void render_bitmapped_text( int xPos, int yPos, std::string numberToRender, color * textColor, int hAlign=gpe::fa_top,int vAlign=gpe::fa_top,int renderAlpha = 255);
            virtual void render_text( int xPos, int yPos, std::string textureText, color * textColor, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top,int renderAlpha = 255);
            virtual void render_text_boxed( int xPos, int yPos, std::string textureText, color * textColor,color * boxColor,int hAlign=gpe::fa_left,int vAlign=gpe::fa_top,int renderAlpha = 255);
            virtual void render_text_scaled( int xPos, int yPos, std::string textureText, color * textColor, float textScale, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int renderAlpha = 255);
            virtual void render_text_resized( int xPos, int yPos, std::string textureText, color * textColor, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int rendWid = -1, int rendHeight = -1,int renderAlpha = 255);
            virtual void render_text_rotated( int xPos, int yPos, std::string textureText, color * textColor, float textAngle = 0,int renderAlpha = 255);
            virtual bool render_text_special( int xPos, int yPos, std::string textureText, color * textColor, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, float renderAngle = 0, float renderScale = 1.f, int renderAlpha = 255);
    };

    extern font_base * font_default;

    class font_system_controller
    {
        private:
            int newFontIdNumber;
            int fontTotalCount;
            font_base * lastFontUsed;
            std::vector < font_base * > loadedFonts;
        public:
            font_base * font_template;
            font_system_controller();
            ~font_system_controller();
            //Font  Handling Functions [ BEGIN ]
            font_base *  open_font( std::string fontLocation, int fontSize = 12, bool isMonoSpaced = false, std::string fNickName = "", int dynamicId = -1 );
            void close_font(int fontId );
            void close_font(font_base * fontIn);
            void clear_font_cache();
            int get_font_cache_size(bool recordCache = false);
            int get_font_count();
            //Font Handling Function [ END ]

            //Font Rendering [ BEGIN ]
            //Auto-translates
            bool render_bitmap_text( int xPos, int yPos, std::string textureText, color * textColor,font_base * textFont=NULL, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int renderAlpha = 255);
            void render_fps( font_base * textFont,color * color = NULL , int xPos = 0, int yPos = 0, int hAlign = gpe::fa_right, int vAlign = gpe::fa_top );
            bool render_text( int xPos, int yPos, std::string textureText, color * textColor,font_base * textFont=NULL, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int renderAlpha = 255);
            bool render_text_boxed( int xPos, int yPos, std::string textureText, color * textColor,color * boxColor, font_base * textFont=NULL, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int renderAlpha = 255);
            bool render_text_resized( int xPos, int yPos, std::string textureText, color * textColor, font_base * textFont=NULL, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int rendWid = -1, int rendHeight = -1, int renderAlpha = 255);
            bool render_text_rotated( int xPos, int yPos, std::string textureText, color * textColor, font_base * textFont, float textAngle = 0, int renderAlpha  = 255);
            bool render_text_scaled( int xPos, int yPos, std::string textureText, color * textColor, float scaleValue, font_base * textFont=NULL,  int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int renderAlpha = 255);
            bool render_text_special( int xPos, int yPos, std::string textureText, color * textColor, font_base * textFont=NULL, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, float renderAngle = 0, float renderScale = 1.f, int renderAlpha = 255);
            //Doesnt translate ( useful for textfields and code/text editors)
            bool render_only_text( int xPos, int yPos, std::string textureText, color * textColor,font_base * textFont=NULL, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int renderAlpha = 255);
            //Font Rendering [ END ]
    };

    extern font_system_controller * gfs;

    bool init_font_system();
    void quit_font_system();
}

#endif // GPE_FONTS_H
