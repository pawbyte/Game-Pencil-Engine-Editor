/*
gpe_editor_aboutpage.h
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

#ifndef GPE_ABOUT_PAGE_RESOURCES_H
#define GPE_ABOUT_PAGE_RESOURCES_H

#include "gpe_basic_resource_page.h"

class gamePencilAboutPageResource: public generalGameResource
{
public:
    GPE_SelectBoxBasic * sideAreaPanel;
    gpe::shape_rect * sidePanelRect;
    //Meta information to be built into the project
    GPE_TextAreaInputBasic * pencilCredits;
    GPE_TextAreaInputBasic * pencilDescription;
    GPE_TextAreaInputBasic * pencilLicense;
    GPE_TextAreaInputBasic * thirdPartyLicenses;
    GPE_TextAreaInputBasic * pencilMissionStatement;
    gpe::shape_rect subViewedSpace;
    GPE_GuiElementList * aboutPageList;
    gamePencilAboutPageResource();
    ~gamePencilAboutPageResource();
    bool include_local_files( std::string pBuildDir , int buildType );

    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilAboutPageResource * main_ABOUT_PAGE;

#endif

