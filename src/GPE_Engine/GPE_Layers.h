/*
GPE_Layers.h
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

#ifndef GPE_LAYERS_H
#define GPE_LAYERS_H

#include "GPE_Spatial_Partitions.h"
#include "GPE_Backgrounds.h"
#include "GPE_Basic_Object.h"
#include "GPE_Lights.h"
#include "GPE_Tiles.h"

class GPE_Layer
{
private:
    int collisionPairsTotal;
    int parsed_objects;
    //Spatial Grid stuff
    bool spatialGridIsCreated;
    int spatialMapWidthSize;
    int spatialMapHeightSize;
    int spatialGridBoxSize;
    int spatialGridWidthSize;
    int spatialGridHeightSize;
    int spatialGridWidthAmount;
    int spatialGridHeightAmount;
    int objectCollisionHashesChecked;
    int spatialCollisionChecks;
    std::vector<GPE_LayerTileMapObject * > layerMaps;
    std::vector<GPE_GameObject * > layerObjects;
    std::vector<GPE_GameObject * > independentObjects;
    std::vector<GPE_Background * >layerBackgrounds;
    std::vector< GPE_BasicLight * > layerAmbientLights;
    std::vector< GPE_Directionight * > layerDirectionLights;
    std::vector< GPE_PointLight * > layerPointLights;
    /*
    scnStartBackgrounds = [];
    scnStartObjects = [];
    scnStartTiles  = [];
    */

    int layerId;
    int layerAlpha;
    std::string layerName;
    //Collision related variables
    std::vector<GPE_SuperSpatialPartition * > collisionSpacesInView;
    std::vector<GPE_SuperSpatialPartition * > COLLISION_AREA_SPACES; //Used for spatial partitioning collision checker
    GPE_SuperSpatialPartition * COLLISION_AREA_OUTSIDE_SCENE;
    bool superDebugCollisions;


    std::map<std::string,bool> objectCollisionIsChecked;
    std::vector<GPE_GameObject * > collisionPairA;
    std::vector<GPE_GameObject * > collisionPairB;

    std::vector<GPE_GameObject * > prerenderObjectRenderList;
    std::vector<GPE_GameObject * > regularObjectRenderList;
    std::vector<GPE_GameObject * > renderAboveObjectRenderList;
    std::vector<GPE_GameObject * > renderHudObjectRenderList;
public:
    GPE_Layer( int newLayerId, double newLayerAlpha = 1);
    ~GPE_Layer();
    GPE_LayerTileMapObject *  add_tilemap( std::string name = "", int x = 0, int y = 0, int w = -1, int h = -1);
    void add_collision_pair( GPE_GameObject *objPairA, GPE_GameObject *objPairB);
    void begin_spatialpartioning( int cSceneWidth, int cSceneHeight );
    void begin_render ( std::vector< int >&spacesInView );
    void clear_dynamic_data();
    void clear_spaces();

    int find_spatial_at( int xIn, int yIn);
    int get_id();
    void remove_object ( GPE_GameObject *objToRemove);
    void render_hud_objects();
    void render_objects();
    void replace_all_backgrounds ( int needleBgId, int newBgId );
    void scroll_backgrounds();
    void set_default_grid_size();
    void set_layer_background ( int layerBeingChanged, int bgId, int newBgId );
};

struct GPE_SceneLayer_Meta
{
    int lId;
    double lAlpha;
};

#endif // GPE_LAYERS_H
