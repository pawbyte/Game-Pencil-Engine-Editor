/*
GPE_Game_Load_State.cpp
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

#include "GPE_Artist_Base.h"
#include "GPE_Game_Load_State.h"
#include "GPE_Game_Master_State.h"

GPE_GameLoader::GPE_GameLoader( std::string sName )
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

    std::string mainGuiFontLocation = APP_DIRECTORY_NAME+"resources/fonts/dejavu_sans_mono/DejaVuSansMono.ttf";
    if( gfs!=NULL )
    {
        GPE_Report("Loading default font...");
        GPE_DEFAULT_FONT = gfs->open_font(mainGuiFontLocation,14,false, "GPE_DEFAULT_FONT");
    }
    else
    {
        GPE_Report("GFS is NULL...");
    }

    loadbarColor = c_lime->duplicate_color();
    loadbarOutlineColor = c_gray->duplicate_color();
    loaderTextColor = c_white->duplicate_color();

    bgColor = c_black->duplicate_color();
    bgTexture = gpeph->get_new_texture();


    loadTextPosition = 0;
    loadBarPosition = 0;
    textPadding = 0;
    textHalign = FA_RIGHT;
    textValign = FA_BOTTOM;
    currentLoadValue = 0;
    seekedLoadValue = 0;

    seekedCategories.push_back( "textures" );
    seekedCategories.push_back( "tilesheets" );
    seekedCategories.push_back( "audio" );
    seekedCategories.push_back( "particles" );
    seekedCategories.push_back( "lights" );

    errorOccurred = false;
    if( GPE_ENGINE_SETTINGS != NULL )
    {
        GPE_KeyPair * cCategorty  = NULL;
        for( int i = 0; i < (int)seekedCategories.size(); i++ )
        {
            cCategorty = GPE_ENGINE_SETTINGS->gameResourcesInstructions->find_option_named( seekedCategories[i] );
            if( cCategorty !=NULL )
            {
                currentLoadTitle = cCategorty->keyString;
                seekedLoadValue += (int)cCategorty->subOptions.size();
            }
            else
            {
                GPE_Report("Error: Engine Settings group ["+seekedCategories[i]+"] is currently NULL!" );
            }
        }
    }
    else
    {
        GPE_Report("Error: Global variable [GPE_ENGINE_SETTINGS] is currently NULL!" );
        errorMessage = "GPE_ENGINE_SETTINGS not defined!";
        errorOccurred = true;
    }
}

GPE_GameLoader::~GPE_GameLoader()
{
    clean_up();
}

void GPE_GameLoader::apply_logic()
{

    bool resourcesAllProcessed = false;
    if( GPE_ENGINE_SETTINGS == NULL )
    {
        errorOccurred = true;
        errorMessage = "GPE_ENGINE_SETTINGS not defined!";
    }
    else
    {
        GPE_KeyPair * mPair = NULL;
        GPE_KeyPair * cPair = NULL;
        int j = 0;
        int maxResourceTypeSize = (int)GPE_ENGINE_SETTINGS->gameResourcesInstructions->subOptions.size();
        for( int i = currentLoadCateory; i < maxResourceTypeSize; i++ )
        {
            mPair = GPE_ENGINE_SETTINGS->gameResourcesInstructions->subOptions[i];
            currentLoadTitle = "Loading" + mPair->keyString + "s";
            if( mPair !=NULL )
            {
                process_keypair( mPair );            }
        }
    }

    if( seekedLoadValue >= seekedLoadValue )
    {
        resourcesAllProcessed = true;
    }
    if( resourcesAllProcessed )
    {
        /*
        GPE_GameMaster * gpeMaster = new GPE_GameMaster( "GPE_Master");
        gpe->state_add( gpeMaster );
        gpe->state_set( gpeMaster->get_state_name() );
        */
    }
}

void GPE_GameLoader::clean_up()
{

}

void GPE_GameLoader::end_state()
{

}

bool GPE_GameLoader::keypair_is_seeked( GPE_KeyPair * pairToProcess )
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

void GPE_GameLoader::process_input()
{
    if( GPE_MAIN_WINDOW->windowClosed || input->done  )
    {
        gpe->state_set( "exit" );
    }
}

void GPE_GameLoader::process_keypair( GPE_KeyPair * pairToProcess )
{
    if( pairToProcess ==NULL )
    {
        return;
    }

}

void GPE_GameLoader::render()
{
    if( bgTexture!= NULL )
    {
        bgTexture->render_tex_resized( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL );
    }
    gcanvas->render_rectangle( 0, SCREEN_HEIGHT - 64, SCREEN_WIDTH, SCREEN_HEIGHT - 32, loadbarColor, false, 255 );
    gcanvas->render_rectangle( 0, SCREEN_HEIGHT - 64, SCREEN_WIDTH, SCREEN_HEIGHT - 32, loadbarOutlineColor, true, 255 );

    if( GPE_DEFAULT_FONT!=NULL )
    {
        GFS->render_text(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, currentLoadTitle, loaderTextColor, GPE_DEFAULT_FONT,  FA_CENTER, FA_MIDDLE, 255 );
        GFS->render_text(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + ( textPadding + GPE_DEFAULT_FONT->get_mono_height() ) *2, currentLoadSubTitle, loaderTextColor, GPE_DEFAULT_FONT,  FA_CENTER, FA_MIDDLE, 255 );
        GFS->render_text(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + ( textPadding + GPE_DEFAULT_FONT->get_mono_height() )* 4, currentLoadMessage, loaderTextColor, GPE_DEFAULT_FONT,  FA_CENTER, FA_MIDDLE, 255 );

        float loadPercentage = 0;
        if( seekedLoadValue != 0 )
        {
            loadPercentage = currentLoadValue /seekedLoadValue;
            loadPercentage = 100.f * loadPercentage;
        }
        GFS->render_text( SCREEN_WIDTH/2, SCREEN_HEIGHT - 48, int_to_string( loadPercentage )+ "%", loaderTextColor, GPE_DEFAULT_FONT,  FA_CENTER, FA_MIDDLE, 255 );
        gfs->render_fps(GPE_DEFAULT_FONT, c_green, SCREEN_WIDTH -32, 32, FA_RIGHT, FA_TOP );

    }

}

void GPE_GameLoader::start_state()
{

}
