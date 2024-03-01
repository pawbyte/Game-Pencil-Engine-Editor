/*
game_state_master.h
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

#ifndef game_state_master_h
#define game_state_master_h

#include "gpe/gpe.h"

class game_master: public gpe::program_state
{
    private:
        gpe::shape_rect * HUD_RECT;
        int sceneWidth;
        int sceneHeight;
        //Path related functions/objects/variables
        std::vector < gpe::game_path2d * > gamePaths;

        //Scene Variables / Data
        std::vector < gpe::game_scene * > gpeScenes;
        gpe::game_scene * scene_current;
        int scene_position;
        int scene_currentId;
        int pastSceneId;
        std::string pastSceneName;
        bool sceneWasReset;
        int sceneToEnter;

        //Used for handling mouse inputs
        float mouse_cam_x[gpe::max_cameras_allowed];
        float mouse_cam_y[gpe::max_cameras_allowed];
        bool mouse_in_cam[gpe::max_cameras_allowed];
        //scene_current.load_game_scene(GPE_SETTINGS_START_SCENE,'text');

        //Used to keep track of game objects
        std::vector < gpe::game_object_list * > game_objects; //listed version of game objects based on object types
        std::vector < gpe::game_object * > game_objects_delete_list;
        std::vector < gpe::game_object * > game_objects_delete_types;
        std::vector < gpe::game_object * > game_objects_continuous;
        bool needToDeleteObjects;
        //game_objects_FAMILIES = [];
        int object_count;

        //Collision related variables
        std::vector < int > collision_spaces_in_view;
        bool spatial_grid_created;
        int spatial_map_width_size;
        int spatial_map_height_size;

        std::vector < bool >object_collision_is_checked;
        /*collisionPairA = [];
        collisionPairB = [];  */
        int collision_pairs_total;

        int parsed_objects;
        int spatial_grid_x_amount;
        int spatial_grid_y_amount;
        int entity_collisions_hashes_checked;
        int spatial_collisions_checked_counter;

        //Debugging variables
        int collisionsCheckedCount;
        int colisionObjectLoops;
        gpe::spatial_partition_controller * spatialPartitonController;

    public:
        int currentCameraInView;
        game_master( std::string s_name = "");
        ~game_master();

        //Main loop functions
        void activate_object_families();
        void apply_game_objects_logic();
        void apply_game_objects_prelogic();
        void apply_logic();
        void init_collision_handler();

        void clean_up();
        void end_state();

        void parse_spatial_collisions();
        void process_input();
        void process_scene_movement();
        void render();
        int remove_deleted_objects();
        void scene_restart_object_info();
        void start_state();
        void update_spaces_in_view();
};


#endif // game_state_master_h
