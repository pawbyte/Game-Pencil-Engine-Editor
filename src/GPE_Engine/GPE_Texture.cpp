/*
GPE_Texture.cpp
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

#include "GPE_Texture.h"

GPE_BasicTexture::GPE_BasicTexture()
{
    texImg= NULL;
    texId=-1;
    texWid = 0;
    texHeight = 0;
    currentR = 255;
    currentG = 255;
    currentB = 255;

    lastAlphaRendered = 255;
    currentBlendMode = blend_mode_blend;
}

GPE_BasicTexture::~GPE_BasicTexture()
{
    if(texImg!= NULL)
    {
        SDL_DestroyTexture(texImg);
        texImg=NULL;
    }
}


void GPE_BasicTexture::change_alpha( Uint8 alpha  )
{
    if( texImg!=NULL )
    {
        SDL_SetTextureAlphaMod( texImg,alpha);
    }
}

void GPE_BasicTexture::change_color(GPE_Color * newColor)
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

void GPE_BasicTexture::change_color( Uint8 red, Uint8 green, Uint8 blue )
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

int GPE_BasicTexture::get_id()
{
    return texId;
}

std::string GPE_BasicTexture::get_name()
{
    return name;
}

int GPE_BasicTexture::get_width()
{
    return texWid;
}

int GPE_BasicTexture::get_height()
{
    return texHeight;
}


SDL_Texture * GPE_BasicTexture::get_sdl_texture()
{
    return texImg;
}

void GPE_BasicTexture::render_align(  int x, int y, int hAlign, int vAlign, GPE_Rect* clip, GPE_Color * rendColor, int alpha )
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

void GPE_BasicTexture::render_align_resized( int x, int y, int newWidth, int newHeight,  int hAlign, int vAlign, GPE_Rect* clip,GPE_Color * renderColor, int alpha )
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

void GPE_BasicTexture::render_tex(  int x, int y,GPE_Rect* clip , int alpha)
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

            SDL_RenderCopy( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect);
        }
        else
        {
            SDL_RenderCopy( CURRENT_RENDERER->get_sdl_renderer(),texImg,NULL, &renderRect);
        }
    }
}

void GPE_BasicTexture::render_tex_colored(  int x, int y, GPE_Color * rendColor, int alpha , GPE_Rect* clip   )
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

            SDL_RenderCopy( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect);
        }
        else
        {
            SDL_RenderCopy( CURRENT_RENDERER->get_sdl_renderer(),texImg,NULL, &renderRect);
        }
    }
}

void GPE_BasicTexture::render_tex_resized(  int x, int y, double newWidth, double newHeight, GPE_Rect* clip, GPE_Color * renderColor, int alpha )
{
    if(texImg!=NULL && alpha > 0 )
    {
        if( clip != NULL )
        {
            if( clip->w !=0 && clip->h!= 0 )
            {
                double foundXScale = newWidth/clip->w;
                double foundYScale = newHeight/clip->h;
                render_tex_scaled( x, y, foundXScale, foundYScale,clip, renderColor,alpha);
            }
        }
        else
        {
            if( texWid !=0 && texHeight!= 0 )
            {
                double foundXScale = newWidth/(double)texWid;
                double foundYScale = newHeight/(double)texHeight;
                render_tex_scaled( x, y, foundXScale, foundYScale,clip, renderColor,alpha);
            }
        }
        return;
    }
}

void GPE_BasicTexture::render_tex_scaled(  int x, int y, double xScale, double yScale, GPE_Rect* clip, GPE_Color * renderColor, int alpha )
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
                newWidth = (double)sdlClip.w * -xScale;
                flipHori = true;
            }
            else
            {
                newWidth = (double)sdlClip.w * xScale;
            }

            if( yScale < 0 )
            {
                newHeight = (double)sdlClip.h * -yScale;
                flipVert = true;
            }
            else
            {
                newHeight = (double)sdlClip.h * yScale;
            }
            SDL_Rect renderRect = { x, y, newWidth, newHeight };
            if( flipHori )
            {
                if( flipVert )
                {
                    SDL_RenderCopyEx(CURRENT_RENDERER->get_sdl_renderer(), texImg, &sdlClip,&renderRect,0,NULL,CURRENT_RENDERER->bothFlip );
                }
                else
                {
                    SDL_RenderCopyEx(CURRENT_RENDERER->get_sdl_renderer(), texImg, &sdlClip,&renderRect,0,NULL,CURRENT_RENDERER->horiFlip );
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx(CURRENT_RENDERER->get_sdl_renderer(), texImg, &sdlClip,&renderRect,0,NULL, CURRENT_RENDERER->vertFlip );
            }
            else
            {
                SDL_RenderCopy( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect);
            }
        }
        else
        {
            if( xScale < 0 )
            {
                flipHori = true;
                newWidth = (double)texWid * -xScale;
            }
            else
            {
                newWidth = (double)texWid * xScale;
            }

            if( yScale < 0 )
            {
                flipVert = true;
                newHeight = (double)texHeight * -yScale;
            }
            else
            {
                newHeight = (double)texHeight * yScale;
            }
            SDL_Rect renderRect = { x, y, newWidth, newHeight };
            if( flipHori )
            {
                if( flipVert )
                {
                    SDL_RenderCopyEx(CURRENT_RENDERER->get_sdl_renderer(), texImg, NULL,&renderRect,0,NULL, CURRENT_RENDERER->bothFlip );
                }
                else
                {
                    SDL_RenderCopyEx(CURRENT_RENDERER->get_sdl_renderer(), texImg, NULL,&renderRect,0,NULL, CURRENT_RENDERER->horiFlip );
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx(CURRENT_RENDERER->get_sdl_renderer(), texImg, NULL,&renderRect,0,NULL, CURRENT_RENDERER->vertFlip );
            }
            else
            {
                SDL_RenderCopy( CURRENT_RENDERER->get_sdl_renderer(),texImg,NULL, &renderRect);
            }
        }
    }
}

void GPE_BasicTexture::render_tex_rotated(  int x, int y, double renderAngle, GPE_Color * renderColor, GPE_Rect* clip , int alpha )
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

            SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,NULL,SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,NULL,SDL_FLIP_NONE );
        }
    }
}

void GPE_BasicTexture::render_tex_rotated_at_point(  int x, int y, double renderAngle, int pointX, int pointY, GPE_Color * renderColor, GPE_Rect* clip , int alpha )
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
                CURRENT_RENDERER->defaultPoint.y = -texWid/2;
            }
            else if( pointX== FA_RIGHT )
            {
                CURRENT_RENDERER->defaultPoint.y = -texWid;
            }

        }
        else if( pointX < texWid )
        {
            CURRENT_RENDERER->defaultPoint.y = pointX;
        }
        else
        {
            CURRENT_RENDERER->defaultPoint.y = 0;
        }

        if( pointY < 0 )
        {
            if( pointY == FA_MIDDLE )
            {
                CURRENT_RENDERER->defaultPoint.y = texHeight/2;
            }
            else if( pointY== FA_BOTTOM )
            {
                CURRENT_RENDERER->defaultPoint.y = texHeight;
            }

        }
        else if( pointY < texHeight )
        {
            CURRENT_RENDERER->defaultPoint.y = pointY;
        }
        else
        {
            CURRENT_RENDERER->defaultPoint.y = 0;
        }

        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint,SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint,SDL_FLIP_NONE);
        }
    }
}

void GPE_BasicTexture::render_tex_special(  int x, int y, double renderAngle, int newWidth, int newHeight, GPE_Color * renderColor, GPE_Rect* clip , int alpha )
{
    if( texImg==NULL || alpha <= 0 )
    {
        return;
    }

    if( newWidth <=0.l)
    {
        newWidth = texWid;
    }
    if( newHeight<=0.l)
    {
        newHeight = texHeight;
    }
    SDL_Rect renderRect = { x-newWidth/2,y-newHeight/2, newWidth, newHeight };


    change_color(renderColor);
    set_alpha( alpha );

    CURRENT_RENDERER->defaultPoint.x = 0;
    CURRENT_RENDERER->defaultPoint.y = 0;

    if( clip != NULL )
    {
        SDL_Rect sdlClip;

        sdlClip.x = clip->x;
        sdlClip.y = clip->y;
        sdlClip.w = clip->w;
        sdlClip.h = clip->h;

        SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle, NULL,SDL_FLIP_NONE);
    }
    else
    {
        SDL_Rect sdlClip;
        sdlClip.x = 0;
        sdlClip.y = 0;
        sdlClip.w = texWid;
        sdlClip.h = texHeight;
        SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle, NULL,SDL_FLIP_NONE );
    }
}

void GPE_BasicTexture::render_tex_special_at_point(  int x, int y, double renderAngle, int pointX, int pointY,int newWidth, int newHeight, GPE_Color * renderColor, GPE_Rect* clip , int alpha )
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
        SDL_Rect renderRect = { x-newWidth/2, y-newHeight/2, newWidth, newHeight };
        change_color(renderColor);
        set_alpha( alpha );
        if( pointX < 0 )
        {
            if( pointX == FA_CENTER )
            {
                CURRENT_RENDERER->defaultPoint.x = newWidth/2;
            }
            else if( pointX == FA_RIGHT )
            {
                CURRENT_RENDERER->defaultPoint.x = newWidth;
            }
        }
        else if( pointX < newWidth )
        {
            CURRENT_RENDERER->defaultPoint.x = pointX;
        }
        else
        {
            CURRENT_RENDERER->defaultPoint.x = 0;
        }

        if( pointY < 0 )
        {
            if( pointY == FA_MIDDLE )
            {
                CURRENT_RENDERER->defaultPoint.y = newHeight/2;
            }
            else if( pointY == FA_BOTTOM )
            {
                CURRENT_RENDERER->defaultPoint.y = newHeight;
            }

        }
        else if( pointY < newHeight )
        {
            CURRENT_RENDERER->defaultPoint.y = pointY;
        }
        else
        {
            CURRENT_RENDERER->defaultPoint.y = 0;
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
                    CURRENT_RENDERER->defaultPoint.x = sdlClip.w/2;
                }
                else if( pointX== FA_RIGHT )
                {
                    CURRENT_RENDERER->defaultPoint.x = sdlClip.w;
                }
            }
            else if( pointX < sdlClip.w )
            {
                CURRENT_RENDERER->defaultPoint.x = pointX;
            }
            else
            {
                CURRENT_RENDERER->defaultPoint.x = 0;
            }

            if( pointY < 0 )
            {
                if( pointY == FA_MIDDLE )
                {
                    CURRENT_RENDERER->defaultPoint.y = sdlClip.h /2;
                }
                else if( pointY == FA_BOTTOM )
                {
                    CURRENT_RENDERER->defaultPoint.y = -sdlClip.h ;
                }

            }
            else if( pointY < sdlClip.h  )
            {
                CURRENT_RENDERER->defaultPoint.y = pointY;
            }
            else
            {
                CURRENT_RENDERER->defaultPoint.y = 0;
            }
            */
            if( flipHori )
            {
                if( flipVert )
                {
                    SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint,CURRENT_RENDERER->bothFlip);
                }
                else
                {
                    SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint,CURRENT_RENDERER->horiFlip);
                }
            }
            else if( flipVert)
            {
                SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint, CURRENT_RENDERER->vertFlip);
            }
            else
            {
                SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,&sdlClip, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint,SDL_FLIP_NONE);
            }
        }
        else if( flipHori )
        {
            if( flipVert )
            {
                SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint,CURRENT_RENDERER->bothFlip);
            }
            else
            {
                SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint,CURRENT_RENDERER->horiFlip);
            }
        }
        else if( flipVert)
        {
            SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint, CURRENT_RENDERER->vertFlip);
        }
        else
        {
            SDL_RenderCopyEx( CURRENT_RENDERER->get_sdl_renderer(),texImg,NULL, &renderRect, -renderAngle,&CURRENT_RENDERER->defaultPoint,SDL_FLIP_NONE  );
        }
    }
}

