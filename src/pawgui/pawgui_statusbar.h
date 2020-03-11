/*
pawgui_status_bar.h
This file is part of:
PAW GUI
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

#ifndef PAWGUI_STATUS_BAR_H
#define PAWGUI_STATUS_BAR_H

#include "pawgui_base.h"

namespace pawgui
{
    class widget_statusbar: public widget_basic
    {
        private:
            std::string codeEditorStatusBarString;
            std::string insertModeString;
        public:
            std::string statusBarLeftText;
            widget_statusbar();
            ~widget_statusbar();
            void process_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect *cam=NULL);
            void render_self( gpe::shape_rect * viewedSpace=NULL, gpe::shape_rect * cam = NULL);
            void reset_statusbar();
            void set_codestring(std::string newCodeString);
            void set_insertmode(std::string newInsertMode);
    };
    extern widget_statusbar * main_statusbar;
}
#endif //PAWGUI_STATUS_BAR_H
