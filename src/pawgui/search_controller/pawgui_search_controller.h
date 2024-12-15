/*
pawgui_search_controller.h
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


#ifndef PAWGUI_SEARCH_CONTROLLER_H
#define PAWGUI_SEARCH_CONTROLLER_H

#include "../base/pawgui_base.h"
#include "../buttons/pawgui_buttons.h"
#include "../checkbox/pawgui_checkbox.h"
#include "../dropdown/pawgui_dropdown.h"
#include "../labels/pawgui_labels.h"
#include "../text_input/pawgui_text_input.h"

namespace pawgui
{
    class search_controller
    {
        public:
            //Search _settings
            int findAllStartXPos, findAllStartYPos;
            pawgui::search_mode previousSearchMode;
            pawgui::search_mode textSearchMode;
            bool showFindAllResults;
            //For Find/Replace Box of TextArea
            widget_input_text * findTextstringBox;
            widget_input_number * goToLinestringBox;
            widget_input_text * replaceTextstringBox;
            widget_button_label * find_button;
            widget_button_label * findAll_button;
            widget_button_label * goTo_button;
            widget_button_label * replace_button;
            widget_button_label * replaceAll_button;
            widget_checkbox * findMatchCase;

            widget_label_text * scopeLabel;
            widget_dropdown_menu * findScope;
            search_controller();
            ~search_controller();
            void close_finder();
            bool using_search();
    };
    extern search_controller * main_search_controller;
}
#endif //PAWGUI_SEARCH_CONTROLLER_H
