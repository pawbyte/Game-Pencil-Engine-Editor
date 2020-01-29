/*
GPE_Functions.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "GPE_String_Ex.h"
#include "GPE_Renderer_SDL.h"

//The headers
bool copy_file(std::string sourceFile, std::string desinationFile, bool overwriteExisting = true);
int create_directory( std::string newPathToCreate);

std::string get_file_ext(std::string fileLocationIn);
std::string get_file_ext_last_dot(std::string fileLocationIn);
bool file_is_audio(std::string fileLocationIn);
bool file_is_font(std::string fileLocationIn);
bool file_is_image(std::string fileLocationIn);
bool file_is_video(std::string fileLocationIn);

std::string get_file_noext(std::string fileLocationIn);

void update_rect(GPE_Rect * rectIn, int nx, int ny, int nw, int nh);

std::string get_path_from_file(std::string fullFilePath);
std::string get_local_from_global_file(std::string fullFilePath);

bool file_exists(std::string fileNameIn);
bool path_exists(std::string pathNameIn);

#endif
