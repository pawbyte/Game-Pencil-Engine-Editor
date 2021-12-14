/*
game_state_load.h
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

#ifndef game_state_load_h
#define game_state_load_h

#include "gpe/gpe.h"

class game_loader: public gpe::program_state
{
    private:
        bool resources_all_loaded;
        std::string load_title_current;
        std::string load_subtitle_current;
        std::string load_message_current;

        int c_ticks, c_ticks_max;
        bool loading_began;

        gpe::color * color_load_bar;
        gpe::color * color_load_bar_outline;
        gpe::color * color_load_text;

        int padding_text;
        int text_halign, text_valign;
        int text_load_position;
        int load_bar_position;

        //Positions to keep track of loading stance
        float load_value_current, load_value_seeked;
        int load_category_current, load_subcategory_current, update_load_count, update_load_max;
        std::vector< std::string > seeked_categories;
    public:
        bool show_load_percentage;
        game_loader(std::string s_name = "");
        ~game_loader();
        void apply_logic();
        void clean_up();
        void end_state();
        bool keypair_is_seeked( gpe::key_pair * pairToProcess );
        void process_input();
        void process_keypair( gpe::key_pair * pairToProcess );
        void render();
        void start_state();
};

#endif // game_state_load_h
