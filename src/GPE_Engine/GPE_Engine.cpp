/*
GPE_Engine.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the ԓoftwareԩ, to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ԁS ISԬ WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "GPE_Engine.h"
bool GPE_IS_LOADING = false;
int GPE_MAX_OBJECT_TYPES = 0;
GPE_Font * LAST_USED_FONT = NULL;
std::vector < GPE_Font * > loadedGPEFonts;

GPE_Runtime * gpe = NULL;
GPE_Runtime * GPE = NULL;


GPE_PathPoint::GPE_PathPoint(int pointX, int pointY, float speed)
{
    xPos = pointX;
    yPos = pointY;
    pointSpeed = speed;
}

GPE_PathPoint::~GPE_PathPoint()
{

}

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
    spriteId = IS_NULL;
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
    isVisible = false;
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

GPE_SuperSpatialPartition::GPE_SuperSpatialPartition()
{
    boxArea = new GPE_Rect();
    objectsHeld = 0;
    spaceName= "CAS_X2015_";
}

GPE_SuperSpatialPartition::~GPE_SuperSpatialPartition()
{
    empty_list();
    if( boxArea!=NULL)
    {
        delete boxArea;
        boxArea = NULL;
    }
}

void GPE_SuperSpatialPartition::add_object(GPE_GameObject * newObject)
{
    if( newObject!=NULL )
    {
        int fObjType = newObject->get_object_type();
        if( fObjType >=0 && fObjType < (int)heldObjectsArray.size() )
        {
            internalArray[objectsHeld] = newObject;
            heldObjectsArray[newObject->get_object_type() ]->add_object( newObject );
            objectsHeld+=1;
        }
    }
}

void GPE_SuperSpatialPartition::empty_list()
{
    objectsHeld = 0;
    GPE_GameObject_List *  tempList = NULL;
    for( int i = (int)heldObjectsArray.size()-1;i >=0; i-- )
    {
        tempList = heldObjectsArray[i];
        if( tempList!=NULL)
        {
            delete tempList;
            tempList = NULL;
        }
    }
    heldObjectsArray.clear();
    internalArray.clear();
}

int GPE_SuperSpatialPartition::get_size()
{
    return (int)internalArray.size();
}

GPE_GameObject * GPE_SuperSpatialPartition::get_object(int pos)
{
    if( pos >=0 && pos < (int)internalArray.size() )
    {
        return internalArray[pos];
    }
    return NULL;
}

void GPE_SuperSpatialPartition::remove_held_object(GPE_GameObject * objectIn)
{
    if( objectIn!=NULL )
    {
        int fObjType = objectIn->get_object_type();
        GPE_GameObject * foundHeldObject = NULL;
        for( int ii = (int)internalArray.size()-1;  ii>=0; ii--)
        {
            foundHeldObject = internalArray[ii];
            if( foundHeldObject->get_id()==objectIn->get_id() )
            {
                internalArray.erase( internalArray.begin()+ii);
                objectsHeld-=1;
            }
        }
        if( fObjType >=0 && fObjType < (int)heldObjectsArray.size() )
        {
            GPE_GameObject_List * subHeldArray = heldObjectsArray[ fObjType ];
            subHeldArray->remove_held_object( objectIn );
        }
    }
}

void GPE_SuperSpatialPartition::set_space(int newX, int newY, int newW, int newHeight)
{
    boxArea->update_shape(newX, newY, newW, newHeight);
}

GPE_SceneCamera::GPE_SceneCamera( double cameraX, double cameraY, double cameraW, double cameraH, int cameraIdNumber )
{
    cameraRect = new GPE_Rect();
    cameraBoundaries = new GPE_Rect();
    cameraRect->update_shape(cameraX, cameraY, cameraW, cameraH);

    renderRect = new GPE_Rect();
    scaledRect = new GPE_Rect();
    scaledRect->update_shape(cameraX, cameraY, cameraW, cameraH);
    viewTarget = IS_NULL;
    horiBorder = IS_NULL;
    vertBorder = IS_NULL;

    hSpeed = 0;
    vSpeed = 0;

    isVisible = false;

    maxXTilesInView = 0;
    maxYTilesInView = 0;

    cSceneWidth = 0;
    cSceneHeight = 0;
}

GPE_SceneCamera::~GPE_SceneCamera()
{

}

void GPE_SceneCamera::center_camera ( double centerX, double centerY)
{
    cameraRect->update_position(centerX-cameraRect->get_width()/2,centerY-cameraRect->get_height()/2);
    if( cameraRect->get_x2() > cameraBoundaries->get_width() )
    {
        cameraRect->set_x(cameraBoundaries->get_width()-cameraRect->get_width() );
    }
    if( cameraRect->get_y2() > cameraBoundaries->get_height() )
    {
        cameraRect->set_y( cameraBoundaries->get_height()-cameraRect->get_height() );
    }

    if( cameraRect->get_x() < 0 )
    {
        cameraRect->set_x(0);
        //console.log("Setting cam X to 0 for protection...");
    }
    if( cameraRect->get_y() < 0)
    {
        cameraRect->set_y(0);
        //console.log("Setting cam Y to 0 for protection...");

    }
    //console.log( "New Camera Pos:	("+cameraRect.get_x()+" , "+cameraRect.get_y()+")" );

}

void GPE_SceneCamera::move_hori ( double movSpace )
{
    if (movSpace < 0 )
    {
        if( cameraBoundaries->get_x() < cameraRect->get_x() +movSpace )
        {
            cameraRect->add_x( movSpace);
        }
        else
        {
            cameraRect->set_x(0);
        }
    }
    else
    {
        if( cameraBoundaries->get_x2() > cameraRect->get_x2() +movSpace )
        {
            cameraRect->add_x( movSpace);
        }
    }
}

void GPE_SceneCamera::move_vert ( double movSpace )
{
    if (movSpace < 0 )
    {
        if( cameraBoundaries->get_y() < cameraRect->get_y() +movSpace )
        {
            cameraRect->add_y( movSpace);
        }
        else
        {
            cameraRect->set_y(0);
        }
    }
    else
    {
        if( cameraBoundaries->get_y2() > cameraRect->get_y2() +movSpace )
        {
            cameraRect->add_y( movSpace);
        }
    }
}

void GPE_SceneCamera::set_visible( bool visiblility )
{
    isVisible = visiblility;
}

void GPE_SceneCamera::setup_camera (double cameraX,double cameraY, double cameraW, double cameraH)
{
    isVisible = true;
    //console.log("Updaing Camera with ("+cameraX+", "+cameraY+", "+ cameraW+", "+cameraH+") dimensions...");
    renderRect->update_shape(cameraX, cameraY, cameraW, cameraH);
    cameraRect->update_shape(cameraX, cameraY, cameraW, cameraH);
}

void GPE_SceneCamera::update_tile_size (int newTX, int newTY)
{
    maxXTilesInView = newTX;
    maxYTilesInView = newTY;
}

void GPE_SceneCamera::update_screen_space ( double cameraX, double cameraY, double cameraW, double cameraH)
{
    renderRect->update_shape(cameraX, cameraY, cameraW, cameraH);
}

void GPE_SceneCamera::update_view_space ( double cameraX, double cameraY, double cameraW, double cameraH)
{
    cameraRect->update_shape(cameraX, cameraY, cameraW, cameraH);
}

GPE_GameMapTile::GPE_GameMapTile()
{
    tset = IS_NULL;
    tileIdNumb = IS_NULL;
    tSetPos = -1;
    xCoord = 0;
    yCoord = 0;
    tileLayer = -1;
    tileType =0;
    isSolid = false;
    drewBefore = false;
    tileBox = new GPE_Rect();
    tileBox->update_shape(0,0,32,32);
}

GPE_GameMapTile::~GPE_GameMapTile()
{
    if( tileBox!=NULL)
    {
        delete tileBox;
        tileBox = NULL;
    }
}

int GPE_GameMapTile::get_type ()
{
    return tileType;
}

void GPE_GameMapTile::update_box ( int xNew,  int yNew,  int wNew, int hNew)
{
    tileBox->update_shape(xNew,yNew,wNew,hNew);
}

void GPE_GameMapTile::render_self ( GPE_Rect * cam )
{
    //ensures that the Tilesheet for the tile isn't null
    if(tset >=0 && tset < (int)rsm->rTilesheets.size() )
    {
        GPE_Tilesheet * gameTilesheet = rsm->rTilesheets[tset];
        //console.log( tset)
        if( gameTilesheet!=NULL )
        {
            //checks to see if tile is in the camera region
            //if(gpe.check_collision_with_rect(xCoord,yCoord-tileBox.get_height()-cam.get_y(),tileBox.get_width(),tileBox.get_height(),cam) == true )
            //{
                if( gameTilesheet->tsImage!=NULL )
                {
                    if( cam!=NULL)
                    {
                        gameTilesheet->tsImage->render_tex( xCoord - cam->x ,yCoord - cam->y ,tileBox);
                    }
                    else
                    {
                        gameTilesheet->tsImage->render_tex( xCoord ,yCoord ,tileBox);
                    }
                }
            //}
        }
    }
}

void GPE_GameMapTile::render_self_auto ( GPE_Rect * cam)
{
    render_self( cam );
}

GPE_Background::GPE_Background()
{
    stretchBG = false;
    bgId = IS_NULL;
    backgroundTexture = NULL;
    bgXOffset = 0;
    bgYOffset = 0;
    bgXSpeed = 0;
    bgYSpeed = 0;
    bgColor = c_black;
    renderBgSpeed = 0;
    loopBGHori = 0;
    loopBGVert = 0;
}

GPE_Background::~GPE_Background()
{

}

GPE_Color * GPE_Background::get_scene_background_color()
{
    return bgColor;
}


int GPE_Background::get_background_id()
{
    return bgId;
}

void GPE_Background::render_bg_speed( int xx, int yy, GPE_Rect *  cam, GPE_Rect * renderCam)
{
    if(backgroundTexture !=NULL )
    {
        double cameraScaleX = 1;
        double cameraScaleY = 1;
        if( cam!=NULL)
        {
            backgroundTexture->render_tex(  xx*cameraScaleX - cam->get_x(),yy*cameraScaleY- cam->get_y() );
        }
        else
        {
            backgroundTexture->render_tex(  xx*cameraScaleX - cam->get_x(),yy*cameraScaleY );
        }
        //backgroundTexture->render_tex(  xx*cameraScaleX - cam.xPos, yy*cameraScaleY - cam.yPos,backgroundTexture.get_width()*cameraScaleX,backgroundTexture.get_height()*cameraScaleY );
    }
}

void GPE_Background::render_bg( GPE_Rect * renderToArea, GPE_Rect *  cam, GPE_Rect *  renderCam, int lvlW, int lvlH)
{
    bool colCam = true;
    if(backgroundTexture !=NULL )
    {
        if( loopBGHori )
        {
             if( loopBGVert )
             {
                  for( double i=bgXOffset-backgroundTexture->get_width();i<=lvlW+backgroundTexture->get_width(); i+=backgroundTexture->get_width() )
                  {
                       for( double j=bgYOffset-backgroundTexture->get_height();j<=lvlH+backgroundTexture->get_height(); j+=backgroundTexture->get_height() )
                       {
                            //colCam=gpe.check_collision_with_rect (i,j,backgroundTexture->get_width(),backgroundTexture->get_height(),cam );
                            if(colCam)
                            {
                                 //Show the background
                                 render_bg_speed(i, j, cam, renderCam);
                            }
                       }
                  }
             }
             else
             {
                 for(double i=bgXOffset-backgroundTexture->get_width();i<=lvlW+bgXOffset+backgroundTexture->get_width();i+=backgroundTexture->get_width() )
                 {
                      //colCam=gpe.check_collision_with_rect( i, bgYOffset, backgroundTexture->get_width(),backgroundTexture->get_height(),cam );
                      if(colCam)
                      {
                           //Show the background
                           render_bg_speed(i,bgYOffset, cam,renderCam);
                      }
                 }
             }
        }
        else
        {
            if(loopBGVert)
            {
                 for(double j=bgYOffset-backgroundTexture->get_height();j<=lvlH;j+=backgroundTexture->get_height() )
                 {
                      //colCam=gpe.check_collision_with_rect( bgXOffset,j,backgroundTexture->get_width() , backgroundTexture->get_height(),cam );
                      if(colCam)
                      {
                           //Show the background
                           render_bg_speed(bgXOffset,j, cam,renderCam);
                      }
                 }
            }
            else
            {
                 //Show the background
                 render_bg_speed(bgXOffset, bgYOffset, cam,renderCam);
            }
        }
    }

}

void GPE_Background::scroll_bg()
{
    if(backgroundTexture!=NULL && stretchBG==false)
    {
        bgXOffset +=bgXSpeed;
        bgYOffset +=bgYSpeed;
        //If the background has gone too far
        if(loopBGHori)
        {
             if( bgXOffset <= -backgroundTexture->get_width() )
             {
                  //Reset the offset
                  bgXOffset = 0;
             }
             if( bgXOffset >=backgroundTexture->get_width() )
             {
                  //Reset the offset
                 bgXOffset = 0;
             }
        }
        if(loopBGVert)
        {
             if( bgYOffset <= -backgroundTexture->get_height() )
             {
                  //Reset the offset
                  bgYOffset = 0;
             }
             if(bgYOffset >= backgroundTexture->get_height())
             {
                  //Reset the offset
                  bgYOffset = 0;
             }
        }
    }
}

void GPE_Background::set_bgspeed( double speedX, double speedY )
{
        bgXSpeed = speedX;
		bgYSpeed = speedY;
}

void GPE_Background::set_bgoffsets( double nextX, double nextY )
{
    bgXOffset = nextX;
    bgYOffset = nextY;
}

void GPE_Background::set_background( int newBG)
{
    if(  newBG>=0 && newBG < (int)rsm->rTextures.size() )
    {
        bgId = newBG;
        backgroundTexture = rsm->rTextures[newBG ];
        //console.log("Texture changed...");

    }
    else
    {
        bgId = IS_NULL;
        backgroundTexture = NULL;
        //console.log("Unable to set background to "+newBG+".");
    }
}

void GPE_Background::set_background_color(GPE_Color * colorIn)
{
    bgColor=colorIn;
}

GPE_Layer::GPE_Layer(int newLayerType, int newLayerId, double newLayerAlpha )
{
    spatialGridBoxSize = 128;
    spatialGridWidthSize = 128;
    spatialGridHeightSize = 128;
    spatialGridWidthAmount = 2;
    spatialGridHeightAmount = 2;
    layerType = newLayerType;
    layerId = newLayerId;
    layerAlpha = newLayerAlpha;
    tileAmountX = IS_NULL;
    tileAmountY = IS_NULL;
    layerName = "";
    prevTileAmountX = IS_NULL;
    prevTileAmountY = IS_NULL;
    spatialGridIsCreated = false;
    spatialMapWidthSize = 0;
    spatialMapHeightSize = 0;
    COLLISION_AREA_OUTSIDE_SCENE = new GPE_SuperSpatialPartition();
    superDebugCollisions = false;

    spatialGridWidthAmount = 2;
    spatialGridHeightAmount = 2;
    objectCollisionHashesChecked = 0;
    spatialCollisionChecks = 0;

    collisionPairsTotal = 0;
    parsed_objects = 0;
}

GPE_Layer::~GPE_Layer()
{
    clear_spaces();
    if( COLLISION_AREA_OUTSIDE_SCENE!=NULL)
    {
        delete COLLISION_AREA_OUTSIDE_SCENE;
        COLLISION_AREA_OUTSIDE_SCENE = NULL;
    }
    layerObjects.clear();
    independentObjects.clear();
}

void GPE_Layer::add_collision_pair( GPE_GameObject *objPairA, GPE_GameObject *objPairB)
{
    if( objPairA!=NULL && objPairB!=NULL)
    {
        collisionPairA[collisionPairsTotal] = objPairA;
        collisionPairB[collisionPairsTotal] = objPairB;
        collisionPairsTotal+=1;
    }
}

void GPE_Layer::begin_spatialpartioning( int cSceneWidth, int cSceneHeight )
{
	if(spatialGridIsCreated && spatialMapWidthSize == cSceneWidth && spatialMapHeightSize == cSceneHeight)
	{
		/*
		COLLISION_AREA_OUTSIDE_SCENE.empty_list();
		for( var iSpace = COLLISION_AREA_SPACES.size()-1;iSpace >=0; iSpace--)
		{
			COLLISION_AREA_SPACES.at(iSpace).empty_list();
		}
		*/
		//console.log("Not overwriting scene grid...");
	}
	else
	{
		COLLISION_AREA_OUTSIDE_SCENE->empty_list();
		set_default_grid_size();

		clear_spaces();
        if( cSceneWidth>0 && cSceneHeight>0 )
        {
            spatialMapWidthSize = cSceneWidth;
            spatialMapHeightSize = cSceneHeight;

            spatialGridWidthAmount = ceil(cSceneWidth/spatialGridWidthSize);
            if( spatialGridWidthAmount<2)
            {
                spatialGridWidthSize = cSceneWidth/2;
                spatialGridWidthAmount = 2;
            }
            spatialGridHeightAmount = ceil(cSceneHeight/spatialGridHeightSize);
            if( spatialGridHeightAmount<2 )
            {
                spatialGridHeightSize = cSceneHeight/2;
                spatialGridHeightAmount = 2;
            }


            //smallest size possible is 2x2
            int iGrid, jGrid;
            int xStart=0, yStart=0;
            GPE_SuperSpatialPartition* newSpace;
            for( iGrid =0; iGrid < spatialGridWidthAmount; iGrid+=1)
            {
                for( jGrid=0; jGrid < spatialGridHeightAmount; jGrid+=1)
                {
                    newSpace = new GPE_SuperSpatialPartition();
                    newSpace->set_space(xStart,yStart,spatialGridWidthSize,spatialGridHeightSize);
                    yStart+=spatialGridHeightSize;
                    newSpace->spaceName+="testrun_"+int_to_string(iGrid)+"_"+int_to_string(jGrid)+"_test";
                    COLLISION_AREA_SPACES.push_back(newSpace);
                }
                yStart = 0;
                xStart+=spatialGridWidthSize;
            }
            spatialGridIsCreated = true;

        }
        else
        {
            //console.log("Negative dimensions given for spatial partitioning...");
        }
	}

}


