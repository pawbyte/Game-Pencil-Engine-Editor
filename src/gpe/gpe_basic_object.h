/*
gpe_basic_object.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef gpe_basic_object_h
#define gpe_basic_object_h

#include "gpe_camera.h"
#include "gpe_color.h"
#include "gpe_constants.h"
#include "gpe_branch.h"
#include "gpe_animation2d.h"

namespace gpe
{
    extern int object_max_types;
    //used to keep track of the spatial grid
    //should only by edited by the scene/runtime, not local objects.

    namespace spatial_grid_data
    {
        extern int sPxWidth;
        extern int sPxHeight;
        extern int sGridXAmount;
        extern int sGridYAmount;
        extern int sGridTotalCells;
        extern bool soryByXAxis;
        extern std::map<std::string, bool > collisionsCheckedInFrame;
    };

    //The base class for Game Objects
    class game_object: public branch
    {
        protected:
            //Path Related Variables [BEGIN]
            std::string pathId;
            int foundPathObject;
            int pathPosition;
            int pathRelativeX;
            int pathRelativeY;
            int pathTargetX;
            int pathTargetY;
            int pathTargetPosition;
            bool pathReplayOnEnd;
            float pathSpeed;
            float pathCurrentSpeed;
            float pathCurrentDirection;
            bool pathGoingForward;
            //Path Related Variables [END]
            int gpeObjectType;
            std::string gpeObjName;

            //Animation variables
            std::string animation_id;
            std::string collisionMask;
            animaton2d * animationPtr;
            animaton2d * collisionMaskPtr;
            float animationFrame;
            float imageSpeed;
            bool animationEnded;
            int animationDuration;

            int gpeUniqGameObjectId;

            bool isOutsideScene;
            int lastCollisionObjId;
            bool applyGravity;
            float gravityDirection;
            float gravityEffect;
            float xBegin;
            float x2Pos;
            float yBegin;
            float y2Pos;
            float xEnd;
            float yEnd;
            float xVel;
            float yVel;
            float xPast;
            float x2Past;
            float yPast;
            float y2Past;
            float xspeed;
            float yspeed;
            float direction;
            float speed;
            float defaultSpeed;
            bool isInit;
            bool isMovingObject;
            bool isViewIndependent;

            shape_rect * updatedCollisionBox;
            shape_rect * collisionBox;
            shape_circle * updatedCollisionCircle;
            shape_circle * collisionCircle;

            int sceneLayerPos;
            int partitionIndexID;
            int partitionIndexNextID;
            std::vector< game_object *> collisionList;
            int beganCollisionSearch;
        public:
            bool useCollisonRect;
            bool isContinuousObject;
            bool gpeIsBeingDestroyed;
            int sceneLayerStartPos;
            int gpeSceneStartId;
            color * outlineTestColor;
            int objectDepth;
            int prevObjectDepth;
            int objectDepthPosition;
            bool GPE_AddedToGridSystem;
            bool gpeLogicAppliedInFrame;
            bool gpePrelogicAppliedInFrame;
            bool gpeAddedToRenderList;
            bool gpeAddedToContinuousList;
            bool gpeSceneStartCodeUsed;

            game_object(  int x_pos_in,  int y_pos_in,  int object_layer_id);
            virtual ~game_object();
            void apply_gravity();
            void check_spatial_movement();
            void clear_collisions();
            void add_collision_object( game_object * other );
            static game_object * create_object(int x_pos_in,  int y_pos_in,  int object_layer_id) { return new game_object( x_pos_in,   y_pos_in, object_layer_id); }
            bool check_collison_with_object(  game_object * otherObj );
            bool under_mouse( int cameraId );
            void bounce_from( game_object * otherObj);
            void initialize();
            bool equals(game_object * otherObj);
            void default_load();
            std::string get_animation_name();

            animaton2d * get_animation();
            int get_collision_height();
            std::string get_collision_mask_name();
            animaton2d * get_collision_mask();

            int get_collision_space_id();
            int get_collision_width();
            std::string get_component(std::string componentName);
            int get_depth();

            float get_my_direction(float x2,float  y2);
            float get_my_distance(float x2, float y2);
            int get_id();
            int get_object_type();
            std::string get_name();
            float getx2();
            float gety2();

            int get_layer_id();
            virtual void end_frame_logic();

            bool is_init();
            bool is_moving_object();
            void make_view_dependent();
            void make_view_independent();
            void move_left(float leftMore);
            void move_right(float rightMore);
            void move_up(float upMore);
            void move_down(float downMore);
            void move_towards( float targX, float targY, float newSpeed);
            void path_begin(int path, float pathSpeed, bool continueOnEnd = true, bool relativePath = false);
            void path_start(int path, float pathSpeed, bool continueOnEnd = true, bool relativePath = false);
            bool path_move_along();
            bool path_move_towards_point(int pointId);
            void path_end();
            void path_stop();
            bool using_path();
            int get_path_id();
            //get_path_object();
            float get_path_targetx();
            float get_path_targety();
            int get_path_position();
            int get_path_original_size();
            int get_path_size();
            virtual void perform_object_logic();
            virtual void perform_object_prelogic();
            void process_gpe_movement();
            void process_timed_functions();
            virtual void render();
            void reset_branch();
            virtual void scene_end();
            virtual void scene_start();
            virtual bool self_destruct();

            void set_animation(std::string nextAnimation, bool animIsMaskToo = false);
            void set_collision_mask( std::string animationMask );
            void set_collision_space_id( int nSpaceId, bool updatedOnGrid = false );
            void set_layer_id( int nLayerId, bool updatedOnGrid = false );

            void set_angle(float angle );
            void setx(float x_new);
            void sety(float y_new);
            void speed_move();
            void set_velocity(float newDir, float newSpeed );
            void start_countdown ( int timerId, int countDownStart);
            void set_countdown ( int timerId, int countDownStart);
            void set_timer ( int timerId, int secondsToCount);
            void start_timer ( int timerId, int secondsToCount);
            virtual void start_frame_logic();
            void update();
            void update_animation();
            void update_cords( float x_new,  float y_new);
            bool touches_tile(int layerToCheck);
            bool touches_certain_tile(int layerToCheck, int needleTile);
    };
}

#endif //gpe_basic_object_h
