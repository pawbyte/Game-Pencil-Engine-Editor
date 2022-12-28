/*
gpe_renderer_sdl.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "gpe_renderer_sdl.h"
#include "../gpe/gpe_constants.h"
#include "../gpe/gpe_globals.h"
#include "../gpe/gpe_settings.h"
#include "../other_libs/stg_ex.h"

namespace gpe
{
    renderer_system_sdl * renderer_main_sdl = NULL;

    renderer_system_sdl::renderer_system_sdl( int rId, int wWidth, int wHeight, SDL_Window * window_ptr )
    {
        r_type = "sdl";
        r_name = "SDL_Renderer";
        renderTexture = NULL;
        bothFlip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
        horiFlip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL );
        vertFlip = (SDL_RendererFlip)( SDL_FLIP_VERTICAL);
        defaultPoint.x = 0;// = {0,0};
        defaultPoint.y = 0;// = {0,0};
        render_blend_mode = blend_mode_blend;
        last_rendered_width = 0;
        last_rendered_height = 0;
        rendered_once = false;
        SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
        SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        //SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
        sdlRenderer = NULL;
        cleared_this_frame = false;
        r_width = wWidth;
        r_height = wHeight;
        last_screenshot_id = 0;

        sdlRenderer = SDL_CreateRenderer( window_ptr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE );//  | SDL_RENDERER_PRESENTVSYNC );
        SDL_RenderClear( sdlRenderer );
        SDL_SetRenderTarget(sdlRenderer,NULL);
        SDL_SetRenderDrawBlendMode(sdlRenderer,SDL_BLENDMODE_BLEND);
    }

    renderer_system_sdl::~renderer_system_sdl()
    {
        if( renderTexture != NULL )
        {
            SDL_DestroyTexture( renderTexture );
            renderTexture = NULL;
        }

        if( sdlRenderer != NULL )
        {
            SDL_DestroyRenderer( sdlRenderer );
            sdlRenderer = NULL;
        }
    }

    bool renderer_system_sdl::disable_scaling()
    {
        SDL_RenderSetScale( sdlRenderer, 1.f, 1.f);
        renderer_scaling = false;
        return false;
    }

    bool renderer_system_sdl::enable_scaling()
    {
        return true;
    }

    int renderer_system_sdl::get_blend_mode()
    {
        return render_blend_mode;
    }

    SDL_Renderer * renderer_system_sdl::get_sdl_renderer()
    {
        return sdlRenderer;
    }

    void renderer_system_sdl::reset_input()
    {
        cleared_this_frame = false;
    }

    void renderer_system_sdl::reset_viewpoint()
    {
        SDL_RenderSetViewport( sdlRenderer, NULL);
    }

    void renderer_system_sdl::resize_renderer(int newW, int newH )
    {
        if( newW == r_width || newH == r_height)
        {
            return;
        }
        r_width = newW;
        r_height = newH;

         if( previously_scaled && !renderer_scaling )
        {
            SDL_RenderSetLogicalSize( sdlRenderer, newW, newH );
            SDL_RenderSetScale( sdlRenderer, 1.f, 1.f );
        }
        if( renderTexture!=NULL )
        {
            SDL_DestroyTexture( renderTexture );
            renderTexture = NULL;
        }
        renderTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, newW, newH );
        SDL_SetRenderTarget(sdlRenderer, NULL );
        SDL_RenderClear( sdlRenderer );
        SDL_RenderPresent(sdlRenderer);
    }

    void renderer_system_sdl::set_viewpoint( shape_rect * newViewPoint)
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

    std::string renderer_system_sdl::get_renderer_name()
    {
        //if( (int)r_name.size() == 0 )
        {
            SDL_RendererInfo SDLRenderInfo;
            SDL_GetRendererInfo(sdlRenderer, &SDLRenderInfo);
            r_name = SDLRenderInfo.name;
        }
        return r_name;
    }

    std::string renderer_system_sdl::get_renderer_type()
    {
        if( (int)r_type.size() == 0 )
        {
            SDL_RendererInfo SDLRenderInfo = {0};
            SDL_GetRendererInfo(sdlRenderer, &SDLRenderInfo);
            r_type = SDLRenderInfo.name;
        }
        return r_type;
    }

    /*
    SDL_SysWMinfo * renderer_system_sdl::get_sys_info()
    {
        return &sdl_sys;
    }*/

    void renderer_system_sdl::clear_renderer( bool windowIsMinimized )
    {
        if( windowIsMinimized )
        {
            return;
        }
        SDL_RenderClear( sdlRenderer);
        return;
        Uint32 sTicks = SDL_GetTicks();
        set_render_blend_mode( blend_mode_blend );
        if( sdlRenderer!=NULL)
        {
            SDL_RenderSetViewport( sdlRenderer, NULL );
            SDL_Rect fillRect = {0,0,screen_width, screen_height};
            SDL_SetRenderDrawColor( sdlRenderer, 0, 0, 0, 255 );
            SDL_RenderFillRect( sdlRenderer, &fillRect );
            cleared_this_frame = true;
        }
        Uint32 eTicks = SDL_GetTicks();
        error_log->log_ms_action("renderer_system_sdl::clear_renderer()",eTicks - sTicks,10 );
    }

    bool renderer_system_sdl::render_circle_color( Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
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

    void renderer_system_sdl::render_horizontal_line(int y, int x1, int x2)
    {
        SDL_RenderDrawLine(sdlRenderer, x1, y, x2, y);
    }

    void renderer_system_sdl::render_horizontal_line_color( int y, int x1, int x2,  Uint8 r, Uint8 g, Uint8 b, Uint8 a )
    {
        SDL_SetRenderDrawColor(sdlRenderer,r,g,b,a );
        SDL_RenderDrawLine( sdlRenderer, x1, y, x2, y);
    }

    std::string renderer_system_sdl::save_screenshot(std::string file_location)
    {
        std::string returnstring = "";
        if( sdlRenderer!=NULL && rendered_once )
        {
            if( last_rendered_width==r_width && last_rendered_height==r_height)
            {
                //Credits: http://stackoverflow.com/questions/22315980/sdl2-c-taking-a-screenshot
                //TalesM - http://stackoverflow.com/users/2312760/talesm
                //error_log->report("Saving screenshot....");
                try
                {
                    int ssW = 0;
                    int ssH = 0;
                    SDL_GetRendererOutputSize(sdlRenderer, &ssW, &ssH);
                    if( ssW > 0 && ssH > 0)
                    {

                        SDL_Surface *sshot = NULL;
                        //sshot = SDL_GetWindowSurface(gpeWindow);
                        sshot = SDL_CreateRGBSurface(0, ssW, ssH, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                        unsigned char * pixels = new (std::nothrow) unsigned char[sshot->w * sshot->h * sshot->format->BytesPerPixel];
                        if (pixels == 0)
                        {
                            error_log->report("Found 0 pixels...");
                            SDL_FreeSurface( sshot );
                            sshot = NULL;
                        }
                        else
                        {
                            //error_log->report("Reading screenshot pixels ["+ stg_ex::int_to_string(ssW)+","+ stg_ex::int_to_string(ssH)+"]..");
                            if( sshot!=NULL)
                            {
                                //error_log->report("Attempting to process pixels...");
                                if( SDL_RenderReadPixels(sdlRenderer, &sshot->clip_rect, sshot->format->format,pixels, sshot->w* sshot->format->BytesPerPixel) ==0)
                                {
                                    SDL_Surface * saveSurface = SDL_CreateRGBSurfaceFrom(pixels, sshot->w, sshot->h, sshot->format->BitsPerPixel, sshot->w * sshot->format->BytesPerPixel, sshot->format->Rmask, sshot->format->Gmask, sshot->format->Bmask, sshot->format->Amask);
                                    if (saveSurface == NULL)
                                    {
                                        std::string sdlError = SDL_GetError();
                                        error_log->report( "Couldn't create SDL_Surface from renderer pixel data. " + sdlError );
                                    }
                                    else
                                    {
                                        //error_log->report("Processing read in pixels...");
                                        if( (int)file_location.size() > 0)
                                        {
                                            IMG_SavePNG(saveSurface, file_location.c_str() );
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

                                            screenshotNewLocation = app_directory_name+"screenshots/screenshot_"+timeBuffer+".png";
                                            if( sff_ex::file_exists( screenshotNewLocation) ==false )
                                            {
                                                file_location = screenshotNewLocation;
                                                foundBlank = true;
                                                IMG_SavePNG(saveSurface, file_location.c_str() );
                                            }

                                            for(int i = 1; foundBlank == false && i <= 1000; i++)
                                            {
                                                screenshotNewLocation = app_directory_name+"screenshots/screenshot_"+timeBuffer+"_"+ stg_ex::int_to_string(i)+".png";
                                                if( sff_ex::file_exists( screenshotNewLocation) ==false )
                                                {
                                                    file_location = screenshotNewLocation;
                                                    foundBlank = true;
                                                    IMG_SavePNG(saveSurface, file_location.c_str() );
                                                    break;
                                                }
                                            }
                                            if( foundBlank==false)
                                            {
                                                file_location = "";
                                                error_log->report("Unable to save screenshot to "+screenshotNewLocation);
                                            }
                                        }
                                        SDL_FreeSurface(saveSurface);
                                        saveSurface = NULL;
                                    }
                                }
                                else
                                {
                                    error_log->report("Unable to process pixels of screenshot");
                                }
                                SDL_FreeSurface(sshot);
                                sshot = NULL;
                                returnstring= file_location;
                            }
                        }
                        delete[] pixels;
                    }
                    else
                    {
                        error_log->report("Unable to create screenshot of due to invalid sizing...");
                    }
                }
                catch (std::exception& e)
                {
                    std::string exceptionTxt = e.what();
                    error_log->report("Excepion occured on saving screenshot: " + exceptionTxt );
                }
            }
            else
            {
                error_log->report("Unable to save screenshot due to resizing of window");
            }
        }
        return returnstring;
    }

    bool renderer_system_sdl::scale_renderer( int s_width, int s_height, bool scale_int )
    {
        if( sdlRenderer != NULL )
        {
            if( scale_int )
            {
                SDL_RenderSetIntegerScale( sdlRenderer, SDL_TRUE );
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

            }
            else
            {
                SDL_RenderSetIntegerScale( sdlRenderer, SDL_FALSE );
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

            }
            SDL_RenderSetLogicalSize( sdlRenderer, s_width, s_height );
            previously_scaled = true;
            renderer_scaling = true;
            return true;
        }
        return false;
    }

    bool renderer_system_sdl::scale_renderer_factor( float s_width, float s_height, bool scale_int )
    {
        if( sdlRenderer != NULL )
        {
            SDL_RenderSetLogicalSize( sdlRenderer, s_width, s_height );
            if( scale_int )
            {
                SDL_RenderSetIntegerScale( sdlRenderer, SDL_TRUE );
            }
            else
            {
                SDL_RenderSetIntegerScale( sdlRenderer, SDL_FALSE );
            }
            if( s_width != 1.f && s_height !=1.f )
            {
                previously_scaled = true;
            }
            renderer_scaling = true;
            return true;
        }
        return false;
    }

    bool renderer_system_sdl::screen_was_cleared()
    {
        return cleared_this_frame;
    }

    void renderer_system_sdl::set_render_blend_mode( int blend_mode_new )
    {
        if( render_blend_mode!=blend_mode_new)
        {
            render_blend_mode = blend_mode_new;
            switch( blend_mode_new)
            {
                case blend_mode_add:
                    SDL_SetRenderDrawBlendMode(sdlRenderer,SDL_BLENDMODE_ADD );
                break;

                case blend_mode_mod:
                    SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_MOD);
                break;

                case blend_mode_mul:
                    SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_MUL);
                break;

                case blend_mode_none:
                    SDL_SetRenderDrawBlendMode( sdlRenderer, SDL_BLENDMODE_NONE  );
                break;

                default:
                    SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
                    render_blend_mode = blend_mode_blend;
                break;
            }
        }
    }

    void renderer_system_sdl::update_renderer( bool windowIsMinimized )
    {
        Uint32 sTicks = SDL_GetTicks();
        last_rendered_width = r_width;
        last_rendered_height = r_height;
        if( windowIsMinimized == false )
        {
            set_render_blend_mode( blend_mode_blend );
            //SDL_RenderCopy( sdlRenderer, renderTexture, NULL, NULL );

            SDL_SetRenderTarget( sdlRenderer, NULL );
            SDL_RenderPresent( sdlRenderer );

            //SDL_SetRenderTarget( sdlRenderer, renderTexture );
            cleared_this_frame = false;
            rendered_once = true;
            SDL_SetRenderDrawColor( sdlRenderer,0,0,0, 255 );
        }
        Uint32 eTicks = SDL_GetTicks();
        error_log->log_ms_action("renderer_system_sdl::update_renderer()",eTicks - sTicks,10 );
    }
}
