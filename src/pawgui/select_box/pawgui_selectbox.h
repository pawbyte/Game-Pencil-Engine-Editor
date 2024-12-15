/*
pawgui_selectbox.h
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
#ifndef PAWGUI_SELECTBOX_H
#define PAWGUI_SELECTBOX_H

#include "../base/pawgui_base.h"
#include "../parsers/pawgui_parsers.h"
#include "../scrollbars/pawgui_scrollbars.h"

namespace pawgui
{
    class selectbox_option
    {
        public:
            bool sectionHasContent;
            bool useGuiColor;
            bool isClicked;
            std::string optionName;
            gpe::texture_base * optionTexture;
            gpe::animation2d * optionanimation;
            int subimageIndex;
            float optionValue;
            selectbox_option();
            ~selectbox_option();
            void prerender_self( );
    };

    class widget_selectbox: public widget_basic
    {
        private:
            std::vector <selectbox_option * > sub_options;
            int intedPos;
            float pos;
            float startPos;
            float maxOptionsInView;
            int maxHeight;
            int optionHeight;
            bool showHideOthersCheckboxToggle;
            widget_scrollbar_yaxis * optionsScroller;
        public:
            int downDelay;
            int upDelay;
            bool showCheckboxes;
            int optionIconWidth;
            widget_selectbox(std::string name = "");
            ~widget_selectbox();
            std::string get_data();
            void load_data(std::string datastring);
            void add_option(std::string newOptionName, float newOpValue = -1,gpe::texture_base * evRepIcon = nullptr, gpe::animation2d * evRepanimation=nullptr, int subimageInIndex = 0, bool selectNew = false, bool useGuiColor = true);
            void correct_camera();
            void clear_list();
            int get_option_id(std::string optionName );
            std::string get_selected_name();
            float get_selected_value();
            int get_selection();
            int get_size();
            selectbox_option * get_option(int optionId);
            std::string get_option_name(int optionId);
            void insert_option(int optionId, std::string new_name, gpe::texture_base * evRepIcon = nullptr, gpe::animation2d * evRepanimation=nullptr, int subimageInIndex = 0,bool selectNew = false);
            void limit_height(int newH);
            void alter_content( int optionId, bool sectionHasContent);
            void alter_content_from_value( float valueId, bool sectionHasContent);
            bool move_down_space();
            bool move_up_space();
            void prerender_self( );
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void rename_option(int optionId, std::string new_name);
            void remove_option(int optionId);
            void set_option_height( int newOptionHeight);
            void set_selected_option( std::string optionToSelect);
            void set_selection( int selId);
            void toggle_others_checked();
    };
}
#endif //PAWGUI_SELECTBOX_H
