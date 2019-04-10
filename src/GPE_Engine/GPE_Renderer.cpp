/*
GPE_Renderer.cpp
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

#include "GPE_Renderer.h"
#include "GPE_Constants.h"
#include "GPE_Globals.h"
#include "GPE_String_Ex.h"
#include "GPE_Functions.h"


int DEFAULT_WINDOW_MIN_WIDTH = 320;
int DEFAULT_WINDOW_MIN_HEIGHT= 240;

//The camera
GPE_Rect GPE_DEFAULT_CAMERA;
void reset_camera()
{
    GPE_DEFAULT_CAMERA.x = 0;
    GPE_DEFAULT_CAMERA.y = 0;
    GPE_DEFAULT_CAMERA.w = SCREEN_WIDTH;
    GPE_DEFAULT_CAMERA.h = SCREEN_HEIGHT;
}

GPE_Rect* GPE_find_camera(GPE_Rect* rectIn)
{
	return nullptr;
}

bool WINDOW_WAS_JUST_RESIZED = false;

GPE_Renderer::GPE_Renderer(int wWidth, int wHeight,bool showBorder, bool fullScreen, bool maximized)
{
    bothFlip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    horiFlip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL );
    vertFlip = (SDL_RendererFlip)( SDL_FLIP_VERTICAL);
    defaultPoint.x = 0;// = {0,0};
    defaultPoint.y = 0;// = {0,0};
    renderBlendMode = blend_mode_blend;
    minWindowWidth = 640;
    minWindowHeight = 480;
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
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    sdlRenderer = NULL;
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
    windowFlags =  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED /* |  SDL_WINDOW_ALLOW_HIGHDPI */;
    if(showBorder)
    {
        if(fullScreen)
        {
            if(maximized)
            {
                windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED /* |  SDL_WINDOW_ALLOW_HIGHDPI */;
            }
            else
            {
                windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE|SDL_WINDOW_FULLSCREEN /* |  SDL_WINDOW_ALLOW_HIGHDPI */;
            }
        }
        else
        {
            if(maximized)
            {
                windowFlags = SDL_WINDOW_SHOWN |SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED /* |  SDL_WINDOW_ALLOW_HIGHDPI */;
            }
            else
            {
                windowFlags = SDL_WINDOW_SHOWN| SDL_WINDOW_RESIZABLE /* |  SDL_WINDOW_ALLOW_HIGHDPI */;
            }
        }
    }
    else
    {
        if(fullScreen)
        {
            if(maximized)
            {
                windowFlags = SDL_WINDOW_SHOWN|SDL_WINDOW_MAXIMIZED|SDL_WINDOW_BORDERLESS|SDL_WINDOW_RESIZABLE /* |  SDL_WINDOW_ALLOW_HIGHDPI */;
            }
            else
            {
                windowFlags = SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_BORDERLESS /* |  SDL_WINDOW_ALLOW_HIGHDPI */;
            }
        }
        else
        {
            if(maximized)
            {
                windowFlags = SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS /* |  SDL_WINDOW_ALLOW_HIGHDPI */;
            }
            else
            {
                windowFlags = SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE |SDL_WINDOW_BORDERLESS /* |  SDL_WINDOW_ALLOW_HIGHDPI */;
            }
        }
    }
    gpeWindow = SDL_CreateWindow("Game Pencil Engine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,wWidth,wHeight,windowFlags  );
    windowIcon = gpe_sdl->load_surface_image(APP_DIRECTORY_NAME+"icon.png");
    SDL_SetWindowIcon( gpeWindow, windowIcon);
    //SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS(1);
    //If there's an error
    if( gpeWindow!=NULL)
    {
        SDL_GetWindowPosition(gpeWindow,&windowPosX, &windowPosY);
        sdlRenderer = SDL_CreateRenderer( gpeWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE );//  | SDL_RENDERER_PRESENTVSYNC );
    }
    else
    {
        windowOK = false;
        return;
    }
    if( sdlRenderer == NULL )
    {
        windowOK = false;
        GPE_Report("Unable to load Renderer from window. Program exiting...");
        return;
    }
    else
    {
        windowOK = true;
        SDL_SetRenderTarget(sdlRenderer,NULL);
        SDL_SetRenderDrawBlendMode(sdlRenderer,SDL_BLENDMODE_BLEND);
        SDL_SetWindowMinimumSize( gpeWindow, DEFAULT_WINDOW_MIN_WIDTH, DEFAULT_WINDOW_MIN_HEIGHT );
        clear_renderer();
        update_renderer();
    }
    //Set window flag
    windowed = true;
}

GPE_Renderer::~GPE_Renderer()
{
    GPE_Report("Deleting Window Icon....");
    if( windowIcon!=NULL)
    {
        SDL_FreeSurface(windowIcon);
        windowIcon = NULL;
    }
    GPE_Report("Deleting Window...");
    SDL_DestroyWindow( gpeWindow );

}

