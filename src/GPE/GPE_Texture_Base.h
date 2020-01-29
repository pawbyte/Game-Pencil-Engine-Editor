/*
GPE_Texture_Base.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef GPE_TEXTURE_BASE_H
#define GPE_TEXTURE_BASE_H

#include <string>
#include "SDL2/SDL.h"
#include "GPE_Color.h"
#include "GPE_Shapes.h"
#include "GPE_Renderer_Base.h"

class GPE_Texture_Base
{
protected:
    std::string fileLocation;
    bool isTransparent;
    int texWid, texHeight;
    std::string name;
    std::string textureType;
    int texId;
    int currentBlendMode;
    int currentR, currentG, currentB;
public:
    int lastAlphaRendered;
    GPE_Texture_Base();
    ~GPE_Texture_Base();
    virtual void change_alpha( Uint8 alpha );
    virtual void change_color(GPE_Color * newColor);
    virtual void change_color( Uint8 red, Uint8 green, Uint8 blue );
    virtual bool copy_image_source(std::string outDirectoryName);


    std::string get_filename();
    int get_height();
    int get_id();
    std::string get_name();
    std::string get_texture_type();
    int get_width();

    //Loader functions
    virtual void load_new_texture( GPE_Renderer_Base * renderer, std::string fileName, int id = -1, bool transparent = true, bool useLinearScaling = false );

    //pre-render functions...
    virtual void prerender_circle( GPE_Renderer_Base * renderer, int rad, GPE_Color * circleColor,  Uint8 alpha = 255,int id = -1, bool transparent = true, bool useLinearScaling = true, bool isOutline = false  );
    virtual void prerender_rectangle( GPE_Renderer_Base * renderer, int w, int h, GPE_Color * newColor, int id = -1, bool transparent = true, bool useLinearScaling = true , bool isOutline = false);

    //
    virtual GPE_Texture_Base * produce_clone();

    virtual void render_align(  int x, int y, int hAlign, int vAlign, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL, int alpha = 255  );
    virtual void render_align_resized(  int x, int y,int newWidth, int newHeight, int hAlign, int vAlign, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL, int alpha = 255 );
    virtual void render_tex(  int x, int y,GPE_Rect* clip = NULL, int alpha = 255  );
    virtual void render_tex_colored(  int x, int y, GPE_Color * rendColor, int alpha = 255, GPE_Rect* clip = NULL  );
    virtual void render_tex_resized(  int x, int y,float newWidth, float newHeight, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL, int alpha = 255 );
    virtual void render_tex_scaled(  int x, int y,float xScale, float yScale, GPE_Rect* clip = NULL, GPE_Color * rendColor = NULL, int alpha = 255 );
    virtual void render_tex_rotated(  int x, int y, float renderAngle,  GPE_Color * renderColor = NULL, GPE_Rect* clip = NULL , int alpha = 255 );
    virtual void render_tex_rotated_at_point(  int x, int y, float renderAngle, int pointX, int pointY, GPE_Color * renderColor = NULL, GPE_Rect* clip = NULL , int alpha = 255 );
    virtual void render_tex_special(  int x, int y, float renderAngle, int newWidth = -1, int newHeight = -1, GPE_Color * renderColor = NULL, GPE_Rect* clip = NULL , int alpha = 255 );
    virtual void render_tex_special_at_point(  int x, int y, float renderAngle, int pointX, int pointY,int newWidth = -1, int newHeight = -1, GPE_Color * renderColor = NULL, GPE_Rect* clip = NULL , int alpha = 255 );

    virtual void set_alpha( int alpha);
    virtual void set_blend_mode( int newBlendMode);
};


#endif //GPE_TEXTURE_BASE_H
