/*
pawgui_file_popups.h
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

#ifndef PAWGUI_FILE_POPUPS_H
#define PAWGUI_FILE_POPUPS_H

#include "pawgui_base.h"
#include "pawgui_panel_list.h"
#include "pawgui_buttons.h"
#include "pawgui_dropdown.h"
#include "pawgui_popups.h"
#include "pawgui_text_input.h"

namespace pawgui
{
    std::string  get_filename_plain_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory, bool isSaving = false, bool isDirectorySearch = false);
    std::string  get_directory_name_from_popup( const std::string & prompt, std::string &previousDirectory, bool isSaving = false);
    std::string  get_filename_open_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );
    std::string  get_filename_save_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );
}

#endif //PAWGUI_FILE_POPUPS_H
