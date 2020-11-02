/*
game_scene_basic_class.h
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


#ifndef GPE_SCENE_BASIC_CLASS_H
#define GPE_SCENE_BASIC_CLASS_H

#include "gpe_basic_resource_page.h"
#include "gpe_dock_system.h"
#include "../pawgui/pawgui_stree.h"

//Basic Scene Editing Modes
const int SCENE_MODE_NONE = -1;
const int SCENE_MODE_SELECT = 1;
const int SCENE_MODE_PLACE = 2;
const int SCENE_MODE_MOVE = 3;
const int SCENE_MODE_ROTATION = 4;
const int SCENE_MODE_SCALE = 5;
const int SCENE_MODE_ERASE = 6;
const int SCENE_MODE_ASSIGN = 7;

class GPE_SceneBasicClass: public pawgui::widget_branch
{
protected:
    bool dualScaleClass;
    pawgui::widget_resource_container * projectParentFolder;
public:
    GPE_SceneBasicClass * parentSceneBranch;
    bool isBeingMoved;
    int layerParentId;
    float xPos, yPos;
    float xScale, yScale;
    float angle;
    int xPivot, yPivot;
    int width, height;
    pawgui::widget_label_text  * branchGlobalId;
    pawgui::widget_input_number * angleField;
    pawgui::widget_input_number * xPosField;
    pawgui::widget_input_number * yPosField;
    pawgui::widget_input_number * xScaleField;
    pawgui::widget_input_number * yScaleField;
    pawgui::widget_input_text * branchNameField;
    pawgui::widget_input_text * branchTagField;
    pawgui::gpe_widget_color_picker * branchColor;
    pawgui::widget_slide_xaxis * branchAlpha;
    std::vector< GPE_ObjectComponent * >objCustomValPairs;
    GPE_SceneBasicClass();
    virtual ~GPE_SceneBasicClass();
    void add_basic_elements();
    bool add_scene_branch( GPE_SceneBasicClass * branch, bool changeId = true, bool openBranch = false );
    virtual void add_typed_elements();
    GPE_ObjectComponent *  add_variable(pawgui::widget_basic * newVariable);
    virtual bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  pawgui::widget_resource_container * localResTypeController );
    virtual void calculate_size();
    virtual void process_elements();
    void refresh_branch();
    void reset_components();
    float rotx( float rx, float ry);
    float roty( float rx, float ry);
    virtual void render_branch();
    bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);
    void set_angle( float newAngle );
    void set_name( std::string new_name );
    void set_tag ( std::string newTag );
    void set_position( float x, float y);
    bool under_mouse(float mx, float my);
};

#endif //GPE_SCENE_BASIC_CLASS_H
