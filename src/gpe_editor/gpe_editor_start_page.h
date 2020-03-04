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

class gamePencilStartPageResource: public generalGameResource
{
public:
    //Project Browser related variables [ BEGIN ]
    GPE_Label_Text * projectFolderLocationLabel;
    GPE_ToolPushButton * scanProjectFolderButton;
    GPE_ToolPushButton * newProjectButton;
    GPE_ToolPushButton * loadProjectButton;
    GPE_ToolPushButton * refreshButton;
    GPE_Label_Text * emptyProjectsListLabel;
    std::string foundProjectsDirectory;
    std::vector< GPE_VerticalCardButton * > foundProjectsList;
    GPE_GuiElementList * headerPageList;
    GPE_TabBar * projectsCategoryTabs;
    std::vector< GPE_VerticalCardButton * > examplesProjectsList;
    gpe::shape_rect * sidePanelRect;
    gpe::shape_rect * headerViewedSpace;
    gpe::shape_rect * subViewedSpace;
    int headerSpaceHeight;
    std::vector< GPE_VerticalCardButton * > recentProjectsList;

    bool processedFirst;
    //Project Browser related variables [ END ]

    //Meta information to be built into the project
    GPE_SelectBoxBasic * sidePanelSelectBox;
    GPE_GuiElementList * sidePanelList;

    GPE_WrappedTextArea * testText;
    GPE_Label_Title * labelEditorShortcuts;
    GPE_Label_Title * labelPatreon;
    GPE_Label_Image * labelPatreonLogo;
    GPE_Label_Paragraph * labelPatreonDescription;
    GPE_TextURL * urlPatreonPage;

    GPE_Label_Title * labelHelpfulLinks;
    GPE_Label_Image * labelImgGPELogo;
    GPE_Label_Text * labelCopyrightInfo;

    GPE_TabBar * startPageTabBar;
    std::vector< GPE_TextURL * > editorShortcuts;
    std::vector< GPE_TextURL * > helpfulWebLinks;
    GPE_Label_Title * communityLinksTitle;

    std::vector< GPE_ToolIconButton * > communityButtons;

    GPE_ToolPushButton * projectBrowserButton;
    GPE_ToolPushButton * forumPageButton;
    GPE_ToolPushButton * helpPageButton;
    GPE_ToolPushButton * tipOfTheDayButton;

    GPE_GuiElementList * startPageList;
    gamePencilStartPageResource();
    ~gamePencilStartPageResource();
    void add_communitylink( GPE_ToolIconButton * newLink);
    bool include_local_files( std::string pBuildDir , int buildType );

    void load_example_projects_folder( );
    void load_projecs_folder( std::string projectsDirectory);
    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilStartPageResource * main_START_PAGE;

#endif //GPE_START_PAGE_H
