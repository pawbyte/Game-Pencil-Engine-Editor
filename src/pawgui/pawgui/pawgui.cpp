/*
pawgui.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/
#include "pawgui.h"

namespace pawgui
{
    bool init_gui(  std::string mono_font_location, int font_min_size )
    {
        //gpe::cursor_main_controller->cursor_create_from_image( gpe::app_directory_name + "resources/gfx/iconpacks/fontawesome/asterisk.png" );
        rsm_gui = new gpe::asset_manager( gpe::rph->get_default_render_package(), "pawgui");
        if( load_default_fonts( mono_font_location, font_min_size) )
        {
            gpe::error_log->report("IDE properly added all PawGUI Fonts... \n");
            popup_font_size_width = font_min_size;
            popup_font_size_height = font_min_size;
            if( font_popup!=nullptr)
            {
                font_popup->get_metrics("A",&popup_font_size_width,&popup_font_size_height);
            }

        }
        else
        {
            gpe::error_log->report("Unable to load PawGUI Fonts!\n");
            return false;
        }

        main_settings = new gui_settings();
        main_overlay_system = new overlay_system();
        main_search_controller = new search_controller();
        if( main_settings!=nullptr && main_overlay_system!=nullptr && main_search_controller!=nullptr )
        {
            main_context_menu = new popup_menu_option("  ",-1,false,false,true);
            main_context_menu->isTopOfMenu = true;

            texture_color_picker_gradient = rsm_gui->texture_add_filename(  gpe::app_directory_name+"resources/gfx/textures/color_picker_gradient.png" );

            std::string colorPickerFilename = gpe::app_directory_name+"resources/gfx/textures/color_picker_gradient.png";
            checkmark_texture = rsm_gui->texture_add_filename(  gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/chevron-down.png");
            dropdown_arrow_texture = rsm_gui->texture_add_filename(  gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/angle-down.png");
            eyedropper_texture = rsm_gui->texture_add_filename(  gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/eyedropper.png");

            main_scrollbar_arrow = rsm_gui->animation_add("guiScrollArrowDown", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/caret-down.png",1,true,0,0,false);
            return true;
        }
        return false;
    }

    bool quit_gui()
    {
        gpe::error_log->report("Deleting PAWGUI ....");
        texture_color_picker_gradient = nullptr;

        if( main_overlay_system !=nullptr)
        {
            delete main_overlay_system;
            main_overlay_system = nullptr;
        }


        if( main_search_controller !=nullptr)
        {
            delete main_search_controller;
            main_search_controller = nullptr;
        }
        if( main_settings !=nullptr)
        {
            delete main_settings;
            main_settings = nullptr;
        }

        gpe::error_log->report("Deleting mini-code-highlighter....");
        if( main_syntax_highlighter!=nullptr)
        {
            delete main_syntax_highlighter;
            main_syntax_highlighter = nullptr;
        }
        if( rsm_gui!=nullptr )
        {
            delete rsm_gui;
            rsm_gui = nullptr;
        }
        return true;
    }
}
