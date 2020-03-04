/*
gpe_lights2d.h
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

#ifndef gpe_lights2d_h
#define gpe_lights2d_h

#include "gpe_artist_base.h"
#include "gpe_branch.h"
#include "gpe_runtime.h"

namespace gpe
{
    const int light_type_ambient2d = 0;
    const int light_type_direction2d = 1;
    const int light_type_point2d = 2;

    class light_basic_2d: public branch
    {
        public:
            float lightX, lightY;
            int lightIntensity;
            color * lightColor;
            bool lightIsActive, lightActiveOnStart;
            light_basic_2d();
            virtual ~light_basic_2d();
            virtual void render_light( float scale = 1, gpe::shape_rect * cam = NULL);
            virtual void render_light_at( float x, float y, float scale = 1, gpe::shape_rect * cam = NULL);
            void setup_light( int x, int y, int intensity, bool activeNow, bool activeOnLoad );
    };

    class light_direction2d: public light_basic_2d
    {
        private:
            float lightDirection;
        public:
            int lightX2, lightY2;
            texture_base * lightTexture;
            int lightWidth;
            int lightLength;
            light_direction2d();
            ~light_direction2d();
            float get_direction();
            void render_light( float scale = 1, gpe::shape_rect * cam = NULL);
            void render_light_at( float x, float y, float scale = 1, gpe::shape_rect * cam = NULL);
            void set_direction( float newDir );
    };

    class light_point2d: public light_basic_2d
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
            light_point2d();
            ~light_point2d();
            void disable_flicker();
            void enable_flicker();
            void render_light( float scale = 1, gpe::shape_rect * cam = NULL);
            void render_light_at( float x, float y, float scale = 1, gpe::shape_rect * cam = NULL);
            void setup_flicker( int fTime, int fRange );
            void update_light( int delta );
    };
}

#endif //gpe_lights2d_h
