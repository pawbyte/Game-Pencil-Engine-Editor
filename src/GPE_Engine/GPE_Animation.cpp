/*
GPE_Animation.cpp
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

#include "GPE_Animation.h"
//adds a sprite with only one row of subimages

GPE_Animation::GPE_Animation(std::string animName, std::string animFileName, bool imgTransparent)
{
    animationTexture = NULL;
    usingCustomTexture = true;
    name = animName;
    fileName = animFileName;
    load_image( animFileName, imgTransparent );
    width=0;
    height=0;
    xoffset=0;
    yoffset=0;
    hPadding = 0;
    vPadding = 0;
    frameCount=0;
    name = "";
    fileName = "";
    animId = -1;
    colRadius = 0;
    colBox = new GPE_Rect();
    colBox->x = 0;
    colBox->y = 0;
    colBox->w = 0;
    colBox->h = 0;
}

GPE_Animation::~GPE_Animation()
{
    clean_up();
}

void GPE_Animation::change_texture(GPE_Texture * newTexture)
{
    clean_up();
    animationTexture = newTexture;
    usingCustomTexture = false;
}

void GPE_Animation::clean_up()
{
    if( usingCustomTexture && animationTexture!=NULL)
    {
        delete animationTexture;
        animationTexture = NULL;
    }
    usingCustomTexture = false;
    reset_frames();
}

bool GPE_Animation::copy_image_source(std::string outDirectoryName)
{
    if( animationTexture!=NULL )
    {
        return animationTexture->copy_image_source( outDirectoryName );
    }
    return false;
}

void GPE_Animation::edit_collision_box(int cx, int cy, int cw, int ch)
{
    colBox->update_box( cx, cy, cw, ch );
}

void GPE_Animation::edit_collision_circle(int cx, int cy, int cr)
{
    colBox->x=cx;
    colBox->y=cy;
    colBox->w = 0;
    colBox->h = 0;
    colRadius=cr;

}

int GPE_Animation::get_frame_count()
{
    return (int) animFrames.size();
}

int GPE_Animation::get_width()
{
    return width;
}

int GPE_Animation::get_height()
{
    return height;
}

int GPE_Animation::get_hpadding()
{
    return hPadding;
}

int GPE_Animation::get_vpadding()
{
    return vPadding;
}

int GPE_Animation::get_texture_width()
{
    if( animationTexture!=NULL)
    {
        return animationTexture->get_width();
    }
    return -1;
}

int GPE_Animation::get_texture_height()
{
    if( animationTexture!=NULL)
    {
        return animationTexture->get_height();
    }
    return -1;
}

int GPE_Animation::get_frame_xoffset()
{
    return xoffset;
}

int GPE_Animation::get_frame_yoffset()
{
    return yoffset;
}


std::string GPE_Animation::get_file_name()
{
    return fileName;
}

std::string GPE_Animation::get_name()
{
    return name;
}

bool GPE_Animation::has_texture()
{
    return (animationTexture!=NULL);
}

void GPE_Animation::load_image( std::string animFileName, bool transparent )
{
    clean_up();
    usingCustomTexture = true;
    animationTexture = new GPE_Texture();
    animationTexture->load_new_texture( animFileName, -1,transparent );
    fileName = animFileName;
    /*
    if(newSurface!=NULL)
    {
        //gets the height and width of spritesheet
        int nW = newSurface->get_width();
        int nH =  newSurface->get_height();
        if( (nH >= yorig )&&(nW  >=xorig) )
        {
            if(imgnumb<1)
            {
                imgnumb=1;
            }
            newSprite = new GPE_Animation();
            newSprite->width = (nW-xorig)/imgnumb;
            newSprite->height = nH-yorig;
            newSprite->colBox = new GPE_Rect;
            newSprite->colBox->x=0;
            newSprite->colBox->y=0;
            newSprite->colBox->w=newSprite->width;
            newSprite->colBox->h=newSprite->height;

            newSprite->animationTexture= newSurface;
            //newSprite->spriteColor = c_white;
            //newSprite->spritePhase = 0;
            if(addMirror)
            {
                //  GPE_Texture newMirSurface = flip_surface( newSurface, FLIP_HORIZONTAL );
                //  newSprite->spriteMirSurface=newMirSurface;
            }
            newSprite->frameCount = 0;
            //newSprite->animFramesLeft[newSprite->frameCount];
            //newSprite->animFramesNight[newSprite->frameCount];
            //newSprite->animFramesNightLeft[newSprite->frameCount];

            newSprite->fileName=spriteFileName; //the name along with path
            newSprite->name=getShortFileName(spriteFileName); //gets rid of path

            //Go through rows for the spritesheet
            for( int x = 0; x <= nW; x+=newSprite->width )
            {
                GPE_Rect * newSubImage = new GPE_Rect;
                newSubImage->x = x;
                newSubImage->y = yorig;
                if(nW>=x+newSprite->width)
                {
                    newSubImage->w = newSprite->width;
                }
                else
                {
                    newSubImage->w = x+newSprite->width-nW;
                }
                newSubImage->h = newSprite->height;
                newSprite->frameCount+=1;

                newSprite->animFrames.push_back(*newSubImage);
            }
            //big ole duh!
            //gpe_error_logFile << "The Sprite with the file name '" << spriteFileName << "' was added successfully!";
            //gpe_error_logFile << " \n";
            std::stringstream outString;
            rAnimations.push_back(newSprite);
        }
        else
        {
            std::stringstream outString;
            outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to invalid dimensions given";
            GPE_Report( outString.str());
        }
    }
    else
    {
        std::stringstream outString;
        outString << "Unable to load Sprite from file location: (" << spriteFileName << ") due to invalid dimensions given";
        GPE_Report( outString.str());
    }
    */
}

