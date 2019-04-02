/*
paw_gui_checkbox.h
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

#ifndef PAW_GUI_CHECKBOX_H
#define PAW_GUI_CHECKBOX_H

#include "paw_gui_base.h"

extern GPE_Texture * GPE_CHECKMARK_IMAGE;

class GPE_CheckBoxBasic: public GPE_GeneralGuiElement
{
protected:
    int clickBoxW, clickBoxH;
    int labelBoxW, labelBoxH;
public:
    GPE_CheckBoxBasic(std::string name = "", std::string descriptor = "", bool isChecked = true);
    ~GPE_CheckBoxBasic();
    std::string get_data();
    void load_data(std::string dataString);
    void prerender_self( );
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void set_checkbox_size(int nBoxSize, bool resizeelementBox = true);
};

class GPE_CheckBoxBasicList: public GPE_GeneralGuiElement
{
private:
    int opXPadding, opYPadding;
    int opWidth, opHeight;
    bool keepAlphabetized;
protected:
    std::vector < std::string > subOptions;
    std::vector <bool > posIsChecked;
public:
    bool allowDuplicates;
    GPE_CheckBoxBasicList();
    ~GPE_CheckBoxBasicList();
    std::string get_data();
    void load_data(std::string dataString);
    void add_opton(std::string newOption);
    void remove_opton(std::string optionToCut);
    void remove_opton_id(int optionToCut);
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);

};


#endif
