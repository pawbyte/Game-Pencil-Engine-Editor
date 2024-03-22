/*
gpe_scene_tilemap_class.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_scene_tilemap_class.h"

GPE_SceneTile::GPE_SceneTile()
{
    isLocked = false;
    branch_type_id = gpe::branch_type::TILE;
    iconTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/table.png") ;

    tileTypeId = -1;
    tileIndexId = -1;
    tilesheetIndexId = -1;
    tileRect.x = 0;
    tileRect.y = 0;
    tileRect.w = 0;
    tileRect.h = 0;
}

GPE_SceneTile::~GPE_SceneTile()
{

}

void GPE_SceneTile::add_typed_elements()
{
    if( panel_inspector!=nullptr )
    {
    }
}

void GPE_SceneTile::render_branch()
{

}

bool GPE_SceneTile::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    return true;
}

GPE_SceneTileMap::GPE_SceneTileMap( std::string mapName, int x, int y, pawgui::widget_resource_container *pFolder  )
{
    projectParentFolder = pFolder;
    if( projectParentFolder!=nullptr)
    {
        tilesheetDropDown = new pawgui::widget_drop_down_resource_menu( "Tilesheet",projectParentFolder->find_resource_from_name( gpe::resource_type_names_plural[ gpe::resource_type_tilesheet]),-1,true );
        tilesheetDropDown->set_width(192);
        tSPreviewer = new tilesheetPreviewer();

    }
    else
    {
        tilesheetDropDown = nullptr;
        tSPreviewer = nullptr;
    }

    isLocked = false;
    set_position( x, y);
    branch_type_id = gpe::branch_type::TILEMAP;
    iconTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/th.png") ;

    update_rectangle(&tsPlacementArea,0,0,0,0);

    tileWidth = gpe::tile_default_width;
    tileHeight = gpe::tile_default_height;

    tileAmountX = prevTileAmountX = 0;
    tileAmountY = prevTileAmountY = 0;

    x_posField->set_number( x_pos );
    x_posField->set_label("Map-XStart");

    y_posField->set_number( y_pos );
    y_posField->set_label("Map-YStart");

    fieldTileWidth = new pawgui::widget_input_number( "32" );
    fieldTileWidth->set_number( tileWidth );
    fieldTileWidth->set_label("Tile Width");
    fieldTileWidth->scale_width( 0.4 );

    fieldTileHeight = new pawgui::widget_input_number( "32" );
    fieldTileHeight->set_number( tileHeight );
    fieldTileHeight->set_label("Tile Height");
    fieldTileHeight->scale_width( 0.4 );

    fieldAmountX = new pawgui::widget_input_number( "" );
    fieldAmountX->set_number( tileAmountX );
    fieldAmountX->set_label("Map Width");
    fieldAmountX->scale_width( 0.4 );

    fieldAmountY = new pawgui::widget_input_number( "y-tile-amount" );
    fieldAmountY->set_label("Map Height");
    fieldAmountY->set_number( tileAmountY );
    fieldAmountY->scale_width( 0.4 );

    fillScene = new pawgui::widget_checkbox("Fill Scene","Check to use remainder of scene" );
    if( (int)mapName.size()==0 )
    {
        name = "tilemap";
    }
    else
    {
        set_name( mapName );
    }
}

void GPE_SceneTileMap::add_typed_elements()
{
    if( panel_inspector!=nullptr )
    {
        panel_inspector->add_gui_element( tilesheetDropDown, true );
        panel_inspector->add_gui_element( fieldTileWidth, false );
        panel_inspector->add_gui_element( fieldTileHeight, true );
        panel_inspector->add_gui_element( fieldAmountX, false );
        panel_inspector->add_gui_element( fieldAmountY, true );
        panel_inspector->add_gui_element( fillScene, true );
    }
}

GPE_SceneTileMap::~GPE_SceneTileMap()
{
    if( tSPreviewer!=nullptr)
    {
        delete tSPreviewer;
        tSPreviewer = nullptr;
    }
}


void GPE_SceneTileMap::calculate_size()
{

}

void GPE_SceneTileMap::create_new_map(int new_xt, int new_yt,int ntileType )
{
    tileAmountX=new_xt;
    tileAmountY=new_yt;
    int newSize = tileAmountX*tileAmountY;
    if( mapTiles.size() >0 )
    {
        for(std::vector<GPE_SceneTile*>::const_iterator it = mapTiles.begin(); it != mapTiles.end(); it++)
        {
            delete *it;
        }
        mapTiles.clear();
    }
    GPE_SceneTile * newTile = nullptr;
    for(int i=0; i<newSize; i++)
    {
        newTile = new GPE_SceneTile();
        newTile->tileTypeId = ntileType;
        mapTiles.push_back(newTile);
    }

    x_posField->set_number( x_pos );
    y_posField->set_number( y_pos );

    fieldTileWidth->set_number( tileWidth );
    fieldTileHeight->set_number( tileHeight );

    fieldAmountX->set_number( tileAmountX );
    fieldAmountY->set_number( tileAmountY );

    gpe::error_log->report("Tile Map created.");
}

int GPE_SceneTileMap::get_map_size()
{
    return (int)mapTiles.size();
}

int GPE_SceneTileMap::get_xmax()
{
    return x_pos+tileWidth * tileAmountX;
}

int GPE_SceneTileMap::get_ymax()
{
    return y_pos+tileHeight * tileAmountY;
}

int GPE_SceneTileMap::get_sizex()
{
    return tileAmountX;
}

int GPE_SceneTileMap::get_sizey()
{
    return tileAmountY;
}

int GPE_SceneTileMap::get_tile_x( int xInPixels)
{
    if(  tileAmountX == 0 || tileWidth == 0)
    {
        return -1;
    }
    if( xInPixels > x_pos )
    {
        xInPixels-= x_pos;
        if( xInPixels < tileWidth * tileAmountX )
        {
            return xInPixels /tileWidth;
        }
        else
        {
            return tileAmountX;
        }
    }
    return -1;
}

int GPE_SceneTileMap::get_tile_y( int yInPixels)
{
    if( tileAmountY == 0 || tileHeight == 0)
    {
        return -1;
    }
    if( yInPixels > y_pos )
    {
        yInPixels-= y_pos;
        if( yInPixels < tileHeight * tileAmountY )
        {
            return yInPixels /tileHeight;
        }
        else
        {
            return tileAmountY;
        }
    }
    return -1;
}

GPE_SceneTile* GPE_SceneTileMap::get_tile_at(int xIn, int yIn)
{
    if( xIn >=0 && yIn >=0)
    {
        if( xIn < tileAmountX && yIn < tileAmountY)
        {
            int cTilePosition = xIn+ yIn*tileAmountX;
            if( cTilePosition >=0 && cTilePosition < (int)mapTiles.size() )
            {
                return mapTiles.at(cTilePosition);
            }
        }
    }
    return nullptr;
}

void GPE_SceneTileMap::process_elements()
{
    GPE_SceneBasicClass::process_elements();
    int previousTileSheetId = tilesheetDropDown->get_selected_id();
    PANEL_TS_RESOURCE = gpe_dock->find_panel("Tilesheet");
    if(PANEL_TS_RESOURCE!=nullptr )
    {
        if( tSPreviewer!=nullptr )
        {
            tSPreviewer->tileSheetToPreview = nullptr;
            PANEL_TS_RESOURCE->clear_panel();

            PANEL_TS_RESOURCE->add_gui_element(tilesheetDropDown,true);

            if( tilesheetDropDown!=nullptr && tSPreviewer!=nullptr )
            {
                if( tilesheetDropDown->is_clicked() || previousTileSheetId != tilesheetDropDown->get_selected_id() )
                {
                    tileToPlaceX1 = 0;
                    tileToPlaceY1 = 0;
                    tileToPlaceX2 = 0;
                    tileToPlaceY2 = 0;
                    tilesToPlacePerRow = 0;
                    tileIdsToPlace.clear();
                    tSPreviewer->reset_preview(true);
                }
                pawgui::widget_resource_container * tsTypeContainer = tilesheetDropDown->get_selected_container();
                if( tsTypeContainer!=nullptr)
                {
                    tsRes = (tilesheetResource*) tsTypeContainer->get_held_resource();
                    tSPreviewer->tileSheetToPreview = tsRes->tilesheetInEditor;
                }
                tSPreviewer->set_width( PANEL_TS_RESOURCE->get_width()-pawgui::padding_default );
                tSPreviewer->set_height( PANEL_TS_RESOURCE->get_height()-pawgui::padding_default-tilesheetDropDown->get_height() );
                PANEL_TS_RESOURCE->add_gui_element(tSPreviewer,true);
            }
            PANEL_TS_RESOURCE->process_self( );
        }
    }

    int sceneTileMouseX = 0;
    int sceneTileMouseY = 0;
    if( spm !=nullptr )
    {
        if( tileWidth > 0)
        {
            sceneTileMouseX = ( spm->mouseXPos - x_pos) / tileWidth;
        }
        else
        {
            sceneTileMouseX = 0;
        }
        if( tileHeight > 0)
        {
            sceneTileMouseY = ( spm->mouseYPos - y_pos) /tileHeight;
        }
    }
    else
    {
        return;
    }
    //if( shortcut_buttonBar->get_tab_id() == SCENE_MODE_PLACE && mouseIsInScene && tSPreviewer!=nullptr  &&  !selectedLayerMap->isLocked  )
    if( tSPreviewer!=nullptr &&  !isLocked && tSPreviewer!=nullptr && spm->mouseInScene && spm->editMode == SCENE_MODE_PLACE )
    {
        if( gpe::input->check_mouse_down( mb_left ) && tileWidth  > 0 && tileHeight  > 0  && (int)tSPreviewer->tilesIdsInPreview.size()>0  )
        {
            //Place Tiles and Such
            if( tsRes!=nullptr  && tsRes->tilesheetInEditor!=nullptr && tsRes->tilesheetInEditor->tsImage!=nullptr)
            {
                //select tiles to place / place tiles / add tiles
                //if( pawgui::resource_dragged==nullptr && mouseIsInScene && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
                {
                    GPE_SceneTile* fSceneTileToEdit = nullptr;

                    int tileRowItr = 0;
                    int tilesItr= 0;
                    int newTileX = 0, newTileY = 0;

                    for( tilesItr = 0; tilesItr < (int)tSPreviewer->tilesIdsInPreview.size(); tilesItr++ )
                    {
                        fSceneTileToEdit = get_tile_at(sceneTileMouseX+newTileX,sceneTileMouseY+newTileY);
                        if( fSceneTileToEdit!=nullptr)
                        {
                            fSceneTileToEdit->tileIndexId = tSPreviewer->tilesIdsInPreview.at(tilesItr);
                            fSceneTileToEdit->tilesheetIndexId = tilesheetDropDown->get_selected_id();
                            fSceneTileToEdit->tileTypeId = 1;
                        }
                        else
                        {
                            //    pawgui::main_overlay_system->update_tooltip("Unable to find scene tile to edit...");
                        }
                        newTileX+=1;
                        tileRowItr+=1;
                        if( tileRowItr >= tSPreviewer->tilesToPlacePerRow)
                        {
                            tileRowItr = 0;
                            newTileX = 0;
                            newTileY+=1;
                        }
                    }
                }
            }
        }
    }
    //else if(shortcut_buttonBar->get_tab_id() == SCENE_MODE_ERASE && mouseIsInScene && selectedLayerMap!=nullptr  &&  !selectedLayerMap->isLocked  )
    else if(  !isLocked && gpe::input->check_mouse_down( mb_right )   && spm->mouseInScene  && spm->editMode == SCENE_MODE_ERASE  )
    {
        //Remove Tiles / Delete Tiles
        //if( mouseIsInScene && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
        if( spm!=nullptr )
        {
            GPE_SceneTile* fSceneTileToEdit = nullptr;

            fSceneTileToEdit = get_tile_at(sceneTileMouseX,sceneTileMouseY);
            if( fSceneTileToEdit!=nullptr )
            {
                fSceneTileToEdit->tileIndexId = -1;
                fSceneTileToEdit->tilesheetIndexId = -1;
                fSceneTileToEdit->tileTypeId = 1;
            }
        }
    }
}

void GPE_SceneTileMap::resize_tilemap( int new_xt, int new_yt,int ntileType)
{
    if( (new_xt!=tileAmountX || new_yt!=tileAmountY )&& new_xt>0 && new_yt>0)
    {
        int newSize = new_xt*new_yt;
        GPE_SceneTile *newTile = nullptr;
        GPE_SceneTile *prevTile = nullptr;
        int i, j;
        std::vector <GPE_SceneTile*> tempMapTiles;
        for( i=0; i<(int)mapTiles.size(); i++)
        {
            prevTile = mapTiles[i];
            newTile = new GPE_SceneTile();
            if( prevTile!=nullptr)
            {
                newTile->tileTypeId = ntileType;
                newTile->tileIndexId = prevTile->tileIndexId;
                newTile->tilesheetIndexId = prevTile->tilesheetIndexId;
            }
            tempMapTiles.push_back(newTile);
        }

        //destroys old map to be reborn later
        if( (int)mapTiles.size() >0 )
        {
            for(std::vector<GPE_SceneTile*>::const_iterator it = mapTiles.begin(); it != mapTiles.end(); it++)
            {
                delete *it;
            }
        }
        mapTiles.clear();

        gpe::error_log->report("Map cleared ("+ stg_ex::int_to_string((int)mapTiles.size() ) +").");
        gpe::error_log->report("New Dimensions ("+ stg_ex::int_to_string(new_xt)  +" x "+ stg_ex::int_to_string(new_yt)+" = "+ stg_ex::int_to_string(newSize)+".");

        for( i=0; i<newSize; i++)
        {
            newTile = new GPE_SceneTile();
            newTile->tileTypeId = ntileType;
            mapTiles.push_back(newTile);
        }

        gpe::error_log->report("Map resized ("+ stg_ex::int_to_string((int)mapTiles.size() ) +").");
        //creates the tile layer with new dimensions all blanked out
        newTile = nullptr;
        int iMaxPrevXTiles = std::max(new_xt, tileAmountX);
        int jMaxPrevYTiles = std::max(new_yt, tileAmountY);

        if( (int)tempMapTiles.size() >0 && (int)mapTiles.size() >0 )
        {
            for( j=0; j<jMaxPrevYTiles; j++)
            {
                for( i=0; i<iMaxPrevXTiles; i++)
                {
                    if( tileAmountX > i && tileAmountY > j)
                    {
                        prevTile = tempMapTiles.at(i+tileAmountX*j);
                    }
                    else
                    {
                        prevTile = nullptr;
                    }
                    if( new_xt > i && new_yt > j)
                    {
                        newTile = mapTiles.at(i+new_xt*j);
                    }
                    else
                    {
                        newTile = nullptr;
                    }
                    if( prevTile!=nullptr && newTile!=nullptr)
                    {
                        newTile->tileTypeId = prevTile->tileIndexId;
                        newTile->tileIndexId = prevTile->tileIndexId;
                        newTile->tilesheetIndexId = prevTile->tilesheetIndexId;
                    }
                }
            }
        }

        tileAmountX=new_xt;
        tileAmountY=new_yt;
        //destroys the temp map
        if( (int)tempMapTiles.size() >0 )
        {
            for(std::vector<GPE_SceneTile*>::const_iterator itOld = tempMapTiles.begin(); itOld != tempMapTiles.end(); itOld++)
            {
                delete *itOld;
            }
            tempMapTiles.clear();
        }

        x_posField->set_number( x_pos );
        y_posField->set_number( y_pos );

        fieldTileWidth->set_number( tileWidth );
        fieldTileHeight->set_number( tileHeight );

        fieldAmountX->set_number( tileAmountX );
        fieldAmountY->set_number( tileAmountY );

        gpe::error_log->report("Tile Map created.");
        gpe::error_log->report("Map updated after resize ("+ stg_ex::int_to_string((int)mapTiles.size() ) +").");
    }
}

void GPE_SceneTileMap::set_map_size( int newW, int newH )
{
    if( tileWidth !=0 & tileHeight!=0 )
    {
        int expectedNewSizeX = ceil( (float)newW/tileWidth );
        int expectedNewSizeY = ceil( (float)newW/tileHeight );
        if( prevTileAmountX <  tileAmountX || prevTileAmountX <  tileAmountY )
        {
            resize_tilemap( expectedNewSizeX, expectedNewSizeY);
        }
    }
}

void GPE_SceneTileMap::render_branch()
{
    //Avoid the time waste and don't continue if alpha is too low.
    if( branchAlpha!=nullptr && branchAlpha->get_value() < 5)
    {
        return;
    }

    if( spm == nullptr)
    {
        return;
    }

    if( spm->cSceneTstList!=nullptr &&  tileWidth!=0 && tileHeight!=0 && spm->zoomValue!=0 )
    {
        gpe::shape_rect foundTsRect;
        int cTileXPos = 0;
        int cTileYPos = 0;
        int i = 0;
        int j = 0;
        int foundTilePos = 0;
        int tileXStartPos = std::max(0, get_tile_x( spm->cameraFloorXPos )-2 );
        int tileYStartPos = std::max(0, get_tile_y( spm->cameraFloorYPos )-2 );


        int tileXEndPos = tileXStartPos + std::max(0,get_tile_x( ( spm->currentCamera->w  / spm->zoomValue) ) ) +2;
        int tileYEndPos = tileYStartPos + std::max(0,get_tile_y( ( spm->currentCamera->h  / spm->zoomValue) ) ) +2;

        if( tileXStartPos>=0 && tileXStartPos > tileXEndPos)
        {
            tileXEndPos = tileAmountX;
        }

        if( tileYStartPos>=0 && tileYStartPos > tileYEndPos)
        {
            tileYEndPos = tileAmountY;
        }

        GPE_SceneTile * fSceneTile = nullptr;
        pawgui::widget_resource_container * foundTSResource = nullptr;
        tilesheetResource * foundHeldTSRes = nullptr;

        //gpe::error_log->report("Performing 1st loop");
        for( i = tileXStartPos; i < tileAmountX; i++ )
        {
            //gpe::error_log->report("Performing 2nd loop");
            for(  j = tileYStartPos; j < tileAmountY; j++)
            {
                fSceneTile = get_tile_at( i, j);
                if( fSceneTile!=nullptr)
                {
                    if( fSceneTile->tilesheetIndexId >= 0 && fSceneTile->tileIndexId >= 0 )
                    {
                        foundTSResource = spm->cSceneTstList->find_resource_from_id(fSceneTile->tilesheetIndexId);
                        if( foundTSResource!=nullptr)
                        {
                            foundHeldTSRes = (tilesheetResource * )foundTSResource->get_held_resource();
                            if( foundHeldTSRes->tilesheetInEditor!=nullptr )
                            {
                                if( foundHeldTSRes->tilesheetInEditor->tsImage!=nullptr && fSceneTile->tileIndexId < (int)foundHeldTSRes->tilesheetInEditor->tsRects.size() )
                                {
                                    cTileXPos = floor( (i*tileWidth )  * spm->zoomValue ) - spm->currentCamera->x * spm->zoomValue;
                                    cTileYPos = floor( (j*tileHeight ) * spm->zoomValue ) - spm->currentCamera->y * spm->zoomValue;
                                    foundTsRect = foundHeldTSRes->tilesheetInEditor->tsRects.at(fSceneTile->tileIndexId );
                                    //if( check_collision(editorCameraRect,cTileXPos,cTileYPos,foundTsRect.w,foundTsRect.h) )
                                    {
                                        foundHeldTSRes->tilesheetInEditor->tsImage->render_tex_scaled( cTileXPos,cTileYPos, spm->zoomValue,spm->zoomValue,&foundTsRect,branchColor->get_color(), branchAlpha->get_value() );
                                        /*if( renderOutlines )
                                        {
                                            gpe::gcanvas->render_rectangle( cTileXPos,cTileYPos, cTileXPos+ceil( (float)foundTsRect.w*spm->zoomValue),cTileYPos+ceil( (float)foundTsRect.h*spm->zoomValue ), c_red, true, 255 );
                                            //render_text( cTileXPos,cTileYPos, stg_ex::int_to_string(fSceneTile->tilesheetIndexId)+"-"+ stg_ex::int_to_string(fSceneTile->tileIndexId),c_red,gpe::font_default,gpe::fa_left,gpe::fa_top, 255 );
                                        }
                                        */
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //gpe::error_log->report("2nd loop completed");
        }
        //gpe::error_log->report("1st loop completed");

        //Previews tilesheet if selected
        pawgui::widget_resource_container * tsTypeContainer = tilesheetDropDown->get_selected_container();
        if( tsTypeContainer!=nullptr  && spm->mouseInScene && spm->editMode == SCENE_MODE_PLACE && pawgui::resource_dragged==nullptr )
        {
            tsRes = (tilesheetResource*) tsTypeContainer->get_held_resource();
            if( tsRes!=nullptr && tsRes->tilesheetInEditor!=nullptr && tsRes->tilesheetInEditor->tsImage!=nullptr)
            {
                //if( sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
                {
                    int sceneTileMouseX  = spm->mouseXPos;
                    int sceneTileMouseY  = spm->mouseYPos;

                    if( tileWidth!=0 &&tileHeight!=0)
                    {
                        sceneTileMouseX = sceneTileMouseX/tileWidth;
                        sceneTileMouseX = (sceneTileMouseX *tileWidth);
                        sceneTileMouseY = sceneTileMouseY/tileHeight;
                        sceneTileMouseY = (sceneTileMouseY *tileHeight);
                    }
                    sceneTileMouseX = floor( (sceneTileMouseX*spm->zoomValue-spm->cameraFloorXPos * spm->zoomValue ) );
                    sceneTileMouseY = floor( (sceneTileMouseY*spm->zoomValue-spm->cameraFloorYPos * spm->zoomValue) );

                    if( isLocked )
                    {
                        tSPreviewer->render_selection( sceneTileMouseX,sceneTileMouseY, nullptr,nullptr, spm->zoomValue, gpe::c_orangered );
                    }
                    else if( sceneTileMouseX >=0 && sceneTileMouseY >=0 )
                    {
                        tSPreviewer->render_selection( sceneTileMouseX,sceneTileMouseY,nullptr,nullptr,spm->zoomValue, gpe::c_white );
                    }
                    /*
                    else if( spm->mouseXPos < 0 || spm->mouseYPos < 0 || spm->mouseXPos > spm->tempRect->w || spm->mouseYPos > spm->tempRect->h )
                    {
                        tSPreviewer->render_selection( sceneTileMouseX,sceneTileMouseY,nullptr,nullptr,true,spm->zoomValue, c_red );
                    }
                    else
                    {
                        tSPreviewer->render_selection( sceneTileMouseX,sceneTileMouseY,nullptr,nullptr,true,spm->zoomValue,c_white);
                    }
                    */
                }
            }
        }
    }
    pawgui::widget_branch::render_branch();
}

