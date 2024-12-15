/*
pawgui_radiobuttons.h
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

#ifndef PAWGUI_RADIOBUTTONS_H
#define PAWGUI_RADIOBUTTONS_H

#include "../base/pawgui_base.h"
#include "../parsers/pawgui_parsers.h"

namespace pawgui
{
    class widget_radio_button_controller: public widget_basic
    {
        private:
            int selectedId;
            int hoveredOption;
            int opXPadding, opYPadding;
            int opWidth, opHeight;
            bool keepAlphabetized;
            int numCols;
            int rowsPerCol;
        protected:
            std::vector <gpe::key_pair *> sub_options;
        public:
            bool allowDuplicates;
            widget_radio_button_controller(std::string cName = "", bool alphabetize = false, int colNum=1);
            ~widget_radio_button_controller();
            std::string get_data();
            void load_data(std::string datastring);
            void add_opton(std::string newOption);
            void add_menu_option(std::string optionName, std::string optionSubStr= "",int optionValue = 0, bool selectOption = false);
            std::string get_menu_option(int atNumb);
            std::string get_plain_string();
            int get_selected_id();
            std::string get_selected_tag();
            int get_selected_value();

            void organize_options();
            void remove_data(std::string datastring);
            void remove_option(std::string optionToCut);
            void remove_option_id(int optionToCut);
            void set_selection( int newSelection);
            bool set_from_tag(std::string newselectedTag);
            void set_value( int valueToSelect);
            void prerender_self( );
            void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
            void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    };
}

#endif //PAWGUI_RADIOBUTTONS_H
