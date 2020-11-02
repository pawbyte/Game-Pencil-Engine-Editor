/*
gpe_editor_start_page.h
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
#ifndef GPE_START_PAGE_H
#define GPE_START_PAGE_H

#include "gpe_basic_resource_page.h"
#include "gpe_editor.h"
#include "gpe_editor_aboutpage.h"

class quickProjectReader
{
public:
    int resourcesInProject;
    std::string detectedName;
    std::string detectedIcon;
    std::string detectedLanguage;
    std::string detectedLocation;
    std::string detectedSaveTime;
    std::string detectedVersion;
    quickProjectReader();
    ~quickProjectReader();
    bool review_project(std::string projectFileName);
};

class gamePencilStartPageResource: public pawgui::general_resource
{
public:
    int recent_project_list_size;

    //Project Browser related variables [ BEGIN ]
    pawgui::widget_label_text  * projectFolderLocationLabel;
    pawgui::widget_button_push * scanProjectFolderButton;
    pawgui::widget_button_push * newProjectButton;
    pawgui::widget_button_push * loadProjectButton;
    pawgui::widget_button_push * refreshButton;
    pawgui::widget_label_text  * emptyProjectsListLabel;
    std::string foundProjectsDirectory;
    std::vector< pawgui::widget_button_card_vertical * > foundProjectsList;
    pawgui::widget_panel_list * headerPageList;
    pawgui::widget_tabbar * projectsCategoryTabs;
    std::vector< pawgui::widget_button_card_vertical * > examplesProjectsList;
    gpe::shape_rect * sidePanelRect;
    gpe::shape_rect * headerViewedSpace;
    gpe::shape_rect * subViewedSpace;
    int headerSpaceHeight;
    std::vector< pawgui::widget_button_card_vertical * > recentProjectsList;

    bool processedFirst;
    //Project Browser related variables [ END ]

    //Meta information to be built into the project
    pawgui::widget_selectbox * sidePanelSelectBox;
    pawgui::widget_panel_list * sidePanelList;

    pawgui::widget_text_editor_wrapped * testText;
    pawgui::widget_label_title * labelEditorShortcuts;
    pawgui::widget_label_title * labelPatreon;
    pawgui::widget_label_image * labelPatreonLogo;
    pawgui::widget_label_paragraph * labelPatreonDescription;
    pawgui::widget_text_url * urlPatreonPage;

    pawgui::widget_label_title * labelHelpfulLinks;
    pawgui::widget_label_image * labelImgGPELogo;
    pawgui::widget_label_text  * labelCopyrightInfo;

    pawgui::widget_tabbar * startPageTabBar;
    std::vector< pawgui::widget_text_url * > editorShortcuts;
    std::vector< pawgui::widget_text_url * > helpfulWebLinks;
    pawgui::widget_label_title * communityLinksTitle;

    std::vector< pawgui::widget_button_icon * > communityButtons;

    pawgui::widget_button_push * projectBrowserButton;
    pawgui::widget_button_push * forumPageButton;
    pawgui::widget_button_push * helpPageButton;
    pawgui::widget_button_push * tipOfTheDayButton;

    pawgui::widget_panel_list * startPageList;
    gamePencilStartPageResource();
    ~gamePencilStartPageResource();
    void add_communitylink( pawgui::widget_button_icon * newLink);
    bool include_local_files( std::string pBuildDir , int buildType );

    void load_example_projects_folder( );
    void load_projecs_folder( std::string projectsDirectory);
    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilStartPageResource * main_START_PAGE;

#endif //GPE_START_PAGE_H
