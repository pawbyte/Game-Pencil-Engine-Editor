/*
paw_gui_parsers.h
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

#ifndef PAW_GUI_PARSERS_H
#define PAW_GUI_PARSERS_H

#include "paw_gui_base.h"


class GPE_ParsedText
{
public:
    int textStart;
    int textEnd;
    GPE_ParsedText(int nStart, int nEnd);
    ~GPE_ParsedText();
};

class GPE_SyntaxLine
{
public:
    std::vector <GPE_ParsedText*> foundParses;
    int firstPos;
    int lastPos;
    GPE_SyntaxLine();
    ~GPE_SyntaxLine();
    std::string get_parsed_line(std::string sIn);
    void render_tokens(GPE_Font * fontIn,std::string sIn, int xPos, int yPos, int lineStartPos, int lineMaxPos, GPE_Color * renderColor = NULL, bool isNumber = false);
    void reset_self();
};


//Tab Related Functions
bool has_early_tab(std::string strIn);
std::string untab_string(std::string strIn);
std::string generate_tabs(int tabCount);
int get_tab_space_count();

#endif
