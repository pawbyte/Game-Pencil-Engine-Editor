/*
GPE_Texture.cpp
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

#include "GPE_Texture_SDL.h"


GPE_Texture_SDL::GPE_Texture_SDL()
{
    texImg= NULL;
    textureType = "sdl";
}

GPE_Texture_SDL::~GPE_Texture_SDL()
{
    if(texImg!= NULL)
    {
        SDL_DestroyTexture(texImg);
        texImg=NULL;
    }
}

void GPE_Texture_SDL::change_alpha( Uint8 alpha  )
{
    if( texImg!=NULL )
    {
        SDL_SetTextureAlphaMod( texImg,alpha);
    }
}

void GPE_Texture_SDL::change_color(GPE_Color * newColor)
{
    if(texImg!=NULL)
    {
        if( newColor!=NULL)
        {
            if( newColor->get_r() == currentR && newColor->get_g() == currentG && newColor->get_b() == currentB )
            {
                return;
            }
            SDL_SetTextureColorMod( texImg, newColor->get_r(), newColor->get_g(), newColor->get_b() );
            currentR = newColor->get_r();
            currentG = newColor->get_g();
            currentB = newColor->get_b();
        }
        else if( currentR != 255 &&  currentG != 255 &&  currentB != 255 )
        {
            SDL_SetTextureColorMod( texImg, 255,255,255 );
            currentR = 255;
            currentG = 255;
            currentB = 255;
        }
    }
}

void GPE_Texture_SDL::change_color( Uint8 red, Uint8 green, Uint8 blue )
{
    if(texImg!=NULL)
    {
        if( red == currentR && green == currentG && blue == currentB )
        {
            return;
        }
        SDL_SetTextureColorMod( texImg, red, green, blue );
        currentR = red;
        currentG = green;
        currentB = blue;
    }
}

void GPE_Texture_SDL::change_texture(SDL_Texture * newTexture)
{
    texWid = 0;
    texHeight = 0;
    if(texImg!=NULL)
    {
        SDL_DestroyTexture(texImg);
        texImg = NULL;
    }
    texImg = newTexture;
    if( texImg!=NULL)
    {
        SDL_QueryTexture(texImg, NULL, NULL, &texWid, &texHeight);
        currentBlendMode = blend_mode_blend;
    }
}

bool GPE_Texture_SDL::copy_image_source(std::string outDirectoryName)
{
    if( get_width()>0)
    {
        std::string copyDestinationStr = outDirectoryName+"/"+ get_short_filename(fileLocation,true);
        return copy_file(fileLocation,copyDestinationStr );
    }
    return false;
}

GPE_Renderer_SDL * GPE_Texture_SDL::get_gpe_renderer_sdl(GPE_Renderer_Base * renderer)
{
    if( renderer == NULL)
    {
        return NULL;
    }
    if( renderer->get_renderer_type() == "sdl")
    {
        GPE_Renderer_SDL * gpeSDLRenderer = (GPE_Renderer_SDL * )renderer;
        return gpeSDLRenderer;
    }
    return NULL;
}

SDL_Renderer * GPE_Texture_SDL::get_sdl_renderer(GPE_Renderer_Base * renderer)
{
    GPE_Renderer_SDL * sdlRenderer = get_gpe_renderer_sdl( renderer);

    if( sdlRenderer == NULL)
    {
        return NULL;
    }
    return sdlRenderer->get_sdl_renderer();
}

void GPE_Texture_SDL::load_new_texture( GPE_Renderer_Base * renderer,std::string fileName, int id, bool transparent, bool useLinearScaling )
{
    if(texImg!=NULL)
    {
        SDL_DestroyTexture(texImg);
        texImg = NULL;
    }
    texId=id;
    isTransparent = transparent;
    //The image that's loaded
    if( file_exists(fileName ) == false )
    {
        GPE_Report("[Bad] Unable to load filed  <"+fileName+">. Error: FILE_NOT_FOUND.");
        return;
    }
    currentBlendMode = blend_mode_blend;

    //Load the image
    SDL_Surface * loadedImage = SDL_SurfaceEx::load_surface_image( fileName.c_str() );

    //If the image loaded
    if( loadedImage == NULL )
    {
        texWid = 0;
        texHeight = 0;
        fileLocation ="notfound.png";
        GPE_Report("[Bad] Unable to load filed loacated at <"+fileName+">. Error: "+IMG_GetError()+".");
        return;
    }

    //Create an optimized surface
    if( useLinearScaling )
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    }
    else
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    }
    texImg = SDL_SurfaceEx::create_texture_from_surface( get_sdl_renderer( renderer ),loadedImage);
    fileLocation = fileName;
    texWid = loadedImage->w;
    texHeight = loadedImage->h;
    texId = id;
    //Free the old surface
    SDL_FreeSurface( loadedImage );
    if(texImg==NULL)
    {
        texWid = 0;
        texHeight = 0;
        fileLocation ="notfound.png";
        GPE_Report("[Bad] Unable to load file loacated at <"+fileName+">. Error: "+IMG_GetError()+".\n");
    }
}

SDL_Texture * GPE_Texture_SDL::get_sdl_texture()
{
    return texImg;
}

void GPE_Texture_SDL::prerender_circle( GPE_Renderer_Base * renderer, int rad, GPE_Color * circleColor,   Uint8 alpha, int id, bool transparent, bool useLinearScaling, bool isOutline )
{
    if( renderer == NULL)
    {
        return;
    }
    SDL_Renderer * sdlRenderer = get_sdl_renderer( renderer);
    if( sdlRenderer == NULL)
    {
        return;
    }

    if( rad <= 0 || circleColor == NULL )
    {
        return;
    }
    //SDL_Texture textImg
    if(texImg!=NULL)
    {
        SDL_DestroyTexture(texImg);
        texImg = NULL;
    }
    currentBlendMode = blend_mode_blend;
    texId=id;
    isTransparent = transparent;
    //The image that's loaded


    SDL_Surface * loadedImage = SDL_SurfaceEx::create_filled_surface( rad * 2,  rad * 2, c_gray->get_sdl_color(), 0 );
    SDL_SetColorKey( loadedImage, SDL_TRUE, SDL_MapRGBA( loadedImage->format, c_gray->get_r(), c_gray->get_g(),c_gray->get_b(),0 ) );
    SDL_SetSurfaceBlendMode( loadedImage, SDL_BLENDMODE_BLEND );

    //If the image loaded
    if( loadedImage == NULL )
    {
        texWid = 0;
        texHeight = 0;
        fileLocation ="notfound.png";
        return;
    }

    SDL_SurfaceEx::surface_render_circle_color( loadedImage, rad, rad, rad, circleColor->get_r(), circleColor->get_g(),circleColor->get_b(), 255  );
    SDL_SetSurfaceBlendMode( loadedImage, SDL_BLENDMODE_BLEND );
    //Create an optimized surface
    if( useLinearScaling )
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    }
    else
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    }

    //If the image loaded
    //Create an optimized surface
    if( useLinearScaling )
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    }
    else
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    }
    fileLocation = "circle:rad:"+int_to_string(rad);
    texImg = SDL_SurfaceEx::create_texture_from_surface( sdlRenderer, loadedImage);

    //Free the old surface
    if( loadedImage!=NULL )
    {
        SDL_FreeSurface( loadedImage );
        loadedImage = NULL;
    }

    if( texImg!=NULL )
    {
        texWid = rad*2;
        texHeight = rad*2;
        SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_BLEND  );
    }
    else
    {
        texWid = 0;
        texHeight = 0;
        fileLocation ="notfound.png";
        //GPE_Report("[Bad] Unable to load file loacated at <"+fileName+">. Error: "+IMG_GetError()+".\n");
    }
}

void GPE_Texture_SDL::prerender_rectangle( GPE_Renderer_Base * renderer, int w, int h, GPE_Color * newColor, int id, bool transparent, bool useLinearScaling , bool isOutline)
{
    if( renderer == NULL)
    {
        return;
    }
    SDL_Renderer * sdlRenderer = get_sdl_renderer( renderer);
    if( sdlRenderer == NULL)
    {
        return;
    }
    if(texImg!=NULL)
    {
        SDL_DestroyTexture(texImg);
        texImg = NULL;
    }
    texId=id;
    isTransparent = transparent;
    //The image that's loaded
    if( w > 0 && h > 0  )
    {
        currentBlendMode = blend_mode_blend;
        SDL_Surface * loadedImage = NULL;
        if( newColor!=NULL )
        {
            loadedImage = SDL_SurfaceEx::create_filled_surface( w, h,newColor->get_sdl_color() );
        }
        else
        {
            loadedImage = SDL_SurfaceEx::create_filled_surface( w, h, c_white->get_sdl_color() );
        }

        //If the image loaded
        if( loadedImage != NULL )
        {
            //Create an optimized surface
            if( useLinearScaling )
            {
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            }
            else
            {
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
            }
            texImg = SDL_SurfaceEx::create_texture_from_surface( sdlRenderer, loadedImage);
            fileLocation = "";
            texWid = loadedImage->w;
            texHeight = loadedImage->h;
            texId = id;
            //Free the old surface
            SDL_FreeSurface( loadedImage );
            if(texImg==NULL)
            {
                texWid = 0;
                texHeight = 0;
                fileLocation ="notfound.png";
                //GPE_Report("[Bad] Unable to load file loacated at <"+fileName+">. Error: "+IMG_GetError()+".\n");
            }
            else
            {
                //GPE_Report("[GOOD] Loaded filed loacated at <"+fileName+"> with <"+int_to_string(texWid)+" , "+int_to_string(texHeight)+"> dimensions.");
            }
        }
        else
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
            //GPE_Report("[Bad] Unable to load filed loacated at <"+fileName+">. Error: "+IMG_GetError()+".");
        }
    }
    else
    {
        //GPE_Report("[Bad] Unable to load filed  <"+fileName+">. Error: FILE_NOT_FOUND.");
    }
}

GPE_Texture_Base * GPE_Texture_SDL::produce_clone()
{
    return new GPE_Texture_SDL();
}


void GPE_Texture_SDL::render_align(  int x, int y, int hAlign, int vAlign, GPE_Rect* clip, GPE_Color * rendColor, int alpha )
{
    if(hAlign==FA_CENTER)
    {
        x-=get_width()/2;
    }
    else if( hAlign==FA_RIGHT)
    {
        x-= get_width();
    }

    if(vAlign==FA_MIDDLE)
    {
        y-= get_height()/2;
    }
    else if( vAlign==FA_BOTTOM)
    {
        y-= get_height();
    }
    render_tex_colored( x,y, rendColor, alpha, clip );
}

void GPE_Texture_SDL::render_align_resized( int x, int y, int newWidth, int newHeight,  int hAlign, int vAlign, GPE_Rect* clip,GPE_Color * renderColor, int alpha )
{
    if(hAlign==FA_CENTER)
    {
        x-=newWidth/2;
    }
    else if( hAlign==FA_RIGHT)
    {
        x-= newWidth;
    }

    if(vAlign==FA_MIDDLE)
    {
        y-= newHeight/2;
    }
    else if( vAlign==FA_BOTTOM)
    {
        y-= newHeight;
    }
    render_tex_resized( x,y,newWidth, newHeight,clip, renderColor, alpha );
}

void GPE_Texture_SDL::render_tex(  int x, int y,GPE_Rect* clip , int alpha)
{
    if(texImg!=NULL && alpha > 0 )
    {
        SDL_Rect renderRect = { x, y, texWid, texHeight };

        change_color( 255,255,255 );
        set_alpha( alpha );
        //Set clip rendering dimensions
        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            renderRect.w = clip->w;
            renderRect.h = clip->h;

            SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect);
        }
        else
        {
            SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,NULL, &renderRect);
        }
    }
}

void GPE_Texture_SDL::render_tex_colored(  int x, int y, GPE_Color * rendColor, int alpha , GPE_Rect* clip   )
{
    if(texImg!=NULL && alpha > 0 )
    {
        SDL_Rect renderRect = { x, y, texWid, texHeight };
        change_color(rendColor);
        set_alpha( alpha );
        //Set clip rendering dimensions
        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            renderRect.w = clip->w;
            renderRect.h = clip->h;

            SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect);
        }
        else
        {
            SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,NULL, &renderRect);
        }
    }
}

void GPE_Texture_SDL::render_tex_resized(  int x, int y, float newWidth, float newHeight, GPE_Rect* clip, GPE_Color * renderColor, int alpha )
{
    if(texImg!=NULL && alpha > 0 )
    {
        if( clip != NULL )
        {
            if( clip->w !=0 && clip->h!= 0 )
            {
                float foundXScale = newWidth/clip->w;
                float foundYScale = newHeight/clip->h;
                render_tex_scaled( x, y, foundXScale, foundYScale,clip, renderColor,alpha);
            }
        }
        else
        {
            if( texWid !=0 && texHeight!= 0 )
            {
                float foundXScale = newWidth/(float)texWid;
                float foundYScale = newHeight/(float)texHeight;
                render_tex_scaled( x, y, foundXScale, foundYScale,clip, renderColor,alpha);
            }
        }
        return;
    }
}

void GPE_Texture_SDL::render_tex_scaled(  int x, int y, float xScale, float yScale, GPE_Rect* clip, GPE_Color * renderColor, int alpha )
{
    if( xScale == 0 || yScale == 0 )
    {
        return;
    }

    if(texImg!=NULL)
    {
        change_color(renderColor);
        set_alpha( alpha );

        int newWidth = 0;
        int newHeight = 0;
        bool flipHori = false,  flipVert = false;
        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            if( sdlClip.x > texWid)
            {
                sdlClip.x = 0;
            }

            if( sdlClip.y > texHeight )
            {
                sdlClip.y = 0;
            }

            if( sdlClip.w > texWid)
            {
                sdlClip.w = texWid;
            }

            if( sdlClip.h > texHeight )
            {
                sdlClip.h = texHeight;
            }

            if( sdlClip.x+sdlClip.w > texWid )
            {
                sdlClip.w = texWid - sdlClip.x;
            }

            if( sdlClip.y+sdlClip.h > texHeight )
            {
                sdlClip.h = texHeight - sdlClip.y;
            }

            if( xScale < 0 )
            {
                newWidth = (float)sdlClip.w * -xScale;
                flipHori = true;
            }
            else
            {
                newWidth = (float)sdlClip.w * xScale;
            }

            if( yScale < 0 )
            {
                newHeight = (float)sdlClip.h * -yScale;
                flipVert = true;
            }
            else
            {
                newHeight = (float)sdlClip.h * yScale;
            }
            SDL_Rect renderRect = { x, y, newWidth, newHeight };
            if( flipHori )
            {
                if( flipVert )
                {
                    SDL_RenderCopyEx(GPE_MAIN_RENDERER_SDL->get_sdl_renderer(), texImg, &sdlClip,&renderRect,0,NULL,GPE_MAIN_RENDERER_SDL->bothFlip );

                }
                else
                {
                    SDL_RenderCopyEx(GPE_MAIN_RENDERER_SDL->get_sdl_renderer(), texImg, &sdlClip,&renderRect,0,NULL,GPE_MAIN_RENDERER_SDL->horiFlip );
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx(GPE_MAIN_RENDERER_SDL->get_sdl_renderer(), texImg, &sdlClip,&renderRect,0,NULL, GPE_MAIN_RENDERER_SDL->vertFlip );
            }
            else
            {
                SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect);
            }
        }
        else
        {
            if( xScale < 0 )
            {
                flipHori = true;
                newWidth = (float)texWid * -xScale;
            }
            else
            {
                newWidth = (float)texWid * xScale;
            }

            if( yScale < 0 )
            {
                flipVert = true;
                newHeight = (float)texHeight * -yScale;
            }
            else
            {
                newHeight = (float)texHeight * yScale;
            }
            SDL_Rect renderRect = { x, y, newWidth, newHeight };
            if( flipHori )
            {
                if( flipVert )
                {
                    SDL_RenderCopyEx(GPE_MAIN_RENDERER_SDL->get_sdl_renderer(), texImg, NULL,&renderRect,0,NULL, GPE_MAIN_RENDERER_SDL->bothFlip );
                }
                else
                {
                    SDL_RenderCopyEx(GPE_MAIN_RENDERER_SDL->get_sdl_renderer(), texImg, NULL,&renderRect,0,NULL, GPE_MAIN_RENDERER_SDL->horiFlip );
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx(GPE_MAIN_RENDERER_SDL->get_sdl_renderer(), texImg, NULL,&renderRect,0,NULL, GPE_MAIN_RENDERER_SDL->vertFlip );
            }
            else
            {
                SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,NULL, &renderRect);
            }
        }
    }
}

void GPE_Texture_SDL::render_tex_rotated(  int x, int y, float renderAngle, GPE_Color * renderColor, GPE_Rect* clip , int alpha )
{
    if(texImg!=NULL && alpha > 0 )
    {
        SDL_Rect renderRect = { x-texWid/2, y-texHeight/2, texWid, texHeight };
        change_color(renderColor);
        set_alpha( alpha );
        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,NULL,SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,NULL,SDL_FLIP_NONE );
        }
    }
}

void GPE_Texture_SDL::render_tex_rotated_at_point(  int x, int y, float renderAngle, int pointX, int pointY, GPE_Color * renderColor, GPE_Rect* clip , int alpha )
{
    if(texImg!=NULL && alpha > 0 )
    {
        SDL_Rect renderRect = { x-texWid/2, y-texHeight/2, texWid, texHeight };
        change_color(renderColor);
        set_alpha( alpha );

        if( pointX < 0 )
        {
            if( pointX == FA_CENTER )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.y = -texWid/2;
            }
            else if( pointX== FA_RIGHT )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.y = -texWid;
            }

        }
        else if( pointX < texWid )
        {
            GPE_MAIN_RENDERER_SDL->defaultPoint.y = pointX;
        }
        else
        {
            GPE_MAIN_RENDERER_SDL->defaultPoint.y = 0;
        }

        if( pointY < 0 )
        {
            if( pointY == FA_MIDDLE )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.y = texHeight/2;
            }
            else if( pointY== FA_BOTTOM )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.y = texHeight;
            }

        }
        else if( pointY < texHeight )
        {
            GPE_MAIN_RENDERER_SDL->defaultPoint.y = pointY;
        }
        else
        {
            GPE_MAIN_RENDERER_SDL->defaultPoint.y = 0;
        }

        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&GPE_MAIN_RENDERER_SDL->defaultPoint,SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&GPE_MAIN_RENDERER_SDL->defaultPoint,SDL_FLIP_NONE);
        }
    }
}

void GPE_Texture_SDL::render_tex_special(  int x, int y, float renderAngle, int newWidth, int newHeight, GPE_Color * renderColor, GPE_Rect* clip , int alpha )
{
    if( texImg==NULL || alpha <= 0 )
    {
        return;
    }

    if( newWidth <=0.f)
    {
        newWidth = texWid;
    }
    if( newHeight<=0.f)
    {
        newHeight = texHeight;
    }
    SDL_Rect renderRect = { x-newWidth/2,y-newHeight/2, newWidth, newHeight };


    change_color(renderColor);
    set_alpha( alpha );

    GPE_MAIN_RENDERER_SDL->defaultPoint.x = 0;
    GPE_MAIN_RENDERER_SDL->defaultPoint.y = 0;

    if( clip != NULL )
    {
        SDL_Rect sdlClip;

        sdlClip.x = clip->x;
        sdlClip.y = clip->y;
        sdlClip.w = clip->w;
        sdlClip.h = clip->h;

        SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle, NULL,SDL_FLIP_NONE);
    }
    else
    {
        SDL_Rect sdlClip;
        sdlClip.x = 0;
        sdlClip.y = 0;
        sdlClip.w = texWid;
        sdlClip.h = texHeight;
        SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle, NULL,SDL_FLIP_NONE );
    }
}

void GPE_Texture_SDL::render_tex_special_at_point(  int x, int y, float renderAngle, int pointX, int pointY,int newWidth, int newHeight, GPE_Color * renderColor, GPE_Rect* clip , int alpha )
{
    if(texImg!=NULL && alpha > 0 )
    {
        bool flipHori = false,  flipVert = false;
        if( newWidth < 0)
        {
            flipHori = true;
            newWidth*=-1;
        }
        if( newHeight < 0)
        {
            flipVert = true;
            newHeight*=-1;
        }
        if( newWidth ==0)
        {
            newWidth = texWid;
        }
        if( newHeight==0)
        {
            newHeight = texHeight;
        }
        SDL_Rect renderRect = { x-newWidth, y-newHeight, newWidth, newHeight };
        change_color(renderColor);
        set_alpha( alpha );
        if( pointX < 0 )
        {
            if( pointX == FA_CENTER )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.x = newWidth/2;
            }
            else if( pointX == FA_RIGHT )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.x = newWidth;
            }
        }
        else if( pointX < newWidth )
        {
            GPE_MAIN_RENDERER_SDL->defaultPoint.x = pointX;
        }
        else
        {
            GPE_MAIN_RENDERER_SDL->defaultPoint.x = newWidth / 2;
        }

        if( pointY < 0 )
        {
            if( pointY == FA_MIDDLE )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.y = newHeight/2;
            }
            else if( pointY == FA_BOTTOM )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.y = newHeight;
            }
        }
        else if( pointY < newHeight )
        {
            GPE_MAIN_RENDERER_SDL->defaultPoint.y = pointY;
        }
        else
        {
            GPE_MAIN_RENDERER_SDL->defaultPoint.y = newHeight / 2;
        }

        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;
            /*
            if( pointX < 0 )
            {
                if( pointX == FA_CENTER )
                {
                    GPE_MAIN_RENDERER_SDL->defaultPoint.x = sdlClip.w/2;
                }
                else if( pointX== FA_RIGHT )
                {
                    GPE_MAIN_RENDERER_SDL->defaultPoint.x = sdlClip.w;
                }
            }
            else if( pointX < sdlClip.w )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.x = pointX;
            }
            else
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.x = 0;
            }

            if( pointY < 0 )
            {
                if( pointY == FA_MIDDLE )
                {
                    GPE_MAIN_RENDERER_SDL->defaultPoint.y = sdlClip.h /2;
                }
                else if( pointY == FA_BOTTOM )
                {
                    GPE_MAIN_RENDERER_SDL->defaultPoint.y = -sdlClip.h ;
                }

            }
            else if( pointY < sdlClip.h  )
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.y = pointY;
            }
            else
            {
                GPE_MAIN_RENDERER_SDL->defaultPoint.y = 0;
            }
            */
            if( flipHori )
            {
                if( flipVert )
                {
                    SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&GPE_MAIN_RENDERER_SDL->defaultPoint,GPE_MAIN_RENDERER_SDL->bothFlip);
                }
                else
                {
                    SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&GPE_MAIN_RENDERER_SDL->defaultPoint,GPE_MAIN_RENDERER_SDL->horiFlip);
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&GPE_MAIN_RENDERER_SDL->defaultPoint, GPE_MAIN_RENDERER_SDL->vertFlip);
            }
            else
            {
                //&GPE_MAIN_RENDERER_SDL->defaultPoint
                SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle, NULL, SDL_FLIP_NONE);
            }
        }
        else if( flipHori )
        {
            if( flipVert )
            {
                SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&GPE_MAIN_RENDERER_SDL->defaultPoint,GPE_MAIN_RENDERER_SDL->bothFlip);
            }
            else
            {
                SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&GPE_MAIN_RENDERER_SDL->defaultPoint,GPE_MAIN_RENDERER_SDL->horiFlip);
            }
        }
        else if( flipVert)
        {
            SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&GPE_MAIN_RENDERER_SDL->defaultPoint, GPE_MAIN_RENDERER_SDL->vertFlip);
        }
        else
        {
            SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&GPE_MAIN_RENDERER_SDL->defaultPoint,SDL_FLIP_NONE  );
        }
    }
}

void GPE_Texture_SDL::set_alpha( int alpha )
{
    if( alpha < 0 )
    {
        alpha = 0;
    }
    else if( alpha > 255)
    {
        alpha = 255;
    }
    if( texImg!=NULL && lastAlphaRendered!=alpha )
    {
        SDL_SetTextureAlphaMod(texImg,alpha);
        lastAlphaRendered = alpha;
    }
}

void GPE_Texture_SDL::set_blend_mode( int newBlendMode)
{
    if( texImg!=NULL )//&& currentBlendMode!=newBlendMode)
    {
        currentBlendMode = newBlendMode;
        switch( newBlendMode)
        {
            case blend_mode_add:
                SDL_SetTextureBlendMode(texImg,SDL_BLENDMODE_ADD );
                break;

            case blend_mode_mod:
                SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_MOD);
                break;

            case blend_mode_none:
                SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_NONE  );
                break;

            default:
                SDL_SetTextureBlendMode(texImg, SDL_BLENDMODE_BLEND);
                currentBlendMode = blend_mode_blend;
            break;
        }
    }
}

