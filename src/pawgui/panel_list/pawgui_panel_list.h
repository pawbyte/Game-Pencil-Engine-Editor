/*
pawgui_panel_list.h
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

#ifndef PAWGUI_PANEL_LIST_H
#define PAWGUI_PANEL_LIST_H

#include "../scrollbars/pawgui_scrollbars.h"

namespace pawgui
{
    enum
    {
        panel_align_default = 0,     /**< Panel elements not modified */
        panel_align_full = 1,     /**< Panel elements modified to fit within panel */
        panel_align_left =2,     /**< Panel elements modified to fit within panel with left element getting most of space */
        panel_align_right =3,     /**< Panel elements modified to fit within panel with right element getting most of space */
        panel_align_equal = 4,     /**< Panel elements modified to resize equally after non-resizable elements */
    };

    enum
    {
        row_type_general = 0,
        row_type_spacing = 1,
        row_type_hr = 2,
    };

    class widget_content_row: public widget_basic
    {
        private:
            int calculatedRowWidth, calculatedRowHeight;
        public:
            int rowClassType;
            bool lastColumnFloatsRight;
            bool inDebugMode;
            int rowSizingStyle;
            int outterWidth, outterHeight;
            int barXPadding, barYPadding;
            std::vector < widget_basic * > sub_options;
            widget_content_row();
            ~widget_content_row();
            void add_gui_element(widget_basic *  newElement );
            void calculate_width();
            void clear_list();
            int get_sub_width();
            void set_coords(int x_new = -1, int y_new = -1);
            void set_full_width();
            void set_full_width( int maxRowWidth );
            void set_horizontal_align(int height_valueue);
            void set_vertical_align(int vValue);
            void set_maxed_out_width();
            void set_maxed_out_height();

    };

    struct panel_section_item
    {
        widget_basic * widget_element;
        bool requiresNewLine;
    };


    class widget_panel_section: public widget_basic
    {
        private:
            bool autoIndents;
            bool sectionIsOpen;
        public:
            std::vector < panel_section_item > sub_options;
            widget_panel_section(std::string section_name, bool auto_indent_elements = true);
            ~widget_panel_section();
            bool auto_indents();
            bool add_widget( widget_basic * widget_element, bool requires_nl = true );
            bool is_section_open();
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    };

    class widget_panel_list: public widget_basic
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
        widget_scrollbar_xaxis * xScroll;
        widget_scrollbar_yaxis * yScroll;

        widget_content_row * selectedRow;
        widget_basic * selectedElement;
        std::vector < widget_content_row * > subRows;
        std::vector < widget_basic * > allElements;
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
        bool lastColumnFloatsRight;
        widget_panel_list();
        ~widget_panel_list();
        void add_gui_auto(widget_basic *  newElement );
        void add_gui_element(widget_basic *  newElement, bool isNLElement );
        void add_gui_element_fullsize(widget_basic * newElement );
        void add_gui_section(widget_panel_section *  newElement );
        void add_indented_element( int level, widget_basic * newElement );
        void clear_list();
        void scroll_left(int xToMove );
        void scroll_right(int xToMove );
        void scroll_down(int yToMove );
        void scroll_up(int yToMove );
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void reset_self();
        void set_horizontal_align(int height_valueue);
        void set_full_width();
        void set_maxed_out_width();
        void set_maxed_out_height();
    };
}

#endif //PAWGUI_PANEL_LIST_H
