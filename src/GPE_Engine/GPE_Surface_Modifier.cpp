/*
GPE_SDL_Surface_Modifier.h
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

#include "GPE_Surface_Modifier.h"

GPE_SDL_Surface_Modifier * gpe_sdl = NULL;


GPE_SDL_Surface_Modifier::GPE_SDL_Surface_Modifier()
{
    defaultColor.r = 255;
    defaultColor.g = 255;
    defaultColor.b = 255;
    defaultColor.a = 255;
}

GPE_SDL_Surface_Modifier::~GPE_SDL_Surface_Modifier()
{

}

Uint32 GPE_SDL_Surface_Modifier::get_pixel32( SDL_Surface *surface, int x, int y )
{
    if( surface!=NULL)
    {
        if( x>=0 && x < surface->w && y>=0 && y < surface->h)
        {
            Uint32 *ptr = (Uint32*)surface->pixels;
            //GPE_Report( double_to_string(surface->pitch) );
            int lineoffset = y * (surface->pitch/4 );
            return ptr[lineoffset + x ];
        }
    }
    return 0;
}

void GPE_SDL_Surface_Modifier::put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
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

SDL_Texture * GPE_SDL_Surface_Modifier::create_texture_from_surface( SDL_Renderer * sdlRenderer, SDL_Surface * surf, int format , SDL_Color * sdlColorKey, bool destroySurface  )
{
    if( surf == NULL || sdlRenderer == NULL )
    {
        return NULL;
    }
    int pixelbytes=0;
    Uint8 r,g,b,a;
    SDL_Surface* cast_img=SDL_ConvertSurfaceFormat(surf, format, 0 );
    pixelbytes=cast_img->w*cast_img->h;
    pixelbytes=pixelbytes*4;
    unsigned char* pixels = (unsigned char*)cast_img->pixels;
    if( sdlColorKey!=NULL )
    {
        for( int i=0;i<pixelbytes;i++ )
        {
            a=pixels[i+0];
            r=pixels[i+1];
            g=pixels[i+2];
            b=pixels[i+3];
            if( r==sdlColorKey->r && g==sdlColorKey->g && b==sdlColorKey->b )
            {
                pixels[i+0]=0;
            }
        }
    }
    SDL_Rect rect = { 0,0,cast_img->w,cast_img->h};
    SDL_Texture * newTexture = SDL_CreateTexture( sdlRenderer, format, SDL_TEXTUREACCESS_STATIC, cast_img->w, cast_img->h );
    SDL_UpdateTexture(newTexture, &rect, cast_img->pixels,cast_img->w*4);
    SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
    SDL_FreeSurface(cast_img);
    if( destroySurface )
    {
        SDL_FreeSurface( surf );
    }
    return newTexture;
    /*
    SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat( surf, format, NULL );
    if( convertedSurface == NULL )
    {
        GPE_Report( "Surface Conversion Failed!", SDL_GetError() );
        return NULL;
    }
    //Create blank streamable texture IN HOPES it can be used everywhere
    SDL_Texture * newTexture = SDL_CreateTexture( sdlRenderer, format, SDL_TEXTUREACCESS_STREAMING, convertedSurface->w, convertedSurface->h );
    if( newTexture == NULL )
    {
        GPE_Report( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
        return NULL;
    }
    SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );
    //Lock texture for manipulation
    void* texPixels;
    int texPitch = 0;
    SDL_LockTexture( newTexture, &convertedSurface->clip_rect, &texPixels, &texPitch );

    //Copy loaded/formatted surface pixels
    memcpy( texPixels, convertedSurface->pixels, convertedSurface->pitch * convertedSurface->h );

    //Get pixel data in editable format
    Uint32* pixels = (Uint32*)texPixels;
    int pixelCount = ( texPitch / 4 ) * convertedSurface->h;


    //Map colors
    if( sdlColorKey!=NULL )
    {
        Uint32 colorKey = SDL_MapRGB( convertedSurface->format, sdlColorKey->r, sdlColorKey->g, sdlColorKey->b );
        //Uint32 transparent = SDL_MapRGBA( convertedSurface->format, 0x00, 0xFF, 0xFF, 0x00 );
        Uint32 transparentColor = SDL_MapRGBA( convertedSurface->format, 0, 0, 0, 0x00 );

        //Copy all the pixels
        for( int i = 0; i < pixelCount; i++ )
        {
            if( pixels[ i ] == colorKey )
            {
                pixels[ i ] = transparentColor;
            }
        }
    }

    //Unlock texture to update
    SDL_UnlockTexture( newTexture );
    texPixels = NULL;

    SDL_FreeSurface( convertedSurface );

    if( destroySurface )
    {
        SDL_FreeSurface( surf );
    }

    return newTexture;
    */
    return NULL;
}

