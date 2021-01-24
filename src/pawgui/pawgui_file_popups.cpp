/*
pawgui_file_popups.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#if defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__)
#include <cstring>
#include <SDL2/SDL_syswm.h>
#include "../other_libs/dlgmodule.h"
#include "../sdl_libs/gpe_window_controller_sdl.h"
#endif

#include "pawgui_file_popups.h"

namespace pawgui
{
    std::string  get_filename_open_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory)
    {
        return get_filename_plain_from_popup( prompt, allowedFileTypes,previousDirectory,false);
    }

    std::string get_filename_save_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory )
    {
        return get_filename_plain_from_popup( prompt, allowedFileTypes,previousDirectory,true);
    }

    std::string  get_directory_name_from_popup( const std::string & prompt, std::string &previousDirectory, bool isSaving )
    {
        return get_filename_plain_from_popup(prompt,"",previousDirectory,isSaving, true);
    }

    std::string get_filename_plain_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory,bool isSaving, bool isDirectorySearch )
    {
    std::string returnFile;
    #if defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__)
            if (!isDirectorySearch) {
                std::string osFileFilterString = gpe::parse_file_types(allowedFileTypes);
                #if defined(_WIN32) || (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__)
                SDL_SysWMinfo wmInfo;
                SDL_VERSION(&wmInfo.version);
                SDL_GetWindowWMInfo(gpe::window_controller_main_sdl->get_sdl_window(), &wmInfo);
                #if defined(_WIN32)
                widget_set_owner((char *)std::to_string((unsigned long long)wmInfo.info.win.window).c_str());
                #elif (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__)
                widget_set_owner((char *)std::to_string((unsigned long long)wmInfo.info.x11.window).c_str());
                #endif
                #endif
                widget_set_icon((char *)"icon.png");
                if (!osFileFilterString.empty() && osFileFilterString.back() == '|')
                    osFileFilterString.pop_back();
                if (!isSaving) {
                    returnFile = get_open_filename_ext((char *)osFileFilterString.c_str(), (char *)"", (char *)previousDirectory.c_str(), (char *)prompt.c_str());
                } else {
                    returnFile = get_save_filename_ext((char *)osFileFilterString.c_str(), (char *)"", (char *)previousDirectory.c_str(), (char *)prompt.c_str());
                }
            }
    #endif
        return returnFile;
    }
}
