/*
font_resource.h
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
#ifndef GPE_FONT_RESOURCES_H
#define GPE_FONT_RESOURCES_H

#include "../../resources/basic/gpe_basic_resource_page.h"


const int FONT_FILE_TYPES = 6;
const int FONT_EOT = 0;
const int FONT_OTF = 1;
const int FONT_SVG = 2;
const int FONT_TTF = 3;
const int FONT_WOFF = 4;
const int FONT_WOFF2 = 5;

extern std::string SUPPORTED_FONT_EXT[FONT_FILE_TYPES];

class fontResource: public standardEditableGameResource
{
private:
    pawgui::widget_radio_button_controller * fontType_buttonController;
    pawgui::widget_button_icon * openExternalEditor_button;
    std::string fontInEditorFileName;
    std::string font_family_name;
    gpe::font_base * fontInEditor;
public:
    int font_size;
    int fontId;
    int fontType;
    int fontWidth;
    int fontHeight;
    pawgui::widget_input_number * font_sizeField;
    pawgui::widget_input_text * fontPreviewTextField;
    std::string storedFontFileNames[FONT_FILE_TYPES];
    fontResource(pawgui::widget_resource_container * pFolder = nullptr);
    ~fontResource();
    bool build_css3_file(std::ofstream * fileTarget, int leftTabAmount);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool include_local_files( std::string pBuildDir , int buildType );
    bool is_build_ready();
    void load_font(std::string new_file_name, int newfont_size = 12);
    void load_resource(std::string file_path = "");
    void prerender_self( );
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    bool render_held_font( int x_pos, int y_pos, std::string text_to_render, gpe::color * text_color = nullptr,int alignment_h = gpe::fa_left,int alignment_v = gpe::fa_top, float render_angle = 0.f, float render_scale = 1, int render_alpha = 255 );
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
