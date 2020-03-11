/*
paw_gui_panel_list.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef PAW_GUI_PANEL_LIST_H
#define PAW_GUI_PANEL_LIST_H

#include "paw_gui_scrollbars.h"

typedef enum
{
    GPE_PANEL_ALIGN_DEFAULT,     /**< Panel elements not modified */
    GPE_PANEL_ALIGN_FULLROWS,     /**< Panel elements modified to fit within panel */
    GPE_PANEL_ALIGN_FULL_LEFT,     /**< Panel elements modified to fit within panel with left element getting most of space */
    GPE_PANEL_ALIGN_FULL_RIGHT,     /**< Panel elements modified to fit within panel with right element getting most of space */
    GPE_PANEL_ALIGN_FULL_EQUAL,     /**< Panel elements modified to resize equally after non-resizable elements */
} GPE_Panel_Setup_Type;

class GPE_GuiElementRow: public GPE_GeneralGuiElement
{
private:
    int calculatedRowWidth, calculatedRowHeight;
public:
    bool inDebugMode;
    int rowSizingStyle;
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
    float tabDelay;
    float leserKeyDelay;
    float greaterKeyDelay;
    float upDelay;
    float downDelay;
    float leftDelay;
    float rightDelay;
    gpe::shape_rect entireBox, cameraBox, menuBox;
    GPE_ScrollBar_XAxis * xScroll;
    GPE_ScrollBar_YAxis * yScroll;

    GPE_GuiElementRow * selectedRow;
    GPE_GeneralGuiElement * selectedElement;
    std::vector < GPE_GuiElementRow * > subRows;
    std::vector < GPE_GeneralGuiElement * > allElements;
    bool usingFullSizeElement;
    int panelAlignType;
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
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void reset_self();
    void set_horizontal_align(int hValue);
    void set_full_width();
    void set_maxed_out_width();
    void set_maxed_out_height();
};

#endif
