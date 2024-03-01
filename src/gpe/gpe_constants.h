/*
gpe_constants.h
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
#ifndef gpe_constants_h
#define gpe_constants_h

namespace gpe
{
    enum class render_system
    {
        render_sys_sdl = 3,
        render_sys_opengl = 3,
        render_sys_directx = 3,
        render_sys_vulkan = 3,
        render_sys_other = 4,
    };

    //Game Pencil Runtime
    const float version_number_major = 1;
    const float version_number_minor = 6;
    const float version_number_update = 0;
    const float version_number_total = version_number_major + version_number_minor/10.f + version_number_update/100.f;
    const bool version_is_lts = true;

    const int is_null = -1;

    const int fa_left = 0;
    const int fa_center = 1;
    const int fa_right = 2;

    const int fa_error = -1;

    const int fa_top = 0;
    const int fa_middle = 1;
    const int fa_bottom = 2;

    const int system_os_html5 = 0;
    const int system_os_windows = 1;
    const int system_os_mac = 2;
    const int system_os_linux = 3;
    const int system_os_android = 4;
    const int system_os_ios = 5;

    const int system_os_xboxone = 6;
    const int system_os_ps4 = 7;

    const int system_os_switch = 8;
    const int system_os_xbox_series_x = 9;
    const int system_os_ps5 = 10;

    const int system_os_max = 20;

    const int anim_frame_count = 0;
    const int anim_frame_width = 1;
    const int anim_frame_height = 2;
    const int anim_pixel_offset_hori = 3;
    const int anim_pixel_offset_vert = 4;
    const int anim_pixel_padding_hori = 5;
    const int anim_pixel_padding_vert = 6;

    const int resource_type_project_settings = 1;
    const int resource_type_animation = 2;
    const int resource_type_texture = 3;
    const int resource_type_tilesheet = 4;
    const int resource_type_audio = 5;
    const int resource_type_video = 6;
    const int resource_type_function = 7;
    const int resource_type_object = 8;
    const int resource_type_scene = 9;
    const int resource_type_class = 10;
    const int resource_type_path = 11;
    const int resource_type_font = 12;

    const int resource_type_emitter = 13;
    const int resource_type_light = 14;

    //15 thru 20 not looked yet
    const int resource_type_plugin = 15;
    const int resource_type_dictionary = 16;
    const int resource_type_spreadsheet = 17;
    const int resource_type_event = 18;
    const int resource_type_quest = 19;
    const int resource_type_achievement = 20;
    const int resource_type_shader = 21;
    const int resource_type_back_button = 22;

    const int resource_type_max = 23;
}

#endif //gpe_constants_h
