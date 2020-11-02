/*
gpe_background.cpp
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

#include "gpe_background.h"

namespace gpe
{
    background::background()
    {
        stretchBG = false;
        bgId = is_null;
        backgroundTexture = NULL;
        bgXOffset = 0;
        bgYOffset = 0;
        bgXSpeed = 0;
        bgYSpeed = 0;
        bg_color = c_black;
        renderBgSpeed = 0;
        loopBGHori = 0;
        loopBGVert = 0;
    }

    background::~background()
    {

    }

    color * background::get_scene_background_color()
    {
        return bg_color;
    }


    int background::get_background_id()
    {
        return bgId;
    }

    void background::render()
    {

    }

    void background::render_bg_speed( int xx, int yy, shape_rect *  cam, shape_rect * renderCam)
    {
        if(backgroundTexture !=NULL )
        {
            float cameraScaleX = 1;
            float cameraScaleY = 1;
            if( cam!=NULL)
            {
                backgroundTexture->render_tex(  xx*cameraScaleX - cam->get_x(),yy*cameraScaleY- cam->get_y() );
            }
            else
            {
                backgroundTexture->render_tex(  xx*cameraScaleX - cam->get_x(),yy*cameraScaleY );
            }
            //backgroundTexture->render_tex(  xx*cameraScaleX - cam.xPos, yy*cameraScaleY - cam.yPos,backgroundTexture.get_width()*cameraScaleX,backgroundTexture.get_height()*cameraScaleY );
        }
    }

    void background::render_bg( shape_rect * renderToArea, shape_rect *  cam, shape_rect *  renderCam, int lvlW, int lvlH)
    {
        bool colCam = true;
        if(backgroundTexture !=NULL )
        {
            if( loopBGHori )
            {
                if( loopBGVert )
                {
                    for( float i=bgXOffset-backgroundTexture->get_width(); i<=lvlW+backgroundTexture->get_width(); i+=backgroundTexture->get_width() )
                    {
                        for( float j=bgYOffset-backgroundTexture->get_height(); j<=lvlH+backgroundTexture->get_height(); j+=backgroundTexture->get_height() )
                        {
                            //colCam=gpe.check_collision_with_rect (i,j,backgroundTexture->get_width(),backgroundTexture->get_height(),cam );
                            if(colCam)
                            {
                                //Show the background
                                render_bg_speed(i, j, cam, renderCam);
                            }
                        }
                    }
                }
                else
                {
                    for(float i=bgXOffset-backgroundTexture->get_width(); i<=lvlW+bgXOffset+backgroundTexture->get_width(); i+=backgroundTexture->get_width() )
                    {
                        //colCam=gpe.check_collision_with_rect( i, bgYOffset, backgroundTexture->get_width(),backgroundTexture->get_height(),cam );
                        if(colCam)
                        {
                            //Show the background
                            render_bg_speed(i,bgYOffset, cam,renderCam);
                        }
                    }
                }
            }
            else
            {
                if(loopBGVert)
                {
                    for(float j=bgYOffset-backgroundTexture->get_height(); j<=lvlH; j+=backgroundTexture->get_height() )
                    {
                        //colCam=gpe.check_collision_with_rect( bgXOffset,j,backgroundTexture->get_width() , backgroundTexture->get_height(),cam );
                        if(colCam)
                        {
                            //Show the background
                            render_bg_speed(bgXOffset,j, cam,renderCam);
                        }
                    }
                }
                else
                {
                    //Show the background
                    render_bg_speed(bgXOffset, bgYOffset, cam,renderCam);
                }
            }
        }

    }

    void background::scroll_bg()
    {
        if(backgroundTexture!=NULL && stretchBG==false)
        {
            bgXOffset +=bgXSpeed;
            bgYOffset +=bgYSpeed;
            //If the background has gone too far
            if(loopBGHori)
            {
                if( bgXOffset <= -backgroundTexture->get_width() )
                {
                    //Reset the offset
                    bgXOffset = 0;
                }
                if( bgXOffset >=backgroundTexture->get_width() )
                {
                    //Reset the offset
                    bgXOffset = 0;
                }
            }
            if(loopBGVert)
            {
                if( bgYOffset <= -backgroundTexture->get_height() )
                {
                    //Reset the offset
                    bgYOffset = 0;
                }
                if(bgYOffset >= backgroundTexture->get_height())
                {
                    //Reset the offset
                    bgYOffset = 0;
                }
            }
        }
    }

    void background::set_bgspeed( float speedX, float speedY )
    {
        bgXSpeed = speedX;
        bgYSpeed = speedY;
    }

    void background::set_bgoffsets( float nextX, float nextY )
    {
        bgXOffset = nextX;
        bgYOffset = nextY;
    }

    void background::set_background( int newBG)
    {
        /*
        if(  newBG>=0 && newBG < (int)rsm->rTextures.size() )
        {
            bgId = newBG;
            backgroundTexture = rsm->rTextures[newBG ];
            //console.log("Texture changed...");

        }
        else
        {
            bgId = is_null;
            backgroundTexture = NULL;
            //console.log("Unable to set background to "+newBG+".");
        }
        */
    }

    void background::set_background_color( color * colorIn)
    {
        bg_color=colorIn;
    }

    void background::update()
    {

    }
}
