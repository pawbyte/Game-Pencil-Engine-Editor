/*
gpe_background.h
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

#ifndef gpe_background_h
#define gpe_background_h

#include "gpe_shared_resources.h"

namespace gpe
{
    class background: public branch
    {
    private:
        texture_base * backgroundTexture;
        float bgXOffset;
        float bgYOffset;
        float bgXSpeed;
        float bgYSpeed;
        int bgId;
        color * bgColor;
        bool renderBgSpeed;
        bool loopBGHori;
        bool loopBGVert;
        bool stretchBG;
    public:
        background();
        ~background();
        int get_background_id();
        color * get_scene_background_color();
        void render();
        void render_bg_speed( int xx, int yy, shape_rect *  cam, shape_rect * renderCam);
        void render_bg( shape_rect * renderToArea, shape_rect *  cam, shape_rect * renderCam, int lvlW, int lvlH);
        void scroll_bg();
        void set_bgspeed( float speedX, float speedY );
        void set_bgoffsets( float nextX, float nextY );
        void set_background( int newBG);
        void set_background_color( color * colorIn);
        void update();
    };
}

#endif //gpe_background_h
