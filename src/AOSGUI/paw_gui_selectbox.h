/*
paw_gui_selectbox.h
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
#ifndef PAW_GUI_SELECTBOX_H
#define PAW_GUI_SELECTBOX_H

#include "paw_gui_base.h"
#include "paw_gui_parsers.h"
#include "paw_gui_scrollbars.h"


class GPE_SelectBoxBasic_Option
{
public:
    bool sectionHasContent;
    bool useGuiColor;
    bool isChecked;
    std::string optionName;
    GPE_Texture * optionTexture;
    GPE_Animation * optionSprite;
    int subimageIndex;
    double optionValue;
    GPE_SelectBoxBasic_Option();
    ~GPE_SelectBoxBasic_Option();
    void prerender_self( );
};

class GPE_SelectBoxBasic: public GPE_GeneralGuiElement
{
private:
    std::vector <GPE_SelectBoxBasic_Option * > subOptions;
    int intedPos;
    double pos;
    double startPos;
    double maxOptionsInView;
    int maxHeight;
    int optionHeight;
    bool showHideOthersCheckboxToggle;
    GPE_ScrollBar_YAxis * optionsScroller;
public:
    int downDelay;
    int upDelay;
    bool showCheckboxes;
    int optionIconWidth;
    GPE_SelectBoxBasic(std::string name = "");
    ~GPE_SelectBoxBasic();
    std::string get_data();
    void load_data(std::string dataString);
    void add_option(std::string newOptionName, double newOpValue = -1,GPE_Texture * evRepIcon = NULL, GPE_Animation * evRepSprite=NULL, int subimageInIndex = 0, bool selectNew = false, bool useGuiColor = true);
    void correct_camera();
    void clear_list();
    std::string get_selected_name();
    double get_selected_value();
    int get_selection();
    int get_size();
    GPE_SelectBoxBasic_Option * get_option(int optionId);
    std::string get_option_name(int optionId);
    void insert_option(int optionId, std::string newName, GPE_Texture * evRepIcon = NULL, GPE_Animation * evRepSprite=NULL, int subimageInIndex = 0,bool selectNew = false);
    void limit_height(int newH);
    void alter_content( int optionId, bool sectionHasContent);
    void alter_content_from_value( double valueId, bool sectionHasContent);
    bool move_down_space();
    bool move_up_space();
    void prerender_self( );
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void rename_option(int optionId, std::string newName);
    void remove_option(int optionId);
    void set_option_height( int newOptionHeight);
    void set_selected_option( std::string optionToSelect);
    void set_selection( int selId);
    void toggle_others_checked();
};

#endif
