/*
GPE_Layers.cpp
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

#include "GPE_Layers.h"
#include "GPE_Engine.h"


GPE_Layer::GPE_Layer(  int newLayerId, float newLayerAlpha )
{
    spatialPartitonController = NULL;
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

    layerObjects.clear();
    independentObjects.clear();
}

void GPE_Layer::init_spatialpartioning( int cSceneWidth, int cSceneHeight )
{
    if(spatialGridIsCreated && spatialMapWidthSize == cSceneWidth && spatialMapHeightSize == cSceneHeight)
    {
        return;
    }

    if( cSceneWidth>0 && cSceneHeight>0 )
    {
        if( spatialPartitonController!=NULL )
        {
            spatialPartitonController->init_spatialpartioning( cSceneWidth, cSceneHeight );
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
    GPE_SpatialPartition *  currentSpace = NULL;
    int foundCollisionSpaceId = 0;
    int jLoop = 0;
    //Adds camera independent objects of this layer into render list
    for( jLoop = (int)independentObjects.size() -1; jLoop >=0; jLoop--)
    {
        foundGameObject = independentObjects[jLoop];

        if( foundGameObject->is_init() && foundGameObject->is_visible() && !foundGameObject->gpeIsBeingDestroyed  )
        {
            //if(foundGameObject->hasPreRenderFunction)
            //    prerenderObjectRenderList.push_back(foundGameObject);

            //if( foundGameObject.hasRenderFunction)
            regularObjectRenderList.push_back(foundGameObject);
            foundGameObject->gpeAddedToRenderList = true;
        }
    }
}

void GPE_Layer::clear_spaces()
{
    spatialPartitonController->clear_spaces();
}



void GPE_Layer::clear_dynamic_data()
{
    /*independentObjects = [];
    layerObjects = [];
    layerBackgrounds = [];*/
    clear_spaces();
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

void GPE_Layer::render()
{

}

void GPE_Layer::render_objects()
{
    GPE_GameObject * foundGameObject = NULL;
    int iLoop = 0;

    gcanvas->set_render_alpha(255);

    //renders the game objects
    int listSize = (int)layerObjects.size();
    for( iLoop = 0; iLoop < listSize; iLoop++ )
    {
        foundGameObject = layerObjects[iLoop];
        //if(  foundGameObject.renderedAboveInFrame[gTemp]==false )
        {
            foundGameObject->render();
            //foundGameObject.renderedAboveInFrame[gTemp] = true;
        }
    }
    gcanvas->set_render_alpha(255);
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

void GPE_Layer::update()
{

}

void GPE_Layer::set_default_grid_size()
{
    spatialGridBoxSize = 256;
    spatialGridWidthSize = 256;
    spatialGridHeightSize = 256;
}
