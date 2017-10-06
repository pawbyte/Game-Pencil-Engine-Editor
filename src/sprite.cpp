/*
sprite.cpp
This file is part of:
GAME PENCI ENGINE
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

#include "sprite.h"
//adds a sprite with only one row of subimages
GPE_Sprite::GPE_Sprite()
{
    boundingBox = new SDL_Rect();
    //boundingBox.x=0;
    //boundingBox.y=0;
    //boundingBox.w=0;
    //boundingBox.h=0;s
    spriteTexture = NULL;
    width=0;
    height=0;
    xoffset=0;
    yoffset=0;
    framesForSprite=0;
    isMirrored=false;
    name = "";
    fileName = "";
    spriteId = -1;
}

GPE_Sprite::GPE_Sprite(std::string fileName, bool imgTransparent)
{
    boundingBox = new SDL_Rect();
    //boundingBox.x=0;
    //boundingBox.y=0;
    //boundingBox.w=0;
    //boundingBox.h=0;s
    spriteTexture = new GPE_Texture();
    spriteTexture->load_new_texture( MAIN_RENDERER,fileName, -1,imgTransparent );
    width=0;
    height=0;
    xoffset=0;
    yoffset=0;
    framesForSprite=0;
    isMirrored=false;
    name = "";
    fileName = "";
    spriteId = -1;
}

GPE_Sprite::~GPE_Sprite()
{
    if( spriteTexture!=NULL)
    {
        delete spriteTexture;
        spriteTexture = NULL;
    }
}

void GPE_Sprite::render(GPE_Renderer * cRenderer,int subImageToDraw, int xPos, int yPos, GPE_Texture *destination, SDL_Rect *cam)
{
    render_sprite(cRenderer,this,subImageToDraw,xPos,yPos,destination,cam);
}

void GPE_Sprite::render_resized(GPE_Renderer * cRenderer,int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Texture *destination , SDL_Rect *cam )
{
    if(spriteTexture!=NULL)
    {
        if( (subImageToDraw < (int)spriteImages.size() )&& (subImageToDraw>=0) )
        {
            if( cam!=NULL)
            {
//                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+width,yPos+height) == true )
                {
                    render_texture_resized( cRenderer,spriteTexture , xPos-cam->x,yPos-cam->y, newWidth, newHeight,&(spriteImages.at(subImageToDraw)) , destination);
                }
            }
            else
            {
                render_texture_resized( cRenderer,spriteTexture , xPos,yPos, newWidth, newHeight,&(spriteImages.at(subImageToDraw)) , destination);
            }
        }
    }
}

void GPE_Sprite::clean_up()
{
    if( spriteTexture!=NULL)
    {
        delete spriteTexture;
        spriteTexture = NULL;
    }
}

void render_sprite(GPE_Renderer * cRenderer,GPE_Sprite* spriteToDraw, int subImageToDraw, int xPos, int yPos, GPE_Texture *destination, SDL_Rect *cam)
{
    if(spriteToDraw!=NULL)
    {
        if( (subImageToDraw < (int)spriteToDraw->spriteImages.size() )&& (subImageToDraw>=0) )
        {
            if( cam!=NULL)
            {
                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
                {
                    render_texture( cRenderer,spriteToDraw->spriteTexture , xPos-cam->x,yPos-cam->y, &(spriteToDraw->spriteImages.at(subImageToDraw)) , destination);
                }
            }
            else
            {
                render_texture( cRenderer,spriteToDraw->spriteTexture , xPos,yPos, &(spriteToDraw->spriteImages.at(subImageToDraw)) , destination);
            }
        }
    }
}


void render_sprite_resized(GPE_Renderer * cRenderer,GPE_Sprite* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Texture *destination, SDL_Rect * cam)
{
    if(spriteToDraw!=NULL)
    {
        if( (subImageToDraw < (int)spriteToDraw->spriteImages.size() )&& (subImageToDraw>=0) )
        {
            if( cam!=NULL)
            {
//                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
                {
                    render_texture_resized( cRenderer,spriteToDraw->spriteTexture , xPos-cam->x,yPos-cam->y, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) , destination);
                }
            }
            else
            {
                render_texture_resized( cRenderer,spriteToDraw->spriteTexture , xPos,yPos, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) , destination);
            }
        }
    }
}

void render_sprite_ext(GPE_Renderer * cRenderer,GPE_Sprite* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Color * rendColor,GPE_Texture *destination, SDL_Rect * cam)
{
    if( cam==NULL)
    {
        cam = &camera;
    }
    if(spriteToDraw!=NULL)
    {
        if( spriteToDraw->spriteTexture!=NULL )
        {
            //spriteToDraw->spriteTexture->change_color(rendColor);
            if( (subImageToDraw < (int)spriteToDraw->spriteImages.size() )&& (subImageToDraw>=0) )
            {
                if( cam!=NULL)
                {
                    //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
                    {
                        render_texture_resized( cRenderer,spriteToDraw->spriteTexture , xPos-cam->x,yPos-cam->y, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) , destination,-1,-1,rendColor);
                    }
                }
                else
                {
                    render_texture_resized( cRenderer,spriteToDraw->spriteTexture , xPos,yPos, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) , destination,-1,-1,rendColor);
                }
            }
            spriteToDraw->spriteTexture->change_color(c_white);
        }
    }
}

void render_sprite_rotated(GPE_Renderer * cRenderer,GPE_Sprite* spriteToDraw, int subImageToDraw, int xPos, int yPos, double newAngle, int newWidth, int newHeight, GPE_Texture *destination, SDL_Rect *cam)
{
    if( cam==NULL)
    {
        cam = &camera;
    }
    if(spriteToDraw!=NULL)
    {
        if( spriteToDraw->spriteTexture!=NULL )
        {
            //spriteToDraw->spriteTexture->change_color(rendColor);
            if( (subImageToDraw < (int)spriteToDraw->spriteImages.size() )&& (subImageToDraw>=0) )
            {
                if( cam!=NULL)
                {
                    //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
                    {
                        render_texture_rotated( cRenderer,spriteToDraw->spriteTexture , xPos-cam->x,yPos-cam->y, newAngle, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) , destination);
                    }
                }
                else
                {
                    render_texture_rotated( cRenderer,spriteToDraw->spriteTexture , xPos,yPos, newAngle, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) , destination);
                }
            }
            //spriteToDraw->spriteTexture->change_color(c_white);
        }
    }
}

int sprite_get_number(GPE_Sprite* spriteIn)
{
    if(spriteIn!=NULL)
    {
        return spriteIn->framesForSprite;
    }
    else
    {
        return -1;
    }
}

