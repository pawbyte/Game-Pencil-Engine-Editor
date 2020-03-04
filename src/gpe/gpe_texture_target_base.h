/*
texture_target_base.h
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

#ifndef gpe_texture_target_base_h
#define gpe_texture_target_base_h

#include "gpe_texture_base.h"

namespace gpe
{
    class texture_target_base: public texture_base
    {
        public:
            texture_target_base();
            virtual ~texture_target_base();
            texture_base * create_new();
            virtual void render_overlay(  renderer_base * renderer,int x, int y, gpe::shape_rect* clip = NULL, int alpha = 255  );
            virtual void render_overlay_scaled(  renderer_base * renderer,int x, int y,float xScale, float yScale, gpe::shape_rect* clip = NULL, color * rendColor = NULL, int alpha = 255 );

            virtual void resize_target( renderer_base * renderer,int w, int h, int id = -1,bool useLinearScaling = true  );
    };
}

#endif //gpe_texture_target_base_h
