/*
paw_gui_search_controller.h
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


#ifndef PAW_GUI_SEARCH_CONTROLLER_H
#define PAW_GUI_SEARCH_CONTROLLER_H

#include "paw_gui_base.h"
#include "paw_gui_buttons.h"
#include "paw_gui_checkbox.h"
#include "paw_gui_dropdown.h"
#include "paw_gui_labels.h"
#include "paw_gui_text_input.h"

class PAW_GUI_SEARCH_CONTROLLER
{
public:
    //Search Settings
    int findAllStartXPos, findAllStartYPos;
    int previousSearchMode;
    int textSearchMode;
    bool showFindAllResults;
    //For Find/Replace Box of TextArea
    GPE_TextInputBasic * findTextStringBox;
    GPE_TextInputNumber * goToLineStringBox;
    GPE_TextInputBasic * replaceTextStringBox;
    GPE_ToolLabelButton * findButton;
    GPE_ToolLabelButton * findAllButton;
    GPE_ToolLabelButton * goToButton;
    GPE_ToolLabelButton * replaceButton;
    GPE_ToolLabelButton * replaceAllButton;
    GPE_CheckBoxBasic * findMatchCase;

    GPE_Label_Text * scopeLabel;
    GPE_DropDown_Menu * findScope;
    PAW_GUI_SEARCH_CONTROLLER();
    ~PAW_GUI_SEARCH_CONTROLLER();
    void close_finder();
    bool using_search();
};

extern PAW_GUI_SEARCH_CONTROLLER * MAIN_SEARCH_CONTROLLER;

#endif
