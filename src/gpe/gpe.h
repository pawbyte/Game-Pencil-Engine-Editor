/*
gpe.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef gpe_main_h
#define gpe_main_h

#include "gpe_artist_base.h"
#include "gpe_animation2d.h"
#include "gpe_basic_object_list.h"
#include "gpe_branch_factory.h"
#include "gpe_color.h"
#include "gpe_color_manager.h"
#include "gpe_error_logger.h"
#include "gpe_file_system.h"
#include "gpe_font_base.h"
#include "gpe_ini_file.h"
#include "gpe_layer.h"
#include "gpe_particles2d.h"
#include "gpe_parser.h"
#include "gpe_path2d.h"
#include "gpe_program_state.h"
#include "gpe_render_package.h"
#include "gpe_render_package_handler.h"
#include "gpe_runtime.h"
#include "gpe_scene.h"
#include "gpe_settings.h"

#include "internal_libs/semath.h"
#include "internal_libs/stg_ex.h"

#include <ctime>
#include <stdlib.h>

namespace gpe
{
    bool init_core_engine( int argc, char* args[] , std::string error_log_location );
    bool init_engine();
    bool quit_engine();
    bool quit_core_engine();
}

#endif //gpe_render_package

