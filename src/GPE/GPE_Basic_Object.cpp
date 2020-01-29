/*
GPE_Basic_Object.cpp
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

#include "GPE_Basic_Object.h"
#include "GPE_Color_Manager.h"
#include "GPE_Engine.h"
#include "GPE_Artist_Base.h"

int gpeSpatialGridData::sPxWidth = 0;
int gpeSpatialGridData::sPxHeight = 0;
int gpeSpatialGridData::sGridXAmount = 0;
int gpeSpatialGridData::sGridYAmount = 0;
int gpeSpatialGridData::sGridTotalCells = gpeSpatialGridData::sGridXAmount  * gpeSpatialGridData::sGridYAmount;
std::map<std::string, bool > gpeSpatialGridData::collisionsCheckedInFrame;
bool gpeSpatialGridData::soryByXAxis = true;

GPE_GameObject::GPE_GameObject(int xPosIn,  int yPosIn,  int objectLayerId)
{
    /*if( objectType >=0 && objectType < objectType)
    {
        //gpeObjName = GPE_OBJ_NAMES[ objectType];
    }*/
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

    sceneLayerPos = objectLayerId;
    sceneLayerStartPos = objectLayerId;
    //customComponents = {};
    gpeSceneStartId = IS_NULL;
    animId = IS_NULL;
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
    for( int iView = 0; iView < GPE_MAX_CAMERAS; iView++)
    {
        prerenderedInFrame[iView] = false;
        renderedInFrame[iView] = false;
        renderedAboveInFrame[iView] = false;
        hudrenderedInFrame[iView] = false;
        isInView[iView] = false;
        timeToCheckForInView[iView] = 0;
    }


    collisionBox = new GPE_Rect();
    collisionBox->update_shape( xPosIn, yPosIn, 32, 32);
    /************ END OF GETTER/SETTER ONLY VARIABLES *///////////////
    isOutsideScene = false;
    lastCollisionObjId = -1;
    applyGravity = true;
    gravityDirection = 270;
    gravityEffect = 0;
    animationFrame = 0;
    imageSpeed = 0;
    animationEnded = false;
    isVisible = true;
    xBegin = xPosIn;
    xpos = xPosIn;
    x2Pos = xPosIn + 32;
    yBegin = yPosIn;
    ypos = yPosIn;
    y2Pos = yPosIn + 32;
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

GPE_GameObject::~GPE_GameObject()
{

}

void GPE_GameObject::apply_gravity()
{

}

bool GPE_GameObject::check_collison_with_object(  GPE_GameObject * otherObj )
{
    if( otherObj == NULL )
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

void GPE_GameObject::check_spatial_movement()
{
    partitionIndexNextID = (  (xpos / gpeSpatialGridData::sPxWidth ) * gpeSpatialGridData::sPxHeight  )+ ( ypos /gpeSpatialGridData::sPxHeight);

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

void GPE_GameObject::collide_with( GPE_GameObject * otherObj)
{

}

void GPE_GameObject::end_frame_logic()
{

}

int GPE_GameObject::get_id()
{
    return gpeUniqGameObjectId;
}

int GPE_GameObject::get_layer_id()
{
    return sceneLayerPos;
}

int GPE_GameObject::get_object_type()
{
    return gpeObjectType;
}

std::string GPE_GameObject::get_name()
{
    return gpeObjName;
}

bool GPE_GameObject::is_init()
{
    return isInit;
}

bool GPE_GameObject::is_moving_object()
{
    return isMovingObject;
}

bool GPE_GameObject::is_visible()
{
    return isVisible;
}

void GPE_GameObject::move_left(float leftMore)
{

}

void GPE_GameObject::move_right(float rightMore)
{

}

void GPE_GameObject::move_up(float upMore)
{

}

void GPE_GameObject::move_down(float downMore)
{

}

void GPE_GameObject::perform_object_logic()
{

}

void GPE_GameObject::perform_object_prelogic()
{

}

void GPE_GameObject::render()
{
    if( gcanvas!=NULL )
    {
        gcanvas->render_rectangle( xpos, ypos, xpos + 32, ypos + 32, c_red, false );
    }
}

void GPE_GameObject::scene_end()
{

}
void GPE_GameObject::scene_start()
{

}

void GPE_GameObject::set_collision_space_id( int nSpaceId, bool updatedOnGrid  )
{
    partitionIndexNextID = nSpaceId;
    if( updatedOnGrid )
    {
        partitionIndexID = nSpaceId;
    }
}

void GPE_GameObject::start_frame_logic()
{

}

void GPE_GameObject::self_obliterate()
{

}

void GPE_GameObject::self_destruct()
{

}

void GPE_GameObject::update()
{
    apply_gravity();
    perform_object_logic();
    collisionList.clear(); //clears the collision list of the object
}


void GPE_GameObject::update_animation()
{

}

void GPE_GameObject::update_cords(float newX,  float newY)
{

}
