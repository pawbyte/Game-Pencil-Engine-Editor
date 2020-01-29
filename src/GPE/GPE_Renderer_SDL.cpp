/*
GPE_Renderer_SDL.cpp
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

#include "GPE_Renderer_SDL.h"
#include "GPE_Constants.h"
#include "GPE_Globals.h"
#include "GPE_String_Ex.h"
#include "GPE_Functions.h"
#include "GPE_Settings.h"

GPE_Renderer_SDL * GPE_MAIN_RENDERER_SDL = NULL;

//The camera
GPE_Rect GPE_CAMERA_DEFAULT;
GPE_Rect GPE_CAMERA_CURRENT;

GPE_Rect * GPE_find_camera(GPE_Rect * rectIn)
{
    if(rectIn==NULL)
    {
        return &GPE_CAMERA_DEFAULT;
    }
    return rectIn;
}

void reset_camera()
{
    GPE_CAMERA_DEFAULT.x = 0;
    GPE_CAMERA_DEFAULT.y = 0;
    GPE_CAMERA_DEFAULT.w = SCREEN_WIDTH;
    GPE_CAMERA_DEFAULT.h = SCREEN_HEIGHT;
}


GPE_Renderer_SDL::GPE_Renderer_SDL( int rId, int wWidth, int wHeight )
{
    renderType = "sdl";
    rendererName = "SDL_Renderer";
    renderTexture = NULL;
    bothFlip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    horiFlip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL );
    vertFlip = (SDL_RendererFlip)( SDL_FLIP_VERTICAL);
    defaultPoint.x = 0;// = {0,0};
    defaultPoint.y = 0;// = {0,0};
    renderBlendMode = blend_mode_blend;
    lastRenderedWidth = 0;
    lastRenderedHeight = 0;
    screenRenderedBefore = false;
    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    sdlRenderer = NULL;
    screenClearedOnFrame = false;
    rWidth = wWidth;
    rHeight = wHeight;
    lastIdForScreenshot = 0;

    //SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS(1);
    //If there's an error

    renderTexture = NULL; //SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, rWidth, rHeight );
    //SDL_SetRenderTarget(sdlRenderer, renderTexture);
}

GPE_Renderer_SDL::~GPE_Renderer_SDL()
{


}

int GPE_Renderer_SDL::get_blend_mode()
{
    return renderBlendMode;
}

SDL_Renderer * GPE_Renderer_SDL::get_sdl_renderer()
{
    return sdlRenderer;
}

void GPE_Renderer_SDL::init_renderer_sdl( SDL_Window * cWindow )
{
    if( cWindow ==NULL)
    {
        return;
    }
    if( sdlRenderer !=NULL)
    {
        SDL_DestroyRenderer( sdlRenderer );
        sdlRenderer = NULL;
    }
    sdlRenderer = SDL_CreateRenderer( cWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE );//  | SDL_RENDERER_PRESENTVSYNC );

    if( sdlRenderer == NULL)
    {
        return;
    }
    SDL_SetRenderTarget(sdlRenderer,NULL);
    SDL_SetRenderDrawBlendMode(sdlRenderer,SDL_BLENDMODE_BLEND);
    clear_renderer( SDL_GetWindowGrab(cWindow) );
    update_renderer( SDL_GetWindowGrab(cWindow)  );

}

void GPE_Renderer_SDL::reset_input()
{
    screenClearedOnFrame = false;
}

void GPE_Renderer_SDL::reset_viewpoint()
{
    SDL_RenderSetViewport( sdlRenderer, NULL);
}

void GPE_Renderer_SDL::resize_renderer(int newW, int newH )
{
    if( newW == rWidth || newH == rHeight)
    {
        return;
    }
    rWidth = newW;
    rHeight = newH;

    if( renderTexture!=NULL )
    {
        SDL_DestroyTexture( renderTexture );
    }
    renderTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, newW, newH );
    SDL_SetRenderTarget(sdlRenderer, NULL );

}

void GPE_Renderer_SDL::set_viewpoint(GPE_Rect * newViewPoint)
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

std::string GPE_Renderer_SDL::get_renderer_name()
{
    if( (int)rendererName.size() == 0 )
    {
        SDL_RendererInfo SDLRenderInfo = {0};
        SDL_GetRendererInfo(sdlRenderer, &SDLRenderInfo);
        rendererName = SDLRenderInfo.name;
    }
    return rendererName;
}

std::string GPE_Renderer_SDL::get_renderer_type()
{
    if( (int)renderType.size() == 0 )
    {
        SDL_RendererInfo SDLRenderInfo = {0};
        SDL_GetRendererInfo(sdlRenderer, &SDLRenderInfo);
        renderType = SDLRenderInfo.name;
    }
    return renderType;
}

/*
SDL_SysWMinfo * GPE_Renderer_SDL::get_sys_info()
{
    return &sdl_sys;
}*/

