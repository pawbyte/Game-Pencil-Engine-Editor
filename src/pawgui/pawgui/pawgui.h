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

#include "../base/pawgui_base.h"
#include "../buttons/pawgui_buttons.h"
#include "../checkbox/pawgui_checkbox.h"
#include "../color_picker/pawgui_color_picker.h"
#include "../context_selector/pawgui_content_selector.h"
#include "../context/pawgui_context.h"
#include "../dropdown/pawgui_dropdown.h"
#include "../file_popups/pawgui_file_popups.h"
#include "../globals/pawgui_globals.h"
#include "../labels/pawgui_labels.h"
#include "../notifications/pawgui_notifications.h"
#include "../overlay/pawgui_overlay.h"
#include "../panel_list/pawgui_panel_list.h"
#include "../parsers/pawgui_parsers.h"
#include "../popups/pawgui_popups.h"
#include "../radio_buttons/pawgui_radiobuttons.h"
#include "../resource_dropdown/pawgui_resource_dropdown.h"
#include "../scrollbars/pawgui_scrollbars.h"
#include "../search_controller/pawgui_search_controller.h"
#include "../select_box/pawgui_selectbox.h"
#include "../sliders/pawgui_sliders.h"
#include "../statusbar/pawgui_statusbar.h"
#include "../stree/pawgui_stree.h"
#include "../tab_basic/pawgui_tab_basic.h"
#include "../tab_manager/pawgui_tab_manager.h"
#include "../text_editor/pawgui_text_editor.h"
#include "../text_input/pawgui_text_input.h"
#include "../text_url/pawgui_text_url.h"
#include "../theme_controller/pawgui_theme_controller.h"
#include "../themes/pawgui_themes.h"


namespace pawgui
{
    bool init_gui( std::string mono_font_location, int font_min_size );
    bool quit_gui();
}

#endif //PAWGUI_LIBRARY_H
