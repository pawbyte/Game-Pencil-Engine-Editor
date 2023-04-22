/*
gpe_background.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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
        strech_bg = false;
        bg_id = is_null;
        texture_background = nullptr;
        bg_xoffset = 0;
        bg_yoffset = 0;
        bg_xspeed = 0;
        bg_yspeed = 0;
        bg_color = c_black;
        render_using_bg_speed = 0;
        loop_bg_hori = 0;
        loop_bg_vert = 0;
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
        return bg_id;
    }

    void background::render()
    {

    }

    void background::render_bg_speed( int xx, int yy, shape_rect *  cam, shape_rect * render_camera)
    {
        if(texture_background !=nullptr )
        {
            float camera_scaleX = 1;
            float camera_scaleY = 1;
            if( cam!=nullptr)
            {
                texture_background->render_tex(  xx*camera_scaleX - cam->get_x(),yy*camera_scaleY- cam->get_y() );
            }
            else
            {
                texture_background->render_tex(  xx*camera_scaleX - cam->get_x(),yy*camera_scaleY );
            }
            //texture_background->render_tex(  xx*camera_scaleX - cam.x_pos, yy*camera_scaleY - cam.y_pos,texture_background.get_width()*camera_scaleX,texture_background.get_height()*camera_scaleY );
        }
    }

    void background::render_bg( shape_rect * renderToArea, shape_rect *  cam, shape_rect *  render_camera, int scene_width, int scene_height)
    {
        bool colCam = true;
        if(texture_background !=nullptr )
        {
            if( loop_bg_hori )
            {
                if( loop_bg_vert )
                {
                    for( float i=bg_xoffset-texture_background->get_width(); i<=scene_width+texture_background->get_width(); i+=texture_background->get_width() )
                    {
                        for( float j=bg_yoffset-texture_background->get_height(); j<=scene_height+texture_background->get_height(); j+=texture_background->get_height() )
                        {
                            //colCam=gpe.check_collision_with_rect (i,j,texture_background->get_width(),texture_background->get_height(),cam );
                            if(colCam)
                            {
                                //Show the background
                                render_bg_speed(i, j, cam, render_camera);
                            }
                        }
                    }
                }
                else
                {
                    for(float i=bg_xoffset-texture_background->get_width(); i<=scene_width+bg_xoffset+texture_background->get_width(); i+=texture_background->get_width() )
                    {
                        //colCam=gpe.check_collision_with_rect( i, bg_yoffset, texture_background->get_width(),texture_background->get_height(),cam );
                        if(colCam)
                        {
                            //Show the background
                            render_bg_speed(i,bg_yoffset, cam,render_camera);
                        }
                    }
                }
            }
            else
            {
                if(loop_bg_vert)
                {
                    for(float j=bg_yoffset-texture_background->get_height(); j<=scene_height; j+=texture_background->get_height() )
                    {
                        //colCam=gpe.check_collision_with_rect( bg_xoffset,j,texture_background->get_width() , texture_background->get_height(),cam );
                        if(colCam)
                        {
                            //Show the background
                            render_bg_speed(bg_xoffset,j, cam,render_camera);
                        }
                    }
                }
                else
                {
                    //Show the background
                    render_bg_speed(bg_xoffset, bg_yoffset, cam,render_camera);
                }
            }
        }

    }

    void background::scroll_bg()
    {
        if(texture_background!=nullptr && strech_bg==false)
        {
            bg_xoffset +=bg_xspeed;
            bg_yoffset +=bg_yspeed;
            //If the background has gone too far
            if(loop_bg_hori)
            {
                if( bg_xoffset <= -texture_background->get_width() )
                {
                    //Reset the offset
                    bg_xoffset = 0;
                }
                if( bg_xoffset >=texture_background->get_width() )
                {
                    //Reset the offset
                    bg_xoffset = 0;
                }
            }
            if(loop_bg_vert)
            {
                if( bg_yoffset <= -texture_background->get_height() )
                {
                    //Reset the offset
                    bg_yoffset = 0;
                }
                if(bg_yoffset >= texture_background->get_height())
                {
                    //Reset the offset
                    bg_yoffset = 0;
                }
            }
        }
    }

    void background::set_bgspeed( float speed_x, float speed_y )
    {
        bg_xspeed = speed_x;
        bg_yspeed = speed_y;
    }

    void background::set_bgoffsets( float nextX, float nextY )
    {
        bg_xoffset = nextX;
        bg_yoffset = nextY;
    }

    void background::set_background( int new_bg_id)
    {
        /*
        if(  new_bg_id>=0 && new_bg_id < (int)rsm->rTextures.size() )
        {
            bg_id = new_bg_id;
            texture_background = rsm->rTextures[new_bg_id ];
            //console.log("Texture changed...");

        }
        else
        {
            bg_id = is_null;
            texture_background = nullptr;
            //console.log("Unable to set background to "+new_bg_id+".");
        }
        */
    }

    void background::set_background_color( color * color_in)
    {
        bg_color=color_in;
    }

    void background::update(float delta_time)
    {

    }
}