int GPE_Renderer::get_blend_mode()
{
    return renderBlendMode;
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
                GPE_Report("Window unminimized");
            }
            else
            {
                if( e.window.data1>0 && e.window.data2 > 0)
                {
                    SCREEN_WIDTH = mWidth = rWidth = e.window.data1;
                    SCREEN_HEIGHT = mHeight = rHeight = e.window.data2;
                    isMinimized = false;
                    GPE_Report("Window resized to ["+int_to_string(SCREEN_WIDTH)+"X "+int_to_string(SCREEN_HEIGHT)+"]." );
                    //GPE_Report("Clearing screen");
                    clear_renderer();
                    //GPE_Report("Updating renderer...");
                    update_renderer();
                    //GPE_Report("Resize completed, will redraw in 0.125 seconds if no future resizes happen...");
                }
                else
                {
                    SCREEN_WIDTH = rWidth = mWidth;
                    SCREEN_HEIGHT = rHeight = mHeight;
                    GPE_Report("Window resized with improper data");
                }
            }
            recentResizeHappened= true;
            WINDOW_WAS_JUST_RESIZED = true;
            //currentState->render();
            //SDL_RenderPresent( sdlRenderer );
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            SDL_SetWindowFullscreen( gpeWindow, windowFlags);
            windowed = true;
            isMinimized = true;
            isResized = true;
            mWidth = rWidth;
            mHeight = rHeight;
            WINDOW_WAS_JUST_RESIZED = true;
            GPE_Report("Window minimized");
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
                GPE_Report("Window restored and unminimized");
            }
            else
            {
                GPE_Report("Window restored.");
            }
            isResized = true;
            WINDOW_WAS_JUST_RESIZED = true;
            recentResizeHappened = true;
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            break;
        case SDL_WINDOWEVENT_NONE:
            //GPE_Report("Unknown Window event happened.");
            break;
        default:
            //GPE_Report("Default Window event happened."+int_to_string(e.window.event ) );
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
    SDL_RenderSetViewport( sdlRenderer, NULL);
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
        SDL_RenderSetViewport( sdlRenderer, &tempRectangle);
    }
    else
    {
        SDL_RenderSetViewport( sdlRenderer, NULL);
    }
}


void GPE_Renderer::set_window_title(std::string newTitle)
{
    SDL_SetWindowTitle(gpeWindow, newTitle.c_str() );
}

