/*
gpe_scene_tilemap_class.cpp
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

#include "gpe_scene_tilemap_class.h"

GPE_SceneTile::GPE_SceneTile()
{
    isLocked = false;
    branchType = BRANCH_TYPE_TILE;
    iconTexture = guiRCM->texture_add( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/table.png") ;

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
    if( PANEL_INSPECTOR!=NULL )
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

GPE_SceneTileMap::GPE_SceneTileMap( std::string mapName, int x, int y, GPE_GeneralResourceContainer *pFolder  )
{
    projectParentFolder = pFolder;
    if( projectParentFolder!=NULL)
    {
        tilesheetDropDown = new GPE_DropDown_Resouce_Menu( "Tilesheet",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TILESHEET]+"s"),-1,true);
        tilesheetDropDown->set_width(192);
        tSPreviewer = new tilesheetPreviewer();

    }
    else
    {
        tilesheetDropDown = NULL;
        tSPreviewer = NULL;
    }

    isLocked = false;
    set_position( x, y);
    branchType = BRANCH_TYPE_TILEMAP;
    iconTexture = guiRCM->texture_add( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/th.png") ;

    update_rect(&tsPlacementArea,0,0,0,0);

    tileWidth = GPE_DEFAULT_TILEHEIGHT;
    tileHeight = GPE_DEFAULT_TILEHEIGHT;

    tileAmountX = prevTileAmountX = 0;
    tileAmountY = prevTileAmountY = 0;

    xPosField->set_number( xPos );
    xPosField->set_label("Map-XStart");

    yPosField->set_number( yPos );
    yPosField->set_label("Map-YStart");

    fieldTileWidth = new GPE_TextInputNumber( "32" );
    fieldTileWidth->set_number( tileWidth );
    fieldTileWidth->set_label("Tile Width");
    fieldTileWidth->scale_width( 0.4 );

    fieldTileHeight = new GPE_TextInputNumber( "32" );
    fieldTileHeight->set_number( tileHeight );
    fieldTileHeight->set_label("Tile Height");
    fieldTileHeight->scale_width( 0.4 );

    fieldAmountX = new GPE_TextInputNumber( "" );
    fieldAmountX->set_number( tileAmountX );
    fieldAmountX->set_label("Map Width");
    fieldAmountX->scale_width( 0.4 );

    fieldAmountY = new GPE_TextInputNumber( "y-tile-amount" );
    fieldAmountY->set_label("Map Height");
    fieldAmountY->set_number( tileAmountY );
    fieldAmountY->scale_width( 0.4 );

    fillScene = new GPE_CheckBoxBasic("Fill Scene","Check to use remainder of scene" );
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
    if( PANEL_INSPECTOR!=NULL )
    {
        PANEL_INSPECTOR->add_gui_element( tilesheetDropDown, true );
        PANEL_INSPECTOR->add_gui_element( fieldTileWidth, false );
        PANEL_INSPECTOR->add_gui_element( fieldTileHeight, true );
        PANEL_INSPECTOR->add_gui_element( fieldAmountX, false );
        PANEL_INSPECTOR->add_gui_element( fieldAmountY, true );
        PANEL_INSPECTOR->add_gui_element( fillScene, true );
    }
}

GPE_SceneTileMap::~GPE_SceneTileMap()
{
    if( tSPreviewer!=NULL)
    {
        delete tSPreviewer;
        tSPreviewer = NULL;
    }
}

bool GPE_SceneTileMap::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount, GPE_GeneralResourceContainer * localResTypeController )
{
    if( localResTypeController == NULL )
    {
        return false;
    }
    std::string nestedTabsStr = generate_tabs( leftTabAmount  );
    GPE_SceneTile * fSceneTile = NULL;
    GPE_GeneralResourceContainer * fTSToPlace = NULL;
    int maxTilesInLayer = (int)mapTiles.size();

    for(  int i = 0; i < maxTilesInLayer; i++)
    {
        fSceneTile = mapTiles.at(i);
        if( fSceneTile!=NULL &&fSceneTile->tilesheetIndexId>=0 && fSceneTile->tileIndexId>=0)
        {
            fTSToPlace = localResTypeController->find_resource_from_id(fSceneTile->tilesheetIndexId);
            if( fTSToPlace!=NULL)
            {
                *fileTarget << nestedTabsStr << "_scn_temp_layer.scnStartTiles.push( {tileNumber: " << int_to_string( i ) << ",";
                *fileTarget << "tileSheetId: " << int_to_string(fTSToPlace->exportBuildGlobalId) << ",";
                *fileTarget << "tileIndexId: " << int_to_string(fSceneTile->tileIndexId);
                *fileTarget << "}); \n";
            }
        }
    }
    GPE_SceneBasicClass::build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController);
    return true;
}

void GPE_SceneTileMap::calculate_size()
{

}

void GPE_SceneTileMap::create_new_map(int newTX, int newTY,int ntileType )
{
    tileAmountX=newTX;
    tileAmountY=newTY;
    int newSize = tileAmountX*tileAmountY;
    if( mapTiles.size() >0 )
    {
        for(std::vector<GPE_SceneTile*>::const_iterator it = mapTiles.begin(); it != mapTiles.end(); it++)
        {
            delete *it;
        }
        mapTiles.clear();
    }
    GPE_SceneTile * newTile = NULL;
    for(int i=0; i<newSize; i++)
    {
        newTile = new GPE_SceneTile();
        newTile->tileTypeId = ntileType;
        mapTiles.push_back(newTile);
    }

    xPosField->set_number( xPos );
    yPosField->set_number( yPos );

    fieldTileWidth->set_number( tileWidth );
    fieldTileHeight->set_number( tileHeight );

    fieldAmountX->set_number( tileAmountX );
    fieldAmountY->set_number( tileAmountY );

    GPE_Report("Tile Map created.");
}

int GPE_SceneTileMap::get_map_size()
{
    return (int)mapTiles.size();
}

int GPE_SceneTileMap::get_xmax()
{
    return xPos+tileWidth * tileAmountX;
}

int GPE_SceneTileMap::get_ymax()
{
    return yPos+tileHeight * tileAmountY;
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
    if( xInPixels > xPos )
    {
        xInPixels-= xPos;
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
    if( yInPixels > yPos )
    {
        yInPixels-= yPos;
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
    return NULL;
}

void GPE_SceneTileMap::process_elements()
{
    GPE_SceneBasicClass::process_elements();
    int previousTileSheetId = tilesheetDropDown->get_selected_id();
    PANEL_TS_RESOURCE = GPE_DOCK->find_panel("Tilesheet");
    if(PANEL_TS_RESOURCE!=NULL )
    {
        if( tSPreviewer!=NULL )
        {
            tSPreviewer->tileSheetToPreview = NULL;
            PANEL_TS_RESOURCE->clear_panel();

            PANEL_TS_RESOURCE->add_gui_element(tilesheetDropDown,true);

            if( tilesheetDropDown!=NULL && tSPreviewer!=NULL )
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
                GPE_GeneralResourceContainer * tsTypeContainer = tilesheetDropDown->get_selected_container();
                if( tsTypeContainer!=NULL)
                {
                    tsRes = (tilesheetResource*) tsTypeContainer->get_held_resource();
                    tSPreviewer->tileSheetToPreview = tsRes->tilesheetInEditor;
                }
                tSPreviewer->set_width( PANEL_TS_RESOURCE->get_width()-GENERAL_GPE_GUI_PADDING );
                tSPreviewer->set_height( PANEL_TS_RESOURCE->get_height()-GENERAL_GPE_GUI_PADDING-tilesheetDropDown->get_height() );
                PANEL_TS_RESOURCE->add_gui_element(tSPreviewer,true);
            }
            PANEL_TS_RESOURCE->process_self( );
        }
    }

    int sceneTileMouseX = 0;
    int sceneTileMouseY = 0;
    if( spm !=NULL )
    {
        if( tileWidth > 0)
        {
            sceneTileMouseX = ( spm->mouseXPos - xPos) / tileWidth;
        }
        else
        {
            sceneTileMouseX = 0;
        }
        if( tileHeight > 0)
        {
            sceneTileMouseY = ( spm->mouseYPos - yPos) /tileHeight;
        }
    }
    else
    {
        return;
    }
    //if( shortcutButtonBar->get_tab_id() == SCENE_MODE_PLACE && mouseIsInScene && tSPreviewer!=NULL  &&  !selectedLayerMap->isLocked  )
    if( tSPreviewer!=NULL &&  !isLocked && tSPreviewer!=NULL && spm->mouseInScene && spm->editMode == SCENE_MODE_PLACE )
    {
        if( input->check_mouse_down( mb_left ) && tileWidth  > 0 && tileHeight  > 0  && (int)tSPreviewer->tilesIdsInPreview.size()>0  )
        {
            //Place Tiles and Such
            if( tsRes!=NULL  && tsRes->tilesheetInEditor!=NULL && tsRes->tilesheetInEditor->tsImage!=NULL)
            {
                //select tiles to place / place tiles / add tiles
                //if( RESOURCE_TO_DRAG==NULL && mouseIsInScene && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
                {
                    GPE_SceneTile* fSceneTileToEdit = NULL;

                    int tileRowItr = 0;
                    int tilesItr= 0;
                    int newTileX = 0, newTileY = 0;

                    for( tilesItr = 0; tilesItr < (int)tSPreviewer->tilesIdsInPreview.size(); tilesItr++ )
                    {
                        fSceneTileToEdit = get_tile_at(sceneTileMouseX+newTileX,sceneTileMouseY+newTileY);
                        if( fSceneTileToEdit!=NULL)
                        {
                            fSceneTileToEdit->tileIndexId = tSPreviewer->tilesIdsInPreview.at(tilesItr);
                            fSceneTileToEdit->tilesheetIndexId = tilesheetDropDown->get_selected_id();
                            fSceneTileToEdit->tileTypeId = 1;
                        }
                        else
                        {
                            //    MAIN_OVERLAY->update_tooltip("Unable to find scene tile to edit...");
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
    //else if(shortcutButtonBar->get_tab_id() == SCENE_MODE_ERASE && mouseIsInScene && selectedLayerMap!=NULL  &&  !selectedLayerMap->isLocked  )
    else if(  !isLocked && input->check_mouse_down( mb_right )   && spm->mouseInScene  && spm->editMode == SCENE_MODE_ERASE  )
    {
        //Remove Tiles / Delete Tiles
        //if( mouseIsInScene && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
        if( spm!=NULL )
        {
            GPE_SceneTile* fSceneTileToEdit = NULL;

            fSceneTileToEdit = get_tile_at(sceneTileMouseX,sceneTileMouseY);
            if( fSceneTileToEdit!=NULL )
            {
                fSceneTileToEdit->tileIndexId = -1;
                fSceneTileToEdit->tilesheetIndexId = -1;
                fSceneTileToEdit->tileTypeId = 1;
            }
        }
    }
}

void GPE_SceneTileMap::resize_tilemap( int newTX, int newTY,int ntileType)
{
    if( (newTX!=tileAmountX || newTY!=tileAmountY )&& newTX>0 && newTY>0)
    {
        int newSize = newTX*newTY;
        GPE_SceneTile *newTile = NULL;
        GPE_SceneTile *prevTile = NULL;
        int i, j;
        std::vector <GPE_SceneTile*> tempMapTiles;
        for( i=0; i<(int)mapTiles.size(); i++)
        {
            prevTile = mapTiles[i];
            newTile = new GPE_SceneTile();
            if( prevTile!=NULL)
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

        GPE_Report("Map cleared ("+int_to_string((int)mapTiles.size() ) +").");
        GPE_Report("New Dimensions ("+int_to_string(newTX)  +" x "+int_to_string(newTY)+" = "+int_to_string(newSize)+".");

        for( i=0; i<newSize; i++)
        {
            newTile = new GPE_SceneTile();
            newTile->tileTypeId = ntileType;
            mapTiles.push_back(newTile);
        }

        GPE_Report("Map resized ("+int_to_string((int)mapTiles.size() ) +").");
        //creates the tile layer with new dimensions all blanked out
        newTile = NULL;
        int iMaxPrevXTiles = std::max(newTX, tileAmountX);
        int jMaxPrevYTiles = std::max(newTY, tileAmountY);

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
                        prevTile = NULL;
                    }
                    if( newTX > i && newTY > j)
                    {
                        newTile = mapTiles.at(i+newTX*j);
                    }
                    else
                    {
                        newTile = NULL;
                    }
                    if( prevTile!=NULL && newTile!=NULL)
                    {
                        newTile->tileTypeId = prevTile->tileIndexId;
                        newTile->tileIndexId = prevTile->tileIndexId;
                        newTile->tilesheetIndexId = prevTile->tilesheetIndexId;
                    }
                }
            }
        }

        tileAmountX=newTX;
        tileAmountY=newTY;
        //destroys the temp map
        if( (int)tempMapTiles.size() >0 )
        {
            for(std::vector<GPE_SceneTile*>::const_iterator itOld = tempMapTiles.begin(); itOld != tempMapTiles.end(); itOld++)
            {
                delete *itOld;
            }
            tempMapTiles.clear();
        }

        xPosField->set_number( xPos );
        yPosField->set_number( yPos );

        fieldTileWidth->set_number( tileWidth );
        fieldTileHeight->set_number( tileHeight );

        fieldAmountX->set_number( tileAmountX );
        fieldAmountY->set_number( tileAmountY );

        GPE_Report("Tile Map created.");
        GPE_Report("Map updated after resize ("+int_to_string((int)mapTiles.size() ) +").");
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
    if( branchAlpha!=NULL && branchAlpha->get_value() < 5)
    {
        return;
    }

    if( spm == NULL)
    {
        return;
    }

    if( spm->cSceneTstList!=NULL &&  tileWidth!=0 && tileHeight!=0 && spm->zoomValue!=0 )
    {
        GPE_Rect foundTsRect;
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

        GPE_SceneTile * fSceneTile = NULL;
        GPE_GeneralResourceContainer * foundTSResource = NULL;
        tilesheetResource * foundHeldTSRes = NULL;

        //GPE_Report("Performing 1st loop");
        for( i = tileXStartPos; i < tileAmountX; i++ )
        {
            //GPE_Report("Performing 2nd loop");
            for(  j = tileYStartPos; j < tileAmountY; j++)
            {
                fSceneTile = get_tile_at( i, j);
                if( fSceneTile!=NULL)
                {
                    if( fSceneTile->tilesheetIndexId >= 0 && fSceneTile->tileIndexId >= 0 )
                    {
                        foundTSResource = spm->cSceneTstList->find_resource_from_id(fSceneTile->tilesheetIndexId);
                        if( foundTSResource!=NULL)
                        {
                            foundHeldTSRes = (tilesheetResource * )foundTSResource->get_held_resource();
                            if( foundHeldTSRes->tilesheetInEditor!=NULL )
                            {
                                if( foundHeldTSRes->tilesheetInEditor->tsImage!=NULL && fSceneTile->tileIndexId < (int)foundHeldTSRes->tilesheetInEditor->tsRects.size() )
                                {
                                    cTileXPos = floor( (i*tileWidth )  * spm->zoomValue ) - spm->currentCamera->x * spm->zoomValue;
                                    cTileYPos = floor( (j*tileHeight ) * spm->zoomValue ) - spm->currentCamera->y * spm->zoomValue;
                                    foundTsRect = foundHeldTSRes->tilesheetInEditor->tsRects.at(fSceneTile->tileIndexId );
                                    //if( check_collision(editorCameraRect,cTileXPos,cTileYPos,foundTsRect.w,foundTsRect.h) )
                                    {
                                        foundHeldTSRes->tilesheetInEditor->tsImage->render_tex_scaled( cTileXPos,cTileYPos, spm->zoomValue,spm->zoomValue,&foundTsRect,branchColor->get_color(), branchAlpha->get_value() );
                                        /*if( renderOutlines )
                                        {
                                            gcanvas->render_rectangle( cTileXPos,cTileYPos, cTileXPos+ceil( (float)foundTsRect.w*spm->zoomValue),cTileYPos+ceil( (float)foundTsRect.h*spm->zoomValue ), c_red, true, 255 );
                                            //render_text( cTileXPos,cTileYPos, int_to_string(fSceneTile->tilesheetIndexId)+"-"+int_to_string(fSceneTile->tileIndexId),c_red,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP, 255 );
                                        }
                                        */
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //GPE_Report("2nd loop completed");
        }
        //GPE_Report("1st loop completed");

        //Previews tilesheet if selected
        GPE_GeneralResourceContainer * tsTypeContainer = tilesheetDropDown->get_selected_container();
        if( tsTypeContainer!=NULL  && spm->mouseInScene && spm->editMode == SCENE_MODE_PLACE && RESOURCE_TO_DRAG==NULL )
        {
            tsRes = (tilesheetResource*) tsTypeContainer->get_held_resource();
            if( tsRes!=NULL && tsRes->tilesheetInEditor!=NULL && tsRes->tilesheetInEditor->tsImage!=NULL)
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
                        tSPreviewer->render_selection( sceneTileMouseX,sceneTileMouseY, NULL,NULL, spm->zoomValue, c_orangered );
                    }
                    else if( sceneTileMouseX >=0 && sceneTileMouseY >=0 )
                    {
                        tSPreviewer->render_selection( sceneTileMouseX,sceneTileMouseY,NULL,NULL,spm->zoomValue,c_white);
                    }
                    /*
                    else if( spm->mouseXPos < 0 || spm->mouseYPos < 0 || spm->mouseXPos > spm->tempRect->w || spm->mouseYPos > spm->tempRect->h )
                    {
                        tSPreviewer->render_selection( sceneTileMouseX,sceneTileMouseY,NULL,NULL,true,spm->zoomValue, c_red );
                    }
                    else
                    {
                        tSPreviewer->render_selection( sceneTileMouseX,sceneTileMouseY,NULL,NULL,true,spm->zoomValue,c_white);
                    }
                    */
                }
            }
        }
    }
    GPE_SpecialMenu_Branch::render_branch();
}

