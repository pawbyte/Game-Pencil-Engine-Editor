/*
sdl2_surface_ex.cpp
This file is part of:
SDL_SurfaceEx
https://www.pawbyte.com/sdl2_surface_ex
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.

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

-Game Pencil Engine <https://www.pawbyte.com/sdl2_surface_ex>


*/

#include <algorithm>
#include <math.h>
#include "sdl2_surface_ex.h"

namespace sdl2_surface_ex
{
    Uint32 get_pixel32( SDL_Surface *surface, int x, int y )
    {
        if( surface!=NULL)
        {
            if( x>=0 && x < surface->w && y>=0 && y < surface->h)
            {
                Uint32 *ptr = (Uint32*)surface->pixels;
                int lineoffset = y * (surface->pitch/4 );
                return ptr[lineoffset + x ];
            }
        }
        return 0;
    }

    uint32_t get_pixel32_t( SDL_Surface *surface, int x, int y )
    {
        if( surface!=NULL)
        {
            if( x>=0 && x < surface->w && y>=0 && y < surface->h)
            {
                uint32_t *ptr = (uint32_t*)surface->pixels;
                int lineoffset = y * (surface->pitch/4 );
                return ptr[lineoffset + x ];
            }
        }
        return 0;
    }

    Uint8 merge_channel(Uint8 a, Uint8 b, float amount)
    {
        float result = (b * amount) + (a * (1.0f - amount));
        return (Uint8)result;
    }

    uint8_t merge_channel_t(uint8_t a, uint8_t b, float amount)
    {
        float result = (b * amount) + (a * (1.0f - amount));
        return (uint8_t)result;
    }

    void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
    {
        if( surface==NULL)
        {
            return;
        }
        if( x < surface->w && y < surface->h)
        {
            //Convert the pixels to 32 bit
            Uint32 *pixels = (Uint32 *)surface->pixels;

            //Set the pixel
            pixels[ ( y * surface->w ) + x ] = pixel;
        }
    }

    void put_pixel32_t( SDL_Surface *surface, int x, int y, uint32_t pixel )
    {
        if( surface==NULL)
        {
            return;
        }
        if( x < surface->w && y < surface->h)
        {
            //Convert the pixels to 32 bit
            uint32_t *pixels = (uint32_t *)surface->pixels;

            //Set the pixel
            pixels[ ( y * surface->w ) + x ] = pixel;
        }
    }

    SDL_Texture * create_texture_from_surface( SDL_Renderer * sdlRenderer, SDL_Surface * surf,  int format  )
    {
        if( surf == NULL)
        {
            return NULL;
        }

        if( sdlRenderer == NULL )
        {
            return NULL;
        }
        SDL_Surface * cast_img = SDL_ConvertSurfaceFormat(surf, format, 0 );
        SDL_Rect rect = { 0,0,cast_img->w,cast_img->h};
        SDL_Texture * newTexture = SDL_CreateTexture( sdlRenderer, format, SDL_TEXTUREACCESS_STATIC, cast_img->w, cast_img->h );
        SDL_UpdateTexture(newTexture, &rect, cast_img->pixels,cast_img->w*4);
        SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
        SDL_FreeSurface(cast_img);
        return newTexture;
    }

    SDL_Surface * create_clear_surface(int w, int h, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, Uint8 alpha  )
    {
        if( w <= 0 || h  <= 0 )
        {
            return NULL;
        }
        SDL_Surface * newSurface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE , w, h, 32, SDL_PIXELFORMAT_RGBA8888 );

        SDL_SetSurfaceRLE( newSurface, 3 );
        SDL_SetSurfaceBlendMode( newSurface, SDL_BLENDMODE_BLEND );

