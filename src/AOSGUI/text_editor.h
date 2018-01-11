/*
text_editor.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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
#ifndef PAW_TEXT_EDITOR_H
#define PAW_TEXT_EDITOR_H
#include "gpe_syntax_handler.h"
#include "ambitious_gui_library.h"


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

class GPE_TextAreaInputBasic: public GPE_GeneralGuiElement
{
    protected:
        float editorZoomLevel;
        float defaultLineHeight;
        bool undoableActionOccurred;
        time_t lastUndoableActionTime;
        int lastDoubleClickAction;
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
        int prevLinesWithinView;
        int lineStartXPos;
        int charactersWithinView;
        int prevCharactersWithinView;
        int lineStartYPos;

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
        GPE_Font * editorFont;
        std::string placeHolderString;
        std::string textInputString;
        std::string lineToEdit;
        std::vector <std::string> listOfStrings;
        std::vector < GPE_TextAnchor *> anchorPositions;
        GPE_ScrollBar_XAxis * textXScroll;
        GPE_ScrollBar_YAxis * textYScroll;
        GPE_Rect textSpaceRect;
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
        GPE_Rect renderBox;
    public:
        int parseForErrorsTimerGoal;
        int parseForErrorsTimerPos;
        bool missingSymbolDetected;
        int missingSymbolStartYPos;
        int missingSymbolStartXPos;
        std::vector<std::string> foundSyntaxErrors;
        int misingSymbolSuggestedEndYPos;
        int misingSymbolSuggestedEndXPos;
        //Used for code highlights and suggestions.
        std::vector <GPE_Compiler_Term *> suggestedCompilerTerms;
        int highlightXPos, highlightYPos;
        bool documentationIsBeingShown;
        bool codeBeingSuggested;
        int iSuggestedStartPos;
        int iSuggestionPos;
        int suggestedTextMaxInViewCount;
        int maxSuggestedTextWidth;
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
        GPE_TextAreaInputBasic(int newX, int newY , int boxW=-1, int boxH=-1,std::string startName="",std::string placeHolderText="", bool saveFirstEdit = false);
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
        int get_renderbox_height();
        int get_xcursor();
        int get_ycursor();
        std::string get_line_string(int lineNumberIn);
        std::string get_short_hightlighted();
        std::string get_string();
        std::string get_string_spaces();
        std::vector <std::string> get_all_strings();
        void handle_scrolling();
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
        void export_text(std::string newFileName);
        void export_text_anchors(std::string newFileName);
        int find_all_strings(std::string strTarget, bool matchCase = true, bool addAnchor = false, std::string areaTitle = "");
        void find_mouse_cursor(int *mXCursor, int *mYCursor, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        bool find_string(std::string strTarget, bool searchDown = true, bool matchCase = true, bool startAtFront = false);
        void find_documentation_description(int tCursorX = -1, int tCursorY = -1);
        void find_suggested_text();
        bool write_data_into_file(std::ofstream * fileTarget, int leftTabAmount = 0, bool useNewLines = true, bool useLastNewLine = false);
        bool write_short_data_into_file(std::ofstream * fileTarget, int leftTabAmount = 0, bool useNewLines = true, bool useLastNewLine = false);
        bool import_text(std::string newFileName);
        void paste_clipboard();
        bool parse_code_javascript();
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void redo_edit();
        void reset_selection(int resetDirection = 0);
        void reset_self();
        void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = false);
        void render_code( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);

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
        void undo_edit();
        void update_cursor_to_mouse(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
};

class GPE_WrappedTextArea: public GPE_GeneralGuiElement
{
    protected:
        std::string paragraphText;
    public:
        GPE_TextAreaInputBasic * heldTextArea;
        GPE_WrappedTextArea();
        ~GPE_WrappedTextArea();
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void set_string( std::string strIn);
        void set_width(int newWidth);
        void set_height( int newHeight);
        void set_max_width( int nMW);
        void set_max_height( int nMH);
        void update_paragraph( int foundMaxWidth = -1 );
};

class GPE_LogManager: public GPE_GeneralGuiElement
{
    private:
        std::vector<GPE_Log_Entry *> generalLog;
        GPE_TextAreaInputBasic * generalTextLog;

        std::vector<GPE_Log_Entry *> debugLog;
        GPE_TextAreaInputBasic * debugTextLog;

        std::vector<GPE_Log_Entry *> buildLog;
        GPE_TextAreaInputBasic * buildTextLog;
        GPE_TextAreaInputBasic * otherLog;
        GPE_TextAreaInputBasic * logToView;
        GPE_Label_Text * emptyResultsLabel;
        GPE_GuiElementList * bottomInfoList;
        int seekedY1Pos;
        GPE_TabBar * logTabs;
        GPE_ScrollBar_XAxis * xScroll;
        GPE_ScrollBar_YAxis * yScroll;
        bool showXLogScroll, showLogYScroll;
    public:
        int minLogHeight;
        bool isVisible;
        bool menuResized;
        bool beingResized;
        bool justResized;
        std::vector< GPE_TextAnchor * > searchAnchors;
        GPE_LogManager();
        ~GPE_LogManager();
        void clear_search_anchors();
        void process_self(GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL);
        void render_self( GPE_Rect * viewedSpace=NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        bool being_resized();
        void clear_all_logs();
        void clear_debug_log();
        void clear_build_log();
        void clear_general_log();
        void log_general_line(std::string newLogLine);

        void log_general_comment(std::string newLogLine);
        void log_general_warning(std::string newLogLine);
        void log_general_error(std::string newLogLine);

        void log_debug_line(std::string newLogLine);
        //The core 3 special messages to show for build problems and suggestions
        void log_build_line(std::string newLogLine);
        void log_build_comment(std::string newLogLine);
        void log_build_warning(std::string newLogLine);
        void log_build_error(std::string newLogLine);

        void log_other_line(std::string newLogLine);
        void open_general_log();
        void open_build_log();
        void open_other_log();
        void open_replace_mode();
        void open_search_mode();
        void open_search_results();
};

extern GPE_LogManager * GPE_Main_Logs;
#endif