bool GPE_SceneTileMap::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+ "[GPE_TileMap=";
        *fileTarget << int_to_string( tileWidth )+",";
        *fileTarget << int_to_string( tileHeight )+",";
        *fileTarget << int_to_string( tileAmountX )+",";
        *fileTarget << int_to_string( tileAmountY )+",";
        *fileTarget << int_to_string( xPos )+",";
        *fileTarget << int_to_string( yPos )+",";
        if( fillScene!=NULL)
        {
            *fileTarget << int_to_string( fillScene->is_clicked() )+",";
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
        nestedTabsStr = generate_tabs( nestedFoldersIn+1 );
        GPE_SceneTile * fSceneTile = NULL;
        for(  int i = 0; i < maxTilesInMap; i++)
        {
            fSceneTile = mapTiles.at(i);
            if(  fSceneTile!=NULL && fSceneTile->tilesheetIndexId > 0 && fSceneTile->tileIndexId >= 0  )
            {
                *fileTarget << nestedTabsStr+ "GPE_SingleTile=";
                //This is the position of the tile in the tilemap
                *fileTarget << int_to_string( i )+",";
                *fileTarget << int_to_string( fSceneTile->tilesheetIndexId )+",";
                *fileTarget << int_to_string( fSceneTile->tileIndexId )+",";
                *fileTarget << int_to_string( fSceneTile->tileTypeId )+",";
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
