/*
GPE_Resources_Controller.h
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

#ifndef RESOURCES_CONTROLLER_H
#define RESOURCES_CONTROLLER_H

#include "GPE_CIncludes.h"
#include "GPE_Constants.h"
#include "GPE_Globals.h"
#include "GPE_Texture_Base.h"
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include "GPE_Color_Manager.h"

#include "GPE_Animation.h"
#include "GPE_Tilesheet.h"
#include "GPE_Audio.h"
#include "GPE_Functions.h"
#include "GPE_String_Ex.h"

extern std::string RESOURCE_TYPE_NAMES[RESOURCE_TYPE_MAX];

//Controls and handles all of the game's resources( animations & Backgrounds and maybe sounds and music
class GPE_DataManager
{
private:
    bool randomVariableOn; //random private variable for testing purposes
    std::string managerName;
    GPE_Render_Package * rRenderPackage;
public:
    std::map<std::string, GPE_Animation*> rAnimations; //map of every game animation
    std::map<std::string, GPE_Texture_Base *> rTextures; //map of every game texture
    std::map<std::string, GPE_Tilesheet*> rTilesheets; //map of every game tilesheet
    std::map<std::string, GPE_Audio *> rAudio; //map of every game sound

    GPE_DataManager(GPE_Render_Package * mRenderPackage, std::string mName); // constructor
    ~GPE_DataManager();  //deconstructor
    //adds a animation with only one row of subimages

    GPE_Animation* animation_add(std::string animationFileName,int imgnumb,bool transparent=true,int xorig=0,int yorig=0, bool addMirror=false);
    GPE_Animation* animation_add_collision(std::string animationFileName,int imgnumb,bool transparent=true,int xorig=0,int yorig=0, int cx=0, int cy=0, int cw=-1, int ch=-1, bool addMirror=false);
    //GPE_Animation* animation_add_centered(std::string filename,int imgnumb,bool transparent,int xorig,int yorig, bool addMirror);
    GPE_Animation* animation_addsheet(std::string animationFileName, bool transparent, int width, int height);
    GPE_Animation* animation_addsheet_ext(std::string animationFileName, bool transparent, unsigned short imgnumb, unsigned short imgPerRow, unsigned short width, unsigned short height, unsigned short cellOffX, unsigned short cellOffY, unsigned short pixelOffX, unsigned short pixOffY, unsigned short hSep, unsigned short vSep);
    GPE_Texture_Base * texture_add(std::string textureFileName);

    //gets the animation based on it's name
    GPE_Animation * get_animation(int idIn);
    GPE_Animation * get_animation(std::string nameIn);

    std::string get_name();

    GPE_Texture_Base * get_texture(std::string textureFileName);

    GPE_Audio * get_audio(int audioId);
    GPE_Audio * get_audio(std::string nameIn);
    void play_audio( int audioId, int loops=-1);
    GPE_Audio * add_audio(std::string soundFileName, int soundTypeIn=0);

    void remove_animation( GPE_Animation * anim );
    void remove_texture( GPE_Texture_Base * tex);
    //File loading
    bool load_files();
    //Clean up
    void clean_up();
};

#endif
