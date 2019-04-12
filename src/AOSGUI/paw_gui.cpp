/*
paw_gui.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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
#include "paw_gui.h"

bool PAW_GUI_START()
{
    guiRCM = new GPE_DataManager();
    if( PAW_GUI_LOAD_FONTS() )
    {
        GPE_Report("IDE properly added all GPE Editor Fonts... \n");
    }
    else
    {
        GPE_Report("Unable to load GPE Editor Fonts!\n");
        return false;
    }

    MAIN_GUI_SETTINGS = new GPE_GUI_Settings();
    MAIN_OVERLAY = new GPE_Overlay_System();
    GPE_LOADER = new GPE_LoaderDiplay();
    MAIN_SEARCH_CONTROLLER = new PAW_GUI_SEARCH_CONTROLLER();
    if( MAIN_GUI_SETTINGS!=NULL && MAIN_OVERLAY!=NULL && MAIN_SEARCH_CONTROLLER!=NULL )
    {
        MAIN_CONTEXT_MENU = new GPE_PopUpMenu_Option("  ",-1,false,false,true);
        MAIN_CONTEXT_MENU->isTopOfMenu = true;

        GPE_TEXTURE_COLOR_PICKER_GRADIENT = new GPE_Texture();
        if( GPE_TEXTURE_COLOR_PICKER_GRADIENT!=NULL)
        {
            GPE_TEXTURE_COLOR_PICKER_GRADIENT->load_new_texture(APP_DIRECTORY_NAME+"resources/gfx/textures/color_picker_gradient.png",-1,false);
        }

        GPE_SURFACE_COLOR_PICKER_GRADIENT = gpe_sdl->load_surface_image(APP_DIRECTORY_NAME+"resources/gfx/textures/color_picker_gradient.png");
        if( SDL_MUSTLOCK( GPE_SURFACE_COLOR_PICKER_GRADIENT) )
        {
            SDL_LockSurface(GPE_SURFACE_COLOR_PICKER_GRADIENT);
        }
        GPE_LOGO = new GPE_Texture();
        if( GPE_LOGO!=NULL)
        {
            GPE_LOGO->load_new_texture(APP_DIRECTORY_NAME+"resources/gamepencil_icon_72dpi.png",-1,false);
        }

        GPE_TEXTURE_TRANSPARENT_BG = new GPE_Texture();
        if( GPE_TEXTURE_TRANSPARENT_BG!=NULL)
        {
            GPE_TEXTURE_TRANSPARENT_BG->load_new_texture(APP_DIRECTORY_NAME+"resources/gfx/textures/transparent_bg.png",-1,false);
        }

        GPE_CHECKMARK_IMAGE = new GPE_Texture();
        if( GPE_CHECKMARK_IMAGE!=NULL)
        {
            GPE_CHECKMARK_IMAGE->load_new_texture(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/chevron-down.png",-1,false);
        }
        GPE_DROPDOWN_ARROW = new GPE_Texture();
        if( GPE_DROPDOWN_ARROW!=NULL)
        {
            GPE_DROPDOWN_ARROW->load_new_texture(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/angle-down.png",-1,false);
        }
        GPE_SCROLLBAR_ARROW = guiRCM->sprite_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/caret-down.png",1,true,0,0,false);

        GPE_Cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
        GPE_HoverCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        GPE_LoadingCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);

        return true;
    }
    return false;
}

bool PAW_GUI_END()
{
    GPE_Report("Deleting PAW_GUI ....");
    if( MAIN_OVERLAY !=NULL)
    {
        delete MAIN_OVERLAY;
        MAIN_OVERLAY = NULL;
    }
    if( GPE_LOADER !=NULL)
    {
        delete GPE_LOADER;
        GPE_LOADER = NULL;
    }

    if( MAIN_SEARCH_CONTROLLER !=NULL)
    {
        delete MAIN_SEARCH_CONTROLLER;
        MAIN_SEARCH_CONTROLLER = NULL;
    }
    if( MAIN_GUI_SETTINGS !=NULL)
    {
        delete MAIN_GUI_SETTINGS;
        MAIN_GUI_SETTINGS = NULL;
    }

    GPE_Report("Deleting mini-compiler....");
    if( GPE_MAIN_HIGHLIGHTER!=NULL)
    {
        delete GPE_MAIN_HIGHLIGHTER;
        GPE_MAIN_HIGHLIGHTER = NULL;
    }
    if( guiRCM!=NULL )
    {
        delete guiRCM;
        guiRCM = NULL;
    }
}
