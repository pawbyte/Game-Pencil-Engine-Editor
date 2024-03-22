/*
pawgui.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PAWGUI contributors ( Contributors Page )

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

-PAWGUI <https://www.pawbyte.com/pawgui>


*/

#ifndef PAWGUI_LIBRARY_H
#define PAWGUI_LIBRARY_H

#include "pawgui_base.h"
#include "pawgui_buttons.h"
#include "pawgui_checkbox.h"
#include "pawgui_color_picker.h"
#include "pawgui_content_selector.h"
#include "pawgui_context.h"
#include "pawgui_dropdown.h"
#include "pawgui_file_popups.h"
#include "pawgui_globals.h"
#include "pawgui_labels.h"
#include "pawgui_notifications.h"
#include "pawgui_overlay.h"
#include "pawgui_panel_list.h"
#include "pawgui_parsers.h"
#include "pawgui_popups.h"
#include "pawgui_radiobuttons.h"
#include "pawgui_resource_dropdown.h"
#include "pawgui_scrollbars.h"
#include "pawgui_search_controller.h"
#include "pawgui_selectbox.h"
#include "pawgui_sliders.h"
#include "pawgui_statusbar.h"
#include "pawgui_stree.h"
#include "pawgui_tab_basic.h"
#include "pawgui_tab_manager.h"
#include "pawgui_text_editor.h"
#include "pawgui_text_input.h"
#include "pawgui_text_url.h"
#include "pawgui_theme_controller.h"
#include "pawgui_themes.h"


namespace pawgui
{
    bool init_gui( std::string mono_font_location, int font_min_size );
    bool quit_gui();
}

#endif //PAWGUI_LIBRARY_H
