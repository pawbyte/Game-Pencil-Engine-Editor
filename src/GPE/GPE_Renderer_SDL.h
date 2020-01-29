/*
GPE_Renderer_SDL.h
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

#ifndef GPE_RENDERER_SDL_H
#define GPE_RENDERER_SDL_H

//The headers
#include "GPE_CIncludes.h"
#include "SDL_SurfaceEx.h"
#include "GPE_Shapes.h"
#include "GPE_Renderer_Base.h"

extern GPE_Rect GPE_CAMERA_DEFAULT;
extern GPE_Rect GPE_CAMERA_CURRENT;

void reset_camera();
void update_rectangle(GPE_Rect * rectIn, float nx, float ny, float nw, float nh);
GPE_Rect * GPE_find_camera(GPE_Rect * rectIn = NULL );

//Our Renderer SDL Wrapper
class GPE_Renderer_SDL: public GPE_Renderer_Base
{
private:
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
    SDL_Renderer * sdlRenderer;
    SDL_Texture * renderTexture;
public:
    SDL_RendererFlip bothFlip;
    SDL_RendererFlip horiFlip;
    SDL_RendererFlip vertFlip;
    SDL_Point  defaultPoint;
    bool recentResizeHappened;
    //SDL_SysWMinfo * get_sys_info();
    GPE_Renderer_SDL(int rId, int wWidth, int wHeight );
    ~GPE_Renderer_SDL();

    void clear_renderer( bool windowIsMinimized );

    //Check if anything's wrong with the window
    bool error_check();

    int get_blend_mode();
    std::string get_renderer_name();
    std::string get_renderer_type();
    SDL_Renderer * get_sdl_renderer();

    void init_renderer_sdl( SDL_Window * cWindow );
    //SDL_SysWMinfo * get_info();

    //Some Shape Stuff
    void render_horizontal_line(int y, int x1, int x2);
    void render_horizontal_line_color( int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    bool render_circle_color( Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255 );

    void reset_input();
    void reset_viewpoint();
    void resize_renderer(int newW, int newH );

    std::string save_screenshot(std::string screenShotLocation = "");
    bool screen_was_cleared();
    void set_render_blend_mode( int newBlendMode );
    void set_viewpoint(GPE_Rect * newViewPoint = NULL);
    void set_window_title(std::string newTitle);
    void update_renderer( bool windowIsMinimized);

};

extern GPE_Renderer_SDL * GPE_MAIN_RENDERER_SDL;

#endif // GPE_RENDERER_SDL_H