SDL_Surface * GPE_SDL_Surface_Modifier::create_filled_surface(int w, int h, SDL_Color color, int alpha  )
{
    if( w <= 0 || h  <= 0 )
    {
        return NULL;
    }
    SDL_Surface * newSurface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888 );
    if (newSurface == NULL)
    {
        GPE_Report(  SDL_GetError() );
        return NULL;
    }
    int x = 0, y = 0;
    if( SDL_MUSTLOCK( newSurface ) )
    {
        //Lock the surface
        SDL_LockSurface( newSurface );
    }
    Uint8 rr=0, bb=0, gg=0, aa =0;
    /*if( color!=NULL )
    {*/
    //Go through columns
    for(  x = 0; x < newSurface->w; x++)
    {
        //Go through rows
        for(  y = 0; y < newSurface->h; y++)
        {
            //Get pixel
            Uint32 pixel = SDL_MapRGBA(newSurface->format,color.r,color.g,color.b, alpha );
            put_pixel32( newSurface, x, y, pixel );
        }
    }
    /*
    }
    else
    {
        //Go through columns
        for(  x = 0; x < newSurface->w; x++)
        {
            //Go through rows
            for(  y = 0;y < newSurface->h; y++)
            {
                //Get pixel
                Uint32 pixel = SDL_MapRGBA(newSurface->format,255, 255, 255, 255 );
                put_pixel32( newSurface, x, y, pixel );
            }
        }
    }
    */
    return newSurface;
}

SDL_Surface * GPE_SDL_Surface_Modifier::load_surface_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        //loadedImage = SDL_DisplayFormatAlpha( loadedImage );

        //If the surface was optimized
        //Color key surface
        //SDL_SetColorKey( loadedImage, SDL_TRUE, SDL_MapRGBA( loadedImage->format, 255, 0, 255,255 ) );

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

void GPE_SDL_Surface_Modifier::surface_render_horizontal_line( SDL_Surface * surface, int y, int x1, int x2)
{
    if( surface == NULL )
    {
        return;
    }
    surface_render_horizontal_line_color( surface, y, x1, x2, defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a );
}

