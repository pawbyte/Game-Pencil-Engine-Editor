/*
gpe_editor_start_page.h
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
#ifndef GPE_START_PAGE_H
#define GPE_START_PAGE_H

#include "gpe_basic_resource_page.h"

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
    GPE_Rect * sidePanelRect;
    GPE_Rect * headerViewedSpace;
    GPE_Rect * subViewedSpace;
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

    void load_example_projects_folder( );
    void load_projecs_folder( std::string projectsDirectory);
    void prerender_self( );
    void preprocess_self(std::string alternatePath = "");
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void save_resource(std::string alternatePath = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilStartPageResource * MAIN_START_PAGE;

#endif //GPE_START_PAGE_H
