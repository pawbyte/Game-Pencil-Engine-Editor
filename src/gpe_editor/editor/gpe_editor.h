/*
gpe_editor.h
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

#ifndef GPE_EDITOR_H
#define GPE_EDITOR_H

#include "../pawgui/pawgui/pawgui.h"
#include "../pawgui/resource_dropdown/pawgui_resource_dropdown.h"
#include "../pawgui/dock_system/pawgui_dock_system.h"

#include "gpe_editor_constants.h"
#include "../editor/gpe_editor_globals.h"
#include "gpe_editor_project.h"
#include "gpe_editor_project_helper.h"
#include "../log/gpe_log_manager.h"
#include "../resources/gpe/gpe_resource_tree.h"

extern std::string GPE_BUILD_NAMES[ gpe::system_os_max ];

extern pawgui::popup_menu_option * main_TOOLBAR_RECENT_PROJECTS;

class GPE_Gui_Engine
{
private:
    std::vector<std::string> gpeRecentProjects;
    std::vector<std::string> gpeTips;
public:
    bool includeNintendoSwitchExport;
    bool includePlaystation4Export;
    bool includeXboxOneExport;
    int release_current_mode;
    bool showStartPageAtStartUp;
    bool showTipsAtStartUp;
    std::string fileOpenProjectDir;
    std::string fileOpenProjectIconDir;
    std::string fileSaveProjectDir;

    std::vector<GPE_ProjectFolder * > gpeProjects;
    pawgui::widget_toolbar * main_toolbar;
    pawgui::widget_notfications_holder * main_notification_holder;

    GPE_ResourceTree * mainResourceTree;
    GPE_Gui_Engine();
    ~GPE_Gui_Engine();
    void add_to_recent_project_list(std::string newProjectFileName, bool saveData = false);
    void apply_logic();
    void clean_current_project_build_folder(int buildMetaTemplate);
    void clear_recent_project_list();
    void close_project(std::string projectFileName );
    void dock_reset();
    GPE_ProjectFolder * find_project_from_name(std::string projectName);
    GPE_ProjectFolder * find_project_from_filename(std::string projectFileName);
    int find_project_id_from_name(std::string projectName, int ignoreId = -1);
    int find_project_id_from_filename(std::string projectFileName, int ignoreId = -1);
    int get_recent_project_list_size();
    int get_tip_count();
    int get_random_tip();
    int get_previous_tip(int tipId);
    int get_next_tip(int tipId);
    std::string get_recent_project_name(int pId);
    std::string get_tip(int tipId);

    GPE_ResourceTree  * init_resourcebar();
    pawgui::widget_toolbar * init_toolbar();
    pawgui::widget_toolbar * init_toolbar(std::string bName, gpe::shape_rect bRect);
    void launch_new_project();
    void load_tips();
    void load_settings();
    pawgui::widget_toolbar * get_main_toolbar();
    pawgui::widget_button_iconbar * get_main_buttonbar();
    void open_project(std::string projName);
    void open_new_project();
    void prerender_gui( );
    void process_overlay_message();
    void process_window_title();

    void render_foreground_engine();
    void render_gui_info();
    void remove_project(std::string projectFileName );
    void reset_gui_info();
    void reset_settings();
    void rum_current_project(std::string projectBuildDirectory, int buildMetaTemplate);

    void save_all_projects();
    void save_current_project();
    void save_settings();
    void set_main_toolbar(pawgui::widget_toolbar * newToolbar);
    void take_live_screenshor();
    void update_recent_project_list(bool saveData);
};

extern GPE_Gui_Engine * editor_gui_main;

bool set_current_gpe_project( GPE_ProjectFolder * newMainProject = nullptr);
bool set_current_gpe_project_from_name( std::string newMainProjectName );

void GPE_Show_Tip_Of_Day();
void init_gpe_editor_globals();
bool quit_gpe_editor();

#endif
