/*
gpe_game_master_state.h
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

#ifndef gpe_game_master_state_h
#define gpe_game_master_state_h

#include "gpe_runtime.h"
#include "gpe_font_base.h"
#include "gpe_ini_file.h"
#include "gpe_layer.h"
#include "gpe_particles2d.h"
#include "gpe_parser.h"
#include "gpe_path2d.h"
#include "gpe_program_state.h"
#include "gpe_scene.h"
#include "gpe_file_system.h"
#include "gpe_basic_object_list.h"
#include "gpe_basic_object_type_mapper.h"

namespace gpe
{
    class game_master: public program_state
    {
        private:
            gpe::shape_rect * HUD_RECT;
            int sceneWidth;
            int sceneHeight;
            //Path related functions/objects/variables
            std::vector < game_path2d * > gamePaths;

            //Scene Variables / Data
            std::vector < game_scene * > gpeScenes;
            game_scene * currentScene;
            int gpeScenePos;
            int currentSceneId;
            int pastSceneId;
            std::string pastSceneName;
            bool sceneWasReset;
            int sceneToEnter;

            //Used for handling mouse inputs
            float MOUSE_CAM_X[max_cameras_allowed];
            float MOUSE_CAM_Y[max_cameras_allowed];
            bool MOUSE_IN_CAM[max_cameras_allowed];
            //currentScene.load_game_scene(GPE_SETTINGS_START_SCENE,'text');

            //Used to keep track of game objects
            std::vector < game_object_list * > GAME_OBJECTS; //listed version of game objects based on object types
            std::vector < game_object * > GAME_OBJECTS_TO_BE_DESTROYED;
            std::vector < game_object * > GAME_OBJECTS_TO_DELETE_TYPES;
            std::vector < game_object * > GAME_OBJECTS_CONTINUOUS;
            bool needToDeleteObjects;
            //GAME_OBJECTS_FAMILIES = [];
            int OBJECT_COUNT;
            bool main_GAME_CODE_IS_READ;
            //Collision related variables
            std::vector < int > collisionSpacesInView;
            bool spatialGridIsCreated;
            int spatialMapWidthSize;
            int spatialMapHeightSize;

            std::vector < bool >objectCollisionIsChecked;
            /*collisionPairA = [];
            collisionPairB = [];  */
            int collisionPairsTotal;

            int parsed_objects;
            int spatialGridWidthAmount;
            int spatialGridHeightAmount;
            int objectCollisionHashesChecked;
            int spatialCollisionChecks;

            //Debugging variables
            int collisionsCheckedCount;
            int colisionObjectLoops;
            spatial_partition_controller * spatialPartitonController;

        public:
            int currentCameraInView;
            game_master( std::string sName = "");
            ~game_master();

            //Main loop functions
            void activate_object_families();
            void apply_game_objects_logic();
            void apply_game_objects_prelogic();
            void apply_logic();
            void init_spatialpartioning();

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
}

#endif // GPE_GAME_MASTER_H
