/*
gpe_log_manager.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#include "../pawgui/pawgui/pawgui.h"

class log_manager: public pawgui::widget_basic
{
    private:
        std::vector<pawgui::log_entry *> generalLog;
        pawgui::widget_text_editor * generalTextLog;

        std::vector<pawgui::log_entry *> debugLog;
        pawgui::widget_text_editor * debugTextLog;

        std::vector<pawgui::log_entry *> buildLog;
        pawgui::widget_text_editor * buildTextLog;
        pawgui::widget_text_editor * otherLog;
        pawgui::widget_text_editor * logToView;
        pawgui::widget_label_text  * emptyResultsLabel;
        pawgui::widget_tabbar * logTabs;
        pawgui::widget_scrollbar_xaxis * xScroll;
        pawgui::widget_scrollbar_yaxis * yScroll;
        bool showXLogScroll, showLogYScroll;
        int previousHeight;
        std::vector< gpe::key_pair * > defaultTabs;
    public:
        bool tabsAtBottom;
        int minLogHeight;
        bool is_visible;
        std::vector< pawgui::widget_text_anchor * > searchAnchors;
        pawgui::widget_panel_list * bottomInfoList;
        log_manager();
        ~log_manager();
        void add_default_tab(  std::string name,  bool open = false );
        void clear_defaults();
        void clear_search_anchors();
        void process_self( gpe::shape_rect * view_space=nullptr, gpe::shape_rect *cam=nullptr);
        void render_self( gpe::shape_rect * view_space=nullptr, gpe::shape_rect * cam = nullptr);
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

extern log_manager * main_editor_log;

#endif //GPE_EDITOR_LOGS_H
