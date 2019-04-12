/*
GPE_Basic_Object.cpp
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

#include "GPE_Basic_Object.h"
#include "GPE_Color_Manager.h"

GPE_GameObject::GPE_GameObject(int objectType,  int xPosIn,  int yPosIn,  int objectLayerId)
{
    if( objectType >=0 && objectType < objectType)
    {
        //gpeObjName = GPE_OBJ_NAMES[ objectType];
    }
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
    GPE_AddedToGridSystem = false;
    gpeLogicAppliedInFrame = false;
    gpePrelogicAppliedInFrame = false;
    gpeAddedToRenderList = false;
    gpeAddedToContinuousList = false;
    hasPreRenderFunction = false;
    hasRenderFunction = true;
    hasPostRenderFunction = false;
    gpeHasTimedFunctions = false;
    hasHudRenderFunction = false;
    gpeHasStartLogicFunction = false;
    gpeHasLogicFunction = false;
    gpeSceneStartCodeUsed = false;

    for( int iView = 0; iView < GPE_MAX_CAMERAS; iView++)
    {
        prerenderedInFrame[iView] = false;
        renderedInFrame[iView] = false;
        renderedAboveInFrame[iView] = false;
        hudrenderedInFrame[iView] = false;
        isInView[iView] = false;
        timeToCheckForInView[iView] = 0;
    }

    for( int iTimer = 0; iTimer < GPE_MAX_DEFAULT_TIMERS; iTimer++)
    {
        functionTimer[iTimer] = 0;
    }
    collisionBox = new GPE_Rect();
    collisionBox->update_shape( xPosIn, yPosIn, 32, 32);
    /************ END OF GETTER/SETTER ONLY VARIABLES *///////////////
    isOutsideScene = false;
    lastCollisionObjId = -1;
    applyGravity = true;
    gravityDirection = 270;
    gravityEffect = 0;
    spriteFrame = 0;
    imageSpeed = 0;
    animationEnded = false;
    isVisible = true;
    xBegin = xPosIn;
    xPos = xPosIn;
    x2Pos = xPosIn + 32;
    yBegin = yPosIn;
    yPos = yPosIn;
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
}

GPE_GameObject::~GPE_GameObject()
{

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

bool GPE_GameObject::is_visible()
{
    return isVisible;
}

void GPE_GameObject::perform_object_logic()
{

}

void GPE_GameObject::perform_object_prelogic()
{

}

void GPE_GameObject::render_above()
{

}
void GPE_GameObject::render_hud()
{

}

void GPE_GameObject::render_under()
{

}

void GPE_GameObject::render_self()
{

}

void GPE_GameObject::scene_end()
{

}
void GPE_GameObject::scene_start()
{

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

void GPE_GameObject::timedFunction0()
{

}

void GPE_GameObject::timedFunction1()
{

}

void GPE_GameObject::timedFunction2()
{

}

void GPE_GameObject::timedFunction3()
{

}

void GPE_GameObject::timedFunction4()
{

}

void GPE_GameObject::timedFunction5()
{

}

void GPE_GameObject::timedFunction6()
{

}

void GPE_GameObject::timedFunction7()
{

}

void GPE_GameObject::timedFunction8()
{

}

void GPE_GameObject::timedFunction9()
{

}

void GPE_GameObject::update_animation()
{

}

void GPE_GameObject::update_cords(double newX,  double newY)
{

}

GPE_GameObject_List::GPE_GameObject_List()
{

}

GPE_GameObject_List::~GPE_GameObject_List()
{
    internalArray.clear();
}

void GPE_GameObject_List::add_object(GPE_GameObject * objectIn)
{
    if( objectIn!=NULL)
    {
        internalArray.push_back(objectIn);
    }
}

void GPE_GameObject_List::remove_held_object(GPE_GameObject * objectIn)
{
    if( objectIn!=NULL )
    {
        GPE_GameObject * foundHeldObject = NULL;
        for( int ii = (int)internalArray.size()-1;  ii>=0; ii--)
        {
            foundHeldObject = internalArray[ii];
            if( foundHeldObject->get_id()==objectIn->get_id() )
            {
                internalArray.erase( internalArray.begin()+ii);
            }
        }
    }
}
