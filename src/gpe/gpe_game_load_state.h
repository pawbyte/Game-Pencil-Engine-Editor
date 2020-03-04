/*
gpe_game_load_state.h
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

#ifndef gpe_game_load_state_h
#define gpe_game_load_state_h

#include "gpe_runtime.h"
#include "gpe_font_base.h"
#include "gpe_ini_file.h"
#include "gpe_particles2d.h"
#include "gpe_parser.h"
#include "gpe_path2d.h"
#include "gpe_layer.h"
#include "gpe_scene.h"
#include "gpe_file_system.h"
#include "gpe_program_state.h"
#include "gpe_settings.h"
#include "gpe_color.h"

namespace gpe
{
    class game_loader: public program_state
    {
        private:
            bool errorOccurred;
            int texturesAllLoaded;
            int texturesLoadedCount;
            bool animationsLoaded;
            bool audioLoaded;
            std::string currentLoadTitle;
            std::string currentLoadSubTitle;
            std::string currentLoadMessage;
            std::string errorMessage;

            int cTicks, cTicksMax;
            bool loadingBegan;

            color * bgColor;
            texture_base * bgTexture;

            color * loadbarColor;
            color * loadbarOutlineColor;
            color * loaderTextColor;

            int textPadding;
            int textHalign, textValign;
            int loadTextPosition;
            int loadBarPosition;

            //Positions to keep track of loading stance
            float currentLoadValue, seekedLoadValue;
            int currentLoadCateory, currentLoadSubcategory, updateLoaderCount, updateLoaderMax;
            std::vector< std::string > seekedCategories;
        public:
            game_loader(std::string sName = "");
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
}
#endif // gpe_game_load_state_h
