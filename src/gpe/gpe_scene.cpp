/*
gpe_scene.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "gpe_scene.h"

namespace gpe
{
    game_scene::game_scene(int sId, std::string s_name, std::string sFile )
    {
        sceneFilename = sFile;
        name = s_name;
        sceneId = sId;
        sceneHasBeenActivated = false;
        sceneWidth = 1024;
        sceneHeight = 1024;
        spatialWidth = 256;
        spatialHeight = 256;
        sceneWasVisitedBefore = false;

        defaultLayerName = "game - layer";

        //background data
        bg_color = new color("bgcolor", c_dkgray->get_r(), c_dkgray->get_b(), c_dkgray->get_b() );
        sceneVersion = -1;
        bgMusic = -1;
        startAudio = -1;
        endAudio = -1;

        sceneIsProcessed = false;
        sceneResourcesAreProcessed = false;
        sceneIsSpatiallyPartitioned = false;
        load_scene( sceneFilename );
    }

    game_scene::~game_scene()
    {
        if( bg_color!=NULL )
        {
            delete bg_color;
            bg_color = NULL;
        }

        sceneObjects.clear();
        persistentObjects.clear();
        sub_elements.clear();
        scenesStartLayers.clear();

        if( main_spatial_grid != NULL )
        {
            main_spatial_grid->deactivate_all_layers();
        }
    }

    void game_scene::add_branch( branch * new_branch )
    {
        if( new_branch == NULL )
        {
            return;
        }

        int branch_layer_id = new_branch->get_layer_id();
        if( branch_layer_id < 0 || branch_layer_id >=32 )
        {
            return;
        }

        scene_layer * found_layer = find_layer( branch_layer_id );
        if( new_branch->get_type() == branch_type::LAYER )
        {
            if( found_layer == NULL )
            {
                sub_elements.push_back( new_branch );
            }
        }
        else
        {

        }
    }

    scene_layer * game_scene::add_layer(int newLayerId, float newLayerAlpha )
    {
        if( newLayerId >=0 && newLayerId < 32 )
        {
            if( main_spatial_grid != NULL )
            {
                main_spatial_grid->activate_layer( newLayerId );
            }
            if( find_layer( newLayerId )== NULL  )
            {
                scene_layer * new_layer = new scene_layer( newLayerId, newLayerAlpha);
                sub_elements.push_back( new_layer );
                return new_layer;
            }
        }
        return NULL;
    }

    scene_layer * game_scene::add_start_layer(   int newLayerId, float newLayerAlpha )
    {
        scene_layer * newLayer = add_layer(   newLayerId, newLayerAlpha );
        if( newLayer!= NULL )
        {
            scene_layer_meta_data * newStartLayer = new scene_layer_meta_data();
            newStartLayer->lId = newLayerId;
            newStartLayer->lAlpha = newLayerAlpha;
            scenesStartLayers.push_back( newStartLayer );
        }
        return newLayer;
    }

    void game_scene::apply_logic()
    {
        branch::update();
    }

    void game_scene::apply_postlogic()
    {

    }

    bool game_scene::add_to_persistent_objects( game_object * nObject)
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

    branch * game_scene::find_branch( const std::string branchName )
    {
        auto branchIterator = branchesByName.find( branchName );

        if( branchIterator != branchesByName.end() )
        {
            return branchIterator->second;
        }
        return nullptr;
    }

    branch * game_scene::find_tagged_branch( const std::string branchName )
    {
        std::vector<branch *> foundVector = find_all_tagged_branches( branchName );
        if( foundVector.size() > 0 )
        {
            return foundVector[ 0 ];
        }
        return NULL;
    }

    int game_scene::find_tagged_branch_count( const std::string branchName )
    {
        std::vector< branch * > foundVector = find_all_tagged_branches( branchName );
        return (int)foundVector.size();
    }

    branch * game_scene::find_tagged_branch_last( const std::string branchName )
    {
        auto foundVector = find_all_tagged_branches( branchName );
        if( foundVector.size() > 0 )
        {
            return foundVector[ foundVector.size() -1 ];
        }
        return NULL;
    }

    std::vector< branch * > game_scene::find_all_tagged_branches( const std::string branchName )
    {
        std::vector<branch *> foundVector;

        auto branchIterator = branchesByTags.find( branchName );

        if( branchIterator != branchesByTags.end() )
        {
            foundVector = branchIterator->second;
            return foundVector;
        }
    }

    void game_scene::init_collision_handler()
    {
        branch * current_branch = NULL;
        scene_layer * current_layer = NULL;
        for( int branch_itr = 0; branch_itr < (int)sub_elements.size(); branch_itr++ )
        {
            current_branch = sub_elements[branch_itr];

            current_layer = (scene_layer *)(current_branch);

            if( current_layer != nullptr )
            {
                current_layer->init_collision_handler( sceneWidth, sceneHeight );
            }
        }
    }

    scene_layer * game_scene::find_layer ( int layerIdIn )
    {
        scene_layer * foundLayer = NULL;
        branch * current_branch = NULL;
        if( layerIdIn < 32 && layerIdIn>=0 )
        {
            for( int branch_itr = 0; branch_itr < (int)sub_elements.size(); branch_itr++ )
            {
                current_branch = sub_elements[branch_itr];
                if( current_branch != NULL && current_branch->get_type() == branch_type::LAYER )
                {
                    foundLayer = (scene_layer *)(current_branch);
                    if( foundLayer!= nullptr )
                    {
                        if( foundLayer->get_id()==layerIdIn )
                        {
                            return foundLayer;
                        }
                    }
                }
            }
        }
        return NULL;
    }


    scene_layer * game_scene::get_layer ( int layerIdIn)
    {
        scene_layer * foundLayer = find_layer( layerIdIn );
        if( foundLayer !=NULL )
        {
            foundLayer;
        }

        if(  layerIdIn >=0 && layerIdIn < 32)
        {
            //console.log( "Unable to find layer with ID[" +layerIdIn+"]" );
            return add_layer(  layerIdIn,1);

        }
        return NULL;
    }

    int game_scene::get_scene_width()
    {
        return sceneWidth;
    }

    int game_scene::get_scene_height()
    {
        return sceneHeight;
    }

    void game_scene::reset_scene_meta()
    {
        sceneIsSpatiallyPartitioned = false;
        sceneHasBeenActivated = false;
    }

    bool game_scene::scene_is_ready()
    {
        return sceneHasBeenActivated;
    }

    void game_scene::scene_restart()
    {

    }

    void game_scene::scene_init()
    {

    }

    void game_scene::update_scene_specs( int lvlW, int lvlH)
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

    void game_scene::add_background( int bSurf, bool loopH, bool loopV, float bgX, float bgY, float bgXSpd, float bgYSpd)
    {

    }

    void game_scene::replace_background( int layerBeingChanged, int needleBgId, int newBgId )
    {

    }

    void game_scene::replace_all_backgrounds(  int needleBgId, int newBgId )
    {

    }

    void game_scene::set_background( int layerBeingChanged, int needleBgId, int newBgId )
    {

    }


    std::string game_scene::get_scene_name()
    {
        return name;
    }

    void game_scene::set_scene_name(std::string inSceneName)
    {
        name = inSceneName;
    }

    void game_scene::change_background( int newBG, int bgNumber)
    {

    }

    void game_scene::change_foreground( int newBG, int bgNumber)
    {

    }

    //depends on GPE_main_OBJECT_MAPPER
    bool game_scene::load_scene( std::string sceneFileName )
    {
        //
        if( ! sff_ex::file_exists( sceneFileName) )
        {
            return false;
        }

        std::ifstream gameResourceFileIn( sceneFileName.c_str() );
        float foundFileVersion = -1;

        //makes sure the file is open
        if (gameResourceFileIn.fail() || !gameResourceFileIn.is_open())
        {
            gameResourceFileIn.close();
            return false;
        }

        int equalPos = 0;
        std::string firstChar="";
        std::string section="";
        std::string keyString="";
        std::string valString="";
        std::string subValString="";
        std::string currLine="";
        std::string currLineToBeProcessed;
        std::string sprDataStr;

        int i = 0;
        while ( gameResourceFileIn.good() )
        {
            getline (gameResourceFileIn,currLine); //gets the next line of the file
            currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
            currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

            if( foundFileVersion <=0)
            {
                //Empty Line skipping is only allowed at the top of the file
                if(!currLineToBeProcessed.empty() )
                {
                    //Comment skipping is only allowed at the top of the file
                    if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                    {
                        //searches for an equal character and parses through the variable
                        equalPos=currLineToBeProcessed.find_first_of("=");
                        if(equalPos!=(int)std::string::npos)
                        {
                            //if the equalPos is present, then parse on through and carryon
                            keyString = currLineToBeProcessed.substr(0,equalPos);
                            valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                            if( keyString=="Version")
                            {
                                foundFileVersion = stg_ex::string_to_float(valString);
                            }
                        }
                    }
                }
            }
            else
            {
                //Begin processing the file.
                if(!currLineToBeProcessed.empty() )
                {
                    //Comment skipping is only allowed at the top of the file
                    if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                    {
                        equalPos=currLineToBeProcessed.find_first_of("=");
                        if(equalPos!=(int)std::string::npos)
                        {
                            //if the equalPos is present, then parse on through and carryon
                            keyString = currLineToBeProcessed.substr(0,equalPos);
                            keyString= stg_ex::string_lower( keyString );
                            valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                            if( keyString == "name" )
                            {
                                set_scene_name( valString );
                            }
                            else if( keyString == "width")
                            {
                                sceneWidth = stg_ex::string_to_int( valString, 0);
                            }
                            else if( keyString == "height")
                            {
                                sceneHeight = stg_ex::string_to_int( valString, 0);
                            }
                            else if( keyString == "space_width")
                            {
                                spatialWidth = stg_ex::string_to_int( valString, 0);
                            }
                            else if( keyString == "space_height")
                            {
                                spatialHeight = stg_ex::string_to_int( valString, 0);
                            }
                            else if( keyString == "gridwidth")
                            {
                                //sceneWidth = stg_ex::string_to_int( valString, 0);
                            }
                            else if( keyString == "gridheight")
                            {
                                //sceneHeight = stg_ex::string_to_int( valString, 0);
                            }
                            else if( keyString == "bgcolor")
                            {
                                //;
                            }
                            else if( keyString=="continuous" )
                            {

                            }
                            else if( keyString =="layer" )
                            {

                            }
                        }
                    }
                }
            }
        }

        if( foundFileVersion > 0 )
        {
            return  true;
        }
        return false;
    }

    void game_scene::load_and_parse_scene()
    {

    }

    bool game_scene::process_scene()
    {
        //Function not yet created
        return false;
    }

    void game_scene::render_background_color()
    {
        gpe::gcanvas->render_rectangle(0,0, screen_width, screen_height, bg_color,  false , 255 );
    }

    void game_scene::render_backgrounds()
    {

    }

    void game_scene::render_foregrounds()
    {

    }

    void game_scene::render_scene()
    {
        render_background_color();
        /*
        //gpe.GPE_Current_Context = renderToArea;
        if( sceneResourcesAreProcessed && sceneIsProcessed)
        {
            int tempTileId;
            layer_game_tile * cTile = NULL;
            int gTemp = 0;
            int hTemp=0;
            int iTemp=0;
            int jTemp=0;
            int kTemp = 0;
            int mainRenderToArea = 0;
            scene_camera * camInUse = NULL;
            scene_layer_ptr cLayer = NULL;

            int rendableCameraCount = 0;

            for (gTemp = 0; gTemp < max_cameras_allowed; gTemp++)
            {
                camInUse = sceneCamera[ gTemp];
                if( camInUse->is_visible() )
                {
                    rendableCameraCount+=1;
                }
            }

            GPE.currentCameraInView = 0;
            //

            int cSpacesIn = state_controller.collisionSpacesInView;
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
            for (gTemp = 0; gTemp < max_cameras_allowed; gTemp++ )
            {
                GPE.currentCameraInView = gTemp;
                camInUse = sceneCamera[ gTemp];
                if( camInUse.isVisible == true )
                {
                    renderToArea = gpe.GPE_Current_Context = gpe.EXTRA_CONTEXTS[gTemp];
                    renderToArea.fillStyle = bg_color;
                    gpe.set_render_alpha(255);
                    renderToArea.fillRect(0, 0, gpe.screen_width, gpe.screen_height);

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

            game_runtime->currentCameraInView = 0;
            splitRenderMode+=1;
            renderToArea = mainRenderToArea;
            renderToArea.font = 'bold 16pt Calibri';
            renderToArea.textAlign = 'right';
            renderToArea.fillStyle = '#000';
            if( GPE_SETTINGS_IS_DEBUGGING )
            {
                for (gTemp = 0; gTemp < sceneCamera.length; gTemp++)
                {
                    game_runtime->currentCameraInView = gTemp;
                    camInUse = sceneCamera[ gTemp];
                    if( camInUse.isVisible )
                    {
                        //renderToArea.fillText("["+gTemp+"] "+camInUse.cameraRect.get_x()+","+camInUse.cameraRect.get_y()+","+ camInUse.cameraRect.get_width()+","+ camInUse.cameraRect.get_height(), gpe.screen_width-32,48*(2+gTemp ) );

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

                renderToArea.fillText('Collision Grids: '+state_controller.COLLISION_AREA_SPACES.length+' ('+state_controller.spatialGridWidthAmount+' X '+state_controller.spatialGridHeightAmount+') for '+state_controller.object_count+' objects', 32, 364);
                renderToArea.textAlign = 'left';
                renderToArea.fillText('Scene Name: '+sceneName, 32, gpe.SCREEN2_HEIGHT-64);
            }
        }
        else
        {
            gpe.context.fillStyle = bg_color;
            gpe.context.fillRect(0, 0, gpe.screen_width, gpe.screen_height);
        }
        */
    }

    void game_scene::update_tilemap()
    {

    }
}
