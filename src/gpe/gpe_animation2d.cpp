/*
gpe_animation2d.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "gpe_animation2d.h"

namespace gpe
{
    //adds a animation with only one row of subimages
    animaton2d::animaton2d(render_package * renderPackage,std::string animName, std::string animFileName, bool imgTransparent)
    {
        animationTexture = NULL;
        name = animName;
        fileName = animFileName;
        load_image( renderPackage, animFileName, imgTransparent );
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
        colBox = new shape_rect();
        colBox->x = 0;
        colBox->y = 0;
        colBox->w = 0;
        colBox->h = 0;
    }

    animaton2d::~animaton2d()
    {
        clean_up();
    }

    void animaton2d::clean_up()
    {
        reset_frames();
    }

    bool animaton2d::copy_image_source(std::string outDirectoryName)
    {
        if( animationTexture!=NULL )
        {
            return animationTexture->copy_image_source( outDirectoryName );
        }
        return false;
    }

    void animaton2d::edit_collision_box(int cx, int cy, int cw, int ch)
    {
        colBox->update_box( cx, cy, cw, ch );
    }

    void animaton2d::edit_collision_circle(int cx, int cy, int cr)
    {
        colBox->x=cx;
        colBox->y=cy;
        colBox->w = 0;
        colBox->h = 0;
        colRadius=cr;

    }

    int animaton2d::get_frame_count()
    {
        return (int) animFrames.size();
    }

    int animaton2d::get_width()
    {
        return width;
    }

    int animaton2d::get_height()
    {
        return height;
    }

    int animaton2d::get_hpadding()
    {
        return hPadding;
    }

    int animaton2d::get_vpadding()
    {
        return vPadding;
    }

    int animaton2d::get_texture_width()
    {
        if( animationTexture!=NULL)
        {
            return animationTexture->get_width();
        }
        return -1;
    }

    int animaton2d::get_texture_height()
    {
        if( animationTexture!=NULL)
        {
            return animationTexture->get_height();
        }
        return -1;
    }

    int animaton2d::get_frame_xoffset()
    {
        return xoffset;
    }

    int animaton2d::get_frame_yoffset()
    {
        return yoffset;
    }


    std::string animaton2d::get_file_name()
    {
        return fileName;
    }

    std::string animaton2d::get_name()
    {
        return name;
    }

    bool animaton2d::has_texture()
    {
        return (animationTexture!=NULL);
    }

    void animaton2d::load_image( render_package * renderPackage, std::string animFileName, bool transparent )
    {
        if( renderPackage == NULL)
        {
            return;
        }
        clean_up();
        if( animationTexture != NULL )
        {
            delete animationTexture;
            animationTexture = NULL;
        }
        animationTexture = renderPackage->create_texture();
        fileName = animFileName;
        if( sff_ex::file_exists( animFileName) )
        {
            animationTexture->load_new_texture(renderPackage->packageRenderer,animFileName, -1,transparent );
        }
    }

    void animaton2d::render(int subImageToDraw, int xPos, int yPos,  shape_rect *cam)
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

    void animaton2d::render_colored(int subImageToDraw, int xPos, int yPos,color * renderColor, int alpha ,  shape_rect *cam)
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

    void animaton2d::render_piece( int xPos, int yPos, shape_rect * rectPiece, shape_rect * cam )
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

    void animaton2d::render_piece_resized( int xPos, int yPos, int newWidth, int newHeight, shape_rect * rectPiece, shape_rect * cam )
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

    void animaton2d::render_resized(int subImageToDraw, int xPos, int yPos, float newWidth, float newHeight, shape_rect * cam )
    {
        if( width !=0 && height!= 0 )
        {
            float foundXScale = newWidth/(float)width;
            float foundYScale = newHeight/(float)height;
            render_scaled(subImageToDraw, xPos, yPos, foundXScale, foundYScale, cam );
        }
    }

    void animaton2d::render_rotated(int subImageToDraw, int xPos, int yPos, float newAngle, float xScale, float yScale, shape_rect * cam)
    {
        if( newAngle == 0 || newAngle==360 )
        {
            render_scaled(subImageToDraw, xPos, yPos, xScale, yScale, cam );
            return;
        }
        else if(animationTexture!=NULL && (subImageToDraw < (int)animFrames.size() )&& (subImageToDraw>=0) )
        {
            int newWidth = (float)width * xScale;
            int newHeight = (float)height * yScale;
            if( cam!=NULL)
            {
                //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+animationToDraw->width,yPos+animationToDraw->height) == true )
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

    void animaton2d::render_scaled(int subImageToDraw, int xPos, int yPos, float xScale, float yScale, shape_rect * cam )
    {
        if( xScale ==0 || yScale ==0 )
        {
            return;
        }

        if(animationTexture!=NULL && (subImageToDraw < (int)animFrames.size() )&& (subImageToDraw>=0) )
        {
            if( cam!=NULL)
            {
                //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+animationToDraw->width,yPos+animationToDraw->height) == true )
                animationTexture->render_tex_scaled( xPos-cam->x-abs(colBox->xCenter*xScale),yPos-cam->y-abs(colBox->yMiddle*yScale),xScale, yScale, animFrames.at(subImageToDraw) );
            }
            else
            {
                animationTexture->render_tex_scaled(   xPos-abs(colBox->xCenter*xScale),yPos-abs(colBox->yMiddle*yScale), xScale, yScale,animFrames.at(subImageToDraw) );
            }
        }
    }

    void animaton2d::render_special( int subImageToDraw, int xPos, int yPos, float xScale, float yScale, float newAngle,color * renderColor, int alpha, shape_rect * cam )
    {
        if( xScale <=0 || yScale <=0 )
        {
            return;
        }

        if(animationTexture!=NULL && (subImageToDraw < (int)animFrames.size() )&& (subImageToDraw>=0) )
        {
            int newWidth = (float)width * xScale;
            int newHeight = (float)height * yScale;
            float xPivot = colBox->get_center() * xScale;
            float yPivot = colBox->get_middle() * yScale;
            if( cam!=NULL)
            {
                //if(check_collision(*cam,(int)xPos,(int)yPos,(int)xPos+animationToDraw->width,yPos+animationToDraw->height) == true )
                {
                    animationTexture->render_tex_special_at_point( xPos + xPivot - cam->x,yPos + yPivot - cam->y, newAngle, xPivot, yPivot, newWidth, newHeight,renderColor,animFrames.at(subImageToDraw), alpha );
                }
            }
            else
            {
                animationTexture->render_tex_special_at_point( xPos + xPivot,yPos + yPivot, newAngle, xPivot, yPivot, newWidth, newHeight,renderColor,animFrames.at(subImageToDraw), alpha );
            }
            //animationTexture->change_color(c_white);
        }
    }

    void animaton2d::reset_frames()
    {
        shape_rect * cRect = NULL;
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

    void animaton2d::setup_animation( int fCount, int aw, int ah, int sofx, int sofy, int hPad, int vPad )
    {
        reset_frames();
        frameCount = fCount;
        width = aw;
        height = ah;
        xoffset = sofx;
        yoffset = sofy;
        hPadding = hPad;
        vPadding = vPad;

        int animationsAdded = 0;
        if( animationTexture==NULL)
        {
            frameCount = 0;
            return;
        }
        shape_rect * newRect = NULL;

        int i = 0, j = 0;
        for( i  = yoffset; i < animationTexture->get_height();  i += height+vPadding)
        {
            for( j = xoffset; j < animationTexture->get_width();  j += width+hPadding)
            {
                if( animationsAdded <  frameCount )
                {
                    newRect = new shape_rect();
                    newRect->x = j;
                    newRect->y = i;
                    newRect->w = width;
                    newRect->h = height;
                    animFrames.push_back( newRect );
                    animationsAdded++;
                }
                else
                {
                    return;
                }
            }
        }
    }


    void animaton2d::setup_fullimg_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad )
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

        shape_rect * newRect = NULL;
        int i = 0, j = 0;
        for( i  = yoffset; i < animationTexture->get_height();  i += height+vPadding)
        {
            for( j = xoffset; j < animationTexture->get_width();  j += width+hPadding)
            {
                newRect = new shape_rect();
                newRect->x = j;
                newRect->y = i;
                newRect->w = width;
                newRect->h = height;
                animFrames.push_back( newRect );
                frameCount++;
            }
        }
    }
}
