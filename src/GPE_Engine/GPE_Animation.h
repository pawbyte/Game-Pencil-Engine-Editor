/*
GPE_Animation.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "GPE_Functions.h"
#include "GPE_Collisions.h"
#include "GPE_String_Ex.h"
#include "GPE_Globals.h"
#include "GPE_Texture.h"

class GPE_Animation
{
    public:
        GPE_Texture * animationTexture;
        std::string name;
        int spriteId;
        std::string fileName;
        //GPE_Texture *spriteFlipSurface;
        //GPE_Texture *spriteMirFlipSurface;
        int width, height, xoffset, yoffset, framesForSprite;
        bool isMirrored;
        std::vector <GPE_Rect> spriteImages;
        GPE_Rect * boundingBox;
        GPE_Rect * colBox;
        GPE_Animation();
        GPE_Animation(std::string fileName, bool imgTransparent=false);
        ~GPE_Animation();
        GPE_Animation * copy_self();
        void render(int subImageToDraw, int xPos, int yPos, GPE_Rect * cam = NULL);
        void render_resized(int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Rect * cam = NULL);
        void clean_up();
};

void render_animation(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, GPE_Rect * cam= NULL);
void render_animation_resized(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, GPE_Rect * cam = NULL);
void render_animation_ext(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Color * renderColor, GPE_Rect * cam= NULL);
void render_animation_rotated(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, double newAngle, int newWidth, int newHeight, GPE_Rect * cam= NULL);
int sprite_get_number(GPE_Animation* spriteIn);

#endif
