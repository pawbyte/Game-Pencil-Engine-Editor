/*
GPE_Camera.h
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

#ifndef GPE_CAMERA_H
#define GPE_CAMERA_H

#include "GPE_CIncludes.h"
#include "GPE_Shapes.h"

const int GPE_MAX_CAMERAS = 8;

class GPE_SceneCamera
{
private:
    GPE_Rect * cameraRect;
    GPE_Rect * cameraBoundaries;

    GPE_Rect * renderRect;
    GPE_Rect * scaledRect;
    int viewTarget;
    int horiBorder;
    int vertBorder;

    double hSpeed;
    double vSpeed;

    bool isVisible;

    int maxXTilesInView;
    int maxYTilesInView;

    int cSceneWidth;
    int cSceneHeight;
public:
    GPE_SceneCamera(double cameraX, double cameraY, double cameraW, double cameraH, int cameraIdNumber );
    ~GPE_SceneCamera();
    void center_camera ( double centerX, double centerY);
    void move_hori ( double movSpace );
    void move_vert ( double movSpace );
    void set_visible( bool visiblility );
    void setup_camera (double cameraX,double cameraY, double cameraW, double cameraH);
    void update_tile_size (int newTX, int newTY);
    void update_screen_space ( double cameraX, double cameraY, double cameraW, double cameraH);
    void update_view_space ( double cameraX, double cameraY, double cameraW, double cameraH);
};

#endif
