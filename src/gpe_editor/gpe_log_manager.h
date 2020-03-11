/*
gpe_log_manager.h
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

#ifndef GPE_EDITOR_LOGS_H
#define GPE_EDITOR_LOGS_H

#include "../pawgui/paw_gui.h"


//Glorious
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
    GPE_TabBar * logTabs;
    GPE_ScrollBar_XAxis * xScroll;
    GPE_ScrollBar_YAxis * yScroll;
    bool showXLogScroll, showLogYScroll;
    int previousHeight;
    std::vector< gpe::key_pair * > defaultTabs;
public:
    bool tabsAtBottom;
    int minLogHeight;
    bool isVisible;
    std::vector< GPE_TextAnchor * > searchAnchors;
    GPE_GuiElementList * bottomInfoList;
    GPE_LogManager();
    ~GPE_LogManager();
    void add_default_tab(  std::string name,  bool open = false );
    void clear_defaults();
    void clear_search_anchors();
    void process_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect *cam=NULL);
    void render_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect * cam = NULL);
    void clear_all_logs();
    void clear_debug_log();
    void clear_build_log();
    void clear_general_log();

    void handle_error_log();

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
    void process_anchors();
    void remove_default_tab( std::string name );
    void toggle_manager();
};

extern GPE_LogManager * GPE_main_Logs;

#endif
