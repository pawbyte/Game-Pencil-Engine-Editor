/*
pawgui_dock_system.h
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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


#ifndef PAWGUI_DOCK_SYSTEM_H
#define PAWGUI_DOCK_SYSTEM_H

#include "pawgui_resource_dropdown.h"
#include "pawgui_general_resource.h"
#include "pawgui_buttons.h"
#include "pawgui_tab_basic.h"
#include "pawgui_panel_list.h"

namespace pawgui
{
    class widget_dock_panel: public pawgui::widget_basic
    {
    private:
        pawgui::widget_tabbar * panelBar;
        gpe::shape_rect * panelRect;
    public:
        pawgui::widget_basic * specialPanelElement;
        pawgui::widget_button_icon * dock_settings_button;
        bool just_resized;
        bool being_resized;
        bool menuResized;
        bool wasProcessed;
        pawgui::widget_panel_list * panelGuiList;
        gpe::shape_rect * containerRect;
        widget_dock_panel();
        ~widget_dock_panel();
        bool add_container( std::string name, bool openNew = false );
        void add_gui_element(pawgui::widget_basic * element, bool isNLElement = true );
        void add_gui_auto(pawgui::widget_basic * element);
        void add_gui_element_fullsize(pawgui::widget_basic * element);
        void add_gui_section( widget_panel_section *  element );

        void clear_panel();
        bool container_in_view( std::string name );
        bool contains_tab( std::string name );
        int get_panel_height();
        std::string get_selected_container();
        int get_tab_count();
        std::string get_tab_name( int pos );
        bool has_content();
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr );
        bool save_panel_data_to_file(std::ofstream * fileTarget);
        void setup_panel( bool addSpacing = true, bool clearList = true );
        bool remove_container( std::string name );
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr );
        void reset_panel();
    };

    class widget_dock: public pawgui::widget_basic
    {
    private:
        bool is_hidden;
        int panel_x_count;
        int panel_y_count;
        int panel_max_count;

        std::vector< gpe::key_pair * > default_panels;
        std::vector< int > panel_column_width;
        std::vector< float > panel_width_percentage;
        std::vector< float > panel_height_percentages;
        int panel_primary_id_x;
        int panel_primary_id_y;
        int panel_primary_id;
        bool dock_updated_this_frame;
        int dock_padding_vertical;
        int dock_padding_horizontal;
        float dock_minimum_column_percentage;
        float dock_minimum_row_percentage;
        int current_column_count, current_row_count;
        float dock_width_minus_column_padding, dock_height_minus_column_padding;
        bool just_resized;
        bool being_resized;
        int hori_resize_start_x;
        int resize_pane_x1, resize_pane_x2;

        bool horizontal_resize;
        bool vertical_resize;


        //Reside related Data [ panel_height_percentagesSTART ]
        int column_being_resized_id;
        int row_being_resized_id;

        std::vector< widget_dock_panel * > panels;
        std::vector< std::string > panels_default_names;

        public:
        widget_dock();
        ~widget_dock();
        void add_to_panel(  std::string name, int panelId = 0,  bool open = false);
        void add_default_panel(  std::string name, int panelId = 0,  bool open = false );
        void clear_all_panels();
        void clear_defaults();
        bool column_is_open( int col_numb );
        bool container_in_view( std::string name );
        widget_dock_panel * contains_panel( std::string name );
        widget_dock_panel * find_panel( std::string name );

        int get_column(int panelId);

        int get_dock_left_width();
        int get_dock_right_width();
        bool get_hidden();

        widget_dock_panel * get_panel( int panel_id );
        int get_panel_count();
        int get_panel_x_count();
        int get_panel_y_count();

        int get_primary_panel();

        bool handle_resizing();
        void hide_dock();
        bool load_dock_setings( std::string filename );
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr );
        void remove_panel( std::string name );
        void remove_default_panel( std::string name );
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr );
        void reset_dock();
        bool resize_dock( int new_col, int new_row );

        bool save_dock_setings( std::string filename );
        bool set_primary_panel( int primaryXId, int primaryYId );

        void setup_dock( bool clear_panels_on_setup = true );
        void toggle_default_pane( std::string name );
        void toggle_panel(  std::string name, int panelId = 0,  bool open = true );
        void unhide_dock();
    };
}

#endif //PAWGUI_DOCK_SYSTEM_H
