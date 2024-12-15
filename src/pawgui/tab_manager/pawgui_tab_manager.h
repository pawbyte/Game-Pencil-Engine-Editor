/*
pawgui_tab_manager.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/

#ifndef PAWGUI_TAB_RESOURCEBAR_H
#define PAWGUI_TAB_RESOURCEBAR_H

#include "../base/pawgui_base.h"
#include "../buttons/pawgui_buttons.h"
#include "../general_resource/pawgui_general_resource.h"

namespace pawgui
{
    class widget_tab_resource_bar: public widget_basic
    {
        private:
            bool openContextMenu;
            gpe::shape_rect tabBox;
            gpe::shape_rect tabHeaderBox;
            std::vector <general_resource *> sub_options;
            widget_button_icon * tabExpand_button;
            bool isExpanded;
        public:
            bool useDock_button;
            general_resource * selectedResource;
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
            widget_tab_resource_bar();
            ~widget_tab_resource_bar();

            void add_new_tab(general_resource * newTabResource);
            general_resource * get_selected_resource();
            void close_resource_tab( std::string projectParentName, int resIdIn);
            void close_tab( int tabIdToClose);
            void close_tabs_left( int tabIdToClose);
            void close_tabs_right( int tabIdToClose);
            void close_tabs_from_project( std::string projectDirNameIn);
            void close_all_tabs();
            void move_to_tab(int newTabId);
            void process_self( gpe::shape_rect * view_space=nullptr, gpe::shape_rect *cam=nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            bool requests_fullscreen();
            int search_for_string(std::string needle);
            int search_and_replace_string(std::string needle, std::string newStr = "");
            void set_coords(int x_new, int y_new);
            void set_height(int new_height);
            void set_selected_gresource(general_resource * newResource = nullptr);
            void set_width(int new_width);
            void update_tabsizes();
            void toggle_full_width();
    };
    extern widget_tab_resource_bar * main_tab_resource_bar;
}
#endif //PAWGUI_TAB_RESOURCEBAR_H
