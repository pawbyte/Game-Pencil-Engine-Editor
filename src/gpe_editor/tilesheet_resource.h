/*
tilesheet_resource.h
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
#ifndef GPE_TILESHEET_RESOURCES_H
#define GPE_TILESHEET_RESOURCES_H

#include "gpe_basic_resource_page.h"

const int TILESHEET_DATA_FIELD_COUNT = 6;
extern std::string tsDataLabels[TILESHEET_DATA_FIELD_COUNT];
extern int tsDataLabelWidth;

class tilesheetPreviewer: public GPE_GeneralGuiElement
{
protected:
    gpe::shape_rect tsEditorViewRect;
    gpe::shape_rect tsCameraRect;
    gpe::shape_rect tsImageCameraRect;
    gpe::shape_rect tsRect;
    gpe::shape_rect tsselectedArea;
    GPE_ScrollBar_XAxis * previewXScroll;
    GPE_ScrollBar_YAxis * previewYScroll;
    bool areaIsScrollable;
public:
    float zoomValue;
    float minZoomValue;
    float maxZoomValue;
    float areaMouseXPos, areaMouseYPos;

    GPE_Label_Text * labelImageDimensions;
    GPE_Label_Text * labelFrameInfo;
    GPE_Label_Text * labelanimationMessage;
    int tileToPrevX1,tileToPrevY1, tileToPrevX2,tileToPrevY2;
    int tilesToPlacePerRow;
    gpe::tilesheet * tileSheetToPreview;
    std::vector< int > tilesIdsInPreview;
    bool allowDuplicates;
    bool showGrid;
    tilesheetPreviewer();
    ~tilesheetPreviewer();
    GPE_Label_Text * labelInfoMaxTextureSize;
    bool get_mouse_coords( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void handle_scrolling();
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_selection( int xPos = 0, int yPos = 0, gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL, float scaleSize = 1, gpe::color * fColor = NULL);
    void reset_preview(bool moveCamera);
};

class tilesheetResource: public standardEditableGameResource
{
public:
    bool isPreloaded;
    tilesheetPreviewer * tsPreviewer;
    gpe::tilesheet * tilesheetInEditor;
    GPE_CheckBoxBasic * preloadCheckBox;
    GPE_Label_Text * tilesheetDimensionsStr;
    GPE_ToolIconButton * transformResourceButton;
    GPE_ToolIconButton * openExternalEditorButton;
    GPE_ToolIconButton * refreshResourceDataButton;
    gpe_text_widget_number * tsDataFields[TILESHEET_DATA_FIELD_COUNT];
    GPE_Label_Text * labelInfoMaxTextureSize;
    tilesheetResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~tilesheetResource();
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    gpe::texture_base * get_resource_texture();
    bool include_local_files( std::string pBuildDir , int buildType );
    void load_image( std::string new_file_name);
    bool is_build_ready();
    void load_resource(std::string file_path = "");
    void prerender_self( );
    void process_data_fields();
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void save_resource(std::string file_path = "", int backupId = -1);
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
