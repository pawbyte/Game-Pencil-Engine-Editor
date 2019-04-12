/*
gpe_editor_helppage.h
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

#ifndef GPE_HELP_PAGE_RESOURCES_H
#define GPE_HELP_PAGE_RESOURCES_H

#include "gpe_basic_resource_page.h"

class gamePencilHelpPageResource: public generalGameResource
{
public:
    GPE_SelectBoxBasic * sideAreaPanel;
    GPE_Rect * sidePanelRect;
    //General Help Tab
    GPE_Label_Title * helpfulLinksTitle;
    GPE_Label_Title * communityLinksTitle;
    std::vector< GPE_ToolPushButton * > helpfulButtons;
    std::vector< GPE_TextURL * > helpfulLinks;
    std::vector< GPE_ToolIconButton * > communityButtons;
    GPE_Label_Image * engineLogo;
    GPE_TextAreaInputBasic * pencilInformation;
    GPE_TextAreaInputBasic * pencilChangelog;
    GPE_TextAreaInputBasic * pencilShortkeys;
    GPE_ToolPushButton * updateCheckerButton;
    GPE_Rect subViewedSpace;
    GPE_GuiElementList * helpPageList;
    gamePencilHelpPageResource();
    ~gamePencilHelpPageResource();
    void add_helpfulbutton(GPE_ToolPushButton * newButton);
    void add_helpfullink(GPE_TextURL * newLink);
    void add_communitylink( GPE_ToolIconButton * newLink);
    void prerender_self( );
    void preprocess_self(std::string alternatePath = "");
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void save_resource(std::string alternatePath = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilHelpPageResource * MAIN_HELP_PAGE;


#endif
