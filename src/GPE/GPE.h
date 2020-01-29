/*
GPE.h
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

#ifndef GPE_MAIN
#define GPE_MAIN

#include "GPE_Render_Package.h"
#include "GPE_Render_Package_Handler.h"
#include "GPE_Settings.h"
#include "GPE_Artist_Base.h"
#include "GPE_Animation.h"
#include "GPE_Engine.h"
#include "GPE_Fonts.h"
#include "GPE_Ini_File.h"
#include "GPE_Particles.h"
#include "GPE_Parser.h"
#include "GPE_Paths.h"
#include "GPE_Layers.h"
#include "GPE_Scene.h"
#include "GPE_File_System.h"
#include "GPE_Program_State.h"
#include "GPE_Game_Load_State.h"
#include "GPE_Math.h"

bool GPE_Init( int argc, char* args[] );
bool GPE_Quit();

#endif //GPE_MAIN