void GPE_Layer::begin_render ( std::vector< int >&spacesInView )
{
    prerenderObjectRenderList.clear();
    regularObjectRenderList.clear();
    renderAboveObjectRenderList.clear();
    renderHudObjectRenderList.clear();
    GPE_GameObject *  foundGameObject = NULL;
    int jObjectHolderParse = 0;
    GPE_SuperSpatialPartition *  currentSpace = 0;
    int foundCollisionSpaceId = 0;
    int jLoop = 0;
    //Adds camera independent objects of this layer into render list
    for( jLoop = (int)independentObjects.size() -1; jLoop >=0; jLoop--)
    {
        foundGameObject = independentObjects[jLoop];

        if( foundGameObject->is_init() && foundGameObject->is_visible() && !foundGameObject->gpeIsBeingDestroyed  )
        {
            if(foundGameObject->hasPreRenderFunction)
            prerenderObjectRenderList.push_back(foundGameObject);

            //if( foundGameObject.hasRenderFunction)
            regularObjectRenderList.push_back(foundGameObject);

            if( foundGameObject->hasPostRenderFunction)
            renderAboveObjectRenderList.push_back(foundGameObject);

            if( foundGameObject->hasHudRenderFunction)
            renderHudObjectRenderList.push_back(foundGameObject);

            foundGameObject->gpeAddedToRenderList = true;
        }
    }

    int cSpacesMax = (int)COLLISION_AREA_SPACES.size();
    //Adds camera dependent objects of this layer into render list
    for( jLoop = (int)spacesInView.size()-1; jLoop >=0; jLoop-- )
    {
        foundCollisionSpaceId= spacesInView[jLoop];

        if( foundCollisionSpaceId >=0 && foundCollisionSpaceId < cSpacesMax )
        {
            currentSpace = COLLISION_AREA_SPACES[foundCollisionSpaceId];
            for( jObjectHolderParse = currentSpace->get_size()-1; jObjectHolderParse >=0; jObjectHolderParse-- )
            {
                foundGameObject = currentSpace->get_object( jObjectHolderParse );
                if( !foundGameObject->gpeAddedToRenderList &&  foundGameObject->is_init() && foundGameObject->is_visible() && !foundGameObject->gpeIsBeingDestroyed )
                {
                    if(foundGameObject->hasPreRenderFunction)
                    prerenderObjectRenderList.push_back(foundGameObject);

                    //if( foundGameObject.hasRenderFunction)
                    regularObjectRenderList.push_back(foundGameObject);

                    if( foundGameObject->hasPostRenderFunction)
                    renderAboveObjectRenderList.push_back(foundGameObject);

                    if( foundGameObject->hasHudRenderFunction)
                    renderHudObjectRenderList.push_back(foundGameObject);

                    foundGameObject->gpeAddedToRenderList = true;
                }
            }
        }
    }
}

