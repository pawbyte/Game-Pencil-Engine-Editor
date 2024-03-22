/*
gpe_editor_globals.h
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

#ifndef gpe_editor_globals_h
#define gpe_editor_globals_h

#include "../pawgui/pawgui_theme_controller.h"
#include "../pawgui/pawgui_dock_system.h"
#include "gpe_editor_splash_page.h"

extern int release_current_mode;
extern bool release_is_lts_version;
extern pawgui::themes_controller * editor_theme_controller;

extern pawgui::widget_dock * gpe_dock;
extern pawgui::widget_dock_panel * panel_center_area;
extern pawgui::widget_dock_panel * panel_resource_tree;
extern pawgui::widget_dock_panel * panel_main_editor;
extern pawgui::widget_dock_panel * panel_inspector;
extern pawgui::widget_dock_panel * panel_meta;
extern pawgui::widget_dropdown_menu * editor_view_mode;

#endif // gpe_editor_globals_hpawgui::
