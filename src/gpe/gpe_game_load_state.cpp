/*
gpe_game_load_state.cpp
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

#include "gpe_artist_base.h"
#include "gpe_game_load_state.h"
#include "gpe_game_master_state.h"

namespace gpe
{
    game_loader::game_loader( std::string sName )
    {
        if( (int)sName.size() > 0 )
        {
            stateName = sName;
        }
        else
        {
            stateName = "GPE_Loader";
        }

        audioLoaded = false;
        animationsLoaded = false;
        loadingBegan = false;

        currentLoadTitle = "Loading Game Settings";
        currentLoadMessage = "Please wait";
        currentLoadSubTitle = "...";
        //For now images take up the main amount of resources.
        //This will either be a full even split or actually dependent on amount of files per category

        cTicks = 0;
        cTicksMax = 1000;

        updateLoaderCount = 0;
        updateLoaderMax = 20;

        currentLoadCateory = 0;
        currentLoadSubcategory = 0;
        loadingBegan = false;

        std::string mainGuiFontLocation = app_directory_name+"resources/fonts/dejavu_sans_mono/DejaVuSansMono.ttf";
        if( gfs!=NULL )
        {
            error_log->report("Loading default font...");
            font_default = gfs->open_font(mainGuiFontLocation,14,false, "font_default");
        }
        else
        {
            error_log->report("gfs is NULL...");
        }

        loadbarColor = c_lime->duplicate_color();
        loadbarOutlineColor = c_gray->duplicate_color();
        loaderTextColor = c_white->duplicate_color();

        bgColor = c_black->duplicate_color();
        bgTexture = rph->get_new_texture();


        loadTextPosition = 0;
        loadBarPosition = 0;
        textPadding = 0;
        textHalign = fa_right;
        textValign = fa_bottom;
        currentLoadValue = 0;
        seekedLoadValue = 0;

        seekedCategories.push_back( "textures" );
        seekedCategories.push_back( "tilesheets" );
        seekedCategories.push_back( "audio" );
        seekedCategories.push_back( "particles" );
        seekedCategories.push_back( "lights" );

        errorOccurred = false;
        if( settings != NULL )
        {
            key_pair * cCategorty  = NULL;
            for( int i = 0; i < (int)seekedCategories.size(); i++ )
            {
                cCategorty = settings->gameResourcesInstructions->find_option_named( seekedCategories[i] );
                if( cCategorty !=NULL )
                {
                    currentLoadTitle = cCategorty->keyString;
                    seekedLoadValue += (int)cCategorty->subOptions.size();
                }
                else
                {
                    error_log->report("Error: Engine Settings group ["+seekedCategories[i]+"] is currently NULL!" );
                }
            }
        }
        else
        {
            error_log->report("Error: Global variable [settings] is currently NULL!" );
            errorMessage = "settings not defined!";
            errorOccurred = true;
        }
    }

    game_loader::~game_loader()
    {
        clean_up();
    }

    void game_loader::apply_logic()
    {
        bool resourcesAllProcessed = false;
        if( settings == NULL )
        {
            errorOccurred = true;
            errorMessage = "settings not defined!";
        }
        else
        {
            key_pair * mPair = NULL;
            key_pair * cPair = NULL;
            int j = 0;
            int maxResourceTypeSize = (int)settings->gameResourcesInstructions->subOptions.size();
            for( int i = currentLoadCateory; i < maxResourceTypeSize; i++ )
            {
                mPair = settings->gameResourcesInstructions->subOptions[i];
                currentLoadTitle = "Loading" + mPair->keyString + "s";
                if( mPair !=NULL )
                {
                    process_keypair( mPair );
                }
            }
        }

        if( seekedLoadValue >= seekedLoadValue )
        {
            resourcesAllProcessed = true;
        }
        if( resourcesAllProcessed )
        {
            game_master * gpeMaster = new game_master( "GPE_Master");
            game_runtime->state_add( gpeMaster );
            game_runtime->state_set( gpeMaster->get_state_name() );
        }
    }

    void game_loader::clean_up()
    {

    }

    void game_loader::end_state()
    {

    }

    bool game_loader::keypair_is_seeked( key_pair * pairToProcess )
    {
        if( pairToProcess!=NULL )
        {
            for( int i = (int)seekedCategories.size(); i >=0; i--)
            {
                if( pairToProcess->keyString == seekedCategories[i] )
                {
                    return true;
                }
            }
        }
        return false;
    }

    void game_loader::process_input()
    {
        if( window_controller_main->window_closed || input->exit_requested  )
        {
            game_runtime->state_set( "exit" );
        }
    }

    void game_loader::process_keypair( key_pair * pairToProcess )
    {
        if( pairToProcess ==NULL )
        {
            return;
        }

    }

    void game_loader::render()
    {
        gcanvas->render_rectangle( 0, 0, screen_width, screen_height, c_black, false, 255 );

        if( bgTexture!= NULL )
        {
            bgTexture->render_tex_resized( 0, 0, screen_width, screen_height, NULL );
        }
        gcanvas->render_rectangle( 0, screen_height - 64, screen_width, screen_height - 32, loadbarColor, false, 255 );
        gcanvas->render_rectangle( 0, screen_height - 64, screen_width, screen_height - 32, loadbarOutlineColor, true, 255 );

        if( font_default!=NULL )
        {
            gfs->render_text(screen_width/2, screen_height/2, currentLoadTitle, loaderTextColor, font_default,  fa_center, fa_middle, 255 );
            gfs->render_text(screen_width/2, screen_height/2 + ( textPadding + font_default->get_mono_height() ) *2, currentLoadSubTitle, loaderTextColor, font_default,  fa_center, fa_middle, 255 );
            gfs->render_text(screen_width/2, screen_height/2 + ( textPadding + font_default->get_mono_height() )* 4, currentLoadMessage, loaderTextColor, font_default,  fa_center, fa_middle, 255 );

            float loadPercentage = 0;
            if( seekedLoadValue != 0 )
            {
                loadPercentage = currentLoadValue /seekedLoadValue;
                loadPercentage = 100.f * loadPercentage;
            }
            gfs->render_text( screen_width/2, screen_height - 48, stg_ex::int_to_string( loadPercentage )+ "%", loaderTextColor, font_default,  fa_center, fa_middle, 255 );
            gfs->render_fps(font_default, c_green, screen_width -32, 32, fa_right, fa_top );

        }

    }

    void game_loader::start_state()
    {

    }
}