void GPE_Layer::clear_spaces()
{
    GPE_SuperSpatialPartition *  tempSpace = NULL;
    for( int i = (int)COLLISION_AREA_SPACES.size()-1;i >=0; i-- )
    {
        tempSpace = COLLISION_AREA_SPACES[i];
        if( tempSpace!=NULL)
        {
            delete tempSpace;
            tempSpace = NULL;
        }
    }
    COLLISION_AREA_SPACES.clear();
}

void GPE_Layer::clear_tiles()
{
    GPE_GameMapTile *  tempTile = NULL;
    for( int i = (int)layerTiles.size()-1;i >=0; i-- )
    {
        tempTile = layerTiles[i];
        if( tempTile!=NULL)
        {
            delete tempTile;
            tempTile = NULL;
        }
    }
    layerTiles.clear();
}

void GPE_Layer::clear_dynamic_data()
{
    /*independentObjects = [];
    layerObjects = [];
    layerBackgrounds = [];*/
    clear_spaces();
    clear_tiles();
    COLLISION_AREA_OUTSIDE_SCENE->empty_list();
}

void GPE_Layer::create_new_map ( int newTX, int newTY)
{
    tileAmountX=newTX;
    tileAmountY=newTY;
    int newSize = tileAmountX*tileAmountY;
    int i=0;
    clear_tiles();
    for(i=0;i<newSize;i++)
    {
        layerTiles[i] = new GPE_GameMapTile();;
        layerTiles[i]->tileType = 0;
    }
}

int GPE_Layer::get_id()
{
    return layerId;
}

int GPE_Layer::get_map_size()
{
    return (int)layerTiles.size();
}

int GPE_Layer::get_type()
{
    return layerType;
}

void GPE_Layer::remove_object ( GPE_GameObject * objToRemove)
{
    GPE_GameObject *  tObject = NULL;
    for( int iItr = (int)layerObjects.size()-1; iItr >=0; iItr--)
    {
        tObject = layerObjects[iItr];
        if( tObject->get_id()==objToRemove->get_id() )
        {
            layerObjects.erase( layerObjects.begin()+iItr );
        }
    }
}

int  GPE_Layer::find_spatial_at ( int xIn, int yIn)
{
    int sPar = IS_NULL;
    if( xIn >=0 && yIn >=0 )
    {
        sPar =  (( xIn / spatialGridWidthSize)|0 )*spatialGridHeightAmount;
        sPar+=( yIn / spatialGridHeightSize)|0;
        if( sPar < 0 || sPar >= (int)COLLISION_AREA_SPACES.size() )
        {
            sPar =IS_NULL;
        }
    }
    return sPar;
}

GPE_GameMapTile * GPE_Layer::get_tile_at ( int xIn,  int yIn )
{
    int seekedTile = xIn+ yIn*tileAmountX;
    if(  seekedTile>=0 && seekedTile < (int)layerTiles.size() )
        return layerTiles[seekedTile];
    return NULL;
}

void GPE_Layer::render_objects()
{
    GPE_GameObject * foundGameObject = NULL;
    int jLoop = 0;
    //Renders pre-rendered objects under regular render cals.
    for( jLoop = prerenderObjectRenderList.size()-1; jLoop >=0; jLoop-- )
    {
        foundGameObject = prerenderObjectRenderList[jLoop];
        //if( foundGameObject.prerenderedInFrame[gTemp]==false )
        {
            foundGameObject->render_under();
            //foundGameObject.prerenderedInFrame[gTemp] = true;
        }
    }
    gpe->set_render_alpha(255);

    //Does regular render calls(For objects missing render info. just draws the object's sprite at its x,y.
    for( jLoop = regularObjectRenderList.size()-1; jLoop >=0; jLoop-- )
    {
        foundGameObject = regularObjectRenderList[jLoop];
        //if(  foundGameObject.renderedInFrame[gTemp]==false )
        {
            foundGameObject->render_self();
            //foundGameObject.renderedInFrame[gTemp] = true;
        }
    }
    gpe->set_render_alpha(255);

    //renders the object's 3rd layer render call.
    for( jLoop = renderAboveObjectRenderList.size()-1; jLoop >=0; jLoop-- )
    {
        foundGameObject = renderAboveObjectRenderList[jLoop];
        //if(  foundGameObject.renderedAboveInFrame[gTemp]==false )
        {
            foundGameObject->render_above();
            //foundGameObject.renderedAboveInFrame[gTemp] = true;
        }
    }
    gpe->set_render_alpha(255);
}

void GPE_Layer::render_hud_objects ()
{
    GPE_GameObject *  foundGameObject = NULL;
    int jLoop = 0;
    for( jLoop = (int)renderHudObjectRenderList.size()-1; jLoop >=0; jLoop-- )
    {
        foundGameObject = renderHudObjectRenderList[jLoop];
        //if(  foundGameObject.hudrenderedInFrame[gTemp]==false )
        {
            foundGameObject->render_hud();
            //foundGameObject.hudrenderedInFrame[gTemp] = true;
        }
    }
}

void GPE_Layer::scroll_backgrounds()
{
    for (int iTemp = (int)layerBackgrounds.size()-1; iTemp >=0; iTemp--)
    {
         if( layerBackgrounds[iTemp]!= NULL )
         layerBackgrounds[iTemp]->scroll_bg();
    }
}

//Layer New Background functions added as of Version 1.12 [ BEGIN ]
void GPE_Layer::replace_all_backgrounds ( int needleBgId, int newBgId )
{
    for( int i = (int)layerBackgrounds.size()-1; i >=0; i-- )
    {
        if( layerBackgrounds[i]->get_background_id() == needleBgId )
        {
            layerBackgrounds[i]->set_background( newBgId );
        }
    }
}

void GPE_Layer::set_layer_background ( int layerBeingChanged, int bgId, int newBgId )
{
    if( layerBeingChanged >=0 && layerBeingChanged < (int)layerBackgrounds.size() )
    {
        layerBackgrounds[layerBeingChanged]->set_background( newBgId );
    }
}

void GPE_Layer::set_default_grid_size()
{
    spatialGridBoxSize = 128;
    spatialGridWidthSize = 128;
    spatialGridHeightSize = 128;
}

GPE_GameScene::GPE_GameScene(int sId, std::string sName)
{
    sceneName = sName;
    sceneId = sId;
    sceneHasBeenActivated = false;
    sceneWidth = 1024;
    sceneHeight = 1024;
    sceneWasVisitedBefore = false;

    defaultLayerName = "game - layer";

    //tile data info
    tileAmountX = 0;
    tileAmountY = 0;
    tileWidth = 32;
    tileHeight = 32;

    //background data
    bgColor = c_aqua;
    sceneVersion = -1;
    bgMusic = -1;
    startAudio = -1;
    endAudio = -1;

    sceneIsProcessed = false;
    sceneResourcesAreProcessed = false;
    sceneIsSpatiallyPartitioned = false;
    for(int iV=0; iV < GPE_MAX_CAMERAS; iV++)
    {
        sceneCamera[iV] = new GPE_SceneCamera(0,0,SCREEN_WIDTH ,SCREEN_HEIGHT,iV);
    }
}

GPE_GameScene::~GPE_GameScene()
{
    GPE_SceneCamera * tCamera = NULL;
    for(int iV= GPE_MAX_CAMERAS-1; iV >=0; --iV )
    {
        tCamera = sceneCamera[iV];
        if( tCamera!=NULL)
        {
            delete tCamera;
            tCamera = NULL;
        }
        sceneCamera[iV] = NULL;
    }
}

void GPE_GameScene::disable_camera ( int cameraId)
{
    if( cameraId >=0 && cameraId < GPE_MAX_CAMERAS )
    {
        sceneCamera[cameraId]->set_visible( false );
    }
}

