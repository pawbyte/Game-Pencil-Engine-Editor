/*
sdl_surface_ex.cpp
This file is part of:
sdl_surface_ex
https://www.pawbyte.com/opensource
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.

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

-Game Pencil Engine <https://www.pawbyte.com/opensource>


*/

#include "sdl_surface_ex.h"

namespace sdl_surface_ex
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

    Uint8 merge_channel(Uint8 a, Uint8 b, float amount)
    {
        float result = (b * amount) + (a * (1.0f - amount));
        return (Uint8)result;
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

    SDL_Texture * create_texture_from_surface( SDL_Renderer * sdlRenderer, SDL_Surface * surf,  int format , bool destroySurface  )
    {
        if( surf == NULL)
        {
            return NULL;
        }

        if( sdlRenderer == NULL )
        {
            if( destroySurface )
            {
                SDL_FreeSurface( surf );
                surf = NULL;
            }
            return NULL;
        }
        SDL_Surface * cast_img = SDL_ConvertSurfaceFormat(surf, format, 0 );
        SDL_Rect rect = { 0,0,cast_img->w,cast_img->h};
        SDL_Texture * newTexture = SDL_CreateTexture( sdlRenderer, format, SDL_TEXTUREACCESS_STATIC, cast_img->w, cast_img->h );
        SDL_UpdateTexture(newTexture, &rect, cast_img->pixels,cast_img->w*4);
        SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
        SDL_FreeSurface(cast_img);
        if( destroySurface )
        {
            SDL_FreeSurface( surf );
            surf = NULL;
        }
        return newTexture;
    }

    SDL_Surface * create_filled_surface_rgba(int w, int h, Uint8 color_key_r, Uint8 color_key_g, Uint8 color_key_b, Uint8 alpha  )
    {
        if( w <= 0 || h  <= 0 )
        {
            return NULL;
        }
        SDL_Surface * newSurface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888 );
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
                    surface_render_horizontal_line_color_rgba( surface, ypcy,xmcx, xpcx, r, g, b, a );
                    surface_render_horizontal_line_color_rgba( surface,  ymcy,xmcx, xpcx, r, g, b, a );
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
                        surface_render_horizontal_line_color_rgba( surface,  ymcx,xmcy, xpcy, r,g,b,a );
                        surface_render_horizontal_line_color_rgba( surface,  ypcx,xmcy, xpcy, r,g,b,a );
                    }
                    else
                    {
                        surface_render_horizontal_line_color_rgba( surface,  y,xmcy, xpcy, r,g,b,a );
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
                    Uint8 grayedColor=(rr+gg+bb)/3;
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
}

