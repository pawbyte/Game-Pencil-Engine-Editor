/*
gpe_scene_helper_class.h
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
    GPE_TextInputBasic * searchField;
    GPE_TextInputBasic * nameField;

    //Do not remove except for destructor
    std::vector <gameScenePopupCategories * > popupCategories;
public:
    int editMode;
    GPE_DropDown_Menu * layerListsDropDown;
    GPE_GeneralResourceContainer * cSceneAnimtList;
    GPE_GeneralResourceContainer * cSceneObjList;
    GPE_GeneralResourceContainer * cSceneTexList;
    GPE_GeneralResourceContainer * cSceneTstList;
    int cameraFloorXPos, cameraFloorYPos;
    bool mouseInScene;
    double mouseXPos, mouseYPos;
    GPE_Rect * currentCamera;
    GPE_Rect * tempRect;
    int sWidth, sHeight;
    double zoomValue;
    std::string chosenName;
    GPE_Texture * highlightRect;
    GPE_Texture * lightCircleTexture;
    GPE_SceneEditorHelper();
    ~GPE_SceneEditorHelper();
    gameScenePopupCategories * add_category( std::string name );
    int get_new_resource(std::string title );
    void reset_meta();
};

extern GPE_SceneEditorHelper * spm;

#endif //GPE_SCENE_HELPER_CLASS_H
