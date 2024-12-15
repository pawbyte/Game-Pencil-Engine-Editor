/*
pawgui_dock_system.cpp
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
- P Industries <https://p.industries>*/

#include "pawgui_dock_system.h"

namespace pawgui
{
    widget_dock_panel::widget_dock_panel()
    {
        specialPanelElement = nullptr;
        panelBar = new pawgui::widget_tabbar(true);
        panelRect = new gpe::shape_rect();
        widget_box.w = 0;
        wasProcessed = false;
        panelGuiList = new pawgui::widget_panel_list();
        panelGuiList->set_horizontal_align(gpe::fa_left);
        panelGuiList->panelAlignType = pawgui::panel_align_left;
        panelGuiList->barXPadding = pawgui::padding_default;
        panelGuiList->barXMargin = 0;
        containerRect = new gpe::shape_rect();
        dock_settings_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/bars.png","Panel Settings");
    }

    widget_dock_panel::~widget_dock_panel()
    {
        if( panelBar!=nullptr )
        {
            delete panelBar;
            panelBar = nullptr;
        }

        if( dock_settings_button!=nullptr )
        {
            delete dock_settings_button;
            dock_settings_button = nullptr;
        }

        if( panelRect!=nullptr )
        {
            delete panelRect;
            panelRect = nullptr;
        }

        if( containerRect!=nullptr )
        {
            delete containerRect;
            containerRect = nullptr;
        }

        if( panelGuiList!=nullptr )
        {
            delete panelGuiList;
            panelGuiList = nullptr;
        }
    }

    bool widget_dock_panel::add_container( std::string name, bool openNew )
    {
        if( (int)name.size() > 0  )
        {
            panelBar->add_new_tab( name, openNew );
            return true;
        }
        return false;
    }

    void widget_dock_panel::add_gui_element(pawgui::widget_basic * element, bool isNLElement)
    {
        if( panelGuiList!=nullptr )
        {
            panelGuiList->add_gui_element( element, isNLElement);
        }
    }

    void widget_dock_panel::add_gui_auto(pawgui::widget_basic * element)
    {
        if( panelGuiList!=nullptr )
        {
            panelGuiList->add_gui_auto( element );
        }
    }

    void widget_dock_panel::add_gui_element_fullsize(pawgui::widget_basic * element )
    {
        if( panelGuiList!=nullptr && element!=nullptr)
        {
            panelGuiList->add_gui_element_fullsize( element);
        }
    }

    void widget_dock_panel::add_gui_section( widget_panel_section *  element )
    {
        if( panelGuiList!=nullptr && element!=nullptr)
        {
            panelGuiList->add_gui_section( element);
        }
    }

    void widget_dock_panel::clear_panel()
    {
        if( panelGuiList!=nullptr )
        {
            panelGuiList->clear_list();
        }
        specialPanelElement = nullptr;
        wasProcessed = false;
    }

    bool widget_dock_panel::container_in_view( std::string name )
    {
        if( panelBar!=nullptr )
        {
            if( panelBar->get_selected_name() == name )
            {
                return true;
            }
        }
        return false;
    }

    bool widget_dock_panel::contains_tab( std::string name )
    {
        if( panelBar!=nullptr )
        {
            if( panelBar->contains_tab( name ) )
            {
                return true;
            }
        }
        return false;
    }

    int widget_dock_panel::get_panel_height()
    {
        return panelRect->h;
    }

    std::string widget_dock_panel::get_selected_container()
    {
        if( panelBar!=nullptr )
        {
            return panelBar->get_selected_name();
        }
        return "";
    }

    int widget_dock_panel::get_tab_count()
    {
        if( panelBar!=nullptr )
        {
            return panelBar->get_tab_count();
        }
        return 0;
    }

    std::string widget_dock_panel::get_tab_name( int pos )
    {
        if( panelBar==nullptr || pos < 0 )
        {
            return "";
        }

        if( pos < panelBar->get_tab_count() )
        {
            return panelBar->get_tab_at( pos );
        }
        return "";
    }

    bool widget_dock_panel::has_content()
    {
        if( specialPanelElement !=nullptr )
        {
            return true;
        }
        else if( panelBar!=nullptr)
        {
            return ( panelBar->get_tab_count() > 0 );
        }
        return false;
    }

