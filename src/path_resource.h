/*
path_resource.h
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

#ifndef GPE_PATH_RESOURCE_H
#define GPE_PATH_RESOURCE_H

#include "gpe_basic_resource_page.h"
#include "game_scene_resource.h"
#include "GPE_Engine/GPE_Paths.h"

class gamePathResource: public standardEditableGameResource
{
public:
    int pointPos;
    int selectedPointPos;
    GPE_PathPoint * selectedPathPoint;
    GPE_TextInputNumber * currentPointX;
    GPE_TextInputNumber * currentPointY;
    GPE_TextInputNumber * currentPointSpeed;
    GPE_Rect sceneEditorView;
    GPE_Rect scenePreviewRect;
    GPE_Rect sceneRect;
    GPE_Rect editorCommentPane;
    GPE_DropDown_Resouce_Menu * sceneToPreview;
    GPE_DropDown_Menu * sceneZoomLevel;
    double zoomValue;
    std::vector <GPE_PathPoint * >  pathPoints;
    GPE_SelectBoxBasic * pathOptions;
    GPE_ToolIconButton * pointSettingsButtton;
    GPE_ToolIconButton * pointRemoveButton;
    GPE_ToolIconButton * pointMoveUpButtton;
    GPE_ToolIconButton * pointMoveDownButton;

    GPE_Color * pathLineColor;
    GPE_Color * pathPointColor;
    GPE_CheckBoxBasic * pathTypeIsClosed;
    GPE_RadioButtonControllerBasic * pathShapeType;
    bool scnPostProcessed;
    GPE_ScrollBar_XAxis * sceneXScroll;
    GPE_ScrollBar_YAxis * sceneYScroll;
    bool areaIsScrollable;
    double sceneMouseXPos, sceneMouseYPos;
    GPE_GuiElementList * bottomPaneList;
    gamePathResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~gamePathResource();
    GPE_PathPoint * add_point( int pointX, int pointY, double pointSpeed = 1);
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void clear_points();
    void compile_cpp();
    bool export_and_play_native( bool launchProgram = true);
    bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void handle_scrolling();
    void integrate_into_syntax();
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    void prerender_self( );
    void preprocess_self(std::string alternatePath = "");
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    bool remove_point( int pointId );
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void save_resource(std::string alternatePath = "", int backupId = -1);
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    void update_project_layers();
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif

