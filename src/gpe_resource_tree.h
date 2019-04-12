/*
gpe_resource_tree.h
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

#ifndef GPE_RESOURCE_TREE_H
#define GPE_RESOURCE_TREE_H

#include "AOSGUI/paw_gui.h"

extern int lastResTypeRendered;

class GPE_ResourceTree: public GPE_GeneralGuiElement
{
private:
    std::vector <GPE_GeneralResourceContainer *> subOptions;
    int lastWidth;
    int barTitleWidth;
    int barTitleHeight;
public:
    bool showYScroll;
    GPE_Rect entireBox, cameraBox, viewBox;
    GPE_ScrollBar_XAxis * xScroll;
    GPE_ScrollBar_YAxis * yScroll;
    int barXPadding, barYPadding;
    bool subMenuIsOpen;
    int selectedSubOption;
    GPE_Texture * opTexture;
    GPE_ResourceTree();
    ~GPE_ResourceTree();
    GPE_GeneralResourceContainer * add_resource_folder(int resourceType, std::string projFolderName,std::string resourceTypeName);
    GPE_GeneralResourceContainer * add_project_folder(int resourceType, std::string projFolderName,std::string resourceTypeName);
    void delete_project_resources(std::string projectFileName );
    void prerender_self( );
    void process_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL );
    void render_self( GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void remove_project_resources(std::string projectFileName );
};

#endif
