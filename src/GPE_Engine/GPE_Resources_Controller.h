/*
GPE_Resources_Controller.h
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

#ifndef RESOURCES_CONTROLLER_H
#define RESOURCES_CONTROLLER_H

#include "GPE_CIncludes.h"
#include "GPE_Constants.h"
#include "GPE_Globals.h"
#include "GPE_Texture.h"
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include "GPE_Color_Manager.h"

#include "GPE_Animation.h"
#include "GPE_Audio.h"
#include "GPE_Functions.h"
#include "GPE_String_Ex.h"


const int res_type_count = 20;
extern std::string RESOURCE_TYPE_NAMES[res_type_count];

class generalGameResource
{
protected:
    GPE_Rect encapBox;
    bool isModified;
    int editorMode;
    int requestedMode;
public:
    bool isFullScreenResource;
    int globalResouceIdNumber;
    std::string resourceFileName;
    std::string resourceName;
    std::string parentProjectName;
    std::string parentProjectDirectory;
    int resourceType;
    int subResouceId;
    int exportBuildGlobalId;
    bool justOpenedThisFrame;
    generalGameResource();
    virtual ~generalGameResource() = 0;
    int get_global_rid();
    virtual std::string get_current_name();
    std::string get_name();
    virtual bool is_modified();
    bool matches(generalGameResource * otherResource);
    virtual void integrate_into_syntax();
    virtual void open_code(int lineNumb, int colNumb, std::string codeTitle = "");
    virtual void preprocess_self(std::string alternatePath = "");
    virtual void prerender_self( );
    virtual void process_resource(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    virtual void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_resource(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    virtual void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    virtual void save_resource(std::string alternatePath = "", int backupId = -1);
    virtual void set_global_rid(int newRId);
    virtual void set_name(std::string newName);
    virtual void set_parent_project(std::string newName);
    virtual void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    virtual void update_string(std::string newName);
    virtual bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0 );
    virtual int search_for_string(std::string needle);
    virtual int search_and_replace_string(std::string needle, std::string newStr = "");
    bool write_header_on_file(std::ofstream * fileTarget);

};


//Controls and handles all of the game's resources( Sprites & Backgrounds and maybe sounds and music
class GPE_DataManager
{
private:
    bool randomVariableOn; //random private variable for testing purposes
public:
    std::vector<GPE_Animation*> rAnimations; //vector of every game sprite
    std::vector<GPE_Texture*> rTextures; //vector of every game texture
    std::vector<GPE_Tilesheet*> rTilesheets; //vector of every game tilesheet
    std::vector<GPE_Audio *> rAudio; //vector of every game sound

    std::vector<std::string> nameBackgrounds;
    GPE_DataManager(); // constructor
    ~GPE_DataManager();  //deconstructor
    //adds a sprite with only one row of subimages

    GPE_Animation* sprite_add(std::string spriteFileName,int imgnumb,bool transparent=true,int xorig=0,int yorig=0, bool addMirror=false);
    GPE_Animation* sprite_add_collision(std::string spriteFileName,int imgnumb,bool transparent=true,int xorig=0,int yorig=0, int cx=0, int cy=0, int cw=-1, int ch=-1, bool addMirror=false);
    //GPE_Animation* sprite_add_centered(std::string filename,int imgnumb,bool transparent,int xorig,int yorig, bool addMirror);
    GPE_Animation* sprite_addsheet(std::string spriteFileName, bool transparent, int width, int height);
    GPE_Animation* sprite_addsheet_ext(std::string spriteFileName, bool transparent, unsigned short imgnumb, unsigned short imgPerRow, unsigned short width, unsigned short height, unsigned short cellOffX, unsigned short cellOffY, unsigned short pixelOffX, unsigned short pixOffY, unsigned short hSep, unsigned short vSep);
    GPE_Texture * texture_add(std::string textureFileName);

    //gets the sprite based on it's name
    GPE_Animation * get_sprite(int idIn);
    GPE_Animation * get_sprite(std::string nameIn);
    GPE_Texture * get_texture(std::string textureFileName);
    GPE_Audio * get_audio(int audioId);
    GPE_Audio * get_audio(std::string nameIn);
    void play_audio( int audioId, int loops=-1);
    GPE_Audio * add_audio(std::string soundFileName, int soundTypeIn=0);

    void remove_animation( GPE_Animation * anim );
    void remove_texture( GPE_Texture * tex);
    //File loading
    bool load_files();
    //Clean up
    void clean_up();
};

#endif