void GPE_Animation::render(int subImageToDraw, int xPos, int yPos,  GPE_Rect *cam)
{
    if(animationTexture!=NULL)
    {
        if( (subImageToDraw < (int)animFrames.size() )&& (subImageToDraw>=0) )
        {
            if( cam!=NULL)
            {
//                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+width,yPos+height) == true )
                {
                    animationTexture->render_tex( xPos-cam->x-colBox->get_center(),yPos-cam->y-colBox->get_middle(), (animFrames.at(subImageToDraw)) );
                }
            }
            else
            {
                animationTexture->render_tex( xPos-colBox->get_center(),yPos-colBox->get_middle(), (animFrames.at(subImageToDraw)) );
            }
        }
    }
}

void GPE_Animation::render_colored(int subImageToDraw, int xPos, int yPos,GPE_Color * renderColor, int alpha ,  GPE_Rect *cam)
{
    if(animationTexture!=NULL)
    {
        if( (subImageToDraw < (int)animFrames.size() )&& (subImageToDraw>=0) )
        {
            if( cam!=NULL)
            {
//                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+width,yPos+height) == true )
                {
                    animationTexture->render_tex_colored( xPos-cam->x-colBox->get_center(),yPos-cam->y-colBox->get_middle(),renderColor,alpha,(animFrames.at(subImageToDraw)) );
                }
            }
            else
            {
                animationTexture->render_tex_colored( xPos-colBox->get_center(),yPos-colBox->get_middle(), renderColor,alpha,animFrames.at(subImageToDraw) );
            }
        }
    }
}

void GPE_Animation::render_piece( int xPos, int yPos, GPE_Rect * rectPiece,GPE_Rect * cam )
{
    if(animationTexture!=NULL  )
    {
        //if( animationTexture->get_width() < rectPiece->x + rectPiece->w && animationTexture->get_height() < rectPiece->y +rectPiece->h )
        if( rectPiece!=NULL )
        {
            if( cam!=NULL)
            {
//                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+width,yPos+height) == true )
                {
                    animationTexture->render_tex( xPos-cam->x,yPos-cam->y, rectPiece );
                }
            }
            else
            {
                animationTexture->render_tex( xPos,yPos, rectPiece );
            }
        }
        else if( (int)animFrames.size() > 0 )
        {
            if( cam!=NULL)
            {
//                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+width,yPos+height) == true )
                {
                    animationTexture->render_tex( xPos-cam->x,yPos-cam->y, animFrames[0] );
                }
            }
            else
            {
                animationTexture->render_tex( xPos,yPos,animFrames[0] );
            }
        }
    }
}

void GPE_Animation::render_piece_resized( int xPos, int yPos, int newWidth, int newHeight,GPE_Rect * rectPiece,GPE_Rect * cam )
{
    if(animationTexture!=NULL )
    {
        if( rectPiece!=NULL )
        {
            //if( animationTexture->get_width() < rectPiece->x + rectPiece->w && animationTexture->get_height() < rectPiece->y +rectPiece->h )
            {
                if( cam!=NULL)
                {
    //                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+width,yPos+height) == true )
                    {
                        animationTexture->render_tex_resized( xPos-cam->x,yPos-cam->y, newWidth, newHeight,rectPiece );
                    }
                }
                else
                {
                    animationTexture->render_tex_resized( xPos,yPos, newWidth, newHeight,rectPiece );
                }
            }
        }
        else if( (int)animFrames.size() > 0 )
        {
            if( cam!=NULL)
            {
//                if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+width,yPos+height) == true )
                {
                    animationTexture->render_tex_resized( xPos-cam->x,yPos-cam->y, newWidth, newHeight,animFrames[0] );
                }
            }
            else
            {
                animationTexture->render_tex_resized( xPos,yPos, newWidth, newHeight,animFrames[0] );
            }
        }
    }
}

void GPE_Animation::render_resized(int subImageToDraw, int xPos, int yPos, double newWidth, double newHeight, GPE_Rect * cam )
{
    if( width !=0 && height!= 0 )
    {
        double foundXScale = newWidth/(double)width;
        double foundYScale = newHeight/(double)height;
        render_scaled(subImageToDraw, xPos, yPos, foundXScale, foundYScale, cam );
    }
}

