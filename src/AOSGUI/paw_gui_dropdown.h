/*
paw_gui_dropdown.h
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
#ifndef PAW_GUI_DROPDOWN_H
#define PAW_GUI_DROPDOWN_H

#include "paw_gui_base.h"
#include "paw_gui_parsers.h"
#include "paw_gui_context.h"


class GPE_DropDown_Menu: public GPE_GeneralGuiElement
{
private:
    std::vector <GPE_KeyPair *> subOptions;
    int widestOptionSpace;
    int maxOptionsAllowed;
    std::string dropdownName;
    int opId;
    bool isSelectable;
    int selectedId;
    bool isOpen;
    bool justActivated;
    bool showJustOptions;
public:
    bool subMenuIsOpen;
    GPE_Color *barColor;
    GPE_DropDown_Menu( std::string name = "", bool justOptions = false);
    ~GPE_DropDown_Menu();

    std::string get_data();
    void load_data(std::string dataString);
    void remove_data(std::string dataString);

    void add_menu_option(std::string optionName, std::string optionSubStr= "",double optionValue = 0, bool selectOption = false);
    std::string get_menu_option(int atNumb);
    std::string get_plain_string();
    int get_selected_id();
    std::string get_selected_tag();
    std::string get_selected_name();

    double get_selected_value();

    int get_menu_size();
    bool just_activated();
    void remove_option(std::string optionToRemove);
    void reset_suboptions();
    void set_id(int newId);
    void set_option( std::string newSelectedOptionName );
    void set_option_subvalue( std::string newSelectedOptionName );
    void set_value(int valueToFind);
    void show_just_options(bool justOptions);
    void process_self(GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL);
    void render_self( GPE_Rect * viewedSpace=NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void set_name(std::string newName);
    void set_selection(int newId, bool autoCorrect = true);
};

#endif
