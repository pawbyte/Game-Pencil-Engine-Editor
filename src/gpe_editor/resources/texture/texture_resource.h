/*
texture_resource.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/
#ifndef GPE_TEXTURE_RESOURCES_H
#define GPE_TEXTURE_RESOURCES_H

#include "../../resources/basic/gpe_basic_resource_page.h"


enum
{
    gpe_tex_preview_full = 0,
    gpe_tex_preview_circle = 1,
    gpe_tex_preview_quad = 2,
    gpe_tex_preview_polygon = 3,
};

class textureResource: public standardEditableGameResource
{
    protected:
        gpe::shape_point2d quad_points[4];
        int quad_place_position;
        int polygon_point_count;
    public:
        bool isPreloaded;
        gpe::texture_base * textureInEditor;

        pawgui::widget_checkbox * preview_mode_follows_mouse;
        pawgui::widget_dropdown_menu * preview_mode_checkbox;

        pawgui::widget_checkbox * preloadCheckBox;
        pawgui::widget_checkbox * textureUsesPixels;
        pawgui::widget_checkbox * imageUsesColorKey;
        pawgui::gpe_widget_color_picker * imageColorKey;
        pawgui::widget_label_text  * labelImageDimensions;
        pawgui::widget_label_text  * labelTextureMessage;
        pawgui::widget_button_icon * openExternalEditor_button;
        pawgui::widget_button_icon * refreshResourceData_button;
        textureResource(pawgui::widget_resource_container * pFolder = nullptr);
        ~textureResource();
        pawgui::widget_label_text  * labelInfoMaxTextureSize;
        bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        void compile_cpp();
        gpe::texture_base * get_resource_texture();
        bool include_local_files( std::string pBuildDir , int buildType );
        bool is_build_ready();
        int load_image(std::string new_file_name);
        void load_resource(std::string file_path = "");
        void prerender_self( );
        void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
        void save_resource(std::string file_path = "", int backupId = -1);
        void update_box(int x_new=-1, int y_new=-1, int newW=-1, int newH=-1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
