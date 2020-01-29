/*
GPE_Window.cpp
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

//The headers
#include "GPE_Window.h"

GPE_Window::GPE_Window(std::string windowTitle, int wWidth, int wHeight,bool showBorder, bool fullScreen, bool maximized, bool isResizable )
{
    windowRenderer = NULL;
    windowId = -1;
    windowClosed = false;
    windowHasMouse= false;
    windowHasFocus = false;
    windowFocusChanged = false;
    recentResizeHappened = false;
    mWidth = wWidth;
    mHeight = wHeight;
    isResized = false;
    isMinimized = false;
    windowPosX = 0;
    windowPosY = 0;
    //Set up the window and render area

    windowFlags = SDL_WINDOW_SHOWN;
    if( isResizable )
    {
        windowFlags = windowFlags | SDL_WINDOW_RESIZABLE;
    }

    if( !showBorder)
    {
        windowFlags = windowFlags | SDL_WINDOW_BORDERLESS;
    }
    if(fullScreen)
    {
        windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;
    }
    else if( maximized )
    {
        windowFlags = windowFlags | SDL_WINDOW_MAXIMIZED;
    }

    sdlWindow = SDL_CreateWindow(windowTitle.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,wWidth,wHeight,windowFlags  );
    std::string windowIconFName = "icon.png";
    windowIcon = SDL_SurfaceEx::load_surface_image( windowIconFName.c_str() );
    SDL_SetWindowIcon( sdlWindow, windowIcon);
    SDL_SetWindowMinimumSize( sdlWindow, GPE_ENGINE_SETTINGS->minWindowWidth, GPE_ENGINE_SETTINGS->minWindowHeight );

    //Set window flag
    windowed = true;
    windowId = SDL_GetWindowID( sdlWindow );

    if( sdlWindow!=NULL)
    {
        SDL_GetWindowPosition(sdlWindow,&windowPosX, &windowPosY);
        SDL_GetWindowSize( sdlWindow, &mWidth, &mHeight );
    }
    else
    {
        windowOK = false;
        return;
    }
}

GPE_Window::~GPE_Window()
{

}

SDL_Window * GPE_Window::get_sdl_window()
{
    return sdlWindow;
}

int GPE_Window::get_window_id()
{
    return windowId;
}

int GPE_Window::get_window_width()
{
    SDL_GetWindowSize( sdlWindow,&mWidth,&mHeight);
    return mWidth;
}

int GPE_Window::get_window_height()
{
    SDL_GetWindowSize(sdlWindow,&mWidth,&mHeight);
    return mHeight;
}

bool GPE_Window::has_focus()
{
    return windowHasFocus;
}

bool GPE_Window::has_mouse()
{
    return windowHasFocus;
}

bool GPE_Window::hide_window()
{
    if( sdlWindow == NULL )
    {
        return false;
    }
    SDL_HideWindow( sdlWindow );
    return true;
}

bool GPE_Window::is_fullscreen()
{
    return !windowed;
}


bool GPE_Window::is_minimized()
{
    return isMinimized;
}

bool GPE_Window::is_resized()
{
    return isResized;
}

void GPE_Window::process_events(SDL_Event& e)
{
    recentResizeHappened = false;
    windowFocusChanged = false;

    if( e.type != SDL_WINDOWEVENT || e.window.windowID != windowId )
    {
        return;
    }

    switch( e.window.event )
    {
        //Get new dimensions and repaint on window size change
        case SDL_WINDOWEVENT_SIZE_CHANGED:
        case SDL_WINDOWEVENT_RESIZED:
            if( isMinimized)
            {
                //SDL_RestoreWindow(gpeWindow);
                SDL_SetWindowSize(sdlWindow,mWidth,mHeight);
                isMinimized = false;
                GPE_Report("Window unminimized");
            }
            else
            {
                if( e.window.data1>0 && e.window.data2 > 0)
                {
                    mWidth = e.window.data1;
                    mHeight = e.window.data2;
                    isMinimized = false;
                    resize_window();
                    GPE_Report("Window resized to ["+int_to_string(mWidth)+"X "+int_to_string(mHeight)+"]." );
                }
                else
                {
                    //SCREEN_WIDTH = rWidth = mWidth;
                    //SCREEN_HEIGHT = rHeight = mHeight;
                    resize_window();
                    GPE_Report("Window resized with improper data");
                }
            }
            isResized= true;
        break;

        case SDL_WINDOWEVENT_MINIMIZED:
            //SDL_SetWindowFullscreen( gpeWindow, windowFlags);
            windowed = true;
            isMinimized = true;
            isResized = true;
            mWidth = 0;
            mHeight = 0;
            isResized = true;
            GPE_Report("Window minimized");
            recentResizeHappened = true;
        break;

        case SDL_WINDOWEVENT_ENTER:
            windowFocusChanged= true;
            windowHasMouse= true;
            isMinimized = false;
        break;

        case SDL_WINDOWEVENT_LEAVE:
            windowFocusChanged = true;
            windowHasMouse = false;
            isMinimized = false;
        break;

        case SDL_WINDOWEVENT_FOCUS_GAINED:
            windowFocusChanged = true;
            windowHasFocus = true;
            isMinimized = false;
        break;

        case SDL_WINDOWEVENT_FOCUS_LOST:
            windowFocusChanged= true;
            windowHasFocus= false;
            isMinimized = false;
        break;

        case SDL_WINDOWEVENT_EXPOSED:
            if( isMinimized)
            {
                isMinimized = false;
                isResized = true;
                SDL_RestoreWindow(sdlWindow);
                SDL_MaximizeWindow(sdlWindow);
                SDL_GetWindowSize(sdlWindow,&mWidth,&mHeight);
                SDL_RaiseWindow(sdlWindow);
                resize_window();
                GPE_Report("Window unminimized from being exposed!");
            }
            else
            {
                //GPE_Report("Window exposed!!!!");
            }
            recentResizeHappened= true;
        break;

        case SDL_WINDOWEVENT_HIDDEN:
            GPE_Report("Window hidden!!!!");
            isMinimized = true;
        break;

        case SDL_WINDOWEVENT_CLOSE:
            windowClosed = true;
        break;

        case SDL_WINDOWEVENT_SHOWN:
        case SDL_WINDOWEVENT_RESTORED:
            isMinimized = false;
            SDL_GetWindowSize(sdlWindow,&mWidth,&mHeight);
            if( isMinimized)
            {
                isMinimized = false;
                GPE_Report("Window restored and unminimized");
            }
            else
            {
                GPE_Report("Window restored.");
            }
            isResized = true;
            recentResizeHappened = true;
            resize_window();
        break;

        case SDL_WINDOWEVENT_MAXIMIZED:
            isMinimized = false;
        break;

        case SDL_WINDOWEVENT_NONE:
            //GPE_Report("Unknown Window event happened.");
            break;
        default:
            //GPE_Report("Default Window event happened."+int_to_string(e.window.event ) );
            isMinimized =false;
        break;
    }
}

bool GPE_Window::error_check()
{
    return !windowOK;
}

void GPE_Window::reset_input()
{
    isResized = false;
    windowClosed = false;
    windowFocusChanged = false;
    if( sdlWindow!=NULL && windowRenderer!=NULL)
    {
        SDL_GetWindowSize( sdlWindow, &mWidth, &mHeight );
        windowRenderer->resize_renderer( mWidth, mHeight );
        windowRenderer->reset_input();
    }
}

void GPE_Window::resize_window()
{
    if( windowRenderer!=NULL )
    {
        windowRenderer->resize_renderer( mWidth, mHeight );
    }
    isMinimized = false;
}

void GPE_Window::set_renderer( GPE_Renderer_Base * newRenderer, bool removeCurrent )
{
    if( removeCurrent && windowRenderer!=NULL )
    {
        delete windowRenderer;
        windowRenderer = NULL;
    }
    windowRenderer = newRenderer;
    newRenderer->resize_renderer( mWidth, mHeight );
}

void GPE_Window::set_window_title(std::string newTitle)
{
    SDL_SetWindowTitle(sdlWindow, newTitle.c_str() );
}

bool GPE_Window::show_window()
{
    if( sdlWindow == NULL )
    {
        return false;
    }
    SDL_ShowWindow( sdlWindow );
    return true;
}


void GPE_Window::start_loop()
{
    isResized = false;
}

void GPE_Window::toggle_fullscreen()
{
    if( windowed )
    {
        SDL_SetWindowFullscreen( sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP   );
        windowed = false;

    }
    else
    {
        SDL_SetWindowFullscreen( sdlWindow, windowFlags);
        windowed = true;
    }
    SDL_GetWindowSize(sdlWindow,&mWidth,&mHeight);

    isResized = true;
    GPE_Report("Window Size: "+int_to_string(mWidth)+","+int_to_string(mHeight) );
    GPE_Report("Window Flags: "+int_to_string(windowFlags) );
}

bool GPE_Window::window_changed()
{
    return isResized;
}
