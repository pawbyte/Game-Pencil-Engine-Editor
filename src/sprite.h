/*
sprite.h
This file is part of:
GAME PENCI ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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
#include "functions.h"
#include "collision_checks.h"
#include "string_ex.h"
#include "GPE_Globals.h"
#include "GPE_Texture.h"

class GPE_Sprite
{
    public:
        GPE_Texture *spriteTexture;
        std::string name;
        int spriteId;
        std::string fileName;
        //GPE_Texture *spriteFlipSurface;
        //GPE_Texture *spriteMirFlipSurface;
        int width, height, xoffset, yoffset, framesForSprite;
        bool isMirrored;
        std::vector <SDL_Rect> spriteImages;
        SDL_Rect * boundingBox;
        SDL_Rect * colBox;
        GPE_Sprite();
        GPE_Sprite(std::string fileName, bool imgTransparent=false);
        ~GPE_Sprite();
        GPE_Sprite * copy_self();
        void render(GPE_Renderer * cRenderer,int subImageToDraw, int xPos, int yPos,  GPE_Texture *destination = NULL, SDL_Rect *cam = NULL);
        void render_resized(GPE_Renderer * cRenderer,int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Texture *destination = NULL, SDL_Rect *cam = NULL);
        void clean_up();
};

void render_sprite(GPE_Renderer * cRenderer,GPE_Sprite* spriteToDraw, int subImageToDraw, int xPos, int yPos, GPE_Texture *destination= NULL, SDL_Rect * cam= NULL);
void render_sprite_resized(GPE_Renderer * cRenderer,GPE_Sprite* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Texture *destination= NULL, SDL_Rect *cam = NULL);
void render_sprite_ext(GPE_Renderer * cRenderer,GPE_Sprite* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Color * renderColor,GPE_Texture *destination= NULL, SDL_Rect *cam= NULL);
void render_sprite_rotated(GPE_Renderer * cRenderer,GPE_Sprite* spriteToDraw, int subImageToDraw, int xPos, int yPos, double newAngle, int newWidth, int newHeight, GPE_Texture *destination= NULL, SDL_Rect *cam= NULL);
int sprite_get_number(GPE_Sprite* spriteIn);

#endif