void GPE_GameScene::enable_camera ( int cameraId )
{
    if( cameraId >=0 && cameraId < GPE_MAX_CAMERAS )
    {
        sceneCamera[cameraId]->update_view_space(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
        sceneCamera[cameraId]->update_screen_space(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
        sceneCamera[cameraId]->set_visible( true );
    }
}

void GPE_GameScene::reset_scene_meta()
{
    sceneCamera[0]->update_view_space(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
    sceneCamera[0]->update_screen_space(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
    sceneCamera[0]->set_visible( true );


    sceneCamera[1]->update_view_space(0,0,SCREEN_WIDTH, SCREEN_HEIGHT/2);
    sceneCamera[1]->update_screen_space(0,SCREEN_HEIGHT/2,SCREEN_WIDTH, SCREEN_HEIGHT/2);
    sceneCamera[1]->set_visible( false);
    //useful for swapping between screens per 60 fps
    //splitRenderMode = 0;


    sceneCamera[2]->update_view_space(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
    sceneCamera[2]->update_screen_space(0,SCREEN_HEIGHT/2,SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    sceneCamera[2]->set_visible( false);

    sceneCamera[3]->update_view_space(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
    sceneCamera[3]->update_screen_space(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    sceneCamera[3]->set_visible( false);

    sceneIsSpatiallyPartitioned = false;
    sceneHasBeenActivated = false;
}


void GPE_GameScene::clear_layers_data()
{
    GPE_Layer * cLayer = NULL;
    for(int tl= (int)sceneLayers.size()-1; tl>=0; tl--)
    {
        cLayer = sceneLayers[tl];
        if( cLayer!= NULL )
        {
            cLayer->clear_dynamic_data();
        }
    }
}

bool GPE_GameScene::add_to_persistent_objects( GPE_GameObject * nObject)
{
    if( nObject!= NULL)
    {
        for( int i = (int)persistentObjects.size() - 1; i >=0; i--)
        {
            if( persistentObjects[i]->get_id() == nObject->get_id() )
            {
                return false;
            }
        }
    }
    persistentObjects.push_back( nObject );
    return true;
}


GPE_Layer * GPE_GameScene::find_layer ( int layerIdIn, int seekedType)
{
    GPE_Layer * foundLayer = NULL;
    if( layerIdIn < 32 && layerIdIn>=0 )
    {
        for( int ii = (int)sceneLayers.size() - 1; ii >=0; ii--)
        {
            foundLayer =sceneLayers[ii];
            if( foundLayer!= NULL)
            {
                if( foundLayer->get_id()==layerIdIn && foundLayer->get_type() == seekedType)
                {
                    return foundLayer;
                }
            }
        }
    }
    return NULL;
}


GPE_Layer * GPE_GameScene::get_layer ( int layerIdIn, int newLayerType)
{
    GPE_Layer * foundLayer = NULL;
    if( layerIdIn < 32 && layerIdIn>=0 )
    {
        for( int ii = (int)sceneLayers.size() - 1; ii >=0; ii--)
        {
            foundLayer = sceneLayers[ii];
            if( foundLayer!= NULL)
            {
                if( foundLayer->get_id()==layerIdIn )
                {
                    return foundLayer;
                }
            }
        }
    }
    if(  layerIdIn >=0 && layerIdIn < 32)
    {
        //console.log( "Unable to find layer with ID[" +layerIdIn+"]" );
        return add_layer( newLayerType, layerIdIn,1);

    }
    return NULL;
}

int GPE_GameScene::get_scene_width()
{
    return sceneWidth;
}

int GPE_GameScene::get_scene_height()
{
    return sceneHeight;
}

int GPE_GameScene::find_tile ( int layerToCheck, int xPos, int yPos )
{

    // find rectangle to check (limited to field size):
    xPos = std::max(0, std::min(sceneWidth - 1, (xPos / tileWidth)|0 ) );
    yPos = std::max(0, std::min(sceneHeight - 1,(yPos / tileHeight)|0 ) );
    GPE_Layer * cLayer = find_layer( layerToCheck, GPE_LAYER_TYPE_TILES );
    //if we have an actual layer to look for
    if( cLayer!= NULL )
    {
        GPE_GameMapTile * foundTile = NULL;
        foundTile = cLayer->get_tile_at(xPos, yPos);
        if(foundTile!= NULL )
        {
            return foundTile->tset;
        }
    }
    return IS_NULL;
}

int GPE_GameScene::find_tile_in_box( int layerToCheck, int x1,  int y1,  int x2, int y2 )
{
    int i=0;
    int j=0;
    int l=0;
    int t=0;
    int r=0;
    int b=0;
    GPE_GameMapTile * foundTile = NULL;
    // find rectangle to check (limited to field size):
    l = std::max(0, std::min(sceneWidth - 1, (x1 / tileWidth)|0 ) );
    t = std::max(0, std::min(sceneHeight - 1,(y1 / tileHeight)|0 ) );
    r = std::max(0, std::min(sceneWidth - 1, (x2 / tileWidth)|0 ) );
    b = std::max(0, std::min(sceneHeight - 1, (y2 / tileHeight)|0  ) );
    GPE_Layer * cLayer = find_layer( layerToCheck, GPE_LAYER_TYPE_TILES );
    //if we have an actual layer to look for
    if( cLayer!= NULL )
    {
        for (j = t; j <= b; j += 1)
        {
            for (i = l; i <= r; i += 1)
            {
                foundTile = cLayer->get_tile_at(i, j);
                if(foundTile!= NULL )
                {
                    return foundTile->tset;
                }
            }
        }
    }
    return IS_NULL;
}

bool GPE_GameScene::find_matching_tile( int layerToCheck, int xPos,  int yPos,  int tileTypeToSearchFor)
{
    GPE_Layer * cLayer = find_layer( layerToCheck, GPE_LAYER_TYPE_TILES );
    //if we have an actual layer to look for
    if( cLayer!= NULL )
    {
        GPE_GameMapTile * foundTile = NULL;
        // find rectangle to check (limited to field size):
        xPos = std::max(0, std::min(sceneWidth - 1, (xPos / tileWidth)|0 ) );
        yPos = std::max(0, std::min(sceneHeight - 1,(yPos / tileHeight)|0 ) );
        foundTile = cLayer->get_tile_at(xPos, yPos);
        if( foundTile!= NULL && foundTile->tset == tileTypeToSearchFor )
        {
            return true;
        }
    }
    return false;
}

bool GPE_GameScene::find_matching_tile_in_box( int layerToCheck, int x1,  int y1,  int x2, int y2 ,  int tileTypeToSearchFor)
{
    GPE_Layer * cLayer = find_layer( layerToCheck, GPE_LAYER_TYPE_TILES );
    //if we have an actual layer to look for
    if( cLayer!= NULL )
    {
        int l = std::max(0, std::min(sceneWidth - 1, (x1 / tileWidth)|0 ) );
        int t = std::max(0, std::min(sceneHeight - 1,(y1 / tileHeight)|0 ) );
        int r = std::max(0, std::min(sceneWidth - 1, (x2 / tileWidth)|0 ) );
        int b = std::max(0, std::min(sceneHeight - 1, (y2 / tileHeight)|0  ) );
        GPE_GameMapTile * foundTile = NULL;
        int i=0;
        int j=0;
        // find rectangle to check (limited to field size):
        for (j = t; j <= b; j += 1)
        {
            for (i = l; i <= r; i += 1)
            {
                foundTile = cLayer->get_tile_at(i, j);
                if(foundTile!= NULL && foundTile->tset == tileTypeToSearchFor )
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GPE_GameScene::scene_is_ready()
{
    return sceneHasBeenActivated;
}

void GPE_GameScene::scene_init()
{

}

void GPE_GameScene::update_scene_specs( int lvlW, int lvlH)
{
    /*lvlW = abs(lvlW);
    lvlH = abs(lvlH);
    cSceneWidth = lvlW;
    cSceneHeight = lvlH;
    gpe.sceneWidth = lvlW;
    gpe.sceneHeight = lvlH;
    for(var iV=gpe.maxCameras-1; iV>=0; iV--)
    {
        this.sceneCamera[iV].cameraBoundaries.update_shape(0,0,lvlW, lvlH);
    }*/
}

void GPE_GameScene::update_tilemap()
{

}

void GPE_GameScene::add_background( int bSurf, bool loopH, bool loopV, double bgX, double bgY, double bgXSpd, double bgYSpd)
{

}

void GPE_GameScene::replace_background( int layerBeingChanged, int needleBgId, int newBgId )
{

}

void GPE_GameScene::replace_all_backgrounds(  int needleBgId, int newBgId )
{

}

void GPE_GameScene::set_background( int layerBeingChanged, int needleBgId, int newBgId )
{

}

GPE_Layer * GPE_GameScene::add_layer( int newLayerType, int newLayerId, double newLayerAlpha )
{
    if( newLayerId >=0 && newLayerId < 32 )
    {
        GPE_Layer * newLayer = new GPE_Layer(newLayerType,newLayerId, newLayerAlpha);
        if( newLayerType == GPE_LAYER_TYPE_TILES )
        {
            newLayer->create_new_map( tileAmountX,tileAmountY );
        }
        sceneLayers.push_back(newLayer);
        return newLayer;
    }
    return NULL;
}

GPE_Layer * GPE_GameScene::add_start_layer( int newLayerType, int newLayerId, double newLayerAlpha )
{
    GPE_Layer * newLayer = add_layer( newLayerType, newLayerId, newLayerAlpha );
    if( newLayer!= NULL )
    {
        GPE_SceneLayer_Meta * newStartLayer = new GPE_SceneLayer_Meta();
        newStartLayer->lType= newLayerType;
        newStartLayer->lId = newLayerType;
        newStartLayer->lAlpha = newLayerAlpha;
        scenesStartLayers.push_back( newStartLayer );
    }
    return newLayer;
}

int GPE_GameScene::get_twsize()
{
    return tileWidth;
}

int GPE_GameScene::get_thsize()
{
    return tileHeight;
}

int GPE_GameScene::get_map_width()
{
    return tileAmountX;
}

int GPE_GameScene::get_map_height()
{
    return tileAmountY;
}

void GPE_GameScene::apply_logic()
{

}

void GPE_GameScene::apply_postlogic()
{

}

std::string GPE_GameScene::get_scene_name()
{
    return sceneName;
}

void GPE_GameScene::set_scene_name(std::string inSceneName)
{
    sceneName = inSceneName;
}

GPE_GameScene::change_background( int newBG, int bgNumber)
{

}

GPE_GameScene::change_foreground( int newBG, int bgNumber)
{

}

int GPE_GameScene::snap_xto_grid(int xPos)
{
    if( xPos / tileWidth > tileAmountX)
    {
        return tileAmountX;
    }
    else
    {	return std::max(0,(xPos/tileWidth)|0 );
    }
}

int GPE_GameScene::snap_yto_grid(int yPos)
{
    if( yPos / tileHeight > tileAmountY )
    {
        return tileAmountY;
    }
    else
    {
        return std::max(0,( yPos/tileHeight)|0 );
    }
}

void GPE_GameScene::load_game_scene(std::string sceneFileName)
{

}

void GPE_GameScene::load_and_parse_scene()
{

}

bool GPE_GameScene::process_scene()
{

}

void GPE_GameScene::render_background_color()
{

}

void GPE_GameScene::render_backgrounds()
{

}

void GPE_GameScene::render_foregrounds()
{

}

void GPE_GameScene::render_scene()
{
    /*
    //gpe.GPE_Current_Context = renderToArea;
    if( sceneResourcesAreProcessed && sceneIsProcessed)
    {
        int tempTileId;
        GPE_GameMapTile * cTile = 0;
        int gTemp = 0;
        int hTemp=0;
        int iTemp=0;
        int jTemp=0;
        int kTemp = 0;
        int mainRenderToArea = renderToArea;
        GPE_SceneCamera * camInUse = 0;
        GPE_Layer * cLayer = 0;

        int rendableCameraCount = 0;

        for (gTemp = 0; gTemp < GPE_MAX_CAMERAS; gTemp++)
        {
            camInUse = sceneCamera[ gTemp];
            if( camInUse.isVisible)
            {
                rendableCameraCount+=1;
            }
        }

        GPE.currentCameraInView = 0;
        //

        var cSpacesIn = state_controller.collisionSpacesInView;
        gpe.render_calls = 0;

        //Loops through each scene layer to prepare for rendering...
        for ( gTemp= (int)sceneLayers.size()-1; gTemp >=0; gTemp-- )
        {
            cLayer = sceneLayers.at(gTemp);
            if(cLayer!=-1 )
            {
                cLayer->begin_render( cSpacesIn );
            }
        }

        var iYTempFrom = 0;
        var jXTempFrom = 0;
        var iYTempTo = 0;
        var jXTempTo = 0;
        //Renders each camera
        for (gTemp = sceneCamera.length - 1; gTemp >= 0; gTemp-- )
        {
            GPE.currentCameraInView = gTemp;
            camInUse = sceneCamera[ gTemp];
            if( camInUse.isVisible == true )
            {
                renderToArea = gpe.GPE_Current_Context = gpe.EXTRA_CONTEXTS[gTemp];
                renderToArea.fillStyle = bgColor;
                gpe.set_render_alpha(255);
                renderToArea.fillRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);

                //in theory this will render at 60 FPS with alternating switching between even and odd cameras(30FPS per camera) unless its one camera which goes at 60 FPS
                //if(  (rendableViewCount<2) || (gTemp%2 == 0 && splitRenderMode%2==0) || (gTemp%2 != 0 && splitRenderMode%2!=0) )
                {

                    //Variables relaed to rendering layers and tiles
                    hTemp = 0;
                    jTemp = 0;

                    iYTempFrom = camInUse.cameraRect.get_y();
                    jXTempFrom = camInUse.cameraRect.get_x();

                    iYTempTo = camInUse.cameraRect.get_y()+camInUse.cameraRect.get_height();
                    jXTempTo = camInUse.cameraRect.get_x()+camInUse.cameraRect.get_width();

                    //Used to offset render calls by camera data.
                    gpe.CURRENT_VIEW = camInUse;
                    gpe.CURRENT_VIEW_BOX = camInUse.cameraRect;

                    for  (hTemp=0; hTemp<sceneLayers.length; hTemp++ )
                    {
                        cLayer = sceneLayers.at(hTemp);
                        if(cLayer!=-1 )
                        {
                            if( cLayer.layerType == GPE_LAYER_TYPE_TILES )
                            {
                                if( tileWidth > 0 && tileHeight > 0 )
                                {
                                    jXTempFrom = (jXTempFrom/tileWidth)|0;
                                    iYTempFrom = (iYTempFrom/tileHeight)|0;
                                    jXTempTo = ( (jXTempTo+tileWidth)/tileWidth)|0;
                                    iYTempTo = ( (iYTempTo+tileHeight)/tileHeight)|0;

                                    for(  iTemp = iYTempFrom; iTemp < iYTempTo; iTemp++)
                                    {
                                        for(  jTemp = jXTempFrom; jTemp < jXTempTo; jTemp++)
                                        {
                                            cTile = cLayer.get_tile_at(jTemp, iTemp);
                                            if( typeof cTile!="undefined" && cTile!=-1)
                                                cTile.render_self( renderToArea,camInUse.cameraRect );
                                        }
                                    }
                                }
                            }
                            else if( cLayer.layerType == GPE_LAYER_TYPE_BACKGROUND && cLayer.layerBackgrounds.length > 0 )
                            {
                                for( iTemp = 0; iTemp < cLayer.layerBackgrounds.length ; iTemp++)
                                {
                                    cLayer.layerBackgrounds[iTemp].render_bg(renderToArea, camInUse.cameraRect, camInUse.cameraRect,sceneWidth, sceneHeight);
                                }
                            }
                            cLayer.render_objects();

                        }
                    }

                    //
                    //Render HUD/GUI elements
                    GPR.HUD_RECT.copy_rect( camInUse.cameraRect );
                    gpe.CURRENT_VIEW_BOX = GPR.HUD_RECT;
                    gpe.CURRENT_VIEW_BOX.xPos = 0;
                    gpe.CURRENT_VIEW_BOX.yPos = 0;
                    for (hTemp=0; hTemp<sceneLayers.length; hTemp++)
                    {
                        cLayer = sceneLayers.at(hTemp);
                        if(cLayer!=-1 )
                        {
                            cLayer.render_hud_objects();
                        }
                    }

                    if( typeof mainRenderToArea!="undefined")
                    {
                        mainRenderToArea.drawImage(gpe.EXTRA_CANVASES[gTemp],
                        camInUse.renderRect.get_x(),camInUse.renderRect.get_y(), camInUse.renderRect.get_width(), camInUse.renderRect.get_height() );

                    }
                }
            }
        }

        GPE->currentCameraInView = 0;
        splitRenderMode+=1;
        renderToArea = mainRenderToArea;
        renderToArea.font = 'bold 16pt Calibri';
        renderToArea.textAlign = 'right';
        renderToArea.fillStyle = '#000';
        if( GPE_SETTINGS_IS_DEBUGGING )
        {
            for (gTemp = 0; gTemp < sceneCamera.length; gTemp++)
            {
                GPE->currentCameraInView = gTemp;
                camInUse = sceneCamera[ gTemp];
                if( camInUse.isVisible )
                {
                    //renderToArea.fillText("["+gTemp+"] "+camInUse.cameraRect.get_x()+","+camInUse.cameraRect.get_y()+","+ camInUse.cameraRect.get_width()+","+ camInUse.cameraRect.get_height(), gpe.SCREEN_WIDTH-32,48*(2+gTemp ) );

                    var iii = 0;
                    var tempColSpace = 0;
                    var tempColSpacee = 0;
                    for( iii = state_controller.COLLISION_AREA_SPACES.length-1;iii >=-0; iii--)
                    {
                        tempColSpacee = state_controller.COLLISION_AREA_SPACES[iii];
                        tempColSpace = tempColSpacee.boxArea;
                        mainRenderToArea.beginPath();
                        mainRenderToArea.rect(tempColSpace.get_x()-camInUse.cameraRect.get_x(), tempColSpace.get_y()-camInUse.cameraRect.get_y(), tempColSpace.get_width(), tempColSpace.get_height() );
                        mainRenderToArea.lineWidth = 1;
                        if(GPR.collisionSpacesInView.indexOf( iii) >= 0 )
                        {
                            mainRenderToArea.strokeStyle = 'red';
                        }
                        else
                        {
                            mainRenderToArea.strokeStyle = 'blue';
                        }
                        mainRenderToArea.stroke();
                        mainRenderToArea.font = 'bold 16pt Calibri';
                        mainRenderToArea.textAlign = 'center';
                        mainRenderToArea.fillStyle = 'blue';
                        mainRenderToArea.fillText( iii, tempColSpace.get_x()-camInUse.cameraRect.get_x()+32, tempColSpace.get_y()-camInUse.cameraRect.get_y() +32);
                    }
                }
            }


            renderToArea.font = 'bold 16pt Calibri';
            renderToArea.textAlign = 'left';
            renderToArea.fillStyle = 'maroon';
            renderToArea.fillText('Scene Info || Version: '+sceneVersion+" | "+sceneWidth+" | "+sceneHeight, 32, 300);

            renderToArea.fillText('Collision Grids: '+state_controller.COLLISION_AREA_SPACES.length+' ('+state_controller.spatialGridWidthAmount+' X '+state_controller.spatialGridHeightAmount+') for '+state_controller.OBJECT_COUNT+' objects', 32, 364);
            renderToArea.textAlign = 'left';
            renderToArea.fillText('Scene Name: '+sceneName, 32, gpe.SCREEN2_HEIGHT-64);
        }
    }
    else
    {
        gpe.context.fillStyle = bgColor;
        gpe.context.fillRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
    }
    */
}

fontPair::fontPair(TTF_Font * fontIn,std::string strIn)
{
    lastAlphaRendered = 255;
    str = strIn;
    strTexture = NULL;
    textWidth = 0;
    textHeight = 0;
    if( fontIn!=NULL && (int)strIn.size() > 0)
    {
        //Render text surface
        SDL_Surface * textSurface = NULL;
        textSurface = TTF_RenderUTF8_Blended( fontIn, strIn.c_str(),c_white->get_sdl_color() );
        if( textSurface == NULL )
        {
            record_error( "Unable to render text surface! Error:  ",TTF_GetError() );
        }
        else
        {
            //Create texture from surface pixels
            strTexture = SDL_CreateTextureFromSurface( MAIN_RENDERER->get_renderer(), textSurface );
            if( strTexture != NULL )
            {
                //Get image dimensions
                textWidth = textSurface->w;
                textHeight = textSurface->h;
                SDL_QueryTexture(strTexture,NULL, NULL,&textWidth, &textHeight);
                SDL_FreeSurface(textSurface);
                //strTexture->set_blend_mode( blend_mode_add );
                SDL_SetTextureBlendMode(strTexture, SDL_BLENDMODE_ADD );
            }
            else
            {
                record_error( "Unable to create texture from rendered text! SDL Error: ", SDL_GetError() );
            }
        }
    }
}

fontPair::~fontPair()
{
    if( strTexture!=NULL)
    {
        SDL_DestroyTexture(strTexture);
        strTexture = NULL;
    }
}

bool fontPair::texture_matches(std::string strCheck)
{
    if( str==strCheck)
    {
        return true;
    }
    return false;
}

SDL_Texture * fontPair::get_texture()
{
    return strTexture;
}

int fontPair::get_width()
{
    return textWidth;
}

int fontPair::get_height()
{
    return textHeight;
}

GPE_Font::GPE_Font(std::string fFileLocation, int fSize, bool isMonospaced, std::string fNickName )
{
    customFontId = 0;
    monoWidth = 0;
    monoHeight = 0;
    numberWidth = 0;
    numberHeight = 0;
    fontIsMonoSpaced = isMonospaced;
    heldSDLFont = TTF_OpenFontIndex(fFileLocation.c_str(), fSize,0);
    fontFamilyName = "";
    fontNickName = fNickName;
    int i = 0;
    if( heldSDLFont!=NULL)
    {
        fontFamilyName = TTF_FontFaceFamilyName(heldSDLFont);
        fontFamilyName+=" "+int_to_string(fSize)+"pt";
        std::string longCharString = "";
        for( i = 0; i < 256; i++)
        {
            longCharString+=i;
        }

        if( isMonospaced)
        {
            TTF_SetFontHinting(heldSDLFont,TTF_HINTING_MONO);
            fontPair * monoSpacedPair = find_texture("A");
            if( monoSpacedPair!=NULL)
            {
                numberWidth = monoWidth = monoSpacedPair->get_width();
                numberHeight = monoHeight = monoSpacedPair->get_height();
            }
        }
        else
        {
            TTF_SetFontHinting(heldSDLFont,TTF_HINTING_LIGHT );
        }

        for( i = 0; i < 10; i++)
        {
            characterPairs.push_back( new fontPair(heldSDLFont,int_to_string(i) ) );
        }
        characterPairs.push_back( new fontPair(heldSDLFont,"." ) );
        characterPairs.push_back(  new fontPair(heldSDLFont,"," ) );
        characterPairs.push_back(  new fontPair(heldSDLFont,"-" ) );
        characterPairs.push_back(  new fontPair(heldSDLFont,"(" ) );
        characterPairs.push_back( new fontPair(heldSDLFont,")" ) );
        if( isMonospaced == false)
        {
            fontPair * tempFontPair = NULL;
            for( i = (int)characterPairs.size()-1; i>=0; i--)
            {
                tempFontPair = characterPairs[i];
                if( tempFontPair!=NULL)
                {
                    if( tempFontPair->get_width() > numberWidth)
                    {
                        numberWidth = tempFontPair->get_width();
                    }
                    if( tempFontPair->get_height() > numberHeight)
                    {
                        numberHeight = tempFontPair->get_height();
                    }
                }
            }
        }
    }
}

GPE_Font::~GPE_Font()
{
    if( heldSDLFont!=NULL)
    {
        TTF_CloseFont(heldSDLFont);
        heldSDLFont = NULL;
    }
    fontPair * tempFontPair = NULL;
    int i = 0;
    for( i = (int)textPairs.size()-1; i>=0 ; i--)
    {
        tempFontPair = textPairs[i];
        if( tempFontPair!=NULL)
        {
            delete tempFontPair;
            tempFontPair = NULL;
        }
    }
    for( i = (int)characterPairs.size()-1; i>=0 ; i--)
    {
        tempFontPair = characterPairs[i];
        if( tempFontPair!=NULL)
        {
            delete tempFontPair;
            tempFontPair = NULL;
        }
        characterPairs[i] = NULL;
    }
}

void GPE_Font::clear_cache()
{
    fontPair * tempFontPair = NULL;
    for(int i = (int)textPairs.size()-1; i>=0 ; i--)
    {
        tempFontPair = textPairs[i];
        if( tempFontPair!=NULL)
        {
            delete tempFontPair;
            tempFontPair = NULL;
        }
    }
    textPairs.clear();
}

int GPE_Font::get_cache_count()
{
    return (int)textPairs.size();
}

void GPE_Font::get_metrics(std::string textToRender, int * wVal, int * hVal)
{
    int tSize = (int)textToRender.size();
    if( tSize > 0)
    {
        if( fontIsMonoSpaced)
        {
            *wVal = monoWidth*tSize;
            *hVal = monoHeight;
        }
        else if(heldSDLFont!=NULL)
        {
            fontPair *  newFoundPair = find_texture( textToRender);
            if( newFoundPair!=NULL)
            {
                *wVal = newFoundPair->get_width();
                *hVal = newFoundPair->get_height();
            }

        }
    }
    else
    {
        *wVal = 0;
        *hVal = 0;
    }
}

void GPE_Font::get_numbered_metrics(std::string textToRender, int * wVal, int * hVal)
{
    int tSize = (int)textToRender.size();
    if( tSize > 0)
    {
        *wVal = numberWidth*tSize;
        *hVal = numberHeight;
    }
    else
    {
        *wVal = 0;
        *hVal = 0;
    }
}

int GPE_Font::get_mono_height()
{
    return monoHeight;
}

int GPE_Font::get_mono_width()
{
    return monoWidth;
}

std::string GPE_Font::get_family_name()
{
    return fontFamilyName;
}

TTF_Font * GPE_Font::get_sdl_font()
{
    return heldSDLFont;
}

fontPair * GPE_Font::find_character_texture( std::string numbId)
{
    fontPair * fPair = NULL;
    SDL_Texture * fSDLTexture = NULL;
    for( int i = (int)characterPairs.size()-1; i>=0 ; i--)
    {
        fPair = characterPairs[i];
        if( fPair!=NULL)
        {
            if( fPair->texture_matches(numbId) )
            {
                return fPair;
            }
        }
    }
    if( numbId!=" ")
    {
        fPair = new fontPair(heldSDLFont,numbId);
        characterPairs.push_back(fPair);
        return fPair;
    }
    return NULL;
}

fontPair * GPE_Font::find_texture( std::string textToRender)
{
    fontPair * fPair = NULL;
    SDL_Texture * fSDLTexture = NULL;
    for( int i = (int)textPairs.size()-1;  i >=0; i--)
    {
        fPair = textPairs[i];
        if( fPair!=NULL)
        {
            if( fPair->texture_matches(textToRender) )
            {
                return fPair;
            }
        }
        else
        {
            textPairs.erase( textPairs.begin()+i);
        }
    }
    fPair = new fontPair(heldSDLFont,textToRender);
    textPairs.push_back(fPair);
    return fPair;
}

void GPE_Font::render_bitmapped_text( int xPos, int yPos, std::string numberToRender, GPE_Color * textColor, int hAlign,int vAlign, int renderAlpha )
{
    int numberSize = (int)numberToRender.size();
    if( renderAlpha > 0 && numberSize >0 )
    {
        switch(hAlign)
        {
            case FA_CENTER:
                xPos=xPos-(numberWidth*numberSize)/2;
                //LAST_USED_HALIGNMENT=FA_CENTER;
                break;

            case FA_RIGHT:
                xPos=xPos-(numberWidth*numberSize);
                // LAST_USED_HALIGNMENT=FA_RIGHT;
                break;

            //rendering left will be the default
            default:
                // LAST_USED_HALIGNMENT=FA_LEFT;
                break;
        }
        switch(vAlign)
        {
            case FA_MIDDLE:
                yPos=yPos-numberHeight/2;
                // LAST_USED_VALIGNMENT=FA_MIDDLE;
                break;

            case FA_BOTTOM:
                yPos=yPos-numberHeight;
                // LAST_USED_VALIGNMENT=FA_MIDDLE;
                break;

            //rendering left will be the default
            default:
                //LAST_USED_VALIGNMENT=FA_TOP;
                break;
        }
        fontPair * strTex = NULL;
        SDL_Texture * fPairTex = NULL;
        int i = 0;
        std::string currCharacter = "";
        for( i = 0; i < numberSize; i++)
        {
            currCharacter = get_substring(numberToRender,i,1);
            strTex = find_character_texture( currCharacter );
            if( strTex!=NULL)
            {
                fPairTex = strTex->get_texture();
                if( renderAlpha > 255)
                {
                    renderAlpha = 255;
                }
                if( fPairTex!=NULL && renderAlpha >0)
                {
                    int texWid = 0;
                    int texHeight =0;
                    SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                    if( strTex->lastAlphaRendered!=renderAlpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                        strTex->lastAlphaRendered = renderAlpha;
                    }
                    //Get image dimensions
                    texWid = strTex->get_width();
                    texHeight = strTex->get_height();

                    if(hAlign<FA_LEFT || hAlign<0)
                    {
                        hAlign=LAST_USED_HALIGNMENT;
                    }
                    if(vAlign <FA_TOP || vAlign<0)
                    {
                        vAlign=LAST_USED_VALIGNMENT;
                    }
                    SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                    SDL_RenderCopy( CURRENT_RENDERER->gpeRender,fPairTex, NULL, &clip);
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
            xPos+=numberWidth;
        }
    }
}

void GPE_Font::render_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign,int vAlign, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        SDL_Texture * fPairTex=  NULL;
        if( strTex!=NULL)
        {
            fPairTex = strTex->get_texture();

            if( fPairTex!=NULL && renderAlpha >0)
            {
                if( renderAlpha > 255)
                {
                    renderAlpha = 255;
                }
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                if(hAlign<FA_LEFT || hAlign<0)
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }
                if(vAlign <FA_TOP || vAlign<0)
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                    case FA_CENTER:
                        xPos=xPos-texWid/2;
                        //LAST_USED_HALIGNMENT=FA_CENTER;
                        break;

                    case FA_RIGHT:
                        xPos=xPos-texWid;
                        // LAST_USED_HALIGNMENT=FA_RIGHT;
                        break;

                    //rendering left will be the default
                    default:
                        // LAST_USED_HALIGNMENT=FA_LEFT;
                        break;
                }
                switch(vAlign)
                {
                    case FA_MIDDLE:
                        yPos=yPos-texHeight/2;
                        // LAST_USED_VALIGNMENT=FA_MIDDLE;
                        break;

                    case FA_BOTTOM:
                        yPos=yPos-texHeight;
                        // LAST_USED_VALIGNMENT=FA_MIDDLE;
                        break;

                    //rendering left will be the default
                    default:
                        //LAST_USED_VALIGNMENT=FA_TOP;
                        break;
                }
                SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                SDL_RenderCopy( CURRENT_RENDERER->gpeRender,fPairTex, NULL, &clip);
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );

            }
        }
    }
}

void GPE_Font::render_text_ext( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign,int vAlign, int rendWid, int rendHeight, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        if( strTex!=NULL)
        {
            SDL_Texture * fPairTex = strTex->get_texture();
            if( fPairTex!=NULL)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );
                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                //Get image dimensions
                if( rendWid<=0 || rendWid > strTex->get_width())
                {
                    texWid = strTex->get_width();
                }
                else
                {
                    texWid = rendWid;
                }

                if( rendHeight<=0 || rendHeight > strTex->get_height() )
                {
                    texHeight = strTex->get_height();
                }
                else
                {
                    texHeight = rendHeight;
                }

                if(hAlign<FA_LEFT || hAlign<0)
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }
                if(vAlign <FA_TOP || vAlign<0)
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                case FA_CENTER:
                    xPos=xPos-texWid/2;
                    //LAST_USED_HALIGNMENT=FA_CENTER;
                    break;

                case FA_RIGHT:
                    xPos=xPos-texWid;
                    // LAST_USED_HALIGNMENT=FA_RIGHT;
                    break;

                //rendering left will be the default
                default:
                    // LAST_USED_HALIGNMENT=FA_LEFT;
                    break;
                }
                switch(vAlign)
                {
                case FA_MIDDLE:
                    yPos=yPos-texHeight/2;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                case FA_BOTTOM:
                    yPos=yPos-texHeight;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                //rendering left will be the default
                default:
                    //LAST_USED_VALIGNMENT=FA_TOP;
                    break;
                }

                SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                SDL_Rect renderRect = {0, 0, texWid, texHeight};
                SDL_RenderCopy( CURRENT_RENDERER->gpeRender,fPairTex, &renderRect, &clip);
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
            }
        }
    }
}

void GPE_Font::render_text_boxed( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor,int hAlign,int vAlign, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        SDL_Texture * fPairTex=  NULL;
        if( strTex!=NULL)
        {
            fPairTex = strTex->get_texture();
            if( renderAlpha > 255)
            {
                renderAlpha = 255;
            }
            if( fPairTex!=NULL && renderAlpha >0)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                if(hAlign<FA_LEFT || hAlign<0)
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }
                if(vAlign <FA_TOP || vAlign<0)
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                    case FA_CENTER:
                        xPos=xPos-texWid/2;
                        //LAST_USED_HALIGNMENT=FA_CENTER;
                        break;

                    case FA_RIGHT:
                        xPos=xPos-texWid;
                        // LAST_USED_HALIGNMENT=FA_RIGHT;
                        break;

                    //rendering left will be the default
                    default:
                        // LAST_USED_HALIGNMENT=FA_LEFT;
                        break;
                }
                switch(vAlign)
                {
                case FA_MIDDLE:
                    yPos=yPos-texHeight/2;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                case FA_BOTTOM:
                    yPos=yPos-texHeight;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                //rendering left will be the default
                default:
                    //LAST_USED_VALIGNMENT=FA_TOP;
                    break;
                }
                GPE_Rect gpeClip;
                gpeClip.x = xPos;
                gpeClip.y = yPos;
                gpeClip.w = texWid;
                gpeClip.h = texHeight;
                SDL_Rect sdlClip = {xPos, yPos, texWid,texHeight};
                gpe->render_rect(  &gpeClip,boxColor,false,renderAlpha );
                SDL_RenderCopy( CURRENT_RENDERER->gpeRender,fPairTex, NULL, &sdlClip);
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
            }
        }
    }
}

