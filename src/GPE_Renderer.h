/*
GPE_Renderer.h
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

#ifndef GPE_RENDERER_H
#define GPE_RENDERER_H

//The headers
#include "GPE_CIncludes.h"

const int MIN_WINDOW_WIDTH = 800;
const int MIN_WINDOW_HEIGHT = 600;

extern bool WINDOW_WAS_JUST_RESIZED;
//Our window
class GPE_Renderer
{
    private:
        Uint32 windowFlags;
        //Whether the window is windowed or not
        bool screenClearedOnFrame;
        bool windowed;
        int lastRenderedWidth, lastRenderedHeight;
        int rWidth, rHeight;
        int mWidth, mHeight;
        int windowPosX, windowPosY;
        int lastIdForScreenshot;
        //Whether the window is fine
        bool windowOK;
        bool isResized;
        int isMinimized;
        std::string windowName;
        SDL_Window * gpeWindow;
        //SDL_SysWMinfo sdl_sys;
        bool screenRenderedBefore;
    public:
        bool windowFocusChanged;
        bool windowHasFocus;
        bool windowHasMouse;
        bool windowClosed;
        bool recentResizeHappened;
        SDL_Renderer * gpeRender;
        //SDL_SysWMinfo * get_sys_info();
        GPE_Renderer(int wWidth, int wHeight,bool showBorder, bool fullScreen, bool maximized);
        ~GPE_Renderer();
        //Handle window events
        void handle_events(SDL_Event& e);

        //Turn fullscreen on/off
        void toggle_fullscreen();

        //Check if anything's wrong with the window
        bool error_check();

        bool is_fullscreen();

        bool window_changed();
        int get_window_width();
        int get_window_height();
        SDL_Renderer * get_renderer();
        SDL_Window * get_window();
        //SDL_SysWMinfo * get_info();
        void reset_input();
        void clear_renderer();
        bool screen_was_cleared();
        void set_window_title(std::string newTitle);
        void update_renderer();
        std::string save_screenshot(std::string screenShotLocation = "");
};
#endif
