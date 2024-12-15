/*
gpe_editor_project_helper.h
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

#ifndef gpe_editor_project_helper_h
#define gpe_editor_project_helper_h

#include <string>
#include "../gpe/gpe_constants.h"
#include "../gpe/gpe_file_system.h"
#include "../gpe/gpe_globals.h"
#include "../gpe/gpe.h"
#include "../pawgui/popups/pawgui_popups.h"
#include "gpe_editor_constants.h"
#include "../editor/gpe_editor_globals.h"

void setup_project_directory(std::string newProjectDir);
std::string setup_cpp_folder(std::string buildDirectory, int buildType, int buildBits = 32, bool inDebugMode = false );
std::string setup_js_folder(std::string buildDirectory, int buildType, int buildBits = 32, bool inDebugMode = false, int nativeBuildType = native_option_none );

#endif //gpe_editor_project_helper_h
