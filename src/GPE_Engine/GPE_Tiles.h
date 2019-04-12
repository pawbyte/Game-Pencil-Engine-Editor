/*
GPE_Tiles.h
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

#ifndef GPE_TILES_H
#define GPE_TILES_H

const int GPE_DEFAULT_TILEWIDTH = 32;
const int GPE_DEFAULT_TILEHEIGHT = 32;

#include "GPE_Shapes.h"
#include "GPE_Engine.h"

class GPE_GameMapTile
{
private:
    GPE_Rect * tileBox;
    int xCoord;
    int yCoord;
public:
    int tset;
    int tileIdNumb;
    int tSetPos ;

    int tileLayer;
    int tileType;
    bool isSolid;
    bool drewBefore;
    GPE_GameMapTile();
    ~GPE_GameMapTile();
    int get_type();
    void update_box ( int xNew,  int yNew,  int wNew, int hNew);
    void render_self( GPE_Rect * cam );
    void render_self_auto( GPE_Rect * cam );
};

class GPE_LayerTileMapObject
{
private:
    int gridWidth, gridHeight;
    int tileWidth, tileHeight;
    int tileAmountX;
    int tileAmountY;
    int prevTileAmountX;
    int prevTileAmountY;
    std::vector< GPE_GameMapTile *> layerTiles;
public:
    int xOffset, yOffset;
    GPE_LayerTileMapObject();
    ~GPE_LayerTileMapObject();
    void clear_tiles();
    void create_new_map ( int newTX, int newTY);
    int find_tile (  int xPos, int yPos );
    int find_tile_in_box(  int x1,  int y1,  int x2, int y2 );
    bool find_matching_tile (   int xPos,  int yPos,  int tileTypeToSearchFor );
    bool find_matching_tile_in_box(  int x1,  int y1,  int x2, int y2,  int tileTypeToSearchFor);
    GPE_GameMapTile * get_tile_at ( int xIn,  int yIn );
    int get_map_size();
};

#endif
