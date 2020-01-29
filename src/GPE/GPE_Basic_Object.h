/*
GPE_Basic_Object.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef GPE_BASIC_OBJECT_H
#define GPE_BASIC_OBJECT_H

#include "GPE_Camera.h"
#include "GPE_Color.h"
#include "GPE_Constants.h"
#include "GPE_Branch.h"

extern int GPE_MAX_OBJECT_TYPES;

//used to keep track of the spatial grid
//should only by edited by the scene/runtime, not local objects.

namespace gpeSpatialGridData
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
class GPE_GameObject: public GPE_Branch
{
protected:
    //Path Related Variables [BEGIN]
    int pathId;
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

    int animId;
    int gpeUniqGameObjectId;

    bool isOutsideScene;
    int lastCollisionObjId;
    bool applyGravity;
    float gravityDirection;
    float gravityEffect;
    float animationFrame;
    float imageSpeed;
    bool animationEnded;
    bool isVisible;
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
    int animationDuration;
    bool isInit;
    bool isMovingObject;
    bool isViewIndependent;
    bool isInView[GPE_MAX_CAMERAS];
    bool timeToCheckForInView[GPE_MAX_CAMERAS];
    GPE_Rect * setCollisionBox;
    GPE_Rect * collisionBox;
    GPE_Circle * setCollisionCircle;
    GPE_Circle * collisionCircle;
    int sceneLayerPos;
    int partitionIndexID;
    int partitionIndexNextID;
public:
    bool useCollisonRect;
    bool isContinuousObject;
    bool gpeIsBeingDestroyed;
    bool prerenderedInFrame[GPE_MAX_CAMERAS];
    bool renderedInFrame[GPE_MAX_CAMERAS];
    bool renderedAboveInFrame[GPE_MAX_CAMERAS];
    bool hudrenderedInFrame[GPE_MAX_CAMERAS];

    int sceneLayerStartPos;
    /*Make into a MAP
    customComponents = {};
    */
    int gpeSceneStartId;
    GPE_Color * outlineTestColor;
    int objectDepth;
    int prevObjectDepth;
    int objectDepthPosition;
    bool GPE_AddedToGridSystem;
    bool gpeLogicAppliedInFrame;
    bool gpePrelogicAppliedInFrame;
    bool gpeAddedToRenderList;
    bool gpeAddedToContinuousList;
    bool gpeSceneStartCodeUsed;


    std::vector< GPE_GameObject *> collisionList;
    int beganCollisionSearch;

    GPE_GameObject(  int xPosIn,  int yPosIn,  int objectLayerId);
    ~GPE_GameObject();
    void apply_gravity();
    void check_spatial_movement();

    static GPE_GameObject * create_object(int xPosIn,  int yPosIn,  int objectLayerId) { return new GPE_GameObject( xPosIn,   yPosIn, objectLayerId); }
    bool check_collison_with_object(  GPE_GameObject * otherObj );
    bool under_mouse( int cameraId );
    void bounce_from( GPE_GameObject * otherObj);
    virtual void collide_with( GPE_GameObject * otherObj);
    void initialize();
    bool equals(GPE_GameObject * otherObj);
    void default_load();
    std::string get_component(std::string componentName);
    void gpe_constructor();
    int get_depth();
    int get_collision_height();
    int get_collision_width();
    int get_collision_space_id();
    float get_my_direction(float x2,float  y2);
    float get_my_distance(float x2, float y2);
    int get_id();
    int get_object_type();
    std::string get_name();
    int get_animation();
    float getx();
    float getx2();
    float gety();
    float gety2();

    int get_layer_id();
    virtual void end_frame_logic();
    bool is_init();
    bool is_moving_object();
    bool is_visible();
    void init_animation(int nextanimation );
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
    virtual void scene_end();
    virtual void scene_start();
    virtual void self_obliterate();
    void self_destruct();

    void set_animation(int nextanimation = -1);

    void set_collision_space_id( int nSpaceId, bool updatedOnGrid = false );
    void set_layer_id( int nLayerId, bool updatedOnGrid = false );

    void setx(float newX);
    void sety(float newY);
    void speed_move();
    void set_velocity(float newDir, float newSpeed );
    void start_countdown ( int timerId, int countDownStart);
    void set_countdown ( int timerId, int countDownStart);
    void set_timer ( int timerId, int secondsToCount);
    void start_timer ( int timerId, int secondsToCount);
    virtual void start_frame_logic();
    void update();
    void update_animation();
    void update_cords( float newX,  float newY);
    bool touches_tile(int layerToCheck);
    bool touches_certain_tile(int layerToCheck, int needleTile);
};

#endif
