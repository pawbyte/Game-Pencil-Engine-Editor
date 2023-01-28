/*
gpe_camera.cpp
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

#include "gpe_camera.h"
#include "gpe_constants.h"

namespace gpe
{
    //The camera
    shape_rect * camera_default = nullptr;
    shape_rect * camera_current = nullptr;

    shape_rect * camera_find( shape_rect * rect_in)
    {
        if(rect_in==nullptr)
        {
            return camera_default;
        }
        return rect_in;
    }

    void camera_reset()
    {
        if( camera_default != nullptr )
        {
            camera_default->x = 0;
            camera_default->y = 0;
            camera_default->w = screen_width;
            camera_default->h = screen_height;
        }
    }


    scene_camera::scene_camera( float camera_x, float camera_y, float camera_w, float camera_h, int cameraIdNumber )
    {
        camera_rect = new gpe::shape_rect();
        camera_boundaries = new gpe::shape_rect();
        camera_rect->update_shape(camera_x, camera_y, camera_w, camera_h);

        render_rect = new gpe::shape_rect();
        scaled_rect = new gpe::shape_rect();
        scaled_rect->update_shape(camera_x, camera_y, camera_w, camera_h);
        view_target = is_null;
        border_horizontal = is_null;
        border_vertical = is_null;

        speed_horizontal = 0;
        speed_vertical = 0;

        camera_visible = true;

        max_tiles_x_in_view = 0;
        max_tiles_y_in_view = 0;

        camera_scene_width = 0;
        camera_scene_height = 0;
    }

    scene_camera::~scene_camera()
    {
        if( camera_rect!=nullptr )
        {
            delete camera_rect;
            camera_rect = nullptr;
        }

        if( camera_boundaries!=nullptr )
        {
            delete camera_boundaries;
            camera_boundaries = nullptr;
        }

        if( render_rect!=nullptr )
        {
            delete render_rect;
            render_rect = nullptr;
        }

        if( scaled_rect!=nullptr )
        {
            delete scaled_rect;
            scaled_rect = nullptr;
        }
    }

    void scene_camera::center_camera ( float center_x, float center_y)
    {
        camera_rect->update_position(center_x-camera_rect->get_width()/2,center_y-camera_rect->get_height()/2);
        if( camera_rect->get_x2() > camera_boundaries->get_width() )
        {
            camera_rect->set_x(camera_boundaries->get_width()-camera_rect->get_width() );
        }
        if( camera_rect->get_y2() > camera_boundaries->get_height() )
        {
            camera_rect->set_y( camera_boundaries->get_height()-camera_rect->get_height() );
        }

        if( camera_rect->get_x() < 0 )
        {
            camera_rect->set_x(0);
            //console.log("Setting cam X to 0 for protection...");
        }
        if( camera_rect->get_y() < 0)
        {
            camera_rect->set_y(0);
            //console.log("Setting cam Y to 0 for protection...");

        }
        //console.log( "New Camera Pos:	("+camera_rect.get_x()+" , "+camera_rect.get_y()+")" );

    }

    float scene_camera::scene_camera::get_angle()
    {
        return camera_angle;
    }

    bool scene_camera::is_visible()
    {
        return camera_visible;
    }

    void scene_camera::move_hori ( float move_px )
    {
        if (move_px < 0 )
        {
            if( camera_boundaries->get_x() < camera_rect->get_x() +move_px )
            {
                camera_rect->add_x( move_px);
            }
            else
            {
                camera_rect->set_x(0);
            }
        }
        else
        {
            if( camera_boundaries->get_x2() > camera_rect->get_x2() +move_px )
            {
                camera_rect->add_x( move_px);
            }
        }
    }

    void scene_camera::move_vert ( float move_px )
    {
        if (move_px < 0 )
        {
            if( camera_boundaries->get_y() < camera_rect->get_y() +move_px )
            {
                camera_rect->add_y( move_px);
            }
            else
            {
                camera_rect->set_y(0);
            }
        }
        else
        {
            if( camera_boundaries->get_y2() > camera_rect->get_y2() +move_px )
            {
                camera_rect->add_y( move_px);
            }
        }
    }

    void scene_camera::scene_camera::set_angle( float new_angle )
    {
        if( new_angle < 0 )
        {
            new_angle *= -1; // only positives.
        }

        camera_angle = new_angle;
    }


    void scene_camera::set_visible( bool visiblility )
    {
        camera_visible = visiblility;
    }

    void scene_camera::setup_camera (float camera_x,float camera_y, float camera_w, float camera_h)
    {
        camera_visible = true;
        //console.log("Updaing Camera with ("+camera_x+", "+camera_y+", "+ camera_w+", "+camera_h+") dimensions...");
        render_rect->update_shape(camera_x, camera_y, camera_w, camera_h);
        camera_rect->update_shape(camera_x, camera_y, camera_w, camera_h);
    }

    void scene_camera::update_tile_size (int new_xt, int new_yt)
    {
        max_tiles_x_in_view = new_xt;
        max_tiles_y_in_view = new_yt;
    }

    void scene_camera::update_screen_space ( float camera_x, float camera_y, float camera_w, float camera_h)
    {
        render_rect->update_shape(camera_x, camera_y, camera_w, camera_h);
    }

    void scene_camera::update_view_space ( float camera_x, float camera_y, float camera_w, float camera_h)
    {
        camera_rect->update_shape(camera_x, camera_y, camera_w, camera_h);
    }
}
