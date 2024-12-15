/*
gpe_scene_helper_class.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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
    pawgui::widget_label_title * categoryLabel;
    std::vector <pawgui::widget_button_card_vertical * >elements;
    gameScenePopupCategories( std::string cName );
    ~gameScenePopupCategories();
    pawgui::widget_button_card_vertical * add_button(  std::string name, int id, std::string imgLocation, std::string parsedLines  );
    void add_if_available( pawgui::widget_panel_list *  cList, std::string str);
};

class GPE_SceneEditorHelper: public pawgui::widget_basic
{
private:
    bool boxIsMoving;
    bool boxWasResized;
    bool boxbeing_resized;
    pawgui::widget_panel_list * topList;
    pawgui::widget_panel_list * middleList;
    pawgui::widget_panel_list * bottomList;
    pawgui::widget_button_label * confirm_button;
    pawgui::widget_button_label * cancel_button;
    pawgui::widget_label_text  * currentLabel;
    pawgui::widget_label_paragraph * descriptionLabel;
    pawgui::widget_input_text * searchField;
    pawgui::widget_input_text * nameField;

    //Do not remove except for destructor
    std::vector <gameScenePopupCategories * > popupCategories;
public:
    int editMode;
    gpe::animation2d * eraserAnimation;
    pawgui::widget_dropdown_menu * layerListsDropDown;
    pawgui::widget_resource_container * cSceneAnimtList;
    pawgui::widget_resource_container * cSceneObjList;
    pawgui::widget_resource_container * cSceneTexList;
    pawgui::widget_resource_container * cSceneTstList;
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
