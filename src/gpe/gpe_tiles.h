/*
gpe_tiles.h
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

#ifndef gpe_tiles_h
#define gpe_tiles_h

#include "gpe_branch.h"
#include "gpe_shapes.h"
#include "gpe_runtime.h"

namespace gpe
{
    const int tile_default_width = 32;
    const int tile_default_height = 32;

    class layer_game_tile
    {
        private:
            gpe::shape_rect * tileBox;
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
            layer_game_tile();
            ~layer_game_tile();
            int get_type();
            void update_box ( int xNew,  int yNew,  int wNew, int hNew);
            void render_self( gpe::shape_rect * cam );
            void render_self_auto( gpe::shape_rect * cam );
    };

    class layer_tile_map
    {
        private:
            int gridWidth, gridHeight;
            int tileWidth, tileHeight;
            int tileAmountX;
            int tileAmountY;
            int prevTileAmountX;
            int prevTileAmountY;
            std::vector< layer_game_tile *> layerTiles;
        public:
            int xOffset, yOffset;
            layer_tile_map();
            ~layer_tile_map();
            void clear_tiles();
            void create_new_map ( int new_xt, int new_yt);
            int find_tile (  int x_pos, int y_pos );
            int find_tile_in_box(  int x1,  int y1,  int x2, int y2 );
            bool find_matching_tile (   int x_pos,  int y_pos,  int tileTypeToSearchFor );
            bool find_matching_tile_in_box(  int x1,  int y1,  int x2, int y2,  int tileTypeToSearchFor);
            layer_game_tile * get_tile_at ( int xIn,  int yIn );
            int get_map_size();
    };
}
#endif //gpe_tiles
