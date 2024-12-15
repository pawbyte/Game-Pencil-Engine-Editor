/*
pawgui_themes.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/

#ifndef PAWGUI_THEMES_H
#define PAWGUI_THEMES_H

#include "../gpe/gpe.h"
#include "../constants/pawgui_constants.h"
#include "../globals/pawgui_globals.h"

namespace pawgui
{
    const int padding_default = 16;
    const int default_line_height = 20;
    const int default_line_height_padded = 32;
    const int default_padding_title = 16;
    const int default_toolkey_shortcut_width = 64;
    const int default_icon_width = 32;
    const int default_thumbnal_size = 48;
    const int default_icon_width_padded = 32;
    const int default_icon_plusminus_size = 8;
    const int default_toolkey_padding = 16;
    const int default_toolkey_iconx_padding = 4;

    //Default Fonts [ BEGIN ]

    //The font that's going to be used
    extern gpe::font_base * font_buttons;
    extern gpe::font_base * font_popup;
    extern gpe::font_base * font_tab;
    extern gpe::font_base * font_toolbar;
    extern gpe::font_base * font_resourcebar;
    extern gpe::font_base * font_textinput;
    extern gpe::font_base * font_textinput_comment;
    extern gpe::font_base * font_textinput_keyword;
    extern gpe::font_base * font_textinput_function;
    extern gpe::font_base * font_textinput_number;
    extern gpe::font_base * font_textinput_quote;
    extern gpe::font_base * font_textinput_symbol;
    extern gpe::font_base * font_textinput_project_function;
    extern gpe::font_base * FONT_TEXTinput_project_keyword;

    extern gpe::font_base * font_term_name;
    extern gpe::font_base * font_term_description;
    extern gpe::font_base * font_term_scope;

    extern gpe::font_base * font_default_prompt;

    extern gpe::font_base * FONT_HEADER;
    extern gpe::font_base * FONT_LABEL;
    extern gpe::font_base * FONT_LABEL_ANCHOR;
    extern gpe::font_base * FONT_LABEL_TITLE;
    extern gpe::font_base * FONT_LABEL;
    extern gpe::font_base * FONT_TOOLTIP;
    extern gpe::font_base * FONT_STATUSBAR;
    extern gpe::font_base * FONT_STREE_BRANCH;

    //Default Fonts [ END ]

    bool load_default_fonts( std::string mono_font_location, int font_min_size = 12);
    void cleanup_fonts();

    class gui_theme
    {
        private:
            std::vector< gpe::color * > theme_colors;
            bool non_default_theme;
        public:
            std::string theme_name;
            std::string theme_iconpack;
            std::string theme_local_location;
            std::string theme_bg_location;
            gpe::texture_base * theme_texture_bg;

            gpe::animation2d * main_menu_animation;

            //main bg gpe::colors ( defaults everywhere)
            gpe::color * program_color;
            gpe::color * program_color_header;

            gpe::color * panel_color;
            gpe::color * panel_border_color;

            gpe::color * tab_background_color;
            gpe::color * tab_color;
            gpe::color * tab_hover_color;
            gpe::color * tab_selected_color;
            gpe::color * tab_text_color;

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
            //For _buttonBar and Iconed _buttons
            gpe::color * icon_box_color;
            gpe::color * icon_box_highlight_color;
            gpe::color * icon_box_selected_color;
            gpe::color * icon_border_color;
            gpe::color * icon_border_highlight_color;
            gpe::color * icon_border_selected_color;
            gpe::color * icon_font_color;
            gpe::color * icon_font_highlight_color;
            gpe::color * icon_font_selected_color;
            //For _button and such...
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
            gui_theme(std::string name, bool isCustomTheme = false );
            ~gui_theme();
            gpe::color * add_color(std::string name,uint8_t r, uint8_t g, uint8_t b);
            gpe::color * add_color(std::string name, gpe::color * saved_color );
            bool change_color(std::string name,uint8_t r, uint8_t g, uint8_t b);
            gpe::color * find_color(std::string name);
            gpe::color * get_color(int pos);
            int get_color_count();
            bool is_custom_theme();
            bool load_background( std::string bg_textureLocation);
            bool load_theme(std::string themeLocationIn);
            bool render_background( gpe::shape_rect * view_space, gpe::shape_rect * cam );
            bool save_theme();
            bool save_theme_as(std::string themeLocationOut);
    };

    extern gui_theme * theme_default;
    extern gui_theme * theme_system_based;
    extern gui_theme * theme_main;
}
#endif //PAWGUI_THEMES_H
