/*
GPE_Texture.h
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

#ifndef TEXTURE_H
#define TEXTURE_H

#include "GPE_CIncludes.h"
#include "GPE_Renderer.h"
#include "GPE_Color.h"
#include "GPE_Globals.h"
#include "GPE_Constants.h"
#include "GPE_String_Ex.h"
#include "GPE_Functions.h"
#include "GPE_Surface_Modifier.h"

class GPE_BasicTexture
{
protected:
    SDL_Texture * texImg;
    int texWid, texHeight;
    std::string name;
    short texId;
    int currentBlendMode;
    int currentR, currentG, currentB;
public:
    int lastAlphaRendered;
    GPE_BasicTexture();
    ~GPE_BasicTexture();
    void change_alpha( Uint8 alpha );
    void change_color(GPE_Color * newColor);
    void change_color( Uint8 red, Uint8 green, Uint8 blue );

    int get_width();
    int get_height();
    int get_id();
    std::string get_name();
    SDL_Texture * get_sdl_texture();

    void render_align(  int x, int y, int hAlign, int vAlign, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL, int alpha = 255  );
    void render_align_resized(  int x, int y,int newWidth, int newHeight, int hAlign, int vAlign, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL, int alpha = 255 );
    void render_tex(  int x, int y,GPE_Rect* clip = NULL, int alpha = 255  );
    void render_tex_colored(  int x, int y, GPE_Color * rendColor, int alpha = 255, GPE_Rect* clip = NULL  );
    void render_tex_resized(  int x, int y,double newWidth, double newHeight, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL, int alpha = 255 );
    void render_tex_scaled(  int x, int y,double xScale, double yScale, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL, int alpha = 255 );
    void render_tex_rotated(  int x, int y, double renderAngle,  GPE_Color * renderColor = NULL, GPE_Rect* clip = NULL , int alpha = 255 );
    void render_tex_rotated_at_point(  int x, int y, double renderAngle, int pointX, int pointY, GPE_Color * renderColor = NULL, GPE_Rect* clip = NULL , int alpha = 255 );
    void render_tex_special(  int x, int y, double renderAngle, int newWidth = -1, int newHeight = -1, GPE_Color * renderColor = NULL, GPE_Rect* clip = NULL , int alpha = 255 );
    void render_tex_special_at_point(  int x, int y, double renderAngle, int pointX, int pointY,int newWidth = -1, int newHeight = -1, GPE_Color * renderColor = NULL, GPE_Rect* clip = NULL , int alpha = 255 );

    void set_alpha( int alpha);
    void set_blend_mode( int newBlendMode);
};

class GPE_Texture: public GPE_BasicTexture
{
private:
    std::string fileLocation;
    bool isTransparent;
public:
    GPE_Texture();
    ~GPE_Texture();
    void change_texture(SDL_Texture * newTexture);
    bool copy_image_source(std::string outDirectoryName);
    std::string get_filename();

    void load_new_texture( std::string fileName, short id = -1, bool transparent = true, bool useLinearScaling = false );
    //bool loadFromRenderedText( std::string textureText, GPE_Color * textColor, GPE_Font * textFont=NULL);
    void prerender_circle( int rad, GPE_Color * circleColor,  Uint8 alpha = 255,short id = -1, bool transparent = true, bool useLinearScaling = true  );
    void prerender_rectangle( int w, int h, GPE_Color * newColor, short id = -1, bool transparent = true, bool useLinearScaling = true  );
};

class GPE_TargetTexture: public GPE_BasicTexture
{
public:
    void resize_target( int w, int h, short id = -1,bool useLinearScaling = true  );
    GPE_TargetTexture();
    ~GPE_TargetTexture();
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


#endif
