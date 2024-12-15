/*
pawgui_resource_dropdown.h
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

#ifndef PAWGUI_RESOURCE_DROPDOWN_H
#define PAWGUI_RESOURCE_DROPDOWN_H

#include "../base/pawgui_base.h"
#include "../context/pawgui_context.h"
#include "../general_resource/pawgui_general_resource_container.h"

namespace pawgui
{
    class widget_drop_down_resource_menu: public widget_basic
    {
        private:
            widget_resource_container * selectedResource;
            widget_resource_container * containerTree;
            int maxHeightToRender;
            popup_menu_option * sub_options;
            std::string dropdownName;
            std::string displayedResult;
            gpe::animation2d * selectedanimation;
            gpe::texture_base *selectedImage;
            int opId;
            bool isSelectable;
            int selectedId;
            bool isOpen;
            bool justActivated;
        public:
            widget_drop_down_resource_menu(  std::string name, widget_resource_container * cTree = nullptr,int id = -1,bool selectable = true);
            ~widget_drop_down_resource_menu();
            std::string get_data();
            void load_data(std::string datastring);
            std::string get_menu_option(int layerNumb, int atNumb);
            widget_resource_container * get_container_at(int layerNumb, int atNumb);
            widget_resource_container * get_selected_container();
            std::string get_plain_string();
            int get_selected_id();
            std::string get_selected_name();
            int get_menu_size();
            bool just_activated();
            //std::string get_name();
            void process_self( gpe::shape_rect * view_space=nullptr, gpe::shape_rect *cam=nullptr);
            void add_folder_contents(widget_resource_container * fFolder = nullptr, popup_menu_option * fOptionFolder = nullptr);
            void render_self( gpe::shape_rect * view_space=nullptr, gpe::shape_rect *cam=nullptr);
            void set_name(std::string new_name);
            void set_selected_target(std::string new_name);
            void set_selection(int new_id);
    };
}

#endif //PAWGUI_RESOURCE_DROPDOWN_H
