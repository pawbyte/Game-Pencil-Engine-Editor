/*
GPE_Renderer_Base.h
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

#ifndef GPE_RENDERER_BASE_H
#define GPE_RENDERER_BASE_H

#include "GPE_Shapes.h"
#include "SDL2/SDL.h"

#include <string>

const int blend_mode_add = 1;
const int blend_mode_blend = 2;
const int blend_mode_mod = 3;
const int blend_mode_none = 4;

//Our Renderer Wrapper base class
class GPE_Renderer_Base
{
protected:
    int renderCount;
    int renderId;
    //Whether the window is windowed or not
    bool screenClearedOnFrame;
    int lastRenderedWidth, lastRenderedHeight;
    int rWidth, rHeight;
    int lastIdForScreenshot;

    //SDL_SysWMinfo sdl_sys;
    bool screenRenderedBefore;
    int renderBlendMode;
    std::string rendererName;
    std::string renderType;
public:
    bool recentResizeHappened;
    //SDL_SysWMinfo * get_sys_info();
    GPE_Renderer_Base();
    GPE_Renderer_Base(int rId, int widthStart, int heightStart );
    ~GPE_Renderer_Base();


    virtual void clean_up();
    virtual void clear_renderer( bool windowIsMinimized );

    //Check if anything's wrong with the window
    bool error_check();

    int get_blend_mode();
    std::string get_renderer_name();
    std::string get_renderer_type();

    //Some Shape Stuff
    virtual void render_horizontal_line(int y, int x1, int x2);
    virtual void render_horizontal_line_color( int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    virtual bool render_circle_color( Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255 );

    virtual void reset_input();
    virtual void reset_viewpoint();

    virtual void resize_renderer(int newW, int newH );

    virtual std::string save_screenshot(std::string screenShotLocation = "");
    virtual bool screen_was_cleared();
    virtual void set_render_blend_mode( int newBlendMode );
    virtual void set_viewpoint(GPE_Rect * newViewPoint = NULL);
    virtual void update_renderer( bool windowIsMinimized );
};

#endif // GPE_RENDERER_BASE_H
