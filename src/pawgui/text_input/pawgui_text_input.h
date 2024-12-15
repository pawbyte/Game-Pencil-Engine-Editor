/*
pawgui_text_input.h
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


#ifndef PAWGUI_INPUT_TEXT_H
#define PAWGUI_INPUT_TEXT_H

#include "../base/pawgui_base.h"
#include "../context/pawgui_context.h"

namespace pawgui
{
    const int MAX_STORED_TEXT_PREVIOUS_ENTRIES = 32;
    class widget_input_text: public widget_basic
    {
        protected:
            int prevwidget_boxW, prevwidget_boxH;
            std::string textInputstring;
            std::string placeHolderstring;
            int displayMode;
            int cursorPos;
            int prevCursorPos;
            int startXPos;
            int maxCharactersInView;
            int font_sizeW, font_sizeH;
            int cursorTimer;
            bool showCursor;
            int maxLength;
            float bscDelay;
            float delDelay;
            float enterDelay;
            float upArrowDelay;
            int downArrowDelay;
            float leftArrowDelay;
            float rightArrowDelay;
            int inputFieldPos;
            bool hasValidInput;
            int cursorHistory_pos;
            std::vector< std::string> listOfPaststrings;
            gpe::shape_rect fieldElementBox;
        public:
            int inputFieldPadding;
            bool inputIsPassword;
            bool inputSubmitted;
            bool resubmissionAllowed;
            std::string linkedProjectFileName;
            std::string inputLabel;
            bool showLabel;
            bool showPlaceHolder;
            int lastfloatClickAction;
            int selectionCursorPos, selectionEndCursorPos;
            widget_input_text(std::string startName="",std::string placeHolderText="");
            ~widget_input_text();
            std::string get_data();
            void load_data(std::string datastring);
            void copy_selection();
            void cut_selection();
            void delete_selection();
            void get_cursor_from_mouse( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            std::string get_label();
            std::string get_plain_string();
            std::string get_string();
            bool has_content();
            bool is_valid();
            void move_left(int xToMove = 1);
            void move_right(int xToMove = 1);
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void record_string( std::string strToRecord);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void reset_selection(int resetDirection = 0);
            void select_all();
            void set_height(int new_height);
            void set_hovered(bool hoverVal);
            void set_number( float numberIn);
            void set_max_width( int nMW);
            void set_max_height( int nMH);
            void set_width(int new_width);
            void set_label(std::string newLabel);
            void set_name(std::string new_name);
            void set_string(std::string newstring);
            void set_placeholder(std::string newstring);
            bool was_submitted();
    };

    class widget_input_number: public widget_input_text
    {
        protected:
            bool onlyWholeNumbers;
            float heldNumber;
            float minInputNumber, maxInputNumber;
        public:
            bool forceValidNumber;
            widget_input_number( std::string placeHolderText, bool onlyWholeNums = true, float hMin = INT_MIN+1, float = INT_MAX-1);
            ~widget_input_number();
            int get_held_int();
            float get_held_number();
            std::string get_plain_string();
            float make_valid_number(float fallbackNumber );
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    };
}
#endif //PAWGUI_INPUT_TEXT_H