        if (newSurface == NULL)
        {
            return NULL;
        }
        int x = 0, y = 0;
        bool surface_is_locked = SDL_MUSTLOCK( newSurface );
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_LockSurface( newSurface );
        }

        Uint8 rr=0, bb=0, gg=0, aa =0;
        Uint32 pixel;

        for(  x = 0; x < newSurface->w; x++)
        {
            //Go through rows
            for(  y = 0; y < newSurface->h; y++)
            {
                //Get pixel
                pixel = SDL_MapRGBA(newSurface->format, color_key_r,color_key_g,color_key_b, alpha );
                put_pixel32( newSurface, x, y, pixel );
            }
        }
        SDL_SetColorKey( newSurface, SDL_TRUE, SDL_MapRGBA( newSurface->format, color_key_r,color_key_g,color_key_b, alpha) );

        SDL_SetSurfaceBlendMode( newSurface, SDL_BLENDMODE_ADD );

        for(  x = 0; x < newSurface->w; x++)
        {
            //Go through rows
            for(  y = 0; y < newSurface->h; y++)
            {
                //Get pixel
                //pixel = SDL_MapRGBA(newSurface->format, color_key_r,color_key_g,color_key_b, alpha );
                //put_pixel32( newSurface, x, y, pixel );
            }
        }
        SDL_SetSurfaceBlendMode( newSurface, SDL_BLENDMODE_ADD );

        if( surface_is_locked )
        {
            //Lock the surface
            SDL_UnlockSurface( newSurface );
        }
        return newSurface;
    }


    SDL_Surface * create_filled_surface_rgba(int w, int h, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, Uint8 alpha  )
    {
        if( w <= 0 || h  <= 0 )
        {
            return NULL;
        }
        SDL_Surface * newSurface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE , w, h, 32, SDL_PIXELFORMAT_RGBA8888 );
        SDL_SetSurfaceRLE( newSurface, 3 );
        SDL_SetSurfaceBlendMode( newSurface, SDL_BLENDMODE_NONE );

        if (newSurface == NULL)
        {
            return NULL;
        }
        int x = 0, y = 0;
        bool surface_is_locked = SDL_MUSTLOCK( newSurface );
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_LockSurface( newSurface );
        }

        Uint8 rr=0, bb=0, gg=0, aa =0;
        Uint32 pixel;

        for(  x = 0; x < newSurface->w; x++)
        {
            //Go through rows
            for(  y = 0; y < newSurface->h; y++)
            {
                //Get pixel
                pixel = SDL_MapRGB( newSurface->format, 254,254,254 );
                put_pixel32( newSurface, x, y, pixel );
            }
        }
        SDL_SetSurfaceBlendMode( newSurface, SDL_BLENDMODE_ADD );

        for(  x = 0; x < newSurface->w; x++)
        {
            //Go through rows
            for(  y = 0; y < newSurface->h; y++)
            {
                //Get pixel
                pixel = SDL_MapRGBA(newSurface->format, color_key_r,color_key_g,color_key_b, alpha );
                put_pixel32( newSurface, x, y, pixel );
            }
        }
        SDL_SetSurfaceBlendMode( newSurface, SDL_BLENDMODE_BLEND );

        if( surface_is_locked )
        {
            //Lock the surface
            SDL_UnlockSurface( newSurface );
        }
        return newSurface;
    }

    SDL_Surface * load_surface_image( const char *filename )
    {
        //The image that's loaded
        SDL_Surface* loadedImage = IMG_Load( filename);

        //If the image loaded
        if( loadedImage != NULL )
        {
            //Return the optimized surface
            SDL_Surface* formatedImage = SDL_ConvertSurfaceFormat(loadedImage,SDL_PIXELFORMAT_RGBA8888,0);
            if( formatedImage!=NULL)
            {
                SDL_FreeSurface(loadedImage);
                return formatedImage;
            }
            return loadedImage;
        }
        return NULL;
    }


    void surface_render_horizontal_line_color_rgba( SDL_Surface * surface, int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a )
    {
        if( surface == NULL )
        {
            return;
        }
        if( surface->w <= x1 && surface->h <= y )
        {
            return;
        }
        int temp = SDL_min( x1, x2 );
        x2 = SDL_max( x1, x2 );
        x1 = temp;
        Uint32 pixel;
        for( int i = x1; i < x2; i++ )
        {
            pixel = SDL_MapRGBA(surface->format, r,g,b,a );
            put_pixel32( surface, i, y, pixel );
        }
    }

    void surface_render_vertical_line_color_rgba( SDL_Surface * surface, int x, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a )
    {
        if( surface == NULL )
        {
            return;
        }
        if( surface->h <= y1 && surface->w <= x )
        {
            return;
        }
        int temp = SDL_min( y1, y2 );
        y2 = SDL_max( y1, y2 );
        y1 = temp;
        Uint32 pixel;
        for( int i = y1; i < y2; i++ )
        {
            pixel = SDL_MapRGBA(surface->format, r,g,b,a );
            put_pixel32( surface, x, i, pixel );
        }
    }


    bool surface_render_circle_color_rgba( SDL_Surface * surface, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a  )
    {
        if( surface == NULL )
        {
            return false;
        }
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
        float alphaRatio = 1;
        if (rad <= 0 )
        {
            return false;
        }
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
                    surface_render_horizontal_line_color_rgba( surface, ypcy,xmcx, xpcx, r, g, b, 255 );
                    surface_render_horizontal_line_color_rgba( surface,  ymcy,xmcx, xpcx, r, g, b, 255);
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
                        surface_render_horizontal_line_color_rgba( surface,  ymcx,xmcy, xpcy, r,g,b,255 );
                        surface_render_horizontal_line_color_rgba( surface,  ypcx,xmcy, xpcy, r,g,b,255 );
                    }
                    else
                    {
                        surface_render_horizontal_line_color_rgba( surface,  y,xmcy, xpcy, r,g,b,255 );
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
        return true;
    }

    //// This code is contributed by PrinciRaj1992 ( modified to return via HSV pointers)
    bool rgb_to_hsv(float r, float g, float b, float *h, float *s, float *v )
    {
        //Divide each color by 255 to begin range from 0.0f to 1.f
        r /= 255.f;
        g /= 255.f;
        b /= 255.f;

        //calculates min and max values of colors
        float max_color_val = fmaxf ( r, fmaxf ( g,b ) );
        float min_color_val = fminf( r, fminf( g,b ) );

        float delta_color = max_color_val - min_color_val;

        //Avoids division by 0;

        *h = 0;
        *s = 0;
        *v = 0;


        if( max_color_val == min_color_val )
        {
            *h = 0;
        }
        else if (r == max_color_val )
        {
            *h = ( 60.f * ( ( g - b ) / delta_color) )+360.f;
        }
        else if (g == max_color_val )
        {
            *h = ( 60.f * ( (b - r) / delta_color) ) + 120.f;
        }
        else if( b == max_color_val )
        {
            *h = ( 60.f * ((r - g) / delta_color) ) + 240.f;
        }

        if( *h < 0.000001f )
        {
            *h = *h + 360.f;
        }

        if( *h > 360.f )
        {
            *h = *h - 360.f;
        }

        if( max_color_val == 0.f)
        {
            *s = 0.f;
        }
        else
        {
            *s = ( delta_color / max_color_val )*255.f;
        }

        *v = max_color_val * 255.f;

        //We made it this far, so "should be successful....
        return true;
    }

    SDL_Surface * surface_grayscale( SDL_Surface *surface)
    {
        if( surface==NULL)
        {
            return NULL;
        }
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        if( coloredSurface!=NULL)
        {
            //coloredSurface = SDL_CreateRGBSurface(0, width, height, 32,SDL_rmask, SDL_gmask, SDL_bmask, SDL_amask);
            //If the surface must be locked
            bool surface_is_locked = SDL_MUSTLOCK( surface );
            if( surface_is_locked )
            {
                //Lock the surface
                SDL_LockSurface( surface );
            }
            Uint8 rr=0, bb=0, gg=0, aa =0;
            //Go through columns
            for( int x = 0; x < coloredSurface->w; x++)
            {
                //Go through rows
                for( int y = 0; y < coloredSurface->h; y++)
                {
                    //Get pixel
                    Uint32 pixel = get_pixel32( surface, x, y );
                    SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);
                    //if the color is a shade of white/black
                    // Uint8 grayedColor=(rr+gg+bb)/3; //old method
                    Uint8 grayedColor = (30 * rr + 59 * gg + 11 * bb) / 100;
                    pixel = SDL_MapRGBA(surface->format,grayedColor,grayedColor,grayedColor,aa);
                    put_pixel32( coloredSurface, x, y, pixel );
                }
            }

            //Unlock surface
            if( surface_is_locked )
            {
                //Lock the surface
                SDL_UnlockSurface( surface );
            }

            //Return coloredSurface surface
            return coloredSurface;
        }
        return NULL;
    }

    SDL_Surface * surface_invert( SDL_Surface *surface)
    {
        if( surface!=NULL)
        {
            //Pointer to the soon to be coloredSurface surface
            SDL_Surface *tempSurface = NULL;
            SDL_Surface *coloredSurface = NULL;

            coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
            if( coloredSurface!=NULL)
            {
                //If the surface must be locked
                bool surface_is_locked = SDL_MUSTLOCK( surface );
                if( surface_is_locked )
                {
                    //Lock the surface
                    SDL_LockSurface( surface );
                }
                Uint8 rr=0, bb=0, gg=0, aa = 0;
                //Go through columns
                for( int x = 0; x < coloredSurface->w; x++)
                {
                    //Go through rows
                    for( int y = 0; y < coloredSurface->h; y++)
                    {
                        //Get pixel
                        Uint32 pixel = get_pixel32( surface, x, y );
                        SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);
                        pixel = SDL_MapRGBA(surface->format,255-rr,255-gg,255-bb,aa);
                        put_pixel32( coloredSurface, x, y, pixel );
                    }
                }

                //Unlock surface
                if( surface_is_locked )
                {
                    SDL_UnlockSurface( surface );
                }

                //Return coloredSurface surface
                return coloredSurface;
            }
            else if( tempSurface!=NULL)
            {
                SDL_FreeSurface(tempSurface);
                tempSurface = NULL;
            }
        }
        return NULL;
    }

    SDL_Surface * surface_modify_only_color( SDL_Surface * surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float difference_allowed, int grayscale_technique )
    {
        if( surface == NULL )
        {
            return NULL;
        }

        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        if( coloredSurface==NULL)
        {
            return NULL;
        }

        float r = color_key_r;
        float g = color_key_g;
        float b = color_key_b;
        float h = 0, s = 0, v = 0;
        float temp_h = 0, temp_s = 0, temp_v = 0;

        rgb_to_hsv( r,g,b, &h, &s, &v );

        //If the surface must be locked
        bool surface_is_locked = SDL_MUSTLOCK( surface );
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_LockSurface( surface );
        }
        Uint8 rr=0, bb=0, gg=0, aa =0;
        bool use_pixel = false;
        Uint32 pixel = 0;
        //Go through columns
        for( int x = 0; x < coloredSurface->w; x++)
        {
            //Go through rows
            for( int y = 0; y < coloredSurface->h; y++)
            {
                //Get pixel
                pixel = get_pixel32( surface, x, y );
                SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);
                rgb_to_hsv( rr,gg,bb, &temp_h, &temp_s, &temp_v );

                use_pixel = false;
                if( hue_within_threshold( temp_h, h, difference_allowed ) )
                {
                    if( within_threshold( temp_s, s, difference_allowed ) || within_threshold( temp_v, v, difference_allowed )  )
                    {
                        use_pixel = true;
                    }
                }

                if( use_pixel == false )
                {
                    if( ( rr == gg && gg == bb)  )
                    {
                        if( grayscale_technique == grayscale_all_shades )
                        {
                            use_pixel = true;
                        }
                        else if( grayscale_technique == grayscale_dark_shades_only && rr <= 64 )
                        {
                            use_pixel = true;
                        }
                        else if( grayscale_technique == grayscale_light_shades_only && rr <= 192 )
                        {
                            use_pixel = true;
                        }
                        else if( grayscale_technique == grayscale_medium_shades_only && ( rr >= 64 && rr <= 192) )
                        {
                            use_pixel = true;
                        }
                        else
                        {
                            use_pixel = false;
                        }
                    }
                    else
                    {
                        use_pixel = false;
                    }
                }

                if( !use_pixel )
                {
                    pixel = SDL_MapRGBA(coloredSurface->format,0,0,0,255);
                }
                put_pixel32( coloredSurface, x, y, pixel );
            }
        }

        //Unlock surface
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_UnlockSurface( surface );
        }

        //Return coloredSurface surface
        SDL_SetColorKey( coloredSurface, SDL_TRUE, SDL_MapRGB( coloredSurface->format, 0,0,0) );

        return coloredSurface;
    }

    SDL_Surface * surface_modify_2bit( SDL_Surface * surface, SDL_Color c1, SDL_Color c2,  SDL_Color c3, SDL_Color c4 )
    {
        if( surface == NULL )
        {
            return NULL;
        }

        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        if( coloredSurface==NULL)
        {
            return NULL;
        }
        //If the surface must be locked
        bool surface_is_locked = SDL_MUSTLOCK( surface );
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_LockSurface( surface );
        }
        Uint8 rr=0, bb=0, gg=0, aa =0;
        bool use_pixel = false;
        Uint32 pixel = 0;
        Uint32 grayedColor = 0;

        Uint32 pixel_c1 = SDL_MapRGBA( surface->format,c1.r,c1.g,c1.b, 255 );
        Uint32 pixel_c2 = SDL_MapRGBA( surface->format,c2.r,c2.g,c2.b, 255 );
        Uint32 pixel_c3 = SDL_MapRGBA( surface->format,c3.r,c3.g,c3.b, 255 );
        Uint32 pixel_c4 = SDL_MapRGBA( surface->format,c4.r,c4.g,c4.b, 255 );


        //Go through columns
        for( int x = 0; x < coloredSurface->w; x++)
        {
            //Go through rows
            for( int y = 0; y < coloredSurface->h; y++)
            {
                //Get pixel
                pixel = get_pixel32( surface, x, y );

                SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);

                grayedColor = (30 * rr + 59 * gg + 11 * bb) / 100;


                if( grayedColor < 64 )
                {
                    pixel = SDL_MapRGBA( surface->format,c1.r,c1.g,c1.b, aa );
                }
                else if( grayedColor < 128)
                {
                    pixel = SDL_MapRGBA( surface->format,c2.r,c2.g,c2.b, aa );
                }
                else if( grayedColor < 192)
                {
                    pixel = SDL_MapRGBA( surface->format,c3.r,c3.g,c3.b, aa );
                }
                else
                {
                    pixel = SDL_MapRGBA( surface->format,c4.r,c4.g,c4.b, aa );
                }

                put_pixel32( coloredSurface, x, y, pixel );
            }
        }

        //Unlock surface
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_UnlockSurface( surface );
        }

        //Return coloredSurface surface
        SDL_SetColorKey( coloredSurface, SDL_TRUE, SDL_MapRGB( coloredSurface->format, 0,0,0) );

        return coloredSurface;
    }

    SDL_Surface * surface_modify_mono_chrome( SDL_Surface * surface, SDL_Color c1, SDL_Color c2  )
    {
        if( surface == NULL )
        {
            return NULL;
        }

        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        if( coloredSurface==NULL)
        {
            return NULL;
        }
        //If the surface must be locked
        bool surface_is_locked = SDL_MUSTLOCK( surface );
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_LockSurface( surface );
        }
        Uint8 rr=0, bb=0, gg=0, aa =0;
        bool use_pixel = false;
        Uint32 pixel = 0;
        Uint32 grayedColor = 0;

        Uint32 pixel_c1 = SDL_MapRGBA( surface->format,c1.r,c1.g,c1.b, 255 );
        Uint32 pixel_c2 = SDL_MapRGBA( surface->format,c2.r,c2.g,c2.b, 255 );


        //Go through columns
        for( int x = 0; x < coloredSurface->w; x++)
        {
            //Go through rows
            for( int y = 0; y < coloredSurface->h; y++)
            {
                //Get pixel
                pixel = get_pixel32( surface, x, y );
                SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);

                grayedColor = (30 * rr + 59 * gg + 11 * bb) / 100;

                if( grayedColor < 128 )
                {
                    pixel = pixel_c1;
                }
                else
                {
                    pixel = pixel_c2;
                }

                put_pixel32( coloredSurface, x, y, pixel );
            }
        }

        //Unlock surface
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_UnlockSurface( surface );
        }

        //Return coloredSurface surface
        SDL_SetColorKey( coloredSurface, SDL_TRUE, SDL_MapRGB( coloredSurface->format, 0,0,0) );

        return coloredSurface;
    }

    SDL_Surface * surface_modify_remove_background( SDL_Surface * surface, float difference_allowed, int grayscale_technique  )
    {
        if( surface == NULL )
        {
            return NULL;
        }

        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        if( coloredSurface==NULL)
        {
            return NULL;
        }

        //If the surface must be locked
        bool surface_is_locked = SDL_MUSTLOCK( surface );
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_LockSurface( surface );
        }
        Uint8 rr=0, bb=0, gg=0, aa =0;
        float temp_h = 0, temp_s = 0, temp_v = 0;
        //Go through columns

        bool keep_pixel = false;
        Uint8 grayedColor  = 0;
        float max_colors = coloredSurface->w * coloredSurface->h;
        float totaled_h = 0, totaled_s = 0, totaled_v = 0;
        float average_h = 0, average_s = 0, average_v = 0;
        Uint32 pixel = 0;
        int x = 0,  y = 0;
        for(  x = 0; x < coloredSurface->w; x++)
        {
            //Go through rows
            for(  y = 0; y < coloredSurface->h; y++)
            {
                //Get pixel
                pixel = get_pixel32( surface, x, y );
                SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);
                rgb_to_hsv( rr,gg,bb, &temp_h, &temp_s, &temp_v );
                totaled_h += temp_h;
                totaled_s += temp_s;
                totaled_v += temp_v;
            }
        }
        average_h = totaled_h / max_colors;
        average_s = totaled_s / max_colors;
        average_v = totaled_v / max_colors;

        for(  x = 0; x < coloredSurface->w; x++)
        {
            //Go through rows
            for(  y = 0; y < coloredSurface->h; y++)
            {
                //Get pixel
                pixel = get_pixel32( surface, x, y );
                SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);
                rgb_to_hsv( rr,gg,bb, &temp_h, &temp_s, &temp_v );

                keep_pixel = false;

                //if(  hue_within_threshold( temp_s, average_s, difference_allowed ) )
                if(  temp_v > average_v - average_v*difference_allowed  )
                {
                    //if(  within_threshold( temp_s, s, difference_allowed ) )
                    {
                        keep_pixel = true;
                    }
                }
                else  if( rr == gg && gg == bb )
                {
                    keep_pixel = true;
                }
                if( !keep_pixel )
                {
                    grayedColor = (30 * rr + 59 * gg + 11 * bb) / 100;
                    pixel = SDL_MapRGBA( coloredSurface->format,0,0,0,0);
                }
                put_pixel32( coloredSurface, x, y, pixel );
            }
        }

        //Unlock surface
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_UnlockSurface( surface );
        }

        //Return coloredSurface surface
        return coloredSurface;
    }


    SDL_Surface * surface_modify_remove_color( SDL_Surface * surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float difference_allowed, int grayscale_technique )
    {
        if( surface == NULL )
        {
            return NULL;
        }

        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        if( coloredSurface==NULL)
        {
            return NULL;
        }

        float r = color_key_r;
        float g = color_key_g;
        float b = color_key_b;
        float h = 0, s = 0, v = 0;
        float temp_h = 0, temp_s = 0, temp_v = 0;

        rgb_to_hsv( r,g,b, &h, &s, &v );

        //If the surface must be locked
        bool surface_is_locked = SDL_MUSTLOCK( surface );
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_LockSurface( surface );
        }
        Uint8 rr=0, bb=0, gg=0, aa =0;
        bool use_pixel = false;
        Uint32 pixel = 0;
        //Go through columns
        for( int x = 0; x < coloredSurface->w; x++)
        {
            //Go through rows
            for( int y = 0; y < coloredSurface->h; y++)
            {
                //Get pixel
                pixel = get_pixel32( surface, x, y );
                SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);
                rgb_to_hsv( rr,gg,bb, &temp_h, &temp_s, &temp_v );

                use_pixel = false;
                if( hue_within_threshold( temp_h, h, difference_allowed ) )
                {
                    //if( within_threshold( temp_s, s, difference_allowed ) || within_threshold( temp_v, v, difference_allowed )  )
                    {
                        use_pixel = true;
                    }
                }
                else
                {
                    if( ( rr == gg && gg == bb)  )
                    {
                        if( grayscale_technique == grayscale_all_shades )
                        {
                            use_pixel = true;
                        }
                        else if( grayscale_technique == grayscale_dark_shades_only && temp_v <= 64 )
                        {
                            use_pixel = true;
                        }
                        else if( grayscale_technique == grayscale_light_shades_only && temp_v <= 192 )
                        {
                            use_pixel = true;
                        }
                        else if( grayscale_technique == grayscale_medium_shades_only && ( temp_v >= 64 && temp_v <= 192) )
                        {
                            use_pixel = true;
                        }
                        else
                        {
                            use_pixel = false;
                        }
                    }
                    else
                    {
                        use_pixel = false;
                    }
                }

                //Exact opposite of the show only function, should make the selected color black
                if( use_pixel )
                {
                    if( temp_v < 32 )
                    {
                        pixel = SDL_MapRGBA(surface->format,0,0,0,aa);
                    }
                    else if( temp_v < 64)
                    {
                        pixel = SDL_MapRGBA(surface->format,32,32,32,aa);
                    }
                    else if( temp_v < 96 )
                    {
                        pixel = SDL_MapRGBA(surface->format,64,64,64,aa);
                    }
                    else if( temp_v < 128 )
                    {
                        pixel = SDL_MapRGBA(surface->format,96,96,96,aa);
                    }
                    else if( temp_v < 160 )
                    {
                        pixel = SDL_MapRGBA(surface->format,128,128,128,aa);
                    }
                    else if( temp_v < 192 )
                    {
                        pixel = SDL_MapRGBA(surface->format,160,160,160,aa);

                    }
                    else if( temp_v < 224  )
                    {
                        pixel = SDL_MapRGBA(surface->format,192,192,192,aa);
                    }
                    else
                    {
                        pixel = SDL_MapRGBA(surface->format,228,228,228,aa);
                    }
                }

                put_pixel32( coloredSurface, x, y, pixel );
            }
        }

        //Unlock surface
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_UnlockSurface( surface );
        }

        //Return coloredSurface surface
        SDL_SetColorKey( coloredSurface, SDL_TRUE, SDL_MapRGB( coloredSurface->format, r,g,b) );

        return coloredSurface;
    }

    SDL_Surface * surface_modify_selective_color( SDL_Surface * surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float difference_allowed, int grayscale_technique )
    {
        if( surface == NULL )
        {
            return NULL;
        }

        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        if( coloredSurface==NULL)
        {
            return NULL;
        }

        float r = color_key_r;
        float g = color_key_g;
        float b = color_key_b;
        float h = 0, s = 0, v = 0;
        float temp_h = 0, temp_s = 0, temp_v = 0;

        rgb_to_hsv( r,g,b, &h, &s, &v );

        //If the surface must be locked
        bool surface_is_locked = SDL_MUSTLOCK( surface );
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_LockSurface( surface );
        }
        Uint8 rr=0, bb=0, gg=0, aa =0;
        //Go through columns

        bool keep_pixel = false;
        bool use_pixel = false;
        Uint8 grayedColor  = 0;
        for( int x = 0; x < coloredSurface->w; x++)
        {
            //Go through rows
            for( int y = 0; y < coloredSurface->h; y++)
            {
                //Get pixel
                Uint32 pixel = get_pixel32( surface, x, y );

                SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);

                grayedColor = (30.f * rr + 59.f * gg + 11.f * bb) / 100.f;
                rgb_to_hsv( rr,gg,bb, &temp_h, &temp_s, &temp_v );

                keep_pixel = false;
                use_pixel = false;

                if(  hue_within_threshold( temp_h, h, difference_allowed ) )
                {
                    //if(  within_threshold( temp_s, s, difference_allowed*2.f ) || within_threshold( temp_v, v, difference_allowed*2.f )  )
                    {
                        keep_pixel = true;
                    }
                }
                else
                {
                    use_pixel = false;
                    if( grayscale_technique == grayscale_all_shades )
                    {
                        use_pixel = true;
                    }
                    else if( temp_v <= 64 )
                    {
                        if( grayscale_technique == grayscale_dark_shades_only || grayscale_technique == grayscale_light_and_dark_shades_only || grayscale_technique == grayscale_medium_and_dark_shades_only )
                        {
                            use_pixel = true;
                        }
                    }
                    else if(  temp_v > 192 )
                    {
                        if( grayscale_technique == grayscale_light_shades_only || grayscale_technique == grayscale_medium_and_light_only || grayscale_technique == grayscale_light_and_dark_shades_only )
                        {
                            use_pixel = true;
                        }
                    }
                    else
                    {
                        if( grayscale_technique == grayscale_medium_shades_only || grayscale_technique == grayscale_medium_and_light_only || grayscale_technique == grayscale_medium_and_dark_shades_only )
                        {
                            use_pixel = true;
                        }
                    }
                }

                if( !keep_pixel )
                {
                    if( use_pixel )
                    {
                        pixel = SDL_MapRGBA(surface->format,grayedColor,grayedColor,grayedColor,aa);
                    }
                    else if( grayscale_technique == grayscale_no_shades )
                    {
                        pixel = SDL_MapRGBA(surface->format,0,0,0,aa/2);
                    }
                    else
                    {
                        aa = aa / 2;
                        if( temp_v < 32 )
                        {
                            pixel = SDL_MapRGBA(surface->format,0,0,0,aa);
                        }
                        else if( temp_v < 64)
                        {
                            pixel = SDL_MapRGBA(surface->format,32,32,32,aa);
                        }
                        else if( temp_v < 96 )
                        {
                            pixel = SDL_MapRGBA(surface->format,64,64,64,aa);
                        }
                        else if( temp_v < 128 )
                        {
                            pixel = SDL_MapRGBA(surface->format,96,96,96,aa);
                        }
                        else if( temp_v < 160 )
                        {
                            pixel = SDL_MapRGBA(surface->format,128,128,128,aa);
                        }
                        else if( temp_v < 192 )
                        {
                            pixel = SDL_MapRGBA(surface->format,160,160,160,aa);

                        }
                        else if( temp_v < 224  )
                        {
                            pixel = SDL_MapRGBA(surface->format,192,192,192,aa);
                        }
                        else
                        {
                            pixel = SDL_MapRGBA(surface->format,228,228,228,aa);
                        }
                    }
                }
                put_pixel32( coloredSurface, x, y, pixel );
            }
        }

        //Unlock surface
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_UnlockSurface( surface );
        }

        //Return coloredSurface surface
        return coloredSurface;
    }

    SDL_Surface * surface_modify_selective_color_list( SDL_Surface * surface, std::vector <SDL_Color > &colorList, float difference_allowed, int grayscale_technique, int primary_color, float primary_color_different_allowed, bool sv_matters  )
    {
        if( surface == NULL )
        {
            return NULL;
        }

        int color_size = (int) colorList.size();
        if( color_size == 0 )
        {
            return NULL;
        }

        if( color_size == 1 )
        {
            return surface_modify_selective_color( surface, colorList[0].r,colorList[0].g,colorList[0].b, difference_allowed, grayscale_technique );
        }
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        if( coloredSurface==NULL)
        {
            return NULL;
        }

        float r = 0;
        float g = 0;
        float b = 0;
        float h = 0, s = 0, v = 0;
        float temp_h = 0, temp_s = 0, temp_v = 0;

        int i_col = 0;

        std::vector < float > colors_h;
        std::vector < float > colors_s;
        std::vector < float > colors_v;

        for(  i_col = 0; i_col < color_size; i_col++ )
        {
            r = colorList[i_col].r;
            g = colorList[i_col].g;
            b = colorList[i_col].b;
            rgb_to_hsv( r,g,b, &h, &s, &v );
            colors_h.push_back( h );
            colors_s.push_back( s );
            colors_v.push_back( v );
        }
        //If the surface must be locked
        bool surface_is_locked = SDL_MUSTLOCK( surface );
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_LockSurface( surface );
        }
        Uint8 rr=0, bb=0, gg=0, aa =0;
        //Go through columns

        bool matches_at_least_one_color = false;
        bool keep_pixel = false;
        bool use_pixel = false;
        Uint8 grayedColor  = 0;
        for( int x = 0; x < coloredSurface->w; x++)
        {
            //Go through rows
            for( int y = 0; y < coloredSurface->h; y++)
            {
                //Get pixel
                Uint32 pixel = get_pixel32( surface, x, y );

                SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);

                grayedColor = (30.f * rr + 59.f * gg + 11.f * bb) / 100.f;
                rgb_to_hsv( rr,gg,bb, &temp_h, &temp_s, &temp_v );

                keep_pixel = false;
                use_pixel = false;
                matches_at_least_one_color = false;
                for(  i_col = 0; i_col < color_size; i_col++ )
                {
                    h = colors_h[i_col];
                    s = colors_s[i_col];
                    v = colors_v[i_col];
                    if( i_col == primary_color )
                    {
                        if(  hue_within_threshold( temp_h, h, primary_color_different_allowed ) )
                        {
                            if(  !sv_matters )
                            {
                                matches_at_least_one_color = true;
                                keep_pixel = true;
                                break;
                            }
                            else if(  within_threshold( temp_s, s, primary_color_different_allowed ) || within_threshold( temp_v, v, primary_color_different_allowed )  )
                            {
                                matches_at_least_one_color = true;
                                keep_pixel = true;
                                break;
                            }
                        }
                    }
                    else if(  hue_within_threshold( temp_h, h, difference_allowed ) )
                    {
                        if(  !sv_matters)
                        {
                            matches_at_least_one_color = true;
                            keep_pixel = true;
                            break;
                        }
                        else if(   within_threshold( temp_s, s, difference_allowed ) || within_threshold( temp_v, v, difference_allowed)  )
                        {
                            matches_at_least_one_color = true;
                            keep_pixel = true;
                            break;
                        }
                        else
                        {
                            keep_pixel = false;
                            matches_at_least_one_color = false;
                        }
                    }
                }


                colors_h.clear();
                colors_s.clear();
                colors_v.clear();
                if( matches_at_least_one_color == false )
                {
                    use_pixel = false;
                    if( grayscale_technique == grayscale_all_shades )
                    {
                        use_pixel = true;
                    }
                    else if( temp_v <= 64 )
                    {
                        if( grayscale_technique == grayscale_dark_shades_only || grayscale_technique == grayscale_light_and_dark_shades_only || grayscale_technique == grayscale_medium_and_dark_shades_only )
                        {
                            use_pixel = true;
                        }
                    }
                    else if(  temp_v > 192 )
                    {
                        if( grayscale_technique == grayscale_light_shades_only || grayscale_technique == grayscale_medium_and_light_only || grayscale_technique == grayscale_light_and_dark_shades_only )
                        {
                            use_pixel = true;
                        }
                    }
                    else
                    {
                        if( grayscale_technique == grayscale_medium_shades_only || grayscale_technique == grayscale_medium_and_light_only || grayscale_technique == grayscale_medium_and_dark_shades_only )
                        {
                            use_pixel = true;
                        }
                    }
                }

                if( !keep_pixel )
                {
                    if( use_pixel )
                    {
                        pixel = SDL_MapRGBA(surface->format,grayedColor,grayedColor,grayedColor,aa);
                    }
                    else if( grayscale_technique == grayscale_no_shades )
                    {
                        pixel = SDL_MapRGBA(surface->format,0,0,0,aa/2);
                    }
                    else
                    {
                        aa = aa / 2;
                        if( temp_v < 32 )
                        {
                            pixel = SDL_MapRGBA(surface->format,0,0,0,aa);
                        }
                        else if( temp_v < 64)
                        {
                            pixel = SDL_MapRGBA(surface->format,32,32,32,aa);
                        }
                        else if( temp_v < 96 )
                        {
                            pixel = SDL_MapRGBA(surface->format,64,64,64,aa);
                        }
                        else if( temp_v < 128 )
                        {
                            pixel = SDL_MapRGBA(surface->format,96,96,96,aa);
                        }
                        else if( temp_v < 160 )
                        {
                            pixel = SDL_MapRGBA(surface->format,128,128,128,aa);
                        }
                        else if( temp_v < 192 )
                        {
                            pixel = SDL_MapRGBA(surface->format,160,160,160,aa);

                        }
                        else if( temp_v < 224  )
                        {
                            pixel = SDL_MapRGBA(surface->format,192,192,192,aa);
                        }
                        else
                        {
                            pixel = SDL_MapRGBA(surface->format,228,228,228,aa);
                        }
                    }
                }
                put_pixel32( coloredSurface, x, y, pixel );
            }
        }

        //Unlock surface
        if( surface_is_locked )
        {
            //Lock the surface
            SDL_UnlockSurface( surface );
        }

        //Return coloredSurface surface
        return coloredSurface;
    }

    void surface_merge_color_rgba( SDL_Surface * surface, int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a )
    {
        if( surface == NULL )
        {
            return;
        }
        if( surface->w <= x1 && surface->h <= y )
        {
            return;
        }
        int temp = SDL_min( x1, x2 );
        x2 = SDL_max( x1, x2 );
        x1 = temp;
        Uint32 pixel;
        for( int i = x1; i < x2; i++ )
        {
            pixel = SDL_MapRGBA(surface->format, r,g,b,a );
            put_pixel32( surface, i, y, pixel );
        }
    }

    SDL_Surface * surface_recolor( SDL_Surface *surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, float amount )
    {
        if( surface!=NULL )
        {
            //Pointer to the soon to be coloredSurface surface
            SDL_Surface *tempSurface = NULL;
            SDL_Surface *coloredSurface = NULL;

            //If the image is color keyed
            coloredSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
            if( coloredSurface!=NULL)
            {
                //If the surface must be locked
                bool surface_is_locked = SDL_MUSTLOCK( surface );
                if( surface_is_locked )
                {
                    //Lock the surface
                    SDL_LockSurface( surface );
                }

                Uint8 rr=0, bb=0, gg=0, aa = 0;
                //Go through columns
                for( int x = 0; x < coloredSurface->w; x++)
                {
                    //Go through rows
                    for( int y = 0; y < coloredSurface->h; y++)
                    {
                        //Get pixel
                        Uint32 pixel = get_pixel32( surface, x, y );
                        SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);

                        rr = merge_channel(rr,color_key_r,amount);
                        gg = merge_channel(gg,color_key_g,amount);
                        bb = merge_channel(bb,color_key_b,amount);
                        pixel = SDL_MapRGBA(surface->format,rr,gg,bb, aa);
                        put_pixel32( coloredSurface, x, y, pixel );
                    }
                }

                //Unlock surface
                if( surface_is_locked )
                {
                    SDL_UnlockSurface( surface );
                }

                //Return coloredSurface surface
                return coloredSurface;
            }
        }
        return NULL;
    }

    SDL_Surface * surface_remove_color_rgba( SDL_Surface *surface, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b )
    {
        if( surface!=NULL )
        {
            //Pointer to the soon to be coloredSurface surface
            SDL_Surface *coloredSurface = NULL;

            coloredSurface = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGBA8888,0);
            if( coloredSurface!=NULL)
            {
                //If the surface must be locked
                bool surface_is_locked = SDL_MUSTLOCK( coloredSurface );
                if( surface_is_locked )
                {
                    //Lock the surface
                    SDL_LockSurface( coloredSurface );
                }

                Uint8 rr=0, bb=0, gg=0, aa = 0;
                int y = 0;
                Uint32 pixel;
                //Go through columns
                for( int x = 0; x < coloredSurface->w; x++)
                {
                    //Go through rows
                    for(  y = 0; y < coloredSurface->h; y++)
                    {
                        //Get pixel
                        pixel = get_pixel32( coloredSurface, x, y );
                        SDL_GetRGBA(pixel,coloredSurface->format,&rr,&gg,&bb, &aa);
                        //if the color is shade of white/gray
                        if( rr==color_key_r && gg==color_key_g && bb==color_key_b)
                        {
                            pixel = SDL_MapRGBA(coloredSurface->format,255,0,255,0);
                        }
                        put_pixel32( coloredSurface, x, y, pixel );
                    }
                }

                //Unlock surface
                if( surface_is_locked )
                {
                    SDL_UnlockSurface( coloredSurface );
                }
                return coloredSurface;
            }
        }
        return NULL;
    }

    SDL_Surface * surface_flip( SDL_Surface *surface, int flags )
    {
        if( surface!=NULL)
        {
            //Pointer to the soon to be flipped surface
            SDL_Surface *flipped = NULL;

            flipped = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGBA8888,0);
            if( flipped!=NULL)
            {
                //If the surface must be locked
                bool surface_is_locked = SDL_MUSTLOCK( flipped );
                if( surface_is_locked )
                {
                    //Lock the surface
                    SDL_LockSurface( flipped );
                }

                //Go through columns
                for( int x = 0, rx = flipped->w - 1; x < flipped->w; x++, rx-- )
                {
                    //Go through rows
                    for( int y = 0, ry = flipped->h - 1; y < flipped->h; y++, ry-- )
                    {
                        //Get pixel
                        Uint32 pixel = get_pixel32( surface, x, y );

                        //Copy pixel
                        if( ( flags & SDL_FLIP_VERTICAL ) && ( flags & SDL_FLIP_HORIZONTAL ) )
                        {
                            put_pixel32( flipped, rx, ry, pixel );
                        }
                        else if( flags & SDL_FLIP_HORIZONTAL )
                        {
                            put_pixel32( flipped, rx, y, pixel );
                        }
                        else if( flags & SDL_FLIP_VERTICAL )
                        {
                            put_pixel32( flipped, x, ry, pixel );
                        }
                    }
                }

                //Unlock surface
                if( surface_is_locked )
                {
                    SDL_UnlockSurface( flipped );
                }

                //Return flipped surface
                return flipped;
            }
        }
        return NULL;
    }

    SDL_Surface * surface_sephia( SDL_Surface *surface )
    {
        if( surface==NULL)
        {
            return NULL;
        }
        //Pointer to the soon to be sephiadSurface surface
        SDL_Surface * sephiadSurface = NULL;
        //If the image is color keyed
        sephiadSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        if( sephiadSurface!=NULL)
        {
            //sephiadSurface = SDL_CreateRGBSurface(0, width, height, 32,SDL_rmask, SDL_gmask, SDL_bmask, SDL_amask);
            //If the surface must be locked
            bool surface_is_locked = SDL_MUSTLOCK( surface );
            if( surface_is_locked )
            {
                //Lock the surface
                SDL_LockSurface( surface );
            }
            Uint8 rr = 0, gg = 0, bb = 0, aa =0;
            float rfloat = 0, gfloat = 0, bfloat = 0;
            int r_sephia = 0, g_sephia = 0, b_sephia = 0;

            //Go through columns
            for( int x = 0; x < sephiadSurface->w; x++)
            {
                //Go through rows
                for( int y = 0; y < sephiadSurface->h; y++)
                {
                    //Get pixel
                    Uint32 pixel = get_pixel32( surface, x, y );
                    SDL_GetRGBA(pixel,surface->format,&rr,&gg,&bb, &aa);

                    rfloat = rr;
                    gfloat = gg;
                    bfloat = bb;

                    r_sephia = (int)(0.393*rfloat + 0.769*gfloat + 0.189*bfloat );
                    g_sephia = (int)(0.349*rfloat + 0.686*gfloat + 0.168*bfloat );
                    b_sephia = (int)(0.272*rfloat + 0.534*gfloat + 0.131*bfloat );

                    if( r_sephia > 255 )
                    {
                        r_sephia = 255;
                    }

                    if( g_sephia > 255 )
                    {
                        g_sephia = 255;
                    }

                    if( b_sephia > 255 )
                    {
                        b_sephia = 255;
                    }

                    pixel = SDL_MapRGBA(surface->format,r_sephia, g_sephia, b_sephia,aa);
                    put_pixel32( sephiadSurface, x, y, pixel );
                }
            }

            //Unlock surface
            if( surface_is_locked )
            {
                //Lock the surface
                SDL_UnlockSurface( surface );
            }

            //Return coloredSurface surface
            return sephiadSurface;
        }
        return NULL;
    }

    bool hue_within_threshold( float value, float base, float difference )
    {
        if( difference < 0.00001f )
        {
            difference *= -1.f;
        }

        if( difference >= 1.000f )
        {
            return true;
        }

        difference = difference * 180.f;

        if( ( value <= base + difference ) && (value >= base ) )
        {
            return true;
        }

        if( ( value >= base - difference ) && value <= base )
        {
            return true;
        }


        float negative_direction = 0;
        if( base - difference <= -0.000001f )
        {
            negative_direction = (base - difference);
            if( value >=  360.f - negative_direction )
            {
                return true;
            }
        }

        if(  base + difference  >= 360.f )
        {
            negative_direction = base + difference - 360.f;
            if( value <=  negative_direction )
            {
                return true;
            }
        }

        return false;
    }

    bool within_threshold( float value, float base, float difference )
    {
        if( difference < 0 )
        {
            difference *= -1.f;
        }

        if( difference >= 1.000f )
        {
            return true;
        }
        difference = difference * 128.f;

        if( ( value <= base + (difference) ) && value >= base )
        {
            return true;
        }

        if( ( value >= base - (difference) )&& value <= base )
        {
            return true;
        }

        return false;
    }

}