void GPE_Font::render_text_rotated( int xPos, int yPos, std::string textureText, GPE_Color * textColor, double textAngle, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        SDL_Texture * fPairTex=  NULL;
        if( strTex!=NULL)
        {
            fPairTex = strTex->get_texture();
            if( renderAlpha > 255)
            {
                renderAlpha = 255;
            }
            if( fPairTex!=NULL && renderAlpha >0)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                SDL_Rect sdlDstrect = {xPos-texWid, yPos-texHeight, texWid,texHeight};
                SDL_Point sdlCenterPoint = {texWid, texHeight};
                SDL_RenderCopyEx( CURRENT_RENDERER->gpeRender,fPairTex, NULL,&sdlDstrect,textAngle,&sdlCenterPoint, SDL_FLIP_NONE );
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
            }
        }
    }
}


bool render_bitmap_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( textFont!=NULL)
    {
        textFont->render_bitmapped_text( xPos,yPos,mainTranslator->get_str(textureText ),textColor,hAlign,vAlign,renderAlpha);
        return true;
    }
    else
    {
        std::cout << "Unable to render number ("+ textureText + ").\n";
    }
    return false;
}

bool render_new_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( (int)textureText.size() > 0 && textFont!=NULL )
    {
            textFont->render_text( xPos,yPos,mainTranslator->get_str(textureText ),textColor,hAlign,vAlign,renderAlpha);
            return true;
    }
    else if( textFont!=NULL )
    {
       // std::cout << "Unable to render text (" << textureText << ").\n";
    }
    return false;
}