void GPE_SDL_Surface_Modifier::surface_render_horizontal_line_color( SDL_Surface * surface, int y, int x1, int x2, Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{
    if( surface == NULL )
    {
        return;
    }
    if( surface->w <= x1 && surface->h <= y )
    {
        return;
    }
    int temp = std::min( x1, x2 );
    x2 = std::max( x1, x2 );
    x1 = temp;
    Uint32 pixel;
    for( int i = x1; i < x2; i++ )
    {
        pixel = SDL_MapRGBA(surface->format, r,g,b,a );
        put_pixel32( surface, i, y, pixel );
    }
}

void GPE_SDL_Surface_Modifier::surface_render_vertical_line( SDL_Surface * surface, int x, int y1, int y2 )
{
    if( surface == NULL )
    {
        return;
    }
    surface_render_vertical_line_color( surface, x, y1, y2, defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a );
}

void GPE_SDL_Surface_Modifier::surface_render_vertical_line_color( SDL_Surface * surface, int x, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{
    if( surface == NULL )
    {
        return;
    }
    if( surface->h <= y1 && surface->w <= x )
    {
        return;
    }
    int temp = std::min( y1, y2 );
    y2 = std::max( y1, y2 );
    y1 = temp;
    Uint32 pixel;
    for( int i = y1; i < y2; i++ )
    {
        pixel = SDL_MapRGBA(surface->format, r,g,b,a );
        put_pixel32( surface, x, i, pixel );
    }
}


bool GPE_SDL_Surface_Modifier::surface_render_circle_color( SDL_Surface * surface, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a  )
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
    double alphaRatio = 1;
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
                surface_render_horizontal_line_color( surface, ypcy,xmcx, xpcx, r, g, b, a );
                surface_render_horizontal_line_color( surface,  ymcy,xmcx, xpcx, r, g, b, a );
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
                    surface_render_horizontal_line_color( surface,  ymcx,xmcy, xpcy, r,g,b,a );
                    surface_render_horizontal_line_color( surface,  ypcx,xmcy, xpcy, r,g,b,a );
                }
                else
                {
                    surface_render_horizontal_line_color( surface,  y,xmcy, xpcy, r,g,b,a );
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

SDL_Surface * GPE_SDL_Surface_Modifier::surface_grayscale( SDL_Surface *surface)
{
    if( surface!=NULL)
    {
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *tempSurface = NULL;
        SDL_Surface *coloredSurface = NULL;
        //If the image is color keyed
        tempSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        coloredSurface = SDL_ConvertSurfaceFormat(tempSurface,SDL_PIXELFORMAT_RGBA8888,0);
        if( coloredSurface!=NULL)
        {
            SDL_FreeSurface(tempSurface);
            //coloredSurface = SDL_CreateRGBSurface(0, width, height, 32,SDL_rmask, SDL_gmask, SDL_bmask, SDL_amask);
            //If the surface must be locked
            if( SDL_MUSTLOCK( surface ) )
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
            if( SDL_MUSTLOCK( surface ) )
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

SDL_Surface * GPE_SDL_Surface_Modifier::surface_invert( SDL_Surface *surface)
{
    if( surface!=NULL)
    {
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *tempSurface = NULL;
        SDL_Surface *coloredSurface = NULL;

        tempSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        coloredSurface = SDL_ConvertSurfaceFormat(tempSurface,SDL_PIXELFORMAT_RGBA8888,0);
        if( coloredSurface!=NULL)
        {
            SDL_FreeSurface(tempSurface);
            //If the surface must be locked
            if( SDL_MUSTLOCK( surface ) )
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
            if( SDL_MUSTLOCK( surface ) )
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

SDL_Surface * GPE_SDL_Surface_Modifier::surface_recolor( SDL_Surface *surface, SDL_Color colorMerge, double amount )
{
    if( surface!=NULL)
    {
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *tempSurface = NULL;
        SDL_Surface *coloredSurface = NULL;

        //If the image is color keyed
        tempSurface = SDL_CreateRGBSurface( 0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
        coloredSurface = SDL_ConvertSurfaceFormat(tempSurface,SDL_PIXELFORMAT_RGBA8888,0);
        if( coloredSurface!=NULL)
        {
            SDL_FreeSurface(tempSurface);
            //If the surface must be locked
            if( SDL_MUSTLOCK( surface ) )
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

                    rr=merge_channel(rr,colorMerge.r,amount);
                    gg=merge_channel(gg,colorMerge.g,amount);
                    bb=merge_channel(bb,colorMerge.b,amount);
                    pixel = SDL_MapRGBA(surface->format,rr,gg,bb, aa);
                    put_pixel32( coloredSurface, x, y, pixel );
                }
            }

            //Unlock surface
            if( SDL_MUSTLOCK( surface ) )
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

SDL_Surface * GPE_SDL_Surface_Modifier::surface_remove_color( SDL_Surface *surface, SDL_Color colorToRemove )
{
    //GPE_Report("Removing Color.... rgb("+int_to_string( colorToRemove.r)+","+int_to_string( colorToRemove.g)+","+int_to_string( colorToRemove.b)+").");
    if( surface!=NULL)
    {
        //Pointer to the soon to be coloredSurface surface
        SDL_Surface *coloredSurface = NULL;

        coloredSurface = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGBA8888,0);
        if( coloredSurface!=NULL)
        {
            //If the surface must be locked
            if( SDL_MUSTLOCK( coloredSurface ) )
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
                    if(rr==colorToRemove.r&&gg==colorToRemove.g&&bb==colorToRemove.b)
                    {
                        pixel = SDL_MapRGBA(coloredSurface->format,255,0,255,0);
                    }
                    put_pixel32( coloredSurface, x, y, pixel );
                }
            }

            //Unlock surface
            if( SDL_MUSTLOCK( coloredSurface ) )
            {
                SDL_UnlockSurface( coloredSurface );
            }

            //Return coloredSurface surface
            GPE_Report("Returning colored surface");
            return coloredSurface;
        }
    }
    GPE_Report("Returning NULL surface");
    return NULL;
}

SDL_Surface * GPE_SDL_Surface_Modifier::surface_flip( SDL_Surface *surface, int flags )
{
    if( surface!=NULL)
    {
        //Pointer to the soon to be flipped surface
        SDL_Surface *flipped = NULL;

        flipped = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGBA8888,0);
        if( flipped!=NULL)
        {
            //If the surface must be locked
            if( SDL_MUSTLOCK( flipped ) )
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
                    if( ( flags & FLIP_VERTICAL ) && ( flags & FLIP_HORIZONTAL ) )
                    {
                        put_pixel32( flipped, rx, ry, pixel );
                    }
                    else if( flags & FLIP_HORIZONTAL )
                    {
                        put_pixel32( flipped, rx, y, pixel );
                    }
                    else if( flags & FLIP_VERTICAL )
                    {
                        put_pixel32( flipped, x, ry, pixel );
                    }
                }
            }

            //Unlock surface
            if( SDL_MUSTLOCK( flipped ) )
            {
                SDL_UnlockSurface( flipped );
            }

            //Return flipped surface
            return flipped;
        }
    }
    return NULL;
}

