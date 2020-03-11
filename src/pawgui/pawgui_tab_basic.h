/*
pawgui_tab_basic.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#ifndef PAWGUI_TAB_BASIC_H
#define PAWGUI_TAB_BASIC_H

#include "pawgui_base.h"

namespace pawgui
{
    //Essentially the same as the widget_tab_resource_bar class, but tabs can not be closed by clicking an "X"
    class widget_tabbar: public widget_basic
    {
        private:
            std::vector <std::string> subOptions;
        public:
            bool canCloseTabs;
            bool tabIsRightClicked;
            int fontTextWidth, fontTextHeight;
            int barXPadding, barYPadding;
            int tabSize;
            int miniTabSize;
            int extraSmallSize;
            int smallTabSize;
            int medTabSize;
            int largeTabSize;
            int extraLargeTabSize;
            int xxLargeTabSize;
            int tabsPerView;
            int tabPos;
            int tabInUse;
            bool isInUse;
            widget_tabbar( bool dynamicClosing = false);
            ~widget_tabbar();
            void add_new_tab(std::string newOption, bool switchToNew = true);
            void calculate_tabs();
            bool contains_tab( std::string tabName );
            void open_tab(int tabId);
            void open_tab(std::string tabName);
            std::string get_selected_name();
            int get_selected_tab();
            int get_tab_count();
            void process_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect *cam = NULL);
            void remove_all_tabs( );
            bool remove_tab( int tabId );
            bool remove_tab( std::string tabName );
            void render_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect * cam = NULL);
            void select_tab( int tabToSelect );
            void toggle_tab( std::string tabName );
    };
}
#endif //PAWGUI_TAB_BASIC_H