bool render_only_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( (int)textureText.size() > 0)
    {
        if( textFont!=NULL)
        {
            textFont->render_text( xPos,yPos,textureText,textColor,hAlign,vAlign,renderAlpha);
            return true;
        }
    }
    return false;
}

bool render_new_text_ext( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int rendWid, int rendHeight, int renderAlpha )
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( textFont!=NULL)
    {
        textFont->render_text_ext( xPos,yPos,mainTranslator->get_str(textureText ),textColor,hAlign,vAlign,rendWid,rendHeight,renderAlpha);
        return true;
    }
    else if( textFont!=NULL )
    {
        std::cout << "Unable to render EXT text (" << textureText << ").\n";
    }
    return false;
}

bool render_new_boxed( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor, GPE_Font * textFont, int hAlign,int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( textFont!=NULL)
    {
        textFont->render_text_boxed( xPos,yPos,mainTranslator->get_str(textureText ),textColor,boxColor,hAlign,vAlign,renderAlpha);
        return true;
    }
    return false;
}

bool render_new_text_rotated( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, double textAngle, int renderAlpha )
{
    if( textFont==NULL)
    {
        textFont = LAST_USED_FONT;
    }
    if( textFont!=NULL)
    {
        textFont->render_text_rotated( xPos,yPos,mainTranslator->get_str(textureText ),textColor,textAngle,renderAlpha);
        return true;
    }
    return false;
}

