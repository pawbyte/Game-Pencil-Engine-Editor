/*
pawgui_overlay.h
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

#ifndef PAWGUI_OVERLAY_SYSTEM_H
#define PAWGUI_OVERLAY_SYSTEM_H

#include "../themes/pawgui_themes.h"

namespace pawgui
{
    class overlay_system
    {
        private:
            gpe::texture_base * toolTipTexture;
            int toolTipX, toolTipY;
            int toolTip_halign, toolTip_valign;
            gpe::texture_base * previousScreenshot;
            bool tipTipJustUpdated;
            std::string tempUserMessageTitle;
            std::string tempUserMessageSubtitle;
            std::string tempUserMessageText;
            bool tempTopLeftMessage;
            float temporayMessageDisplayTimer;
            float temporayMessageDisplayStartTimer;
        public:
            std::string toolTipstring;
            overlay_system();
            ~overlay_system();
            bool overhead_used();
            void process_cursor();

            void update_tooltip(std::string newTip);
            void render_frozen_screenshot();
            void render_temporary_message();
            void render_tooltip(int x_pos = -1, int y_pos = -1 );
            void take_frozen_screenshot();
            void update_temporary_message(std::string mTitle="", std::string mSubTitle="", std::string mText="", float messDuration = 3, bool topLeftMessage = true );
    };
    extern overlay_system * main_overlay_system;
}

#endif //PAWGUI_OVERLAY_SYSTEM_H
