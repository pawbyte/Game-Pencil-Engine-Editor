/*
GPE_Basic_Object.h
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

#ifndef GPE_BASIC_OBJECT_H
#define GPE_BASIC_OBJECT_H

#include "GPE_Camera.h"
#include "GPE_Color.h"
#include "GPE_Constants.h"

const int GPE_MAX_DEFAULT_TIMERS = 10;
extern int GPE_MAX_OBJECT_TYPES;

//The base class for Game Objects
class GPE_GameObject
{
private:
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
    double pathSpeed;
    double pathCurrentSpeed;
    double pathCurrentDirection;
    bool pathGoingForward;
    //Path Related Variables [END]
    int gpeObjectType;
    std::string gpeObjName;

    int animId;
    int gpeUniqGameObjectId;

    bool isOutsideScene;
    int lastCollisionObjId;
    bool applyGravity;
    double gravityDirection;
    double gravityEffect;
    double spriteFrame;
    double imageSpeed;
    bool animationEnded;
    bool isVisible;
    double xBegin;
    double xPos;
    double x2Pos;
    double yBegin;
    double yPos;
    double y2Pos;
    double xEnd;
    double yEnd;
    double xVel;
    double yVel;
    double xPast;
    double x2Past;
    double yPast;
    double y2Past;
    double xspeed;
    double yspeed;
    double direction;
    double speed;
    double defaultSpeed;
    int animationDuration;
    bool isInit;
    bool isViewIndependent;
    bool isInView[GPE_MAX_CAMERAS];
    bool timeToCheckForInView[GPE_MAX_CAMERAS];
    GPE_Rect * collisionBox;
public:
    bool gpeIsBeingDestroyed;
    bool prerenderedInFrame[GPE_MAX_CAMERAS];
    bool renderedInFrame[GPE_MAX_CAMERAS];
    bool renderedAboveInFrame[GPE_MAX_CAMERAS];
    bool hudrenderedInFrame[GPE_MAX_CAMERAS];

    int functionTimer[GPE_MAX_DEFAULT_TIMERS];
    int sceneLayerPos;
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
    bool hasPreRenderFunction;
    bool hasRenderFunction;
    bool hasPostRenderFunction;
    bool gpeHasTimedFunctions;
    bool hasHudRenderFunction;
    bool gpeHasStartLogicFunction;
    bool gpeHasLogicFunction;
    bool gpeSceneStartCodeUsed ;

    std::vector< int >OBJECT_COLLISION_LIST;
    std::vector< int >SPATIAL_PARTITION_LIST;
    int GPE_SearchForCollisionBegin;

    GPE_GameObject( int objectType,  int xPosIn,  int yPosIn,  int objectLayerId);
    ~GPE_GameObject();
    virtual void timedFunction0();
    virtual void timedFunction1();
    virtual void timedFunction2();
    virtual void timedFunction3();
    virtual void timedFunction4();
    virtual void timedFunction5();
    virtual void timedFunction6();
    virtual void timedFunction7();
    virtual void timedFunction8();
    virtual void timedFunction9();
    bool under_mouse( int cameraId );
    GPE_GameObject * add_object(int objType, int nX, int nY);
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

    double get_my_direction(double x2,double  y2);
    double get_my_distance(double x2, double y2);
    int get_id();
    int get_object_type();
    std::string get_name();
    int get_sprite();
    double getx();
    double getx2();
    double gety();
    double gety2();
    virtual void end_frame_logic();
    void gravity_use();
    bool is_init();
    bool is_visible();
    void init_sprite(int nextSprite );
    void make_view_dependent();
    void make_view_independent();
    void move_left(double leftMore);
    void move_right(double rightMore);
    void move_up(double upMore);
    void move_down(double downMore);
    void move_towards( double targX, double targY, double newSpeed);
    void path_begin(int path, double pathSpeed, bool continueOnEnd = true, bool relativePath = false);
    void path_start(int path, double pathSpeed, bool continueOnEnd = true, bool relativePath = false);
    bool path_move_along();
    bool path_move_towards_point(int pointId);
    void path_end();
    void path_stop();
    bool using_path();
    int get_path_id();
    //get_path_object();
    double get_path_targetx();
    double get_path_targety();
    int get_path_position();
    int get_path_original_size();
    int get_path_size();
    virtual void perform_object_logic();
    virtual void perform_object_prelogic();
    void process_gpe_movement();
    void process_timed_functions();
    virtual void render_above();
    virtual void render_under();
    virtual void render_hud();
    virtual void render_self();
    virtual void scene_end();
    virtual void scene_start();
    virtual void self_obliterate();
    void self_destruct();
    void set_sprite(int nextSprite = -1);
    void setx(double newX);
    void sety(double newY);
    void speed_move();
    void set_velocity(double newDir, double newSpeed );
    void start_countdown ( int timerId, int countDownStart);
    void set_countdown ( int timerId, int countDownStart);
    void set_timer ( int timerId, int secondsToCount);
    void start_timer ( int timerId, int secondsToCount);
    virtual void start_frame_logic();
    void update_animation();
    void update_cords( double newX,  double newY);
    bool touches_tile(int layerToCheck);
    bool touches_certain_tile(int layerToCheck, int needleTile);
};

class GPE_GameObject_List
{
public:
    std::vector<GPE_GameObject *> internalArray;
    int objListType;
    GPE_GameObject_List();
    ~GPE_GameObject_List();
    void add_object(GPE_GameObject * objectIn);
    void remove_held_object(GPE_GameObject * objectIn);

};

#endif
