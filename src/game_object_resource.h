/*
game_object_resource.h
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

#ifndef GAME_OBJECT_RESOURCE_H
#define GAME_OBJECT_RESOURCE_H

#include "gpe_basic_resource_page.h"
#include "gpe_editor_settings.h"
#include "gpe_js_compiler_settings.h"


class collisionContainer
{
public:
    int otherObjectType;
    std::string otherObjectName;
    GPE_TextAreaInputBasic * textEditor;
    collisionContainer();
    ~collisionContainer();

};

class customFunctionContainer
{
public:
    std::string functionName;
    std::string functionReturnType;
    std::string functionParameters;
    GPE_TextInputBasic * returnTypeField;
    GPE_TextInputBasic * parametersField;
    GPE_TextAreaInputBasic * textEditor;
    customFunctionContainer();
    ~customFunctionContainer();

};

const int DEFAULT_OBJECT_FUNCTIONS_COUNT = 16;
const int DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT = 10;
const int DEFAULT_SPECIAL_FUNCTIONS_COUNT= 6;


const int objectFunctionTypes = 5; //normal, timed, collisions, user added, special funcions
extern std::string DEFAULT_OBJECT_FUNCTIONS_NAMES[DEFAULT_OBJECT_FUNCTIONS_COUNT];
extern std::string DEFAULT_OBJECT_SPECIALS_NAMES[DEFAULT_SPECIAL_FUNCTIONS_COUNT];

extern std::string DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[DEFAULT_OBJECT_FUNCTIONS_COUNT];
extern std::string DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[DEFAULT_SPECIAL_FUNCTIONS_COUNT];


const int FUNC_ID_PRELOGIC = 2;
const int FUNC_ID_LOGIC = 3;
const int FUNC_ID_ENDLOGIC = 5;


const int FUNC_ID_PRERENDER = 6;
const int FUNC_ID_RENDER = 7;
const int FUNC_ID_POSTRENDER = 8;
const int FUNC_ID_HUD_RENDER = 9;

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



const int OBJ_EDITOR_MODE_COMPONENTS = 0;
const int OBJ_EDITOR_MODE_BASIC = 1;
const int OBJ_EDITOR_MODE_TIMED = 2;
const int OBJ_EDITOR_MODE_COLLISIONS = 3;
const int OBJ_EDITOR_MODE_CUSTOM = 4;
const int OBJ_EDITOR_MODE_SPECIAL = 5;

class gameObjectResource: public standardEditableGameResource
{
public:
    GPE_Label_Title * resourceNameLabel;
    GPE_ToolIconButtonBar * editorButtonBar;
    GPE_SelectBoxBasic * basicFunctionsSelector;
    GPE_SelectBoxBasic * timedFunctionsSelector;
    GPE_SelectBoxBasic * colliderFunctionsSelector;
    GPE_SelectBoxBasic * customFunctionsSelector;
    GPE_SelectBoxBasic * specialFunctionsSelector;
    GPE_CheckBoxBasic * checkBoxNeedsCamera;
    GPE_CheckBoxBasic * checkBoxIsMoveable;
    GPE_CheckBoxBasic * checkBoxIsVisible;
    GPE_CheckBoxBasic * checkBoxIsContinuous;
    GPE_TextAreaInputBasic * classDeclarationBox;
    GPE_TextAreaInputBasic * objectFunctions [DEFAULT_OBJECT_FUNCTIONS_COUNT];
    GPE_TextAreaInputBasic * timedObjectFunctions [DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT];
    GPE_TextAreaInputBasic * specialObjectFunctions [DEFAULT_SPECIAL_FUNCTIONS_COUNT];
    int objectFunctionInEdit[objectFunctionTypes];
    std::vector< collisionContainer * > collisionFunctions;
    std::vector< customFunctionContainer * > customFunctions;
    int spriteIndex;
    int imageIndex;
    int codeSection;
    int parentObjectId;
    GPE_Animation * animInEditor;
    GPE_DropDown_Resouce_Menu * spriteField;
    GPE_DropDown_Resouce_Menu * parentObjectField;
    GPE_DropDown_Resouce_Menu * newObjectToCollideDropDown;
    GPE_DropDown_Resouce_Menu * changeObjectToCollideDropDown;
    GPE_ToolPushButton * removeCollideFunctionButton;
    GPE_ToolIconButton * newCustomObjectFunctionButton;
    GPE_ToolIconButton * newCustomObjectFunctionSettingsButton;
    GPE_ToolIconButton * removeCustomObjectFunctionButton;

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
    collisionContainer *  find_collision_event( int otherObjId);
    customFunctionContainer * find_custom_function( std::string functionNameIn, bool createNewOne = true);
    GPE_TextAreaInputBasic * find_function_textarea(std::string functionNameIn,int functionTypeIn = 0);
    GPE_GeneralResourceContainer * get_parent_resource();
    void integrate_into_syntax();
    void manage_components(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    void prerender_self( );
    void preprocess_self(std::string alternatePath = "");
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void save_resource(std::string alternatePath = "", int backupId = -1);
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
