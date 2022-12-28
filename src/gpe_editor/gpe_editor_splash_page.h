/*
gpe_editor_splash_page.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.
Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef GPE_EDITOR_SPLASH_PAGE_H
#define GPE_EDITOR_SPLASH_PAGE_H

#include <string>
#include "../gpe/gpe.h"
#include "../pawgui/pawgui.h"

class gpe_splash_page
{
    private:
        float item1Value;
        float item1Max;
        float item2Value;
        float item2Max;
        gpe::texture_base * gpe_logo;
    public:
        bool in_startup_mode;
        std::string displayMessageTitle;
        std::string displayMessageSubtitle;
        std::string displayMessagestring;
        gpe_splash_page();
        ~gpe_splash_page();
        void increment_bar1();
        void increment_bar2();
        void increment_and_update( std::string subTitle, std::string message, int barNumber = 0, bool renderUpdate = true );

        void set_bar1( float max_value);
        void set_bar2( float max_value);
        void render_loader();
        void reset();
        void update_messages(std::string title, std::string subTitle, std::string message, bool renderUpdate = true );
        void update_submessages(  std::string subTitle, std::string message, bool renderUpdate = true  );
};

extern gpe_splash_page * main_gpe_splash_page;

#endif //GPE_EDITOR_SPLASH_PAGE_H
