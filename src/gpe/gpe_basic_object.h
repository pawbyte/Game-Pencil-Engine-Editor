/*
gpe_basic_object.h
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
        private:
            bool isMovingObject;
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
            bool pathGoingForward;

            float pathSpeed;
            float pathCurrentSpeed;
            float pathCurrentDirection;
            //Path Related Variables [END]
            int gpeObjectType;
            std::string gpeObjName;

            //Animation variables
            std::string animation_id;
            std::string collisionMask;
            animation2d * animationPtr;
            animation2d * collisionMaskPtr;
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
            bool objectMovedLastFrame;
        public:
            bool useCollisonRect;
            bool isContinuousObject;
            bool gpeIsBeingDestroyed;
            int sceneLayerStartPos;
            int gpeSceneStartId;
            color * outlineTestColor;
            int depth;
            int prevdepth;
            int depthPosition;
            bool GPE_AddedToGridSystem;
            bool gpeLogicAppliedInFrame;
            bool gpePrelogicAppliedInFrame;
            bool gpeAddedToRenderList;
            bool gpeSceneStartCodeUsed;

            game_object(  int object_layer_id, int x_pos_in,  int y_pos_in, int z_pos_in = 0 );
            virtual ~game_object();

            /*
            Functions are now all alphabetized for consistency
            */

            //A Functions
            void add_collision_object( game_object * other );
            void apply_gravity();

            //B functions
            void bounce_from( game_object * otherObj);

            //C functions
            bool check_collison_with_object(  game_object * otherObj );
            void check_spatial_movement();
            void clear_collisions();
            static game_object * create_object( int object_layer_id, int x_pos_in,  int y_pos_in , int z_pos_in = 0 ) { return new game_object( object_layer_id, x_pos_in,   y_pos_in, z_pos_in ); }

            //D Functions
            void default_load();

            //E functions
            virtual void end_frame_logic();
            bool equals(game_object * otherObj);


            //G / Get functions
            animation2d * get_animation();

            std::string get_animation_name();
            int get_collision_height();
            std::string get_collision_mask_name();
            animation2d * get_collision_mask();
            int get_collision_space_id();
            int get_collision_width();
            std::string get_component(std::string componentName);
            int get_depth();
            float get_my_direction(float x2,float  y2);
            float get_my_distance(float x2, float y2);
            int get_id();
            int get_layer_id();
            std::string get_name();
            int get_object_type();
            int get_path_id();
            int get_path_original_size();
            int get_path_position();
            int get_path_size();
            float get_path_targetx();
            float get_path_targety();
            float getx2();
            float gety2();

            //I functions
            void initialize();
            bool is_init();
            bool is_moving_object();

            //M functions
            bool make_moving_object();
            bool make_static_object();
            void make_view_dependent();
            void make_view_independent();
            void move_left(float leftMore);
            void move_right(float rightMore);
            void move_up(float upMore);
            void move_down(float downMore);
            void move_towards( float targX, float targY, float newSpeed);

            //P functions

            void path_begin(int path, float pathSpeed, bool continueOnEnd = true, bool relativePath = false);
            void path_end();
            bool path_move_towards_point(int pointId);
            bool path_move_along();
            void path_start(int path, float pathSpeed, bool continueOnEnd = true, bool relativePath = false);
            void path_stop();
            virtual void perform_object_logic();
            virtual void perform_object_prelogic();
            virtual void process_collision( game_object * other );
            void process_gpe_movement();
            void process_timed_functions();

            //R Functions
            virtual void render();
            virtual void render_start(); //Useful for rendering on start(i.e, floors and shadows)
            virtual void render_end(); //Useful for post-rendering(i.e  effects and texts)
            virtual void render_hud(); //Useful for rendering on the HUD level
            virtual void render_hud_start(); //Useful for rendering at start the HUD level
            virtual void render_hud_end(); //Useful for rendering on last layer of HUD level

            void reset_branch();

            //S Functions
            virtual void scene_end();
            virtual void scene_start();
            virtual bool self_destruct();

            void set_animation(std::string nextAnimation, bool animIsMaskToo = false);
            void set_angle(float angle );
            void set_collision_mask( std::string animationMask );
            void set_collision_space_id( int nSpaceId, bool updatedOnGrid = false );
            void set_countdown ( int timerId, int countDownStart);
            void set_layer_id( int nLayerId, bool updatedOnGrid = false );
            void set_timer ( int timerId, int secondsToCount);
            void set_velocity(float newDir, float newSpeed );

            void setx(float x_new);
            void sety(float y_new);

            void speed_move();
            void start_countdown ( int timerId, int countDownStart);
            virtual void start_frame_logic();
            void start_timer ( int timerId, int secondsToCount);

            //T Functions
            bool touches_tile(int layerToCheck);
            bool touches_certain_tile(int layerToCheck, int needleTile);

            //U functions
            bool under_mouse( int cameraId );
            void update_cords( float x_new,  float y_new);
            bool using_path();
            void update( float delta_time );
            void update_animation();

    };

    bool compare_depth(game_object * a, game_object * b);
    bool compare_depth_desc(game_object * a, game_object * b);

}

#endif //gpe_basic_object_h
