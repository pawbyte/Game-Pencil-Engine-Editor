/*
gpe_gamepad_tester.h
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

#ifndef gpe_gamepad_tester_h
#define gpe_gamepad_tester_h

#include "../resources/basic/gpe_basic_resource_page.h"


class gamePencilgamepadTesterResource: public pawgui::general_resource
{
public:
    int controllerInView;
    pawgui::widget_button_push * confirmResource_button;
    pawgui::widget_button_push * cancelResource_button;
    pawgui::widget_selectbox * sideAreaPanel;
    gpe::shape_rect * sidePanelRect;
    pawgui::widget_checkbox * requireInputToConnect;
    pawgui::widget_checkbox * renderSceneBGColor;
    pawgui::widget_selectbox * subEditorSelectBox;

    pawgui::widget_checkbox * showControllerInfo;
    pawgui::widget_label_title * playerStats;
    int playerRadius[gp_max_devices];
    float playerDeadZone[gp_max_devices];

    pawgui::widget_panel_section * playerDeadZoneSection[gp_max_devices];
    pawgui::widget_input_number * playerDeadZoneField[gp_max_devices];
    pawgui::widget_button_push * playerAutoDeadZone[gp_max_devices];

    pawgui::widget_input_number * playerSizeRadius[gp_max_devices];

    pawgui::widget_panel_section * playerColorsSection[gp_max_devices];
    pawgui::gpe_widget_color_picker * playerTestColor[gp_max_devices];
    pawgui::gpe_widget_color_picker * playerFontColor[gp_max_devices];

    pawgui::widget_button_push * detectControllers_button;
    pawgui::widget_button_push * vibrateController_button;
    pawgui::widget_button_icon * previousController_button;
    pawgui::widget_button_icon * nextController_button;
    pawgui::widget_input_number * conrollerIdField;
    pawgui::widget_label_title * controllerViewTitleLabel;
    pawgui::widget_label_text  * controllerNameLabel;

    int playerRadiusMin, playerRadiusMax;
    int playerX[gp_max_devices];
    int playerY[gp_max_devices];
    //Editor Related variables
    gpe::shape_rect subViewedSpace;
    pawgui::widget_panel_list * editorPageList;
    std::string projectFolderListLocation;
    gamePencilgamepadTesterResource();
    ~gamePencilgamepadTesterResource();
    bool include_local_files( std::string pBuildDir , int buildType );

    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gamePencilgamepadTesterResource * main_gamepad_tester;
#endif // gpe_gamepad_tester_h
