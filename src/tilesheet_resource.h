/*
tilesheet_resource.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://create.pawbyte.com>


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
    GPE_Rect tsEditorViewRect;
    GPE_Rect tsCameraRect;
    GPE_Rect tsImageCameraRect;
    GPE_Rect tsRect;
    GPE_Rect tsSelectedArea;
    GPE_ScrollBar_XAxis * previewXScroll;
    GPE_ScrollBar_YAxis * previewYScroll;
    bool areaIsScrollable;
public:
    double zoomValue;
    double minZoomValue;
    double maxZoomValue;
    double areaMouseXPos, areaMouseYPos;

    GPE_Label_Text * labelImageDimensions;
    GPE_Label_Text * labelFrameInfo;
    GPE_Label_Text * labelSpriteMessage;
    int tileToPrevX1,tileToPrevY1, tileToPrevX2,tileToPrevY2;
    int tilesToPlacePerRow;
    GPE_Tilesheet * tileSheetToPreview;
    std::vector< int > tilesIdsInPreview;
    bool allowDuplicates;
    bool showGrid;
    tilesheetPreviewer();
    ~tilesheetPreviewer();
    GPE_Label_Text * labelInfoMaxTextureSize;
    bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void handle_scrolling();
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    void render_selection( int xPos = 0, int yPos = 0, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true, double scaleSize = 1, GPE_Color * fColor = NULL);
    void reset_preview(bool moveCamera);
};

class tilesheetResource: public standardEditableGameResource
{
public:
    bool isPreloaded;
    tilesheetPreviewer * tsPreviewer;
    GPE_Tilesheet * tilesheetInEditor;
    GPE_CheckBoxBasic * preloadCheckBox;
    GPE_Label_Text * tilesheetDimensionsStr;
    GPE_ToolIconButton * transformResourceButton;
    GPE_ToolIconButton * openExternalEditorButton;
    GPE_ToolIconButton * refreshResourceDataButton;
    GPE_TextInputNumber * tsDataFields[TILESHEET_DATA_FIELD_COUNT];
    GPE_Label_Text * labelInfoMaxTextureSize;
    tilesheetResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~tilesheetResource();
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    void load_image( std::string newFileName);
    void preprocess_self(std::string alternatePath = "");
    void prerender_self( );
    void process_data_fields();
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void save_resource(std::string alternatePath = "", int backupId = -1);
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
