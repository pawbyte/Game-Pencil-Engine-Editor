/*
pawgui_tab_manager.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_tab_manager.h"
#include "pawgui_search_controller.h"
#include "pawgui_text_editor.h"

namespace pawgui
{
    widget_tab_resource_bar * main_tab_resource_bar = NULL;

    widget_tab_resource_bar::widget_tab_resource_bar()
    {
        isFullWidth = true;
        needsNewLine = true;
        useDockButton = false;
        isExpanded = false;
        tabExpandButton = new widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/expand.png","Undock View",-1,18);
        selectedResource = NULL;
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
        if( tabExpandButton!=NULL )
        {
            delete tabExpandButton;
            tabExpandButton = NULL;
        }
    }

    void widget_tab_resource_bar::add_new_tab(general_resource * newTabResource)
    {
        if( newTabResource!=NULL)
        {
            if( (int)subOptions.size() > 0 )
            {
                int i = 0;
                int foundTab = -1;
                general_resource * cGResource = NULL;
                //makes sure the tab is not already open
                for( i = 0; i < (int)subOptions.size(); i +=1)
                {
                    cGResource = subOptions[i];
                    if( cGResource!=NULL)
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
                    subOptions.push_back(newTabResource);
                    newTabResource->justOpenedThisFrame = true;
                    tabInUse = (int)subOptions.size() - 1;
                }
            }
            else
            {
                //automatically add  resource to when the tab bar is empty
                subOptions.push_back(newTabResource);
                tabInUse = 0;
                newTabResource->justOpenedThisFrame = true;
            }
            if( tabInUse < (int)subOptions.size() )
            {
                subOptions[tabInUse]->justOpenedThisFrame = true;
            }
            main_syntax_highlighter->clear_all();
        }
    }

    void widget_tab_resource_bar::close_resource_tab( std::string projectParentName, int resIdIn)
    {
        general_resource * tRes = NULL;
        gpe::error_log->report("Removing tab... ["+projectParentName+"]["+ stg_ex::int_to_string( resIdIn ) +"]");
        main_syntax_highlighter->clear_all();
        gpe::error_log->report("Highlighter cleared...");
        for( int i = (int)subOptions.size()-1; i >=0; i--)
        {
            tRes = subOptions[i];
            if( tRes!=NULL)
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
        if( tabIdToClose>=0 && tabIdToClose < (int)subOptions.size() )
        {
            subOptions.erase(subOptions.begin()+tabIdToClose);
            if( tabInUse>=tabIdToClose )
            {
                move_to_tab( tabInUse - 1 );
            }
            tabToClose = -1;
            update_tabsizes();
            if( tabInUse >=0 && tabInUse < (int)subOptions.size() )
            {
                subOptions[tabInUse]->justOpenedThisFrame = true;
            }
            main_search_controller->close_finder();
            main_overlay_system->update_temporary_message("","","",0);
            main_syntax_highlighter->clear_all();
        }
    }

    void widget_tab_resource_bar::close_tabs_left(int tabIdToClose)
    {
        if( tabIdToClose>0 && tabIdToClose < (int) subOptions.size() )
        {
            for( int i = tabIdToClose-1; i >=0; i--)
            {
                subOptions.erase(subOptions.begin()+i);
            }
            tabToClose = -1;
            tabInUse = 0;
            update_tabsizes();
            if( tabInUse < (int)subOptions.size() )
            {
                subOptions[tabInUse]->justOpenedThisFrame = true;
            }
            main_search_controller->close_finder();
            main_overlay_system->update_temporary_message("","","",0);
            main_syntax_highlighter->clear_all();
        }
    }

    void widget_tab_resource_bar::close_tabs_right(int tabIdToClose)
    {
        if( tabIdToClose>=0 && tabIdToClose < (int) subOptions.size()-1 )
        {
            for( int i = (int) subOptions.size()-1; i >tabIdToClose; i--)
            {
                subOptions.erase(subOptions.begin()+i);
            }
            tabToClose = -1;
            update_tabsizes();
            if( tabInUse < (int)subOptions.size() )
            {
                subOptions[tabInUse]->justOpenedThisFrame = true;
            }
            main_search_controller->close_finder();
            main_overlay_system->update_temporary_message("","","",0);
            main_syntax_highlighter->clear_all();
        }
    }

    void widget_tab_resource_bar::close_tabs_from_project( std::string projectDirNameIn)
    {
        general_resource * tRes = NULL;
        for( int i = (int)subOptions.size()-1; i >=0; i--)
        {
            tRes = subOptions[i];
            if( tRes!=NULL)
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
        main_syntax_highlighter->highlightedTerm = NULL;
    }

    void widget_tab_resource_bar::close_all_tabs()
    {
        tabPos = 0;
        tabToClose = -1;
        tabXHover = -1;
        tabInUse= 0;
        subOptions.clear();
        update_tabsizes();
        main_search_controller->close_finder();
        main_overlay_system->update_temporary_message("","","",0);
        main_syntax_highlighter->highlightedTerm = NULL;

    }

    void widget_tab_resource_bar::update_tabsizes()
    {
        if( tabHeaderBox.w!=0)
        {
            //gets tab size for tabs, dependent on number of tiles
            int subOpCount = (int)subOptions.size();
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
        if(tabInUse>=0 && tabInUse < (int)subOptions.size() )
        {
            general_resource * cGenResource = subOptions.at(tabInUse);
            return cGenResource;
        }
        return NULL;
    }

    void widget_tab_resource_bar::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam )
    {
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );
        widget_basic::process_self( view_space, cam );
        add_new_tab( selectedResource );
        set_selected_gresource( NULL );
        update_tabsizes();
        general_resource * fResource = NULL;
        if(useDockButton && tabExpandButton!=NULL)
        {
            tabExpandButton->set_coords(widget_box.x+widget_box.w - tabExpandButton->get_width(), widget_box.y );
            tabExpandButton->process_self();
            if( tabExpandButton->is_clicked() )
            {
                toggle_full_width();
                gpe::input->reset_all_input();
            }
        }

        if( openContextMenu )
        {
            openContextMenu = false;
            int prevTabInUse = tabInUse;
            if( prevTabInUse >=0 && prevTabInUse < (int)subOptions.size() )
            {
                fResource = subOptions.at(prevTabInUse);
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
                    if( main_syntax_highlighter!=NULL)
                    {
                        main_syntax_highlighter->highlightedTerm = NULL;
                    }
                    break;
                case 2:
                    close_tabs_right(prevTabInUse);
                    close_tabs_left(prevTabInUse);
                    tabInUse = 0;
                    if( tabInUse < (int)subOptions.size() )
                    {
                        subOptions[tabInUse]->justOpenedThisFrame = true;
                    }
                    break;

                case 3:
                    close_tabs_left(prevTabInUse);
                    tabInUse = 0;
                    if( tabInUse < (int)subOptions.size() )
                    {
                        subOptions[tabInUse]->justOpenedThisFrame = true;
                    }
                    break;

                case 4:
                    close_tabs_right(prevTabInUse);
                    tabInUse = prevTabInUse;
                    if( tabInUse < (int)subOptions.size() )
                    {
                        subOptions[tabInUse]->justOpenedThisFrame = true;
                    }
                    break;

                case 5:
                    if( fResource!=NULL)
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
                for(int i=tabPos; i< (int)subOptions.size() && i < tabsPerView+tabPos; i+=1)
                {
                    fResource = subOptions[i];
                    if( fResource!=NULL)
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
                                if( tabInUse < (int)subOptions.size() )
                                {
                                    subOptions[tabInUse]->justOpenedThisFrame = true;
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

            if( tabToClose>=0 && tabToClose < (int) subOptions.size() )
            {
                close_tab(tabToClose);
                tabToClose = -1;
            }

            //Proceses open tab...
            //if(!editor_gui_main->mainResourceTree->beingResized )
            {
                general_resource * cGenResource  = NULL;
                if(tabInUse>=0 && tabInUse < (int)subOptions.size() )
                {
                    cGenResource = subOptions.at(tabInUse);
                }
                if( cGenResource!=NULL)
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
        if( pawgui::theme_main->theme_texture_bg==NULL )
        {
            gpe::gcanvas->render_rect(&tabHeaderBox,pawgui::theme_main->program_color,false);
            gpe::gcanvas->render_rect(&tabHeaderBox,pawgui::theme_main->main_border_color,true);
        }
        if( (int)subOptions.size() >0 )
        {
            int subPos = 0;
            general_resource * fResource = NULL;
            std::string tabOptionStr = "";
            int tabFontWidth = 0;
            int tabFontHeight = 0;
            int maxCharactersAllowed = 0;
            font_tab->get_metrics("A", &tabFontWidth, &tabFontHeight );
            maxCharactersAllowed = tabSize/tabFontWidth -1;

            gpe::renderer_main->set_viewpoint(&tabHeaderBox);
            for(int i=tabPos; i< (int)subOptions.size() && i < tabsPerView+tabPos; i+=1)
            {
                fResource = subOptions[i];
                if( tabInUse==i)
                {
                    gpe::gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h,pawgui::theme_main->program_color_header,false);
                }
                else
                {
                    gpe::gcanvas->render_rectangle( subPos*(tabSize+tabXPAdding),0,(subPos)*(tabSize+tabXPAdding)+tabSize,tabHeaderBox.h,pawgui::theme_main->program_color,false);
                }
                if( fResource!=NULL)
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
                        gpe::gfs->render_text_resized(subPos*(tabSize+tabXPAdding)+padding_default,tabHeaderBox.h/2,tabOptionStr,pawgui::theme_main->popup_box_font_color, font_tab,gpe::fa_left,gpe::fa_middle,tabSize-tabXPAdding-16);
                    }
                    else
                    {
                        gpe::gfs->render_text_resized(subPos*(tabSize+tabXPAdding)+padding_default,tabHeaderBox.h/2,tabOptionStr,pawgui::theme_main->main_box_font_color,font_tab,gpe::fa_left,gpe::fa_middle,tabSize-tabXPAdding-16);
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
                gpe::gcanvas->render_horizontal_line_color(widget_box.y,widget_box.x, widget_box.x+widget_box.w, gpe::c_black );
            if( useDockButton )
            {
                tabExpandButton->render_self(NULL, NULL );
            }

            if(tabInUse>=0 && tabInUse < (int)subOptions.size() )
            {
                general_resource * cGenResource = subOptions.at(tabInUse);
                if( cGenResource!=NULL)
                {
                    gpe::renderer_main->set_viewpoint( &tabBox );
                    cGenResource->render_resource(&tabBox, NULL);
                    gpe::renderer_main->reset_viewpoint();
                }
            }
        }
        else
        {
            gpe::renderer_main->reset_viewpoint();
            gpe::renderer_main->set_viewpoint( view_space );

            if( pawgui::theme_main->theme_texture_bg==NULL)
            {
                gpe::gcanvas->render_rect(&tabBox,pawgui::theme_main->program_color,false);
            }
            gpe::gfs->render_text(view_space->w / 2, view_space->h / 2,"No Tabs currently open",pawgui::theme_main->popup_box_font_color, font_tab, gpe::fa_center, gpe::fa_middle );
            gpe::gfs->render_text(view_space->w / 2, view_space->h / 2 + 32,"Press CTRL+O to open a project",pawgui::theme_main->popup_box_font_color, font_tab, gpe::fa_center, gpe::fa_middle );
        }
        //gpe::gcanvas->render_rect(&tabBox,pawgui::theme_main->text_box_outline_color,true);
        gpe::renderer_main->reset_viewpoint();
    }

    bool widget_tab_resource_bar::requests_fullscreen()
    {
        return isExpanded;
        if( tabInUse>=0 && tabInUse < (int)subOptions.size())
        {
            if( subOptions[tabInUse]!=NULL )
            {
                return subOptions[tabInUse]->isFullScreenResource;
            }
        }
        return false;
    }

    int widget_tab_resource_bar::search_for_string(std::string needle)
    {
        int foundStrings = 0;
        general_resource * fOption  = NULL;
        for( int i = (int)subOptions.size()-1; i >=0; i--)
        {
            fOption = subOptions[i];
            if( fOption!=NULL)
            {
                foundStrings+=fOption->search_for_string(needle);
            }
        }

        return foundStrings;
    }

    int widget_tab_resource_bar::search_and_replace_string(std::string needle, std::string newStr )
    {
        int foundStrings = 0;
        general_resource * fOption  = NULL;
        for( int i = (int)subOptions.size()-1; i >=0; i--)
        {
            fOption = subOptions[i];
            if( fOption!=NULL)
            {
                foundStrings+=fOption->search_and_replace_string(needle, newStr);
            }
        }
        return foundStrings;
    }

    void widget_tab_resource_bar::set_coords(int newX, int newY)
    {
        if( newX!=-1)
        {
            widget_box.x = newX;
            tabBox.x = newX;
            tabHeaderBox.x = newX;
            tabBox.x = tabHeaderBox.x = widget_box.x;
            tabHeaderBox.w = widget_box.w  - tabExpandButton->get_width();
            tabBox.w = widget_box.w;
        }
        if(newY!=-1)
        {
            widget_box.y = newY;
            tabHeaderBox.y = newY;
            tabBox.y = newY+tabHeaderBox.h;
            tabBox.h = widget_box.h-tabHeaderBox.h;
        }
    }

    void widget_tab_resource_bar::set_height(int newHeight)
    {
        widget_box.h = newHeight;
        tabBox.h = newHeight-tabHeaderBox.h;
    }

    void widget_tab_resource_bar::set_selected_gresource(general_resource *newResource)
    {
        selectedResource = newResource;
    }

    void widget_tab_resource_bar::set_width(int newWidth)
    {
        tabBox.w = newWidth;
        tabHeaderBox.w = newWidth - tabExpandButton->get_width()  - tabExpandButton->get_width();
        widget_box.w = newWidth;
    }

    void widget_tab_resource_bar::move_to_tab(int newTabId)
    {
        if( (int)subOptions.size() > 0)
        {
            tabInUse= newTabId;
            if( newTabId<0)
            {
                tabInUse = subOptions.size()-1;
            }
            if( tabInUse >=(int)subOptions.size() )
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
        main_syntax_highlighter->highlightedTerm = NULL;
    }

    void widget_tab_resource_bar::toggle_full_width()
    {
        isExpanded = !isExpanded;
        if( isExpanded )
        {
            tabExpandButton->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/compress.png");
            tabExpandButton->descriptionText = "Dock View";
        }
        else
        {
            tabExpandButton->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/expand.png");
            tabExpandButton->descriptionText = "Undock View";
        }
    }
}
