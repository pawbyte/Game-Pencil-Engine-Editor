/*
gpe_scene_helper_class.h
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

#ifndef GPE_SCENE_HELPER_CLASS_H
#define GPE_SCENE_HELPER_CLASS_H

#include "gpe_scene_basic_class.h"

class gameScenePopupCategories
{
public:
    std::string name;
    GPE_Label_Title * categoryLabel;
    std::vector <GPE_VerticalCardButton * >elements;
    gameScenePopupCategories( std::string cName );
    ~gameScenePopupCategories();
    GPE_VerticalCardButton * add_button(  std::string name, int id, std::string imgLocation, std::string parsedLines  );
    void add_if_available( GPE_GuiElementList *  cList, std::string str);
};

class GPE_SceneEditorHelper: public GPE_GeneralGuiElement
{
private:
    bool boxIsMoving;
    bool boxWasResized;
    bool boxBeingResized;
    GPE_GuiElementList * topList;
    GPE_GuiElementList * middleList;
    GPE_GuiElementList * bottomList;
    GPE_ToolLabelButton * confirmButton;
    GPE_ToolLabelButton * cancelButton;
    GPE_Label_Text * currentLabel;
    GPE_Label_Paragraph * descriptionLabel;
    gpe_text_widget_string * searchField;
    gpe_text_widget_string * nameField;

    //Do not remove except for destructor
    std::vector <gameScenePopupCategories * > popupCategories;
public:
    int editMode;
    gpe::animaton2d * eraserAnimation;
    GPE_DropDown_Menu * layerListsDropDown;
    GPE_GeneralResourceContainer * cSceneAnimtList;
    GPE_GeneralResourceContainer * cSceneObjList;
    GPE_GeneralResourceContainer * cSceneTexList;
    GPE_GeneralResourceContainer * cSceneTstList;
    int cameraFloorXPos, cameraFloorYPos;
    bool mouseInScene;
    float mouseXPos, mouseYPos;
    gpe::shape_rect * currentCamera;
    gpe::shape_rect * tempRect;
    int sWidth, sHeight;
    float zoomValue;
    std::string chosenName;
    gpe::texture_base * highlightRect;
    GPE_SceneEditorHelper();
    ~GPE_SceneEditorHelper();
    gameScenePopupCategories * add_category( std::string name );
    int get_new_resource(std::string title );
    void reset_meta();
};

extern GPE_SceneEditorHelper * spm;

#endif //GPE_SCENE_HELPER_CLASS_H
