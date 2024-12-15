/*
gpe_editor_aboutpage.h
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

#ifndef GPE_ABOUT_PAGE_RESOURCES_H
#define GPE_ABOUT_PAGE_RESOURCES_H

#include "../resources/basic/gpe_basic_resource_page.h"

class gamePencilAboutPageResource: public pawgui::general_resource
{
public:
    pawgui::widget_selectbox * sideAreaPanel;
    gpe::shape_rect * sidePanelRect;

    //General Help Tab [ begin]
    pawgui::widget_panel_section * helpTabSection;
    pawgui::widget_label_title * communityLinksTitle;
    std::vector< pawgui::widget_button_push * > helpful_buttons;
    std::vector< pawgui::widget_text_url * > helpfulLinks;
    std::vector< pawgui::widget_button_icon * > community_buttons;
    pawgui::widget_label_image * engineLogo;
    pawgui::widget_text_editor * pencilInformation;
    pawgui::widget_text_editor * pencilChangelog;
    pawgui::widget_text_editor * pencilShortkeys;
    pawgui::widget_button_push * updateChecker_button;
    //General Help Tab [ end ]

    //Meta information to be built into the project
    pawgui::widget_text_editor * pencilCredits;
    pawgui::widget_text_editor * pencilDescription;
    pawgui::widget_text_editor * pencilLicense;
    pawgui::widget_text_editor * thirdPartyLicenses;
    pawgui::widget_text_editor * pencilMissionStatement;
    gpe::shape_rect subViewedSpace;
    pawgui::widget_panel_list * aboutPageList;
    gamePencilAboutPageResource();
    ~gamePencilAboutPageResource();

    void add_helpfulbutton(pawgui::widget_button_push * new_button);
    void add_helpfullink(pawgui::widget_text_url * newLink);
    void add_communitylink( pawgui::widget_button_icon * newLink);

    bool include_local_files( std::string pBuildDir , int buildType );

    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilAboutPageResource * main_about_page;

#endif

