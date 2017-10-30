/*
GPE_Renderer.cpp
This file is part of:
GAME PENCIL ENGINE
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

#include "GPE_Renderer.h"
#include "GPE_Globals.h"
#include "GPE_String_Ex.h"
#include "GPE_Functions.h"

//The camera
GPE_Rect GPE_DEFAULT_CAMERA;
void reset_camera()
{
    GPE_DEFAULT_CAMERA.x = 0;
    GPE_DEFAULT_CAMERA.y = 0;
    GPE_DEFAULT_CAMERA.w = SCREEN_WIDTH;
    GPE_DEFAULT_CAMERA.h = SCREEN_HEIGHT;
}

bool WINDOW_WAS_JUST_RESIZED = false;

GPE_Renderer::GPE_Renderer(int wWidth, int wHeight,bool showBorder, bool fullScreen, bool maximized)
{
    windowClosed = false;
    windowHasMouse= false;
    windowHasFocus = false;
    windowFocusChanged = false;
    recentResizeHappened = false;
    lastRenderedWidth = 0;
    lastRenderedHeight = 0;
    screenRenderedBefore = false;
    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
    gpeRender = NULL;
    screenClearedOnFrame = false;
    rWidth = wWidth;
    rHeight = wHeight;
    mWidth = wWidth;
    mHeight = wHeight;
    isResized = false;
    isMinimized = false;
    windowPosX = 0;
    windowPosY = 0;
    lastIdForScreenshot = 0;
    //Set up the window and render area
    windowFlags =  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED /*| SDL_WINDOW_ALLOW_HIGHDPI */;
    if(showBorder)
    {
        if(fullScreen)
        {
            if(maximized)
            {
                windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED /*| SDL_WINDOW_ALLOW_HIGHDPI */;
            }
            else
            {
                windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE|SDL_WINDOW_FULLSCREEN /*| SDL_WINDOW_ALLOW_HIGHDPI */;
            }
        }
        else
        {
            if(maximized)
            {
                windowFlags = SDL_WINDOW_SHOWN |SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED /*| SDL_WINDOW_ALLOW_HIGHDPI */;
            }
            else
            {
                windowFlags = SDL_WINDOW_SHOWN| SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_ALLOW_HIGHDPI */;
            }
        }
    }
    else
    {
        if(fullScreen)
        {
            if(maximized)
            {
                windowFlags = SDL_WINDOW_SHOWN|SDL_WINDOW_MAXIMIZED|SDL_WINDOW_BORDERLESS|SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_ALLOW_HIGHDPI */;
            }
            else
            {
                windowFlags = SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_BORDERLESS /*| SDL_WINDOW_ALLOW_HIGHDPI */;
            }
        }
        else
        {
            if(maximized)
            {
                windowFlags = SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS /*| SDL_WINDOW_ALLOW_HIGHDPI */;
            }
            else
            {
                windowFlags = SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE |SDL_WINDOW_BORDERLESS /*| SDL_WINDOW_ALLOW_HIGHDPI */;
            }
        }
    }
    gpeWindow = SDL_CreateWindow("Game Pencil Engine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,wWidth,wHeight,windowFlags);

    //SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS(1);
    //If there's an error
    if( gpeWindow!=NULL)
    {
        SDL_GetWindowPosition(gpeWindow,&windowPosX, &windowPosY);
        gpeRender = SDL_CreateRenderer( gpeWindow, -1, SDL_RENDERER_ACCELERATED   );
    }
    else
    {
        windowOK = false;
        return;
    }
    if( gpeRender == NULL )
    {
        windowOK = false;
        record_error("Unable to load Renderer from window. Program exiting...");
        return;
    }
    else
    {
        windowOK = true;
        SDL_SetRenderTarget(gpeRender,NULL);
        SDL_SetRenderDrawBlendMode(gpeRender,SDL_BLENDMODE_BLEND);
        SDL_SetWindowMinimumSize(gpeWindow,MIN_WINDOW_WIDTH,MIN_WINDOW_HEIGHT);

        clear_renderer();
        update_renderer();
    }
    //Set window flag
    windowed = true;
}

void GPE_Renderer::handle_events(SDL_Event& e)
{
    recentResizeHappened = false;
    windowFocusChanged = false;
     //Window event occured
    if( e.type == SDL_WINDOWEVENT )
    {
        //Caption update flag
        switch( e.window.event )
        {
            //Get new dimensions and repaint on window size change
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            case SDL_WINDOWEVENT_RESIZED:
                if( isMinimized)
                {
                    //SDL_RestoreWindow(gpeWindow);
                    //SDL_SetWindowSize(gpeWindow,mWidth,mHeight);
                    rWidth = mWidth;
                    rHeight = mHeight;
                    isMinimized = false;
                    clear_renderer();
                    record_error("Window unminimized");
                }
                else
                {
                    if( e.window.data1>0 && e.window.data2 > 0)
                    {
                        SCREEN_WIDTH = mWidth = rWidth = e.window.data1;
                        SCREEN_HEIGHT = mHeight = rHeight = e.window.data2;
                        isMinimized = false;
                        record_error("Window resized to ["+int_to_string(SCREEN_WIDTH)+"X "+int_to_string(SCREEN_HEIGHT)+"]." );
                        record_error("Clearing screen");
                        clear_renderer();
                        record_error("Updating renderer...");
                        update_renderer();
                        record_error("Resize completed, will redraw in 0.125 seconds if no future resizes happen...");
                    }
                    else
                    {
                        SCREEN_WIDTH = rWidth = mWidth;
                        SCREEN_HEIGHT = rHeight = mHeight;
                        record_error("Window resized with improper data");
                    }
                }
                recentResizeHappened= true;
                WINDOW_WAS_JUST_RESIZED = true;
                //currentState->render();
                //SDL_RenderPresent( gpeRender );
            break;
            case SDL_WINDOWEVENT_MINIMIZED:
                SDL_SetWindowFullscreen( gpeWindow, windowFlags);
                windowed = true;
                isMinimized = true;
                isResized = true;
                mWidth = rWidth;
                mHeight = rHeight;
                WINDOW_WAS_JUST_RESIZED = true;
                record_error("Window minimized");
                recentResizeHappened = true;
            break;
            case SDL_WINDOWEVENT_ENTER:
                windowFocusChanged= true;
                windowHasMouse= true;
            break;
            case SDL_WINDOWEVENT_LEAVE:
                windowFocusChanged = true;
                windowHasMouse = false;
            break;

            case SDL_WINDOWEVENT_FOCUS_GAINED:
                windowFocusChanged = true;
                windowHasFocus = true;
            break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                windowFocusChanged= true;
                windowHasFocus= false;
            break;
            case SDL_WINDOWEVENT_EXPOSED:
                if( isMinimized)
                {
                    isMinimized = false;
                    WINDOW_WAS_JUST_RESIZED = true;
                    SDL_RestoreWindow(gpeWindow);
                    SDL_MaximizeWindow(gpeWindow);
                    SDL_GetWindowSize(gpeWindow,&mWidth,&mHeight);
                    SDL_RaiseWindow(gpeWindow);
                    SCREEN_WIDTH =  rWidth = mWidth;
                    SCREEN_HEIGHT = rHeight = mHeight;
                    clear_renderer();
                    update_renderer();
                    record_error("Window unminimized from being exposed!");
                }
                else
                {
                    //record_error("Window exposed!!!!");
                }
                recentResizeHappened= true;
            break;
            case SDL_WINDOWEVENT_HIDDEN:
                record_error("Window hidden!!!!");
            break;
            case SDL_WINDOWEVENT_CLOSE:
                windowClosed = true;
            break;
            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_RESTORED:
                SDL_GetWindowSize(gpeWindow,&rWidth,&rHeight);
                SCREEN_WIDTH =  mWidth = rWidth;
                SCREEN_HEIGHT = mHeight = rHeight;
                if( isMinimized)
                {
                    isMinimized = false;
                    record_error("Window restored and unminimized");
                }
                else
                {
                    record_error("Window restored.");
                }
                isResized = true;
                WINDOW_WAS_JUST_RESIZED = true;
                recentResizeHappened = true;
            break;
            case SDL_WINDOWEVENT_MAXIMIZED:
            break;
            case SDL_WINDOWEVENT_NONE:
                //record_error("Unknown Window event happened.");
            break;
            default:
                //record_error("Default Window event happened."+int_to_string(e.window.event ) );
            break;
        }
    }
}

