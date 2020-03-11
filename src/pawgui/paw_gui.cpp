/*
paw_gui.cpp
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
#include "paw_gui.h"

bool PAW_GUI_START()
{
    //gpe::cursor_main_controller->cursor_create_from_image( gpe::app_directory_name + "resources/gfx/iconpacks/fontawesome/asterisk.png" );
    paw_gui_rsm = new gpe::asset_manager( gpe::rph->get_default_render_package(), "Paw_GUI");
    if( PAW_GUI_LOAD_FONTS() )
    {
        gpe::error_log->report("IDE properly added all GPE Editor Fonts... \n");
    }
    else
    {
        gpe::error_log->report("Unable to load GPE Editor Fonts!\n");
        return false;
    }

    main_GUI_SETTINGS = new GPE_GUI_Settings();
    main_OVERLAY = new GPE_Overlay_System();
    GPE_LOADER = new GPE_LoaderDiplay();
    main_SEARCH_CONTROLLER = new PAW_GUI_SEARCH_CONTROLLER();
    if( main_GUI_SETTINGS!=NULL && main_OVERLAY!=NULL && main_SEARCH_CONTROLLER!=NULL )
    {
        main_CONTEXT_MENU = new GPE_PopUpMenu_Option("  ",-1,false,false,true);
        main_CONTEXT_MENU->isTopOfMenu = true;

        texture_color_picker_gradient = paw_gui_rsm->texture_add_filename(  gpe::app_directory_name+"resources/gfx/textures/color_picker_gradient.png" );

        std::string colorPickerFilename = gpe::app_directory_name+"resources/gfx/textures/color_picker_gradient.png";


        GPE_LOGO = paw_gui_rsm->texture_add_filename(  gpe::app_directory_name+"resources/gamepencil_icon_72dpi.png" );
        GPE_TEXTURE_TRANSPARENT_BG = paw_gui_rsm->texture_add_filename(  gpe::app_directory_name+"resources/gfx/textures/transparent_bg.png");
        GPE_CHECKMARK_IMAGE = paw_gui_rsm->texture_add_filename(  gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/chevron-down.png");
        GPE_DROPDOWN_ARROW = paw_gui_rsm->texture_add_filename(  gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/angle-down.png");
        GPE_EYE_DROPPER = paw_gui_rsm->texture_add_filename(  gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/eyedropper.png");

        GPE_SCROLLBAR_ARROW = paw_gui_rsm->animation_add("guiScrollArrowDown", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/caret-down.png",1,true,0,0,false);
        return true;
    }
    return false;
}

bool PAW_GUI_END()
{
    gpe::error_log->report("Deleting PAW_GUI ....");
    if( main_OVERLAY !=NULL)
    {
        delete main_OVERLAY;
        main_OVERLAY = NULL;
    }
    if( GPE_LOADER !=NULL)
    {
        delete GPE_LOADER;
        GPE_LOADER = NULL;
    }

    if( main_SEARCH_CONTROLLER !=NULL)
    {
        delete main_SEARCH_CONTROLLER;
        main_SEARCH_CONTROLLER = NULL;
    }
    if( main_GUI_SETTINGS !=NULL)
    {
        delete main_GUI_SETTINGS;
        main_GUI_SETTINGS = NULL;
    }

    gpe::error_log->report("Deleting mini-code-highlighter....");
    if( GPE_main_HIGHLIGHTER!=NULL)
    {
        delete GPE_main_HIGHLIGHTER;
        GPE_main_HIGHLIGHTER = NULL;
    }
    if( paw_gui_rsm!=NULL )
    {
        delete paw_gui_rsm;
        paw_gui_rsm = NULL;
    }
    return true;
}