    void widget_dock_panel::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam  )
    {
        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );
        if( panelBar!=nullptr)
        {
            panelRect->x = widget_box.x +4;
            panelRect->y = widget_box.y + panelBar->get_height() +4;
            panelRect->w = widget_box.w -8;
            panelRect->h = widget_box.h - panelBar->get_height() -8;

            setup_panel( false, false );

            if( wasProcessed == false )
            {
                if( specialPanelElement!=nullptr )
                {
                    specialPanelElement->set_coords( widget_box.x + 4, widget_box.y +4 );
                    specialPanelElement->set_width( widget_box.w -4);
                    specialPanelElement->set_height( widget_box.h -4 );
                    specialPanelElement->process_self( view_space, cam );
                }
                else
                {
                    //For normal tabs
                    //Processes the gui element list
                    if( panelGuiList!=nullptr )
                    {
                        panelGuiList->set_coords( 4, 4 );
                        panelGuiList->set_width( panelRect->w  -4);
                        panelGuiList->set_height( panelRect->h -4);
                        panelGuiList->barXMargin = pawgui::padding_default;
                        panelGuiList->barYMargin = pawgui::padding_default;
                        panelGuiList->barXPadding = pawgui::padding_default;
                        panelGuiList->barYPadding = pawgui::padding_default;
                        panelGuiList->process_self( panelRect, cam  );
                    }

                    if( panelBar!=nullptr )
                    {
                        panelBar->process_self( view_space, cam);
                    }
                    if( dock_settings_button!=nullptr )
                    {
                        dock_settings_button->process_self( view_space, cam);
                    }
                }
            }
            wasProcessed = true;
        }
    }

    bool widget_dock_panel::remove_container( std::string name )
    {
        if( (int)name.size() > 0 )
        {
            if( panelBar!=nullptr )
            {
                if( panelBar->get_selected_name()==name )
                {
                    panelGuiList->reset_self();
                }
                return panelBar->remove_tab( name );
            }
        }
        return false;
    }

    void widget_dock_panel::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam   )
    {
        if( has_content() == false)
        {
            //return;
        }

        view_space = gpe::camera_find( view_space );
        cam = gpe::camera_find( cam );
        gpe::renderer_main->reset_viewpoint();
        gpe::renderer_main->set_viewpoint( view_space);

        gpe::gcanvas->render_roundrect_filled_color(widget_box.x, widget_box.y, widget_box.get_x2(), widget_box.get_y2(), pawgui::theme_main->panel_border_color, 255 );
        gpe::gcanvas->render_roundrect_filled_color(widget_box.x+4, widget_box.y+4, widget_box.get_x2()-4, widget_box.get_y2()-4, pawgui::theme_main->panel_color, 255 );
        //gpe::gcanvas->render_rectangle(panelRect->x, panelRect->y, panelRect->x + panelRect->w, panelRect->y + 4, pawgui::theme_main->panel_border_color, false, 255 );

        if( specialPanelElement!=nullptr )
        {
            specialPanelElement->render_self( nullptr,nullptr );
        }
        else
        {
            gpe::renderer_main->reset_viewpoint();
            gpe::renderer_main->set_viewpoint( panelRect );
            //gpe::gcanvas->render_roundrect_filled_color(0, 0, panelRect->w, panelRect->h, pawgui::theme_main->panel_border_color, 255 );
            //gpe::gcanvas->render_roundrect_filled_color(4, 4, panelRect->w-4, panelRect->h-4, pawgui::theme_main->panel_color, 255 );
            //gpe::gcanvas->render_rectangle(0, 0, panelRect->w-4, 4, pawgui::theme_main->panel_border_color, false, 255 );

            if( panelGuiList!=nullptr )
            {
                panelGuiList->render_self( panelRect, cam );
            }
            gpe::renderer_main->reset_viewpoint();

            if( panelBar!=nullptr  )
            {
                panelBar->render_self( nullptr, nullptr );
            }
            if( dock_settings_button!=nullptr )
            {
                dock_settings_button->render_self( nullptr, nullptr );
            }

        }
    }

    void widget_dock_panel::reset_panel()
    {
        specialPanelElement = nullptr;
        if( panelGuiList!=nullptr )
        {
            panelGuiList->reset_self();
        }

        if( panelBar!=nullptr )
        {
            panelBar->remove_all_tabs();
        }
    }

    bool widget_dock_panel::save_panel_data_to_file(std::ofstream * fileTarget)
    {
        if( fileTarget!=nullptr && fileTarget->is_open() )
        {
            *fileTarget << "#    Created automatically via the Game Pencil Engine Editor \n";
            return true;
        }
        return false;
    }

    void widget_dock_panel::setup_panel(  bool addSpacing, bool clearList  )
    {
        panelRect->x = widget_box.x +4;
        panelRect->y = widget_box.y+panelBar->get_height();
        panelRect->w = widget_box.w -4;
        panelRect->h = widget_box.h - panelBar->get_height() -4;

        if( specialPanelElement!=nullptr )
        {
            specialPanelElement->set_coords(widget_box.x +4, widget_box.y +4);
            specialPanelElement->set_width( widget_box.w -4 );
            specialPanelElement->set_height( widget_box.h-4 );
        }
        else if( panelGuiList!=nullptr )
        {
            panelGuiList->set_horizontal_align( gpe::fa_left );
            panelGuiList->set_coords( panelRect->x, panelRect->y );
            panelGuiList->set_width( panelRect->w );
            panelGuiList->set_height( panelRect->h );
            if( clearList )
            {
                panelGuiList->clear_list();
            }
            if( addSpacing )
            {
                panelGuiList->barXPadding = pawgui::padding_default;
                panelGuiList->barYPadding = pawgui::padding_default;
                panelGuiList->barXMargin = pawgui::padding_default;
                panelGuiList->barYMargin = pawgui::padding_default;
            }
        }

        if( panelBar!=nullptr)
        {
            panelBar->set_width( widget_box.w - dock_settings_button->get_height() - pawgui::padding_default );
            panelBar->set_coords( widget_box.x +4 , widget_box.y +4);

            if( dock_settings_button!=nullptr )
            {
                if( wasProcessed==false )
                {
                    dock_settings_button->set_clicked( false );
                }
                dock_settings_button->set_coords( panelBar->get_x2() + pawgui::padding_default, panelBar->get_ypos() );
                dock_settings_button->set_width(  panelBar->get_height() );
                dock_settings_button->set_height(  panelBar->get_height() );
            }
        }
        else if( dock_settings_button!=nullptr )
        {
            if( wasProcessed==false )
            {
                dock_settings_button->set_clicked( false );
            }
            dock_settings_button->set_coords(widget_box.w - 32 - pawgui::padding_default, 0 );
            dock_settings_button->set_width(  32 );
            dock_settings_button->set_height(  32 );
        }
    }

    widget_dock::widget_dock()
    {
        panel_x_count = 5;
        panel_y_count = 3;
        panel_max_count = panel_x_count * panel_y_count;

        dock_updated_this_frame = false;
        dock_minimum_column_percentage = 5;
        dock_minimum_row_percentage = 5;

        int x_panel_numb = 0;
        int y_panel_numb = 0;
        int iPanel = 0;
        widget_dock_panel * temp_panel = nullptr;

        for( x_panel_numb = 0; x_panel_numb < panel_x_count; x_panel_numb++ )
        {
            panel_width_percentage.push_back(  100.f / (float)panel_x_count );
            panel_column_width.push_back( dock_minimum_column_percentage * panel_width_percentage[ x_panel_numb ] );

            for( y_panel_numb = 0; y_panel_numb < panel_y_count; y_panel_numb++ )
            {
                panel_height_percentages.push_back( 100.f/ (float)panel_y_count );
                panels_default_names.push_back( "["+stg_ex::int_to_string( x_panel_numb )+","+stg_ex::int_to_string( y_panel_numb ) +"]" );
                for( iPanel = 0; iPanel < panel_max_count; iPanel++ )
                {
                    gpe::error_log->report("Adding Pane["+ stg_ex::int_to_string(iPanel) +"]..");
                    temp_panel = new widget_dock_panel();
                    panels.push_back( temp_panel );
                }
            }
        }

        panel_primary_id_x = 2;
        panel_primary_id_y = 1;
        panel_primary_id = panel_y_count * panel_primary_id_x + panel_primary_id_y;

        dock_padding_horizontal = 16;
        dock_padding_vertical = 16;

        is_hidden = false;
        column_being_resized_id = -1;
        row_being_resized_id = -1;
        hori_resize_start_x = 0;
        resize_pane_x1 = 0; resize_pane_x2 = 0;
        just_resized = being_resized = horizontal_resize = vertical_resize = false;


        gpe::error_log->report( "Attempt to create ["+ stg_ex::int_to_string(panel_max_count)+"] panels...");
        reset_dock();
    }

    widget_dock::~widget_dock()
    {
        clear_defaults();

        widget_dock_panel * temp_panel = nullptr;

        for( int i =(int)panels.size() -1;  i >=0; i--)
        {
            temp_panel = panels[i];
            if(temp_panel != nullptr )
            {
                delete temp_panel;
                temp_panel = nullptr;
            }
            panels[i] = nullptr;
        }
        panels.clear();
        panels_default_names.clear();
        panel_column_width.clear();
        panel_width_percentage.clear();
        panel_height_percentages.clear();
    }


    void widget_dock::add_default_panel( std::string name, int panelId, bool open )
    {
        gpe::key_pair * newPair = new gpe::key_pair( panelId, name, name, panelId );
        default_panels.push_back( newPair);
        if( open)
        {
            add_to_panel( name, panelId, open );
        }
    }

    void widget_dock::add_to_panel( std::string name, int panelId, bool open )
    {
        if( panelId >=0 && panelId < panel_max_count && (int)name.size() > 0 )
        {
            //Removes container from all other panels
            widget_dock_panel *  currentPanel = nullptr;
            widget_dock_panel *  otherPanel = nullptr;
            for( int iPanel = 0; iPanel < panel_max_count; iPanel++ )
            {
                currentPanel = panels[iPanel];
                if( currentPanel!=nullptr )
                {
                    currentPanel->remove_container( name );
                }
            }
            //Resizes the width to dock_minimum_column_percentage * dock_width_minus_column_padding if its "unopen"
            int assignedColumn = get_column( panelId );
            if( assignedColumn < 0 )
            {
                return;
            }
            currentPanel = panels[panelId];
            if( panelId < panel_x_count )
            {
                otherPanel = panels[panelId + panel_x_count];
            }
            else
            {
               otherPanel = panels[ panelId - panel_x_count];
            }
            if( currentPanel == nullptr || otherPanel == nullptr )
            {
                return;
            }
            //panels[assignedColumn + panel_x_count];

            //If we are adding this panel to an empty column
            if( panel_width_percentage[assignedColumn] < dock_minimum_column_percentage )
            {
                panel_width_percentage[assignedColumn] = dock_minimum_column_percentage;
            }

            currentPanel->add_container( name, open );

            if( !currentPanel->has_content() && !otherPanel->has_content() )
            {
                if( panelId < panel_x_count )
                {
                    panel_height_percentages[panelId ] = 100.f;
                    panel_height_percentages[panelId + panel_x_count ] = 0.0;
                }
                else
                {
                    panel_height_percentages[panelId ] = 100.f;
                    panel_height_percentages[panelId - panel_x_count ] = 0.0;
                }
            }
            else if( panel_height_percentages[panelId ] <= dock_minimum_row_percentage )
            {
                //If our panel is on the top
                if( panelId < panel_x_count )
                {
                    panel_height_percentages[panelId ] = dock_minimum_row_percentage;
                    panel_height_percentages[panelId + panel_x_count ] = 80.f;
                }
                else
                {
                    //If our panel is on the bottom,
                    panel_height_percentages[panelId ] = dock_minimum_row_percentage;
                    panel_height_percentages[panelId - panel_x_count ] = 80.f;
                }
            }
        }
    }

    void widget_dock::clear_all_panels()
    {
        widget_dock_panel * temp_panel = nullptr;
        for( int i = 0; i < panel_max_count; i++)
        {
            temp_panel = panels[i];
            if( temp_panel!=nullptr)
            {
                temp_panel->clear_panel();
            }
        }
    }

    void widget_dock::clear_defaults()
    {
        gpe::key_pair * tempPair = nullptr;
        for( int i = (int)default_panels.size()-1; i >=0; i--)
        {
            tempPair = default_panels[i];
            if( tempPair !=nullptr)
            {
                delete tempPair;
                tempPair = nullptr;
            }
        }
        default_panels.clear();
    }

    bool widget_dock::column_is_open( int col_numb )
    {
        if( col_numb < 0 || col_numb >= panel_x_count )
        {
            return false;
        }

        widget_dock_panel * temp_panel = nullptr;
        for(  int iRow = 0; iRow < panel_y_count;  iRow++)
        {
            temp_panel = panels[ col_numb * panel_y_count + iRow ];
            if( temp_panel!=nullptr && temp_panel->has_content() )
            {
                return true;
            }
        }
        return false;
    }

    bool widget_dock::container_in_view( std::string name )
    {
        widget_dock_panel *  tPanel = nullptr;
        for( int iPanel = 0; iPanel < panel_max_count; iPanel++ )
        {
            tPanel = panels[iPanel];
            if( tPanel!=nullptr && tPanel->container_in_view(name) )
            {
                return true;
            }
        }
        return false;
    }

    widget_dock_panel * widget_dock::contains_panel( std::string name )
    {
        widget_dock_panel *  tPanel = nullptr;
        for( int iPanel = 0; iPanel < panel_max_count; iPanel++ )
        {
            tPanel = panels[iPanel];
            if( tPanel!=nullptr && tPanel->contains_tab(name) )
            {
                return tPanel;
            }
        }
        return nullptr;
    }

    widget_dock_panel * widget_dock::find_panel( std::string name )
    {
        widget_dock_panel *  tPanel = nullptr;
        for( int iPanel = 0; iPanel < panel_max_count; iPanel++ )
        {
            tPanel = panels[iPanel];
            if( tPanel!=nullptr && tPanel->container_in_view(name) )
            {
                return tPanel;
            }
        }
        return nullptr;
    }

    int widget_dock::get_column(int panelId)
    {
        if( panelId < panel_x_count )
        {
            return panelId;
        }
        else if( panelId < panel_max_count )
        {
            return panelId - panel_x_count;
        }
        return -1;
    }

    int widget_dock::get_panel_x_count()
    {
        return panel_x_count;
    }

    int widget_dock::get_dock_left_width()
    {
        if( panel_primary_id_x > 0 )
        {
            int calculatedPanelWidth = 0;
            for( int i = 0; i < panel_primary_id_x; i++)
            {
                calculatedPanelWidth += panel_column_width[i];
            }
            return calculatedPanelWidth;
        }
        return 0;
    }

    int widget_dock::get_dock_right_width()
    {
    {
        if( panel_primary_id_x <  panel_x_count )
        {
            int calculatedPanelWidth = 0;
            for( int i = panel_x_count - 1; i > panel_primary_id_x ; i--)
            {
                calculatedPanelWidth += panel_column_width[i];
            }
            return calculatedPanelWidth;
        }
        return 0;
    }
    }

    widget_dock_panel * widget_dock::get_panel( int panel_id )
    {
        if( panel_id >=0 && panel_id < panel_max_count )
        {
            return panels[ panel_id ];
        }
        return nullptr;
    }

    int widget_dock::get_panel_count()
    {
        return panel_max_count;
    }

    int widget_dock::get_primary_panel()
    {
        return  panel_primary_id;
    }

    int widget_dock::get_panel_y_count()
    {
        return panel_y_count;
    }

    bool widget_dock::handle_resizing()
    {

        bool panel_above_found = false, panel_below_found = false;
        widget_dock_panel * temp_panel = nullptr;
        widget_dock_panel * temp_panel_other = nullptr;
        int x_panel_numb = 0, y_panel_numb = 0, y_panel_numb_other = 0;

        //Starts off checking if the user wishes to end resize
        if( being_resized )
        {
            if( gpe::input->check_kb_released( kb_esc )  || gpe::input->window_input_received || gpe::input->check_mouse_released( mb_middle)  || gpe::input->check_mouse_released( mb_right) )
            {
                just_resized = being_resized = horizontal_resize = vertical_resize = false;
                hori_resize_start_x = 0;
                column_being_resized_id = -1;
                row_being_resized_id = -1;
                resize_pane_x1 = resize_pane_x2 = 0;
                return true;
            }
            else if( gpe::point_between_rect( gpe::input->mouse_position_x, gpe::input->mouse_position_y, &widget_box ) )
            {
                //The above if makes sure we are actually in the dock, avoiding negative results
                //Checks if we
                bool resizeExited = false;
                float panel_resize_difference = 0;

                if( horizontal_resize )
                {
                    gpe::cursor_main_controller->cursor_change( "sizewe" );

                    //The code, below may appear unoptimized, but it rarely is executed since it only happens on ending of resize of dock...
                    if( gpe::input->check_mouse_released( mb_left) )
                    {
                        //Last check to make sure we are not out of bounds, we only resize from column 1 and to the right tho...
                        if( column_being_resized_id > 0 && column_being_resized_id < panel_x_count )
                        {
                            widget_dock_panel * temp_panel = nullptr;
                            widget_dock_panel * foundBPanel = nullptr;
                            int currentpanel_x = 0;
                            int lastPanelWidth = 0;
                            int lastLeftPanel = -1;
                            int panelsUsedOnLeft = 0;
                            int i_col = 0;
                            //Let's add up the past columsn to see where our current colum should be
                            for( i_col = 0; i_col < column_being_resized_id; i_col++)
                            {
                                if( panel_column_width[i_col] > 0 && column_is_open( i_col) )
                                {
                                    lastLeftPanel = i_col;
                                    lastPanelWidth = panel_column_width[i_col];
                                    currentpanel_x+= lastPanelWidth + pawgui::padding_default;
                                    panelsUsedOnLeft++;
                                }
                            }

                            //Resize to the left...
                            if( gpe::input->mouse_position_x < currentpanel_x  &&  panelsUsedOnLeft > 0 && lastLeftPanel >=0  )
                            {
                                //We will move to the left
                                //If the mouse is to the right of the last panel to the left
                                if( gpe::input->mouse_position_x > currentpanel_x - lastPanelWidth - pawgui::padding_default  )
                                {
                                    //Only going to interact with the panel on the immediate left
                                    panel_resize_difference = (currentpanel_x - gpe::input->mouse_position_x - pawgui::padding_default );

                                    panel_resize_difference = (float)panel_resize_difference / (  dock_width_minus_column_padding / 100.f);
                                    if( panel_resize_difference >= 0.0001 )
                                    {
                                        panel_width_percentage[ column_being_resized_id ] +=  panel_resize_difference;
                                        panel_width_percentage[ lastLeftPanel ] -= panel_resize_difference;
                                    }
                                }
                                else
                                {
                                    //A more complex resize operation not needed for 1.3.0 RC

                                }
                            }
                            //Move the panel to the right, where possible.
                            else if( panelsUsedOnLeft > 0 && lastLeftPanel >=0  )
                            {
                                //We will add to the panel
                                if( gpe::input->mouse_position_x > currentpanel_x )
                                {
                                    panel_resize_difference = ( gpe::input->mouse_position_x - currentpanel_x );

                                    //Panel resizing only is between two panels, so let's calculate the differences;
                                    panel_resize_difference = (float)panel_resize_difference / (  dock_width_minus_column_padding / 100.00000);
                                    if( panel_resize_difference >= 0.0001 )
                                    {
                                        panel_width_percentage[ column_being_resized_id ] -=  panel_resize_difference;
                                        panel_width_percentage[ lastLeftPanel ] += panel_resize_difference;
                                    }
                                }
                                else
                                {
                                    //A more complex resize operation not needed for 1.5.0 RC
                                }
                            }
                        }
                        resizeExited = true;
                        hori_resize_start_x = 0;
                        column_being_resized_id = row_being_resized_id = -1;
                        being_resized = horizontal_resize = vertical_resize = false;
                    }
                }
                else if( vertical_resize )
                {
                    gpe::cursor_main_controller->cursor_change( "sizens" );
                    if( gpe::input->check_mouse_released( mb_left) )
                    {
                        //Last check to make sure we are not out of bounds...
                        if( gpe::input->mouse_position_y > widget_box.y && gpe::input->mouse_position_y < widget_box.get_y2())
                        {
                            if( column_being_resized_id >= 0 && column_being_resized_id < panel_x_count )
                            {
                                if( row_being_resized_id >= 0 && row_being_resized_id < panel_y_count )
                                {
                                    float panel_percentages_before = 0;
                                    float panel_percentages_after = 0;
                                    int current_panel_id = 0, other_panel_id = 0;
                                    int rows_before_position = 0,  rows_after_position = 0;
                                    int first_row_below = -1;

                                    current_panel_id = column_being_resized_id * panel_y_count + row_being_resized_id ;


                                    for( y_panel_numb = 0; y_panel_numb < panel_y_count; y_panel_numb++ )
                                    {
                                        other_panel_id = column_being_resized_id * panel_y_count + y_panel_numb;
                                        temp_panel = panels[other_panel_id ];
                                        if( temp_panel!=nullptr && temp_panel->has_content() )
                                        {
                                            if( y_panel_numb < row_being_resized_id )
                                            {
                                                panel_percentages_before += panel_height_percentages[other_panel_id ];
                                                rows_before_position++;
                                            }
                                            else if( y_panel_numb > row_being_resized_id)
                                            {
                                                panel_percentages_after += panel_height_percentages[other_panel_id ];
                                                rows_after_position++;

                                                if( first_row_below < 0 )
                                                {
                                                    first_row_below = y_panel_numb;
                                                }
                                            }
                                        }
                                    }

                                    //Since we are resizing only the current row and the one below it, we make sure we can actually resize
                                    if( first_row_below >= 0  && first_row_below < panel_y_count )
                                    {
                                        other_panel_id = column_being_resized_id * panel_y_count + first_row_below;
                                        float found_y_percentage =   floor( ((float) ( gpe::input->mouse_position_y - widget_box.y ) / widget_box.h  ) * 100.f );

                                        //Checks if we are below the start of our panel
                                        if( found_y_percentage > panel_percentages_before )
                                        {
                                            found_y_percentage -= panel_percentages_before;

                                            //If the mouse position is taller than the current panel
                                            if( found_y_percentage > panel_height_percentages[current_panel_id] )
                                            {
                                                found_y_percentage -= panel_height_percentages[current_panel_id];

                                                //If we are only smaller than the next row, we will resize
                                                if( found_y_percentage < panel_height_percentages[other_panel_id] )
                                                {
                                                    panel_resize_difference = floor( panel_height_percentages[other_panel_id] - found_y_percentage );

                                                    panel_height_percentages[other_panel_id] -= panel_resize_difference;
                                                    panel_height_percentages[current_panel_id] = found_y_percentage;
                                                }
                                            }
                                            else if( found_y_percentage > 0.00000000001f )
                                            {
                                                panel_resize_difference = panel_height_percentages[current_panel_id] - found_y_percentage;
                                                panel_height_percentages[current_panel_id] = found_y_percentage;
                                                panel_height_percentages[other_panel_id] += panel_resize_difference;

                                            }
                                        }
                                    }
                                }
                            }
                        }

                        resizeExited = true;
                        hori_resize_start_x = 0;
                        column_being_resized_id = row_being_resized_id = -1;
                        being_resized = horizontal_resize = vertical_resize = false;
                        resize_pane_x1 = 0;
                        resize_pane_x2 = 0;
                    }
                }
                else
                {
                    resizeExited = true;
                    hori_resize_start_x = 0;
                    column_being_resized_id = -1;
                    row_being_resized_id = -1;
                    being_resized = horizontal_resize = vertical_resize = false;
                }
                if( resizeExited)
                {
                    return true;
                }
            }
            else
            {
                //Exits the resize if we get out of range
                just_resized = being_resized = horizontal_resize = vertical_resize = false;
                resize_pane_x1 = resize_pane_x2 = 0;
                column_being_resized_id = -1;
                row_being_resized_id = -1;
                hori_resize_start_x = 0;
                return false;
            }
        }
        else
        {
            //If we started off not being resized we will check if user wants to resize
            int panel_x = 0;
            resize_pane_x1 = resize_pane_x2 = 0;
            for( x_panel_numb = 0; x_panel_numb < panel_x_count; x_panel_numb++)
            {
                //We will 1st check for horizonal resizing
                if( panel_column_width[x_panel_numb] > 0 )
                {
                    //Checks to see if user is attempting to resize panels horizontally
                    if( !horizontal_resize )
                    {
                        //Unlike vertical check, we only require one panel to be visible(top or bottom)
                        if( gpe::point_between( gpe::input->mouse_position_x, gpe::input->mouse_position_y, panel_x - dock_padding_horizontal,widget_box.y, panel_x, widget_box.get_y2() ) )
                        {
                            gpe::cursor_main_controller->cursor_change( "sizewe" );
                            if(gpe::input->check_mouse_pressed( mb_left ) )
                            {
                                column_being_resized_id = x_panel_numb;
                                horizontal_resize = being_resized = true;
                                vertical_resize = false;
                                hori_resize_start_x = gpe::input->mouse_position_x;
                                if( pawgui::main_overlay_system!=nullptr )
                                {
                                    pawgui::main_overlay_system->take_frozen_screenshot();
                                }
                                return true;
                            }
                        }
                    }

                    panel_below_found = false;
                    //Next we check for vertical resizing in same column
                    for( y_panel_numb = 0; y_panel_numb < panel_y_count; y_panel_numb++)
                    {
                        temp_panel = panels[x_panel_numb * panel_y_count + y_panel_numb];
                        if( temp_panel!=nullptr && temp_panel->has_content() )
                        {
                            //Checks to see if user is attempting to resize panels vertically
                            //NS resize
                            if( gpe::point_between(gpe::input->mouse_position_x, gpe::input->mouse_position_y,temp_panel->get_xpos(),temp_panel->get_y2(), temp_panel->get_x2(),temp_panel->get_y2()+dock_padding_vertical ) )
                            {
                                for( y_panel_numb_other = y_panel_numb +1; y_panel_numb_other < panel_y_count; y_panel_numb_other++)
                                {
                                    temp_panel_other = panels[x_panel_numb * panel_y_count + y_panel_numb_other];
                                    if( temp_panel_other!=nullptr && temp_panel_other->has_content() )
                                    {
                                        panel_below_found = true;
                                    }
                                }

                                if( panel_below_found )
                                {
                                    gpe::cursor_main_controller->cursor_change( "sizens" );
                                    if(gpe::input->check_mouse_pressed( mb_left ) )
                                    {
                                        resize_pane_x1 = temp_panel->get_xpos();
                                        resize_pane_x2 = temp_panel->get_x2();
                                        column_being_resized_id = x_panel_numb;
                                        row_being_resized_id = y_panel_numb;
                                        vertical_resize = being_resized = true;
                                        horizontal_resize = false;
                                        hori_resize_start_x = 0;
                                        if( pawgui::main_overlay_system!=nullptr )
                                        {
                                            pawgui::main_overlay_system->take_frozen_screenshot();
                                        }
                                        return true;
                                    }
                                }
                            }

                        }
                    }
                }

                //IF the above else has not kicked in, we add to panel_x for next column
                if( panel_column_width[x_panel_numb] > 0 )
                {
                    panel_x += panel_column_width[x_panel_numb] + dock_padding_horizontal;
                }
            }
        }
        return being_resized;
    }

    void widget_dock::hide_dock()
    {
        for( int i = 0; i < panel_max_count; i++ )
        {
            if( panels[i] !=nullptr )
            {
                panels[i]->clear_panel();
            }
        }
        is_hidden = true;
    }

    bool widget_dock::get_hidden()
    {
        return is_hidden;
    }

    bool widget_dock::load_dock_setings( std::string filename )
    {
        //showStatupTipsBox->set_clicked( editor_gui_main->showTipsAtStartUp );
        std::string otherColContainerName = "";

        std::ifstream gameResourceFileIn( filename.c_str() );

        gpe::error_log->report("Loading Local Dock settings - "  + filename);
        //If the file failed to read we return false
        if( gameResourceFileIn.fail() )
        {
            gpe::error_log->report("Failed to open dock settings!");
            return false;
        }
        //makes sure the file is open
        if ( !gameResourceFileIn.is_open())
        {
            gpe::error_log->report("Failed to read dock settings!");
            return false;
        }

        int iLoop = 0;
        int equalPos = 0;

        std::string firstChar="";
        std::string key_string="";
        std::string valstring="";
        std::string subValstring="";
        std::string currLine="";
        std::string currLineToBeProcessed;
        std::string colortheme_name;
        float foundFileVersion = 0;
        int panelIdNumb = 0;
        std::string panelName = "";
        bool panelOpenTab = false;
        while ( gameResourceFileIn.good() )
        {
            getline (gameResourceFileIn,currLine); //gets the next line of the file
            currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
            currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

            if( foundFileVersion <=0)
            {
                //Empty Line skipping is only allowed at the top of the file
                if(!currLineToBeProcessed.empty() )
                {
                    //Comment skipping is only allowed at the top of the file
                    if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                    {
                        //searches for an equal character and parses through the variable
                        equalPos=currLineToBeProcessed.find_first_of("=");
                        if(equalPos!=(int)std::string::npos)
                        {
                            //if the equalPos is present, then parse on through and carryon
                            key_string = currLineToBeProcessed.substr(0,equalPos);
                            valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                            if( key_string=="Version")
                            {
                                foundFileVersion = stg_ex::string_to_float(valstring);
                            }
                        }
                    }
                }
            }
            else if( foundFileVersion <= 2.f )
            {
                //Begin processing the file.
                if(!currLineToBeProcessed.empty() )
                {
                    equalPos=currLineToBeProcessed.find_first_of("=");
                    if(equalPos!=(int)std::string::npos)
                    {
                        //if the equalPos is present, then parse on through and carryon
                        key_string = currLineToBeProcessed.substr(0,equalPos);
                        valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                        //gpe::error_log->report("Detected Key:     "+key_string);
                        if( key_string == "DockHidden" )
                        {
                            is_hidden = stg_ex::string_to_bool( valstring);
                        }
                        else if( stg_ex::string_starts(key_string, "PanelWidthPercent[" ) )
                        {
                            for(  iLoop = 0; iLoop < panel_x_count; iLoop++)
                            {
                                if( key_string == "PanelWidthPercent["+ stg_ex::int_to_string(iLoop)+"]" )
                                {
                                    panel_width_percentage[ iLoop ] = abs( stg_ex::string_to_float( valstring) );
                                }
                            }
                        }
                        else if( stg_ex::string_starts(key_string, "PanelHeightPercent[" ) )
                        {
                            for(  iLoop = 0; iLoop < panel_max_count; iLoop++)
                            {
                                if( key_string == "PanelHeightPercent["+ stg_ex::int_to_string(iLoop)+"]" )
                                {
                                    panel_height_percentages[ iLoop ] = abs( stg_ex::string_to_float( valstring) );
                                }
                            }
                        }
                        else if( key_string == "PanelTab" )
                        {
                            panelIdNumb = stg_ex::split_first_int( valstring, ',' );
                            panelName = stg_ex::split_first_string( valstring, ',' );
                            if( panelName == "Resource"  || panelName == "Resources" )
                            {
                                panelName = "Resource Browser";
                            }
                            panelOpenTab = stg_ex::string_to_bool( stg_ex::split_first_string(  valstring, ',' ) );

                            if( panelIdNumb >=0 && panelIdNumb < panel_max_count )
                            {
                                if( (int)panelName.size() >=0 )
                                {
                                    add_to_panel( panelName, panelIdNumb, panelOpenTab );
                                }
                            }
                        }
                    }
                }
            }
        }
        gpe::error_log->report("Loaded in dock settings..");
        gpe::error_log->report("Setting up new dock..");
        return true;
    }

    void widget_dock::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam  )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);

        if( handle_resizing()  )
        {
            dock_updated_this_frame = true;
            setup_dock();
            return;
        }

        setup_dock( false );
        if( panel_primary_id >= 0 && panel_primary_id < panel_max_count )
        {
            if( panels[panel_primary_id]!=nullptr)
            {
                panels[panel_primary_id]->process_self( view_space, cam );
            }
        }

        //Process all of the remaining panels
        widget_dock_panel *  tPanel = nullptr;
        widget_dock_panel *  bPanel = nullptr;
        int tPanel_buttonHitID = -1;
        int iPanel = 0;
        for( iPanel = 0; iPanel < panel_max_count; iPanel++ )
        {
            if( iPanel != panel_primary_id )
            {
                tPanel = panels[iPanel];
                if( tPanel!=nullptr )
                {
                    tPanel->process_self(  view_space, cam );
                    if( tPanel->dock_settings_button!=nullptr && tPanel->dock_settings_button->is_clicked() )
                    {
                        tPanel_buttonHitID = iPanel;
                        tPanel->dock_settings_button->set_clicked( false );
                        gpe::input->reset_all_input();
                    }
                }
            }
        }

        if( tPanel_buttonHitID >=0 && tPanel_buttonHitID < panel_max_count )
        {
            //Creates a context menu to allow user to move panel content over
            pawgui::context_menu_open(gpe::input->mouse_position_x-32,gpe::input->mouse_position_y+32,128);
            for( iPanel = 0; iPanel < panel_max_count; iPanel++ )
            {
                //Prevents us from adding to main editor/log or the same panel
                if( iPanel != tPanel_buttonHitID && iPanel != panel_primary_id )
                {
                    pawgui::main_context_menu->add_menu_option("Move to "+panels_default_names[iPanel], iPanel,nullptr,-1,nullptr,true,true);
                }
            }

            int closePanelId = 128;
            pawgui::main_context_menu->add_menu_option("Close Content", closePanelId,nullptr,-1,nullptr,true,true);
            //tPanel->dock_settings_button->set_clicked( false );
            int menuSelection = pawgui::context_menu_process();
            if( menuSelection >= 0 && menuSelection < panel_max_count )
            {
                //Add content to new panel
                std::string selectedPanelName = panels[tPanel_buttonHitID]->get_selected_container();
                remove_panel( selectedPanelName );
                add_to_panel(  selectedPanelName, menuSelection, true );
            }
            else if( menuSelection == closePanelId)
            {
                //Simply removes the panel otherwise
                remove_panel( panels[tPanel_buttonHitID]->get_selected_container() );
                //Temporary note to see if this is being called
                //main_editor_log->log_general_line("Removing panel ["+ stg_ex::int_to_string(tPanel_buttonHitID)+"]");
            }
            gpe::input->reset_all_input();
        }
        dock_updated_this_frame = true;
    }


    void widget_dock::remove_default_panel( std::string name )
    {
        gpe::key_pair * tempPair = nullptr;
        for( int i = (int)default_panels.size()-1; i >=0; i--)
        {
            tempPair = default_panels[i];
            if( tempPair !=nullptr)
            {
                if( tempPair->key_string == name || tempPair->key_substring == name )
                {
                    delete tempPair;
                    tempPair = nullptr;
                    default_panels.erase( default_panels.begin()+i );
                }
            }
        }
    }

    void widget_dock::remove_panel( std::string name )
    {
        widget_dock_panel *  tPanel = nullptr;
        for( int iPanel = 0; iPanel < panel_max_count; iPanel++ )
        {
            tPanel = panels[iPanel];
            if( tPanel!=nullptr )
            {
                tPanel->remove_container( name );
            }
        }
    }

    void widget_dock::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam   )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);

        dock_updated_this_frame = false;

        if( is_hidden )
        {
            //return;
        }

        if( being_resized && column_being_resized_id >=0 )
        {
            gpe::renderer_main->reset_viewpoint();
            //if( forceRedraw )
            {
                pawgui::main_overlay_system->render_frozen_screenshot( );
            }
            if( horizontal_resize )
            {
                gpe::gcanvas->render_rectangle( gpe::input->mouse_position_x-4, get_ypos(),gpe::input->mouse_position_x+4, get_y2(), pawgui::theme_main->popup_box_border_color, false );
            }
            else if( vertical_resize)
            {
                gpe::gcanvas->render_rectangle( resize_pane_x1,gpe::input->mouse_position_y+4, resize_pane_x2, gpe::input->mouse_position_y-4, pawgui::theme_main->popup_box_border_color, false );
            }
        }
        else
        {
            gpe::renderer_main->reset_viewpoint();
            widget_dock_panel *  tPanel = nullptr;
            for( int iPanel = 0; iPanel < panel_max_count; iPanel++ )
            {
                tPanel = panels[iPanel];
                if( tPanel!=nullptr && tPanel->has_content() )
                {
                    tPanel->render_self( view_space, cam);
                }
            }
        }

        //gpe::gcanvas->render_rectangle(0,0, gpe::screen_width, gpe::screen_height, gpe::c_red, false, 128 );
    }

    void widget_dock::reset_dock()
    {
        current_column_count = 0;
        dock_width_minus_column_padding = gpe::screen_width;
        dock_minimum_column_percentage = 5;
        dock_minimum_row_percentage = 5;
        for( int x_panel_numb = 0; x_panel_numb < panel_x_count; x_panel_numb++ )
        {
            panel_width_percentage[ x_panel_numb ] = 100.f / (float)panel_x_count;
            panel_column_width[x_panel_numb] = dock_minimum_column_percentage * panel_width_percentage[ x_panel_numb ];
        }

        for( int iPanels = 0; iPanels < panel_max_count; iPanels++ )
        {
            panel_height_percentages[iPanels] = 100.f/ (float)panel_y_count;
        }

        widget_dock_panel *  tPanel = nullptr;
        for( int iPanel = 0; iPanel < panel_max_count; iPanel++ )
        {
            tPanel = panels[iPanel];
            if( tPanel!=nullptr )
            {
                tPanel->reset_panel();
            }
        }

        gpe::key_pair * tempPair;
        for( int iDefaultPane = 0; iDefaultPane < (int)default_panels.size(); iDefaultPane++)
        {
            tempPair = default_panels.at(iDefaultPane);
            if( tempPair !=nullptr)
            {
                add_to_panel( tempPair->key_string, tempPair->key_value, true );
            }
            /*delete tempPair;
            tempPair = nullptr; */
        }
    }

    bool widget_dock::resize_dock( int new_col, int new_row )
    {
        //Avoids negative sized docks
        if( new_col < 0 || new_row < 0 )
        {
            return false;
        }

        //If the dock sizes havent changed we dont do anything
        if( new_col == panel_x_count && new_row == panel_y_count )
        {
            return false;
        }

        panel_x_count = new_col;
        panel_y_count = new_row;
        panel_max_count = panel_x_count * panel_y_count;

        return true;
    }

    bool widget_dock::save_dock_setings( std::string filename )
    {
        if( (int)filename.size() == 0 )
        {
            return false;
        }

        std::ofstream newSaveDataFile( filename.c_str() );
        //If the scene file could be saved
        if( !newSaveDataFile.fail() )
        {
            //makes sure the file is open
            if (newSaveDataFile.is_open() )
            {
                newSaveDataFile << "Version=" << gpe::version_number_total << "\n";
                newSaveDataFile << "#     \n";
                newSaveDataFile << "DockHidden=" << is_hidden << "\n";
                int i = 0;


                widget_dock_panel * tPanel = nullptr;
                int jTab =0, jTabMax = 0;
                std::string jTabName = "";
                std::string selectedTabName = "";
                for( i = 0; i < panel_max_count; i++ )
                {
                    tPanel = panels[ i ];
                    if( tPanel !=nullptr && tPanel->has_content() )
                    {
                        jTabMax = tPanel->get_tab_count();
                        selectedTabName = tPanel->get_selected_container();
                        for( jTab = 0; jTab < jTabMax; jTab++ )
                        {
                            jTabName = tPanel->get_tab_name( jTab );
                            if( (int)jTabName.size() > 0 )
                            {
                                newSaveDataFile << "PanelTab="+ stg_ex::int_to_string(i)+ "," << jTabName+",";

                                if( jTabName == selectedTabName )
                                {
                                    newSaveDataFile << "1, \n";
                                }
                                else
                                {
                                     newSaveDataFile << "0, \n";
                                }
                            }
                        }
                    }
                }

                for ( i = 0; i < panel_x_count; i++)
                {
                    newSaveDataFile << "PanelWidthPercent["+ stg_ex::int_to_string(i)+ "]=" << panel_width_percentage[i] << "\n";
                }
                for ( i = 0; i < panel_max_count; i++)
                {
                    newSaveDataFile << "PanelHeightPercent["+ stg_ex::int_to_string(i)+ "]=" << panel_height_percentages[i] << "\n";
                }
                newSaveDataFile << "\n";
                newSaveDataFile.close();
                return true;
            }
        }
        return false;
    }

    bool widget_dock::set_primary_panel( int primaryXId, int primaryYId )
    {
        if( primaryXId <= 0 || primaryXId >= panel_x_count )
        {
            return false;
        }

        if( primaryYId <= 0 || primaryYId >= panel_y_count )
        {
            return false;
        }

        panel_primary_id_x = primaryXId;
        panel_primary_id_y = primaryYId;
        int primaryId = get_panel_y_count() *primaryXId + primaryYId ;

        if( panel_primary_id != primaryId )
        {
            panel_primary_id = primaryId;
        }

        return true;
    }

    void widget_dock::setup_dock( bool clear_panels_on_setup  )
    {
        if( dock_updated_this_frame )
        {
            //return;
        }
        //panels[ panel_primary_id ]->specialPanelElement = pawgui::main_tab_resource_bar;
        //panels[ DOCK_BOTTOM_MIDDLE ]->specialPanelElement = nullptr;

        dock_width_minus_column_padding = widget_box.w;
        dock_height_minus_column_padding = widget_box.h;
        current_column_count = 0;
        //checks the column max widths
        widget_dock_panel * temp_panel = nullptr;
        bool col_found = false;
        float calculated_percentages = 0.f, percentage_difference = 0.f;
        int i_col = 0, j_row = 0;
        int first_row_round  = -1;
        int current_panel_id = -1;
        int skinny_panel_id = 0, wide_panel_id = 0;
        float skinny_panel_width = 100, wide_panel_width = 0;

        int shortest_panel_id = 0, tallest_panel_id = 0;
        float shortest_panel_height = 100, tallest_panel_height = 0;
        int current_column_row_count = 0;
        for(  i_col = 0; i_col < panel_x_count; i_col++)
        {
            if( column_is_open( i_col) )
            {
                current_column_count++;
                if( panel_width_percentage[i_col] < dock_minimum_column_percentage )
                {
                    panel_width_percentage[i_col] = dock_minimum_column_percentage;
                }
                if( panel_width_percentage[i_col] > wide_panel_width )
                {
                    wide_panel_width = panel_width_percentage[i_col];
                    wide_panel_id = i_col;
                }

                if( panel_width_percentage[i_col] < skinny_panel_width )
                {
                    skinny_panel_width = panel_width_percentage[i_col];
                    skinny_panel_id = i_col;
                }

                calculated_percentages += panel_width_percentage[i_col];
            }
            else
            {
                panel_width_percentage[ i_col ] = 0.f;
                for(  j_row = 0; j_row < panel_y_count;  j_row++)
                {
                    panel_height_percentages[ i_col * panel_y_count + j_row  ] = 0.f;
                }
            }
        }

        dock_width_minus_column_padding -=  (current_column_count -1) * (float)dock_padding_horizontal;

        if( calculated_percentages < 100.f )
        {
            percentage_difference = 100.f - calculated_percentages;
            panel_width_percentage[skinny_panel_id] += percentage_difference;
            //return;
        }
        else if( calculated_percentages > 100.f )
        {
            percentage_difference =  calculated_percentages -100.f;
            panel_width_percentage[wide_panel_id] -= percentage_difference;
            //return;
        }

        //Manages the heights of the panels
        //Loops through each column
        for( i_col = 0; i_col < panel_x_count; i_col++ )
        {
            col_found = column_is_open( i_col );

            dock_height_minus_column_padding = widget_box.h;
            current_row_count = 0; // Used to determine how many rows are in this column
            for(  j_row = 0; j_row < panel_y_count;  j_row++)
            {
                current_panel_id = i_col * panel_y_count + j_row ;
                temp_panel = panels[ current_panel_id ];
                if( temp_panel != nullptr )
                {
                    if( clear_panels_on_setup )
                    {
                        temp_panel->clear_panel();
                        if( temp_panel->dock_settings_button!=nullptr )
                        {
                            temp_panel->dock_settings_button->set_clicked( false );
                        }
                    }

                    if( temp_panel->has_content() )
                    {
                        current_row_count++;
                    }
                }
            }

            dock_height_minus_column_padding -=  (current_row_count -1) * (float)dock_padding_vertical;

            //Now we loop and set the actual heights in pixels
            if( col_found )
            {
                first_row_round = -1;
                current_column_row_count = 0;
                calculated_percentages = 0;
                shortest_panel_id = -1;
                tallest_panel_id = -1;
                shortest_panel_height = 100;
                tallest_panel_height = 0;

                //We do an initial loop to add up all of the perctanges as well as
                for(  j_row = 0; j_row < panel_y_count;  j_row++)
                {
                    current_panel_id = i_col * panel_y_count + j_row;
                    temp_panel = panels[ current_panel_id ];
                    if( temp_panel != nullptr && temp_panel->has_content() )
                    {
                        if( first_row_round < 0 )
                        {
                            first_row_round = j_row;
                        }
                        calculated_percentages += panel_height_percentages[current_panel_id];

                        if( panel_height_percentages[current_panel_id] < shortest_panel_height  )
                        {
                          shortest_panel_height  = panel_height_percentages[current_panel_id];
                          shortest_panel_id = j_row;
                        }

                        if( panel_height_percentages[current_panel_id] > tallest_panel_height  )
                        {
                          tallest_panel_height  = panel_height_percentages[current_panel_id];
                          tallest_panel_id = j_row;
                        }
                    }
                    else
                    {
                        panel_height_percentages[current_panel_id] = 0;
                    }
                }

                //If we actually found a usable row, we check if we've gone out of bounds and readjust
                if( first_row_round >= 0 )
                {
                    if( calculated_percentages < 100.f )
                    {
                        if( shortest_panel_id >=0 && shortest_panel_id < panel_y_count )
                        {
                            current_panel_id = i_col * panel_y_count + shortest_panel_id;
                            percentage_difference = 100.f - calculated_percentages;
                            panel_height_percentages[current_panel_id] += percentage_difference;
                        }
                    }
                    else if( calculated_percentages > 100.f )
                    {
                        if( tallest_panel_id >=0 && tallest_panel_id < panel_y_count )
                        {
                            current_panel_id = i_col * panel_y_count + tallest_panel_id;
                            percentage_difference =  calculated_percentages -100.f;
                            panel_height_percentages[current_panel_id] -= percentage_difference;
                        }
                    }

                    //Now that we have re-calcuated our height percenages lets set the actual heights
                    for( j_row = 0; j_row < panel_y_count ; j_row++)
                    {
                        current_panel_id = j_row + i_col*panel_y_count;
                        temp_panel = panels[ current_panel_id ];
                        if( temp_panel!=nullptr && temp_panel->has_content() )
                        {
                            temp_panel->set_height( panel_height_percentages[current_panel_id]  / 100.f * dock_height_minus_column_padding );
                        }
                    }
                }
            }
        }

        //Horizontally Resizing panels;
        bool panelBeingHovered = false;

        //Let's set the position of the  panels
        int panel_x = 0;
        int panelY = 0;
        int cPanel = 0;

        //Places the panels on the docks
        for( i_col = 0; i_col < panel_x_count ; i_col++)
        {
            panel_column_width[i_col] =  floor( dock_width_minus_column_padding * panel_width_percentage[i_col]/100.f );
            for( j_row = 0; j_row < panel_y_count ; j_row++)
            {
                current_panel_id = j_row + i_col*panel_y_count;
                temp_panel = panels[ current_panel_id ];
                if( temp_panel!=nullptr && temp_panel->has_content() )
                {
                    temp_panel->set_coords(panel_x, widget_box.y + panelY );
                    temp_panel->set_width( panel_column_width[ i_col ] );

                    //Basically only add to PanelY if its not the bottom one
                    panelY += temp_panel->get_height() + dock_padding_vertical;
                }
            }
            panelY = 0;
            if( panel_column_width[i_col] > 0.f )
            {
                panel_x+= panel_column_width[ i_col ] + dock_padding_horizontal;
            }
        }
    }

    void widget_dock::toggle_default_pane( std::string name )
    {
        gpe::error_log->report("Trying to toggle ["+name+"] pane...");
        gpe::key_pair * tempPair = nullptr;
        for( int i = 0; i < (int)default_panels.size(); i++)
        {
            tempPair = default_panels[i];
            if( tempPair !=nullptr)
            {
                if( tempPair->key_string == name || tempPair->key_substring == name )
                {
                    gpe::error_log->report("Panel["+ stg_ex::float_to_string(tempPair->key_value)+"] found to toggle...");
                    toggle_panel( tempPair->key_string, tempPair->key_value, true );
                }
            }
        }
    }

    void widget_dock::toggle_panel(  std::string name, int panelId,  bool open  )
    {
        if( (int)name.size() == 0 )
        {
            return;
        }
        if( find_panel( name) != nullptr )
        {
            gpe::error_log->report("Panel already found, so removing...");
            remove_panel( name );
        }
        else
        {
            gpe::error_log->report("Adding toggled panel to dock...");
            add_to_panel( name, panelId, open );
        }
    }

    void widget_dock::unhide_dock()
    {
        is_hidden = false;

    }
}
