/*
GPE_Engine.h
This file is part of:
GAME PENCI ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef GPE_ENGINE_FUNCTIONS_H
#define GPE_ENGINE_FUNCTIONS_H

#include "GPE_CIncludes.h"
#include "shared_resources.h"
#include "functions.h"
#include "GPE_Globals.h"

void render_rect(GPE_Renderer * gRender, SDL_Rect * rendRect,  GPE_Color * rendColor,bool outline = false, int alphaChannel = 255);
void render_rectangle(GPE_Renderer * gRender, int x1, int y1, int x2, int y2,  GPE_Color *rendColor,bool outline = false, int alphaChannel = 255);


void render_rounded_rectangle(GPE_Renderer * gRender, int x1, int y1, int x2, int y2, int rad, GPE_Color * rendColor = NULL);
void render_rounded_rectangle_rgba(GPE_Renderer * gRender, int x1, int y1,int x2, int y2, int rad, GPE_Color * rendColor = NULL, Uint8 a = 255);

bool render_circle_filled_rgba(GPE_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

void render_gradient_horizontal(GPE_Renderer * gRender = NULL, SDL_Rect * rendRect = NULL, GPE_Color * rendColor = NULL);
void render_gradient_vertical(GPE_Renderer * gRender = NULL, SDL_Rect * rendRect = NULL, GPE_Color * rendColor = NULL);
void render_line(GPE_Renderer * gRender, int x1, int y1, int x2, int y2,  GPE_Color *rendColor, int alphaChannel = 255);
void render_line_width(GPE_Renderer * gRender, int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel = 255);

void render_horizontal_line(GPE_Renderer * gRender, int y, int x1, int x2);
void render_horizontal_line_color(GPE_Renderer * gRender, int y, int x1, int x2, GPE_Color *rendColor, int alphaChannel = 255);
void render_vertical_line(GPE_Renderer * gRender,   int x, int y1, int y2);
void render_vertical_line_color(GPE_Renderer * gRender,   int x, int y1, int y2, GPE_Color *rendColor, int alphaChannel = 255);

void limit_space_to_rect(SDL_Rect * rectIn = NULL,int * limitedX = NULL, int * limitedY = NULL);

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
        std::string fontFileLocation;
        std::string fontFamilyName;
        int fontSize;
        bool fontIsMonoSpaced;
        int monoWidth;
        int monoHeight;
        int numberWidth;
        int numberHeight;
        std::vector < fontPair * > textPairs;
        std::vector <fontPair * > characterPairs;
        TTF_Font * heldSDLFont;
    public:
        std::string fontNickName;
        int customFontId;
        GPE_Font(std::string fFileLocation, int fSize, bool isMonospaced = false, std::string fNickName = "");
        ~GPE_Font();
        void clear_cache();
        void get_metrics(std::string textToRender, int * wVal, int *hVal);
        void get_numbered_metrics(std::string textToRender, int * wVal, int *hVal);
        int get_mono_width();
        std::string get_family_name();
        TTF_Font * get_sdl_font();
        int get_cache_count();
        fontPair * find_character_texture( std::string numbId);
        fontPair * find_texture( std::string textToRender);
        void render_bitmapped_text( GPE_Renderer * cRenderer,int xPos, int yPos, std::string numberToRender, GPE_Color * textColor, int hAlign=-1,int vAlign=-1,int renderAlpha = 255);
        void render_text( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign=-1,int vAlign=-1,int renderAlpha = 255);
        void render_text_ext( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign=-1,int vAlign=-1, int rendWid = -1, int rendHeight = -1,int renderAlpha = 255);
        void render_text_rotated( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, double textAngle = 0,int renderAlpha = 255);
        void render_text_boxed( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor,int hAlign=-1,int vAlign=-1,int renderAlpha = 255);
};

extern GPE_Font * LAST_USED_FONT;
extern std::vector < GPE_Font * > loadedGPEFonts;
extern int GLOBAL_FONT_ID;

GPE_Font * GPE_OpenFont(std::string fontLocation, int fontSize = 12, bool isMonoSpaced = false, std::string fNickName = "" );
void GPE_CloseFont(GPE_Font * fontIn);
void GPE_ClearFontCache();
int GPE_GetFontCacheCount(bool recordCache = false);

bool render_bitmap_text( GPE_Renderer * cRenderer,int xPos, int yPos, std::string numberToRender, GPE_Color * textColor,GPE_Font * textFont=NULL, int hAlign=-1,int vAlign=-1, int renderAlpha = 255);
bool render_new_text( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Font * textFont=NULL, int hAlign=-1,int vAlign=-1, int renderAlpha = 255);
bool render_new_text_ext( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont=NULL, int hAlign=-1,int vAlign=-1, int rendWid = -1, int rendHeight = -1, int renderAlpha = 255);
bool render_new_boxed( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor, GPE_Font * textFont=NULL, int hAlign=-1,int vAlign=-1, int renderAlpha = 255);
bool render_new_text_rotated( GPE_Renderer * cRenderer,int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, double textAngle = 0, int renderAlpha  = 255);

#endif
