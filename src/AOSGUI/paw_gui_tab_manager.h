/*
paw_gui_tab_manager.h
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

#ifndef PAW_GUI_TAB_MANAGER_H
#define PAW_GUI_TAB_MANAGER_H

#include "paw_gui_base.h"
#include "paw_gui_buttons.h"

class GPE_TabManager: public GPE_GeneralGuiElement
{
private:
    bool openContextMenu;
    GPE_Rect tabBox;
    GPE_Rect tabHeaderBox;
    std::vector <generalGameResource *> subOptions;
    GPE_ToolIconButton * tabExpandButton;
    bool isExpanded;
public:
    bool useDockButton;
    generalGameResource * selectedResource;
    int barXPadding, barYPadding;
    int tabToClose;
    int tabSize;
    int tabXPAdding;
    int miniTabSize;
    int extraSmallSize;
    int smallTabSize;
    int medTabSize;
    int largeTabSize;
    int extraLargeTabSize;
    int xxLargeTabSize;
    int tabsPerView;
    int tabPos;
    int tabXHover;
    int tabInUse;
    GPE_TabManager();
    ~GPE_TabManager();

    void add_new_tab(generalGameResource * newTabResource);
    generalGameResource * get_selected_resource();
    void close_resource_tab( std::string projectParentName, int resIdIn);
    void close_tab( int tabIdToClose);
    void close_tabs_left( int tabIdToClose);
    void close_tabs_right( int tabIdToClose);
    void close_tabs_from_project( std::string projectDirNameIn);
    void close_all_tabs();
    void move_to_tab(int newTabId);
    void process_self(GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    bool requests_fullscreen();
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    void set_coords(int newX, int newY);
    void set_height(int newHeight);
    void set_selected_gresource(generalGameResource * newResource = NULL);
    void set_width(int newWidth);
    void update_tabsizes();
    void toggle_full_width();
};

extern GPE_TabManager * GPE_Main_TabManager;

#endif
