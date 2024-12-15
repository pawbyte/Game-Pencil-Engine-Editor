/*
gpe_editor_state.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef GPE_EDITOR_STATE_H
#define GPE_EDITOR_STATE_H


#include "gpe_editor.h"
#include "gpe_editor_settings.h"
#include "../project/project_properties.h"

#include "../log/gpe_log_manager.h"
#include "../resources/gpe/gpe_resource_tree.h"
#include "../project/gpe_project_resources.h"
#include "../gpe/gpe_program_state.h"
#include "../pawgui/pawgui/pawgui.h"

class gpe_editor_state : public gpe::program_state
{
protected:
    gpe::texture_base * message;
    pawgui::widget_toolbar * main_toolbar;
    pawgui::widget_button_iconbar * main_buttonbar;
    pawgui::widget_button_icon * editor_view_icon;

public:
    gpe::texture_base * country_language_image;
    //SDL_version SDL_compiled_version;
    std::string SDL_VersionText;
    //Loads intro resources
    gpe_editor_state( std::string s_name = "");
    //Frees intro resources
    ~gpe_editor_state();

    //Main loop functions
    void process_input();
    void process_view_mode();
    void apply_logic();
    void clean_up();
    void end_state();
    void render();
    void start_state();
};

bool init_gpe_editor( int argc, char* args[]);

#endif // GPE_EDITOR_STATE_H
