/*
GPE_Texture.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef TEXTURE_H
#define TEXTURE_H

#include "GPE_CIncludes.h"
#include "GPE_Renderer.h"
#include "GPE_Color.h"
#include "GPE_Globals.h"
#include "GPE_Constants.h"
#include "GPE_String_Ex.h"
#include "GPE_Functions.h"

const int blend_mode_none = 0;
const int blend_mode_add = 1;
const int blend_mode_blend = 2;
const int blend_mode_mod = 3;

class GPE_Texture
{
    private:
        SDL_Texture * texImg;
        int texWid, texHeight;
        std::string fileLocation;
        std::string name;
        short texId;
        bool isTransparent;
    public:
        GPE_Texture();
        ~GPE_Texture();
        void change_color(GPE_Color * newColor);
        void change_color( Uint8 red, Uint8 green, Uint8 blue );
        void change_texture(SDL_Texture * newTexture);
        bool copy_image_source(std::string outDirectoryName);
        std::string get_filename();
        std::string get_name();
        int get_width();
        int get_height();
        int get_id();
        SDL_Texture * get_sdl_texture();
        void load_new_texture( std::string fileName, short id = -1, bool transparent = true);
        //bool loadFromRenderedText( std::string textureText, GPE_Color * textColor, GPE_Font * textFont=NULL);
        void render_tex(  int x, int y,GPE_Rect* clip = NULL );
        void render_tex_resized(  int x, int y,int newWidth, int newHeight, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL);
        void render_tex_rotated(  int x, int y, double renderAngle, int newWidth = -1, int newHeight = -1, GPE_Rect* clip = NULL );
        void set_blend_mode( int newBlendMode);
};

class GPE_Tilesheet
{
    public:
        int tsId;
        GPE_Texture * tsImage;
        std::string name;
        std::string fileNameLocation;
        bool isPreloaded;
        std::vector<GPE_Rect> tsRects;
        int tsWidth, tsHeight, tsXOff, tsYOff, tsXPadding, tsYPadding, tilesPerRow, tilesPerCol;
        GPE_Tilesheet();
        ~GPE_Tilesheet();
        void organize_tilesheet();
};
void render_texture( GPE_Texture * texIn, int x, int y,GPE_Rect* clip = NULL, int hAlign = -1, int vAlign = -1 );
void render_texture_resized( GPE_Texture * texIn, int x, int y,int newWidth, int newHeight,GPE_Rect* clip = NULL, int hAlign = -1, int vAlign = -1, GPE_Color * renderColor=NULL );
void render_texture_rotated( GPE_Texture * texIn, int x, int y,double newAngle,int newWidth, int newHeight,GPE_Rect* clip = NULL );

#endif
