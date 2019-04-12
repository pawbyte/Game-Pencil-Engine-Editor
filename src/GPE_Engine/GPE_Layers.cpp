/*
GPE_Layers.cpp
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

#include "GPE_Layers.h"
#include "GPE_Engine.h"


GPE_Layer::GPE_Layer(  int newLayerId, double newLayerAlpha )
{
    spatialGridBoxSize = 128;
    spatialGridWidthSize = 128;
    spatialGridHeightSize = 128;
    spatialGridWidthAmount = 2;
    spatialGridHeightAmount = 2;
    layerId = newLayerId;
    layerAlpha = newLayerAlpha;

    layerName = "";
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
    for( int i = (int)COLLISION_AREA_SPACES.size()-1; i >=0; i-- )
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



void GPE_Layer::clear_dynamic_data()
{
    /*independentObjects = [];
    layerObjects = [];
    layerBackgrounds = [];*/
    clear_spaces();
    COLLISION_AREA_OUTSIDE_SCENE->empty_list();
}


int GPE_Layer::get_id()
{
    return layerId;
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
    gcanvas->set_render_alpha(255);

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
    gcanvas->set_render_alpha(255);

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
    gcanvas->set_render_alpha(255);
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