bool GPE_SceneTileMap::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=nullptr && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+ "[GPE_TileMap=";
        *fileTarget << stg_ex::int_to_string( tileWidth )+",";
        *fileTarget << stg_ex::int_to_string( tileHeight )+",";
        *fileTarget << stg_ex::int_to_string( tileAmountX )+",";
        *fileTarget << stg_ex::int_to_string( tileAmountY )+",";
        *fileTarget << stg_ex::int_to_string( x_pos )+",";
        *fileTarget << stg_ex::int_to_string( y_pos )+",";
        if( fillScene!=nullptr)
        {
            *fileTarget << stg_ex::int_to_string( fillScene->is_clicked() )+",";
        }
        else
        {
            *fileTarget << "0,";
        }
        if( (int)name.size() > 0 )
        {
            *fileTarget <<                name +",,";
        }
        else
        {
            *fileTarget <<                 +"tilemap,,";
        }
        *fileTarget << "]\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        int maxTilesInMap = (int)mapTiles.size();
        nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn+1 );
        GPE_SceneTile * fSceneTile = nullptr;
        for(  int i = 0; i < maxTilesInMap; i++)
        {
            fSceneTile = mapTiles.at(i);
            if(  fSceneTile!=nullptr && fSceneTile->tilesheetIndexId > 0 && fSceneTile->tileIndexId >= 0  )
            {
                *fileTarget << nestedTabsStr+ "GPE_SingleTile=";
                //This is the position of the tile in the tilemap
                *fileTarget << stg_ex::int_to_string( i )+",";
                *fileTarget << stg_ex::int_to_string( fSceneTile->tilesheetIndexId )+",";
                *fileTarget << stg_ex::int_to_string( fSceneTile->tileIndexId )+",";
                *fileTarget << stg_ex::int_to_string( fSceneTile->tileTypeId )+",";
                /*
                *fileTarget <<                fSceneTile->get_name() +",,";
                //For later version I plan to add more data here...
                fSceneTile->save_branch_data( fileTarget, nestedFoldersIn+1 );
                */
                *fileTarget << "\n";
            }
        }
        *fileTarget << nestedTabsStr+"[/GPE_TileMap]\n";
        return true;
    }
    return false;
}
