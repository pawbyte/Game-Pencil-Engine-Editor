/*
gpe_scene.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef gpe_scene_h
#define gpe_scene_h

#include "gpe_basic_object.h"
#include "gpe_branch.h"
#include "gpe_layer.h"

namespace gpe
{
    class game_scene: public branch
    {
        private:
            int sceneId;
            bool sceneHasBeenActivated;
            int sceneWidth, sceneHeight;
            int spatialWidth, spatialHeight;
            bool sceneIsContinuous;
            bool sceneWasVisitedBefore;
            std::string sceneFilename;
            std::string defaultLayerName;

            std::vector< game_object * >sceneObjects;

            std::map<std::string, branch * > assetsByName;
            std::map<std::string, std::vector< branch * > > assetsByTags;

            std::vector< game_object * >persistentObjects;
            std::vector< scene_layer_meta_data * >scenesStartLayers;

            std::vector<branch *> masterBranchList;
            std::map<std::string, branch * > branchesByName;
            std::map<std::string, std::vector< branch * > > branchesByTags;

            //background data
            color * bg_color;
            int sceneVersion;
            int bgMusic;
            int startAudio;
            int endAudio;

            bool sceneIsProcessed;
            bool sceneResourcesAreProcessed;
            bool sceneIsSpatiallyPartitioned;
        public:
            game_scene(int sId, std::string s_name, std::string sFile);
            ~game_scene();
            void add_branch( branch * newBranch );
            scene_layer * add_layer( int newLayerId, float newLayerAlpha );
            scene_layer * add_start_layer( int newLayerId, float newLayerAlpha );

            branch * find_branch( const std::string branch_name );
            branch * find_tagged_branch( const std::string branch_name );
            int find_tagged_branch_count( const std::string branch_name );
            branch * find_tagged_branch_last( const std::string branch_name );
            std::vector< branch *> find_all_tagged_branches( const std::string branch_name );

            void reset_scene_meta();
            bool add_to_persistent_objects( game_object * nObject);
            void init_collision_handler();

            scene_layer * find_layer ( int layerIdIn);
            scene_layer * get_layer ( int layerIdI );

            int get_scene_width();
            int get_scene_height();

            bool scene_activated();
            void scene_init();
            void update_scene_specs( int scene_width, int scene_height);
            void update_tilemap();
            void add_background( int bSurf, bool loopH, bool loopV, float bgX, float bgY, float bgXSpd, float bgYSpd);
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
            void change_background( int new_bg_id, int bgNumber);
            void change_foreground( int new_bg_id, int bgNumber);

            bool load_scene(std::string sceneFileName);
            void load_and_parse_scene();
            bool process_scene();
            void render_background_color();
            void render_backgrounds();
            void render_foregrounds();
            void render_scene();
            bool scene_is_ready();
            void scene_restart();
    };
}

#endif //gpe_scene_h