void GPE_BasicTexture::set_alpha( int alpha )
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
void GPE_BasicTexture::set_blend_mode( int newBlendMode)
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

GPE_Texture::GPE_Texture()
{
    texImg= NULL;
    fileLocation =" ";
    texId=-1;
    texWid = 0;
    texHeight = 0;
    isTransparent = false;
    lastAlphaRendered = 255;
}

GPE_Texture::~GPE_Texture()
{

}

void GPE_Texture::change_texture(SDL_Texture * newTexture)
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

bool GPE_Texture::copy_image_source(std::string outDirectoryName)
{
    if( get_width()>0)
    {
        std::string copyDestinationStr = outDirectoryName+"/"+ getShortFileName(fileLocation,true);
        return copy_file(fileLocation,copyDestinationStr );
    }
    return false;
}


void GPE_Texture::load_new_texture( std::string fileName, short id, bool transparent, bool useLinearScaling )
{
    if(texImg!=NULL)
    {
        SDL_DestroyTexture(texImg);
        texImg = NULL;
    }
    texId=id;
    isTransparent = transparent;
    //The image that's loaded
    if( file_exists(fileName ) )
    {
        currentBlendMode = blend_mode_blend;
        SDL_Surface *loadedImage = NULL;

        //Load the image
        loadedImage = gpe_sdl->load_surface_image( fileName.c_str() );

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
            texImg = gpe_sdl->create_texture_from_surface( CURRENT_RENDERER->get_sdl_renderer(),loadedImage);
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
            GPE_Report("[Bad] Unable to load filed loacated at <"+fileName+">. Error: "+IMG_GetError()+".");
        }
    }
    else
    {
        GPE_Report("[Bad] Unable to load filed  <"+fileName+">. Error: FILE_NOT_FOUND.");
    }
}

