/*
pawgui_file_popups.h
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

#ifndef PAWGUI_FILE_POPUPS_H
#define PAWGUI_FILE_POPUPS_H

#include "../base/pawgui_base.h"
#include "../panel_list/pawgui_panel_list.h"
#include "../buttons/pawgui_buttons.h"
#include "../dropdown/pawgui_dropdown.h"
#include "../popups/pawgui_popups.h"
#include "../text_input/pawgui_text_input.h"

namespace pawgui
{
    std::string  get_filename_plain_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory, bool isSaving = false, bool file_is_dirSearch = false);
    std::string  get_directory_name_from_popup( const std::string & prompt, std::string &previousDirectory, bool isSaving = false);
    std::string  get_filename_open_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );
    std::string  get_filename_save_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );
}

#endif //PAWGUI_FILE_POPUPS_H
