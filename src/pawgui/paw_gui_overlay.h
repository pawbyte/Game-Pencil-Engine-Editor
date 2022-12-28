/*
paw_gui_overlay.h
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

#ifndef PGUI_OVERLAY_SYSTEM
#define PGUI_OVERLAY_SYSTEM

#include "paw_gui_themes.h"

class GPE_Overlay_System
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
    std::string toolTipString;
    GPE_Overlay_System();
    ~GPE_Overlay_System();
    bool overhead_used();
    void process_cursor();

    void update_tooltip(std::string newTip);
    void render_frozen_screenshot();
    void render_temporary_message();
    void render_tooltip(int xPos = -1, int yPos = -1 );
    void take_frozen_screenshot();
    void update_temporary_message(std::string mTitle="", std::string mSubTitle="", std::string mText="", float messDuration = 3, bool topLeftMessage = true );

};

extern GPE_Overlay_System * main_OVERLAY;

#endif
