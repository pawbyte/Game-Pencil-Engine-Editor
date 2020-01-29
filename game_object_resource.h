/*
game_object_resource.h
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

#ifndef GAME_OBJECT_RESOURCE_H
#define GAME_OBJECT_RESOURCE_H

#include "gpe_basic_resource_page.h"
#include "gpe_editor_settings.h"
#include "gpe_js_compiler_settings.h"

const int objectFunctionTypes = 5; //normal, timed, collisions, user added, special funcions

class objectResParentChainLink
{
public:
    int chainObjectType;
    objectResParentChainLink( int origObjectType);
    ~objectResParentChainLink();
    std::vector< int> heldChains;
    void grab_chain( objectResParentChainLink * chainIn = NULL);
    bool is_inchain(int chainId);
};


const int OBJ_MODE_COMPONENTS = 0;
const int OBJ_MODE_HEADER = 1;
const int OBJ_MODE_SOURCE = 2;

class gameObjectResource: public standardEditableGameResource
{
public:
    GPE_Label_Title * resourceNameLabel;

    GPE_SelectBoxBasic * objModeSelector;
    GPE_CheckBoxBasic * checkBoxNeedsCamera;
    GPE_CheckBoxBasic * checkBoxIsMoveable;
    GPE_CheckBoxBasic * checkBoxIsVisible;
    GPE_CheckBoxBasic * checkBoxIsContinuous;
    GPE_TextAreaInputBasic * headerCodeArea;
    GPE_TextAreaInputBasic * sourceCodeArea;
    int animationIndex;
    int imageIndex;
    int selectedMode;
    int parentObjectId;
    GPE_Animation * animInEditor;
    GPE_DropDown_Resouce_Menu * animationField;
    GPE_DropDown_Resouce_Menu * parentObjectField;

    //Component Related Variables
    GPE_GuiElementList * customComponentsGuiList;
    GPE_Label_Text * componentsMainGuiLabel;
    GPE_Label_Text * componentsGuiLabel;
    GPE_ToolPushButton * inheritParentComponentButton;
    GPE_ToolPushButton * resetComponentsButton;
    GPE_DropDown_Menu * addNewComponentDropDown;
    GPE_ToolIconButton * removeComponentButton;
    GPE_ToolIconButton * editCompnentButton;
    GPE_ToolIconButton * componentSettingsButton;

    gameObjectResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~gameObjectResource();
    void add_component(std::string newComponentData);
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    GPE_GeneralResourceContainer * get_parent_resource();
    GPE_Animation * get_resource_animation();
    bool include_local_files( std::string pBuildDir , int buildType );
    void integrate_into_syntax();
    void manage_components(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    void prerender_self( );
    void preprocess_self(std::string alternatePath = "");
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);

    void save_resource(std::string alternatePath = "", int backupId = -1);
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

void setup_object_components( GPE_DropDown_Menu * componentMenu);
#endif
