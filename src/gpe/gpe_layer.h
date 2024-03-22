/*
gpe_layer.h
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

#ifndef gpe_layer_h
#define gpe_layer_h

#include "gpe_background.h"
#include "gpe_basic_object.h"
#include "gpe_branch.h"
#include "gpe_lights2d.h"
#include "gpe_spatial_partition_controller.h"
#include "gpe_tiles.h"

namespace gpe
{
    class scene_layer: public branch
    {
        private:
            std::vector< branch * > master_layer_list;
            int collision_pairs_total;
            int parsed_objects;
            //Spatial Grid stuff
            bool spatial_grid_created;
            int spatial_map_width_size;
            int spatial_map_height_size;
            int spatial_grid_box_size;
            int spatial_grid_width_size;
            int spatial_grid_height_size;
            int spatial_grid_x_amount;
            int spatial_grid_y_amount;
            int entity_collisions_hashes_checked;
            int spatial_collisions_checked_counter;

            std::vector<layer_tile_map * > layer_maps;
            std::vector<game_object * > layer_entities;
            std::vector<game_object * > independent_entities;
            std::vector<background * >layer_backgrounds;
            std::vector< light_basic_2d * > layer_lights_ambient;
            std::vector< light_direction2d * > layer_lights_direction;
            std::vector< light_point2d * > layer_lights_points;
            /*
            scnStartBackgrounds = [];
            scnStartObjects = [];
            scnStartTiles  = [];
            */

            int layer_id;
            int layer_alpha;
            //Collision related variables
            std::vector<spatial_partition * > collision_spaces_in_view;

            std::vector<game_object * > render_list_prerender_entities;
            std::vector<game_object * > render_list_regular_entities;
            std::vector<game_object * > render_list_front_entities;
            std::vector<game_object * > render_list_hud_entities;
        public:
            scene_layer( int newLayerId, float newLayerAlpha = 1);
            ~scene_layer();

            void add_branch( branch * new_branch );
            layer_tile_map *  add_tilemap( std::string name = "", int x = 0, int y = 0, int w = -1, int h = -1 );
            void add_collision_pair( game_object *objPairA, game_object *objPairB);

            void init_collision_handler( int camera_scene_width, int camera_scene_height );
            void begin_render ( std::vector< int >&spacesInView );
            void clear_spaces();

            int get_id();
            void remove_object ( game_object *objToRemove);
            void render_objects();
            void render();
            void replace_all_backgrounds ( int needleBgId, int newBgId );
            void reset_branch();
            void scroll_backgrounds();
            void set_default_grid_size();
            void set_layer_background ( int layerBeingChanged, int bg_id, int newBgId );
            void update( float delta_time );
    };

    struct scene_layer_meta_data
    {
        int lId;
        float lAlpha;
    };
}

#endif // gpe_layer_h
