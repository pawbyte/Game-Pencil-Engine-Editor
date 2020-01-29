/*
GPE_Window.h
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

#ifndef GPE_WINDOW_H
#define GPE_WINDOW_H

//The headers
#include "GPE_CIncludes.h"
#include "SDL_SurfaceEx.h"
#include "GPE_Shapes.h"
#include "GPE_Renderer_Base.h"
#include "GPE_Settings.h"

//Our Window Wrapper
class GPE_Window
{
private:
    Uint32 windowId;
    SDL_Surface * windowIcon;
    Uint32 windowFlags;
    //Whether the window is windowed or not
    bool windowed;
    int mWidth, mHeight;
    int windowPosX, windowPosY;
    //Whether the window is fine
    bool windowOK;
    bool isResized;
    bool isMinimized;
    std::string windowName;
    SDL_Window * sdlWindow;
    //SDL_SysWMinfo sdl_sys;
    GPE_Renderer_Base * windowRenderer;
public:
    bool windowFocusChanged;
    bool windowHasFocus;
    bool windowHasMouse;
    bool windowClosed;
    bool recentResizeHappened;

    //SDL_SysWMinfo * get_sys_info();
    GPE_Window(std::string windowTitle,int wWidth, int wHeight,bool showBorder, bool fullScreen, bool maximized, bool isResizable );
    ~GPE_Window();
    //Handle window events
    void process_events(SDL_Event& e);

    //Check if anything's wrong with the window
    bool error_check();

    //Window getters
    int get_blend_mode();
    int get_window_id();
    int get_window_width();
    int get_window_height();
    std::string get_window_name();
    SDL_Window * get_sdl_window();
    //SDL_SysWMinfo * get_info();

    bool has_focus();
    bool has_mouse();
    bool hide_window();
    bool is_fullscreen();
    bool is_minimized();
    bool is_resized();

    void reset_input();
    void resize_window();
    void set_renderer( GPE_Renderer_Base * newRenderer, bool removeCurrent );
    bool show_window();
    void set_window_title(std::string newTitle);
    std::string save_screenshot(std::string screenShotLocation = "");

    void start_loop();
    //Turn fullscreen on/off
    void toggle_fullscreen();

    bool window_changed();

};

#endif //GPE_WINDOW_H
