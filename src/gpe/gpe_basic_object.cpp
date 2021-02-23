/*
gpe_basic_object.cpp
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

#include "gpe_artist_base.h"
#include "gpe_basic_object.h"
#include "gpe_color_manager.h"
#include "gpe_runtime.h"

namespace gpe
{
    int object_max_types = 0;

    int spatial_grid_data::sPxWidth = 0;
    int spatial_grid_data::sPxHeight = 0;
    int spatial_grid_data::sGridXAmount = 0;
    int spatial_grid_data::sGridYAmount = 0;
    int spatial_grid_data::sGridTotalCells = spatial_grid_data::sGridXAmount  * spatial_grid_data::sGridYAmount;
    std::map<std::string, bool > spatial_grid_data::collisionsCheckedInFrame;
    bool spatial_grid_data::soryByXAxis = true;

    game_object::game_object(int x_pos_in,  int y_pos_in,  int object_layer_id)
    {
        pathId = -1;
        foundPathObject = -1;
        pathPosition = 0;
        pathRelativeX = 0;
        pathRelativeY = 0;
        pathTargetX = 0;
        pathTargetY = 0;
        pathTargetPosition = 0;
        pathReplayOnEnd = true;
        pathSpeed = 0;
        pathCurrentSpeed = 0;
        pathCurrentDirection = 0;
        pathGoingForward = true;

        sceneLayerPos = object_layer_id;
        sceneLayerStartPos = object_layer_id;
        //customComponents = {};
        gpeSceneStartId = is_null;
        animation_id = is_null;
        outlineTestColor = c_lime;
        objectDepth = 0;
        prevObjectDepth = 0;
        objectDepthPosition = 0;
        gpeObjectType = 0;
        gpeUniqGameObjectId = -1;
        isContinuousObject = false;
        GPE_AddedToGridSystem = false;
        gpeLogicAppliedInFrame = false;
        gpePrelogicAppliedInFrame = false;
        gpeAddedToRenderList = false;
        gpeAddedToContinuousList = false;
        gpeSceneStartCodeUsed = false;
        isMovingObject = false;

        collisionBox = new shape_rect();
        collisionBox->update_shape( x_pos_in, y_pos_in, 32, 32);
        /************ END OF GETTER/SETTER ONLY VARIABLES *///////////////
        isOutsideScene = false;
        lastCollisionObjId = -1;
        applyGravity = true;
        gravityDirection = 270;
        gravityEffect = 0;
        animationFrame = 0;
        imageSpeed = 0;
        animationEnded = false;
        branch_visible = true;
        xBegin = x_pos_in;
        xpos = x_pos_in;
        x2Pos = x_pos_in + 32;
        yBegin = y_pos_in;
        ypos = y_pos_in;
        y2Pos = y_pos_in + 32;
        xEnd = 0;
        yEnd = 0;
        xVel = 0;
        yVel = 0;
        xPast = 0;
        x2Past = 0;
        yPast = 0;
        y2Past = 0;
        xspeed = 0;
        yspeed = 0;
        direction = 0;
        speed = 0;
        defaultSpeed = 0;
        animationDuration = 0;
        gpeIsBeingDestroyed = false;
        isInit = true;
        isViewIndependent = false;
        partitionIndexID = -1;
        partitionIndexNextID = -1;
    }

    game_object::~game_object()
    {

    }

    void game_object::add_collision_object( game_object * other )
    {
        if( other == nullptr )
        {
            return;
        }
        collisionList.push_back( other );
    }
    void game_object::apply_gravity()
    {

    }

    bool game_object::check_collison_with_object(  game_object * otherObj )
    {
        if( otherObj == nullptr )
        {
            return false;
        }


        //if both are rects
        if( useCollisonRect )
        {
            //check the AABB between the two rects
            if( otherObj->useCollisonRect)
            {
                return check_collision_rects( *collisionBox, *(otherObj->collisionBox) );
            }
            else
            {
                return check_collision_circle_rect( *collisionBox, *otherObj->collisionCircle );
            }
        }
        else
        {
            //if self is circle and other is circle
            if( otherObj->useCollisonRect)
            {
                return check_collision_circle_rect( *otherObj->collisionBox, *collisionCircle );
            }
            else
            {
                //defaults to circle and circle check
                return check_collision_circles( *collisionCircle, *otherObj->collisionCircle );
            }
        }
        return false;
    }

    void game_object::check_spatial_movement()
    {
        partitionIndexNextID = (  (xpos / spatial_grid_data::sPxWidth ) * spatial_grid_data::sPxHeight  )+ ( ypos /spatial_grid_data::sPxHeight);

        if( partitionIndexNextID < 0 )
        {
            partitionIndexNextID = 0;
        }
        //else if( partitionIndexNextID >  )
        {

        }
        if( partitionIndexNextID != partitionIndexID )
        {

        }
    }

    void game_object::clear_collisions()
    {
        collisionList.clear();
    }

    void game_object::end_frame_logic()
    {

    }

    int game_object::get_id()
    {
        return gpeUniqGameObjectId;
    }

    int game_object::get_layer_id()
    {
        return sceneLayerPos;
    }

    int game_object::get_object_type()
    {
        return gpeObjectType;
    }

    std::string game_object::get_name()
    {
        return gpeObjName;
    }

    bool game_object::is_init()
    {
        return isInit;
    }

    bool game_object::is_moving_object()
    {
        return isMovingObject;
    }

    void game_object::move_left(float leftMore)
    {

    }

    void game_object::move_right(float rightMore)
    {

    }

    void game_object::move_up(float upMore)
    {

    }

    void game_object::move_down(float downMore)
    {

    }

    void game_object::perform_object_logic()
    {

    }

    void game_object::perform_object_prelogic()
    {

    }

    void game_object::render()
    {
        if( gcanvas!=nullptr )
        {
            gcanvas->render_rectangle( xpos, ypos, xpos + 32, ypos + 32, c_red, false );
        }
    }

    void game_object::reset_branch()
    {
        collisionList.clear();
        beganCollisionSearch = false;
    }

    void game_object::scene_end()
    {

    }
    void game_object::scene_start()
    {

    }

    bool game_object::self_destruct()
    {
        return true;
    }

    void game_object::set_animation(std::string nextAnimation, bool animIsMaskToo )
    {

    }

    void game_object::set_collision_mask( std::string animationMask )
    {

    }

    void game_object::set_angle(float angle )
    {

    }

    void game_object::setx(float x_new)
    {

    }

    void game_object::sety(float y_new)
    {

    }

    void game_object::set_collision_space_id( int nSpaceId, bool updatedOnGrid  )
    {
        partitionIndexNextID = nSpaceId;
        if( updatedOnGrid )
        {
            partitionIndexID = nSpaceId;
        }
    }

    void game_object::start_frame_logic()
    {

    }


    void game_object::update()
    {
        apply_gravity();
        perform_object_logic();
        collisionList.clear(); //clears the collision list of the object
    }


    void game_object::update_animation()
    {

    }

    void game_object::update_cords(float x_new,  float y_new)
    {

    }

}
