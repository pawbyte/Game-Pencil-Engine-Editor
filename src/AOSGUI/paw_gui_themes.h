/*
paw_gui_themes.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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
#ifndef PAW_GUI_THEMES_H
#define PAW_GUI_THEMES_H

#include "../GPE_Engine/GPE_Engine.h"
#include "../GPE_Engine/GPE_Fonts.h"


//Default Fonts [ BEGIN ]

//The font that's going to be used
extern GPE_Font * font;
extern GPE_Font * textboxFont;
extern GPE_Font * FONT_BUTTONS_FONT;
extern GPE_Font * FONT_CATEGORY_BAR;
extern GPE_Font * GPE_DEFAULT_FONT;
extern GPE_Font * FONT_POPUP;
extern GPE_Font * GUI_TAB_FONT;
extern GPE_Font * FONT_TOOLBAR;
extern GPE_Font * FONT_RESOURCEBAR;
extern GPE_Font * FONT_TEXTINPUT;
extern GPE_Font * FONT_TEXTINPUT_COMMENT;
extern GPE_Font * FONT_TEXTINPUT_KEYWORD;
extern GPE_Font * FONT_TEXTINPUT_FUNCTION;
extern GPE_Font * FONT_TEXTINPUT_NUMBER;
extern GPE_Font * FONT_TEXTINPUT_QUOTE;
extern GPE_Font * FONT_TEXTINPUT_SYMBOL;
extern GPE_Font * FONT_TEXTINPUT_PROJECT_FUNCTION;
extern GPE_Font * FONT_TEXTINPUT_PROJECT_KEYWORD;

extern GPE_Font * FONT_TEXTINPUT_BOLD;
extern GPE_Font * FONT_TERM_NAME;
extern GPE_Font * FONT_TERM_DESCRIPTION;
extern GPE_Font * FONT_TERM_SCOPE;

extern GPE_Font * FONT_DEFAULT_PROMPT;
extern GPE_Font * FONT_CHECKBOX;

extern GPE_Font * FONT_HEADER;
extern GPE_Font * FONT_LABEL;
extern GPE_Font * FONT_LABEL_ANCHOR;
extern GPE_Font * FONT_LABEL_TITLE;
extern GPE_Font * FONT_PARAGRAGH;
extern GPE_Font * FONT_TOOLTIP;
extern GPE_Font * FONT_STATUSBAR;
extern GPE_Font * FONT_STREE_BRANCH;

//Default Fonts [ END ]

bool PAW_GUI_LOAD_FONTS();
void cleanup_fonts();

class GPE_Theme
{
private:
    std::vector<GPE_Color * > themeColors;
    bool nonDefaultTheme;
public:
    std::string themeName;
    std::string themeLocalLocation;
    std::string themeBgFileLocation;
    GPE_Texture * themeBgTexture;

    GPE_Animation * Main_Menu_Sprite;

    //main bg colors ( defaults everywhere)
    GPE_Color * Program_Color;
    GPE_Color * Program_Header_Color;
    GPE_Color * Main_Box_Color;
    GPE_Color * Main_Box_Highlighted_Color;
    GPE_Color * Main_Box_Font_Color;
    GPE_Color * Main_Box_Font_URL_Color;
    GPE_Color * Main_Box_Font_URL_Hovered_Color;
    GPE_Color * Main_Box_Font_URL_Visited_Color;
    GPE_Color * Main_Box_Font_Highlight_Color;
    GPE_Color * Main_Box_Faded_Font_Color;
    GPE_Color * Main_Box_Faded_Color;
    GPE_Color * Main_Border_Color;
    GPE_Color * Main_Border_Highlighted_Color;
    GPE_Color * Main_Box_Shadow_Color;
    //For Folders and such...
    GPE_Color * Main_Folder_Color;
    GPE_Color * Main_Folder_Highlighted_Color;
    //For Input Fields, Drop Down Menus & Resource Drop Downs
    GPE_Color * Input_Highlight_Color;
    GPE_Color * Input_Highlight_Outline_Color;
    GPE_Color * Input_Highlight_Alt_Color;
    GPE_Color * Input_Color;
    GPE_Color * Input_Error_Font_Color;
    GPE_Color * Input_Error_Box_Color;
    GPE_Color * Input_Error_Outline_Color;
    GPE_Color * Input_Outline_Color;
    GPE_Color * Input_Font_Color;
    GPE_Color * Input_Highlight_Font_Color;
    GPE_Color * Input_Faded_Font_Color;
    GPE_Color * Input_Selected_Color;
    //For ButtonBar and Iconed Buttons
    GPE_Color * Icon_Box_Color;
    GPE_Color * Icon_Box_Highlighted_Color;
    GPE_Color * Icon_Box_Selected_Color;
    GPE_Color * Icon_Border_Color;
    GPE_Color * Icon_Border_Highlighted_Color;
    GPE_Color * Icon_Border_Selected_Color;
    GPE_Color * Icon_Font_Color;
    GPE_Color * Icon_Font_Highlighted_Color;
    GPE_Color * Icon_Font_Selected_Color;
    //For Button and such...
    GPE_Color * Checkbox_Color;
    GPE_Color * Button_Box_Color;
    GPE_Color * Button_Box_Highlighted_Color;
    GPE_Color * Button_Box_Selected_Color;
    GPE_Color * Button_Box_Shadow_Color;
    GPE_Color * Button_Border_Color;
    GPE_Color * Button_Border_Highlighted_Color;
    GPE_Color * Button_Border_Selected_Color;
    GPE_Color * Button_Font_Color;
    GPE_Color * Button_Font_Highlighted_Color;
    //For Gui boxes, like main menu, toolbar and oontext menus
    GPE_Color * PopUp_Box_Highlight_Color;
    GPE_Color * PopUp_Box_Highlight_Alt_Color;
    GPE_Color * PopUp_Box_Close_Color;
    GPE_Color * PopUp_Box_Close_Hightlight_Color;
    GPE_Color * PopUp_Box_Close_Font_Color;
    GPE_Color * PopUp_Box_Color;
    GPE_Color * PopUp_Box_Border_Color;
    GPE_Color * PopUp_Box_Font_Color;
    GPE_Color * PopUp_Box_Highlight_Font_Color;
    GPE_Color * PopUp_Box_Faded_Font_Color;
    GPE_Color * PopUp_Box_Shadow_Color;
    //Scroll box colors
    GPE_Color * Scroll_Box_Color;
    GPE_Color * Scroll_Box_Border_Color;
    GPE_Color * Scroll_Box_Arrow_Color;
    GPE_Color * Scroll_Box_Camera_Color;
    GPE_Color * Scroll_Box_Camera_Highlight_Color;
    //for text boxes
    GPE_Color * Text_Box_Outer_Color;
    GPE_Color * Text_Box_Outer_Font_Color;
    GPE_Color * Text_Box_Color;
    GPE_Color * Text_Box_Outline_Color;
    GPE_Color * Text_Box_Highlight_Color;
    GPE_Color * Text_Box_Font_Color;
    GPE_Color * Text_Box_Font_Comment_Color;
    GPE_Color * Text_Box_Font_Highlight_Color;
    GPE_Color * Text_Box_Font_DataType_Color;
    GPE_Color * Text_Box_Font_DQuote_Color;
    GPE_Color * Text_Box_Font_Function_Color;
    GPE_Color * Text_Box_Font_Function_Alt_Color;
    GPE_Color * Text_Box_Font_Keyword_Color;
    GPE_Color * Text_Box_Font_Keyword_Alt_Color;
    GPE_Color * Text_Box_Font_JavaScript_Color;
    GPE_Color * Text_Box_Font_JavaScript_Alt_Color;
    GPE_Color * Text_Box_Font_Variable_Color;
    GPE_Color * Text_Box_Font_Variable_Alt_Color;
    GPE_Color * Text_Box_Font_Number_Color;
    GPE_Color * Text_Box_Font_SQuote_Color;
    GPE_Color * Text_Box_Font_Symbols_Color;
    GPE_Color * Text_Box_Project_Function_Color;
    GPE_Color * Text_Box_Project_Function_Alt_Color;
    GPE_Color * Text_Box_Project_Keyword_Color;
    GPE_Color * Text_Box_Project_Keyword_Alt_Color;
    //for errors
    GPE_Color * Main_Error_Font_Color;
    GPE_Color * Main_Warning_Font_Color;
    GPE_Color * Main_Suggestion_Font_Color;
    GPE_Theme(std::string name, bool isCustomTheme = false );
    ~GPE_Theme();
    GPE_Color * add_color(std::string name,Uint8 r, Uint8 g, Uint8 b);
    GPE_Color * add_color(std::string name, GPE_Color * savedColor );
    bool change_color(std::string name,Uint8 r, Uint8 g, Uint8 b);
    GPE_Color * find_color(std::string name);
    GPE_Color * get_color(int pos);
    int get_color_count();
    bool is_custom_theme();
    bool load_background( std::string bgTextureLocation);
    bool load_theme(std::string themeLocationIn);
    bool render_background(GPE_Rect * viewedSpace,GPE_Rect * cam );
    bool save_theme();
    bool save_theme_as(std::string themeLocationOut);
};

extern GPE_Theme * GPE_DEFAULT_TEMPLATE;
extern GPE_Theme * GPE_MAIN_THEME;

#endif
