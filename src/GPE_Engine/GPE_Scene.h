/*
GPE_Scene.h
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

#ifndef GPE_SCENE_H
#define GPE_SCENE_H

#include "GPE_Basic_Object.h"
#include "GPE_Layers.h"

class GPE_GameScene
{
private:
    int sceneId;
    bool sceneHasBeenActivated;
    int sceneWidth;
    int sceneHeight;
    bool sceneIsContinuous;
    bool sceneWasVisitedBefore;
    std::string sceneName;
    std::string defaultLayerName;

    //tile data info
    int tileAmountX;
    int tileAmountY;
    int tileWidth;
    int tileHeight;
    std::vector< GPE_GameObject * >sceneObjects;
    std::vector< GPE_GameObject * >persistentObjects;
    std::vector< GPE_Layer * > sceneLayers;
    std::vector< GPE_SceneLayer_Meta * >scenesStartLayers;

    //background data
    GPE_Color * bgColor;
    int sceneVersion;
    int bgMusic;
    int startAudio;
    int endAudio;

    bool sceneIsProcessed;
    bool sceneResourcesAreProcessed;
    bool sceneIsSpatiallyPartitioned;
    GPE_SceneCamera * sceneCamera[GPE_MAX_CAMERAS];
public:
    GPE_GameScene(int sId, std::string sName);
    ~GPE_GameScene();
    GPE_Layer * add_layer( int newLayerId, double newLayerAlpha );
    GPE_Layer * add_start_layer( int newLayerId, double newLayerAlpha );
    void disable_camera ( int cameraId);
    void enable_camera ( int cameraId);
    void reset_scene_meta();
    void clear_layers_data();
    bool add_to_persistent_objects( GPE_GameObject * nObject);
    GPE_Layer * find_layer ( int layerIdIn);
    GPE_Layer * get_layer ( int layerIdI );

    int get_scene_width();
    int get_scene_height();

    void scene_init();
    void update_scene_specs( int lvlW, int lvlH);
    void update_tilemap();
    void add_background( int bSurf, bool loopH, bool loopV, double bgX, double bgY, double bgXSpd, double bgYSpd);
    void replace_background( int layerBeingChanged, int needleBgId, int newBgId );
    void replace_all_backgrounds(  int needleBgId, int newBgId );
    void set_background( int layerBeingChanged, int needleBgId, int newBgId );

    int get_twsize();
    int get_thsize();
    int get_map_width();
    int get_map_height();
    void apply_logic();
    void apply_postlogic();
    std::string get_scene_name();
    void set_scene_name(std::string inSceneName);
    void change_background( int newBG, int bgNumber);
    void change_foreground( int newBG, int bgNumber);
    int snap_xto_grid(int xPos);
    int snap_yto_grid(int yPos);
    void load_game_scene(std::string sceneFileName);
    void load_and_parse_scene();
    bool process_scene();
    void render_background_color();
    void render_backgrounds();
    void render_foregrounds();
    void render_scene();

    bool scene_is_ready();
};

#endif
