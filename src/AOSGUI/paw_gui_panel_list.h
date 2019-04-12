/*
paw_gui_panel_list.h
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

#ifndef PAW_GUI_PANEL_LIST_H
#define PAW_GUI_PANEL_LIST_H

#include "paw_gui_scrollbars.h"

class GPE_GuiElementRow: public GPE_GeneralGuiElement
{
private:
    int calculatedRowWidth, calculatedRowHeight;
public:
    bool inDebugMode;
    int outterWidth, outterHeight;
    int barXPadding, barYPadding;
    std::vector < GPE_GeneralGuiElement * > subOptions;
    GPE_GuiElementRow();
    ~GPE_GuiElementRow();
    void add_gui_element(GPE_GeneralGuiElement *  newElement );
    void calculate_width();
    void clear_list();
    int get_sub_width();
    void set_coords(int newX = -1, int newY = -1);
    void set_full_width();
    void set_full_width( int maxRowWidth );
    void set_horizontal_align(int hValue);
    void set_vertical_align(int vValue);
    void set_maxed_out_width();
    void set_maxed_out_height();

};

class GPE_GuiElementList: public GPE_GeneralGuiElement
{
public:
    bool inDebugMode;
    double tabDelay;
    double leserKeyDelay;
    double greaterKeyDelay;
    double upDelay;
    double downDelay;
    double leftDelay;
    double rightDelay;
    GPE_Rect entireBox, cameraBox, menuBox;
    GPE_ScrollBar_XAxis * xScroll;
    GPE_ScrollBar_YAxis * yScroll;

    GPE_GuiElementRow * selectedRow;
    GPE_GeneralGuiElement * selectedElement;
    std::vector < GPE_GuiElementRow * > subRows;
    std::vector < GPE_GeneralGuiElement * > allElements;
    bool usingFullSizeElement;
    bool newRowRequested;
    bool isInUse;
    bool hideXScroll;
    bool hideYScroll;
    bool subElementsHasArrowControl;
    bool subElementsIsScrolling;
    int barXPadding, barYPadding;
    int barXMargin, barYMargin;
    int selectedId;
    GPE_GuiElementList();
    ~GPE_GuiElementList();
    void add_gui_element(GPE_GeneralGuiElement *  newElement, bool isNLElement );
    void add_gui_auto(GPE_GeneralGuiElement *  newElement );
    void add_gui_element_fullsize(GPE_GeneralGuiElement * newElement );
    void add_indented_element( int level, GPE_GeneralGuiElement * newElement );
    void clear_list();
    void scroll_left(int xToMove );
    void scroll_right(int xToMove );
    void scroll_down(int yToMove );
    void scroll_up(int yToMove );
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void reset_self();
    void set_horizontal_align(int hValue);
    void set_full_width();
    void set_maxed_out_width();
    void set_maxed_out_height();
};

#endif
