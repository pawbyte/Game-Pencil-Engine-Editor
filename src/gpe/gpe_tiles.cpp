/*
gpe_tiles.cpp
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

#include "gpe_tiles.h"

namespace gpe
{
    layer_game_tile::layer_game_tile()
    {
        tset = is_null;
        tileIdNumb = is_null;
        tSetPos = -1;
        xCoord = 0;
        yCoord = 0;
        tileLayer = -1;
        tileType =0;
        isSolid = false;
        drewBefore = false;
        tileBox = new gpe::shape_rect();
        tileBox->update_shape(0,0,32,32);
    }

    layer_game_tile::~layer_game_tile()
    {
        if( tileBox!=NULL)
        {
            delete tileBox;
            tileBox = NULL;
        }
    }

    int layer_game_tile::get_type ()
    {
        return tileType;
    }

    void layer_game_tile::update_box ( int xNew,  int yNew,  int wNew, int hNew)
    {
        tileBox->update_shape(xNew,yNew,wNew,hNew);
    }

    void layer_game_tile::render_self ( gpe::shape_rect * cam )
    {
    /*
        //ensures that the Tilesheet for the tile isn't NULL
        if(tset >=0 && tset < (int)rsm->rTilesheets.size() )
        {
            tilesheet * gameTilesheet = rsm->rTilesheets[tset];
            //console.log( tset)
            if( gameTilesheet!=NULL )
            {
                //checks to see if tile is in the camera region
                //if(gpe.check_collision_with_rect(xCoord,yCoord-tileBox.get_height()-cam.get_y(),tileBox.get_width(),tileBox.get_height(),cam) == true )
                //{
                if( gameTilesheet->tsImage!=NULL )
                {
                    if( cam!=NULL)
                    {
                        gameTilesheet->tsImage->render_tex( xCoord - cam->x,yCoord - cam->y,tileBox);
                    }
                    else
                    {
                        gameTilesheet->tsImage->render_tex( xCoord,yCoord,tileBox);
                    }
                }
                //}
            }
        }
    */
    }

    void layer_game_tile::render_self_auto ( gpe::shape_rect * cam)
    {
        render_self( cam );
    }


    layer_tile_map::layer_tile_map()
    {
        tileAmountX = is_null;
        tileAmountY = is_null;
    }

    layer_tile_map::~layer_tile_map()
    {
        clear_tiles();
    }



    void layer_tile_map::clear_tiles()
    {
        layer_game_tile *  tempTile = NULL;
        for( int i = (int)layerTiles.size()-1; i >=0; i-- )
        {
            tempTile = layerTiles[i];
            if( tempTile!=NULL)
            {
                delete tempTile;
                tempTile = NULL;
            }
        }
        layerTiles.clear();
    }

    void layer_tile_map::create_new_map ( int newTX, int newTY)
    {
        tileAmountX=newTX;
        tileAmountY=newTY;
        int newSize = tileAmountX*tileAmountY;
        int i=0;
        clear_tiles();
        for(i=0; i<newSize; i++)
        {
            layerTiles[i] = new layer_game_tile();
            layerTiles[i]->tileType = 0;
        }
    }

    int layer_tile_map::find_tile ( int xPos, int yPos )
    {
        if( tileWidth==0 || tileHeight == 0)
        {
            return is_null;
        }
        // find rectangle to check (limited to field size):
        xPos = std::max(0, std::min(gridWidth - 1, (xPos / tileWidth)|0 ) );
        yPos = std::max(0, std::min(gridHeight - 1,(yPos / tileHeight)|0 ) );

        layer_game_tile * foundTile = NULL;
        foundTile = get_tile_at(xPos, yPos);
        if(foundTile!= NULL )
        {
            return foundTile->tset;
        }
        return is_null;
    }

    int layer_tile_map::find_tile_in_box(  int x1,  int y1,  int x2, int y2 )
    {
        if( tileWidth==0 || tileHeight == 0)
        {
            return is_null;
        }
        int i=0;
        int j=0;
        int l=0;
        int t=0;
        int r=0;
        int b=0;
        layer_game_tile * foundTile = NULL;
        // find rectangle to check (limited to field size):
        l = std::max(0, std::min(gridWidth - 1, (x1 / tileWidth)|0 ) );
        t = std::max(0, std::min(gridHeight - 1,(y1 / tileHeight)|0 ) );
        r = std::max(0, std::min(gridWidth - 1, (x2 / tileWidth)|0 ) );
        b = std::max(0, std::min(gridHeight - 1, (y2 / tileHeight)|0  ) );

        for (j = t; j <= b; j += 1)
        {
            for (i = l; i <= r; i += 1)
            {
                foundTile = get_tile_at(i, j);
                if(foundTile!= NULL )
                {
                    return foundTile->tset;
                }
            }
        }
        return is_null;
    }

    bool layer_tile_map::find_matching_tile( int xPos,  int yPos,  int tileTypeToSearchFor )
    {
        //if we have an actual layer to look for
        if( tileWidth==0 || tileHeight == 0)
        {
            return false;
        }
        layer_game_tile * foundTile = NULL;
        // find rectangle to check (limited to field size):
        xPos = std::max(0, std::min(gridWidth - 1, (xPos / tileWidth)|0 ) );
        yPos = std::max(0, std::min(gridHeight - 1,(yPos / tileHeight)|0 ) );
        foundTile = get_tile_at(xPos, yPos);
        if( foundTile!= NULL && foundTile->tset == tileTypeToSearchFor )
        {
            return true;
        }
        return false;
    }

    bool layer_tile_map::find_matching_tile_in_box( int x1,  int y1,  int x2, int y2,  int tileTypeToSearchFor)
    {
        if( tileWidth==0 || tileHeight == 0)
        {
            return false;
        }
        int l = std::max(0, std::min(gridWidth - 1, (x1 / tileWidth)|0 ) );
        int t = std::max(0, std::min(gridHeight - 1,(y1 / tileHeight)|0 ) );
        int r = std::max(0, std::min(gridWidth - 1, (x2 / tileWidth)|0 ) );
        int b = std::max(0, std::min(gridHeight - 1, (y2 / tileHeight)|0  ) );
        layer_game_tile * foundTile = NULL;
        int i=0;
        int j=0;
        // find rectangle to check (limited to field size):
        for (j = t; j <= b; j += 1)
        {
            for (i = l; i <= r; i += 1)
            {
                foundTile = get_tile_at(i, j);
                if(foundTile!= NULL && foundTile->tset == tileTypeToSearchFor )
                {
                    return true;
                }
            }
        }
        return false;
    }

    int layer_tile_map::get_map_size()
    {
        return (int)layerTiles.size();
    }

    layer_game_tile * layer_tile_map::get_tile_at ( int xIn,  int yIn )
    {
        int seekedTile = xIn+ yIn*tileAmountX;
        if(  seekedTile>=0 && seekedTile < (int)layerTiles.size() )
            return layerTiles[seekedTile];
        return NULL;
    }
}
