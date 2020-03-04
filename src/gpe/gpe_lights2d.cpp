/*
gpe_lights2d.cpp
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

#include "gpe_artist_base.h"
#include "gpe_lights2d.h"
#include "../other_libs/semath.h"

namespace gpe
{
    light_basic_2d::light_basic_2d()
    {
        lightX = 0;
        lightY = 0;
        lightIntensity = 255;
        lightIsActive = true;
        lightActiveOnStart = true;
        lightColor = new color("Light",255,255 );
    }

    light_basic_2d::~light_basic_2d()
    {
        if( lightColor!=NULL )
        {
            delete lightColor;
            lightColor = NULL;
        }
    }

    void light_basic_2d::render_light( float scale, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find( cam );
        gpe::gcanvas->render_rectangle(0,0, cam->w, cam->h, lightColor, false, lightIntensity );
    }

    void light_basic_2d::render_light_at( float x, float y, float scale, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find( cam );
        gpe::gcanvas->render_rect( cam, lightColor, false, lightIntensity );
    }

    void light_basic_2d::setup_light( int x, int y, int intensity, bool activeNow, bool activeOnLoad )
    {
        lightX = x;
        lightY = y;
        lightIntensity = intensity;
        lightIsActive = activeNow;
        lightActiveOnStart = activeOnLoad;
    }

    light_direction2d::light_direction2d()
    {
        lightX2 = lightY2 = 0;
        lightWidth = 64;
        lightLength = 128;
        lightDirection = 0;
        lightTexture = NULL;
    }

    light_direction2d::~light_direction2d()
    {

    }

    float light_direction2d::get_direction()
    {
        return lightDirection;
    }

    void light_direction2d::render_light( float scale, gpe::shape_rect * cam )
    {
        render_light_at(lightX, lightY, scale, cam );
    }

    void light_direction2d::render_light_at( float x, float y, float scale, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find( cam );
        if( lightTexture!=NULL )
        {
            lightTexture->render_tex_rotated( x - cam->x, y - cam->y,lightDirection, lightColor, NULL, lightIntensity );
        }
        else
        {
            lightX2 = x + semath::lengthdir_x( lightLength*scale,lightDirection ) - cam->x;
            lightY2 = y + semath::lengthdir_y( lightLength*scale,lightDirection ) - cam->y;
            //gpe::gcanvas->render_semi_circle(x,y,lightWidth,lightDirection, false );
            gpe::gcanvas->render_line_width_color(x- cam->x, y - cam->y, lightX2, lightY2, lightWidth, lightColor, lightIntensity );
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
        lightDirection = newDir;
    }

    light_point2d::light_point2d()
    {
        lightRadius = 32;
        secondRadius = 16;
        thirdRadius = 0;
        useFlicker = true;
        currentFlicker = 0;
        lightFlickerTime = 0;
        lightFlickerTimeGoal = 250;
        lightFlickerRange = 2;
        secondRadiusIntensity = 255;
        thirdRadiusIntensity = 0;
    }

    light_point2d::~light_point2d()
    {

    }

    void light_point2d::disable_flicker()
    {
        useFlicker = false;
        lightFlickerTime = 0;
        currentFlicker = 0;
    }

    void light_point2d::enable_flicker()
    {
        useFlicker = true;
    }

    void light_point2d::render_light( float scale, gpe::shape_rect * cam )
    {
        render_light_at(lightX, lightY, scale, cam );
    }

    void light_point2d::render_light_at( float x, float y, float scale, gpe::shape_rect * cam )
    {
        cam = gpe::camera_find( cam );
        gpe::gcanvas->render_circle_color( x, y, (lightRadius+currentFlicker)*scale,lightColor, lightIntensity );
        if( secondRadius > 0 )
        {
            gpe::gcanvas->render_circle_color( x, y, (secondRadius+currentFlicker)*scale, lightColor, secondRadiusIntensity );
        }
        if( thirdRadius > 0 )
        {
            gpe::gcanvas->render_circle_color( x, y, (thirdRadius+currentFlicker)*scale, lightColor, thirdRadiusIntensity  );
        }
    }

    void light_point2d::setup_flicker( int fTime, int fRange )
    {
        if( fTime > 0 && fRange > 0 )
        {
            enable_flicker();
            lightFlickerTimeGoal = fTime;
            lightFlickerRange = fRange;
            if( currentFlicker < -fRange)
            {
                currentFlicker = -fRange;
            }
            else if( currentFlicker > fRange )
            {
                currentFlicker = fRange;
            }
        }
        else
        {
            disable_flicker();
        }
    }

    void light_point2d::update_light( int delta )
    {
        if( useFlicker )
        {
            lightFlickerTime+=delta;
            if( lightFlickerTime >= lightFlickerTimeGoal )
            {
                currentFlicker = semath::random( -lightFlickerRange, lightFlickerRange );
                lightFlickerTime = 0;
            }
        }
    }
}
