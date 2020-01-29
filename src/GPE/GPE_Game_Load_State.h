/*
GPE_Game_Load_State.h
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

#ifndef GPE_GAME_LOADER
#define GPE_GAME_LOADER

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
#include "GPE_Settings.h"
#include "GPE_Color.h"

class GPE_GameLoader: public GPE_ProgramState
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

    GPE_Color * bgColor;
    GPE_Texture_Base * bgTexture;

    GPE_Color * loadbarColor;
    GPE_Color * loadbarOutlineColor;
    GPE_Color * loaderTextColor;

    int textPadding;
    int textHalign, textValign;
    int loadTextPosition;
    int loadBarPosition;

    //Positions to keep track of loading stance
    float currentLoadValue, seekedLoadValue;
    int currentLoadCateory, currentLoadSubcategory, updateLoaderCount, updateLoaderMax;
    std::vector< std::string > seekedCategories;
public:
    GPE_GameLoader(std::string sName = "");
    ~GPE_GameLoader();
    void apply_logic();
    void clean_up();
    void end_state();
    bool keypair_is_seeked( GPE_KeyPair * pairToProcess );
    void process_input();
    void process_keypair( GPE_KeyPair * pairToProcess );
    void render();
    void start_state();
};

#endif // GPE_GAME_LOADER
