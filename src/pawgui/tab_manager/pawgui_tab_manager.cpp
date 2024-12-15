/*
pawgui_tab_manager.cpp
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

#include "../tab_manager/pawgui_tab_manager.h"
#include "../search_controller/pawgui_search_controller.h"
#include "../text_editor/pawgui_text_editor.h"

namespace pawgui
{
    widget_tab_resource_bar * main_tab_resource_bar = nullptr;

    widget_tab_resource_bar::widget_tab_resource_bar()
    {
        isFullWidth = true;
        needsNewLine = true;
        useDock_button = false;
        isExpanded = false;
        tabExpand_button = new widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/expand.png","Undock View",-1,18);
        selectedResource = nullptr;
        widget_type = "tabmanager";
        widget_box.x = 16;
        widget_box.y = 16;
        widget_box.w = gpe::screen_width/2;
        widget_box.h = 18;

        tabHeaderBox.x = 16;
        tabHeaderBox.y = 0;
        tabHeaderBox.w = gpe::screen_width/2;
        tabHeaderBox.h = 18;
        tabBox.x = 16;
        tabBox.y = 32;
        tabBox.w = gpe::screen_width;
        tabBox.h = gpe::screen_height-widget_box.y;
        barXPadding = padding_default;
        barYPadding = padding_default;
        tabXPAdding = padding_default;

        tabToClose = -1;
        tabXHover = -1;
        tabInUse = -1;
        tabSize = -1; //defaults to equally divide the tab onto the screen

        miniTabSize = 32;
        extraSmallSize = 48;
        smallTabSize = 64;
        medTabSize = 96;
        largeTabSize = 128;
        extraLargeTabSize = 160;
        xxLargeTabSize = 192;
        tabsPerView = -1;
        tabPos = 0;
        openContextMenu = false;
    }

    widget_tab_resource_bar::~widget_tab_resource_bar()
    {
        if( tabExpand_button!=nullptr )
        {
            delete tabExpand_button;
            tabExpand_button = nullptr;
        }
    }

    void widget_tab_resource_bar::add_new_tab(general_resource * newTabResource)
    {
        if( newTabResource!=nullptr)
        {
            if( (int)sub_options.size() > 0 )
            {
                int i = 0;
                int foundTab = -1;
                general_resource * cGResource = nullptr;
                //makes sure the tab is not already open
                for( i = 0; i < (int)sub_options.size(); i +=1)
                {
                    cGResource = sub_options[i];
                    if( cGResource!=nullptr)
                    {
                        if( cGResource->matches( newTabResource) )
                        {
                            foundTab = i;
                            tabInUse = i;
                            move_to_tab(i);
                        }
                    }
                }
                //if tab is not already open, open it
                if(foundTab==-1)
                {
                    sub_options.push_back(newTabResource);
                    newTabResource->justOpenedThisFrame = true;
                    tabInUse = (int)sub_options.size() - 1;
                }
            }
            else
            {
                //automatically add  resource to when the tab bar is empty
                sub_options.push_back(newTabResource);
                tabInUse = 0;
                newTabResource->justOpenedThisFrame = true;
            }
            if( tabInUse < (int)sub_options.size() )
            {
                sub_options[tabInUse]->justOpenedThisFrame = true;
            }
            main_syntax_highlighter->clear_all();
        }
    }

    void widget_tab_resource_bar::close_resource_tab( std::string projectParentName, int resIdIn)
    {
        general_resource * tRes = nullptr;
        gpe::error_log->report("Removing tab... ["+projectParentName+"]["+ stg_ex::int_to_string( resIdIn ) +"]");
        main_syntax_highlighter->clear_all();
        gpe::error_log->report("Highlighter cleared...");
        for( int i = (int)sub_options.size()-1; i >=0; i--)
        {
            tRes = sub_options[i];
            if( tRes!=nullptr)
            {
                if( projectParentName==tRes->parentProjectName && tRes->get_global_rid()==resIdIn )
                {
                    close_tab(i);
                }
            }
            else
            {
                gpe::error_log->report("Removing tab value with unknown origin...");
                close_tab(i);
            }
        }
    }

    void widget_tab_resource_bar::close_tab(int tabIdToClose)
    {
        if( tabIdToClose>=0 && tabIdToClose < (int)sub_options.size() )
        {
            sub_options.erase(sub_options.begin()+tabIdToClose);
            if( tabInUse>=tabIdToClose )
            {
                move_to_tab( tabInUse - 1 );
            }
            tabToClose = -1;
            update_tabsizes();
            if( tabInUse >=0 && tabInUse < (int)sub_options.size() )
            {
                sub_options[tabInUse]->justOpenedThisFrame = true;
            }
            main_search_controller->close_finder();
            main_overlay_system->update_temporary_message("","","",0);
            main_syntax_highlighter->clear_all();
        }
    }

    void widget_tab_resource_bar::close_tabs_left(int tabIdToClose)
    {
        if( tabIdToClose>0 && tabIdToClose < (int) sub_options.size() )
        {
            for( int i = tabIdToClose-1; i >=0; i--)
            {
                sub_options.erase(sub_options.begin()+i);
            }
            tabToClose = -1;
            tabInUse = 0;
            update_tabsizes();
            if( tabInUse < (int)sub_options.size() )
            {
                sub_options[tabInUse]->justOpenedThisFrame = true;
            }
            main_search_controller->close_finder();
            main_overlay_system->update_temporary_message("","","",0);
            main_syntax_highlighter->clear_all();
        }
    }

    void widget_tab_resource_bar::close_tabs_right(int tabIdToClose)
    {
        if( tabIdToClose>=0 && tabIdToClose < (int) sub_options.size()-1 )
        {
            for( int i = (int) sub_options.size()-1; i >tabIdToClose; i--)
            {
                sub_options.erase(sub_options.begin()+i);
            }
            tabToClose = -1;
            update_tabsizes();
            if( tabInUse < (int)sub_options.size() )
            {
                sub_options[tabInUse]->justOpenedThisFrame = true;
            }
            main_search_controller->close_finder();
            main_overlay_system->update_temporary_message("","","",0);
            main_syntax_highlighter->clear_all();
        }
    }

    void widget_tab_resource_bar::close_tabs_from_project( std::string projectDirNameIn)
    {
        general_resource * tRes = nullptr;
        for( int i = (int)sub_options.size()-1; i >=0; i--)
        {
            tRes = sub_options[i];
            if( tRes!=nullptr)
            {
                if( tRes->parentProjectName==projectDirNameIn)
                {
                    close_tab(i);
                }
            }
            else
            {
                close_tab(i);
            }
        }
        main_search_controller->close_finder();
        main_overlay_system->update_temporary_message("","","",0);
        main_syntax_highlighter->highlightedTerm = nullptr;
    }

    void widget_tab_resource_bar::close_all_tabs()
    {
        tabPos = 0;
        tabToClose = -1;
        tabXHover = -1;
        tabInUse= 0;
        sub_options.clear();
        update_tabsizes();
        main_search_controller->close_finder();
        main_overlay_system->update_temporary_message("","","",0);
        main_syntax_highlighter->highlightedTerm = nullptr;

    }

    void widget_tab_resource_bar::update_tabsizes()
    {
        if( tabHeaderBox.w!=0)
        {
            //gets tab size for tabs, dependent on number of tiles
            int subOpCount = (int)sub_options.size();
            tabSize = xxLargeTabSize;
            if( subOpCount > 0)
            {
                tabSize = subOpCount*xxLargeTabSize;
                tabsPerView = subOpCount;
                if( tabSize < tabHeaderBox.w)
                {
                    tabSize = xxLargeTabSize;
                }
                else
                {
                    //lol no one wants to divide by 0 right. :-)
                    tabSize = tabHeaderBox.w/(subOpCount+1);
                    if( tabSize < miniTabSize )
                    {
                        tabSize = miniTabSize;
                    }
                    /*
                    tabsPerView = subOpCount;
                    if( tabSize < tabHeaderBox.w)
                    {
                        tabSize = extraLargeTabSize;
                    }
                    else
                    {
                        tabSize = subOpCount*largeTabSize;
                        tabsPerView = subOpCount;
                        if( tabSize < tabHeaderBox.w)
                        {
                            tabSize = largeTabSize;
                        }
                        else
                        {
                            tabSize = subOpCount*medTabSize;
                            if( tabSize < tabHeaderBox.w)
                            {
                                tabSize = medTabSize;
                            }
                            else
                            {
                                tabSize = subOpCount*smallTabSize;
                                if( tabSize < tabHeaderBox.w)
                                {
                                    tabSize = smallTabSize;
                                }
                                else
                                {
                                    tabSize = subOpCount*extraSmallSize;
                                    if( tabSize < tabHeaderBox.w)
                                    {
                                        tabSize = extraSmallSize;
                                    }
                                    else
                                    {
                                        tabsPerView = floor(tabHeaderBox.w/miniTabSize);
                                        tabSize = miniTabSize;
                                    }
                                }
                            }
                        }
                    }
                    */
                }
            }
        }
        else
        {
            tabSize = -1;
        }
    }

    general_resource * widget_tab_resource_bar::get_selected_resource()
    {
        if(tabInUse>=0 && tabInUse < (int)sub_options.size() )
        {
            general_resource * cGenResource = sub_options.at(tabInUse);
            return cGenResource;
        }
        return nullptr;
    }

    void widget_tab_resource_bar::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam )
    {
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );
        widget_basic::process_self( view_space, cam );
        add_new_tab( selectedResource );
        set_selected_gresource( nullptr );
        update_tabsizes();
        general_resource * fResource = nullptr;
        if(useDock_button && tabExpand_button!=nullptr)
        {
            tabExpand_button->set_coords(widget_box.x+widget_box.w - tabExpand_button->get_width(), widget_box.y );
            tabExpand_button->process_self();
            if( tabExpand_button->is_clicked() )
            {
                toggle_full_width();
                gpe::input->reset_all_input();
            }
        }

        if( openContextMenu )
        {
            openContextMenu = false;
            int prevTabInUse = tabInUse;
            if( prevTabInUse >=0 && prevTabInUse < (int)sub_options.size() )
            {
                fResource = sub_options.at(prevTabInUse);
                context_menu_open(gpe::input->mouse_position_x,gpe::input->mouse_position_y,256);
                main_context_menu->add_menu_option("Close",0);
                main_context_menu->add_menu_option("Close All Tabs",1);
                main_context_menu->add_menu_option("Close Other Tabs",2);
                main_context_menu->add_menu_option("Close Tabs to the Left",3);
                main_context_menu->add_menu_option("Close Tabs to the Right",4);
                main_context_menu->add_menu_option("Save",5);
                int foundResult = context_menu_process();
                switch(foundResult)
                {
                case 0:
                    tabToClose = prevTabInUse;
                    tabXHover = -1;
                    gpe::input->reset_all_input();
                    break;
                case 1:
                    close_all_tabs();
                    gpe::input->reset_all_input();
                    if( main_syntax_highlighter!=nullptr)
                    {
                        main_syntax_highlighter->highlightedTerm = nullptr;
                    }
                    break;
                case 2:
                    close_tabs_right(prevTabInUse);
                    close_tabs_left(prevTabInUse);
                    tabInUse = 0;
                    if( tabInUse < (int)sub_options.size() )
                    {
                        sub_options[tabInUse]->justOpenedThisFrame = true;
                    }
                    break;

                case 3:
                    close_tabs_left(prevTabInUse);
                    tabInUse = 0;
                    if( tabInUse < (int)sub_options.size() )
                    {
                        sub_options[tabInUse]->justOpenedThisFrame = true;
                    }
                    break;

                case 4:
                    close_tabs_right(prevTabInUse);
                    tabInUse = prevTabInUse;
                    if( tabInUse < (int)sub_options.size() )
                    {
                        sub_options[tabInUse]->justOpenedThisFrame = true;
                    }
                    break;

                case 5:
                    if( fResource!=nullptr)
                    {
                        fResource->save_resource();
                    }
                    break;

                default:
                    break;
                }
                context_menu_close();
                update_tabsizes();
            }
        }
        if( tabHeaderBox.w!=0)
        {
            bool withinTabArea = false;
            if (gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&tabHeaderBox) )
            {
                withinTabArea = true;
            }

            //Processes tab management
            tabXHover = -1;
            if(withinTabArea)
            {
                int subPos = 0;
                for(int i=tabPos; i< (int)sub_options.size() && i < tabsPerView+tabPos; i+=1)
                {
                    fResource = sub_options[i];
                    if( fResource!=nullptr)
                    {
                        if (gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,tabHeaderBox.x+(subPos)*(tabSize+tabXPAdding)+tabSize-16,tabHeaderBox.y,tabHeaderBox.x+(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.y+tabHeaderBox.h) )
                        {
                            main_overlay_system->update_tooltip("Close "+fResource->get_name() );
                            tabXHover = i;
                            if(gpe::input->check_mouse_released( mb_left))
                            {
                                tabToClose = i;
                                tabXHover = -1;
                                gpe::input->reset_all_input();
                            }
                        }
                        else if(gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,tabHeaderBox.x+subPos*(tabSize+tabXPAdding),tabHeaderBox.y,tabHeaderBox.x+(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.y+tabHeaderBox.h) )
                        {
                            main_overlay_system->update_tooltip( fResource->get_name() );
                            if(gpe::input->check_mouse_released( mb_left))
                            {
                                tabInUse = i;
                                if( tabInUse < (int)sub_options.size() )
                                {
                                    sub_options[tabInUse]->justOpenedThisFrame = true;
                                }
                                gpe::input->reset_all_input();
                            }
                            else if(gpe::input->check_mouse_released(1) )
                            {
                                tabInUse = i;
                                openContextMenu = true;
                            }
                            else if(gpe::input->check_mouse_released(2) )
                            {
                                tabToClose = i;
                                tabXHover = -1;
                                gpe::input->reset_all_input();
                            }
                        }
                    }
                    subPos+=1;
                }
            }

            if( gpe::input->check_kb_down(kb_ctrl)  )
            {
                if( gpe::input->check_kb_released(kb_w))
                {
                    tabToClose = tabInUse;
                }
                else if( gpe::input->check_kb_released(kb_tab) )
                {
                    if( gpe::input->kb_shift_pressed )
                    {
                        move_to_tab(tabInUse-1);
                    }
                    else
                    {
                        move_to_tab(tabInUse+1);
                    }
                }
            }
            else if( gpe::input->gamepad_check_button_released(0, gp_l2) )
            {
                        move_to_tab(tabInUse-1);
            }
            else if( gpe::input->gamepad_check_button_released(0, gp_r2) )
            {
                move_to_tab(tabInUse+1);
            }

            if( tabToClose>=0 && tabToClose < (int) sub_options.size() )
            {
                close_tab(tabToClose);
                tabToClose = -1;
            }

            //Proceses open tab...
            //if(!editor_gui_main->mainResourceTree->being_resized )
            {
                general_resource * cGenResource  = nullptr;
                if(tabInUse>=0 && tabInUse < (int)sub_options.size() )
                {
                    cGenResource = sub_options.at(tabInUse);
                }
                if( cGenResource!=nullptr)
                {
                    cGenResource->process_resource(&tabBox );
                }
            }
        }
    }

    void widget_tab_resource_bar::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find( view_space );
        view_space = gpe::camera_find( cam);
        //if( pawgui::theme_main->theme_texture_bg==nullptr )
        {
            gpe::gcanvas->render_roundrect_filled_color(tabHeaderBox.x, tabHeaderBox.y,tabHeaderBox.get_x2(), tabHeaderBox.get_y2(),  pawgui::theme_main->tab_background_color, 255);
            gpe::gcanvas->render_rectangle(tabHeaderBox.x, tabHeaderBox.get_middle(),tabHeaderBox.get_x2(), tabHeaderBox.get_y2(),  pawgui::theme_main->tab_background_color, false, 255);
        }
        if( (int)sub_options.size() >0 )
        {
            int subPos = 0;
            general_resource * fResource = nullptr;
            std::string tabOptionStr = "";
            int tabFontWidth = 0;
            int tabFontHeight = 0;
            int maxCharactersAllowed = 0;
            font_tab->get_metrics("A", &tabFontWidth, &tabFontHeight );
            maxCharactersAllowed = tabSize/tabFontWidth -1;

            gpe::renderer_main->set_viewpoint(&tabHeaderBox);
            for(int i=tabPos; i< (int)sub_options.size() && i < tabsPerView+tabPos; i+=1)
            {
                fResource = sub_options[i];
                if( tabInUse==i)
                {
                    gpe::gcanvas->render_roundrect_filled_color( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h, pawgui::theme_main->tab_selected_color, 255);
                    gpe::gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),tabHeaderBox.h/2,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h,pawgui::theme_main->tab_selected_color,false);
                }
                else
                {
                    gpe::gcanvas->render_roundrect_filled_color( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h, pawgui::theme_main->tab_color, 255);
                    gpe::gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),tabHeaderBox.h/2,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h,pawgui::theme_main->tab_color, false);
                }
                if( fResource!=nullptr)
                {
                    if( fResource->is_modified() )
                    {
                        tabOptionStr = "* "+fResource->get_name();
                    }
                    else
                    {
                        tabOptionStr = fResource->get_name();
                    }
                    if( (int)tabOptionStr.size() > maxCharactersAllowed )
                    {
                        if( maxCharactersAllowed > 1 )
                        {
                            tabOptionStr = stg_ex::get_substring( tabOptionStr,0,maxCharactersAllowed-1 )+"..";
                        }
                        else
                        {
                            tabOptionStr = stg_ex::get_substring( tabOptionStr,0, 1 );
                        }
                    }

                    if( tabInUse==i)
                    {
                        gpe::gfs->render_text_clipped(subPos*(tabSize+tabXPAdding)+padding_default,tabHeaderBox.h/2,tabOptionStr,pawgui::theme_main->tab_text_color, font_tab,gpe::fa_left,gpe::fa_middle,tabSize-tabXPAdding-16);
                    }
                    else
                    {
                        gpe::gfs->render_text_clipped(subPos*(tabSize+tabXPAdding)+padding_default,tabHeaderBox.h/2,tabOptionStr,pawgui::theme_main->tab_text_color,font_tab,gpe::fa_left,gpe::fa_middle,tabSize-tabXPAdding-16);
                    }
                }

                if( tabXHover==i)
                {
                    gpe::gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding)+tabSize-default_icon_width,0,+(subPos)*(tabSize+tabXPAdding)+tabSize,+tabHeaderBox.h,gpe::c_red,false);
                }
                //gpe::gfs->render_text( (subPos)*(tabSize+tabXPAdding)+tabSize-padding_default,tabHeaderBox.h/2,"×",pawgui::theme_main->main_box_font_color,font_tab,gpe::fa_right,gpe::fa_middle);


                /*if( i!=tabInUse)
                {
                    gpe::gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h,pawgui::theme_main->popup_box_border_color,true);
                    gpe::gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize-1,tabHeaderBox.h-1,pawgui::theme_main->popup_box_border_color,true);
                }*/
                subPos+=1;
            }

            gpe::renderer_main->reset_viewpoint();
            //if( forceRedraw )
            if( useDock_button )
            {
                tabExpand_button->render_self(nullptr, nullptr );
            }

            if(tabInUse>=0 && tabInUse < (int)sub_options.size() )
            {
                general_resource * cGenResource = sub_options.at(tabInUse);
                if( cGenResource!=nullptr)
                {
                    gpe::renderer_main->set_viewpoint( &tabBox );
                    cGenResource->render_resource(&tabBox, nullptr);
                    gpe::renderer_main->reset_viewpoint();
                }
            }
        }
        else
        {
            gpe::renderer_main->reset_viewpoint();
            gpe::renderer_main->set_viewpoint( &tabBox );

            if( pawgui::theme_main->theme_texture_bg==nullptr)
            {
                //gpe::gcanvas->render_rectangle( 0, 0, tabBox.w, tabBox.h, pawgui::theme_main->program_color,false);
            }
            gpe::gfs->render_text(tabBox.get_center(), tabBox.get_middle() - 16 ,"No Tabs currently open",pawgui::theme_main->popup_box_font_color, font_tab, gpe::fa_center, gpe::fa_middle );
            gpe::gfs->render_text( tabBox.get_center(), tabBox.get_middle() + 16,"Press CTRL+O to open a project",pawgui::theme_main->popup_box_font_color, font_tab, gpe::fa_center, gpe::fa_middle );
        }
        //gpe::gcanvas->render_rect(&tabBox,pawgui::theme_main->text_box_outline_color,true);
        gpe::renderer_main->reset_viewpoint();
    }

    bool widget_tab_resource_bar::requests_fullscreen()
    {
        return isExpanded;
        if( tabInUse>=0 && tabInUse < (int)sub_options.size())
        {
            if( sub_options[tabInUse]!=nullptr )
            {
                return sub_options[tabInUse]->isFullScreenResource;
            }
        }
        return false;
    }

    int widget_tab_resource_bar::search_for_string(std::string needle)
    {
        int foundstrings = 0;
        general_resource * fOption  = nullptr;
        for( int i = (int)sub_options.size()-1; i >=0; i--)
        {
            fOption = sub_options[i];
            if( fOption!=nullptr)
            {
                foundstrings+=fOption->search_for_string(needle);
            }
        }

        return foundstrings;
    }

    int widget_tab_resource_bar::search_and_replace_string(std::string needle, std::string newStr )
    {
        int foundstrings = 0;
        general_resource * fOption  = nullptr;
        for( int i = (int)sub_options.size()-1; i >=0; i--)
        {
            fOption = sub_options[i];
            if( fOption!=nullptr)
            {
                foundstrings+=fOption->search_and_replace_string(needle, newStr);
            }
        }
        return foundstrings;
    }

    void widget_tab_resource_bar::set_coords(int x_new, int y_new)
    {
        if( x_new!=-1)
        {
            widget_box.x = x_new;
            tabBox.x = x_new;
            tabHeaderBox.x = x_new;
            tabBox.x = tabHeaderBox.x = widget_box.x;
            tabHeaderBox.w = widget_box.w  - tabExpand_button->get_width();
            tabBox.w = widget_box.w;
        }
        if(y_new!=-1)
        {
            widget_box.y = y_new;
            tabHeaderBox.y = y_new;
            tabBox.y = y_new+tabHeaderBox.h;
            tabBox.h = widget_box.h-tabHeaderBox.h;
        }
    }

    void widget_tab_resource_bar::set_height(int new_height)
    {
        widget_box.h = new_height;
        tabBox.h = new_height-tabHeaderBox.h;
    }

    void widget_tab_resource_bar::set_selected_gresource(general_resource *newResource)
    {
        selectedResource = newResource;
    }

    void widget_tab_resource_bar::set_width(int new_width)
    {
        tabBox.w = new_width;
        tabHeaderBox.w = new_width - tabExpand_button->get_width()  - tabExpand_button->get_width();
        widget_box.w = new_width;
    }

    void widget_tab_resource_bar::move_to_tab(int newTabId)
    {
        if( (int)sub_options.size() > 0)
        {
            tabInUse= newTabId;
            if( newTabId<0)
            {
                tabInUse = sub_options.size()-1;
            }
            if( tabInUse >=(int)sub_options.size() )
            {
                tabInUse = 0;
            }
        }
        else
        {
            tabInUse = 0;
            tabPos = 0;
        }
        main_search_controller->close_finder();
        main_overlay_system->update_temporary_message("","","",0);
        main_syntax_highlighter->highlightedTerm = nullptr;
    }

    void widget_tab_resource_bar::toggle_full_width()
    {
        isExpanded = !isExpanded;
        if( isExpanded )
        {
            tabExpand_button->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/compress.png");
            tabExpand_button->descriptionText = "Dock View";
        }
        else
        {
            tabExpand_button->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/expand.png");
            tabExpand_button->descriptionText = "Undock View";
        }
    }
}
