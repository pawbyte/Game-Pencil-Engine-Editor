/*
paw_gui_text_input.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/


#ifndef PAW_GUI_TEXT_INPUT_H
#define PAW_GUI_TEXT_INPUT_H

#include "paw_gui_base.h"
#include "paw_gui_context.h"

const int MAX_STORED_TEXT_PREVIOUS_ENTRIES = 32;
class GPE_TextInputBasic: public GPE_GeneralGuiElement
{
protected:
    int prevelementBoxW, prevelementBoxH;
    std::string textInputString;
    std::string placeHolderString;
    int displayMode;
    int cursorPos;
    int prevCursorPos;
    int startXPos;
    int maxCharactersInView;
    int fontSizeW, fontSizeH;
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
    int cursorHistoryPos;
    std::vector< std::string> listOfPastStrings;
    GPE_Rect fieldElementBox;
public:
    bool inputSubmitted;
    bool resubmissionAllowed;
    std::string linkedProjectFileName;
    std::string inputLabel;
    bool showLabel;
    bool showPlaceHolder;
    int lastfloatClickAction;
    int selectionCursorPos, selectionEndCursorPos;
    GPE_TextInputBasic(std::string startName="",std::string placeHolderText="");
    ~GPE_TextInputBasic();
    std::string get_data();
    void load_data(std::string dataString);
    void copy_selection();
    void cut_selection();
    void delete_selection();
    void get_cursor_from_mouse(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    std::string get_label();
    std::string get_plain_string();
    std::string get_string();
    bool has_content();
    bool is_valid();
    void move_left(int xToMove = 1);
    void move_right(int xToMove = 1);
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void record_string( std::string strToRecord);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void reset_selection(int resetDirection = 0);
    void select_all();
    void set_height(int newHeight);
    void set_hovered(bool hoverVal);
    void set_number( float numberIn);
    void set_max_width( int nMW);
    void set_max_height( int nMH);
    void set_width(int newWidth);
    void set_label(std::string newLabel);
    void set_name(std::string newName);
    void set_string(std::string newString);
    void set_placeholder(std::string newString);
    bool was_submitted();
};

class GPE_TextInputNumber: public GPE_TextInputBasic
{
protected:
    bool onlyWholeNumbers;
    float heldNumber;
    float minInputNumber, maxInputNumber;
public:
    bool forceValidNumber;
    GPE_TextInputNumber( std::string placeHolderText, bool onlyWholeNums = true, float hMin = INT_MIN+1, float = INT_MAX-1);
    ~GPE_TextInputNumber();
    int get_held_int();
    float get_held_number();
    std::string get_plain_string();
    float make_valid_number(float fallbackNumber );
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
};

#endif
