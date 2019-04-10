/*
GPE_Scene.cpp
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

#include "GPE_Scene.h"


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
        sceneCamera[iV] = new GPE_SceneCamera(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,iV);
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


GPE_Layer * GPE_GameScene::find_layer ( int layerIdIn )
{
    GPE_Layer * foundLayer = NULL;
    if( layerIdIn < 32 && layerIdIn>=0 )
    {
        for( int ii = (int)sceneLayers.size() - 1; ii >=0; ii--)
        {
            foundLayer =sceneLayers[ii];
            if( foundLayer!= NULL)
            {
                if( foundLayer->get_id()==layerIdIn )
                {
                    return foundLayer;
                }
            }
        }
    }
    return NULL;
}


GPE_Layer * GPE_GameScene::get_layer ( int layerIdIn)
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
        return add_layer(  layerIdIn,1);

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

GPE_Layer * GPE_GameScene::add_layer(int newLayerId, double newLayerAlpha )
{
    if( newLayerId >=0 && newLayerId < 32 )
    {
        GPE_Layer * newLayer = new GPE_Layer( newLayerId, newLayerAlpha);

        sceneLayers.push_back(newLayer);
        return newLayer;
    }
    return NULL;
}

GPE_Layer * GPE_GameScene::add_start_layer(   int newLayerId, double newLayerAlpha )
{
    GPE_Layer * newLayer = add_layer(   newLayerId, newLayerAlpha );
    if( newLayer!= NULL )
    {
        GPE_SceneLayer_Meta * newStartLayer = new GPE_SceneLayer_Meta();
        newStartLayer->lId = newLayerId;
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

void GPE_GameScene::change_background( int newBG, int bgNumber)
{

}

void GPE_GameScene::change_foreground( int newBG, int bgNumber)
{

}

int GPE_GameScene::snap_xto_grid(int xPos)
{
    if( xPos / tileWidth > tileAmountX)
    {
        return tileAmountX;
    }
    else
    {
        return std::max(0,(xPos/tileWidth)|0 );
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
	return false;
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
