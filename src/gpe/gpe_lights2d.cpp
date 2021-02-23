/*
gpe_lights2d.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_artist_base.h"
#include "gpe_lights2d.h"
#include "../other_libs/semath.h"

namespace gpe
{
    light_basic_2d::light_basic_2d()
    {
        xpos = 0;
        ypos = 0;
        light_intensity = 255;
        light_is_active = true;
        light_active_on_start = true;
        light_color = new color("Light",255,255 );
    }

    light_basic_2d::~light_basic_2d()
    {
        if( light_color!=nullptr )
        {
            delete light_color;
            light_color = nullptr;
        }
    }

    void light_basic_2d::render_light( float scale, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find( cam );
        gpe::gcanvas->render_rectangle(0,0, cam->w, cam->h, light_color, false, light_intensity );
    }

    void light_basic_2d::render_light_at( float x, float y, float scale, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find( cam );
        gpe::gcanvas->render_rect( cam, light_color, false, light_intensity );
    }

    void light_basic_2d::setup_light( int x, int y, int intensity, bool activeNow, bool activeOnLoad )
    {
        xpos = x;
        ypos = y;
        light_intensity = intensity;
        light_is_active = activeNow;
        light_active_on_start = activeOnLoad;
    }

    light_direction2d::light_direction2d()
    {
        light_x2 = light_y2 = 0;
        light_width = 64;
        light_length = 128;
        light_direction = 0;
        light_texture = nullptr;
    }

    light_direction2d::~light_direction2d()
    {

    }

    float light_direction2d::get_direction()
    {
        return light_direction;
    }

    void light_direction2d::render_light( float scale, gpe::shape_rect * cam )
    {
        render_light_at(xpos, ypos, scale, cam );
    }

    void light_direction2d::render_light_at( float x, float y, float scale, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find( cam );
        if( light_texture!=nullptr )
        {
            light_texture->render_tex_rotated( x - cam->x, y - cam->y,light_direction, light_color, nullptr, light_intensity );
        }
        else
        {
            light_x2 = x + semath::lengthdir_x( light_length*scale,light_direction ) - cam->x;
            light_y2 = y + semath::lengthdir_y( light_length*scale,light_direction ) - cam->y;
            //gpe::gcanvas->render_semi_circle(x,y,light_width,light_direction, false );
            gpe::gcanvas->render_line_width_color(x- cam->x, y - cam->y, light_x2, light_y2, light_width, light_color, light_intensity );
        }
    }

    void light_direction2d::set_direction( float newDir )
    {
        while( newDir < -360.f  )
        {
            newDir += 360.f ;
        }
        while( newDir >= 360.f )
        {
            newDir -= 360.f;
        }
        light_direction = newDir;
    }

    light_point2d::light_point2d()
    {
        light_radius = 32;
        light_radius_second = 16;
        light_radius_third = 0;
        light_flickers = true;
        light_flicker_amount = 0;
        light_flicker_time = 0;
        light_flicker_time_goal = 250;
        light_flicker_range = 2;
        light_radius_second_intensity = 255;
        light_radius_third_intensity = 0;
    }

    light_point2d::~light_point2d()
    {

    }

    void light_point2d::disable_flicker()
    {
        light_flickers = false;
        light_flicker_time = 0;
        light_flicker_amount = 0;
    }

    void light_point2d::enable_flicker()
    {
        light_flickers = true;
    }

    void light_point2d::render_light( float scale, gpe::shape_rect * cam )
    {
        render_light_at(xpos, ypos, scale, cam );
    }

    void light_point2d::render_light_at( float x, float y, float scale, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find( cam );
        gpe::gcanvas->render_circle_filled_color( x, y, (light_radius+light_flicker_amount)*scale,light_color, light_intensity );
        if( light_radius_second > 0 )
        {
            gpe::gcanvas->render_circle_filled_color( x, y, (light_radius_second+light_flicker_amount)*scale, light_color, light_radius_second_intensity );
        }
        if( light_radius_third > 0 )
        {
            gpe::gcanvas->render_circle_filled_color( x, y, (light_radius_third+light_flicker_amount)*scale, light_color, light_radius_third_intensity  );
        }
    }

    void light_point2d::setup_flicker( int fTime, int fRange )
    {
        if( fTime > 0 && fRange > 0 )
        {
            enable_flicker();
            light_flicker_time_goal = fTime;
            light_flicker_range = fRange;
            if( light_flicker_amount < -fRange)
            {
                light_flicker_amount = -fRange;
            }
            else if( light_flicker_amount > fRange )
            {
                light_flicker_amount = fRange;
            }
        }
        else
        {
            disable_flicker();
        }
    }

    void light_point2d::update_light( int delta )
    {
        if( light_flickers )
        {
            light_flicker_time+=delta;
            if( light_flicker_time >= light_flicker_time_goal )
            {
                light_flicker_amount = semath::random( -light_flicker_range, light_flicker_range );
                light_flicker_time = 0;
            }
        }
    }
}
