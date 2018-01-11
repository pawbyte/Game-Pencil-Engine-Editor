/*
GPE_Animation.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

#include "GPE_Animation.h"
//adds a sprite with only one row of subimages
GPE_Animation::GPE_Animation()
{
    boundingBox = new GPE_Rect();
    //boundingBox.x=0;
    //boundingBox.y=0;
    //boundingBox.w=0;
    //boundingBox.h=0;s
    animationTexture = NULL;
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

GPE_Animation::GPE_Animation(std::string fileName, bool imgTransparent)
{
    boundingBox = new GPE_Rect();
    //boundingBox.x=0;
    //boundingBox.y=0;
    //boundingBox.w=0;
    //boundingBox.h=0;s
    animationTexture = new GPE_Texture();
    animationTexture->load_new_texture( fileName, -1,imgTransparent );
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

GPE_Animation::~GPE_Animation()
{
    if( animationTexture!=NULL)
    {
        delete animationTexture;
        animationTexture = NULL;
    }
}

void GPE_Animation::render(int subImageToDraw, int xPos, int yPos,  GPE_Rect *cam)
{
    render_animation( this,subImageToDraw,xPos,yPos,cam);
}

void GPE_Animation::render_resized(int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, GPE_Rect * cam )
{
    if(animationTexture!=NULL)
    {
        if( (subImageToDraw < (int)spriteImages.size() )&& (subImageToDraw>=0) )
        {
            if( cam!=NULL)
            {
//                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+width,yPos+height) == true )
                {
                    render_texture_resized(  animationTexture , xPos-cam->x,yPos-cam->y, newWidth, newHeight,&(spriteImages.at(subImageToDraw)) );
                }
            }
            else
            {
                render_texture_resized(  animationTexture , xPos,yPos, newWidth, newHeight,&(spriteImages.at(subImageToDraw)) );
            }
        }
    }
}

void GPE_Animation::clean_up()
{
    if( animationTexture!=NULL)
    {
        delete animationTexture;
        animationTexture = NULL;
    }
}

void render_animation(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, GPE_Rect *cam)
{
    if(spriteToDraw!=NULL)
    {
        if( (subImageToDraw < (int)spriteToDraw->spriteImages.size() )&& (subImageToDraw>=0) )
        {
            if( cam!=NULL)
            {
                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
                {
                    render_texture(  spriteToDraw->animationTexture , xPos-cam->x,yPos-cam->y, &(spriteToDraw->spriteImages.at(subImageToDraw)) );
                }
            }
            else
            {
                render_texture(  spriteToDraw->animationTexture , xPos,yPos, &(spriteToDraw->spriteImages.at(subImageToDraw)) );
            }
        }
    }
}


void render_animation_resized(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, GPE_Rect * cam)
{
    if(spriteToDraw!=NULL)
    {
        if( (subImageToDraw < (int)spriteToDraw->spriteImages.size() )&& (subImageToDraw>=0) )
        {
            if( cam!=NULL)
            {
//                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
                {
                    render_texture_resized(  spriteToDraw->animationTexture , xPos-cam->x,yPos-cam->y, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) );
                }
            }
            else
            {
                render_texture_resized(  spriteToDraw->animationTexture , xPos,yPos, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) );
            }
        }
    }
}

void render_animation_ext(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Color * rendColor, GPE_Rect * cam)
{
    if( cam==NULL)
    {
        cam = &GPE_DEFAULT_CAMERA;
    }
    if(spriteToDraw!=NULL)
    {
        if( spriteToDraw->animationTexture!=NULL )
        {
            //spriteToDraw->animationTexture->change_color(rendColor);
            if( (subImageToDraw < (int)spriteToDraw->spriteImages.size() )&& (subImageToDraw>=0) )
            {
                if( cam!=NULL)
                {
                    //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
                    {
                        render_texture_resized(  spriteToDraw->animationTexture , xPos-cam->x,yPos-cam->y, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) ,-1,-1,rendColor);
                    }
                }
                else
                {
                    render_texture_resized(  spriteToDraw->animationTexture , xPos,yPos, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) ,-1,-1,rendColor);
                }
            }
            spriteToDraw->animationTexture->change_color(c_white);
        }
    }
}

void render_animation_rotated(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, double newAngle, int newWidth, int newHeight, GPE_Rect *cam)
{
    if( cam==NULL)
    {
        cam = &GPE_DEFAULT_CAMERA;
    }
    if(spriteToDraw!=NULL)
    {
        if( spriteToDraw->animationTexture!=NULL )
        {
            //spriteToDraw->animationTexture->change_color(rendColor);
            if( (subImageToDraw < (int)spriteToDraw->spriteImages.size() )&& (subImageToDraw>=0) )
            {
                if( cam!=NULL)
                {
                    //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
                    {
                        render_texture_rotated(  spriteToDraw->animationTexture , xPos-cam->x,yPos-cam->y, newAngle, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) );
                    }
                }
                else
                {
                    render_texture_rotated(  spriteToDraw->animationTexture , xPos,yPos, newAngle, newWidth, newHeight,&(spriteToDraw->spriteImages.at(subImageToDraw)) );
                }
            }
            //spriteToDraw->animationTexture->change_color(c_white);
        }
    }
}

int sprite_get_number(GPE_Animation* spriteIn)
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