void limit_space_to_rect(GPE_Rect * rectIn,int * limitedX, int * limitedY)
{
    if( rectIn!=NULL && limitedX!=NULL && limitedY!=NULL)
    {
        if( *limitedX < rectIn->x)
        {
            *limitedX = 0;
        }

        if( *limitedX < rectIn->x+rectIn->w)
        {
            *limitedX = 0;
        }

        if( *limitedY < rectIn->y)
        {
            *limitedY = 0;
        }

        if( *limitedY < rectIn->y+rectIn->h)
        {
            *limitedY = 0;
        }
    }
}

GPE_GamePath::GPE_GamePath(int pId, bool pIsClosed)
{
    pathId = pId;
    isClosedPath = pIsClosed;
}

GPE_GamePath::~GPE_GamePath()
{

}


GPE_Runtime::GPE_Runtime()
{
    font_total_count = 0;
    for( int imCam = 0; imCam < GPE_MAX_CAMERAS; imCam++)
    {
        MOUSE_CAM_X[imCam] = 0;
        MOUSE_CAM_Y[imCam] = 0;
        MOUSE_IN_CAM[imCam] = true;
    }
}

GPE_Runtime::~GPE_Runtime()
{

}

/*!
\brief Draw filled circle with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the filled circle.
\param y Y coordinate of the center of the filled circle.
\param rad Radius in pixels of the filled circle.
\param r The red value of the filled circle to draw.
\param g The green value of the filled circle to draw.
\param b The blue value of the filled circle to draw.
\param a The alpha value of the filled circle to draw.

\returns Returns 0 on success, -1 on failure.
*/

GPE_Font * GPE_Runtime::open_font(std::string fontLocation, int fontSize, bool isMonospaced, std::string fNickName )
{
    GPE_Font *  returnVal;
    returnVal = new GPE_Font(fontLocation.c_str(),fontSize, isMonospaced, fNickName );
    if( returnVal==NULL)
    {
        record_error("[FONT ERROR] Unable to load font located at "+fontLocation);
    }
    else
    {
        loadedGPEFonts.push_back(returnVal);
        returnVal->customFontId = font_total_count;
        font_total_count++;
    }
    return returnVal;
}

void GPE_Runtime::close_font(GPE_Font * fontIn)
{
    if( fontIn!=NULL)
    {
        GPE_Font * cFont = NULL;
        for( int i = (int)loadedGPEFonts.size()-1; i >=0; i--)
        {
            cFont = loadedGPEFonts[i];
            if( cFont!=NULL)
            {
                if( cFont->customFontId==fontIn->customFontId)
                {
                    loadedGPEFonts.erase( loadedGPEFonts.begin()+i);
                }
            }
        }
        delete fontIn;
        fontIn = NULL;
    }
}

void GPE_Runtime::clear_font_cache()
{
    record_error("Clearing Font Cache...");
    GPE_Font * cFont = NULL;
    for( int i = (int)loadedGPEFonts.size()-1; i >=0; i--)
    {
        cFont = loadedGPEFonts[i];
        if( cFont!=NULL)
        {
            cFont->clear_cache();
        }
    }
    record_error("Font Cache total: ["+int_to_string(get_font_cache_size() )+"]...");
}

int GPE_Runtime::get_font_cache_size(bool recordCache)
{
    GPE_Font * cFont = NULL;
    int cacheCount = 0;
    int i = 0;
    int foundCache = 0;
    if( recordCache)
    {
        record_error("\nCalculating Font Cache...");
        for( i = (int)loadedGPEFonts.size()-1; i >=0; i--)
        {
            cFont = loadedGPEFonts[i];
            if( cFont!=NULL)
            {
                foundCache =cFont->get_cache_count();
                cacheCount+=foundCache;
                record_error("      Font["+cFont->fontNickName+"] Cache total: ["+int_to_string(foundCache )+"]...");
            }
        }
        record_error("Font Cache total: ["+int_to_string(cacheCount )+"]...");
    }
    else
    {
        for( i = (int)loadedGPEFonts.size()-1; i >=0; i--)
        {
            cFont = loadedGPEFonts[i];
            if( cFont!=NULL)
            {
                cacheCount+=cFont->get_cache_count();
            }
        }
    }

    return cacheCount;
}

//GPE Render calls [ Begin ]
void GPE_Runtime::render_fps(GPE_Font * fontToUse)
{
    if( SHOW_FPS_COUNTER )
    {
        render_bitmap_text( SCREEN_WIDTH-GENERAL_GPE_PADDING,GENERAL_GPE_PADDING,int_to_string(CURRENT_FPS)+" / "+int_to_string(FPS_CAP)+" fps ",c_lime,fontToUse,FA_RIGHT,FA_TOP );
    }
}