void GPE_Renderer_SDL::clear_renderer( bool windowIsMinimized )
{
    if( windowIsMinimized )
    {
        return;
    }
    Uint32 sTicks = SDL_GetTicks();
    set_render_blend_mode( blend_mode_blend );
    if( sdlRenderer!=NULL)
    {
        SDL_RenderSetViewport( sdlRenderer, NULL );
        SDL_Rect fillRect = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_SetRenderDrawColor( sdlRenderer, 0, 0, 0, 255 );
        SDL_RenderFillRect( sdlRenderer, &fillRect );
        screenClearedOnFrame = true;
    }
    Uint32 eTicks = SDL_GetTicks();
    GPE_Log_MS_Action("GPE_Renderer_SDL::clear_renderer()",eTicks - sTicks,10 );
}

bool GPE_Renderer_SDL::render_circle_color( Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
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

void GPE_Renderer_SDL::render_horizontal_line(int y, int x1, int x2)
{
    SDL_RenderDrawLine(sdlRenderer, x1, y, x2, y);
}

void GPE_Renderer_SDL::render_horizontal_line_color( int y, int x1, int x2,  Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{
    SDL_SetRenderDrawColor(sdlRenderer,r,g,b,a );
    SDL_RenderDrawLine( sdlRenderer, x1, y, x2, y);
}

std::string GPE_Renderer_SDL::save_screenshot(std::string screenShotLocation)
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
                                    std::string sdlError = SDL_GetError();
                                    GPE_Report( "Couldn't create SDL_Surface from renderer pixel data. " + sdlError );
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
                std::string exceptionTxt = e.what();
                GPE_Report("Excepion occured on saving screenshot: " + exceptionTxt );
            }
        }
        else
        {
            GPE_Report("Unable to save screenshot due to resizing of window");
        }
    }
    return returnString;
}

bool GPE_Renderer_SDL::screen_was_cleared()
{
    return screenClearedOnFrame;
}

void GPE_Renderer_SDL::set_render_blend_mode( int newBlendMode )
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

void GPE_Renderer_SDL::update_renderer( bool windowIsMinimized )
{
    Uint32 sTicks = SDL_GetTicks();
    lastRenderedWidth = rWidth;
    lastRenderedHeight = rHeight;
    if( windowIsMinimized == false )
    {
        set_render_blend_mode( blend_mode_blend );
        //SDL_RenderCopy( sdlRenderer, renderTexture, NULL, NULL );

        SDL_RenderPresent( sdlRenderer );
        //SDL_SetRenderTarget( sdlRenderer, NULL );

        //SDL_SetRenderTarget( sdlRenderer, renderTexture );
        screenClearedOnFrame = false;
        screenRenderedBefore = true;
    }
    Uint32 eTicks = SDL_GetTicks();
    GPE_Log_MS_Action("GPE_Renderer_SDL::update_renderer()",eTicks - sTicks,10 );
}


