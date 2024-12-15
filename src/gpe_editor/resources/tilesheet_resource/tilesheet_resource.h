/*
tilesheet_resource.h
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
#ifndef GPE_TILESHEET_RESOURCES_H
#define GPE_TILESHEET_RESOURCES_H

#include "../../resources/basic/gpe_basic_resource_page.h"


const int TILESHEET_DATA_FIELD_COUNT = 6;
extern std::string tsDataLabels[TILESHEET_DATA_FIELD_COUNT];
extern int tsDataLabelWidth;

class tilesheetPreviewer: public pawgui::widget_basic
{
protected:
    gpe::shape_rect tsEditorViewRect;
    gpe::shape_rect tsCameraRect;
    gpe::shape_rect tsImageCameraRect;
    gpe::shape_rect tsRect;
    gpe::shape_rect tsselectedArea;
    pawgui::widget_scrollbar_xaxis * previewXScroll;
    pawgui::widget_scrollbar_yaxis * previewYScroll;
    bool areaIsScrollable;
    float local_mouse_x, local_mouse_y;
public:
    float zoomValue;
    float minZoomValue;
    float maxZoomValue;

    pawgui::widget_label_text  * labelImageDimensions;
    pawgui::widget_label_text  * labelFrameInfo;
    pawgui::widget_label_text  * labelanimationMessage;
    int tileToPrevX1,tileToPrevY1, tileToPrevX2,tileToPrevY2;
    int tilesToPlacePerRow;
    gpe::tilesheet * tileSheetToPreview;
    std::vector< int > tilesIdsInPreview;
    bool allowDuplicates;
    bool showGrid;
    tilesheetPreviewer();
    ~tilesheetPreviewer();
    pawgui::widget_label_text  * labelInfoMaxTextureSize;
    bool get_mouse_coords( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void handle_scrolling();
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void render_selection( int x_pos = 0, int y_pos = 0, gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr, float scale_size = 1, gpe::color * fColor = nullptr);
    void reset_preview(bool moveCamera);
};

class tilesheetResource: public standardEditableGameResource
{
public:
    bool isPreloaded;
    tilesheetPreviewer * tsPreviewer;
    gpe::tilesheet * tilesheetInEditor;
    pawgui::widget_checkbox * imageUsesColorKey;
    pawgui::gpe_widget_color_picker * imageColorKey;
    pawgui::widget_checkbox * preloadCheckBox;
    pawgui::widget_label_text  * tilesheetDimensionsStr;
    pawgui::widget_button_icon * openExternalEditor_button;
    pawgui::widget_button_icon * refreshResourceData_button;
    pawgui::widget_input_number * tsDataFields[TILESHEET_DATA_FIELD_COUNT];
    pawgui::widget_label_text  * labelInfoMaxTextureSize;
    tilesheetResource(pawgui::widget_resource_container * pFolder = nullptr);
    ~tilesheetResource();
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    gpe::texture_base * get_resource_texture();
    bool include_local_files( std::string pBuildDir , int buildType );
    bool load_image( std::string new_file_name);
    bool is_build_ready();
    void load_resource(std::string file_path = "");
    void prerender_self( );
    void process_data_fields();
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void save_resource(std::string file_path = "", int backupId = -1);
    void update_box(int x_new=-1, int y_new=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
