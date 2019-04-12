/*
game_scene_basic_class.h
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


#ifndef GPE_SCENE_BASIC_CLASS_H
#define GPE_SCENE_BASIC_CLASS_H

#include "gpe_basic_resource_page.h"
#include "gpe_dock_system.h"
#include "AOSGUI/paw_gui_stree.h"

//Basic Scene Types
const int BRANCH_TYPE_BASIC_SCENE_ELEMENT = 0;
const int BRANCH_TYPE_LAYER = 1;
const int BRANCH_TYPE_GROUP = 2;
const int BRANCH_TYPE_TILEMAP = 3;
const int BRANCH_TYPE_TILE = 4;
const int BRANCH_TYPE_BACKGROUND = 5;
const int BRANCH_TYPE_OBJECT = 6;

const int BRANCH_TYPE_ANIMATION = 7;
const int BRANCH_TYPE_TEXT = 8;
const int BRANCH_TYPE_STEXT = 9;
const int BRANCH_TYPE_PARTIClE_EMITTER = 10;
const int BRANCH_TYPE_LIGHT = 11;

//Basic Scene Editing Modes
const int SCENE_MODE_NONE = -1;
const int SCENE_MODE_SELECT = 1;
const int SCENE_MODE_PLACE = 2;
const int SCENE_MODE_MOVE = 3;
const int SCENE_MODE_ROTATION = 4;
const int SCENE_MODE_SCALE = 5;
const int SCENE_MODE_ERASE = 6;

class GPE_SceneBasicClass: public GPE_SpecialMenu_Branch
{
protected:
    bool dualScaleClass;
    GPE_GeneralResourceContainer * projectParentFolder;

public:
    GPE_SceneBasicClass * parentSceneBranch;
    bool isBeingMoved;
    int layerParentId;
    double xPos, yPos;
    double xScale, yScale;
    double angle;
    int xPivot, yPivot;
    int width, height;
    GPE_TextInputNumber * angleField;
    GPE_TextInputNumber * xPosField;
    GPE_TextInputNumber * yPosField;
    GPE_TextInputNumber * xScaleField;
    GPE_TextInputNumber * yScaleField;
    GPE_TextInputBasic * branchNameField;
    GPE_Input_Field_Color * branchColor;
    GPE_Slider_XAxis * branchAlpha;
    std::vector< GPE_ObjectComponent * >objCustomValPairs;
    GPE_SceneBasicClass();
    ~GPE_SceneBasicClass();
    void add_basic_elements();
    bool add_scene_branch( GPE_SceneBasicClass * branch, bool changeId = true, bool openBranch = false );
    virtual void add_typed_elements();
    GPE_ObjectComponent *  add_variable(GPE_GeneralGuiElement * newVariable);
    virtual bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  GPE_GeneralResourceContainer * localResTypeController );
    virtual void calculate_size();
    void reset_components();
    double rotx( double rx, double ry);
    double roty( double rx, double ry);
    virtual void render_branch();
    virtual void process_elements();
    bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);
    void set_angle( double newAngle );
    void set_name( std::string newName );
    void set_position( double x, double y);
    bool under_mouse(double mx, double my);
};

#endif //GPE_SCENE_BASIC_CLASS_H