void GPE_Animation::render_rotated(int subImageToDraw, int xPos, int yPos, double newAngle, double xScale, double yScale, GPE_Rect * cam)
{
    if( newAngle == 0 || newAngle==360 )
    {
        render_scaled(subImageToDraw, xPos, yPos, xScale, yScale, cam );
        return;
    }
    else if(animationTexture!=NULL && (subImageToDraw < (int)animFrames.size() )&& (subImageToDraw>=0) )
    {
        int newWidth = (double)width * xScale;
        int newHeight = (double)height * yScale;
        if( cam!=NULL)
        {
            //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
            {
                 animationTexture->render_tex_special_at_point( xPos-cam->x,yPos-cam->y, newAngle,abs(colBox->xCenter*xScale),abs(colBox->yMiddle*yScale), newWidth, newHeight, NULL,animFrames.at(subImageToDraw) );
            }
        }
        else
        {
            animationTexture->render_tex_special_at_point( xPos,yPos, newAngle, abs(colBox->xCenter*xScale),abs(colBox->yMiddle*yScale), newWidth, newHeight,NULL,animFrames.at(subImageToDraw) );
        }
        //animationTexture->change_color(c_white);
    }
}

void GPE_Animation::render_scaled(int subImageToDraw, int xPos, int yPos, double xScale, double yScale, GPE_Rect * cam )
{
    if( xScale ==0 || yScale ==0 )
    {
        return;
    }

    if(animationTexture!=NULL && (subImageToDraw < (int)animFrames.size() )&& (subImageToDraw>=0) )
    {
        if( cam!=NULL)
        {
            //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
            animationTexture->render_tex_scaled( xPos-cam->x-abs(colBox->xCenter*xScale),yPos+cam->y-abs(colBox->yMiddle*yScale),xScale, yScale, animFrames.at(subImageToDraw) );
        }
        else
        {
            animationTexture->render_tex_scaled(   xPos-abs(colBox->xCenter*xScale),yPos-abs(colBox->yMiddle*yScale), xScale, yScale,animFrames.at(subImageToDraw) );
        }
    }
}

void GPE_Animation::render_special( int subImageToDraw, int xPos, int yPos, double xScale, double yScale, double newAngle,GPE_Color * renderColor, int alpha, GPE_Rect * cam )
{
    if( xScale <=0 || yScale <=0 )
    {
        return;
    }

    if(animationTexture!=NULL && (subImageToDraw < (int)animFrames.size() )&& (subImageToDraw>=0) )
    {
        int newWidth = (double)width * xScale;
        int newHeight = (double)height * yScale;
        if( cam!=NULL)
        {
            //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+spriteToDraw->width,yPos+spriteToDraw->height) == true )
            {
                 animationTexture->render_tex_special_at_point( xPos-cam->x+colBox->x,yPos-cam->y+colBox->y, newAngle,colBox->x,colBox->y, newWidth, newHeight, renderColor,animFrames.at(subImageToDraw),alpha );
            }
        }
        else
        {
            animationTexture->render_tex_special_at_point( xPos+colBox->x,yPos+colBox->y, newAngle, colBox->x,colBox->y, newWidth, newHeight,renderColor,animFrames.at(subImageToDraw), alpha );
        }
        //animationTexture->change_color(c_white);
    }
}

void GPE_Animation::reset_frames()
{
    GPE_Rect * cRect = NULL;
    for( int i = (int)animFrames.size()-1; i >=0; i-- )
    {
        cRect = animFrames[i];
        if( cRect!=NULL )
        {
            delete cRect;
            cRect = NULL;
        }
    }
    animFrames.clear();
}

void GPE_Animation::setup_animation( int fCount, int aw, int ah, int sofx, int sofy, int hPad, int vPad )
{
    reset_frames();
    frameCount = fCount;
    width = aw;
    height = ah;
    xoffset = sofx;
    yoffset = sofy;
    hPadding = hPad;
    vPadding = vPad;

    int spritesAdded = 0;
    if( animationTexture==NULL)
    {
        frameCount = 0;
        return;
    }
    GPE_Rect * newRect = NULL;

    int i = 0, j = 0;
    for( i  = yoffset; i < animationTexture->get_height();  i += height+vPadding)
    {
        for( j = xoffset; j < animationTexture->get_width();  j += width+hPadding)
        {
            if( spritesAdded <  frameCount )
            {
                newRect = new GPE_Rect();
                newRect->x = j;
                newRect->y = i;
                newRect->w = width;
                newRect->h = height;
                animFrames.push_back( newRect );
                spritesAdded++;
            }
            else
            {
                return;
            }
        }
    }
}


void GPE_Animation::setup_fullimg_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad )
{
    reset_frames();
    frameCount = 0;
    width = aw;
    height = ah;
    xoffset = sofx;
    yoffset = sofy;
    hPadding = hPad;
    vPadding = vPad;

    if( animationTexture==NULL)
    {
        return;
    }

    GPE_Rect * newRect = NULL;
    int i = 0, j = 0;
    for( i  = yoffset; i < animationTexture->get_height();  i += height+vPadding)
    {
        for( j = xoffset; j < animationTexture->get_width();  j += width+hPadding)
        {
            newRect = new GPE_Rect();
            newRect->x = j;
            newRect->y = i;
            newRect->w = width;
            newRect->h = height;
            animFrames.push_back( newRect );
            frameCount++;
        }
    }
}
