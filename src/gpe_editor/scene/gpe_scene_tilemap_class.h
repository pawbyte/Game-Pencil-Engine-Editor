/*
gpe_scene_tilemap_class.cpp
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

#ifndef GPE_SCENE_TILEMAP_CLASS_H
#define GPE_SCENE_TILEMAP_CLASS_H

#include "../../gpe/gpe_tiles.h"

#include "./gpe_scene_basic_class.h"
#include "gpe_scene_helper_class.h"
#include "../resources/tilesheet_resource/tilesheet_resource.h"
#include "../editor/gpe_editor_globals.h"

extern pawgui::widget_dock_panel * PANEL_TS_RESOURCE;

class GPE_SceneTile: public pawgui::widget_branch
{
public:
    int tileTypeId;
    int tileIndexId;
    int tilesheetIndexId;
    gpe::shape_rect tileRect;
    GPE_SceneTile();
    ~GPE_SceneTile();
    void add_typed_elements();
    void process_elements();
    void render_branch();
    bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class GPE_SceneTileMap: public GPE_SceneBasicClass
{
private:
    int tileAmountX, tileAmountY;
    int tileIdPlace;
    std::vector< int > tileIdsToPlace;
    float defaultTileWidth,defaultTileHeight;
    float defaultTileAmountX,defaultTileAmountY;
    float tileToPlaceX1,tileToPlaceY1, tileToPlaceX2,tileToPlaceY2;
    int tilesToPlacePerRow;
    gpe::shape_rect tsPlacementArea;
    tilesheetResource * tsRes;
public:
    std::string name;
    int tileWidth,tileHeight;
    int prevTileAmountX, prevTileAmountY;
    std::vector <GPE_SceneTile*> mapTiles;
    pawgui::widget_input_number * fieldTileWidth;
    pawgui::widget_input_number * fieldTileHeight;
    pawgui::widget_input_number * fieldAmountX;
    pawgui::widget_input_number * fieldAmountY;
    pawgui::widget_checkbox * fillScene;
    pawgui::widget_branch * mapBranch;
    pawgui::widget_drop_down_resource_menu * tilesheetDropDown;
    tilesheetPreviewer * tSPreviewer;

    GPE_SceneTileMap(std::string mapName, int x = 0, int y = 0, pawgui::widget_resource_container *pFolder = nullptr );
    ~GPE_SceneTileMap();
    void add_typed_elements();
    void calculate_size();
    void create_new_map(int new_xt=8, int new_yt=8, int ntileType=0);
    GPE_SceneTile* get_tile_at(int x, int y);
    void resize_tilemap(int new_xt=8, int new_yt=8, int ntileType=0);
    int get_map_size();
    void set_map_size( int newW, int newH );
    int get_xmax();
    int get_ymax();
    int get_sizex();
    int get_sizey();
    int get_tile_x( int xInPixels);
    int get_tile_y( int yInPixels);
    void process_elements();
    void render_branch();
    bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