void GPE_Renderer::reset_input()
{
    isResized = false;
    screenClearedOnFrame = false;
    windowClosed = false;
    windowFocusChanged = false;

}

void GPE_Renderer::reset_viewpoint()
{
    SDL_RenderSetViewport( gpeRender, NULL);
}

void GPE_Renderer::set_viewpoint(GPE_Rect * newViewPoint)
{
    if( newViewPoint!=NULL)
    {
        SDL_Rect tempRectangle;
        tempRectangle.x = newViewPoint->x;
        tempRectangle.y = newViewPoint->y;
        tempRectangle.w = newViewPoint->w;
        tempRectangle.h = newViewPoint->h;
        SDL_RenderSetViewport( gpeRender, &tempRectangle);
    }
    else
    {
        SDL_RenderSetViewport( gpeRender, NULL);
    }
}


void GPE_Renderer::set_window_title(std::string newTitle)
{
    SDL_SetWindowTitle(gpeWindow, newTitle.c_str() );
}

void GPE_Renderer::toggle_fullscreen()
{
    if( windowed )
    {
        SDL_SetWindowFullscreen( gpeWindow, SDL_WINDOW_FULLSCREEN_DESKTOP   );
        windowed = false;

    }
    else
    {
        SDL_SetWindowFullscreen( gpeWindow, windowFlags);
        windowed = true;
    }
    SDL_GetWindowSize(gpeWindow,&rWidth,&rHeight);
    mWidth = rWidth;
    mHeight = mHeight;
    WINDOW_WAS_JUST_RESIZED = true;
    record_error("Window Size: ",int_to_string(rWidth),",",int_to_string(rHeight) );
}

bool GPE_Renderer::error_check()
{
    return !windowOK;
}

bool GPE_Renderer::is_fullscreen()
{
     return !windowed;
}

bool GPE_Renderer::window_changed()
{
    return isResized;
}

int GPE_Renderer::get_window_width()
{
    SDL_GetWindowSize(gpeWindow,&rWidth,&rHeight);
    return rWidth;
}

int GPE_Renderer::get_window_height()
{
    SDL_GetWindowSize(gpeWindow,&rWidth,&rHeight);
    return rHeight;
}

SDL_Renderer * GPE_Renderer::get_renderer()
{
    return gpeRender;
}

SDL_Window * GPE_Renderer::get_window()
{
    return gpeWindow;
}
/*
SDL_SysWMinfo * GPE_Renderer::get_sys_info()
{
    return &sdl_sys;
}*/

