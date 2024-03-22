/*
gpe_resource_tree.h
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

#ifndef GPE_RESOURCE_TREE_H
#define GPE_RESOURCE_TREE_H

#include "../pawgui/pawgui.h"

extern int resourcetype_last_rendered_type;

class GPE_ResourceTree: public pawgui::widget_basic
{
private:
    pawgui::widget_resource_container * backbutton_option;
    pawgui::widget_resource_container * selected_option;
    pawgui::widget_resource_container * selected_folder_option;
    std::vector <pawgui::widget_resource_container *> sub_options;
    std::vector <pawgui::widget_resource_container *> options_in_browser;
    int lastWidth;
    int barTitleWidth;
    int barTitleHeight;
    std::string searchString;
    int treeMode;
    int treeColumnCount, treeRowCount;
    int currentIconSize;
    int smallIconSize, mediumIconSize, largeIconSize, xlIconSize;
    int options_position;
    int options_max_in_view;
    pawgui::widget_input_text * searchBox;
    pawgui::widget_button_iconbar * treeModeBar;
    pawgui::widget_panel_list * treeGuiList;
public:
    bool showYScroll;
    gpe::shape_rect entireBox, cameraBox, viewBox;
    pawgui::widget_scrollbar_xaxis * xScroll;
    pawgui::widget_scrollbar_yaxis * yScroll;
    int barXPadding, barYPadding;
    bool subMenuIsOpen;
    int selectedSubOption;
    gpe::texture_base * opTexture;
    GPE_ResourceTree();
    ~GPE_ResourceTree();
    void add_resource_container( pawgui::widget_resource_container * new_resource );
    pawgui::widget_resource_container * add_resource_folder(int resourceType, std::string projFolderName,std::string resourceTypeName);
    pawgui::widget_resource_container * add_project_folder(int resourceType, std::string projFolderName,std::string resourceTypeName);
    void delete_project_resources(std::string projectFileName );
    void prerender_self( );
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr );
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void remove_project_resources(std::string projectFileName );
};

#endif
