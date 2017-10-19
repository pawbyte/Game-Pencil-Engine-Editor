/*
GPE_Texture.cpp
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

#include "GPE_Texture.h"

GPE_Texture::GPE_Texture()
{
    texImg= NULL;
    fileLocation =" ";
    texId=-1;
    texWid = 0;
    texHeight = 0;
    isTransparent = false;
}

GPE_Texture::~GPE_Texture()
{
    if(texImg!= NULL)
    {
        SDL_DestroyTexture(texImg);
        texImg=NULL;
    }
}

 std::string GPE_Texture::get_filename()
{
    return fileLocation;
}

int GPE_Texture::get_width()
{
    return texWid;
}

int GPE_Texture::get_height()
{
    return texHeight;
}


int GPE_Texture::get_id()
{
    return texId;
}

void GPE_Texture::change_color(GPE_Color * newColor)
{
    if(texImg!=NULL)
    {
        if( newColor!=NULL)
        {
            SDL_SetTextureColorMod( texImg, newColor->get_r(), newColor->get_g(), newColor->get_b() );
        }
    }
}

void GPE_Texture::change_color( Uint8 red, Uint8 green, Uint8 blue )
{
    if(texImg!=NULL)
    {
        SDL_SetTextureColorMod( texImg, red, green, blue );
    }
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

SDL_Texture * GPE_Texture::get_sdl_texture()
{
    return texImg;
}

void GPE_Texture::load_new_texture(GPE_Renderer * cRenderer, std::string fileName, short id, bool transparent)
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
        SDL_Surface *loadedImage = NULL;

        //Load the image
        loadedImage = load_surface_image( fileName.c_str() );

        //If the image loaded
        if( loadedImage != NULL )
        {
            //Create an optimized surface

            texImg = SDL_CreateTextureFromSurface(cRenderer->get_renderer(),loadedImage);
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
                record_error("[Bad] Unable to load file loacated at <"+fileName+">. Error: "+IMG_GetError()+".\n");
            }
            else
            {
                //record_error("[GOOD] Loaded filed loacated at <"+fileName+"> with <"+int_to_string(texWid)+" , "+int_to_string(texHeight)+"> dimensions.");
            }
        }
        else
        {
            texWid = 0;
            texHeight = 0;
            fileLocation ="notfound.png";
            record_error("[Bad] Unable to load filed loacated at <"+fileName+">. Error: "+IMG_GetError()+".");
        }
    }
    else
    {
        record_error("[Bad] Unable to load filed  <"+fileName+">. Error: FILE_NOT_FOUND.");
    }
}

void GPE_Texture::render_tex( GPE_Renderer * cRenderer, int x, int y,GPE_Rect* clip, GPE_Texture* destination)
{
    if(texImg!=NULL)
    {
        if( destination!=NULL)
        {
            SDL_SetRenderTarget( cRenderer->gpeRender, destination->texImg );
        }
        else
        {
            //SDL_SetRenderTarget( cRenderer->gpeRender, NULL);
        }
        SDL_Rect renderRect = { x, y, texWid, texHeight };

        SDL_SetTextureColorMod( texImg, 255,255,255 );
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

            SDL_RenderCopy(cRenderer->gpeRender,texImg,&sdlClip, &renderRect);
        }
        else
        {
            SDL_RenderCopy(cRenderer->gpeRender,texImg,NULL, &renderRect);
        }


    }
}

void GPE_Texture::render_tex_resized( GPE_Renderer * cRenderer, int x, int y, int newWidth, int newHeight, GPE_Rect* clip, GPE_Texture* destination, GPE_Color * renderColor)
{
    if(texImg!=NULL)
    {
        if( destination!=NULL)
        {
            SDL_SetRenderTarget( cRenderer->gpeRender, destination->texImg );
        }
        else
        {
            //SDL_SetRenderTarget( cRenderer->gpeRender, NULL);
        }
        if( newWidth <=0)
        {
            newWidth = texWid;
        }
        if( newHeight<=0)
        {
            newHeight = texHeight;
        }
        SDL_Rect renderRect = { x, y, newWidth, newHeight };
        if( renderColor!=NULL)
        {
            SDL_SetTextureColorMod( texImg, renderColor->get_r(), renderColor->get_g(), renderColor->get_b() );
        }
        else
        {
            SDL_SetTextureColorMod( texImg, 255,255,255 );
        }
        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            SDL_RenderCopy(cRenderer->gpeRender,texImg,&sdlClip, &renderRect);
        }
        else
        {
            SDL_RenderCopy(cRenderer->gpeRender,texImg,NULL, &renderRect);
        }
    }
}

void GPE_Texture::render_tex_rotated( GPE_Renderer * cRenderer, int x, int y, double renderAngle, int newWidth, int newHeight, GPE_Rect* clip, GPE_Texture* destination)
{
    if(texImg!=NULL)
    {
        if( destination!=NULL)
        {
            SDL_SetRenderTarget( cRenderer->gpeRender, destination->texImg );
        }
        else
        {
            SDL_SetRenderTarget( cRenderer->gpeRender, NULL);
        }
        if( newWidth <=0)
        {
            newWidth = texWid;
        }
        if( newHeight<=0)
        {
            newHeight = texHeight;
        }
        SDL_Rect renderRect = { x, y, newWidth, newHeight };
        SDL_SetTextureColorMod( texImg, 255,255,255 );
        if( clip != NULL )
        {
            SDL_Rect sdlClip;

            sdlClip.x = clip->x;
            sdlClip.y = clip->y;
            sdlClip.w = clip->w;
            sdlClip.h = clip->h;

            SDL_RenderCopyEx(cRenderer->gpeRender,texImg,&sdlClip, &renderRect, renderAngle,NULL,SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderCopy(cRenderer->gpeRender,texImg,NULL, &renderRect);
            SDL_RenderCopyEx(cRenderer->gpeRender,texImg,NULL, &renderRect, renderAngle,NULL,SDL_FLIP_NONE);
        }
    }
}

void render_texture(GPE_Renderer * cRenderer, GPE_Texture * texIn, int x, int y,GPE_Rect* clip , GPE_Texture* destination,int hAlign, int vAlign)
{
    if( texIn!=NULL)
    {
        if(hAlign==FA_CENTER)
        {
            x-=texIn->get_width()/2;
        }
        else if( hAlign==FA_RIGHT)
        {
            x-= texIn->get_width();
        }

        if(vAlign==FA_MIDDLE)
        {
            y-= texIn->get_height()/2;
        }
        else if( vAlign==FA_BOTTOM)
        {
            y-= texIn->get_height();
        }
        texIn->render_tex(cRenderer,x,y,clip,destination);
    }
}

void render_texture_resized(GPE_Renderer * cRenderer, GPE_Texture * texIn, int x, int y, int newWidth, int newHeight, GPE_Rect* clip , GPE_Texture* destination, int hAlign, int vAlign, GPE_Color * renderColor )
{
    if( texIn!=NULL)
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

        texIn->render_tex_resized(cRenderer,x,y,newWidth, newHeight,clip,destination, renderColor);
    }
}

void render_texture_rotated(GPE_Renderer * cRenderer, GPE_Texture * texIn, int x, int y, double newAngle, int newWidth, int newHeight, GPE_Rect* clip , GPE_Texture* destination )
{
    if( texIn!=NULL)
    {
        /*
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
        }*/
        texIn->render_tex_rotated(cRenderer,x,y,newAngle,newWidth, newHeight,clip,destination);
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
