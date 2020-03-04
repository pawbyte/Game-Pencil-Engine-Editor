/*
gpe_camera.cpp
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

#include "gpe_camera.h"
#include "gpe_constants.h"

namespace gpe
{
    //The camera
    shape_rect * camera_default = NULL;
    shape_rect * camera_current = NULL;

    shape_rect * camera_find( shape_rect * rectIn)
    {
        if(rectIn==NULL)
        {
            return camera_default;
        }
        return rectIn;
    }

    void camera_reset()
    {
        if( camera_default != NULL )
        {
            camera_default->x = 0;
            camera_default->y = 0;
            camera_default->w = screen_width;
            camera_default->h = screen_height;
        }
    }


    scene_camera::scene_camera( float cameraX, float cameraY, float cameraW, float cameraH, int cameraIdNumber )
    {
        cameraRect = new gpe::shape_rect();
        cameraBoundaries = new gpe::shape_rect();
        cameraRect->update_shape(cameraX, cameraY, cameraW, cameraH);

        renderRect = new gpe::shape_rect();
        scaledRect = new gpe::shape_rect();
        scaledRect->update_shape(cameraX, cameraY, cameraW, cameraH);
        viewTarget = is_null;
        horiBorder = is_null;
        vertBorder = is_null;

        hSpeed = 0;
        vSpeed = 0;

        isVisible = true;

        maxXTilesInView = 0;
        maxYTilesInView = 0;

        cSceneWidth = 0;
        cSceneHeight = 0;
    }

    scene_camera::~scene_camera()
    {
        if( cameraRect!=NULL )
        {
            delete cameraRect;
            cameraRect = NULL;
        }

        if( cameraBoundaries!=NULL )
        {
            delete cameraBoundaries;
            cameraBoundaries = NULL;
        }

        if( renderRect!=NULL )
        {
            delete renderRect;
            renderRect = NULL;
        }

        if( scaledRect!=NULL )
        {
            delete scaledRect;
            scaledRect = NULL;
        }
    }

    void scene_camera::center_camera ( float centerX, float centerY)
    {
        cameraRect->update_position(centerX-cameraRect->get_width()/2,centerY-cameraRect->get_height()/2);
        if( cameraRect->get_x2() > cameraBoundaries->get_width() )
        {
            cameraRect->set_x(cameraBoundaries->get_width()-cameraRect->get_width() );
        }
        if( cameraRect->get_y2() > cameraBoundaries->get_height() )
        {
            cameraRect->set_y( cameraBoundaries->get_height()-cameraRect->get_height() );
        }

        if( cameraRect->get_x() < 0 )
        {
            cameraRect->set_x(0);
            //console.log("Setting cam X to 0 for protection...");
        }
        if( cameraRect->get_y() < 0)
        {
            cameraRect->set_y(0);
            //console.log("Setting cam Y to 0 for protection...");

        }
        //console.log( "New Camera Pos:	("+cameraRect.get_x()+" , "+cameraRect.get_y()+")" );

    }

    bool scene_camera::is_visible()
    {
        return isVisible;
    }

    void scene_camera::move_hori ( float movSpace )
    {
        if (movSpace < 0 )
        {
            if( cameraBoundaries->get_x() < cameraRect->get_x() +movSpace )
            {
                cameraRect->add_x( movSpace);
            }
            else
            {
                cameraRect->set_x(0);
            }
        }
        else
        {
            if( cameraBoundaries->get_x2() > cameraRect->get_x2() +movSpace )
            {
                cameraRect->add_x( movSpace);
            }
        }
    }

    void scene_camera::move_vert ( float movSpace )
    {
        if (movSpace < 0 )
        {
            if( cameraBoundaries->get_y() < cameraRect->get_y() +movSpace )
            {
                cameraRect->add_y( movSpace);
            }
            else
            {
                cameraRect->set_y(0);
            }
        }
        else
        {
            if( cameraBoundaries->get_y2() > cameraRect->get_y2() +movSpace )
            {
                cameraRect->add_y( movSpace);
            }
        }
    }

    void scene_camera::set_visible( bool visiblility )
    {
        isVisible = visiblility;
    }

    void scene_camera::setup_camera (float cameraX,float cameraY, float cameraW, float cameraH)
    {
        isVisible = true;
        //console.log("Updaing Camera with ("+cameraX+", "+cameraY+", "+ cameraW+", "+cameraH+") dimensions...");
        renderRect->update_shape(cameraX, cameraY, cameraW, cameraH);
        cameraRect->update_shape(cameraX, cameraY, cameraW, cameraH);
    }

    void scene_camera::update_tile_size (int newTX, int newTY)
    {
        maxXTilesInView = newTX;
        maxYTilesInView = newTY;
    }

    void scene_camera::update_screen_space ( float cameraX, float cameraY, float cameraW, float cameraH)
    {
        renderRect->update_shape(cameraX, cameraY, cameraW, cameraH);
    }

    void scene_camera::update_view_space ( float cameraX, float cameraY, float cameraW, float cameraH)
    {
        cameraRect->update_shape(cameraX, cameraY, cameraW, cameraH);
    }
}
