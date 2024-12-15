/*
pawgui_parsers.h
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

#ifndef PAWGUI_PARSERS_H
#define PAWGUI_PARSERS_H

#include "../base/pawgui_base.h"

namespace pawgui
{
    class parsed_text
    {
        public:
            int textStart;
            int textEnd;
            parsed_text(int nStart, int nEnd);
            ~parsed_text();
    };

    class syntax_line
    {
        public:
            std::vector <parsed_text*> foundParses;
            int firstPos;
            int lastPos;
            syntax_line();
            ~syntax_line();
            std::string get_parsed_line(std::string sIn);
            void render_tokens( gpe::font_base * fontIn,std::string sIn, int x_pos, int y_pos, int lineStartPos, int lineMax_pos, gpe::color * render_color = nullptr, bool isNumber = false );
            void reset_self();
    };

    //Tab Related Functions
    bool has_early_tab(std::string str_in);
    std::string untab_string(std::string str_in);
    std::string generate_tabs(int tabCount);
    int get_tab_space_count();
}
#endif //PAWGUI_PARSERS_H
