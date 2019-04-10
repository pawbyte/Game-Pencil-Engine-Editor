/*
GPE_Fonts.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef GPE_FONTS_H
#define GPE_FONTS_H

#include "GPE_CIncludes.h"
#include <SDL_ttf.h>
#include "GPE_Shared_Resources.h"
#include "GPE_Functions.h"
#include "GPE_Globals.h"

class fontPair
{
private:
    std::string str;
    SDL_Texture * strTexture;
    int textWidth, textHeight;
public:
    int lastAlphaRendered;
    fontPair(TTF_Font * fontIn, std::string strIn);
    ~fontPair();
    bool texture_matches(std::string strCheck);
    SDL_Texture * get_texture();
    int get_width();
    int get_height();
};

const int GPE_MonoNumberPairCount = 15;
class GPE_Font
{
private:
    int customFontId;
    std::string fontFileLocation;
    std::string fontFamilyName;
    int fontSize;
    bool fontIsMonoSpaced;
    int monoWidth;
    int monoHeight;
    int numberWidth;
    int numberHeight;
    std::map < std::string, fontPair * > textPairs;
    std::map <std::string, fontPair * > characterPairs;
    TTF_Font * heldSDLFont;
public:
    std::string fontNickName;
    GPE_Font(std::string fFileLocation, int fSize, bool isMonospaced = false, std::string fNickName = "", int fontIdNumb =-1);
    ~GPE_Font();
    void clear_cache();
    void get_metrics(std::string textToRender, int * wVal, int *hVal);
    void get_numbered_metrics(std::string textToRender, int * wVal, int *hVal);
    int get_mono_height();
    int get_mono_width();
    std::string get_family_name();
    TTF_Font * get_sdl_font();
    int get_cache_count();
    int get_font_id();
    fontPair * find_character_texture( std::string numbId);
    fontPair * find_texture( std::string textToRender);
    void render_bitmapped_text( int xPos, int yPos, std::string numberToRender, GPE_Color * textColor, int hAlign=FA_TOP,int vAlign=FA_TOP,int renderAlpha = 255);
    void render_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign=FA_LEFT,int vAlign=FA_TOP,int renderAlpha = 255);
    void render_text_boxed( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor,int hAlign=FA_LEFT,int vAlign=FA_TOP,int renderAlpha = 255);
    void render_text_scaled( int xPos, int yPos, std::string textureText, GPE_Color * textColor, double textScale, int hAlign=FA_LEFT,int vAlign=FA_TOP, int renderAlpha = 255);
    void render_text_resized( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign=FA_LEFT,int vAlign=FA_TOP, int rendWid = -1, int rendHeight = -1,int renderAlpha = 255);
    void render_text_rotated( int xPos, int yPos, std::string textureText, GPE_Color * textColor, double textAngle = 0,int renderAlpha = 255);
    bool render_text_special( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign=FA_LEFT,int vAlign=FA_TOP, double renderAngle = 0, double renderScale = 1.l, int renderAlpha = 255);

};

extern GPE_Font * GPE_DEFAULT_FONT;

class GPE_Font_Controller
{
    private:
        int newFontIdNumber;
        int fontTotalCount;
        GPE_Font * lastFontUsed;
        std::vector < GPE_Font * > loadedFonts;
    public:
        GPE_Font_Controller();
        ~GPE_Font_Controller();
        //Font  Handling Functions [ BEGIN ]
        GPE_Font *  open_font( std::string fontLocation, int fontSize = 12, bool isMonoSpaced = false, std::string fNickName = "", int dynamicId = -1 );
        void close_font(int fontId );
        void close_font(GPE_Font * fontIn);
        void clear_font_cache();
        int get_font_cache_size(bool recordCache = false);
        int get_font_count();
        //Font Handling Function [ END ]

        //Font Rendering [ BEGIN ]
        //Auto-translates
        bool render_bitmap_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Font * textFont=NULL, int hAlign=FA_LEFT,int vAlign=FA_TOP, int renderAlpha = 255);
        void render_fps( GPE_Font * textFont, GPE_Color * color = NULL );
        bool render_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Font * textFont=NULL, int hAlign=FA_LEFT,int vAlign=FA_TOP, int renderAlpha = 255);
        bool render_text_boxed( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor, GPE_Font * textFont=NULL, int hAlign=FA_LEFT,int vAlign=FA_TOP, int renderAlpha = 255);
        bool render_text_resized( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont=NULL, int hAlign=FA_LEFT,int vAlign=FA_TOP, int rendWid = -1, int rendHeight = -1, int renderAlpha = 255);
        bool render_text_rotated( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, double textAngle = 0, int renderAlpha  = 255);
        bool render_text_scaled( int xPos, int yPos, std::string textureText, GPE_Color * textColor, double scaleValue, GPE_Font * textFont=NULL,  int hAlign=FA_LEFT,int vAlign=FA_TOP, int renderAlpha = 255);
        bool render_text_special( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont=NULL, int hAlign=FA_LEFT,int vAlign=FA_TOP, double renderAngle = 0, double renderScale = 1.l, int renderAlpha = 255);
        //Doesnt translate ( useful for textfields and code/text editors)
        bool render_only_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Font * textFont=NULL, int hAlign=FA_LEFT,int vAlign=FA_TOP, int renderAlpha = 255);
        //Font Rendering [ END ]
};

extern GPE_Font_Controller * GFS;
extern GPE_Font_Controller * gfs;

bool GPE_Font_Init();
void GPE_Font_Quit();

#endif // GPE_FONTS_H
