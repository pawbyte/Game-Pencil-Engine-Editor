/*
GPE_Backgrounds.h
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

#ifndef GPE_BACKGROUNDS_H
#define GPE_BACKGROUNDS_H

#include "GPE_Shared_Resources.h"

class GPE_Background: public GPE_Branch
{
private:
    GPE_Texture_Base * backgroundTexture;
    float bgXOffset;
    float bgYOffset;
    float bgXSpeed;
    float bgYSpeed;
    int bgId;
    GPE_Color * bgColor;
    bool renderBgSpeed;
    bool loopBGHori;
    bool loopBGVert;
    bool stretchBG;
public:
    GPE_Background();
    ~GPE_Background();
    int get_background_id();
    GPE_Color * get_scene_background_color();
    void render();
    void render_bg_speed( int xx, int yy, GPE_Rect *  cam, GPE_Rect * renderCam);
    void render_bg( GPE_Rect * renderToArea, GPE_Rect *  cam, GPE_Rect * renderCam, int lvlW, int lvlH);
    void scroll_bg();
    void set_bgspeed( float speedX, float speedY );
    void set_bgoffsets( float nextX, float nextY );
    void set_background( int newBG);
    void set_background_color(GPE_Color * colorIn);
    void update();
};

#endif
