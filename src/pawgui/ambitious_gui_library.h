/*
ambitious_gui_library.h
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

#ifndef AMBITIUOS_GUI_LIBRARY_H
#define AMBITIUOS_GUI_LIBRARY_H

#include "paw_gui_base.h"
#include "paw_gui_scrollbars.h"
#include "paw_gui_sliders.h"
#include "paw_gui_panel_list.h"
#include "paw_gui_popups.h"
#include "paw_gui_tab_basic.h"

//Once I remove the other files that require parsers remove the include for parsers
#include "paw_gui_parsers.h"
#include "paw_gui_general_resource.h"

extern gpe::texture_base * GPE_LOGO;
extern gpe::texture_base * GPE_TEXTURE_TRANSPARENT_BG;


class GPE_StatusBar: public GPE_GeneralGuiElement
{
private:
    std::string codeEditorStatusBarString;
    std::string insertModeString;
public:
    std::string statusBarLeftText;
    GPE_StatusBar();
    ~GPE_StatusBar();
    void process_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect *cam=NULL);
    void render_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect * cam = NULL);
    void reset_statusbar();
    void set_codestring(std::string newCodeString);
    void set_insertmode(std::string newInsertMode);
};

extern GPE_StatusBar * GPE_main_Statusbar;

class GPE_TextURL: public GPE_GeneralGuiElement
{
public:
    bool wasClicked;
    std::string webUrl;
    std::string webId;
    GPE_TextURL(std::string name_param="", std::string description="", std::string urlIn="" );
    ~GPE_TextURL();
    std::string get_data();
    std::string get_plain_string();
    void load_data(std::string dataString);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void set_name(std::string name_param);
};

#endif
