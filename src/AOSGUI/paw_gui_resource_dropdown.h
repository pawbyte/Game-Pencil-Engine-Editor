/*
paw_gui_resource_dropdown.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef RESOURCE_DROPDOWN_H
#define RESOURCE_DROPDOWN_H


#include "paw_gui_base.h"
#include "ambitious_gui_library.h"
#include "paw_gui_context.h"
#include "paw_gui_general_resource_container.h"

class GPE_DropDown_Resouce_Menu: public GPE_GeneralGuiElement
{
private:
    GPE_GeneralResourceContainer * selectedResource;
    GPE_GeneralResourceContainer * containerTree;
    int maxHeightToRender;
    GPE_PopUpMenu_Option * subOptions;
    std::string dropdownName;
    std::string displayedResult;
    GPE_Animation * selectedanimation;
    GPE_Texture_Base *selectedImage;
    int opId;
    bool isSelectable;
    int selectedId;
    bool isOpen;
    bool justActivated;
public:
    GPE_DropDown_Resouce_Menu(  std::string name, GPE_GeneralResourceContainer * cTree = NULL,int id = -1,bool selectable = true);
    ~GPE_DropDown_Resouce_Menu();
    std::string get_data();
    void load_data(std::string dataString);
    std::string get_menu_option(int layerNumb, int atNumb);
    GPE_GeneralResourceContainer * get_container_at(int layerNumb, int atNumb);
    GPE_GeneralResourceContainer * get_selected_container();
    std::string get_plain_string();
    int get_selected_id();
    std::string get_selected_name();
    int get_menu_size();
    bool just_activated();
    //std::string get_name();
    void process_self(GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL);
    void add_folder_contents(GPE_GeneralResourceContainer * fFolder = NULL, GPE_PopUpMenu_Option * fOptionFolder = NULL);
    void render_self( GPE_Rect * viewedSpace=NULL, GPE_Rect *cam=NULL);
    void set_name(std::string newName);
    void set_selected_target(std::string newName);
    void set_selection(int newId);
};


#endif
