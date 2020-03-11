/*
paw_gui_file_popups.h
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

#ifndef PAW_GUI_FILE_POPUPS
#define PAW_GUI_FILE_POPUPS

#include "paw_gui_base.h"
#include "paw_gui_panel_list.h"
#include "paw_gui_buttons.h"
#include "paw_gui_dropdown.h"
#include "paw_gui_popups.h"
#include "paw_gui_text_input.h"

std::string  GPE_GetPlainFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory, bool isSaving = false, bool isDirectorySearch = false);
std::string  GPE_GetDirectoryName( const std::string & prompt, std::string &previousDirectory, bool isSaving = false);
std::string  GPE_GetOpenFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );
std::string  GPE_GetSaveFileName( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory );

#endif
