/*
GPE_CIncludes.h
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


#ifndef GPE_CINCLUDES_H
#define GPE_CINCLUDES_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "limits.h"
#include <SDL.h>
#include <SDL_version.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_video.h>

#ifdef _WIN32

#define NOMINMAX
#include <windows.h>
//#include <SDL_sysWM.h>
//#undef WIN32_LEAN_AND_MEAN
#include <Commdlg.h>

#endif

#include <iostream>
#include <fstream>
#include <cerrno>
#include <stdio.h>
#include <dirent.h>
#include <exception>
//Operating Systems
int GPE_Gain_OS();
//File Functions
void appendToFile(std::string fileName, std::string stringIn,std::string stringIn2="",std::string stringIn3="",std::string stringIn4="", std::string stringIn5="", std::string stringIn6="", std::string stringIn7="", std::string stringIn8="", std::string stringIn9="");
void GPE_Report(std::string stringIn,std::string stringIn2="",std::string stringIn3="",std::string stringIn4="", std::string stringIn5="", std::string stringIn6="", std::string stringIn7="", std::string stringIn8="", std::string stringIn9="");

#endif
