/*
GPE_Lights.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef GPE_LIGHTS_H
#define GPE_LIGHTS_H

#include "GPE_Artist_Base.h"
#include "GPE_Engine.h"
#include "GPE_Branch.h"

const int gpe_light_type_ambient = 0;
const int gpe_light_type_direction = 1;
const int gpe_light_type_point = 2;

class GPE_BasicLight: public GPE_Branch
{
public:
    float lightX, lightY;
    int lightIntensity;
    GPE_Color * lightColor;
    bool lightIsActive, lightActiveOnStart;
    GPE_BasicLight();
    ~GPE_BasicLight();
    virtual void render_light( float scale = 1, GPE_Rect * cam = NULL);
    virtual void render_light_at( float x, float y, float scale = 1, GPE_Rect * cam = NULL);
    void setup_light( int x, int y, int intensity, bool activeNow, bool activeOnLoad );
};

class GPE_Directionight: public GPE_BasicLight
{
    private:
        float lightDirection;
public:
    int lightX2, lightY2;
    GPE_Texture_Base * lightTexture;
    int lightWidth;
    int lightLength;
    GPE_Directionight();
    ~GPE_Directionight();
    float get_direction();
    void render_light( float scale = 1, GPE_Rect * cam = NULL);
    void render_light_at( float x, float y, float scale = 1, GPE_Rect * cam = NULL);
    void set_direction( float newDir );
};

class GPE_PointLight: public GPE_BasicLight
{
private:
    bool useFlicker;
    int currentFlicker, lightFlickerRange; // in pixels
    int lightFlickerTime, lightFlickerTimeGoal; // in milliseconds
public:
    int lightRadius;
    int secondRadius;
    int thirdRadius;
    int secondRadiusIntensity;
    int thirdRadiusIntensity;
    GPE_PointLight();
    ~GPE_PointLight();
    void disable_flicker();
    void enable_flicker();
    void render_light( float scale = 1, GPE_Rect * cam = NULL);
    void render_light_at( float x, float y, float scale = 1, GPE_Rect * cam = NULL);
    void setup_flicker( int fTime, int fRange );
    void update_light( int delta );
};

#endif
