/*
gpe_font_sdl.h
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

#ifndef gpe_font_sdl_h
#define gpe_font_sdl_h

#include "../gpe/gpe_artist_base.h"
#include "../gpe/gpe_branch.h"
#include "../gpe/gpe_common_includes.h"
#include "../gpe/gpe_color.h"
#include "../gpe/gpe_font_base.h"
#include "../gpe/gpe_globals.h"
#include "../gpe/gpe_runtime.h"
#include "../gpe/gpe_shared_resources.h"
#include "../other_libs/stg_ex.h"
#include "gpe_renderer_sdl.h"
#include <SDL2/SDL_ttf.h>

namespace gpe
{
    class font_pair_sdl: public font_pair_base
    {
        private:
            std::string str;
            SDL_Texture * strTexture;
        public:
            int lastAlphaRendered;
            font_pair_sdl(TTF_Font * fontIn, std::string strIn);
            ~font_pair_sdl();
            SDL_Texture * get_texture();
    };

    class font_sdl_tff: public font_base
    {
        private:
            std::map < const std::string, font_pair_sdl * > textPairs;
            std::map <const std::string, font_pair_sdl * > characterPairs;
            TTF_Font * heldSDLFont;
        public:
            font_sdl_tff( const std::string fFileLocation, int fSize, bool isMonospaced = false, const std::string fNickName = "", int fontIdNumb =-1);
            ~font_sdl_tff();
            void clear_cache();
            font_base * create_new(std::string fFileLocation, int fSize, bool isMonospaced = false, const std::string fNickName = "", int fontIdNumb =-1);
            void get_metrics(std::string textToRender, int * wVal, int *hVal);
            void get_numbered_metrics(std::string textToRender, int * wVal, int *hVal);
            void get_wrapped_string_metrics( const std::string strIn, int lineWidth, int linePadding, int * wVal, int *hVal);
            TTF_Font * get_sdl_font();
            int get_cache_count();
            int get_font_id();
            font_pair_base * find_character_texture( const std::string numbId);
            font_pair_sdl * find_character_texture_sdl( const std::string numbId);

            font_pair_base * find_texture( const std::string textToRender);
            font_pair_sdl * find_texture_sdl( const std::string textToRender);
            void render_bitmapped_text( int xPos, int yPos, std::string numberToRender, color * textColor, int hAlign=gpe::fa_top,int vAlign=gpe::fa_top,int renderAlpha = 255);
            void render_text( int xPos, int yPos, std::string textureText, color * textColor, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top,int renderAlpha = 255);
            void render_text_boxed( int xPos, int yPos, std::string textureText, color * textColor,color * boxColor,int hAlign=gpe::fa_left,int vAlign=gpe::fa_top,int renderAlpha = 255);
            void render_text_scaled( int xPos, int yPos, std::string textureText, color * textColor, float textScale, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int renderAlpha = 255);
            void render_text_resized( int xPos, int yPos, std::string textureText, color * textColor, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, int rendWid = -1, int rendHeight = -1,int renderAlpha = 255);
            void render_text_rotated( int xPos, int yPos, std::string textureText, color * textColor, float textAngle = 0,int renderAlpha = 255);
            bool render_text_special( int xPos, int yPos, std::string textureText, color * textColor, int hAlign=gpe::fa_left,int vAlign=gpe::fa_top, float renderAngle = 0, float renderScale = 1.f, int renderAlpha = 255);

    };

    bool init_sdl_font_system();
    void quit_sdl_font_system();
}

#endif // gpe_font_sdl_h