void GPE_Renderer::set_window_min_size( int w, int h)
{
    if( w > 0)
    {
        minWindowWidth = w;
    }
    if( h > 0)
    {
        minWindowHeight = h;
    }

    SDL_SetWindowMinimumSize( gpeWindow, minWindowWidth, minWindowHeight );
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
    GPE_Report("Window Size: ",int_to_string(rWidth),",",int_to_string(rHeight) );
    GPE_Report("Window Flags: ",int_to_string(windowFlags) );
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

SDL_Renderer * GPE_Renderer::get_sdl_renderer()
{
    return sdlRenderer;
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
    set_render_blend_mode( blend_mode_blend );
    if( sdlRenderer!=NULL)
    {
        SDL_RenderSetViewport( sdlRenderer, NULL );
        SDL_Rect fillRect = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_SetRenderDrawColor( sdlRenderer, 0, 0, 0, 255 );
        SDL_RenderFillRect( sdlRenderer, &fillRect );
        screenClearedOnFrame = true;
        //update_renderer();
    }
}

bool GPE_Renderer::render_circle_color( Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    //Modified from SDL_GFX
    Sint16 cx = 0;
    Sint16 cy = rad;
    Sint16 ocx = (Sint16) 0xffff;
    Sint16 ocy = (Sint16) 0xffff;
    Sint16 df = 1 - rad;
    Sint16 d_e = 3;
    Sint16 d_se = -2 * rad + 5;
    Sint16 xpcx, xmcx, xpcy, xmcy;
    Sint16 ypcy, ymcy, ypcx, ymcx;

    /*
    * Sanity check radius
    */
    if (rad <= 0 )
    {
        return false;
    }

    /*
    * Special case for rad=0 - draw a point
    */


    if( sdlRenderer!=NULL )
    {
        //result |= SDL_SetRenderDrawBlendMode(sdlRenderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
        /*
        * Set color
        */
        SDL_SetRenderDrawColor( sdlRenderer,r,g,b,a );

        /*
        * Draw
        */
        do
        {
            xpcx = x + cx;
            xmcx = x - cx;
            xpcy = x + cy;
            xmcy = x - cy;
            if (ocy != cy)
            {
                if (cy > 0)
                {
                    ypcy = y + cy;
                    ymcy = y - cy;
                    render_horizontal_line(ypcy,xmcx, xpcx);
                    render_horizontal_line( ymcy,xmcx, xpcx);
                }
                /*else
                {

                }*/
                ocy = cy;
            }
            if (ocx != cx)
            {
                if (cx != cy)
                {
                    if (cx > 0)
                    {
                        ypcx = y + cx;
                        ymcx = y - cx;
                        render_horizontal_line( ymcx,xmcy, xpcy);
                        render_horizontal_line( ypcx,xmcy, xpcy);
                    }
                    else
                    {
                        render_horizontal_line( y,xmcy, xpcy);
                    }
                }
                ocx = cx;
            }

            /*
            * Update
            */
            if (df < 0)
            {
                df += d_e;
                d_e += 2;
                d_se += 2;
            }
            else
            {
                df += d_se;
                d_e += 2;
                d_se += 4;
                cy--;
            }
            cx++;
        }
        while (cx <= cy);
    }
    return true;
}

void GPE_Renderer::render_horizontal_line(int y, int x1, int x2)
{
    SDL_RenderDrawLine(sdlRenderer, x1, y, x2, y);
}

void GPE_Renderer::render_horizontal_line_color( int y, int x1, int x2,  Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{
    SDL_SetRenderDrawColor(sdlRenderer,r,g,b,a );
    SDL_RenderDrawLine( sdlRenderer, x1, y, x2, y);
}

std::string GPE_Renderer::save_screenshot(std::string screenShotLocation)
{

    std::string returnString = "";
    if( sdlRenderer!=NULL && screenRenderedBefore )
    {
        if( lastRenderedWidth==rWidth && lastRenderedHeight==rHeight)
        {
            //Credits: http://stackoverflow.com/questions/22315980/sdl2-c-taking-a-screenshot
            //TalesM - http://stackoverflow.com/users/2312760/talesm
            //GPE_Report("Saving screenshot....");
            try
            {
                int ssW = 0;
                int ssH = 0;
                SDL_GetRendererOutputSize(sdlRenderer, &ssW, &ssH);
                if( ssW > 0 && ssH > 0)
                {
                    if( ssW !=lastRenderedWidth || ssH!=lastRenderedHeight)
                    {
                        GPE_Report("Unable to record screenshot of pixels ["+int_to_string(ssW)+","+int_to_string(ssH)+"]..");
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
                        GPE_Report("Found 0 pixels...");
                    }
                    else
                    {
                        //sshot = SDL_CreateRGBSurface(0, ssW, ssH, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                        //GPE_Report("Reading screenshot pixels ["+int_to_string(ssW)+","+int_to_string(ssH)+"]..");
                        if( sshot!=NULL)
                        {
                            //GPE_Report("Attempting to process pixels...");
                            if( SDL_RenderReadPixels(sdlRenderer, &sshot->clip_rect, sshot->format->format,pixels, sshot->w* sshot->format->BytesPerPixel) ==0)
                            {
                                SDL_Surface * saveSurface = SDL_CreateRGBSurfaceFrom(pixels, sshot->w, sshot->h, sshot->format->BitsPerPixel, sshot->w * sshot->format->BytesPerPixel, sshot->format->Rmask, sshot->format->Gmask, sshot->format->Bmask, sshot->format->Amask);
                                if (saveSurface == NULL)
                                {
                                    GPE_Report( "Couldn't create SDL_Surface from renderer pixel data. ",SDL_GetError() );
                                }
                                else
                                {
                                    //GPE_Report("Processing read in pixels...");
                                    if( (int)screenShotLocation.size() > 0)
                                    {
                                        IMG_SavePNG(saveSurface, screenShotLocation.c_str() );
                                    }
                                    else
                                    {
                                        /*
                                        Get time code taken from: http://stackoverflow.com/questions/5438482/getting-the-current-time-as-a-yyyy-mm-dd-hh-mm-ss-string
                                        By user NULL Set ( http://stackoverflow.com/users/556528/NULL-set ) and user283145
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
                                            GPE_Report("Unable to save screenshot to "+screenshotNewLocation);
                                        }
                                    }
                                    SDL_FreeSurface(saveSurface);
                                    saveSurface = NULL;
                                }
                            }
                            else
                            {
                                GPE_Report("Unable to process pixels of screenshot");
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
                    GPE_Report("Unable to create screenshot of due to invalid sizing...");
                }
            }
            catch (std::exception& e)
            {
                GPE_Report("Excepion occured on saving screenshot: ", e.what() );
            }
        }
        else
        {
            GPE_Report("Unable to save screenshot due to resizing of window");
        }
    }
    return returnString;
}

bool GPE_Renderer::screen_was_cleared()
{
    return screenClearedOnFrame;
}

void GPE_Renderer::set_render_blend_mode( int newBlendMode )
{
    if( renderBlendMode!=newBlendMode)
    {
        renderBlendMode = newBlendMode;
        switch( newBlendMode)
        {
            case blend_mode_add:
                SDL_SetRenderDrawBlendMode(sdlRenderer,SDL_BLENDMODE_ADD );
                break;

            case blend_mode_mod:
                SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_MOD);
                break;

            case blend_mode_none:
                SDL_SetRenderDrawBlendMode( sdlRenderer, SDL_BLENDMODE_NONE  );
                break;

            default:
                SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
                renderBlendMode = blend_mode_blend;
            break;
        }
    }
}

void GPE_Renderer::update_renderer()
{
    lastRenderedWidth = rWidth;
    lastRenderedHeight = rHeight;
    SDL_RenderPresent( sdlRenderer );
    set_render_blend_mode( blend_mode_blend );
    screenClearedOnFrame = false;
    screenRenderedBefore = true;
}