bool GPE_Runtime::render_circle_filled_rgba( Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    //Modified from SDL_GFX
    Sint16 cx = 0;
    Sint16 cy = rad;
    Sint16 ocx = (Sint16) 0xffff;
    Sint16 ocy = (Sint16) 0xffff;
    Sint16 df = 1 - rad;
    Sint16 d_e = 3;
    Sint16 d_se = -2 * rad + 5;
    Sint16 xpcx, xmcx, xpcy, xmcy;
    Sint16 ypcy, ymcy, ypcx, ymcx;

    /*
    * Sanity check radius
    */
    if (rad <= 0 || CURRENT_RENDERER==NULL)
    {
        return false;
    }

    /*
    * Special case for rad=0 - draw a point
    */


    SDL_Renderer * renderer = CURRENT_RENDERER->gpeRender;
    if( renderer!=NULL)
    {
        //result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
        /*
        * Set color
        */
        SDL_SetRenderDrawColor(renderer,r,g,b,a );

        /*
        * Draw
        */
        do
        {
            xpcx = x + cx;
            xmcx = x - cx;
            xpcy = x + cy;
            xmcy = x - cy;
            if (ocy != cy)
            {
                if (cy > 0)
                {
                    ypcy = y + cy;
                    ymcy = y - cy;
                    gpe->render_horizontal_line(ypcy,xmcx, xpcx);
                    gpe->render_horizontal_line( ymcy,xmcx, xpcx);
                }
                else
                {

                }
                ocy = cy;
            }
            if (ocx != cx)
            {
                if (cx != cy)
                {
                    if (cx > 0)
                    {
                        ypcx = y + cx;
                        ymcx = y - cx;
                        gpe->render_horizontal_line( ymcx,xmcy, xpcy);
                        gpe->render_horizontal_line( ypcx,xmcy, xpcy);
                    }
                    else
                    {
                        gpe->render_horizontal_line( y,xmcy, xpcy);
                    }
                }
                ocx = cx;
            }

            /*
            * Update
            */
            if (df < 0)
            {
                df += d_e;
                d_e += 2;
                d_se += 2;
            }
            else
            {
                df += d_se;
                d_e += 2;
                d_se += 4;
                cy--;
            }
            cx++;
        }
        while (cx <= cy);
    }
    return true;
}

void GPE_Runtime::render_gradient_vertical(GPE_Rect * rendRect, GPE_Color * rendColor)
{
    if( CURRENT_RENDERER==NULL)
    {
        CURRENT_RENDERER = MAIN_RENDERER;
    }
    if( rendRect!=NULL && rendColor!=NULL && CURRENT_RENDERER!=NULL)
    {
        float colorShadeDivision = 0;
        if( rendRect->w > 0 && rendRect->h > 0)
        {
            GPE_Color * colorShadeTempColor = new GPE_Color();

            for( int i = 0; i <= rendRect->h; i++)
            {
                colorShadeDivision = (float)i/(float) rendRect->h;
                colorShadeTempColor->change_r( merge_channel(rendColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_g( merge_channel(rendColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_b( merge_channel(rendColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                gpe->render_horizontal_line_color(rendRect->y+i,rendRect->x,rendRect->x+rendRect->w,colorShadeTempColor);
            }
            delete colorShadeTempColor;
            colorShadeTempColor = NULL;
        }
    }
}

void GPE_Runtime::render_gradient_horizontal( GPE_Rect * rendRect, GPE_Color * rendColor)
{
    if( CURRENT_RENDERER==NULL)
    {
        CURRENT_RENDERER = MAIN_RENDERER;
    }
    if( rendRect!=NULL && rendColor!=NULL && CURRENT_RENDERER!=NULL)
    {
        float colorShadeDivision = 0;
        if( rendRect->w > 0 && rendRect->h > 0)
        {
            GPE_Color * colorShadeTempColor = new GPE_Color();

            for( int i = 0; i <= rendRect->w; i++)
            {
                colorShadeDivision = (float)i/(float) rendRect->w;
                colorShadeTempColor->change_r( merge_channel(rendColor->get_r(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_g( merge_channel(rendColor->get_g(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );
                colorShadeTempColor->change_b( merge_channel(rendColor->get_b(),(float)( -colorShadeDivision+1.f)*255.f,(float) fabs( colorShadeDivision-0.5f)+0.5 ) );

                gpe->render_vertical_line_color(rendRect->x+i,rendRect->y,rendRect->y+rendRect->h,colorShadeTempColor);
            }
            delete colorShadeTempColor;
            colorShadeTempColor = NULL;
        }
    }
}

void GPE_Runtime::render_rect( GPE_Rect * rendRect, GPE_Color * rendColor,bool outline, int alphaChannel)
{
    if( rendRect!=NULL)
    {
        SDL_Rect sdlRendRect = {rendRect->x, rendRect->y, rendRect->w, rendRect->h };
        SDL_SetRenderDrawColor( CURRENT_RENDERER->get_renderer(), rendColor->get_r(), rendColor->get_g(),rendColor->get_b(), alphaChannel );
        if( outline)
        {
            SDL_RenderDrawRect(CURRENT_RENDERER->get_renderer(), &sdlRendRect);
        }
        else
        {
            SDL_RenderFillRect( CURRENT_RENDERER->get_renderer(), &sdlRendRect );
        }
    }
}

void GPE_Runtime::render_rectangle( int x1, int y1, int x2, int y2,  GPE_Color * rendColor,bool outline, int alphaChannel )
{
    SDL_Rect rendRect = {0,0,0,0};
    rendRect.x = std::min(x1, x2);
    rendRect.y = std::min(y1, y2);
    rendRect.w = std::abs(x1- x2);
    rendRect.h = std::abs(y1- y2);
    SDL_SetRenderDrawColor( CURRENT_RENDERER->get_renderer(), rendColor->get_r(), rendColor->get_g(),rendColor->get_b(), alphaChannel );
    if( outline)
    {
        SDL_RenderDrawRect(CURRENT_RENDERER->get_renderer(),&rendRect);
    }
    else
    {
        SDL_RenderFillRect( CURRENT_RENDERER->get_renderer(), &rendRect );
    }
}

void GPE_Runtime::render_line( int x1, int y1, int x2, int y2,  GPE_Color * rendColor, int alphaChannel)
{
    SDL_SetRenderDrawColor(CURRENT_RENDERER->get_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(CURRENT_RENDERER->get_renderer(), x1, y1, x2, y2);
}

void GPE_Runtime::render_line_width( int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel )
{
    SDL_SetRenderDrawColor(CURRENT_RENDERER->get_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
}

void GPE_Runtime::render_horizontal_line(int y, int x1, int x2)
{
    SDL_RenderDrawLine(CURRENT_RENDERER->get_renderer(), x1, y, x2, y);
}

void GPE_Runtime::render_horizontal_line_color( int y, int x1, int x2,  GPE_Color * rendColor, int alphaChannel)
{
    SDL_SetRenderDrawColor(CURRENT_RENDERER->get_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(CURRENT_RENDERER->get_renderer(), x1, y, x2, y);
}

void GPE_Runtime::render_vertical_line( int x, int y1, int y2)
{
    SDL_RenderDrawLine(CURRENT_RENDERER->get_renderer(), x, y1, x, y2);
}

void GPE_Runtime::render_vertical_line_color( int x, int y1, int y2,  GPE_Color * rendColor, int alphaChannel)
{
    SDL_SetRenderDrawColor(CURRENT_RENDERER->get_renderer(),rendColor->get_r(),rendColor->get_g(),rendColor->get_b(),alphaChannel );
    SDL_RenderDrawLine(CURRENT_RENDERER->get_renderer(), x, y1, x, y2);
}

void GPE_Runtime::set_render_alpha( int newAlpha )
{

}

//GPE Render calls [ Begin ]

bool GPE_Init()
{
    //Initialize all SDL subsystems
    record_error("Attempting to initialize SDL");
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        record_error("Error initializing SDL");
        return false;
    }
    record_error("Setting starting cursor...");
    SDL_SetCursor(SDL_CreateSystemCursor( SDL_SYSTEM_CURSOR_WAIT) );
    GPE_IS_LOADING = true;
    GPE_DEFAULT_CAMERA.x=0;
    GPE_DEFAULT_CAMERA.y=0;
    GPE_DEFAULT_CAMERA.w=(Uint16)SCREEN_WIDTH;
    GPE_DEFAULT_CAMERA.h=(Uint16)(SCREEN_HEIGHT);
    record_error("Setting event system..");
    SDL_EventState(SDL_DROPFILE,SDL_ENABLE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    /*
    if(SDL_Init(SDL_INIT_AUDIO))
    {
        SDL_putenv("SDL_AUDIODRIVER=dummy");
        SDL_Init(SDL_INIT_AUDIO);
    }
    */
    record_error("Creating colors..");
    activate_colors();
    record_error("Setting up renderer.");
    MAIN_RENDERER = new GPE_Renderer(SCREEN_WIDTH,SCREEN_HEIGHT,true,false,true);
    CURRENT_RENDERER = MAIN_RENDERER;
    mainTranslator = new GPE_Translator("en");
    input = new GPE_InputManager();

    rsm = new ResourceController();

    SDL_ShowCursor(SDL_ENABLE);
    //Initialize SDL_ttf
    record_error("Starting Font systems...");
    if( TTF_Init() == -1 )
    {
         record_error("Error loading SDL_TTF");
         return false;
    }
    /*
    //Initialize OpenGL
    if( init_GL() == false )
    {
        return false;
    }*/

    //loads support for JPG and other image formats.
    int flags=IMG_INIT_JPG|IMG_INIT_PNG |IMG_INIT_TIF;
    if( (IMG_Init(flags)&flags) != flags)
    {
        record_error("IMG_Init: Failed to init required jpg and png support!\n");
        record_error("IMG_Init: ", IMG_GetError());
        record_error("Error initializing SDL_Mixer.");
        record_error("Error initializing SDL_Mixer.");
        return false;
        // handle error
    }
    // load support for the OGG and MOD sample/music formats
    flags=MIX_INIT_OGG|MIX_INIT_MP3;
    if ((Mix_Init(flags)&flags) != flags)
    {
        record_error("Error loading SDL_Mixer");
        record_error("Mix_Init: Failed to init required ogg and mod support!\n");
        record_error("Mix_Init: ", Mix_GetError());
        return false;
    // handle error
    }
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024  ) == -1 )
    {
        record_error("Error initializing SDL_Mixer.");
        AUDIO_WORKS = false;
    }

    GPE = gpe = new GPE_Runtime();
    GPE_FPS_Timer = new Timer();
    GPE_TIMER_CAP = new Timer();
    //Start counting frames per second
    GPE_FPS_Timer->start();
    GPE_TIMER_CAP->start();
    return true;
}


bool GPE_Quit()
{
    record_error("Deleting gpe....");
    if( gpe!=NULL)
    {
        delete gpe;
        gpe = NULL;
    }

    record_error("Deleting resource manager....");
    if( rsm!=NULL)
    {
        rsm->clean_up();
        delete rsm;
        rsm = NULL;
    }
    record_error("Deleting input object...");
    if( input!=NULL)
    {
        delete input;
        input = NULL;
    }

    record_error("Quitting SDL_TFF....");
    TTF_Quit();

    record_error("Quitting SDL_IMG....");
    IMG_Quit();

    record_error("Quitting SDL_Mixer....");
    Mix_Quit();
    return true;
}
