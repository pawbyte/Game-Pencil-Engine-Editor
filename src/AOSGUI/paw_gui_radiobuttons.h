/*
paw_gui_radiobuttons.h
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

#ifndef PAW_GUI_RADIOBUTTONS_H
#define PAW_GUI_RADIOBUTTONS_H

#include "paw_gui_base.h"
#include "paw_gui_parsers.h"

class GPE_RadioButtonControllerBasic: public GPE_GeneralGuiElement
{
private:
    int selectedId;
    int hoveredOption;
    int opXPadding, opYPadding;
    int opWidth, opHeight;
    bool keepAlphabetized;
    int numCols;
    int rowsPerCol;
protected:
    std::vector <GPE_KeyPair *> subOptions;
public:
    bool allowDuplicates;
    GPE_RadioButtonControllerBasic(std::string cName = "", bool alphabetize = false, int colNum=1);
    ~GPE_RadioButtonControllerBasic();
    std::string get_data();
    void load_data(std::string dataString);
    void add_opton(std::string newOption);
    void add_menu_option(std::string optionName, std::string optionSubStr= "",int optionValue = 0, bool selectOption = false);
    std::string get_menu_option(int atNumb);
    std::string get_plain_string();
    int get_selected_id();
    std::string get_selected_tag();
    int get_selected_value();

    void organize_options();
    void remove_data(std::string dataString);
    void remove_option(std::string optionToCut);
    void remove_option_id(int optionToCut);
    void set_selection( int newSelection);
    bool set_from_tag(std::string newSelectedTag);
    void set_value( int valueToSelect);
    void prerender_self( );
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
};


#endif
