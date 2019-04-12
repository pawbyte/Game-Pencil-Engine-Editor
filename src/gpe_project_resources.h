/*
gpe_project_resources.h
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

#ifndef GPE_PROJECT_RESOURCES_H
#define GPE_PROJECT_RESOURCES_H

#include "animation_resource.h"
#include "audio_resource.h"
#include "class_resource.h"
#include "font_resource.h"
#include "function_resource.h"
#include "light_resource.h"
#include "path_resource.h"
#include "particle_resource.h"
#include "video_resource.h"
#include "gpe_editor.h"
#include "gpe_editor_aboutpage.h"
#include "gpe_editor_helppage.h"
#include "gpe_editor_settings.h"
#include "gpe_editor_start_page.h"
#include "gpe_extra_tools.h"
#include "game_object_resource.h"
#include "game_scene_resource.h"

class GPE_Editor_State : public GPE_ProgramState
{
private:
    //Intro message
    GPE_Texture * message;
    GPE_Toolbar * mainToolBar;
    GPE_ToolIconButtonBar * mainButtonBar;
protected:
    //Intro background
    GPE_Texture *background;
public:
    GPE_Texture * country_language_image;
    SDL_version SDL_compiled_version;
    std::string SDL_VersionText;
    //Loads intro resources
    GPE_Editor_State();
    //Frees intro resources
    ~GPE_Editor_State();

    //Main loop functions
    void process_input();
    void apply_logic();
    void clean_up();
    void render();
};

#endif
