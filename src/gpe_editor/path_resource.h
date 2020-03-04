/*
path_resource.h
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

#ifndef GPE_PATH_RESOURCE_H
#define GPE_PATH_RESOURCE_H

#include "gpe_basic_resource_page.h"
#include "game_scene_resource.h"
#include "../gpe/gpe_path2d.h"

class gamePathResource: public standardEditableGameResource
{
public:
    int pointPos;
    int selectedPointPos;
    gpe::game_path_point2d * selectedPathPoint;
    gpe_text_widget_number * currentPointX;
    gpe_text_widget_number * currentPointY;
    gpe_text_widget_number * currentPointSpeed;
    gpe::shape_rect sceneEditorView;
    gpe::shape_rect scenePreviewRect;
    gpe::shape_rect sceneRect;
    gpe::shape_rect editorCommentPane;
    GPE_DropDown_Resouce_Menu * sceneToPreview;
    GPE_DropDown_Menu * sceneZoomLevel;
    float zoomValue;
    std::vector < gpe::game_path_point2d * >  pathPoints;
    GPE_SelectBoxBasic * pathOptions;
    GPE_ToolIconButton * pointSettingsButtton;
    GPE_ToolIconButton * pointRemoveButton;
    GPE_ToolIconButton * pointMoveUpButtton;
    GPE_ToolIconButton * pointMoveDownButton;

    gpe::color * pathLineColor;
    gpe::color * pathPointColor;
    GPE_CheckBoxBasic * pathTypeIsClosed;
    GPE_RadioButtonControllerBasic * pathShapeType;
    bool scnPostProcessed;
    GPE_ScrollBar_XAxis * sceneXScroll;
    GPE_ScrollBar_YAxis * sceneYScroll;
    bool areaIsScrollable;
    float sceneMouseXPos, sceneMouseYPos;
    GPE_GuiElementList * bottomPaneList;
    gamePathResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~gamePathResource();
    gpe::game_path_point2d * add_point( int pointX, int pointY, float pointSpeed = 1);
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void clear_points();
    void compile_cpp();
    bool export_and_play_native( bool launchProgram = true);
    bool get_mouse_coords( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void handle_scrolling();
    bool is_build_ready();
    void integrate_into_syntax();
    bool include_local_files( std::string pBuildDir , int buildType );
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    bool remove_point( int pointId );
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void save_resource(std::string file_path = "", int backupId = -1);
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    void update_project_layers();
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif

