/*
paw_gui_text_editor.h
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

#ifndef PAW_TEXT_EDITOR_H
#define PAW_TEXT_EDITOR_H

#include "paw_gui_syntax_handler.h"
#include "paw_gui_base.h"
#include "paw_gui_buttons.h"
#include "paw_gui_labels.h"
#include "paw_gui_parsers.h"
#include "paw_gui_scrollbars.h"

const int TEXTAREA_OPTION_EXPORT = 0;
const int TEXTAREA_OPTION_IMPORT = 1;
const int TEXTAREA_OPTION_CLEAR = 2;
const int TEXTAREA_OPTION_UNDO = 3;
const int TEXTAREA_OPTION_REDO = 4;
const int TEXTAREA_OPTION_CUT = 5;
const int TEXTAREA_OPTION_COPY = 6;
const int TEXTAREA_OPTION_PASTE = 7;
const int TEXTAREA_OPTION_TEXTINFO = 8;
const int TEXTAREA_OPTION_MAX_OPTIONS = 9;

class GPE_Log_Entry
{
public:
    std::string projectLogBelongsTo;
    std::string resource_nameRelated;
    std::string functionName; //where applicable
    std::string logText;
    int lineToReach, characterToReach;
    GPE_Log_Entry(std::string projectName, std::string resource_name, std::string logTxt, std::string funcName = "", int lineNumb = -1, int charNumb = -1);
    ~GPE_Log_Entry();
};



const int GPE_ANCHOR_REGULAR = 0;
const int GPE_ANCHOR_ERROR = 1;
const int GPE_ANCHOR_WARNING = 2;
const int GPE_ANCHOR_COMMENT = 3;

class GPE_TextAnchor: public GPE_GeneralGuiElement
{
public:
    int anchorType, lineNumber, characterNumber;
    std::string lineMessage;
    std::string lineAlert;
    std::string anchorProjectName;
    std::string anchorProjectResourceName;
    int anchorProjectResourceId;
    GPE_TextAnchor(int lineN, int charN, std::string messageIn, std::string alertInfo, int anchorType = 0);
    ~GPE_TextAnchor();
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
};

//Added this here since the main logs class will just rob this guy :-)
class GPE_TextAnchorController: public GPE_GeneralGuiElement
{
public:
    //For Find/Replace Box of TextArea
    std::string searchResultProjectName;
    std::string searchResultResourceName;
    int searchResultResourceId;

    //List of anchors found
    std::vector< GPE_TextAnchor * > loggedAnchors;
    GPE_TextAnchorController();
    ~GPE_TextAnchorController();
    void clear_list(bool deleteAnchors = false );
};

extern GPE_TextAnchorController * GPE_ANCHOR_GC;

class GPE_TextAreaInputBasic: public GPE_GeneralGuiElement
{
protected:
    float editorZoomLevel;
    float defaultLineHeight;
    bool undoableActionOccurred;
    time_t lastUndoableActionTime;
    int lastfloatClickAction;
    int currentPositionInHistory;
    std::vector<GPE_TextAreaInputBasic *> savedHistory;
    bool isReadOnly;
    int inputFieldPos;
    bool useWordWrap;
    bool showXScroll, showYScroll;
    int displayMode;
    int cursorXPos, cursorYPos;
    int prevCursorXPos, prevCursorYPos;
    int cursorTimer;
    bool showCursor;
    int selectionCursorXPos, selectionCursorYPos;
    int selectionEndCursorXPos, selectionEndCursorYPos;

    int symbolCursorXPos, symbolCursorYPos;
    int symbolEndCursorXPos, symbolEndCursorYPos;

    int maxLength;
    int maxLineCount;
    int lineCountBoxWidth;
    int longestString;
    int linesWithinView;
    float prevLinesWithinView;
    float lineStartXPos;
    int charactersWithinView;
    int prevCharactersWithinView;
    float lineStartYPos;

    //Text Area Delays
    float delDelay;
    float bscDelay;
    float tabDelay;
    float enterDelay;
    float upDelay;
    float downDelay;
    float leftDelay;
    float rightDelay;
    float dKeyDelay;

    int TEXTBOX_FONT_SIZE_WIDTH, TEXTBOX_FONT_SIZE_HEIGHT;
    gpe::font_base * editorFont;
    std::string placeHolderString;
    std::string textInputString;
    std::string lineToEdit;
    std::vector <std::string> listOfStrings;
    std::vector < GPE_TextAnchor *> anchorPositions;
    GPE_ScrollBar_XAxis * textXScroll;
    GPE_ScrollBar_YAxis * textYScroll;
    gpe::shape_rect * lineCountBox;
    gpe::shape_rect * renderBox;
    gpe::shape_rect * textSpaceRect;
    GPE_SyntaxLine * commentLineText;
    GPE_SyntaxLine * datatypeLineText;
    GPE_SyntaxLine * dQuoteLineText;
    GPE_SyntaxLine * functionLineText;
    GPE_SyntaxLine * keywordLineText;
    GPE_SyntaxLine * variableLineText;
    GPE_SyntaxLine * projectFunctionLineText;
    GPE_SyntaxLine * projectKeywordLineText;
    GPE_SyntaxLine * normalLineText;
    GPE_SyntaxLine * numberLineText;
    GPE_SyntaxLine * sQuoteLineText;
    GPE_SyntaxLine * symbolLineText;
public:
    bool showButtonBar;
    int parseForErrorsTimerGoal;
    int parseForErrorsTimerPos;
    bool missingSymbolDetected;
    int missingSymbolStartYPos;
    int missingSymbolStartXPos;
    std::vector<std::string> foundSyntaxErrors;
    int misingSymbolSuggestedEndYPos;
    int misingSymbolSuggestedEndXPos;
    //Used for code highlights and suggestions.
    int highlightXPos, highlightYPos;
    //End of code highlights and suggestion related variables

    GPE_ToolIconButtonBar * textEditorButtonBar;
    bool showPlaceHolder;
    bool isTextLog;
    bool isCodeEditor;
    int codeEditorType;
    int redrawDelay;
    int redrawDelayMax;
    int tempCLineXStartPos;
    int tempCLineXEndPos;
    GPE_TextAreaInputBasic(bool saveFirstEdit = false);
    ~GPE_TextAreaInputBasic();
    void activate_self();
    void add_line( std::string newLine, bool scrollToBottom = false);
    void adjust_fortabs();
    bool can_redo();
    bool can_undo();
    bool can_undo_or_redo();
    void clear_line(int lineNumber);
    void clear_all_lines();
    void clear_text_anchors();
    void copy_selection();
    bool compile_into_code(std::ofstream * fileTarget, int leftTabAmount = 0, bool useNewLines = true, bool useLastNewLine = false);
    int compare_with(GPE_TextAreaInputBasic * other = NULL);
    bool copy_all();
    void copy_source(GPE_TextAreaInputBasic * other = NULL, bool overwriteSelf = false);
    void cut_selection();
    void create_buttonbar();
    void delete_content();
    void delete_selection();
    void duplicate_text();
    bool find_connected_symbols();
    int get_line_count();
    int get_most_anchors_characters();
    int get_most_characters_used();
    int get_xcursor();
    int get_ycursor();
    std::string get_line_string(int lineNumberIn);
    std::string get_short_hightlighted();
    std::string get_string();
    std::string get_string_spaces();
    std::vector <std::string> get_all_strings();
    void handle_scrolling( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL );
    bool has_content();
    bool has_selection();
    void init_save_history();
    bool is_read_only();
    void log_editable_action();
    void move_left(int xToMove = 1);
    void move_right(int xToMove = 1);
    void move_up(int yToMove = 1);
    void move_down(int yToMove = 1);
    void push_to_nextline();
    void use_backspace();
    void use_control();
    void use_shift();
    void export_text(std::string new_file_name);
    void export_text_anchors(std::string new_file_name);
    int find_all_strings(std::string strTarget, bool matchCase = true, bool addAnchor = false, std::string areaTitle = "");
    void find_mouse_cursor(int *mXCursor, int *mYCursor, gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    bool find_string(std::string strTarget, bool searchDown = true, bool matchCase = true, bool startAtFront = false);
    void find_documentation_description(int tCursorX = -1, int tCursorY = -1);
    void find_suggested_text();
    bool write_data_into_file(std::ofstream * fileTarget, int leftTabAmount = 0, bool useNewLines = true, bool useLastNewLine = false);
    bool write_short_data_into_file(std::ofstream * fileTarget, int leftTabAmount = 0, bool useNewLines = true, bool useLastNewLine = false);
    bool import_text(std::string new_file_name);
    void paste_clipboard();
    bool parse_code_javascript();
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void redo_edit();
    void reset_selection(int resetDirection = 0);
    void reset_self();
    void render_linebox( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_plain( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_code( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);


    int replace_all_found(std::string strToReplace, std::string newString);
    void revise_edit(int editId = -1);
    void save_edit();
    void select_all();
    void scroll_to_pos( int lineNumberIn, int characterNumberIn );
    void scroll_to_selection();
    void scroll_to_cursor();
    void set_read_only();
    void set_writeable();
    void set_string(std::string newString);
    void set_placeholder(std::string newString);
    void set_xcursor(int newPos);
    void set_ycursor(int newPos);
    void set_highlight_pos( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL );
    void setup_editor( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL );
    void undo_edit();
    void update_cursor_to_mouse( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
};

class GPE_WrappedTextArea: public GPE_TextAreaInputBasic
{
protected:
    std::string paragraphText;
public:
    GPE_WrappedTextArea();
    ~GPE_WrappedTextArea();
    void set_string( std::string strIn);
    void set_width(int newWidth);
    void set_height( int newHeight);
    void set_max_width( int nMW);
    void set_max_height( int nMH);
    void update_paragraph( int foundMaxWidth = -1 );
};



#endif