void GPE_Renderer::clear_renderer()
{
    if( gpeRender!=NULL)
    {
        SDL_RenderSetViewport( gpeRender, NULL );
        SDL_Rect fillRect = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_SetRenderDrawColor( gpeRender, 0, 0, 0, 255 );
        SDL_RenderFillRect( gpeRender, &fillRect );
        screenClearedOnFrame = true;
        //update_renderer();
    }
}

 std::string GPE_Renderer::save_screenshot(std::string screenShotLocation)
 {
     std::string returnString = "";
     if( gpeRender!=NULL && screenRenderedBefore )
     {
        if( lastRenderedWidth==rWidth && lastRenderedHeight==rHeight)
        {
            //Credits: http://stackoverflow.com/questions/22315980/sdl2-c-taking-a-screenshot
            //TalesM - http://stackoverflow.com/users/2312760/talesm
            record_error("Saving screenshot....");
            try
            {
                int ssW = 0;
                int ssH = 0;
                SDL_GetRendererOutputSize(gpeRender, &ssW, &ssH);
                if( ssW > 0 && ssH > 0)
                {
                    if( ssW !=lastRenderedWidth || ssH!=lastRenderedHeight)
                    {
                        record_error("Unable to record screenshot of pixels ["+int_to_string(ssW)+","+int_to_string(ssH)+"]..");
                        clear_renderer();
                        update_renderer();
                        ssW = lastRenderedWidth;
                        ssH = lastRenderedHeight;
                    }
                    SDL_Surface *sshot = NULL;
                    //sshot = SDL_GetWindowSurface(gpeWindow);
                    sshot = SDL_CreateRGBSurface(0, ssW, ssH, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                    unsigned char * pixels = new (std::nothrow) unsigned char[sshot->w * sshot->h * sshot->format->BytesPerPixel];
                    if (pixels == 0)
                    {
                        record_error("Found 0 pixels...");
                    }
                    else
                    {
                        //sshot = SDL_CreateRGBSurface(0, ssW, ssH, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                        record_error("Reading screenshot pixels ["+int_to_string(ssW)+","+int_to_string(ssH)+"]..");
                        if( sshot!=NULL)
                        {
                            record_error("Attempting to process pixels...");
                            if( SDL_RenderReadPixels(gpeRender, &sshot->clip_rect, sshot->format->format,pixels, sshot->w* sshot->format->BytesPerPixel) ==0)
                            {
                                SDL_Surface * saveSurface = SDL_CreateRGBSurfaceFrom(pixels, sshot->w, sshot->h, sshot->format->BitsPerPixel, sshot->w * sshot->format->BytesPerPixel, sshot->format->Rmask, sshot->format->Gmask, sshot->format->Bmask, sshot->format->Amask);
                                if (saveSurface == NULL)
                                {
                                    record_error( "Couldn't create SDL_Surface from renderer pixel data. ",SDL_GetError() );
                                }
                                else
                                {
                                    record_error("Processing read in pixels...");
                                    if( (int)screenShotLocation.size() > 0)
                                    {
                                        IMG_SavePNG(saveSurface, screenShotLocation.c_str() );
                                    }
                                    else
                                    {
                                        /*
                                        Get time code taken from: http://stackoverflow.com/questions/5438482/getting-the-current-time-as-a-yyyy-mm-dd-hh-mm-ss-string
                                        By user Null Set ( http://stackoverflow.com/users/556528/null-set ) and user283145
                                        */
                                        std::time_t rawtime;
                                        std::tm* timeinfo;
                                        char timeBuffer [80];

                                        std::time(&rawtime);
                                        timeinfo = std::localtime(&rawtime);

                                        std::strftime(timeBuffer,80,"%y%m%d_%H%MS",timeinfo);
                                        std::puts(timeBuffer);

                                        ////
                                        bool foundBlank = false;
                                        std::string screenshotNewLocation = "";

                                        screenshotNewLocation = APP_DIRECTORY_NAME+"screenshots/screenshot_"+timeBuffer+".png";
                                        if( file_exists( screenshotNewLocation) ==false )
                                        {
                                            screenShotLocation = screenshotNewLocation;
                                            foundBlank = true;
                                            IMG_SavePNG(saveSurface, screenShotLocation.c_str() );
                                        }

                                        for(int i = 1; foundBlank == false && i <= 1000; i++)
                                        {
                                            screenshotNewLocation = APP_DIRECTORY_NAME+"screenshots/screenshot_"+timeBuffer+"_"+int_to_string(i)+".png";
                                            if( file_exists( screenshotNewLocation) ==false )
                                            {
                                                screenShotLocation = screenshotNewLocation;
                                                foundBlank = true;
                                                IMG_SavePNG(saveSurface, screenShotLocation.c_str() );
                                                break;
                                            }
                                        }
                                        if( foundBlank==false)
                                        {
                                            screenShotLocation = "";
                                            appendToFile(APP_DIRECTORY_NAME+"screenshots/screenshot_log.txt","Unable to save screenshot to "+screenshotNewLocation);
                                            record_error("Unable to save screenshot to "+screenshotNewLocation);
                                        }
                                    }
                                    SDL_FreeSurface(saveSurface);
                                    saveSurface = NULL;
                                }
                            }
                            else
                            {
                                record_error("Unable to process pixels of screenshot");
                            }
                            SDL_FreeSurface(sshot);
                            sshot = NULL;
                            returnString= screenShotLocation;
                        }
                    }
                    delete[] pixels;
                }
                else
                {
                    record_error("Unable to create screenshot of due to invalid sizing...");
                }
            }
            catch (std::exception& e)
            {
                record_error("Excepion occured on saving screenshot: " , e.what() );
            }
        }
        else
        {
            record_error("Unable to save screenshot due to resizing of window");
        }
    }
    return returnString;
}

bool GPE_Renderer::screen_was_cleared()
{
    return screenClearedOnFrame;
}

void GPE_Renderer::update_renderer()
{
    lastRenderedWidth = rWidth;
    lastRenderedHeight = rHeight;
    SDL_RenderPresent( gpeRender );
    screenClearedOnFrame = false;
    screenRenderedBefore = true;
}