std::string GPE_Texture::get_filename()
{
    return fileLocation;
}



void GPE_Texture::prerender_circle( int rad, GPE_Color * circleColor,   Uint8 alpha, short id, bool transparent, bool useLinearScaling )
{
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
    if( CURRENT_RENDERER == NULL || rad <= 0 || circleColor == NULL )
    {
        return;
    }

    SDL_Surface * loadedImage = gpe_sdl->create_filled_surface( rad * 2,  rad * 2, c_ltgray->get_sdl_color(), 0 );
    SDL_SetColorKey( loadedImage, SDL_TRUE, SDL_MapRGBA( loadedImage->format, c_ltgray->get_r(), c_ltgray->get_g(),c_ltgray->get_b(),0 ) );
    SDL_SetSurfaceBlendMode( loadedImage, SDL_BLENDMODE_BLEND );

    //If the image loaded
    if( loadedImage == NULL )
    {
        texWid = 0;
        texHeight = 0;
        fileLocation ="notfound.png";
        return;
    }

    gpe_sdl->surface_render_circle_color( loadedImage, rad, rad, rad, circleColor->get_r(), circleColor->get_g(),circleColor->get_b(), alpha  );
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

    SDL_Renderer * sdlRenderer = CURRENT_RENDERER->get_sdl_renderer();
    if( sdlRenderer!=NULL )
    {
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
        texImg = gpe_sdl->create_texture_from_surface( sdlRenderer, loadedImage);
        SDL_FreeSurface( loadedImage );
        //Free the old surface
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
    else
    {
        texWid = 0;
        texHeight = 0;
        fileLocation ="notfound.png";
        //GPE_Report("[Bad] Unable to load filed loacated at <"+fileName+">. Error: "+IMG_GetError()+".");
    }
}

void GPE_Texture::prerender_rectangle( int w, int h, GPE_Color * newColor, short id, bool transparent, bool useLinearScaling )
{
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
            loadedImage = gpe_sdl->create_filled_surface( w, h,newColor->get_sdl_color() );
        }
        else
        {
            loadedImage = gpe_sdl->create_filled_surface( w, h, c_gray->get_sdl_color() );
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
            texImg = gpe_sdl->create_texture_from_surface( CURRENT_RENDERER->get_sdl_renderer(),loadedImage);
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


GPE_TargetTexture::GPE_TargetTexture()
{

}

GPE_TargetTexture::~GPE_TargetTexture()
{
    if( texImg != NULL )
    {
        SDL_DestroyTexture( texImg );
        texImg = NULL;
    }
}

void GPE_TargetTexture::resize_target( int w, int h, short id, bool useLinearScaling  )
{
    if(texImg!=NULL)
    {
        //only creates on need
        if( texWid > w && texHeight > h)
        {
            return;
        }
        SDL_DestroyTexture(texImg);
        texImg = NULL;
    }
    texId=id;
    //The image that's loaded
    currentBlendMode = blend_mode_blend;
    if( w > 0 && h > 0  )
    {
        texWid = w + 128;
        texHeight = h + 128;
        texImg = SDL_CreateTexture( CURRENT_RENDERER->get_sdl_renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texWid, texHeight );
        texId = id;
        if(texImg==NULL)
        {
            texWid = 0;
            texHeight = 0;
            //GPE_Report("[Bad] Unable to load file loacated at <"+fileName+">. Error: "+IMG_GetError()+".\n");
        }
        else
        {
            SDL_SetTextureBlendMode( texImg, SDL_BLENDMODE_BLEND );
        }
    }
    else
    {
        //GPE_Report("[Bad] Unable to load filed  <"+fileName+">. Error: FILE_NOT_FOUND.");
    }
}


GPE_Tilesheet::GPE_Tilesheet()
{
    tsId = -1;
    tsImage = NULL;
    tsWidth = 0;
    tsHeight = 0;
    tsXOff = 0;
    tsYOff = 0;
    tsXPadding = 0;
    tsYPadding = 0;
    tilesPerRow= 0;
    tilesPerCol = 0;
    name = "";
    fileNameLocation = "";
    isPreloaded = true;
}

GPE_Tilesheet::~GPE_Tilesheet()
{
    tsRects.clear();
    if( tsImage!=NULL)
    {
        delete tsImage;
        tsImage = NULL;
    }
}

void GPE_Tilesheet::organize_tilesheet()
{
    if( tsImage!=NULL)
    {
        tsRects.clear();
        GPE_Rect tempRect;
        if( tsWidth > 0 && tsHeight > 0 && tsXPadding>=0 && tsYPadding>=0 )
        {
            int startX = tsXOff;
            int startY = tsYOff;
            tilesPerRow= 0;
            tilesPerCol = 0;
            tempRect.x = startX;
            tempRect.y = startY;
            tempRect.w = tsWidth;
            tempRect.h = tsHeight;
            for( int i = startY; i < tsImage->get_height(); i+=tsHeight+tsYPadding)
            {
                tilesPerCol++;
                tilesPerRow = 0;
                for( int j = startX; j < tsImage->get_width(); j+=tsWidth+tsXPadding)
                {
                    tilesPerRow++;
                    tempRect.x = j;
                    tempRect.y = i;
                    tempRect.w = tsWidth;
                    tempRect.h = tsHeight;
                    tsRects.push_back(tempRect);
                }
            }
        }
        else
        {
            tempRect.x = 0;
            tempRect.y = 0;
            tempRect.w = tsImage->get_width();
            tempRect.h = tsImage->get_height();
        }
    }
}

