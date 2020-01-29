/*
GPE_Animation.h
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

#ifndef GPE_ANIMATION_H
#define GPE_ANIMATION_H

#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "GPE_Functions.h"
#include "GPE_Collisions.h"
#include "GPE_String_Ex.h"
#include "GPE_Globals.h"
#include "GPE_Branch.h"
#include "GPE_Render_Package.h"

class GPE_Animation : public GPE_Branch
{
private:
    bool usingCustomTexture;
    GPE_Texture_Base * animationTexture;
    std::string name;
    std::string fileName;
    //GPE_Texture_Base *animationFlipSurface;
    //GPE_Texture_Base *animationMirFlipSurface;
    int width, height, xoffset, yoffset, frameCount;
    int hPadding, vPadding;
    std::vector <GPE_Rect * > animFrames;
    int xMiddle, yMiddle;
public:
    int animId;
    int colRadius;
    GPE_Rect * colBox;
    GPE_Animation( GPE_Render_Package * renderPackage, std::string animName= "", std::string animFileName = "", bool imgTransparent=false);
    ~GPE_Animation();
    void change_name(std::string animName);
    void change_texture(GPE_Texture_Base * newTexture);
    bool copy_image_source(std::string outDirectoryName);
    GPE_Animation * copy_self();
    void clean_up();
    void edit_collision_box(int cx, int cy, int cw, int ch);
    void edit_collision_circle(int cx, int cy, int cr);
    int get_frame_count();
    int get_width();
    int get_height();
    int get_hpadding();
    int get_vpadding();
    int get_texture_width();
    int get_texture_height();
    int get_frame_xoffset();
    int get_frame_yoffset();

    std::string get_file_name();
    std::string get_name();
    bool has_texture();
    void load_image( GPE_Render_Package * renderPackage,std::string animFileName, bool transparent = true );
    void render(int subImageToDraw, int xPos, int yPos,  GPE_Rect * cam = NULL);
    void render_colored(int subImageToDraw, int xPos, int yPos, GPE_Color * renderColor, int alpha = 255, GPE_Rect * cam = NULL);
    void render_piece( int xPos, int yPos, GPE_Rect * rectPiece = NULL,GPE_Rect * cam = NULL);
    void render_piece_resized( int xPos, int yPos, int newWidth, int newHeight,GPE_Rect * rectPiece = NULL,GPE_Rect * cam = NULL);
    void render_resized(int subImageToDraw, int xPos, int yPos, float newWidth, float newHeight, GPE_Rect * cam = NULL);
    void render_rotated(int subImageToDraw, int xPos, int yPos, float newAngle, float xScale, float yScale, GPE_Rect * cam= NULL);
    void render_scaled(int subImageToDraw, int xPos, int yPos, float xScale, float yScale,GPE_Rect * cam = NULL);
    void render_special(int subImageToDraw, int xPos, int yPos, float xScale, float yScale,float newAngle = 0,GPE_Color * renderColor = NULL, int alpha = 255, GPE_Rect * cam= NULL);
    void reset_frames();
    void setup_animation( int fCount, int aw, int ah, int sofx, int sofy, int hPad, int vPad );
    void setup_fullimg_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad );
};

#endif //GPE_ANIMATION_H
