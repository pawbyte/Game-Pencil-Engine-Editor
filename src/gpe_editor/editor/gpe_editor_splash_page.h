/*
gpe_editor_splash_page.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.
Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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
#include "../pawgui/pawgui/pawgui.h"

class gpe_splash_page
{
    protected:
        float item1Value;
        float item1Max;
        float item2Value;
        float item2Max;
        bool startup_mode;
        gpe::texture_base * gpe_logo;
        float post_load_c_ticks, post_load_c_ticks_max;
        std::vector< std::string> top_supporters_credits;
        int random_splash_top_four[4];
        int current_supporter_pos;
        float load_progress; // last 30% is for the remainder credits
    public:
        std::string displayMessageTitle;
        std::string displayMessageSubtitle;
        std::string displayMessagestring;
        gpe_splash_page();
        ~gpe_splash_page();
        bool exit_startup_mode(); //Attempts to exit startup mode based on loading ticks
        bool in_startup_mode(); //Checks if the loader is still in startup mode
        void increment_bar1();
        void increment_bar2();
        void increment_and_update( std::string subTitle, std::string message, int barNumber = 0, bool renderUpdate = true );

        void render();
        void reset();

        void set_bar1( float max_value);
        void set_bar2( float max_value);
        void set_load_percent( float load_percent);
        void update_messages(std::string title, std::string subTitle, std::string message, bool renderUpdate = true );
        void update_submessages(  std::string subTitle, std::string message, bool renderUpdate = true  );
};

extern gpe_splash_page * main_gpe_splash_page;

#endif //GPE_EDITOR_SPLASH_PAGE_H
