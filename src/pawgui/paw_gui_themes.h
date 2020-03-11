/*
paw_gui_themes.cpp
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
#ifndef PAW_GUI_THEMES_H
#define PAW_GUI_THEMES_H

#include "../gpe/GPE.h"
#include "paw_gui_constants.h"
#include "paw_gui_globals.h"

const int GENERAL_GPE_GUI_PADDING = 16;
const int GPE_AVERAGE_LINE_HEIGHT = 20;
const int GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING = 32;
const int GPE_TITLE_BPADDING = 16;
const int TOOLKEY_SHORTCUT_WIDTH = 64;
const int GENERAL_ICON_WIDTH = 24;
const int GENERAL_THUMBNAIL_SIZE = 48;
const int GENERAL_ICON_WIDTH_AND_PADDING = 32;
const int GENERAL_PLUSMINUX_ICON_SIZE = 8;
const int TOOLKEY_OPTION_PADDING = 16;
const int TOOLKEY_ICON_X_PADDING = 4;
const int GENERAL_NEAR_ICON_YPADDING = 8;
const int GPE_MAX_RECENT_project_LIST_SIZE = 15;

//Default Fonts [ BEGIN ]

//The font that's going to be used
extern gpe::font_base * FONT_BUTTONS_FONT;
extern gpe::font_base * font_default;
extern gpe::font_base * FONT_POPUP;
extern gpe::font_base * GUI_TAB_FONT;
extern gpe::font_base * FONT_TOOLBAR;
extern gpe::font_base * FONT_RESOURCEBAR;
extern gpe::font_base * FONT_TEXTINPUT;
extern gpe::font_base * FONT_TEXTinput_COMMENT;
extern gpe::font_base * FONT_TEXTinput_KEYWORD;
extern gpe::font_base * FONT_TEXTinput_FUNCTION;
extern gpe::font_base * FONT_TEXTinput_NUMBER;
extern gpe::font_base * FONT_TEXTinput_QUOTE;
extern gpe::font_base * FONT_TEXTinput_SYMBOL;
extern gpe::font_base * FONT_TEXTinput_project_FUNCTION;
extern gpe::font_base * FONT_TEXTinput_project_keyword;

extern gpe::font_base * FONT_TEXTinput_BOLD;
extern gpe::font_base * FONT_TERM_NAME;
extern gpe::font_base * FONT_TERM_DESCRIPTION;
extern gpe::font_base * FONT_TERM_SCOPE;

extern gpe::font_base * FONT_DEFAULT_PROMPT;

extern gpe::font_base * FONT_HEADER;
extern gpe::font_base * FONT_LABEL;
extern gpe::font_base * FONT_LABEL_ANCHOR;
extern gpe::font_base * FONT_LABEL_TITLE;
extern gpe::font_base * FONT_LABEL;
extern gpe::font_base * FONT_TOOLTIP;
extern gpe::font_base * FONT_STATUSBAR;
extern gpe::font_base * FONT_STREE_BRANCH;

//Default Fonts [ END ]

bool PAW_GUI_LOAD_FONTS();
void cleanup_fonts();

class GPE_Theme
{
private:
    std::vector< gpe::color * > theme_colors;
    bool non_default_theme;
public:
    std::string theme_name;
    std::string theme_local_location;
    std::string themeBgFileLocation;
    gpe::texture_base * theme_texture_bg;

    gpe::animaton2d * main_menu_animation;

    //main bg gpe::colors ( defaults everywhere)
    gpe::color * program_color;
    gpe::color * program_color_header;

    gpe::color * main_box_color;
    gpe::color * main_box_highlight_color;
    gpe::color * main_box_font_color;
    gpe::color * main_box_font_url_color;
    gpe::color * main_box_font_url_hovered_color;
    gpe::color * main_box_font_url_visited_color;
    gpe::color * main_box_font_highlight_color;
    gpe::color * main_box_faded_font_color;
    gpe::color * main_box_faded_color;
    gpe::color * main_border_color;
    gpe::color * main_border_highlight_color;
    gpe::color * main_box_shadow_color;
    //For Folders and such...
    gpe::color * folder_color;
    gpe::color * folder_color_highlight;
    //For Input Fields, Drop Down Menus & Resource Drop Downs
    gpe::color * input_highlight_color;
    gpe::color * input_highlight_outline_color;
    gpe::color * input_highlight_alt_color;
    gpe::color * input_color;
    gpe::color * input_error_font_color;
    gpe::color * input_error_Box_color;
    gpe::color * input_error_outline_color;
    gpe::color * input_outline_color;
    gpe::color * input_font_color;
    gpe::color * input_highlight_font_color;
    gpe::color * input_faded_font_color;
    gpe::color * input_selected_color;
    //For ButtonBar and Iconed Buttons
    gpe::color * icon_box_color;
    gpe::color * icon_box_highlight_color;
    gpe::color * icon_box_selected_color;
    gpe::color * icon_border_color;
    gpe::color * icon_border_highlight_color;
    gpe::color * icon_border_selected_color;
    gpe::color * icon_font_color;
    gpe::color * icon_font_highlight_color;
    gpe::color * icon_font_selected_color;
    //For Button and such...
    gpe::color * checkbox_color;
    gpe::color * button_box_color;
    gpe::color * button_box_highlight_color;
    gpe::color * button_box_selected_color;
    gpe::color * button_box_shadow_color;
    gpe::color * button_border_color;
    gpe::color * button_border_highlight_color;
    gpe::color * button_border_selected_color;
    gpe::color * button_font_color;
    gpe::color * button_font_highlight_color;
    //For Gui boxes, like main menu, toolbar and oontext menus
    gpe::color * popup_box_highlight_color;
    gpe::color * popup_box_highlight_alt_color;
    gpe::color * popup_box_close_color;
    gpe::color * popup_box_close_hightlight_color;
    gpe::color * popup_box_close_font_color;
    gpe::color * popup_box_color;
    gpe::color * popup_box_border_color;
    gpe::color * popup_box_font_color;
    gpe::color * popup_box_highlight_font_color;
    gpe::color * popup_box_faded_font_color;
    gpe::color * popup_box_shadow_color;
    //Scroll box gpe::colors
    gpe::color * scroll_box_color;
    gpe::color * scroll_box_border_color;
    gpe::color * scroll_box_arrow_color;
    gpe::color * scroll_box_camera_color;
    gpe::color * scroll_box_camera_highlight_color;
    //for text boxes
    gpe::color * text_box_outer_color;
    gpe::color * text_box_outer_font_color;
    gpe::color * text_box_color;
    gpe::color * text_box_outline_color;
    gpe::color * text_box_highlight_color;
    gpe::color * text_box_font_color;
    gpe::color * text_box_font_comment_color;
    gpe::color * text_box_font_highlight_color;
    gpe::color * text_box_font_datatype_color;
    gpe::color * text_box_font_dquote_color;
    gpe::color * text_box_font_function_color;
    gpe::color * text_box_font_function_alt_color;
    gpe::color * text_box_font_keyword_color;
    gpe::color * text_box_font_keyword_alt_color;
    gpe::color * text_box_font_js_color;
    gpe::color * text_box_font_js_alt_color;
    gpe::color * text_box_font_variable_color;
    gpe::color * text_box_font_variable_alt_color;
    gpe::color * text_box_font_number_color;
    gpe::color * text_box_font_squote_color;
    gpe::color * text_box_font_symbols_color;
    gpe::color * text_box_project_function_color;
    gpe::color * text_box_project_function_alt_color;
    gpe::color * text_box_project_keyword_color;
    gpe::color * text_box_project_keyword_alt_color;
    //for errors
    gpe::color * main_error_font_color;
    gpe::color * main_warning_font_color;
    gpe::color * main_suggestion_font_color;
    GPE_Theme(std::string name, bool isCustomTheme = false );
    ~GPE_Theme();
    gpe::color * add_color(std::string name,uint8_t r, uint8_t g, uint8_t b);
    gpe::color * add_color(std::string name, gpe::color * saved_color );
    bool change_color(std::string name,uint8_t r, uint8_t g, uint8_t b);
    gpe::color * find_color(std::string name);
    gpe::color * get_color(int pos);
    int get_color_count();
    bool is_custom_theme();
    bool load_background( std::string bgTextureLocation);
    bool load_theme(std::string themeLocationIn);
    bool render_background( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam );
    bool save_theme();
    bool save_theme_as(std::string themeLocationOut);
};

extern GPE_Theme * theme_default;
extern GPE_Theme * theme_main;

#endif
