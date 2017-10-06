/*
gamesceneresource.cpp
This file is part of:
GAME PENCI ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_editor.h"

GPE_SceneTexture::GPE_SceneTexture(GPE_ResourceContainer *pFolder)
{
    textureId = -1;
    texRes = NULL;
    bgRect.x = 0;
    bgRect.y = 0;
    bgRect.w = 0;
    bgRect.h = 0;
    inFront = false;
    tileHori = false;
    tileVert = false;
    stretchBG = false;
    bgXSpeed = 0;
    bgYSpeed =0;
    projectParentFolder = pFolder;

    if( projectParentFolder!=NULL)
    {
        backgroundInEditor = new GPE_DropDown_Resouce_Menu(0,0,"Background Texture",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE]+"s"),-1,true);
        backgroundInEditor->set_width(192);
    }
    else
    {
        backgroundInEditor = NULL;
    }
    bgPosXField = new GPE_TextInputNumber("","");
    bgPosXField->set_string("0");
    bgPosXField->set_label("X-Pos");
    bgPosXField->set_width(192);

    bgPosYField = new GPE_TextInputNumber("","");
    bgPosYField->set_string("0");
    bgPosYField->set_label("Y-Pos");
    bgPosYField->set_width(192);

    bgHorSpeedField = new GPE_TextInputNumber("","");
    bgHorSpeedField->set_string("0");
    bgHorSpeedField->set_label("Horizontal Speed");
    bgHorSpeedField->set_width(192);

    bgVertSpeedField = new GPE_TextInputNumber("","");
    bgVertSpeedField->set_string("0");
    bgVertSpeedField->set_label("Vertical Speed");
    bgVertSpeedField->set_width(192);

    checkTileHori = new GPE_CheckBoxBasic("Tile-Hori","Repeat Tile Horizontally to fill the scene",GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+320);
    checkTileVert = new GPE_CheckBoxBasic("Tile-Vert","Repeat Tile Vertically to fill the scene",GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+352);
    checkStretch = new GPE_CheckBoxBasic("Stretch BG","Stretch across the entire scene",GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+384);

}

GPE_SceneTexture::~GPE_SceneTexture()
{

}

void GPE_SceneTexture::process_background_data()
{
    textureId = backgroundInEditor->get_selected_id();
    if(backgroundInEditor->get_selected_id() > 0)
    {
        //if( bgPosXField->get_held_number() >=0 )
        {
            bgRect.x = bgPosXField->get_held_number();
        }

        //if( bgPosYField->get_held_number() >=0 )
        {
            bgRect.y = bgPosYField->get_held_number();
        }
        //if( bgHorSpeedField->get_held_number() )
        {
            bgXSpeed = bgHorSpeedField->get_held_number();
        }
        //if( bgVertSpeedField->get_held_number() )
        {
            bgYSpeed = bgVertSpeedField->get_held_number();
        }
        tileHori =checkTileHori->is_clicked();
        tileVert =checkTileVert->is_clicked();
        stretchBG =checkStretch->is_clicked();
    }
}


GPE_SceneTile::GPE_SceneTile()
{
    tileTypeId = 0;
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

sceneLayer::sceneLayer(int newLayerType, int foundProjectLayerId,GPE_ResourceContainer *pFolder )
{
    tileAmountX=0;
    tileAmountY =0;
    //layerId=-1;
    layerAlpha=0;
    projectLayerId = foundProjectLayerId;
    prevTileAmountX=0;
    prevTileAmountY=0;
    layerType = newLayerType;
    projectParentFolder = pFolder;
    layerBGSelectBox = new GPE_SelectBoxBasic();
}

sceneLayer::~sceneLayer()
{
    if( (int)mapTiles.size() >0 )
    {
        GPE_SceneTile* tempTile = NULL;
        for( int i = (int)mapTiles.size()-1; i >=0; i--)
        {
            tempTile = mapTiles[i];
            if( tempTile!=NULL)
            {
                delete tempTile;
                tempTile = NULL;
            }
        }
        mapTiles.clear();
    }
    if( (int)layerObjects.size() >0 )
    {
        GPE_SceneGameObject* tempObject = NULL;
        for( int i = (int)layerObjects.size()-1; i >=0; i--)
        {
            tempObject = layerObjects[i];
            if( tempObject!=NULL)
            {
                delete tempObject;
                tempObject = NULL;
            }
        }
        layerObjects.clear();
    }
}

void sceneLayer::create_new_map(int newTX, int newTY,int ntileType)
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
    GPE_SceneTile *newTile = NULL;
    for(int i=0;i<newSize;i++)
    {
        newTile = new GPE_SceneTile();
        newTile->tileTypeId = ntileType;
        mapTiles.push_back(newTile);
    }
    record_error("Tile Layer created.");
}

void sceneLayer::resize_tilemap(int newTX, int newTY,int ntileType)
{
    if( (newTX!=tileAmountX || newTY!=tileAmountY )&& newTX>0 && newTY>0)
    {
        int newSize = newTX*newTY;
        GPE_SceneTile *newTile = NULL;
        GPE_SceneTile *prevTile = NULL;
        int i, j;
        std::vector <GPE_SceneTile*> tempMapTiles;
        for( i=0;i<(int)mapTiles.size();i++)
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
        record_error("Map cleared ("+int_to_string((int)mapTiles.size() ) +").");
        record_error("New Dimensions ("+int_to_string(newTX)  +" x "+int_to_string(newTY)+" = "+int_to_string(newSize)+".");
        for( i=0;i<newSize;i++)
        {
            newTile = new GPE_SceneTile();
            newTile->tileTypeId = ntileType;
            mapTiles.push_back(newTile);
        }
        record_error("Map resized ("+int_to_string((int)mapTiles.size() ) +").");
        //creates the tile layer with new dimensions all blanked out
        newTile = NULL;
        int iMaxPrevXTiles = std::max(newTX, tileAmountX);
        int jMaxPrevYTiles = std::max(newTY, tileAmountY);

        if( (int)tempMapTiles.size() >0 && (int)mapTiles.size() >0 )
        {
            for( j=0;j<jMaxPrevYTiles;j++)
            {
                for( i=0;i<iMaxPrevXTiles;i++)
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

        record_error("Map updated after resize ("+int_to_string((int)mapTiles.size() ) +").");
    }
}

void sceneLayer::clear_all_objects()
{
    GPE_SceneGameObject * tObject = NULL;
    for( int i = layerObjects.size()-1; i >=0; i--)
    {
        tObject = layerObjects[i];
        if( tObject!=NULL)
        {
            delete tObject;
            tObject = NULL;
        }
    }
    layerObjects.clear();
}

void sceneLayer::clear_objects(int objectTypeToClear)
{
    GPE_SceneGameObject * tObject = NULL;
    for( int i = layerObjects.size()-1; i >=0; i--)
    {
        tObject = layerObjects[i];
        if( tObject!=NULL)
        {
            if( tObject->objTypeId == objectTypeToClear)
            {

                delete tObject;
                tObject = NULL;
                layerObjects.erase(layerObjects.begin()+i);
            }
        }
    }
}

int sceneLayer::get_map_size()
{
    return (int)mapTiles.size();
}

void sceneLayer::add_background(GPE_SceneTexture * newBG)
{
    if( newBG!=NULL)
    {
        record_error("Attempting to add BG to layer["+int_to_string(projectLayerId)+"].");
        if( layerBGSelectBox==NULL)
        {
            layerBGSelectBox = new GPE_SelectBoxBasic();
        }
        if( layerBGSelectBox!=NULL)
        {
            layerBGSelectBox->add_option("Background"+int_to_string( (int)layerBackgrounds.size() ) ) ;
            layerBackgrounds.push_back( newBG );
        }
        else
        {
            record_error("BGSelctBox = NULl..");
        }
    }
    else
    {
        record_error("Null BG unable to add...");
    }
}

GPE_SceneTile* sceneLayer::get_tile_at(int xIn, int yIn)
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

GPE_SceneGameObject::GPE_SceneGameObject()
{
    objTypeName = "";
    isBeingMoved = false;
    objTypeId = -1;
    customObjId = -1;
    objBeingPlaced = NULL;
    objRect.x = 0;
    objRect.y = 0;
    objRect.w = 32;
    objRect.h = 32;
    objColor = new GPE_Input_Field_Color();
    objColor->set_name("Object Color");
    objColor->set_label("Object Color");

    objSubName = new GPE_TextInputBasic();
    objSubName->set_name("Nickname");
    objSubName->set_label("Nickname");
    objAngleField = new GPE_TextInputNumber("");
    objAngleField->set_name("Angle");
    objAngleField->set_label("Angle");
    objAngle = 0;
    objXScale = 1;
    objYScale = 1;

}

GPE_SceneGameObject::~GPE_SceneGameObject()
{
    if( objColor!=NULL)
    {
        delete objColor;
        objColor = NULL;
    }
    if( objAngleField!=NULL)
    {
        delete objAngleField;
        objAngleField = NULL;
    }
    if( objSubName!=NULL)
    {
        delete objSubName;
        objSubName = NULL;
    }
    GPE_ObjectComponent * tField = NULL;
    for( int i = (int)objCustomValPairs.size()-1; i >=0; i--)
    {
        tField = objCustomValPairs[i];
        if( tField!=NULL)
        {
            delete tField;
            tField = NULL;
        }
    }
    objCustomVariables = "";
}

GPE_ObjectComponent * GPE_SceneGameObject::add_variable(GPE_GeneralGuiElement * newVariable)
{
    if( newVariable!=NULL)
    {
        std::string newVarName = newVariable->get_name();
        if( (int)newVarName.size() > 0)
        {
            bool foundVal = false;
            GPE_ObjectComponent * tField = NULL;
            for( int i = 0; i < (int)objCustomValPairs.size(); i++)
            {
                tField = objCustomValPairs[i];
                if( tField!=NULL)
                {
                    if( tField->get_name()==newVarName)
                    {
                        foundVal = true;
                        return NULL;
                    }
                }
            }
            if( foundVal==false)
            {
                GPE_ObjectComponent * newComponent = new GPE_ObjectComponent();
                newComponent->set_component(newVariable);
                objCustomValPairs.push_back(newComponent);
                return newComponent;
            }
        }
    }
    return NULL;
}

void GPE_SceneGameObject::reset_components()
{
    GPE_ObjectComponent * tField = NULL;
    for( int i = (int)objCustomValPairs.size()-1; i >=0; i--)
    {
        tField = objCustomValPairs[i];
        if( tField!=NULL)
        {
            delete tField;
            tField = NULL;
        }
    }
    objCustomValPairs.clear();
}

gameSceneResource::gameSceneResource(GPE_ResourceContainer * pFolder)
{
    sceneZoomLevel = new GPE_DropDown_Menu(0,0,"",true);
    sceneZoomLevel->add_menu_option("10%","10",10,false);
    sceneZoomLevel->add_menu_option("25%","25",25,false);
    sceneZoomLevel->add_menu_option("33%","33",33,false);
    sceneZoomLevel->add_menu_option("50%","50",50,false);
    sceneZoomLevel->add_menu_option("67%","67",67,false);
    sceneZoomLevel->add_menu_option("75%","75",75,false);
    sceneZoomLevel->add_menu_option("80%","80",80,false);
    sceneZoomLevel->add_menu_option("90%","90",90,false);
    sceneZoomLevel->add_menu_option("100%","100",100,true);
    sceneZoomLevel->add_menu_option("110%","110",110,false);
    sceneZoomLevel->add_menu_option("125%","125",125,false);
    sceneZoomLevel->add_menu_option("150%","150",150,false);
    sceneZoomLevel->add_menu_option("175%","175",175,false);
    sceneZoomLevel->add_menu_option("200%","200",200,false);
    sceneZoomLevel->add_menu_option("250%","250",250,false);
    sceneZoomLevel->add_menu_option("300%","300",300,false);
    sceneZoomLevel->add_menu_option("400%","400",400,false);
    sceneZoomLevel->add_menu_option("500%","500",500,false);
    sceneZoomLevel->set_width(96);
    sceneZoomLevel->set_height(24);

    sceneZoomAmount = 1;
    defaultObjectLayer = NULL;
    defaultBackgroundLayer = NULL;
    currentLayer = NULL;
    layerPaneList = new GPE_GuiElementList();
    isDraggingObject = false;
    sceneAreaScrollable = false;
    scnPostProcessed = true;
    projectParentFolder = pFolder;
    editorMode = 0;
    editorPane.x = 0;
    editorPane.y = 0;
    editorPane.w = 228;
    rightPaneWidth = 228;
    rightEditorPane.x = 0;
    rightEditorPane.y = 0;
    rightEditorPane.w = rightPaneWidth;
    rightEditorPane.h = 0;
    editorPane.h = SCREEN_HEIGHT;
    sceneRect.x = 0;
    sceneRect.y =0;
    sceneRect.w = 4096;
    sceneRect.h = 4096;
    sceneEditorViewRect.x = 0;
    sceneEditorViewRect.y = 0;
    sceneEditorViewRect.w = 640;
    sceneEditorViewRect.h = 480;
    showTileLines = true;
    tileToPlaceX1 = 0;
    tileToPlaceY1 = 0;
    tileToPlaceX2 = 0;
    tileToPlaceY2 = 0;
    lvlTileWidth = 32;
    lvlTileHeight = 32;
    lvlTileAmountX = sceneRect.w/lvlTileWidth;
    lvlTileAmountY =sceneRect.h/lvlTileHeight;
    objSnapX = objSnapY =16;
    showObjLines = true;
    sceneMouseXPos = 0;
    sceneMouseYPos = 0;
    saveButton = new GPE_ToolIconButton(0,0, "resources/gfx/buttons/save.png","Save Changes",-1,24);
    sceneEditorSubTitle = new GPE_Label_Title("Settings");
    editorButtonBar = new GPE_ToolIconButtonBar(editorPane.x,editorPane.y,32, true);
    editorButtonBar->barPadding = 0;
    editorButtonBar->xPadding = 0;
    editorButtonBar->set_height(24);
    editorButtonBar->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/dashboard.png","Settings",-1,false);
    editorButtonBar->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/object-group.png","Layers",-1,false);
    //editorButtonBar->adkb_dton("resources/gfx/buttons/percent.png","Statistics",-1,false);

    confirmResourceButton->enable_self();
    confirmResourceButton->set_name("Confirm Changes");
    loadResourceButton->disable_self();
    saveResourceButton->enable_self();
    cancelResourceButton->enable_self();
    cancelResourceButton->set_name("Undo Changes  ");
    cancelResourceButton->descriptionText = "Revert Level Settings to previous save";
    //Start Variables used for the settings tab
    renameBox->set_coords(GENERAL_GPE_PADDING,64);
    renameBox->set_label("Scene Title");
    renameBox->set_height(24);
    renameBox->set_width(128);
    renameBox->disable_self();
    levelTitleField = NULL;
    sceneCaptionField = new GPE_TextInputBasic("","Scene Caption");
    sceneCaptionField->set_label("Scene Caption");
    levelHintField = new GPE_TextInputBasic("","Scene Hint");
    levelHintField->set_label("Scene Hint");
    levelPixelWidthField = new GPE_TextInputNumber("",true,0);
    levelPixelWidthField->set_string( int_to_string(sceneRect.w) );
    levelPixelWidthField->set_label("Scene Width(px)");
    levelPixelHeightField = new GPE_TextInputNumber("",true,0);
    levelPixelHeightField->set_string( int_to_string(sceneRect.h) );
    levelPixelHeightField->set_label("Scene Height(px)");

    levelTileWidthField = new GPE_TextInputNumber("width",true,0);
    levelTileWidthField->set_string("32");
    levelTileWidthField->set_label("Tile Width(px)");
    levelTileHeightField = new GPE_TextInputNumber("height",true,0);
    levelTileHeightField->set_string("32");
    levelTileHeightField->set_label("Tile Height(px)");

    musicAudioDropDown = NULL;
    endAudioDropDown = NULL;
    startAudioDropDown = NULL;

    northSceneDropDown = NULL;
    southSceneDropDown = NULL;
    westSceneDropDown = NULL;
    eastSceneDropDown = NULL;
    if( projectParentFolder!=NULL)
    {
        musicAudioDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING+32,GENERAL_GPE_PADDING+288,"BG Music",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_AUDIO]),-1,true);
        musicAudioDropDown->set_width(128);

        startAudioDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING+32,GENERAL_GPE_PADDING+288,"Start Audio",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_AUDIO]),-1,true);
        startAudioDropDown->set_width(128);

        endAudioDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING+32,GENERAL_GPE_PADDING+288,"End Audio",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_AUDIO]),-1,true);
        endAudioDropDown->set_width(128);

        //
        northSceneDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING+32,GENERAL_GPE_PADDING+288,"North",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE]+"s"),-1,true);
        northSceneDropDown->set_width(128);

        southSceneDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING+32,GENERAL_GPE_PADDING+288,"South",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE]+"s"),-1,true);
        southSceneDropDown->set_width(128);

        eastSceneDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING+32,GENERAL_GPE_PADDING+288,"East",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE]+"s"),-1,true);
        eastSceneDropDown->set_width(128);

        westSceneDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING+32,GENERAL_GPE_PADDING+288,"West",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE]+"s"),-1,true);
        westSceneDropDown->set_width(128);
    }

    preloadCheckBox = new GPE_CheckBoxBasic("Preload","Check to load scene at launch of program",GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+336,true);
    isometricCheckBox = new GPE_CheckBoxBasic("Isometric Scene","The Scene is rendered and processed with the isometric engine",GENERAL_GPE_PADDING+128,GENERAL_GPE_PADDING+336,false);
    checkBoxIsContinuous = new GPE_CheckBoxBasic("Continuous Scene","Scene Data & Objects are stored throughout game[Reccommended: Keep unchecked for most scenes]",GENERAL_GPE_PADDING+96,160,false);
    sceneBackgroundColor = new GPE_Input_Field_Color();
    sceneBackgroundColor->set_rgb(0,0,0);
    sceneBackgroundColor->set_label("Scene Background Color");
    //end Variables for settings Tab

    //variables for the backgrounds tab
    bgIdInEdit = 0;
    backgroundToSelect = new GPE_SelectBoxBasic(GENERAL_GPE_PADDING,64,"Backgrounds");
    backgroundToSelect->showCheckboxes = true;
    //variables for the objects tab
    layerInEdit = -1;
    tileIdPlace = -1;
    tilesToPlacePerRow = 0;
    if( projectParentFolder!=NULL)
    {
        addNewComponentDropDown = new GPE_DropDown_Menu(0,0,"Add Component",false);
        addNewComponentDropDown->add_menu_option("Check-Box","checkbox",1,false);
        addNewComponentDropDown->add_menu_option("Color-Picker","colorpicker",2,false);
        addNewComponentDropDown->add_menu_option("Drop-Down","dropdown",3,false);
        addNewComponentDropDown->add_menu_option("Input-Text","inputtext",4,false);
        addNewComponentDropDown->add_menu_option("Input-Number","inputnumber",5,false);
        addNewComponentDropDown->add_menu_option("Radio Button","radio",6,false);
        addNewComponentDropDown->add_menu_option("Text Label","labeltext",7,false);
        addNewComponentDropDown->add_menu_option("Text URL","texturl",8,false);
        addNewComponentDropDown->add_menu_option("Texture","Textures-resourcedropdown",50,false);
        addNewComponentDropDown->add_menu_option("Tilesheet","Tilesheets-resourcedropdown",51,false);
        addNewComponentDropDown->add_menu_option("Sprite","Sprites-resourcedropdown",52,false);
        addNewComponentDropDown->add_menu_option("Audio","Audio-resourcedropdown",53,false);
        addNewComponentDropDown->add_menu_option("Video","Videos-resourcedropdown",54,false);
        addNewComponentDropDown->add_menu_option("Function","Functions-resourcedropdown",55,false);
        addNewComponentDropDown->add_menu_option("Object","Objects-resourcedropdown",56,false);
        addNewComponentDropDown->add_menu_option("Scene","Scenes-resourcedropdown",57,false);
        addNewComponentDropDown->add_menu_option("Font","Fonts-resourcedropdown",58,false);
        addNewComponentDropDown->set_width(128);
        objectInEditor = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING*2+64,"Object",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s"),-1,true);
        objectReplacerInEditor = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING*2+64,"Replace Object",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s"),-1,true);
        objectNewType = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING*2+64,"Change Object",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s"),-1,true);

        closeObjectProperties = new GPE_ToolLabelButton(0,0,"Close Properties","Closes Object Properties of Selected Object");
        levelObjGridWidthField = new GPE_TextInputNumber("width",true,0);
        levelObjGridWidthField->set_string("32");
        levelObjGridWidthField->set_label("Object Grid Width");
        levelObjGridHeightField = new GPE_TextInputNumber("height",true,0);
        levelObjGridHeightField->set_string("32");
        levelObjGridHeightField->set_label("Object Grid Height");
        viewTileGridCheckBox = new GPE_CheckBoxBasic("View Grid","View grid.",GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+224,true);
        useObjGridCheckBox = new GPE_CheckBoxBasic("Place on Grid","Have future objects placed on a uniform grid.",GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+224,true);
        objectLeftClickModeButton = new GPE_CheckBoxBasic("Select Mode","Only Select Objects with left click. Unchecked to place.",GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+224,false);
        onlyRemoveThisObjectTypeButton = new GPE_CheckBoxBasic("Limit Scope","Only select/remove this object on right-click",GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+224,false);
        forceSnapButton = new GPE_ToolLabelButton(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+288,"Force Snap Objects","Aligns all objects with the object grid");
        forceSnapButton->set_width(192);
        removeObjectButton = new GPE_ToolLabelButton(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+288,"Remove from Scene","Removes only this object type from the scene");
        clearObjectsButton = new GPE_ToolLabelButton(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+288,"Clear Objects","Removes all Objects from scene");
        removeObjectButton->set_width(forceSnapButton->get_width());
        clearObjectsButton->set_width(forceSnapButton->get_width());

        inheritParentComponentButton = new GPE_ToolLabelButton(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+288,"Inherit Components","Inherit components from entity's object class");
        resetComponentsButton = new GPE_ToolLabelButton(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING+288,"Reset Components","Removes all components from this object");
        resetComponentsButton->set_width(forceSnapButton->get_width());
        resetComponentsButton->set_width(forceSnapButton->get_width());

        objCustomVariableSettingsButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/edit.png","Edit Variable");
        objCustomVariableAddButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/plus.png","Add Variable");
        objCustomVariableRemoveButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/minus.png", "Remove Variable" );
        objCustomVariableRefeshButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/refresh.png","Refresh Object Default Variables");

        //variables for the tiled tab
        update_rect(&tsRenderRect,0,0,0,0);
        update_rect(&tsPlacementArea,0,0,0,0);
        layerMenu = new GPE_SelectBoxBasic(0,0,"");
        layerMenu->set_width(224);
        layerMenu->limit_height(120);
        layerMenu->showCheckboxes = true;

        layerErrorMessage = new GPE_Label_Text("Error invalid layer type given...","ERROR");
        layerSettingsButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/cogs.png","Layer Settings",-1,32);
        layerAddButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/plus.png","Add Layer",-1,32);
        layerRemoveButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/remove.png", "Remove Layer",-1,32 );
        layerMoveUpButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/arrow-up.png", "Move Layer Up",-1,32);
        layerMoveDownButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/arrow-down.png","Move Layer Down",-1,32 );
        layerToggleHideButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/adjust.png","Hide/Unhide Other Layers",-1,32 );

        layerBackgroundSettingsButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/cogs.png","Backgrounds Settings",-1,32);
        layerBackgroundAddButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/plus.png","Add Background",-1,32);
        layerBackgroundRemoveButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/remove.png", "Remove Background",-1,32 );
        layerBackgroundMoveUpButtton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/arrow-up.png", "Move Background Up",-1,32);
        layerBackgroundMoveDownButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/arrow-down.png","Move Background Down",-1,32 );
        //layerBackgroundToggleHideButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/adjust.png","Hide/Unhide Other Backgrounds",-1,32 );

        tilesheetDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING,layerAddButtton->get_y2pos()+GENERAL_GPE_PADDING,"Tilesheet",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TILESHEET]+"s"),-1,true);
        tSPreviewer = new tilesheetPreviewer();
        tilesheetRenderXPos = 0;
        tilesheetRenderYPos = editorPaneList->get_y2pos()+GENERAL_GPE_PADDING;

    }
    else
    {
        addNewComponentDropDown = NULL;
        objectInEditor = NULL;
        objectReplacerInEditor = NULL;
        objectNewType = NULL;
        tilesheetDropDown = NULL;
        layerMenu = NULL;
    }
    selectedSceneObject = NULL;
    customObjCreationId = 0;
    objectEditorNameTitle = new GPE_Label_Text("None","None");
    objectEditorPropertiesTitle = new GPE_Label_Text("Object Properties","Object Properties");
    selectedObjXPos = new GPE_TextInputNumber("");
    selectedObjXPos->set_label("X-Pos");
    selectedObjYPos = new GPE_TextInputNumber("");
    selectedObjYPos->set_label("Y-Pos");
    lastCreatedObjXPos=-1;
    lastCreatedObjYPos=-1;
    lastCreatedObjTypeId=-1;
    editorMode = LEDITOR_MODE_SETTINGS;
    sceneXScroll = new GPE_ScrollBar_XAxis();
    update_rectangle(&sceneXScroll->contextRect,0,0,640,480);
    sceneYScroll = new GPE_ScrollBar_YAxis();
    update_rectangle(&sceneYScroll->contextRect,0,0,640,480);
    texRes = NULL;
    tsRes = NULL;
}

gameSceneResource::~gameSceneResource()
{
    int i = 0;
    sceneLayer * tempTileLayer = NULL;
    for( i = (int)sceneLayers.size()-1; i >=0; i--)
    {
         tempTileLayer= sceneLayers[i];
         if( tempTileLayer!=NULL)
         {
             delete tempTileLayer;
             tempTileLayer= NULL;
         }
    }
    sceneLayers.clear();

    GPE_SceneTexture * tempSceneBackGround = NULL;
    GPE_DropDown_Resouce_Menu * tempBackgroundInEditor= NULL;
    GPE_TextInputNumber * tempBgPosXField= NULL;
    GPE_TextInputNumber * tempBgPosYField= NULL;
    GPE_TextInputNumber * tempBgHorSpeedField= NULL;
    GPE_TextInputNumber * tempBgVertSpeedField= NULL;

    GPE_CheckBoxBasic * tempCheckForeground= NULL;
    GPE_CheckBoxBasic * tempCheckTileHori= NULL;
    GPE_CheckBoxBasic * tempCheckTileVert= NULL;
    GPE_CheckBoxBasic * tempCheckStretch= NULL;

    if(layerPaneList!=NULL)
    {
        delete layerPaneList;
        layerPaneList = NULL;
    }
    if(objectInEditor!=NULL)
    {
        delete objectInEditor;
        objectInEditor = NULL;
    }
    if(objectNewType!=NULL)
    {
        delete objectNewType;
        objectNewType = NULL;
    }
    if(objectReplacerInEditor!=NULL)
    {
        delete objectReplacerInEditor;
        objectReplacerInEditor = NULL;
    }
    if( viewTileGridCheckBox!=NULL )
    {
        delete viewTileGridCheckBox;
        viewTileGridCheckBox = NULL;
    }
    if( selectedObjXPos!=NULL )
    {
        delete selectedObjXPos;
        selectedObjXPos = NULL;
    }
    if( objectEditorPropertiesTitle!=NULL )
    {
        delete objectEditorPropertiesTitle;
        objectEditorPropertiesTitle = NULL;
    }
    if( objectReplacerInEditor!=NULL )
    {
        delete objectReplacerInEditor;
        objectReplacerInEditor = NULL;
    }
    if( closeObjectProperties!=NULL )
    {
        delete closeObjectProperties;
        closeObjectProperties = NULL;
    }

    if( addNewComponentDropDown!=NULL )
    {
        delete addNewComponentDropDown;
        addNewComponentDropDown = NULL;
    }
    if( inheritParentComponentButton!=NULL )
    {
        delete inheritParentComponentButton;
        inheritParentComponentButton = NULL;
    }

    if( resetComponentsButton!=NULL )
    {
        delete resetComponentsButton;
        resetComponentsButton = NULL;
    }
    if( objCustomVariableSettingsButtton!=NULL )
    {
        delete objCustomVariableSettingsButtton;
        objCustomVariableSettingsButtton = NULL;
    }
    if( objCustomVariableAddButtton!=NULL )
    {
        delete objCustomVariableAddButtton;
        objCustomVariableAddButtton = NULL;
    }

    if( objCustomVariableRemoveButton!=NULL )
    {
        delete objCustomVariableRemoveButton;
        objCustomVariableRemoveButton = NULL;
    }

    if( objCustomVariableRefeshButton!=NULL )
    {
        delete objCustomVariableRefeshButton;
        objCustomVariableRefeshButton = NULL;
    }

    if(tilesheetDropDown!=NULL)
    {
        delete tilesheetDropDown;
        tilesheetDropDown = NULL;
    }
    if(layerMenu!=NULL)
    {
        delete layerMenu;
        layerMenu= NULL;
    }
    if(layerSettingsButtton!=NULL)
    {
        delete layerSettingsButtton;
        layerSettingsButtton= NULL;
    }
    if(layerAddButtton!=NULL)
    {
        delete layerAddButtton;
        layerAddButtton= NULL;
    }
    if(layerRemoveButton!=NULL)
    {
        delete layerRemoveButton;
        layerRemoveButton= NULL;
    }

    if(layerMoveUpButtton!=NULL)
    {
        delete layerMoveUpButtton;
        layerMoveUpButtton= NULL;
    }

    if(layerMoveDownButton!=NULL)
    {
        delete layerMoveDownButton;
        layerMoveDownButton= NULL;
    }
    if(layerToggleHideButton!=NULL)
    {
        delete layerToggleHideButton;
        layerToggleHideButton= NULL;
    }
    if( tSPreviewer!=NULL)
    {
        delete tSPreviewer;
        tSPreviewer = NULL;
    }
    if( checkBoxIsContinuous!=NULL)
    {
        delete checkBoxIsContinuous;
        checkBoxIsContinuous = NULL;
    }

    if( backgroundToSelect!=NULL)
    {
        delete backgroundToSelect;
        backgroundToSelect = NULL;
    }
    if( sceneXScroll!=NULL)
    {
        delete sceneXScroll;
        sceneXScroll = NULL;
    }
    if( sceneYScroll!=NULL)
    {
        delete sceneYScroll;
        sceneYScroll = NULL;
    }
    if( saveButton!=NULL )
    {
        delete saveButton;
        saveButton = NULL;
    }
    if( levelObjGridWidthField!=NULL)
    {
        delete levelObjGridWidthField;
        levelObjGridWidthField = NULL;
    }
    if( levelObjGridHeightField!=NULL)
    {
        delete levelObjGridHeightField;
        levelObjGridHeightField = NULL;
    }
    if( useObjGridCheckBox!=NULL)
    {
        delete useObjGridCheckBox;
        useObjGridCheckBox = NULL;
    }
    if( viewTileGridCheckBox!=NULL)
    {
        delete viewTileGridCheckBox;
        viewTileGridCheckBox = NULL;
    }
    if( objectLeftClickModeButton!=NULL)
    {
        delete objectLeftClickModeButton;
        objectLeftClickModeButton = NULL;
    }
    if( onlyRemoveThisObjectTypeButton!=NULL)
    {
        delete onlyRemoveThisObjectTypeButton;
        onlyRemoveThisObjectTypeButton = NULL;
    }
    if( removeObjectButton!=NULL)
    {
        delete removeObjectButton;
        removeObjectButton = NULL;
    }
    if( clearObjectsButton!=NULL)
    {
        delete clearObjectsButton;
        clearObjectsButton = NULL;
    }

    if( backgroundToSelect!=NULL)
    {
        delete backgroundToSelect;
        backgroundToSelect = NULL;
    }
    if( sceneBackgroundColor!=NULL)
    {
        delete sceneBackgroundColor;
        sceneBackgroundColor = NULL;
    }
    if( checkBoxIsContinuous!=NULL)
    {
        delete checkBoxIsContinuous;
        checkBoxIsContinuous = NULL;
    }
    if( isometricCheckBox!=NULL)
    {
        delete isometricCheckBox;
        isometricCheckBox = NULL;
    }
    if( preloadCheckBox!=NULL)
    {
        delete preloadCheckBox;
        preloadCheckBox = NULL;
    }

    if( levelPixelWidthField!=NULL)
    {
        delete levelPixelWidthField;
        levelPixelWidthField = NULL;
    }
    if( levelPixelHeightField!=NULL)
    {
        delete levelPixelHeightField;
        levelPixelHeightField = NULL;
    }
    if( levelTileWidthField!=NULL)
    {
        delete levelTileWidthField;
        levelTileWidthField = NULL;
    }
    if( levelTileHeightField!=NULL)
    {
        delete levelTileHeightField;
        levelTileHeightField = NULL;
    }
    if( musicAudioDropDown!=NULL)
    {
        delete musicAudioDropDown;
        musicAudioDropDown = NULL;
    }
    if( startAudioDropDown!=NULL)
    {
        delete startAudioDropDown;
        startAudioDropDown = NULL;
    }
    if( endAudioDropDown!=NULL)
    {
        delete endAudioDropDown;
        endAudioDropDown = NULL;
    }
    if( levelTitleField!=NULL)
    {
        delete levelTitleField;
        levelTitleField = NULL;
    }
    if( sceneCaptionField!=NULL)
    {
        delete sceneCaptionField;
        sceneCaptionField = NULL;
    }
    if( levelHintField!=NULL)
    {
        delete levelHintField;
        levelHintField = NULL;
    }
    if( sceneEditorSubTitle!=NULL)
    {
        delete sceneEditorSubTitle;
        sceneEditorSubTitle = NULL;
    }
    if( editorButtonBar!=NULL)
    {
        delete editorButtonBar;
        editorButtonBar = NULL;
    }

}

sceneLayer * gameSceneResource::add_layer(int layerType, int newLayerId)
{
    sceneLayer * previousFoundLayer = NULL;
    sceneLayer * newLayer = NULL;
    if( layerType >=0 && layerType <= LAYER_TYPE_MAX)
    {
        bool makeNewLayer = false;
        if( newLayerId >=0 & newLayerId < 32)
        {
            previousFoundLayer = find_layer(newLayerId);
            if(  previousFoundLayer==NULL )
            {
                makeNewLayer = true;
            }
            else
            {
                return previousFoundLayer;
            }
        }
        else
        {
            std::vector< int > availableLayerIds;
            int availI = 0;
            int sLayerJ = 0;
            bool layerWasUsed = false;
            for( availI = 0; availI < 32; availI++)
            {
                availableLayerIds.push_back(availI);
            }
            for( availI = (int)(availableLayerIds.size() )-1; availI >=0; availI--)
            {
                layerWasUsed = false;
                for( sLayerJ = (int)(sceneLayers.size() )-1; sLayerJ >=0; sLayerJ--)
                {
                    if( availableLayerIds[ availI ]==sceneLayers[sLayerJ]->projectLayerId )
                    {
                        layerWasUsed = true;
                    }
                }
                if( layerWasUsed )
                {
                    availableLayerIds.erase(availableLayerIds.begin()+availI);
                }
            }
            if( (int)(availableLayerIds.size() )> 0 )
            {
                makeNewLayer = true;
                newLayerId = availableLayerIds.at(0);
            }
        }

        if( makeNewLayer )
        {
            newLayer= new sceneLayer(layerType, newLayerId, projectParentFolder);
            if( newLayer!=NULL)
            {
                if( layerType==LAYER_TYPE_TILES)
                {
                    newLayer->create_new_map(lvlTileAmountX,lvlTileAmountY,0);
                }
                sceneLayers.push_back(newLayer);
                layerMenu->add_option(CURRENT_PROJECT->projectLayerNames[newLayerId],newLayerId,NULL,NULL,-1,true );
            }
        }
        else
        {
            record_error("New Layer NOT CREATED of Type ["+int_to_string(layerType)+"] with id"+int_to_string(newLayerId) +"( already existed).");
        }
    }
    else
    {
        record_error("Unable to create Layer of Type ["+int_to_string(layerType)+"] with id"+int_to_string(newLayerId)+"." );
    }
    return newLayer;
}

bool gameSceneResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    bool buildSuccessful = true;
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        GPE_ObjectComponent * objCustVariableField = NULL;
        int iItr = 0, jItr = 0, kItr = 0;
        std::string customObjLabel  = "";
        std::string customObjValue  = "";
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string nestedTabsStrObjFunc = generate_tabs( leftTabAmount +1 );
        std::string html5SceneName = "_scn_" + get_name();
        //*fileTarget << nestedTabsStr << "var " << get_name() << " = " << html5BuildGlobalId << ";\n";
        //*fileTarget << nestedTabsStr << "var " << html5SceneName << " =  GPE.add_gamescene(); \n";

        *fileTarget << nestedTabsStr << html5SceneName << ".sceneId = " << html5BuildGlobalId << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".sceneName = \"" << get_name() << "\";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".sceneCaption = \"" << sceneCaptionField->get_string() << "\";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".sceneWidth = " << sceneRect.w << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".sceneHeight = " << sceneRect.h << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".tileWidth  = " << ceil(lvlTileWidth) << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".tileHeight = " << ceil(lvlTileHeight) << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".tileAmountX = " << floor(lvlTileAmountX) << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".tileAmountY = " << floor(lvlTileAmountY) << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".bgColor = '#" << sceneBackgroundColor->get_hex_string() << "';\n";

        if(checkBoxIsContinuous!=NULL)
        {
            *fileTarget << nestedTabsStr << html5SceneName << ".sceneIsContinuous = " << checkBoxIsContinuous->is_clicked() << ";\n";
        }
        GPE_ResourceContainer * fAudioId = NULL;
        if( musicAudioDropDown!=NULL)
        {
            fAudioId = musicAudioDropDown->get_selected_container();
            if( fAudioId!=NULL)
            {
                *fileTarget << nestedTabsStr << html5SceneName << ".bgMusic = " << fAudioId->html5BuildGlobalId << ";\n";
            }
        }
        if( startAudioDropDown!=NULL)
        {
            fAudioId = startAudioDropDown->get_selected_container();
            if( fAudioId!=NULL)
            {
                *fileTarget << nestedTabsStr << html5SceneName << ".startAudio = " << fAudioId->html5BuildGlobalId << ";\n";
            }
        }
        if( endAudioDropDown!=NULL)
        {
            fAudioId = endAudioDropDown->get_selected_container();
            if( fAudioId!=NULL)
            {
                *fileTarget << nestedTabsStr << html5SceneName << ".endAudio = " << fAudioId->html5BuildGlobalId << ";\n";
            }
        }
        sceneLayer * tempLayer = NULL;
        GPE_SceneTile* fSceneTile= NULL;
        int maxTilesInLayer = 0;
        GPE_SceneTexture * fSceneBg = NULL;
        int maxBgsInLayer = 0;
        GPE_ResourceContainer * allTilesheetsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TILESHEET]+"s");
        GPE_ResourceContainer * fTSToPlace = NULL;
        GPE_SceneGameObject * tempGameObjToPlace = NULL;

        GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
        GPE_ResourceContainer * fObjToPlace = NULL;

        GPE_ResourceContainer * allTexturesFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE]+"s");
        GPE_ResourceContainer * tempBgResource = NULL;
        for( iItr = 0; iItr < (int)sceneLayers.size(); iItr++)
        {
            tempLayer = sceneLayers[iItr];
            if( tempLayer!=NULL)
            {
                *fileTarget << nestedTabsStr << "_scn_temp_layer = " << html5SceneName << ".add_layer( "+int_to_string(tempLayer->layerType)+","+int_to_string(tempLayer->projectLayerId)+","+int_to_string(tempLayer->layerAlpha)+");\n";
                if( tempLayer->layerType == LAYER_TYPE_TILES)
                {
                    maxTilesInLayer = (int)tempLayer->mapTiles.size();
                    for(  jItr = 0; jItr < maxTilesInLayer;jItr++)
                    {
                        fSceneTile = tempLayer->mapTiles.at(jItr);
                        if( fSceneTile!=NULL &&fSceneTile->tilesheetIndexId>=0 && fSceneTile->tileIndexId>=0)
                        {
                            fTSToPlace = allTilesheetsFolder->find_resource_from_id(fSceneTile->tilesheetIndexId);
                            if( fTSToPlace!=NULL)
                            {
                                *fileTarget << nestedTabsStr << "_scn_temp_layer.scnStartTiles.push( {tileNumber: " << int_to_string(jItr) << ",";
                                *fileTarget << "tileSheetId: " << int_to_string(fTSToPlace->html5BuildGlobalId) << ",";
                                *fileTarget << "tileIndexId: " << int_to_string(fSceneTile->tileIndexId);
                                *fileTarget << "}); \n";
                            }
                        }
                    }
                }
                else if( tempLayer->layerType == LAYER_TYPE_OBJECTS)
                {
                    for( jItr = 0; jItr < (int)tempLayer->layerObjects.size(); jItr++)
                    {
                        tempGameObjToPlace = tempLayer->layerObjects[jItr];
                        if( tempGameObjToPlace!=NULL)
                        {
                            fObjToPlace = allObjsFolder->find_resource_from_id(tempGameObjToPlace->objTypeId);
                            if( fObjToPlace!=NULL)
                            {
                                *fileTarget << nestedTabsStr << "_scn_temp_layer.scnStartObjects.push( {objId:" <<int_to_string(fObjToPlace->html5BuildGlobalId) << ",";
                                *fileTarget << "xPos: " << int_to_string(tempGameObjToPlace->objRect.x) << ",";
                                *fileTarget << "yPos: " << int_to_string(tempGameObjToPlace->objRect.y) << ",";
                                *fileTarget << "customComponents: {";
                                for( kItr = 0; kItr < (int)tempGameObjToPlace->objCustomValPairs.size()-1; kItr++)
                                {
                                    objCustVariableField = tempGameObjToPlace->objCustomValPairs.at(kItr);
                                    if( objCustVariableField !=NULL)
                                    {
                                        customObjLabel = objCustVariableField->get_name();
                                        customObjValue = objCustVariableField->get_plain_string();

                                        if( (int)customObjLabel.size() > 0)
                                        {
                                            if( (int)customObjValue.size()==0)
                                            {
                                                customObjValue = "0";
                                            }
                                            *fileTarget << customObjLabel+": "+customObjValue+",";
                                        }
                                    }
                                }

                                if( (int)tempGameObjToPlace->objCustomValPairs.size() > 0 )
                                {
                                    objCustVariableField = tempGameObjToPlace->objCustomValPairs.at(tempGameObjToPlace->objCustomValPairs.size()-1);
                                    if( objCustVariableField !=NULL)
                                    {
                                        customObjLabel = objCustVariableField->get_name();
                                        customObjValue = objCustVariableField->get_plain_string();

                                        if( (int)customObjLabel.size() > 0)
                                        {
                                            if( is_alnum(customObjLabel,false,true)==true)
                                            {
                                                if( (int)customObjValue.size()==0)
                                                {
                                                    customObjValue = "0";
                                                }
                                                *fileTarget << customObjLabel+": "+customObjValue;
                                            }
                                        }
                                    }
                                }
                                *fileTarget << "}";
                                *fileTarget << "}); \n";
                            }
                        }
                    }
                }
                else if( tempLayer->layerType == LAYER_TYPE_BACKGROUND)
                {
                    maxBgsInLayer = (int)tempLayer->layerBackgrounds.size();
                    for(  jItr = 0; jItr < maxBgsInLayer;jItr++)
                    {
                        fSceneBg = tempLayer->layerBackgrounds.at(jItr);
                        if( fSceneBg!=NULL)
                        {
                            tempBgResource = allTexturesFolder->find_resource_from_id(fSceneBg->textureId);
                            if( tempBgResource!=NULL)
                            {
                                *fileTarget << nestedTabsStr << "_scn_temp_layer.scnStartBackgrounds.push( {bgTexId:" <<int_to_string(tempBgResource->html5BuildGlobalId) << ",";

                                *fileTarget << "bgXPos: " << int_to_string(fSceneBg->bgRect.x) << ",";
                                *fileTarget << "bgYPos: " << int_to_string(fSceneBg->bgRect.y) << ",";
                                *fileTarget << "bgXSpeed: " << int_to_string(fSceneBg->bgXSpeed) << ",";
                                *fileTarget << "bgYSpeed: " << int_to_string(fSceneBg->bgYSpeed) << ",";
                                *fileTarget << "bgTileHori: " << int_to_string(fSceneBg->tileHori) << ",";
                                *fileTarget << "bgTileVert: " << int_to_string(fSceneBg->tileVert) << ",";
                                *fileTarget << "bgStartStretch: " << int_to_string(fSceneBg->stretchBG);
                                *fileTarget << "} ); \n";
                            }
                            else
                            {
                                *fileTarget << nestedTabsStr << "_scn_temp_layer.scnStartBackground = -1; \n";
                            }
                        }
                    }
                }
            }
        }



        /*for( iItr = 0; iItr < MAX_BACKGROUND_NUMBER; iItr++)
        {
            tempSceneBackground = sceneBackGrounds[iItr];
            if( tempSceneBackground!=NULL)
            {
                fSceneBg = allTexturesFolder->find_resource_from_id(tempSceneBackground->textureId);
                if( fSceneBg!=NULL)
                {
                    *fileTarget << nestedTabsStr << html5SceneName << ".scnStartBackgrounds["+int_to_string(iItr)+"]= {bgTexId:" <<int_to_string(fSceneBg->html5BuildGlobalId) << ",";
                    *fileTarget << "bgXPos: " << int_to_string(tempSceneBackground->bgRect.x) << ",";
                    *fileTarget << "bgYPos: " << int_to_string(tempSceneBackground->bgRect.y) << ",";
                    *fileTarget << "bgXSpeed: " << int_to_string(tempSceneBackground->bgXSpeed) << ",";
                    *fileTarget << "bgYSpeed: " << int_to_string(tempSceneBackground->bgYSpeed) << ",";
                    *fileTarget << "bgInFont: " << int_to_string(tempSceneBackground->inFront) << ",";
                    *fileTarget << "bgTileHori: " << int_to_string(tempSceneBackground->tileHori) << ",";
                    *fileTarget << "bgTileVert: " << int_to_string(tempSceneBackground->tileVert) << ",";
                    *fileTarget << "bgStartStretch: " << int_to_string(tempSceneBackground->stretchBG);
                    *fileTarget << "}; \n";
                }
                else
                {
                    *fileTarget << nestedTabsStr << html5SceneName << ".scnStartBackgrounds["+int_to_string(iItr)+"] = -1; \n";
                }
            }
        }*/
        return true;
    }
    else
    {
        buildSuccessful = false;
    }
    return buildSuccessful;
}


sceneLayer * gameSceneResource::find_layer( int layerIdToFind )
{
    sceneLayer * fLayer = NULL;
    for( int i = 0; i < (int)sceneLayers.size(); i++)
    {
        fLayer = sceneLayers.at(i);
        if( fLayer!=NULL)
        {
            if( fLayer->projectLayerId==layerIdToFind )
            {
                return fLayer;
            }
        }
    }
    return NULL;
}

bool gameSceneResource::get_mouse_coords(SDL_Rect * viewedSpace, SDL_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    sceneMouseXPos = 0;
    sceneMouseYPos = 0;
    if( viewedSpace!=NULL)
    {
        if( point_within(userInput->mouse_x,userInput->mouse_y,
                         editorView.x+viewedSpace->x,
                         editorView.y+viewedSpace->y,
                         editorView.x+viewedSpace->x+editorView.w,
                         editorView.y+viewedSpace->y+editorView.h ) )
        {
            sceneMouseXPos = (userInput->mouse_x-editorView.x-viewedSpace->x)/sceneZoomAmount +sceneEditorViewRect.x;
            sceneMouseYPos = (userInput->mouse_y-editorView.y-viewedSpace->y)/sceneZoomAmount +sceneEditorViewRect.y;
            return true;
        }
    }
    return false;
}

void gameSceneResource::handle_scrolling()
{
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    if( sceneAreaScrollable && editorPaneList->hasScrollControl==false && ( editorMode!=LEDITOR_MODE_LAYERS || (  editorMode==LEDITOR_MODE_LAYERS/* && tSPreviewer->hasScrollControl==false*/)  ) )
    {
        if( userInput->check_keyboard_down(kb_ctrl) && sceneZoomLevel!=NULL )
        {
            //Zoom In
            if( userInput->mouseScrollingDown > 0 )
            {
                sceneZoomLevel->set_selection(sceneZoomLevel->get_selected_id()-1 );
            }
            else if( userInput->mouseScrollingUp)
            {
                //zoom out
                sceneZoomLevel->set_selection(sceneZoomLevel->get_selected_id()+1 );
            }

        }
        else if( userInput->shiftKeyIsPressed)
        {
            if( userInput->mouseScrollingUp > 0 )
            {
                xScrollHappened = true;
                sceneEditorViewRect.x-=(sceneEditorViewRect.w/16)*sceneZoomAmount;
            }
            else if( userInput->mouseScrollingDown)
            {
                //zoom out
                xScrollHappened = true;
                sceneEditorViewRect.x+=(sceneEditorViewRect.w/16)*sceneZoomAmount;
            }
        }
        else
        {
            if( userInput->mouseScrollingUp )
            {
                yScrollHappened = true;
                sceneEditorViewRect.y-=(sceneEditorViewRect.h/16)*sceneZoomAmount;
            }
            else if( userInput->mouseScrollingDown)
            {
                yScrollHappened = true;
                sceneEditorViewRect.y+=(sceneEditorViewRect.h/16)*sceneZoomAmount;
            }
            else if( editorPaneList->isInUse==false )
            {
                //arrow scrolling
                if( userInput->check_keyboard_down(kb_up) )
                {
                    yScrollHappened = true;
                    sceneEditorViewRect.y-=(sceneEditorViewRect.h/32)*sceneZoomAmount;
                }
                else if( userInput->check_keyboard_down(kb_down) )
                {
                    yScrollHappened = true;
                    sceneEditorViewRect.y+=(sceneEditorViewRect.h/32)*sceneZoomAmount;
                }
                if( userInput->check_keyboard_down(kb_left) )
                {
                    if( sceneEditorViewRect.x > (sceneEditorViewRect.w/32)*sceneZoomAmount )
                    {
                        xScrollHappened = true;
                        sceneEditorViewRect.x-=(sceneEditorViewRect.w/32)*sceneZoomAmount;
                    }
                    else
                    {
                        sceneEditorViewRect.x = 0;
                        xScrollHappened = true;
                    }
                }
                else if( userInput->check_keyboard_down(kb_right) )
                {
                    if( (sceneEditorViewRect.x +sceneEditorViewRect.w/32)*sceneZoomAmount < sceneRect.w*sceneZoomAmount )
                    {
                        xScrollHappened = true;
                        sceneEditorViewRect.x+=(sceneEditorViewRect.w/32)*sceneZoomAmount;
                    }
                }
            }
        }
    }



    if( sceneEditorViewRect.x+sceneEditorViewRect.w/sceneZoomAmount > sceneRect.w )
    {
        xScrollHappened = true;
        sceneEditorViewRect.x = sceneRect.w-sceneEditorViewRect.w/sceneZoomAmount;
    }

    if( sceneEditorViewRect.y+sceneEditorViewRect.h/sceneZoomAmount > sceneRect.h )
    {
        yScrollHappened = true;
        sceneEditorViewRect.y = sceneRect.h-sceneEditorViewRect.h/sceneZoomAmount;
    }

    if( sceneEditorViewRect.x <= 0)
    {
        xScrollHappened = true;
        sceneEditorViewRect.x = 0;
    }

    if( sceneEditorViewRect.y <= 0)
    {
        yScrollHappened = true;
        sceneEditorViewRect.y = 0;
    }

    if( xScrollHappened)
    {
        sceneXScroll->contextRect.x = sceneEditorViewRect.x;
        sceneXScroll->process_self(NULL,NULL,true);
    }
    if( yScrollHappened)
    {
        sceneXScroll->contextRect.y = sceneEditorViewRect.y;
        sceneYScroll->process_self(NULL,NULL,true);
    }
}

void gameSceneResource::prerender_self(GPE_Renderer * cRender )
{
	standardEditableGameResource::prerender_self( cRender);
	if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self(cRender);
    }

    if( isometricCheckBox!=NULL)
    {
        isometricCheckBox->prerender_self(cRender);
    }
    if( checkBoxIsContinuous!=NULL)
    {
        checkBoxIsContinuous->prerender_self(cRender);
    }
    /*for( int i = 0; i < MAX_BACKGROUND_NUMBER; i++)
    {
        if( checkForeground[i]!=NULL)
        {
            checkForeground[i]->prerender_self(cRender);
        }
        if( checkTileHori[i]!=NULL)
        {
            checkTileHori[i]->prerender_self(cRender);
        }
        if( checkTileVert[i]!=NULL)
        {
            checkTileVert[i]->prerender_self(cRender);
        }
        if( checkStretch[i]!=NULL)
        {
            checkStretch[i]->prerender_self(cRender);
        }
    }
    */
    if( useObjGridCheckBox!=NULL)
    {
        useObjGridCheckBox->prerender_self(cRender);
    }
    if( forceSnapButton!=NULL)
    {
        forceSnapButton->prerender_self(cRender);
    }
}

void gameSceneResource::preprocess_self(std::string alternatePath)
{
    if( scnPostProcessed ==false || file_exists(alternatePath) )
    {
        sceneEditorViewRect.x = 0;
        sceneEditorViewRect.y = 0;
        sceneEditorViewRect.w = 640;
        sceneEditorViewRect.h = 480;
        displayMessageTitle = "Processing GameScene";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();

        std::string otherColContainerName = "";
        std::string newScnFilename = "";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/scenes/";
        if( file_exists(alternatePath) )
        {
            newScnFilename = alternatePath;
            soughtDir = get_path_from_file(newScnFilename);
        }
        else
        {
            newScnFilename = soughtDir + resourceName+".gpf";
        }
        std::ifstream gameScnFileIn( newScnFilename.c_str() );

        //record_error("Loading scene - "+newScnFilename);
        //If the level file could be loaded
        if( gameScnFileIn != NULL )
        {
            //makes sure the file is open
            if (gameScnFileIn.is_open())
            {
                int equalPos = 0;
                int commaPos = 0;
                int newSceneLayerWantedId = 0;
                std::string firstChar="";
                std::string section="";
                std::string cur_layer="";
                std::string data_format="";
                std::string keyString="";
                std::string valString="";

                std::string objCustomKeyString="";
                std::string objCustomValString="";
                std::string subValString="";
                std::string currLine="";

                std::string customObjAllFieldData = "";
                std::string customObjEntryTag = "";
                std::string currLineToBeProcessed;
                float foundFileVersion = 0;
                int foundBgId = 0;
                int foundNumberToRead = 0;
                std::string foundBgName= "";
                std::string foundObjName= "";
                int foundR = 0;
                int foundG = 0;
                int foundB = 0;
                bool olderVersionImportBegan = false;
                int foundLayerType = 0;
                int foundLayerId = 0;
                std::string foundLayerName = "";
                GPE_SceneGameObject * newGameObject = NULL;
                GPE_SceneTile * sceneTileToEdit = NULL;
                sceneLayer * newsceneLayer = NULL;
                sceneLayer *newBackgroundLayer = NULL;
                GPE_SceneTexture *newSceneBg = NULL;
                int tileLayerXPos = 0, tileLayerYPos = 0, foundTilePos = 0;

                std::string objectComponentField = "";
                int beginObjectComponentPos = -1;
                int endObjectComponentPos = -1;
                std::string componentTag = "[component]";
                int componentTagSize = (int)componentTag.size();
                GPE_GeneralGuiElement * newObjectComponent = NULL;
                GPE_ObjectComponent * tempObjectComponent = NULL;

                //GPE_TextAreaInputBasic * textAreaToLoad = NULL;
                GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
                GPE_ResourceContainer * foundObjContainer = NULL;
                while ( gameScnFileIn.good() )
                {
                    getline (gameScnFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = trim_left_inplace(currLine);
                    currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

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
                                        foundFileVersion = string_to_double(valString);
                                        if( !compare_doubles(foundFileVersion , GPE_VERSION_DOUBLE_NUMBER) && foundFileVersion < GPE_VERSION_DOUBLE_NUMBER && !olderVersionImportBegan)
                                        {
                                            if( defaultBackgroundLayer == NULL)
                                            {
                                                defaultBackgroundLayer = add_layer(LAYER_TYPE_BACKGROUND,0);
                                            }
                                            if( defaultObjectLayer == NULL)
                                            {
                                                defaultObjectLayer = add_layer(LAYER_TYPE_OBJECTS,1);
                                            }
                                            olderVersionImportBegan = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion < 2)
                    {
                        //Begin processing the file.
                        if(!currLineToBeProcessed.empty() )
                        {
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( keyString=="ResourceName")
                                {
                                    renameBox->set_string(valString);
                                }
                                else if( keyString=="SceneMusic" && musicAudioDropDown!=NULL)
                                {
                                    musicAudioDropDown->set_selected_target(valString);
                                }
                                else if( keyString=="SceneStartAudio" && startAudioDropDown!=NULL)
                                {
                                    startAudioDropDown->set_selected_target(valString);
                                }
                                else if( keyString=="SceneEndAudio" && endAudioDropDown!=NULL)
                                {
                                    endAudioDropDown->set_selected_target(valString);
                                }
                                else if( keyString=="LevelWidth")
                                {
                                    foundNumberToRead = string_to_int(valString,4096);
                                    if( levelPixelWidthField!=NULL)
                                    {
                                        levelPixelWidthField->set_string( int_to_string(foundNumberToRead) );
                                    }
                                    sceneRect.w = foundNumberToRead;
                                }
                                else if( keyString=="LevelHeight")
                                {
                                    foundNumberToRead = string_to_int(valString,4096);
                                    if( levelPixelHeightField!=NULL)
                                    {
                                        levelPixelHeightField->set_string( int_to_string(foundNumberToRead) );
                                    }
                                    sceneRect.h = foundNumberToRead;
                                }
                                else if( keyString=="TileWidth")
                                {
                                    foundNumberToRead = string_to_int(valString,32);
                                    if( levelTileWidthField!=NULL)
                                    {
                                        levelTileWidthField->set_string(int_to_string(foundNumberToRead) );
                                    }
                                    lvlTileWidth= foundNumberToRead;
                                    lvlTileAmountX = ceil( std::abs( (float)sceneRect.w/lvlTileWidth) );
                                }
                                else if( keyString=="TileHeight")
                                {
                                    foundNumberToRead = string_to_int(valString,32);
                                    if( levelTileHeightField!=NULL)
                                    {
                                        levelTileHeightField->set_string(int_to_string(foundNumberToRead) );
                                    }
                                    lvlTileHeight = foundNumberToRead;
                                    lvlTileAmountY = ceil( std::abs( (float)sceneRect.h/lvlTileHeight) );
                                }
                                else if( keyString=="Preload" && preloadCheckBox!=NULL)
                                {
                                    preloadCheckBox->set_clicked( is_bool(valString) );
                                }
                                else if( keyString=="ViewTileGrid" && viewTileGridCheckBox!=NULL)
                                {
                                    viewTileGridCheckBox->set_clicked( is_bool(valString) );
                                }
                                else if(keyString=="Isometric" && isometricCheckBox!=NULL)
                                {
                                    isometricCheckBox->set_clicked( is_bool(valString) );
                                }
                                else if(keyString=="Continuous" && checkBoxIsContinuous!=NULL)
                                {
                                    checkBoxIsContinuous->set_clicked( is_bool(valString) );
                                }
                                else if(keyString=="BGColor" && sceneBackgroundColor!=NULL)
                                {
                                    sceneBackgroundColor->set_color_from_rgb( valString );
                                }
                                else if(keyString=="Background" || keyString=="SceneBackground")
                                {
                                    foundNumberToRead = split_first_int(valString,',');
                                    if( foundNumberToRead >=0  && foundNumberToRead < 8 )
                                    {
                                        foundBgId = split_first_int(valString,',');
                                        if( foundBgId >= 0 && defaultBackgroundLayer!=NULL )
                                        {
                                            newSceneBg = new GPE_SceneTexture(projectParentFolder);
                                            if( newSceneBg->backgroundInEditor!=NULL)
                                            {
                                                newSceneBg->backgroundInEditor->set_selection( foundBgId);
                                                newSceneBg->textureId = newSceneBg->backgroundInEditor->get_selected_id();
                                            }
                                            else
                                            {
                                                split_first_int(valString,',');
                                            }
                                            newSceneBg->bgPosXField->set_string(split_first_string(valString,','));
                                            newSceneBg->bgPosYField->set_string(split_first_string(valString,','));
                                            newSceneBg->bgHorSpeedField->set_string(split_first_string(valString,','));
                                            newSceneBg->bgVertSpeedField->set_string(split_first_string(valString,','));
                                            split_first_string(valString,','); //previously used for checkForeground
                                            newSceneBg->checkTileHori->set_clicked( is_bool( split_first_string(valString,',') ) );
                                            newSceneBg->checkTileVert->set_clicked( is_bool(split_first_string(valString,',') ) );
                                            newSceneBg->checkStretch->set_clicked( is_bool( split_first_string(valString,',') ) );
                                            newSceneBg->process_background_data();
                                            defaultBackgroundLayer->add_background(newSceneBg);
                                        }
                                        else
                                        {
                                         record_error("Background Detected out of range ["+int_to_string(foundBgId)+"]" );
                                        }
                                    }
                                    else
                                    {
                                        record_error("Invalid Scene ID "+int_to_string(foundNumberToRead)+"Given for "+resourceName+".");
                                    }
                                }
                                else if( keyString=="GameObject")
                                {
                                    /*if( foundGPEVersion==0.2)
                                    {
                                        foundNumberToRead = split_first_int(valString,',');
                                        if( foundNumberToRead> 0)
                                        {
                                            newGameObject = new GPE_SceneGameObject();
                                            newGameObject->objTypeId = foundNumberToRead;
                                            newGameObject->objRect.x = split_first_int(valString,',');
                                            newGameObject->objRect.y = split_first_int(valString,',');
                                            newGameObject->objAngle = split_first_int(valString,',');
                                            newGameObject->objXScale = split_first_int(valString,',');
                                            newGameObject->objYScale = split_first_int(valString,',');
                                            foundR = bound_number( split_first_int(valString,','), 0,255);
                                            foundG = bound_number( split_first_int(valString,','), 0,255);
                                            foundB = bound_number( split_first_int(valString,','), 0,255);
                                            newGameObject->objColor->change_rgba(foundR,foundG,foundB);
                                            newGameObject->objSubName >set_string(  split_first_string(valString,',') );
                                            newGameObject->customObjId = customObjCreationId;
                                            customObjCreationId++;
                                            sceneObjects.push_back(newGameObject);
                                        }
                                    }
                                    else*/
                                    {
                                        foundNumberToRead = split_first_int(valString,',');
                                        if( foundNumberToRead > 0)
                                        {
                                            newGameObject = new GPE_SceneGameObject();
                                            newGameObject->objTypeId = foundNumberToRead;
                                            if( allObjsFolder!=NULL)
                                            {
                                                foundObjContainer = allObjsFolder->find_resource_from_id(foundNumberToRead, true);
                                                if( foundObjContainer!=NULL)
                                                {
                                                    newGameObject->objTypeName = foundObjContainer->get_name();
                                                }
                                            }
                                            newGameObject->objRect.x = split_first_int(valString,',');
                                            newGameObject->objRect.y = split_first_int(valString,',');
                                            newGameObject->objAngle = split_first_int(valString,',');
                                            newGameObject->objAngleField->set_string(int_to_string(newGameObject->objAngle) );
                                            newGameObject->objXScale = split_first_int(valString,',');
                                            newGameObject->objYScale = split_first_int(valString,',');
                                            foundR = bound_number( split_first_int(valString,','), 0,255);
                                            foundG = bound_number( split_first_int(valString,','), 0,255);
                                            foundB = bound_number( split_first_int(valString,','), 0,255);
                                            newGameObject->objColor->set_rgb(foundR,foundG,foundB);
                                            newGameObject->objSubName->set_string( split_first_string(valString,",,,") );
                                            customObjAllFieldData = split_first_string(valString,"[/GPECustomFields]");

                                            customObjEntryTag = split_first_string(customObjAllFieldData,"[GPECustomFields]");
                                            while( (int)customObjAllFieldData.size() > 0)
                                            {
                                                subValString = split_first_string(customObjAllFieldData,"[/component]");

                                                beginObjectComponentPos=subValString.find_first_of(componentTag);
                                                if(beginObjectComponentPos!=(int)std::string::npos)
                                                {
                                                    //if the beginObjectComponentPos is present, then parse on through and carryon
                                                    objCustomValString = subValString.substr(beginObjectComponentPos+componentTagSize,subValString.length());
                                                    newObjectComponent = add_gui_component(objCustomValString);
                                                    if( newObjectComponent!=NULL)
                                                    {
                                                        newObjectComponent->set_max_width(192);
                                                        tempObjectComponent = newGameObject->add_variable(newObjectComponent);
                                                        if( tempObjectComponent==NULL)
                                                        {
                                                            //duplicate key added, therefore erase newly created gui element.
                                                            delete newObjectComponent;
                                                            newObjectComponent = NULL;
                                                        }
                                                    }
                                                }
                                            }
                                            newGameObject->customObjId = customObjCreationId;
                                            customObjCreationId++;
                                            if(defaultObjectLayer == NULL)
                                            {
                                                defaultObjectLayer = add_layer(LAYER_TYPE_OBJECTS,1);
                                            }
                                            if( defaultObjectLayer!=NULL)
                                            {
                                                defaultObjectLayer->layerObjects.push_back(newGameObject);
                                            }
                                            else
                                            {
                                                delete newGameObject;
                                                newGameObject = NULL;
                                            }
                                        }
                                    }
                                }
                                else if( keyString=="ObjectGridWidth")
                                {
                                    foundNumberToRead = string_to_int(valString,32);
                                    if( levelObjGridWidthField!=NULL)
                                    {
                                        levelObjGridWidthField->set_string(int_to_string(foundNumberToRead) );
                                    }
                                    objSnapX= foundNumberToRead;
                                }
                                else if( keyString=="ObjectGridHeight")
                                {
                                    foundNumberToRead = string_to_int(valString,32);
                                    if( levelObjGridHeightField!=NULL)
                                    {
                                        levelObjGridHeightField->set_string(int_to_string(foundNumberToRead) );
                                    }
                                    objSnapY = foundNumberToRead;
                                }
                                else if( keyString=="[GPE_Layer")
                                {
                                    foundLayerType = split_first_int(valString,',');
                                    foundLayerName = split_first_string(valString,',');
                                    if( (int)foundLayerName.size() > 0)
                                    {
                                        foundLayerId = string_to_int(foundLayerName, -1);
                                        if( foundLayerId >=0 && foundLayerId < 32)
                                        {
                                            newsceneLayer = add_layer( foundLayerType,  foundLayerId );
                                            if( newsceneLayer!=NULL)
                                            {
                                                tileLayerXPos = 0;
                                                tileLayerYPos = 0;
                                            }
                                            else
                                            {
                                                record_error("Unknown Layer Detected ["+foundLayerName+"] for "+resourceName+"...");
                                            }
                                        }
                                        else
                                        {
                                            record_error("Invalid Layer Detected ["+foundLayerName+"] for "+resourceName+"...");
                                        }
                                        /*
                                        else if( CURRENT_PROJECT!=NULL)
                                        {

                                        }
                                        */
                                    }
                                    /*else
                                    {
                                        newsceneLayer = find_layer( string_to_int(subValString) );
                                        if( newsceneLayer!=NULL)
                                        {
                                            if(newsceneLayer->layerType!= foundLayerType)
                                            {
                                                newsceneLayer = NULL;
                                            }
                                        }
                                    }*/

                                }
                                else if( keyString=="[sceneLayer" || keyString=="[SceneTileLayer")
                                {
                                    /*
                                    newSceneLayerWantedId = split_first_int(valString,']');
                                    newsceneLayer = find_layer(newSceneLayerWantedId);
                                    if( newsceneLayer==NULL)
                                    {
                                        newsceneLayer = add_layer(LAYER_TYPE_TILES,newSceneLayerWantedId);
                                    }
                                    tileLayerXPos = 0;
                                    tileLayerYPos = 0;
                                    */
                                    newsceneLayer = add_layer(LAYER_TYPE_TILES,-1);
                                    tileLayerXPos = 0;
                                    tileLayerYPos = 0;
                                }
                                if( newsceneLayer!=NULL)
                                {
                                    if( keyString=="GPEBackground" || keyString=="GPE_Background" && newsceneLayer->layerType==LAYER_TYPE_BACKGROUND )
                                    {
                                        foundBgName = split_first_string(valString,',');
                                        newSceneBg = new GPE_SceneTexture(projectParentFolder);
                                        if( newSceneBg->backgroundInEditor!=NULL)
                                        {
                                            newSceneBg->backgroundInEditor->set_selected_target( foundBgName );
                                            newSceneBg->textureId = newSceneBg->backgroundInEditor->get_selected_id();
                                        }
                                        else
                                        {
                                            split_first_int(valString,',');
                                        }
                                        newSceneBg->bgPosXField->set_string(split_first_string(valString,','));
                                        newSceneBg->bgPosYField->set_string(split_first_string(valString,','));
                                        newSceneBg->bgHorSpeedField->set_string(split_first_string(valString,','));
                                        newSceneBg->bgVertSpeedField->set_string(split_first_string(valString,','));
                                        newSceneBg->checkTileHori->set_clicked( is_bool( split_first_string(valString,',') ) );
                                        newSceneBg->checkTileVert->set_clicked( is_bool(split_first_string(valString,',') ) );
                                        newSceneBg->checkStretch->set_clicked( is_bool( split_first_string(valString,',') ) );
                                        newSceneBg->process_background_data();//ReturnToLater
                                        newsceneLayer->add_background(newSceneBg);
                                    }
                                    else if( keyString=="GPEObject" || keyString=="GPE_Object" && newsceneLayer->layerType == LAYER_TYPE_OBJECTS )
                                    {
                                        foundObjName = split_first_string(valString,',');
                                        newGameObject = new GPE_SceneGameObject();
                                        if( allObjsFolder!=NULL)
                                        {
                                            foundObjContainer = allObjsFolder->find_resource_target(foundObjName, true);
                                            if( foundObjContainer!=NULL )
                                            {
                                                newGameObject->objTypeName = foundObjContainer->get_name();
                                                newGameObject->objTypeId = foundObjContainer->get_global_id();
                                            }
                                            else
                                            {
                                                newGameObject->objTypeName = foundObjName;
                                                newGameObject->objTypeId = string_to_int(foundObjName, -1);
                                            }
                                        }
                                        else
                                        {
                                            newGameObject->objTypeName = foundObjName;
                                            newGameObject->objTypeId = string_to_int(foundObjName, -1);
                                            record_error("Unable to locate object["+foundObjName+"].");
                                        }
                                        newGameObject->objRect.x = split_first_int(valString,',');
                                        newGameObject->objRect.y = split_first_int(valString,',');
                                        newGameObject->objAngle = split_first_int(valString,',');
                                        newGameObject->objAngleField->set_string(int_to_string(newGameObject->objAngle) );
                                        newGameObject->objXScale = split_first_int(valString,',');
                                        newGameObject->objYScale = split_first_int(valString,',');
                                        foundR = bound_number( split_first_int(valString,','), 0,255);
                                        foundG = bound_number( split_first_int(valString,','), 0,255);
                                        foundB = bound_number( split_first_int(valString,','), 0,255);
                                        newGameObject->objColor->set_rgb(foundR,foundG,foundB);
                                        newGameObject->objSubName->set_string( split_first_string(valString,",,,") );
                                        customObjAllFieldData = split_first_string(valString,"[/GPECustomFields]");

                                        customObjEntryTag = split_first_string(customObjAllFieldData,"[GPECustomFields]");
                                        while( (int)customObjAllFieldData.size() > 0)
                                        {
                                            subValString = split_first_string(customObjAllFieldData,"[/component]");

                                            beginObjectComponentPos=subValString.find_first_of(componentTag);
                                            if(beginObjectComponentPos!=(int)std::string::npos)
                                            {
                                                //if the beginObjectComponentPos is present, then parse on through and carryon
                                                objCustomValString = subValString.substr(beginObjectComponentPos+componentTagSize,subValString.length());
                                                newObjectComponent = add_gui_component(objCustomValString);
                                                if( newObjectComponent!=NULL)
                                                {
                                                    newObjectComponent->set_max_width(192);
                                                    tempObjectComponent = newGameObject->add_variable(newObjectComponent);
                                                    if( tempObjectComponent==NULL)
                                                    {
                                                        //duplicate key added, therefore erase newly created gui element.
                                                        delete newObjectComponent;
                                                        newObjectComponent = NULL;
                                                    }
                                                }
                                            }
                                        }
                                        newGameObject->customObjId = customObjCreationId;
                                        customObjCreationId++;
                                        if( newsceneLayer!=NULL)
                                        {
                                            newsceneLayer->layerObjects.push_back(newGameObject);
                                        }
                                        else
                                        {
                                            record_error("Unable to add object["+foundObjName+"].");
                                        }
                                    }
                                }
                            }
                            else if( newsceneLayer!=NULL)
                            {
                                if( currLineToBeProcessed=="[/sceneLayer]")
                                {
                                    newsceneLayer = NULL;
                                    tileLayerXPos = 0;
                                    tileLayerYPos = 0;
                                }
                                else
                                {
                                    if( newsceneLayer->layerType==LAYER_TYPE_TILES)
                                    {
                                        tileLayerXPos = 0;
                                        commaPos=currLineToBeProcessed.find_first_of(",");
                                        while(commaPos!=(int)std::string::npos)
                                        {
                                            foundTilePos = tileLayerXPos+tileLayerYPos*lvlTileAmountX;
                                            commaPos=currLineToBeProcessed.find_first_of(",");
                                            valString = split_first_string(currLineToBeProcessed,',');
                                            if( (int)newsceneLayer->mapTiles.size() > foundTilePos && tileLayerXPos < lvlTileAmountX)
                                            {
                                                sceneTileToEdit = newsceneLayer->mapTiles.at(foundTilePos);
                                                if( sceneTileToEdit!=NULL)
                                                {
                                                    if( (int)valString.size() > 2)
                                                    {
                                                        sceneTileToEdit->tilesheetIndexId = split_first_int(valString,'-');
                                                        sceneTileToEdit->tileIndexId = string_to_int(valString);
                                                    }
                                                }
                                            }
                                            tileLayerXPos += 1;
                                        }
                                        tileLayerYPos+=1;
                                    }
                                }
                            }
                            else if( currLineToBeProcessed=="[SceneGameObjects]" && defaultObjectLayer == NULL )
                            {
                                defaultObjectLayer = add_layer(LAYER_TYPE_OBJECTS,1);
                            }
                        }
                        else if( newsceneLayer!=NULL)
                        {
                            newsceneLayer = NULL;
                            tileLayerXPos = 0;
                            tileLayerYPos = 0;
                        }
                    }
                    else
                    {
                        record_error("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }

            }
            gameScnFileIn.close();
        }
        scnPostProcessed = true;
        //record_error("Scene loaded successfully...");
    }
}


void gameSceneResource::inherit_components(GPE_SceneGameObject * objectToInherit,gameObjectResource * objParent)
{
    if( objParent!=NULL && objectToInherit!=NULL)
    {
        int i = 0;
        int j = 0;
        GPE_ObjectComponent * pComponent = NULL;
        GPE_ObjectComponent * myComponent = NULL;
        bool componentIsDuplicate = false;
        for( i = 0; i < (int)objParent->customComponentRealList.size(); i++)
        {
            pComponent = objParent->customComponentRealList[i];
            if( pComponent!=NULL)
            {
                componentIsDuplicate = false;
                for( j = objectToInherit->objCustomValPairs.size()-1; j>=0; j--)
                {
                    myComponent = objectToInherit->objCustomValPairs[j];
                    if( myComponent!=NULL)
                    {
                        if( pComponent->get_name()==myComponent->get_name() )
                        {
                            componentIsDuplicate = true;
                            j = -1;
                        }
                    }
                }
                if( !componentIsDuplicate)
                {
                    GPE_GeneralGuiElement * newInheritedGuiElement =add_gui_component( pComponent->get_data() );
                    GPE_ObjectComponent * newComponent = new GPE_ObjectComponent();
                    if( newComponent!=NULL)
                    {
                        newComponent->set_component(newInheritedGuiElement);
                        objectToInherit->objCustomValPairs.push_back( newComponent );
                    }
                }
            }
        }
    }
}


void gameSceneResource::manage_components(SDL_Rect *viewedSpace ,SDL_Rect *cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && layerPaneList!=NULL )
    {
        if( selectedSceneObject!=NULL)
        {
            layerPaneList->clear_list();
            if( selectedSceneObject->isBeingMoved)
            {
                objectEditorPropertiesTitle->set_name("Object Properties [Dragging]");
            }
            else
            {
                objectEditorPropertiesTitle->set_name("Object Properties[Regular]");
            }
            layerPaneList->set_maxed_out_width();
            layerPaneList->add_gui_element(objectEditorNameTitle,true);
            layerPaneList->add_gui_element(objectEditorPropertiesTitle,true);
            layerPaneList->add_gui_element(closeObjectProperties,true);
            layerPaneList->add_gui_element(inheritParentComponentButton,true);
            layerPaneList->add_gui_element(resetComponentsButton,true);
            layerPaneList->add_gui_element(selectedObjXPos,true);
            layerPaneList->add_gui_element(selectedObjYPos,true);
            layerPaneList->add_gui_element(objectNewType,true);
            layerPaneList->add_gui_element(selectedSceneObject->objAngleField,true);
            layerPaneList->add_gui_element(selectedSceneObject->objSubName,true);
            layerPaneList->add_gui_element(selectedSceneObject->objColor,true);
            GPE_ObjectComponent * tComponent = NULL;

            for( int icv = 0; icv < (int)selectedSceneObject->objCustomValPairs.size(); icv++)
            {
                tComponent = selectedSceneObject->objCustomValPairs.at(icv);
                if( tComponent!=NULL)
                {
                    if( tComponent->get_component()!=NULL && tComponent->get_gear()!=NULL)
                    {
                        layerPaneList->add_gui_element( tComponent->get_component(),false );
                        layerPaneList->add_gui_element( tComponent->get_gear(),true );
                    }
                }
            }
            layerPaneList->add_gui_element(addNewComponentDropDown,true);
            layerPaneList->process_self(viewedSpace,cam);

            //Add component to scene object
            if( addNewComponentDropDown!=NULL)
            {
                std::string newComponentType = addNewComponentDropDown->get_selected_tag();
                if( (int)newComponentType.size() > 0)
                {
                    std::string newComponentName = get_string_from_popup("Name your new component","Must be unique!","");
                    bool componentNameInUseAlready = false;
                    if( newComponentName.size() > 0 )
                    {
                        if( newComponentType=="labeltext" || is_alnum(newComponentName,false,true) )
                        {
                            GPE_ObjectComponent * myTempCustomComponent = NULL;
                            for( int iCompo = 0; iCompo < (int)selectedSceneObject->objCustomValPairs.size(); iCompo++)
                            {
                                myTempCustomComponent = selectedSceneObject->objCustomValPairs.at(iCompo);
                                if( myTempCustomComponent!=NULL)
                                {
                                    if( myTempCustomComponent->get_name()==newComponentName)
                                    {
                                        componentNameInUseAlready = true;
                                        break;
                                    }
                                }
                            }
                            if( !componentNameInUseAlready)
                            {
                                std::string newComponentData = newComponentType+":"+newComponentName+"==|||==";

                                std::string extraDataString = "_blank";
                                if( newComponentType=="texturl")
                                {
                                    std::string newurlLink = get_string_from_popup("Please enter a URL for this new component","Example: http://www.pawbyte.com","");
                                    std::string newUrlDescription = get_string_from_popup("Please enter a description of your URL","Please describe your URL","");
                                    if( (int)newurlLink.size() > 0)
                                    {
                                        int fileColonPos = newurlLink.find("file://");
                                        int httpPos = newurlLink.find("http://");
                                        int httpsPos = newurlLink.find("https://");
                                        if( fileColonPos==(int)std::string::npos && httpPos==(int)std::string::npos && httpsPos==(int)std::string::npos )
                                        {
                                            newurlLink = "http://"+newurlLink;
                                        }
                                    }
                                    extraDataString = newUrlDescription+",,,"+newurlLink+",,,";
                                }
                                else if( newComponentType=="dropdown" || newComponentType=="radio")
                                {
                                    extraDataString = "[menu]";
                                    std::string newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                    while( (int)newMenuOptionName.size() > 0 && (  is_alnum(newMenuOptionName,true,true) ) )
                                    {
                                        extraDataString+="[option]"+newMenuOptionName+":"+newMenuOptionName+":-1[/option]";
                                        newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                    }
                                    extraDataString+="[/menu]0,0";
                                }
                                else
                                {
                                    int dropdownResPos = newComponentType.find("-resourcedropdown");
                                    if( dropdownResPos!=(int)std::string::npos)
                                    {
                                        std::string newDropDownType = get_substring(newComponentType,0,dropdownResPos);
                                        newComponentData = "resourcedropdown:"+newComponentName+"==|||==";
                                        extraDataString = newDropDownType+",,,-1,";
                                    }
                                }
                                newComponentData = newComponentData+extraDataString;
                                GPE_GeneralGuiElement * newComponentField = add_gui_component(newComponentData);

                                if( newComponentField!=NULL)
                                {
                                    GPE_ObjectComponent * newComponent = new GPE_ObjectComponent();
                                    if( newComponent!=NULL)
                                    {
                                        newComponent->set_component( newComponentField );
                                        selectedSceneObject->objCustomValPairs.push_back(newComponent);
                                    }
                                }
                            }
                            else
                            {
                                display_user_alert("Invalid Component Name Entered","Please enter a unique component name with alpha-numeric values and no spaces!");
                            }
                        }
                        else
                        {
                            display_user_alert("Invalid Component Name Entered","Please enter a unique component name with alpha-numeric values and no spaces!");
                        }
                    }
                    addNewComponentDropDown->set_selection(-1,false);
                    userInput->reset_all_input();
                    manage_components();
                }
            }

        }
    }
}

void gameSceneResource::process_self(SDL_Rect * viewedSpace, SDL_Rect * cam)
{
    currentLayer = NULL;
    saveResourceButton->disable_self();
    preprocess_self();
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    int paneMode = 0;
    if( cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL && sceneXScroll!=NULL && sceneYScroll!=NULL && layerPaneList!=NULL )
    {
        editorPane.x = 0;
        editorPane.y = 0;
        editorPane.w = 256;
        editorPane.h = viewedSpace->h;

        editorView.x = editorPane.x+editorPane.w;
        editorView.y = 0;

        if( editorButtonBar!=NULL)
        {
            if( RESOURCE_TO_DRAG!=NULL)
            {
                if( RESOURCE_TO_DRAG->is_folder()==false && (RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_OBJECT || RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_TEXTURE || RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_TILESHEET ) && RESOURCE_TO_DRAG->projectParentFileName.compare(CURRENT_PROJECT_NAME)==0 )
                {
                    if( get_mouse_coords(viewedSpace, cam) && userInput->check_mouse_released(0) )
                    {
                        editorButtonBar->set_selection(LEDITOR_MODE_LAYERS);
                    }
                }
            }
            editorButtonBar->set_coords(editorPane.x,editorPane.y);
            editorButtonBar->set_width(96);
            if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
            {
                projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
                if( tProjectProps!=NULL && tProjectProps->sceneEditorPaneMode!=NULL)
                {
                    tProjectProps->sceneEditorPaneMode->set_coords(editorPane.x+editorPane.w-64,editorPane.y);
                    tProjectProps->sceneEditorPaneMode->set_width(64);
                    tProjectProps->sceneEditorPaneMode->barPadding = 0;
                    tProjectProps->sceneEditorPaneMode->xPadding = 0;
                    tProjectProps->sceneEditorPaneMode->set_height(24);
                    tProjectProps->sceneEditorPaneMode->process_self(viewedSpace,cam);
                    paneMode = tProjectProps->sceneEditorPaneMode->get_tab_pos();
                }
            }

            editorButtonBar->set_coords(editorPane.x,editorPane.y);
            editorButtonBar->set_width(64);
            //editorButtonBar->set_width(editorPane.w);
            int prevEditorMode = editorButtonBar->get_tab_pos();
            editorButtonBar->process_self(viewedSpace,cam);
            editorMode = editorButtonBar->get_tab_pos();

            editorPaneList->clear_list();
            layerPaneList->clear_list();
            if( prevEditorMode!=editorMode)
            {
                userInput->reset_all_input();
                if( unselect_object() )
                {
                    layerPaneList->process_self(viewedSpace,cam);
                }
                if( editorMode >=0 && editorMode < 5)
                {
                    sceneEditorSubTitle->set_name(DEFAULT_SCENE_SUBEDITOR_NAMES[editorMode]);
                }
                editorPaneList->reset_self();
                layerPaneList->reset_self();
            }

            if( sceneZoomLevel!=NULL)
            {
                sceneZoomLevel->set_coords(editorButtonBar->get_x2pos(),editorPane.y);
                sceneZoomLevel->process_self(viewedSpace,cam);
                sceneZoomAmount = sceneZoomLevel->get_selected_value();
                if( sceneZoomAmount < 0)
                {
                    sceneZoomAmount = 1;
                    sceneZoomLevel->set_value(100);
                }
                sceneZoomAmount/=100;
            }
            else
            {
                sceneZoomAmount = 1;
            }

            editorCommentPane.x = editorView.x;
            editorCommentPane.y = editorView.y+editorView.h+16;
            editorCommentPane.w = viewedSpace->w-editorView.x;
            editorCommentPane.h = 32;

            if( editorMode==LEDITOR_MODE_SETTINGS)
            {
                editorPaneList->set_height(viewedSpace->h-editorPaneList->get_ypos() );
                rightEditorPane.y = rightEditorPane.x = rightEditorPane.w = rightEditorPane.h = 0;
                layerPaneList->set_coords( 0, editorPaneList->get_y2pos() );
                layerPaneList->set_width( editorPaneList->get_width() );
                layerPaneList->set_height( viewedSpace->h-layerPaneList->get_ypos() );
                sceneEditorViewRect.w = editorView.w = (int)(viewedSpace->w-editorView.x-sceneYScroll->get_box_width() )-16; //xcroll = 16px height
            }
            else if( paneMode==1)
            {
                editorPaneList->set_height(160);
                rightEditorPane.x =  editorView.x+editorView.w+sceneYScroll->barBox.w;
                rightEditorPane.y = 0;
                rightEditorPane.w = viewedSpace->w-rightEditorPane.x;
                rightEditorPane.h = viewedSpace->h;
                layerPaneList->set_coords(0,editorButtonBar->get_y2pos() );
                layerPaneList->set_width(editorPane.w);
                layerPaneList->set_height(viewedSpace->h-layerPaneList->get_ypos() );

                layerPaneList->set_coords( rightEditorPane.x, rightEditorPane.y );
                layerPaneList->set_width( rightEditorPane.w );
                layerPaneList->set_height( rightEditorPane.h );
                sceneEditorViewRect.w = editorView.w = (int)(viewedSpace->w-editorView.x-rightPaneWidth-sceneYScroll->get_box_width() )-16; //xcroll = 16px height
            }
            else
            {
                editorPaneList->set_height(160);
                rightEditorPane.y = rightEditorPane.x = rightEditorPane.w = rightEditorPane.h = 0;
                layerPaneList->set_coords( 0, editorPaneList->get_y2pos() );
                layerPaneList->set_width( editorPaneList->get_width() );
                layerPaneList->set_height( viewedSpace->h-layerPaneList->get_ypos() );
                sceneEditorViewRect.w = editorView.w = (int)(viewedSpace->w-editorView.x-sceneYScroll->get_box_width() )-16; //xcroll = 16px height
            }

            sceneEditorViewRect.h = editorView.h = viewedSpace->h-64; //Comment pane = 32, yscroll = 16 height
            editorPaneList->set_coords(0,editorButtonBar->get_y2pos() );
            editorPaneList->barXPadding = 0;
            editorPaneList->barYPadding = 0;
            editorPaneList->barXMargin = 0;
            editorPaneList->barYMargin = 0;
            editorPaneList->set_width(editorPane.w);

            layerPaneList->barXPadding = GENERAL_GPE_PADDING;
            layerPaneList->barYPadding = 0;
            layerPaneList->barXMargin = 0;
            layerPaneList->barYMargin = 0;
            layerPaneList->set_horizontal_align(FA_LEFT);
            editorPaneList->clear_list();
            layerPaneList->clear_list();
            if( editorMode==LEDITOR_MODE_SETTINGS)
            {
                renameBox->enable_self();
                confirmResourceButton->enable_self();

                cancelResourceButton->enable_self();

                editorPaneList->barXPadding = GENERAL_GPE_PADDING;
                editorPaneList->barYPadding = GENERAL_GPE_PADDING;
                editorPaneList->barXMargin = 0;
                editorPaneList->barYMargin = 0;
                editorPaneList->add_gui_element(sceneEditorSubTitle,true);
                editorPaneList->add_gui_element(renameBox,true);
                editorPaneList->add_gui_element(sceneCaptionField,true);
                editorPaneList->add_gui_element(levelHintField,true);
                editorPaneList->add_gui_element(levelPixelWidthField,true);
                editorPaneList->add_gui_element(levelPixelHeightField,true);
                editorPaneList->add_gui_element(levelTileWidthField,true);
                editorPaneList->add_gui_element(levelTileHeightField,true);
                editorPaneList->add_gui_element(musicAudioDropDown,true);
                editorPaneList->add_gui_element(startAudioDropDown,true);
                editorPaneList->add_gui_element(endAudioDropDown,true);
                //editorPaneList->add_gui_element(isometricCheckBox,true);
                editorPaneList->add_gui_element(checkBoxIsContinuous,true);
                editorPaneList->add_gui_element(sceneBackgroundColor,true);
                editorPaneList->add_gui_element(confirmResourceButton,true);
                editorPaneList->add_gui_element(cancelResourceButton,true);
                editorPaneList->set_maxed_out_width();
                editorPaneList->process_self(viewedSpace,cam);
                //Scene and Tile Dimensions
                int prevLvlWidth = sceneRect.w;
                int prevLvlHeight = sceneRect.h;
                int prevTileWidth = lvlTileWidth;
                int prevTileHeight = lvlTileHeight;
                if( levelPixelWidthField!=NULL)
                {
                    if( !levelPixelWidthField->is_inuse() )
                    {
                        prevLvlWidth = levelPixelWidthField->get_held_number();
                    }
                }
                if( levelPixelHeightField!=NULL)
                {
                    if( !levelPixelHeightField->is_inuse() )
                    {
                        prevLvlHeight = levelPixelHeightField->get_held_number();
                    }
                }
                if( levelTileWidthField!=NULL)
                {
                    if( !levelTileWidthField->is_inuse() )
                    {
                        prevTileWidth = levelTileWidthField->get_held_number();
                    }
                }

                if( levelTileHeightField!=NULL)
                {
                    if( !levelTileHeightField->is_inuse() )
                    {
                        prevTileHeight = levelTileHeightField->get_held_number();
                    }
                }
                if( prevLvlWidth > 0 &&  prevLvlHeight > 0 && prevTileWidth > 0 && prevTileHeight > 0 && prevLvlWidth > prevTileWidth && prevLvlHeight > prevTileHeight)
                {
                    if(prevLvlWidth !=sceneRect.w ||  prevLvlHeight!=sceneRect.h || lvlTileWidth!=prevTileWidth || lvlTileHeight!=prevTileHeight )
                    {
                        bool changeSceneSizeApproved = false;
                        if( (int)sceneLayers.size() > 0 )
                        {
                            if( prevLvlWidth < sceneRect.w || prevLvlHeight < sceneRect.h )
                            {
                                if( display_get_prompt("Are you sure you want to change the scene dimensions?","Some tiles may be lost(Irreversible)!")==DISPLAY_QUERY_YES)
                                {
                                    changeSceneSizeApproved = true;
                                }
                            }
                            else
                            {
                                changeSceneSizeApproved = true;
                            }
                        }
                        else
                        {
                            changeSceneSizeApproved = true;
                        }
                        if( changeSceneSizeApproved)
                        {
                            sceneRect.w = prevLvlWidth;
                            sceneRect.h = prevLvlHeight;
                            lvlTileWidth = prevTileWidth;
                            lvlTileHeight = prevTileHeight;
                            lvlTileAmountX = ceil( (float)sceneRect.w/lvlTileWidth );
                            lvlTileAmountY = ceil( (float)sceneRect.h/lvlTileHeight );
                            sceneLayer * tempTileLayer = NULL;
                            for( int iTileLayer = 0; iTileLayer < (int)sceneLayers.size(); iTileLayer++)
                            {
                                tempTileLayer = sceneLayers.at(iTileLayer);
                                if( tempTileLayer!=NULL)
                                {
                                    tempTileLayer->resize_tilemap(lvlTileAmountX,lvlTileAmountY);
                                }
                            }
                            sceneEditorViewRect.x = 0;
                            sceneEditorViewRect.y = 0;
                        }
                        else
                        {
                            levelPixelWidthField->set_string( int_to_string(sceneRect.w) );
                            levelPixelHeightField->set_string( int_to_string(sceneRect.h) );
                            levelTileWidthField->set_string( int_to_string(lvlTileWidth) );
                            levelTileHeightField->set_string( int_to_string(lvlTileHeight) );
                        }
                    }
                }
            }
            else if( editorMode == LEDITOR_MODE_LAYERS)
            {
                layerPaneList->barXPadding = GENERAL_GPE_PADDING;
                layerPaneList->barXMargin = 0;
                layerPaneList->barYPadding = 0;
                layerPaneList->barYPadding = GENERAL_GPE_PADDING;
                layerPaneList->set_horizontal_align(FA_LEFT);
                layerPaneList->clear_list();


                //editorPaneList->add_gui_element(viewTileGridCheckBox,true);
                editorPaneList->add_gui_element(layerMenu,true);
                editorPaneList->add_gui_element(layerSettingsButtton,false);
                editorPaneList->add_gui_element(layerAddButtton,false);
                editorPaneList->add_gui_element(layerRemoveButton,false);
                editorPaneList->add_gui_element(layerMoveUpButtton,false);
                editorPaneList->add_gui_element(layerMoveDownButton,false);
                editorPaneList->add_gui_element(layerToggleHideButton,true);


                int previousTileSheetId = tilesheetDropDown->get_selected_id();
                //editorPaneList->set_maxed_out_width();
                editorPaneList->process_self(viewedSpace,cam);
                if( layerMenu!=NULL )
                {
                    layerInEdit = layerMenu->get_selection();
                    if( layerInEdit >= 0 && layerInEdit < (int)sceneLayers.size() )
                    {
                        currentLayer =sceneLayers[layerInEdit];
                    }

                    if( ( layerAddButtton!=NULL && layerAddButtton->is_clicked() )  || ( layerSettingsButtton!=NULL && layerSettingsButtton->is_clicked() ) )
                    {

                        std::vector< int > availableLayerIds;
                        int availI = 0;
                        int sLayerJ = 0;
                        bool layerWasUsed = false;
                        for( availI = 0; availI < 32; availI++)
                        {
                            availableLayerIds.push_back(availI);
                        }
                        for( availI = (int)(availableLayerIds.size() )-1; availI >=0; availI--)
                        {
                            layerWasUsed = false;
                            for( sLayerJ = (int)(sceneLayers.size() )-1; sLayerJ >=0; sLayerJ--)
                            {
                                if( availableLayerIds[availI]==sceneLayers[sLayerJ]->projectLayerId )
                                {
                                    layerWasUsed = true;

                                }
                            }
                            if( layerWasUsed )
                            {
                                availableLayerIds.erase(availableLayerIds.begin()+availI);
                            }
                        }
                        int availableLayerSize = (int)(availableLayerIds.size() );
                        if( availableLayerSize > 0)
                        {
                            if( layerAddButtton!=NULL && layerAddButtton->is_clicked() )
                            {
                                GPE_open_context_menu();
                                MAIN_CONTEXT_MENU->set_width(256);
                                GPE_PopUpMenu_Option * tileLayerOption = MAIN_CONTEXT_MENU->add_menu_option("Add Tile Layer",LAYER_TYPE_TILES,NULL,-1,NULL,false,true);
                                int foundNewLayerType = 0;
                                for( availI =0; availI < (int)(availableLayerIds.size() ); availI++)
                                {
                                    tileLayerOption->add_menu_option(CURRENT_PROJECT->projectLayerNames[ availableLayerIds[availI] ],availableLayerIds[availI]+100);
                                }
                                GPE_PopUpMenu_Option * objectLayerOption = MAIN_CONTEXT_MENU->add_menu_option("Add Object Layer",LAYER_TYPE_OBJECTS,NULL,-1,NULL,false,true);
                                for( availI =0; availI < (int)(availableLayerIds.size() ); availI++)
                                {
                                    objectLayerOption->add_menu_option(CURRENT_PROJECT->projectLayerNames[ availableLayerIds[availI] ],availableLayerIds[availI]+200);
                                }
                                GPE_PopUpMenu_Option * bgLayerOption = MAIN_CONTEXT_MENU->add_menu_option("Add Background Layer ",LAYER_TYPE_BACKGROUND,NULL,-1,NULL,false,true);
                                for( availI =0; availI < (int)(availableLayerIds.size() ); availI++)
                                {
                                    bgLayerOption->add_menu_option(CURRENT_PROJECT->projectLayerNames[ availableLayerIds[availI] ],availableLayerIds[availI]+300);
                                }
                                int menuSelection = get_popupmenu_result();
                                if( menuSelection>=100 && menuSelection <=332 )
                                {
                                    if( menuSelection >=300)
                                    {
                                        menuSelection-=300;
                                        foundNewLayerType = LAYER_TYPE_BACKGROUND;
                                    }
                                    else if( menuSelection >= 200)
                                    {
                                        menuSelection-=200;
                                        foundNewLayerType = LAYER_TYPE_OBJECTS;
                                    }
                                    else if( menuSelection >= 100)
                                    {
                                        menuSelection-=100;
                                        foundNewLayerType = LAYER_TYPE_TILES;
                                    }
                                    std::string newLayerName = CURRENT_PROJECT->projectLayerNames[menuSelection];
                                    if( find_layer(menuSelection)==NULL )
                                    {
                                        sceneLayer* newSceneLayer = add_layer(foundNewLayerType,menuSelection);
                                        if( foundNewLayerType==LAYER_TYPE_TILES)
                                        {
                                            newSceneLayer->create_new_map(sceneRect.w/lvlTileWidth,sceneRect.h/lvlTileHeight,0);
                                        }
                                        else if( foundNewLayerType==LAYER_TYPE_BACKGROUND)
                                        {

                                        }
                                        else if( foundNewLayerType==LAYER_TYPE_OBJECTS)
                                        {

                                        }
                                        else if( foundNewLayerType==LAYER_TYPE_RESOURCES)
                                        {

                                        }
                                    }
                                }
                            }
                            else if( layerSettingsButtton!=NULL && layerSettingsButtton->is_clicked() )
                            {
                                GPE_open_context_menu();
                                MAIN_CONTEXT_MENU->set_width(256);
                                GPE_PopUpMenu_Option * tileLayerOption = MAIN_CONTEXT_MENU->add_menu_option("Change Layer",0,NULL,-1,NULL,false,true);
                                int foundNewLayerType = 0;
                                for( availI =0; availI < (int)(availableLayerIds.size() ); availI++)
                                {
                                    tileLayerOption->add_menu_option(CURRENT_PROJECT->projectLayerNames[ availableLayerIds[availI] ],availableLayerIds[availI]+100);
                                }
                                MAIN_CONTEXT_MENU->add_menu_option("Rename Layer in Matrix",1,NULL,-1,NULL,false,true);
                                int menuSelection = get_popupmenu_result();
                                if( menuSelection > 100)
                                {
                                    while( menuSelection>=100 )
                                    {
                                        menuSelection-=100;
                                    }
                                    std::string newLayerName = CURRENT_PROJECT->projectLayerNames[menuSelection];
                                    currentLayer->projectLayerId = menuSelection;
                                    GPE_SelectBoxBasic_Option * currentLayerOption = layerMenu->get_option( layerMenu->get_selection() );
                                    if( currentLayerOption!=NULL)
                                    {
                                        currentLayerOption->optionName = newLayerName;
                                        currentLayerOption->optionValue = menuSelection;
                                    }
                                }
                                else if( menuSelection==1 && currentLayer!=NULL && currentLayer->projectLayerId >=0 && currentLayer->projectLayerId <32 )
                                {
                                    GPE_SelectBoxBasic_Option * currentLayerOption = layerMenu->get_option( currentLayer->projectLayerId );
                                    if( currentLayerOption!=NULL)
                                    {
                                        std::string newLayerName =get_string_from_popup("Rename Layer["+CURRENT_PROJECT->projectLayerNames[ currentLayer->projectLayerId ]+"] Globally?","This will change the name of this layer in the entire project...",CURRENT_PROJECT->projectLayerNames[ currentLayer->projectLayerId ] );
                                        if( (int)newLayerName.size() > 0 && newLayerName!=CURRENT_PROJECT->projectLayerNames[ currentLayer->projectLayerId ] )
                                        {
                                            if( is_alnum(newLayerName,true,true) )
                                            {
                                                bool oldLayerNameMatch = false;
                                                for( int oldI = 0; oldI < 32; oldI++)
                                                {
                                                    if( CURRENT_PROJECT->projectLayerNames[ oldI ]==newLayerName)
                                                    {
                                                        oldLayerNameMatch = true;
                                                    }
                                                }
                                                if( !oldLayerNameMatch)
                                                {
                                                    CURRENT_PROJECT->projectLayerNames[ currentLayer->projectLayerId ] = newLayerName;
                                                    if( currentLayerOption!=NULL)
                                                    {
                                                        currentLayerOption->optionName = newLayerName;
                                                    }
                                                    GPE_ResourceContainer * projectSettingsRSC = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_PROJECT_SETTINGS] );
                                                    if( projectSettingsRSC!=NULL)
                                                    {
                                                        generalGameResource * foundProjectSettingsRSC = projectSettingsRSC->get_held_resource();
                                                        if( foundProjectSettingsRSC!=NULL)
                                                        {
                                                            projectPropertiesResource * tcProjectSettingsRSC= (projectPropertiesResource*)foundProjectSettingsRSC;
                                                            tcProjectSettingsRSC->update_project_layers();
                                                        }
                                                    }
                                                }
                                            }
                                            else if( GPE_Main_Logs!=NULL)
                                            {
                                                GPE_Main_Logs->log_general_line("Non-alphanumeric name given for layer rename operation...");
                                            }
                                        }
                                        else if( GPE_Main_Logs!=NULL)
                                        {
                                            GPE_Main_Logs->log_general_line("Layer name not changed...");
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if( layerRemoveButton!=NULL)
                    {
                        if( layerRemoveButton->is_clicked() )
                        {
                            int tileLayerToRemove = layerMenu->get_selection();
                            if( tileLayerToRemove >=0 && tileLayerToRemove < layerMenu->get_size() )
                            {
                                if( display_get_prompt("Are you sure you want to remove this layer?","Your tiles will be lost and deleted, like forever and such!")==DISPLAY_QUERY_YES)
                                {
                                    layerMenu->remove_option( layerMenu->get_selection() );
                                    if( (int)sceneLayers.size() > tileLayerToRemove)
                                    {
                                        sceneLayer * tlToDelete = sceneLayers.at( tileLayerToRemove);
                                        if(tlToDelete!=NULL )
                                        {
                                            delete tlToDelete;
                                            tlToDelete = NULL;
                                            sceneLayers.erase(sceneLayers.begin() + tileLayerToRemove);
                                            if( layerInEdit >=(int)sceneLayers.size() )
                                            {
                                                layerInEdit = (int)sceneLayers.size()-1;
                                            }
                                            if( layerInEdit < 0 )
                                            {
                                                layerInEdit = 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if( layerMoveUpButtton!=NULL && layerMenu!=NULL)
                    {
                        if( layerMoveUpButtton->is_clicked() )
                        {
                            int tLayerSel = layerMenu->get_selection();
                            if( layerMenu->move_up_space() )
                            {
                                if( tLayerSel > 0 && (int)sceneLayers.size() > tLayerSel-1)
                                {
                                    sceneLayer* tempVar =sceneLayers[tLayerSel-1];
                                    sceneLayers[tLayerSel-1] = sceneLayers[tLayerSel];
                                    sceneLayers[tLayerSel] = tempVar;
                                }
                            }
                        }
                    }

                    if( layerMoveDownButton!=NULL)
                    {
                        if( layerMoveDownButton->is_clicked() )
                        {
                            int tLayerSel = layerMenu->get_selection();
                            if( layerMenu->move_down_space() )
                            {
                                if( tLayerSel +1 < (int)sceneLayers.size() )
                                {
                                    sceneLayer* tempVar =sceneLayers[tLayerSel+1];
                                    sceneLayers[tLayerSel+1] = sceneLayers[tLayerSel];
                                    sceneLayers[tLayerSel] = tempVar;
                                }
                            }
                        }
                    }

                    if( layerToggleHideButton!=NULL)
                    {
                        if( layerToggleHideButton->is_clicked() )
                        {
                            layerMenu->toggle_others_checked();
                        }
                    }

                    if( currentLayer!=NULL)
                    {
                        if( currentLayer->layerType == LAYER_TYPE_TILES )
                        {
                            layerPaneList->clear_list();
                            layerPaneList->barYMargin = 0;
                            layerPaneList->barYPadding = 0;
                            layerPaneList->barYMargin = 0;
                            layerPaneList->set_height(48);
                            layerPaneList->add_gui_element(tilesheetDropDown,true);
                            layerPaneList->process_self(viewedSpace, cam);
                            tilesheetRenderYPos = layerPaneList->get_y2pos()+GENERAL_GPE_PADDING;
                            if( tilesheetDropDown!=NULL)
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
                                //Place Tiles and Such
                                tSPreviewer->tileSheetToPreview = NULL;
                                GPE_ResourceContainer * tsTypeContainer = tilesheetDropDown->get_selected_container();
                                if( tsTypeContainer!=NULL)
                                {
                                    tsRes = (tilesheetResource*) tsTypeContainer->get_held_resource();
                                    if( tsRes!=NULL  )
                                    {
                                        if( tsRes->tilesheetInEditor!=NULL )
                                        {
                                            if( tsRes->tilesheetInEditor->tsImage!=NULL)
                                            {
                                                tsRenderRect.w =tsRes->tilesheetInEditor->tsImage->get_width();
                                                tsRenderRect.h = tsRes->tilesheetInEditor->tsImage->get_height();
                                                if( tsRes->tilesheetInEditor->tsImage->get_width() > editorPane.w-tilesheetRenderXPos-16)
                                                {
                                                    tsRenderRect.w = editorPane.w-tilesheetRenderXPos-16;
                                                }
                                                if( tsRes->tilesheetInEditor->tsImage->get_height() > editorPane.h-tilesheetRenderYPos-16)
                                                {
                                                    tsRenderRect.h = editorPane.h-tilesheetRenderYPos-16;
                                                }
                                                tSPreviewer->set_coords(rightEditorPane.x,tilesheetRenderYPos);
                                                tSPreviewer->set_width(editorPane.w);
                                                tSPreviewer->set_height(editorPane.h-tilesheetRenderYPos);
                                                tSPreviewer->tileSheetToPreview =tsRes->tilesheetInEditor;
                                                tSPreviewer->process_self(viewedSpace,cam);

                                                //select tiles to place / place tiles / add tiles
                                                /*
                                                */
                                                if( RESOURCE_TO_DRAG==NULL && get_mouse_coords(viewedSpace,cam) && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
                                                {
                                                    if( (int)tSPreviewer->tilesIdsInPreview.size()>0 && layerInEdit>=0 && layerInEdit < (int)sceneLayers.size() )
                                                    {
                                                        if( userInput->check_mouse_down(0) )
                                                        {
                                                            sceneLayer* cLayerToPlaceOn = sceneLayers.at(layerInEdit);
                                                            GPE_SceneTile* fSceneTileToEdit = NULL;
                                                            if( cLayerToPlaceOn!=NULL)
                                                            {
                                                                int sceneTileMouseX = sceneMouseXPos/lvlTileWidth;
                                                                int sceneTileMouseY = sceneMouseYPos/lvlTileHeight;
                                                                int tileRowItr = 0;
                                                                int tilesItr= 0;
                                                                int newTileX = 0, newTileY = 0;
                                                                for( tilesItr = 0; tilesItr < (int)tSPreviewer->tilesIdsInPreview.size(); tilesItr++)
                                                                {
                                                                    fSceneTileToEdit = cLayerToPlaceOn->get_tile_at(sceneTileMouseX+newTileX,sceneTileMouseY+newTileY);
                                                                    if( fSceneTileToEdit!=NULL)
                                                                    {
                                                                        fSceneTileToEdit->tileIndexId = tSPreviewer->tilesIdsInPreview.at(tilesItr);
                                                                        fSceneTileToEdit->tilesheetIndexId = tilesheetDropDown->get_selected_id();
                                                                        fSceneTileToEdit->tileTypeId = 1;
                                                                    }
                                                                    else
                                                                    {
                                                                    //    GPE_MAIN_GUI->update_tooltip("Unable to find scene tile to edit...");
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
                                                            else
                                                            {
                                                                GPE_MAIN_GUI->update_tooltip("Unknown scene layer to edit...");
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    tsRes = NULL;
                                }
                            }

                            //Remove Tiles / Delete Tiles
                            if( get_mouse_coords(viewedSpace,cam) && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
                            {
                                if( userInput->check_mouse_down(1) && layerInEdit >=0 && (int)sceneLayers.size() > 0 )
                                {
                                    sceneLayer* cLayerToPlaceOn = sceneLayers.at(layerInEdit);
                                    GPE_SceneTile* fSceneTileToEdit = NULL;
                                    if( cLayerToPlaceOn!=NULL)
                                    {
                                        int sceneTileMouseX = (double)sceneMouseXPos/lvlTileWidth;
                                        int sceneTileMouseY = (double)sceneMouseYPos/lvlTileHeight;

                                        fSceneTileToEdit = cLayerToPlaceOn->get_tile_at(sceneTileMouseX,sceneTileMouseY);
                                        if( fSceneTileToEdit!=NULL)
                                        {
                                            fSceneTileToEdit->tileIndexId = -1;
                                            fSceneTileToEdit->tilesheetIndexId = -1;
                                            fSceneTileToEdit->tileTypeId = 1;
                                        }
                                    }
                                 }
                            }
                        }
                        else if( currentLayer->layerType == LAYER_TYPE_BACKGROUND  )
                        {
                            layerPaneList->clear_list();
                            layerPaneList->barYMargin = 0;
                            layerPaneList->barYPadding = 0;
                            layerPaneList->barYMargin = 0;
                            layerPaneList->set_maxed_out_width();
                            GPE_SceneTexture * tBg = NULL;
                            tBg = NULL;
                            if( currentLayer->layerBGSelectBox!=NULL)
                            {
                                currentLayer->layerBGSelectBox->set_height(120);
                                currentLayer->layerBGSelectBox->limit_height(120);
                            }
                            layerPaneList->add_gui_element(currentLayer->layerBGSelectBox,true);
                            layerPaneList->add_gui_element(layerBackgroundSettingsButtton,false);
                            layerPaneList->add_gui_element(layerBackgroundAddButtton,false);
                            layerPaneList->add_gui_element(layerBackgroundRemoveButton,false);
                            layerPaneList->add_gui_element(layerBackgroundMoveUpButtton,false);
                            layerPaneList->add_gui_element(layerBackgroundMoveDownButton,true);
                            //layerPaneList->add_gui_element(layerBackgroundToggleHideButton,true);

                            if( currentLayer->layerBGSelectBox!=NULL )
                            {
                                int selectedBGPos = currentLayer->layerBGSelectBox->get_selection() ;
                                if( selectedBGPos >=0 && selectedBGPos < (int)currentLayer->layerBackgrounds.size() )
                                {
                                    tBg = currentLayer->layerBackgrounds.at(selectedBGPos);
                                    if( tBg!=NULL )
                                    {
                                        layerPaneList->add_gui_element(tBg->backgroundInEditor,true);
                                        layerPaneList->add_gui_element(tBg->bgPosXField,true);
                                        layerPaneList->add_gui_element(tBg->bgPosYField,true);
                                        layerPaneList->add_gui_element(tBg->bgHorSpeedField,true);
                                        layerPaneList->add_gui_element(tBg->bgVertSpeedField,true);
                                        layerPaneList->add_gui_element(tBg->checkTileHori,true);
                                        layerPaneList->add_gui_element(tBg->checkTileVert,true);
                                        layerPaneList->add_gui_element(tBg->checkStretch,true);
                                        //layerPaneList->set_maxed_out_width();

                                    }
                                }
                                layerPaneList->process_self(viewedSpace,cam);
                                if( layerBackgroundAddButtton!=NULL && layerBackgroundAddButtton->is_clicked() )
                                {
                                    currentLayer->add_background( new GPE_SceneTexture(projectParentFolder) );
                                }
                            }
                            for( int layerBgItr = 0; layerBgItr < (int)currentLayer->layerBackgrounds.size(); layerBgItr++)
                            {
                                tBg = currentLayer->layerBackgrounds.at(layerBgItr);
                                if( tBg!=NULL )
                                {
                                    tBg->process_background_data();
                                }
                            }
                        }
                        else if( currentLayer->layerType == LAYER_TYPE_OBJECTS)
                        {
                            objSnapX = std::max( 8, levelObjGridWidthField->get_held_number() );
                            objSnapY = std::max( 8, levelObjGridHeightField->get_held_number() );

                            objSnapX = std::min( 512, levelObjGridWidthField->get_held_number() );
                            objSnapY = std::min( 512, levelObjGridHeightField->get_held_number() );

                            int objTypeBeingPlaced = objectInEditor->get_selected_id();

                            if( useObjGridCheckBox!=NULL)
                            {
                                showObjLines = useObjGridCheckBox->is_clicked();
                            }

                            if( selectedSceneObject!=NULL)
                            {
                                manage_components(viewedSpace, cam);

                                GPE_ObjectComponent * tempCustomComponent = NULL;
                                GPE_ObjectComponent * otherTempCustomComponent = NULL;
                                int iRealCompo = 0;
                                int jRealCompo = 0;
                                for(  iRealCompo = (int)selectedSceneObject->objCustomValPairs.size()-1; iRealCompo>=0; iRealCompo--)
                                {
                                    tempCustomComponent = selectedSceneObject->objCustomValPairs.at(iRealCompo);
                                    if( tempCustomComponent!=NULL)
                                    {
                                        if(tempCustomComponent->get_gear()!=NULL)
                                        {
                                            if( tempCustomComponent->get_gear()->is_clicked() )
                                            {
                                                GPE_open_context_menu();
                                                MAIN_CONTEXT_MENU->set_width(256);
                                                MAIN_CONTEXT_MENU->add_menu_option("Rename Component",0,NULL,-1,NULL,true,true);
                                                MAIN_CONTEXT_MENU->add_menu_option("Delete Component",1,NULL,-1,NULL,false,true);
                                                if( tempCustomComponent->get_type()=="dropdown" || tempCustomComponent->get_type()=="selectbox")
                                                {
                                                    MAIN_CONTEXT_MENU->add_menu_option("Add Option",2,NULL,-1,NULL,false,true);
                                                    MAIN_CONTEXT_MENU->add_menu_option("Remove Option",3,NULL,-1,NULL,false,true);
                                                }
                                                MAIN_CONTEXT_MENU->add_menu_option("Exit Menu",4,NULL,-1,NULL,true,true);
                                                int menuSelection = get_popupmenu_result();
                                                if( menuSelection==0)
                                                {
                                                    //rename
                                                    std::string newComponentName = get_string_from_popup("Name your new component","Must be unique!","");
                                                    bool componentNameInUseAlready = false;
                                                    if( newComponentName.size() > 0 )
                                                    {
                                                        if( tempCustomComponent->get_type()=="labeltext" || is_alnum(newComponentName,false,true) )
                                                        {
                                                            for(  jRealCompo = 0; jRealCompo < (int)selectedSceneObject->objCustomValPairs.size(); jRealCompo++)
                                                            {
                                                                if( jRealCompo!=iRealCompo)
                                                                {
                                                                    otherTempCustomComponent = selectedSceneObject->objCustomValPairs.at(jRealCompo);
                                                                    if( otherTempCustomComponent!=NULL)
                                                                    {
                                                                        if( otherTempCustomComponent->get_name() ==newComponentName )
                                                                        {
                                                                            componentNameInUseAlready = true;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                            if( !componentNameInUseAlready)
                                                            {
                                                                tempCustomComponent->rename_component( newComponentName );
                                                            }
                                                        }
                                                    }
                                                }
                                                else if( menuSelection==1)
                                                {
                                                    if( display_get_prompt("[WARNING]Object Component deletion?","Are you sure you will like to delete the ["+tempCustomComponent->get_name()+"] component? This action is irreversible!")==DISPLAY_QUERY_YES)
                                                    {
                                                        //delete
                                                        delete tempCustomComponent;
                                                        tempCustomComponent = NULL;
                                                        selectedSceneObject->objCustomValPairs.erase( selectedSceneObject->objCustomValPairs.begin()+iRealCompo);
                                                    }
                                                }
                                                else if( menuSelection==2)
                                                {
                                                    if( tempCustomComponent->get_type()=="dropdown" || tempCustomComponent->get_type()=="selectbox")
                                                    {
                                                        std::string extraDataString = "[menu]";
                                                        std::string newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                                        while( newMenuOptionName.size() > 0 && (  is_alnum(newMenuOptionName,true,true) ) )
                                                        {
                                                            extraDataString+="[option]"+newMenuOptionName+":"+newMenuOptionName+":-1[/option]";
                                                            newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                                        }
                                                        extraDataString+="[/menu]0,0";
                                                        tempCustomComponent->load_data(extraDataString);
                                                    }
                                                }
                                                else if( menuSelection==3)
                                                {
                                                    if( tempCustomComponent->get_type()=="dropdown" || tempCustomComponent->get_type()=="selectbox")
                                                    {
                                                        std::string extraDataString = "";
                                                        std::string newMenuOptionName = get_string_from_popup("Enter option to remove.","option name:","");
                                                        if( newMenuOptionName.size() > 0 && (  is_alnum(newMenuOptionName,true,true) ) )
                                                        {
                                                            extraDataString+="[option]"+newMenuOptionName+"[/option]";
                                                            tempCustomComponent->remove_data(extraDataString);
                                                        }
                                                    }
                                                }
                                                userInput->reset_all_input();
                                                manage_components(viewedSpace, cam);
                                                break;
                                            }
                                        }
                                    }
                                }

                                if( get_mouse_coords(viewedSpace,cam) )
                                {
                                    int sceneObjMouseX  = sceneMouseXPos;
                                    int sceneObjMouseY  = sceneMouseYPos;

                                    if( showObjLines && objSnapX!=0 && objSnapY!=0)
                                    {
                                        sceneObjMouseX = sceneMouseXPos/objSnapX;
                                        sceneObjMouseX = sceneObjMouseX * objSnapX;
                                        sceneObjMouseY = sceneMouseYPos/objSnapY;
                                        sceneObjMouseY = sceneObjMouseY * objSnapY;
                                    }
                                    if( selectedSceneObject->isBeingMoved)
                                    {
                                        if( userInput->check_mouse_released(0) )
                                        {
                                            isDraggingObject = false;
                                            selectedSceneObject->isBeingMoved = false;
                                        }
                                        else
                                        {
                                            selectedSceneObject->objRect.x = sceneObjMouseX;
                                            selectedSceneObject->objRect.y = sceneObjMouseY;
                                            selectedObjXPos->set_number(selectedSceneObject->objRect.x);
                                            selectedObjYPos->set_number(selectedSceneObject->objRect.y);
                                            isDraggingObject = true;
                                            selectedSceneObject->isBeingMoved = true;
                                        }
                                    }
                                    else if( userInput->check_mouse_down(0) )
                                    {
                                        if( point_within_rect(sceneMouseXPos,sceneMouseYPos,&selectedSceneObject->objRect) )
                                        {
                                            isDraggingObject = true;
                                            selectedSceneObject->isBeingMoved = true;
                                            selectedSceneObject->objRect.x = sceneObjMouseX;
                                            selectedSceneObject->objRect.y = sceneObjMouseY;
                                            selectedObjXPos->set_number(selectedSceneObject->objRect.x);
                                            selectedObjYPos->set_number(selectedSceneObject->objRect.y);
                                        }
                                    }
                                }
                                else
                                {
                                    isDraggingObject = false;
                                    if( selectedObjXPos!=NULL && selectedObjYPos!=NULL && selectedSceneObject!=NULL )
                                    {
                                        if( selectedObjXPos->is_valid() )
                                        {
                                            selectedSceneObject->objRect.x =selectedObjXPos->get_held_number();
                                        }
                                        if( selectedObjYPos->is_valid() )
                                        {
                                            selectedSceneObject->objRect.y =selectedObjYPos->get_held_number();
                                        }
                                    }

                                    if( closeObjectProperties!=NULL && closeObjectProperties->is_clicked() )
                                    {
                                        if( unselect_object() )
                                        {
                                            layerPaneList->process_self(viewedSpace,cam);
                                        }
                                    }
                                    else if( inheritParentComponentButton!=NULL &&  inheritParentComponentButton->is_clicked() )
                                    {
                                        GPE_ResourceContainer * foundObjContainer = projectParentFolder->find_resource_from_id(selectedSceneObject->objTypeId);
                                        if( foundObjContainer!=NULL)
                                        {
                                            gameObjectResource* selectedObjectClass = (gameObjectResource*)foundObjContainer->get_held_resource();
                                            if( selectedObjectClass!=NULL)
                                            {
                                                inherit_components(selectedSceneObject,selectedObjectClass);
                                                manage_components(viewedSpace,cam);
                                            }
                                        }
                                    }
                                    else if( objectNewType!=NULL && objectNewType->just_activated() )
                                    {
                                        GPE_ResourceContainer * cObj = objectNewType->get_selected_container();
                                        if( cObj!=NULL)
                                        {
                                            selectedSceneObject->objTypeId = cObj->get_global_id();
                                            selectedSceneObject->objTypeName = cObj->get_name();
                                            if( objectEditorNameTitle!=NULL)
                                            {
                                               objectEditorNameTitle ->set_name( "["+selectedSceneObject->objTypeName+"]" );
                                            }
                                        }
                                        objectNewType->set_name("Change Object");
                                        objectNewType->set_selection(-1);
                                    }
                                    else if( resetComponentsButton!=NULL && resetComponentsButton->is_clicked() )
                                    {
                                        int selectedObjComponentCount= (int)selectedSceneObject->objCustomValPairs.size();
                                        if( selectedObjComponentCount >0)
                                        {
                                            if( display_get_prompt("[WARNING]Object Component deletion?","Are you sure you will like to delete ["+int_to_string(selectedObjComponentCount)+" components] from this object? This action is irreversible!")==DISPLAY_QUERY_YES)
                                            {
                                                selectedSceneObject->reset_components();
                                                manage_components(viewedSpace,cam);
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                layerPaneList->set_maxed_out_width();
                                layerPaneList->add_gui_element(objectInEditor,true);
                                layerPaneList->add_gui_element(objectLeftClickModeButton,true);
                                layerPaneList->add_gui_element(onlyRemoveThisObjectTypeButton,true);
                                layerPaneList->add_gui_element(levelObjGridWidthField,true);

                                layerPaneList->add_gui_element(levelObjGridHeightField,true);
                                layerPaneList->add_gui_element(useObjGridCheckBox,true);
                                layerPaneList->add_gui_element(forceSnapButton,true);
                                layerPaneList->add_gui_element(objectReplacerInEditor,true);
                                layerPaneList->add_gui_element(removeObjectButton,true);
                                layerPaneList->add_gui_element(clearObjectsButton,true);
                                layerPaneList->process_self(viewedSpace,cam);

                                //Object Snapping
                                if( forceSnapButton!=NULL)
                                {
                                    if( forceSnapButton->is_clicked() )
                                    {
                                        if( objSnapX >0 && objSnapY > 0)
                                        {
                                            if( display_get_prompt("Are you sure?","All object coordinates will change and this is irreversible!")==DISPLAY_QUERY_YES)
                                            {
                                                GPE_SceneGameObject * tempObjToSnap = NULL;
                                                for( int iNewSnapObj = 0; iNewSnapObj < (int)currentLayer->layerObjects.size(); iNewSnapObj++)
                                                {
                                                    tempObjToSnap = currentLayer->layerObjects.at(iNewSnapObj);
                                                    if( tempObjToSnap!=NULL)
                                                    {
                                                        tempObjToSnap->objRect.x=(int)tempObjToSnap->objRect.x/objSnapX;
                                                        tempObjToSnap->objRect.x*=objSnapX;
                                                        tempObjToSnap->objRect.y=(int)tempObjToSnap->objRect.y/objSnapY;
                                                        tempObjToSnap->objRect.y*=objSnapY;
                                                    }
                                                }
                                                //display_user_alert("Scene Updated","Objects snapped to grid.");
                                            }
                                        }
                                        else
                                        {
                                                display_user_alert("Error!","Invalid Grid Given!");

                                        }
                                    }
                                }

                                //Clearing Objects
                                if( clearObjectsButton!=NULL && clearObjectsButton->is_clicked() )
                                {
                                    if( display_get_prompt("Are you sure?","All object will be deleted and this is irreversible!")==DISPLAY_QUERY_YES)
                                    {
                                        currentLayer->clear_all_objects();
                                    }
                                }
                            }

                            if( objectInEditor!=NULL )
                            {
                                if( removeObjectButton!=NULL && objectInEditor->get_selected_container()!=NULL )
                                {
                                    if( removeObjectButton->is_clicked() )
                                    {
                                        if( display_get_prompt("Are you sure?","All ["+objectInEditor->get_selected_container()->get_name()+"] objects will be deleted and this is irreversible!")==DISPLAY_QUERY_YES)
                                        {
                                            currentLayer->clear_objects(objectInEditor->get_selected_id());
                                        }
                                    }
                                }

                                if( get_mouse_coords(viewedSpace,cam) )
                                {
                                    if( !isDraggingObject)
                                    {
                                        //delete objscts
                                        if( userInput->check_mouse_down(1)  && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false)
                                        {
                                            objectLeftClickModeButton->set_clicked(false);
                                            GPE_SceneGameObject * tempObjToRemove = NULL;
                                            GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
                                            GPE_ResourceContainer * tempObjTypeContainer = NULL;
                                            GPE_ResourceContainer * allSprsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SPRITE]+"s");
                                            GPE_ResourceContainer * tempSprTypeContainer = NULL;
                                            gameObjectResource * tempObjPointer = NULL;
                                            spriteResource * tempSprPointer = NULL;
                                            int tempSpriteId;
                                            bool foundValidSprite = false;
                                            for( int objRemoveItr = (int)currentLayer->layerObjects.size()-1; objRemoveItr >=0; objRemoveItr--)
                                            {
                                                tempObjToRemove = currentLayer->layerObjects.at(objRemoveItr);
                                                foundValidSprite = false;
                                                if( tempObjToRemove!=NULL)
                                                {
                                                    tempObjToRemove->objRect.w = 32;
                                                    tempObjToRemove->objRect.h = 32;
                                                    if( tempObjToRemove->objTypeId > 0)
                                                    {
                                                        tempObjTypeContainer = allObjsFolder->find_resource_from_id(tempObjToRemove->objTypeId);
                                                        //gets the object's sprite
                                                        if( tempObjTypeContainer!=NULL)
                                                        {
                                                            tempObjPointer = (gameObjectResource*)tempObjTypeContainer->get_held_resource();
                                                            if( tempObjPointer!=NULL)
                                                            {
                                                                tempSpriteId = tempObjPointer->spriteIndex;
                                                                if( tempSpriteId > 0)
                                                                {
                                                                    tempSprTypeContainer = allSprsFolder->find_resource_from_id(tempSpriteId);
                                                                    if( tempSprTypeContainer!=NULL)
                                                                    {
                                                                        tempSprPointer = (spriteResource*)tempSprTypeContainer->get_held_resource();
                                                                        if( tempSprPointer!=NULL)
                                                                        {
                                                                            if( tempSprPointer->spriteInEditor!=NULL)
                                                                            {
                                                                                if( tempSprPointer->spriteInEditor->width > 0 && tempSprPointer->spriteInEditor->height > 0)
                                                                                {
                                                                                    tempObjToRemove->objRect.w = tempSprPointer->spriteInEditor->width;
                                                                                    tempObjToRemove->objRect.h = tempSprPointer->spriteInEditor->height;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if( point_within_rect(sceneMouseXPos,sceneMouseYPos,&tempObjToRemove->objRect) )
                                                    {
                                                        if( (onlyRemoveThisObjectTypeButton->is_clicked() && tempObjToRemove->objTypeId==objectInEditor->get_selected_id()) || (onlyRemoveThisObjectTypeButton->is_clicked()==false) )
                                                        {
                                                            if( unselect_object() )
                                                            {
                                                                layerPaneList->process_self(viewedSpace,cam);
                                                            }
                                                            delete tempObjToRemove;
                                                            tempObjToRemove= NULL;
                                                            currentLayer->layerObjects.erase(currentLayer->layerObjects.begin() + objRemoveItr);
                                                            reset_placement_info();
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                            reset_placement_info();
                                        }
                                        else if( RESOURCE_TO_DRAG!=NULL)
                                        {
                                            if( RESOURCE_TO_DRAG->is_folder()==false && RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_OBJECT && RESOURCE_TO_DRAG->projectParentFileName.compare(CURRENT_PROJECT_NAME)==0 )
                                            {
                                                if( userInput->check_mouse_released(0) )
                                                {
                                                    GPE_MAIN_GUI->update_tooltip("Placing Object");
                                                    GPE_SceneGameObject * newPlacedObject = new GPE_SceneGameObject();
                                                    if( showObjLines)
                                                    {
                                                        if( objSnapX >0)
                                                        {
                                                            int sceneObjMouseX = sceneMouseXPos/objSnapX;
                                                            sceneMouseXPos = sceneObjMouseX * objSnapX;
                                                        }
                                                        if( objSnapY > 0)
                                                        {
                                                            int sceneObjMouseY = sceneMouseYPos/objSnapY;
                                                            sceneMouseYPos = sceneObjMouseY * objSnapY;
                                                        }
                                                    }
                                                    newPlacedObject->objRect.x = sceneMouseXPos;
                                                    newPlacedObject->objRect.y = sceneMouseYPos;
                                                    newPlacedObject->objRect.w = 32;
                                                    newPlacedObject->objRect.h = 32;
                                                    newPlacedObject->objTypeId = RESOURCE_TO_DRAG->get_global_id();
                                                    newPlacedObject->objTypeName = RESOURCE_TO_DRAG->get_name();
                                                    currentLayer->layerObjects.push_back(newPlacedObject);
                                                    lastCreatedObjXPos=sceneMouseXPos;
                                                    lastCreatedObjYPos=sceneMouseYPos;
                                                    lastCreatedObjTypeId=RESOURCE_TO_DRAG->get_global_id();
                                                    selectedSceneObject = NULL;
                                                    if( objectEditorNameTitle!=NULL)
                                                    {
                                                       objectEditorNameTitle ->set_name( "None");
                                                    }
                                                    //objectInEditor->set_selection(RESOURCE_TO_DRAG->get_global_id() );

                                                    gameObjectResource* newObjRes = (gameObjectResource*)RESOURCE_TO_DRAG->get_held_resource();
                                                    if( newObjRes!=NULL)
                                                    {
                                                        inherit_components(newPlacedObject,newObjRes);
                                                    }
                                                    newPlacedObject->customObjId = customObjCreationId;
                                                    customObjCreationId++;
                                                    selectedObjXPos->set_string(int_to_string( newPlacedObject->objRect.x) );
                                                    selectedObjYPos->set_string(int_to_string( newPlacedObject->objRect.y) );
                                                    RESOURCE_TO_DRAG = NULL;
                                                }
                                            }
                                            else
                                            {
                                                record_error("Unable to place dragged resource ["+RESOURCE_TO_DRAG->projectParentFileName+"] with ["+CURRENT_PROJECT_NAME+"]...");
                                            }
                                        }
                                        else if( objectLeftClickModeButton->is_clicked() || objTypeBeingPlaced<0 )
                                        {
                                            if( userInput->check_mouse_down(0) && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
                                            {
                                                GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
                                                GPE_ResourceContainer * tempObjTypeContainer = NULL;
                                                GPE_ResourceContainer * allSprsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SPRITE]+"s");
                                                GPE_ResourceContainer * tempSprTypeContainer = NULL;
                                                gameObjectResource * tempObjPointer = NULL;
                                                spriteResource * tempSprPointer = NULL;
                                                int tempSpriteId;
                                                bool foundValidSprite = false;
                                                GPE_SceneGameObject * tempObjToSelect = NULL;
                                                for( int objRemoveItr = (int)currentLayer->layerObjects.size()-1; objRemoveItr >=0; objRemoveItr--)
                                                {
                                                    tempObjToSelect = currentLayer->layerObjects.at(objRemoveItr);
                                                    foundValidSprite = false;
                                                    if( tempObjToSelect!=NULL )
                                                    {
                                                        tempObjToSelect->objRect.w = 32;
                                                        tempObjToSelect->objRect.h = 32;
                                                        if( tempObjToSelect->objTypeId > 0)
                                                        {
                                                            tempObjTypeContainer = allObjsFolder->find_resource_from_id(tempObjToSelect->objTypeId);
                                                            if( tempObjTypeContainer!=NULL)
                                                            {
                                                                tempObjPointer = (gameObjectResource*)tempObjTypeContainer->get_held_resource();
                                                                if( tempObjPointer!=NULL)
                                                                {
                                                                    tempSpriteId = tempObjPointer->spriteIndex;
                                                                    if( tempSpriteId > 0)
                                                                    {
                                                                        tempSprTypeContainer = allSprsFolder->find_resource_from_id(tempSpriteId);
                                                                        if( tempSprTypeContainer!=NULL)
                                                                        {
                                                                            tempSprPointer = (spriteResource*)tempSprTypeContainer->get_held_resource();
                                                                            if( tempSprPointer!=NULL)
                                                                            {
                                                                                if( tempSprPointer->spriteInEditor!=NULL)
                                                                                {
                                                                                    if( tempSprPointer->spriteInEditor->width > 0 && tempSprPointer->spriteInEditor->height > 0)
                                                                                    {
                                                                                        tempObjToSelect->objRect.w = tempSprPointer->spriteInEditor->width;
                                                                                        tempObjToSelect->objRect.h = tempSprPointer->spriteInEditor->height;
                                                                                        foundValidSprite = true;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if( RESOURCE_TO_DRAG==NULL )
                                                        {
                                                            if( point_within_rect(sceneMouseXPos,sceneMouseYPos,&tempObjToSelect->objRect) )
                                                            {
                                                                if( (onlyRemoveThisObjectTypeButton->is_clicked() && tempObjToSelect->objTypeId==objectInEditor->get_selected_id() ) || (onlyRemoveThisObjectTypeButton->is_clicked()==false) )
                                                                {
                                                                    select_object(tempObjToSelect);
                                                                    GPE_ResourceContainer * foundSelectedObjContainer = projectParentFolder->find_resource_from_id(selectedSceneObject->objTypeId);
                                                                    if( objectEditorNameTitle!=NULL)
                                                                    {
                                                                        if( foundSelectedObjContainer!=NULL)
                                                                        {
                                                                            objectEditorNameTitle->set_name( "["+foundSelectedObjContainer->get_name()+"]" );
                                                                        }
                                                                        else if( (int)selectedSceneObject->objTypeName.size() > 0 )
                                                                        {
                                                                           objectEditorNameTitle->set_name( "{Unknown["+selectedSceneObject->objTypeName+"] }" );
                                                                        }
                                                                        else
                                                                        {
                                                                            objectEditorNameTitle->set_name( "{Unknown Objects["+int_to_string(selectedSceneObject->objTypeId)+"] }" );
                                                                        }
                                                                    }

                                                                    selectedObjXPos->set_string(int_to_string( tempObjToSelect->objRect.x) );
                                                                    selectedObjYPos->set_string(int_to_string( tempObjToSelect->objRect.y) );
                                                                }
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        //removes null objects
                                                        currentLayer->layerObjects.erase( currentLayer->layerObjects.begin()+objRemoveItr );
                                                    }
                                                }
                                            }
                                        }
                                        else if( objTypeBeingPlaced> 0)
                                        {
                                            if( showObjLines)
                                            {
                                                if( objSnapX >0)
                                                {
                                                    int sceneObjMouseX = sceneMouseXPos/objSnapX;
                                                    sceneMouseXPos = sceneObjMouseX * objSnapX;
                                                }
                                                if( objSnapY > 0)
                                                {
                                                    int sceneObjMouseY = sceneMouseYPos/objSnapY;
                                                    sceneMouseYPos = sceneObjMouseY * objSnapY;
                                                }
                                            }
                                            if( userInput->check_mouse_down(0) )
                                            {
                                                if( (lastCreatedObjXPos < 0 || lastCreatedObjYPos < 0 || lastCreatedObjTypeId < 0) || sceneMouseXPos!=lastCreatedObjXPos && lastCreatedObjXPos|| sceneMouseYPos!=lastCreatedObjYPos || lastCreatedObjTypeId!=objTypeBeingPlaced )
                                                {
                                                    GPE_MAIN_GUI->update_tooltip("Placing Object");
                                                    GPE_SceneGameObject * newPlacedObject = new GPE_SceneGameObject();
                                                    newPlacedObject->objRect.x = sceneMouseXPos;
                                                    newPlacedObject->objRect.y = sceneMouseYPos;
                                                    newPlacedObject->objRect.w = 32;
                                                    newPlacedObject->objRect.h = 32;
                                                    newPlacedObject->objTypeId = objTypeBeingPlaced;

                                                    currentLayer->layerObjects.push_back(newPlacedObject);
                                                    lastCreatedObjXPos=sceneMouseXPos;
                                                    lastCreatedObjYPos=sceneMouseYPos;
                                                    lastCreatedObjTypeId=objTypeBeingPlaced;
                                                    selectedSceneObject = NULL;
                                                    if( objectEditorNameTitle!=NULL)
                                                    {
                                                       objectEditorNameTitle ->set_name( "None");
                                                    }
                                                    GPE_ResourceContainer * objContainer = objectInEditor->get_selected_container();
                                                    if( objContainer!=NULL)
                                                    {
                                                        gameObjectResource* newObjRes = (gameObjectResource*)objContainer->get_held_resource();
                                                        if( newObjRes!=NULL)
                                                        {
                                                            newPlacedObject->objTypeName = newObjRes->get_name();
                                                            inherit_components(newPlacedObject,newObjRes);
                                                        }
                                                    }
                                                    newPlacedObject->customObjId = customObjCreationId;
                                                    customObjCreationId++;
                                                    selectedObjXPos->set_string(int_to_string( newPlacedObject->objRect.x) );
                                                    selectedObjYPos->set_string(int_to_string( newPlacedObject->objRect.y) );
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            //
                            if( objectReplacerInEditor!=NULL && objectReplacerInEditor->just_activated() )
                            {
                                //checks if the collision event already exists
                                if( objectInEditor!=NULL )
                                {
                                    GPE_ResourceContainer * cObj = objectInEditor->get_selected_container();
                                    GPE_ResourceContainer * rObj = objectReplacerInEditor->get_selected_container();
                                    if( rObj!=NULL && cObj!=NULL)
                                    {
                                        if( display_get_prompt("[Warning]Object Replacing","Are you sure you want to replace all ["+objectInEditor->get_name()+"] objects with ["+objectReplacerInEditor->get_name()+"] objects? This action is irreversible!")==DISPLAY_QUERY_YES )
                                        {
                                            GPE_SceneGameObject * tObj = NULL;
                                            for( int i = 0; i < (int)currentLayer->layerObjects.size(); i++)
                                            {
                                                tObj = currentLayer->layerObjects[i];
                                                if( tObj!=NULL)
                                                {
                                                    if( tObj->objTypeId==cObj->get_global_id() )
                                                    {
                                                        tObj->objTypeId= rObj->get_global_id();
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                objectReplacerInEditor->set_name("Replace Object");
                                objectReplacerInEditor->set_selection(-1);
                            }

                            if( selectedSceneObject!=NULL && RESOURCE_TO_DRAG==NULL )
                            {
                                if( addNewComponentDropDown!=NULL && addNewComponentDropDown->is_clicked() )
                                {
                                    std::string newComponentType = addNewComponentDropDown->get_selected_tag();
                                    if( (int)newComponentType.size() > 0)
                                    {
                                        std::string newComponentName = get_string_from_popup("Name your new component","Must be unique!","");
                                        bool componentNameInUseAlready = false;
                                        if( newComponentName.size() > 0 && ( (newComponentType=="labeltext") || is_alnum(newComponentName,false,true) ) )
                                        {
                                            std::string newComponentData = newComponentType+":"+newComponentName+"==|||==";
                                            std::string extraDataString = "_blank";
                                            if( newComponentType=="texturl")
                                            {
                                                std::string newurlLink = get_string_from_popup("Please enter a URL for this new component","Example: http://www.pawbyte.com","");
                                                std::string newUrlDescription = get_string_from_popup("Please enter a description of your URL","Please describe your URL","");
                                                if( (int)newurlLink.size() > 0)
                                                {
                                                    int fileColonPos = newurlLink.find("file://");
                                                    int httpPos = newurlLink.find("http://");
                                                    int httpsPos = newurlLink.find("https://");
                                                    if( fileColonPos==(int)std::string::npos && httpPos==(int)std::string::npos && httpsPos==(int)std::string::npos )
                                                    {
                                                        newurlLink = "http://"+newurlLink;
                                                    }
                                                }
                                                extraDataString = newUrlDescription+",,,"+newurlLink+",,,";
                                            }
                                            else if( newComponentType=="dropdown" || newComponentType=="radio")
                                            {
                                                extraDataString = "[menu]";
                                                std::string newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                                while( (int)newMenuOptionName.size() > 0 && (  is_alnum(newMenuOptionName,true,true) ) )
                                                {
                                                    extraDataString+="[option]"+newMenuOptionName+":"+newMenuOptionName+":-1[/option]";
                                                    newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                                }
                                                extraDataString+="[/menu]0,0";
                                            }
                                            else
                                            {
                                                int dropdownResPos = newComponentType.find("-resourcedropdown");
                                                if( dropdownResPos!=(int)std::string::npos)
                                                {
                                                    std::string newDropDownType = get_substring(newComponentType,0,dropdownResPos);
                                                    newComponentData = "resourcedropdown:"+newComponentName+"==|||==";
                                                    extraDataString = newDropDownType+",,,-1,";
                                                }
                                            }
                                            newComponentData = newComponentData+extraDataString;
                                            GPE_GeneralGuiElement * newComponentField = add_gui_component(newComponentData);

                                            if( newComponentField!=NULL)
                                            {
                                                newComponentField->set_max_width(192);
                                                GPE_ObjectComponent * tempObjectComponent = selectedSceneObject->add_variable(newComponentField);
                                                if( tempObjectComponent==NULL)
                                                {
                                                    delete newComponentField;
                                                    newComponentField = NULL;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            display_user_alert("Invalid Component Name Entered","Please enter a unique component name with alpha-numeric values and no spaces!");
                                        }
                                    }
                                    addNewComponentDropDown->set_selection(-1,false);
                                }

                                if( objCustomVariableSettingsButtton!=NULL && objCustomVariableSettingsButtton->is_clicked() )
                                {
                                        GPE_open_context_menu();
                                        MAIN_CONTEXT_MENU->set_width(256);
                                        MAIN_CONTEXT_MENU->add_menu_option("Clear Object Properties",0,NULL,-1,NULL,true,true);
                                        MAIN_CONTEXT_MENU->add_menu_option("Load Object Properties",1,NULL,-1,NULL,false,true);
                                        MAIN_CONTEXT_MENU->add_menu_option("Save Object Properties",1,NULL,-1,NULL,false,true);
                                        MAIN_CONTEXT_MENU->add_menu_option("Copy Object Properties",2,NULL,-1,NULL,false,true);
                                        MAIN_CONTEXT_MENU->add_menu_option("Paste Object Properties",3,NULL,-1,NULL,false,true);
                                        int menuSelection = get_popupmenu_result();
                                }

                                if( userInput->check_keyboard_released(kb_esc) )
                                {
                                    if( unselect_object() )
                                    {
                                        layerPaneList->process_self(viewedSpace,cam);
                                    }
                                }
                            }
                        }
                        else
                        {
                            layerPaneList->clear_list();
                            layerPaneList->barYMargin = 0;
                            layerPaneList->barYPadding = 0;
                            layerPaneList->barYMargin = 0;
                            layerPaneList->set_maxed_out_width();
                            layerPaneList->add_gui_element(layerErrorMessage, true );
                            layerPaneList->process_self(viewedSpace, cam);
                            //means that something is simply broken in editor code here
                        }
                    }
                }
            }
        }

        //if( editorPaneList->hasScrollControl==false && layerPaneList->hasScrollControl==false )
        {
            //Horizontal scrolling
            update_rectangle(&sceneXScroll->barBox,editorView.x,editorView.y+editorView.h,editorView.w,16);
            update_rectangle(&sceneXScroll->fullRect,0,0,(double)sceneRect.w, (double)sceneRect.h );
            update_rectangle(&sceneXScroll->contextRect,(double)sceneEditorViewRect.x,(double)sceneEditorViewRect.y, (double)sceneEditorViewRect.w/sceneZoomAmount, (double)sceneEditorViewRect.h/sceneZoomAmount );
            sceneXScroll->process_self(viewedSpace,cam, true );
            //if( sceneXScroll->has_moved() )
            {
                sceneEditorViewRect.x = (double)(sceneXScroll->contextRect.x);
            }

            //Vertical Scrolling
            update_rectangle(&sceneYScroll->barBox,editorView.x+editorView.w,editorView.y,16,editorView.h);
            update_rectangle(&sceneYScroll->fullRect,0,0,(double)sceneRect.w, (double)sceneRect.h );
            update_rectangle(&sceneYScroll->contextRect,(double)sceneEditorViewRect.x,(double)sceneEditorViewRect.y, (double)sceneEditorViewRect.w/sceneZoomAmount, (double)sceneEditorViewRect.h/sceneZoomAmount );
            //sceneYScroll->contextRect.h = editorView.h;
            sceneYScroll->process_self(viewedSpace,cam, true );
            //if( sceneYScroll->has_moved() )
            {
                sceneEditorViewRect.y = double(sceneYScroll->contextRect.y);
            }
        }

        //if( userInput->input_received() )
        {
            get_mouse_coords(viewedSpace,cam);
        }
        //if( userInput->check_mouse_pressed(0) ||  userInput->check_mouse_pressed(1) || userInput->check_mouse_pressed(2) )
        {
            if( get_mouse_coords(viewedSpace,cam) )
            {
                sceneAreaScrollable = true;
                editorPaneList->hasScrollControl = false;
            }
            else
            {
                sceneAreaScrollable = false;
            }
        }

        if( userInput->check_mouse_down(0) )
        {
            if( get_mouse_coords(viewedSpace,cam) )
            {
                if( sceneMouseXPos-sceneEditorViewRect.x < 0 )
                {
                    sceneEditorViewRect.x = 0;
                }

                if( sceneMouseYPos-sceneEditorViewRect.y < 0 )
                {
                    sceneEditorViewRect.y = 0;
                }
            }
            /*if(sceneYScroll->is_held() )
            {
                sceneEditorViewRect.x+=16;
            }
            if(sceneXScroll->is_held() )
            {
                //sceneEditorViewRect.y+=16;
            }*/
        }
        handle_scrolling();
    }
}

void gameSceneResource::render_scene_layers(GPE_Renderer * cRender,SDL_Rect * viewedSpace, SDL_Rect * cam, GPE_Rect * renderZone, GPE_Rect * sceneCamera,double renderScale, bool showEditorPreviews,  bool checkListDependent, bool forceRedraw )
{
    if( forceRedraw)
    {
        if( renderScale <=0 )
        {
            renderScale = sceneZoomAmount;
        }
        if( renderZone==NULL)
        {
            renderZone = &editorView;
        }
        if( sceneCamera==NULL)
        {
            sceneCamera = &sceneEditorViewRect;
        }
        GPE_ResourceContainer * allTilesheetsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TILESHEET]+"s");
        int iItr = 0, jItr = 0;
        double i = 0,j = 0, k = 0;
        //renders the tile layers onto the screen
        sceneLayer* cTempLayer = NULL;
        GPE_SceneTile* fSceneTile = NULL;
        GPE_SceneTexture * currBg = NULL;
        SDL_Rect tempBgRect = {0,0,32,32};
        GPE_ResourceContainer * texTypeContainer = NULL;

        GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
        GPE_ResourceContainer * fObjToPlace = NULL;

        GPE_ResourceContainer * allTexturesFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE]+"s");
        GPE_SceneGameObject * cGGO = NULL;
        SDL_Rect tempObjRect = {0,0,32,32};
        GPE_ResourceContainer * objTypeContainer = NULL;
        GPE_ResourceContainer * sprTypeContainer = NULL;
        GPE_ResourceContainer * allSpritesFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SPRITE]+"s");
        gameObjectResource * tempGameObj = NULL;
        spriteResource * tempGameSpr = NULL;
        bool objectSpriteRender = false;
        double fObjAngle = 0;

        int foundTilePos = 0;
        int tileXStartPos = std::max(0,(int)(floor(sceneCamera->x/lvlTileWidth))-1 );
        if( tileXStartPos < 0)
        {
            tileXStartPos = 0;
        }
        int tileYStartPos = std::max(0,(int)(floor(sceneCamera->y/lvlTileHeight))-1 );
        if( tileYStartPos < 0)
        {
            tileYStartPos = 0;
        }
        int tileXEndPos = std::min((int)lvlTileAmountX,2+(int)(ceil( (sceneCamera->w/renderScale)/lvlTileWidth) ) );
        tileXEndPos = std::max(0, tileXEndPos)+tileXStartPos;

        int tileYEndPos = std::min((int)lvlTileAmountY,2+(int)(ceil( (sceneCamera->h/renderScale)/lvlTileHeight) ) );
        tileYEndPos = std::max(0, tileYEndPos)+tileYStartPos;

        /*if( userInput->check_keyboard_down(kb_ctrl) )
        {
            if( userInput->check_keyboard_released(kb_r) )
            {
                record_error("Editor View:("+int_to_string(sceneCamera->x)+","+int_to_string(sceneCamera->y)+","+int_to_string(sceneCamera->x+sceneCamera.w)+","+int_to_string(sceneCamera->y+sceneCamera.h)+")");
                record_error("Tiles View:("+int_to_string(tileXStartPos)+","+int_to_string(tileYStartPos)+","+int_to_string(tileXEndPos)+","+int_to_string(tileYEndPos)+")");
            }
        }
        */
        SDL_Rect foundTsRect;
        int cTileXPos = 0;
        int cTileYPos = 0;
        int currentSceneBGCount = 0;
        GPE_ResourceContainer * foundTSResource = NULL;
        tilesheetResource * foundHeldTSRes = NULL;
        GPE_SelectBoxBasic_Option * tSelectOption = NULL;
        for(  iItr = 0; iItr < (int)sceneLayers.size();iItr++)
        {
            cTempLayer = sceneLayers[iItr];
            tSelectOption = layerMenu->get_option(iItr);
            if( cTempLayer!=NULL && tSelectOption!=NULL)
            {
                if( (checkListDependent && tSelectOption->isChecked ) || !checkListDependent )
                {
                    if( cTempLayer->layerType==LAYER_TYPE_TILES )
                    {
                        for(  j = tileXStartPos; j >=0 && j <= tileXEndPos && j < lvlTileAmountX; j++)
                        {
                            for(  k = tileYStartPos; k>=0 && k <= tileYEndPos && k < lvlTileAmountY; k++)
                            {
                                cTileXPos = floor( (j*lvlTileWidth*renderScale )+renderZone->x-sceneCamera->x*renderScale );
                                cTileYPos = floor( (k*lvlTileHeight*renderScale)+renderZone->y-sceneCamera->y*renderScale );

                                fSceneTile = cTempLayer->get_tile_at(j,k);
                                if( fSceneTile!=NULL)
                                {
                                    if( fSceneTile->tilesheetIndexId>=0 && fSceneTile->tileIndexId>=0)
                                    {
                                        foundTSResource = allTilesheetsFolder->find_resource_from_id(fSceneTile->tilesheetIndexId);
                                        if( foundTSResource!=NULL)
                                        {
                                            foundHeldTSRes = (tilesheetResource * )foundTSResource->get_held_resource();
                                            if( foundHeldTSRes->tilesheetInEditor!=NULL)
                                            {
                                                if( foundHeldTSRes->tilesheetInEditor->tsImage!=NULL && fSceneTile->tileIndexId < (int)foundHeldTSRes->tilesheetInEditor->tsRects.size() )
                                                {
                                                    foundTsRect = foundHeldTSRes->tilesheetInEditor->tsRects.at(fSceneTile->tileIndexId );
                                                    //if( check_collision(sceneEditorViewRect,cTileXPos,cTileYPos,foundTsRect.w,foundTsRect.h) )
                                                    {
                                                        foundHeldTSRes->tilesheetInEditor->tsImage->render_tex_resized(cRender,cTileXPos,cTileYPos, ceil( (double)foundTsRect.w*renderScale),ceil( (double)foundTsRect.h*renderScale ),&foundTsRect );
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if( cTempLayer->layerType==LAYER_TYPE_BACKGROUND )
                    {
                        currentSceneBGCount = (int)cTempLayer->layerBackgrounds.size();
                        if( currentSceneBGCount > 0)
                        {
                            for(  jItr = 0; jItr < currentSceneBGCount;jItr++)
                            {
                                currBg = cTempLayer->layerBackgrounds.at(jItr);
                                if( currBg!=NULL )
                                {
                                    if( currBg->textureId > 0 )
                                    {
                                        texTypeContainer = allTexturesFolder->find_resource_from_id(currBg->textureId );
                                        if( texTypeContainer!=NULL)
                                        {
                                            tempBgRect.x = floor( (currBg->bgRect.x*renderScale)+renderZone->x-sceneCamera->x );
                                            tempBgRect.y = floor( (currBg->bgRect.y*renderScale)+renderZone->y-sceneCamera->y );
                                            texRes = (textureResource*) texTypeContainer->get_held_resource();
                                            if( texRes!=NULL)
                                            {
                                                if( texRes->textureInEditor!=NULL)
                                                {
                                                    if( texRes->textureInEditor->get_width() >0 && texRes->textureInEditor->get_height() >0 )
                                                    {
                                                        tempBgRect.x = floor( (currBg->bgRect.x*renderScale)+renderZone->x-sceneCamera->x );
                                                        tempBgRect.y = floor( (currBg->bgRect.y*renderScale)+renderZone->y-sceneCamera->y );
                                                        if( currBg->stretchBG)
                                                        {
                                                            texRes->textureInEditor->render_tex_resized(cRender,renderZone->x-sceneCamera->x,renderZone->y-sceneCamera->y,sceneRect.w-(renderZone->x),sceneRect.h-(renderZone->y));
                                                        }
                                                        else if(currBg->tileHori  )
                                                        {
                                                            for(i = tempBgRect.x; i <=(sceneRect.w)*renderScale+renderZone->x; i+=texRes->textureInEditor->get_width()*renderScale )
                                                            {
                                                                if( currBg->tileVert)
                                                                {
                                                                    for(j =tempBgRect.y; j <= (sceneRect.h)*renderScale+renderZone->y; j+=texRes->textureInEditor->get_height()*renderScale )
                                                                    {
                                                                        texRes->textureInEditor->render_tex_resized(cRender,floor(i),floor(j), ceil( texRes->textureInEditor->get_width()*renderScale ),ceil( texRes->textureInEditor->get_height()*renderScale ) );
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    texRes->textureInEditor->render_tex_resized(cRender,floor(i),floor(tempBgRect.y), ceil( texRes->textureInEditor->get_width()*renderScale ), ceil( texRes->textureInEditor->get_height()*renderScale ) );
                                                                }
                                                            }
                                                        }
                                                        else if(currBg->tileVert)
                                                        {
                                                            for(j = tempBgRect.y; j <=(sceneRect.h)*renderScale+editorView.y; j+=texRes->textureInEditor->get_height()*renderScale )
                                                            {
                                                                texRes->textureInEditor->render_tex_resized(cRender,floor(tempBgRect.x),floor(j), ceil( texRes->textureInEditor->get_width()*renderScale ),ceil( texRes->textureInEditor->get_height()*renderScale ) );
                                                            }
                                                        }
                                                        else
                                                        {
                                                            texRes->textureInEditor->render_tex_resized(cRender,floor(tempBgRect.x),floor(tempBgRect.y), ceil( texRes->textureInEditor->get_width()*renderScale ), ceil( texRes->textureInEditor->get_height()*renderScale ) );
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    GPE_MAIN_GUI->update_tooltip("Texture held image not found!");
                                                }
                                            }
                                            else
                                            {
                                                GPE_MAIN_GUI->update_tooltip("Texture not loading properly");
                                            }
                                        }
                                        else
                                        {
                                            GPE_MAIN_GUI->update_tooltip("Texture not found");
                                        }
                                    }
                                    else
                                    {
                                        //GPE_MAIN_GUI->update_tooltip("Texture id <=0");
                                    }
                                }
                                else
                                {
                                    //GPE_MAIN_GUI->update_tooltip("CurrentBG = NULL ");
                                }
                            }
                        }
                        else
                        {
                            //GPE_MAIN_GUI->update_tooltip("Empty Background Layer");
                        }
                    }
                    else if( cTempLayer->layerType==LAYER_TYPE_OBJECTS)
                    {
                        //for( j = (int)cTempLayer->layerObjects.size()-1; j>=0; j-- )
                        for( j = 0; j < (int)cTempLayer->layerObjects.size(); j++ )
                        {
                            cGGO = cTempLayer->layerObjects[j];
                            if( cGGO!=NULL)
                            {
                                objectSpriteRender = false;
                                tempObjRect.x = floor( (cGGO->objRect.x*renderScale+renderZone->x-sceneCamera->x*renderScale) );
                                tempObjRect.y = floor( (cGGO->objRect.y*renderScale+renderZone->y-sceneCamera->y*renderScale) );
                                if( cGGO->objTypeId > 0)
                                {
                                    //renders the object's sprite
                                    objTypeContainer = allObjsFolder->find_resource_from_id(cGGO->objTypeId);
                                    if( objTypeContainer!=NULL && objTypeContainer->get_held_resource()!=NULL )
                                    {
                                        tempGameObj = (gameObjectResource*) objTypeContainer->get_held_resource();
                                        if( tempGameObj!=NULL)
                                        {
                                            //fObjAngle = cGGO->objAngleField->get_held_number();
                                            //if( fObjAngle!=0 && fObjAngle!=360 && fObjAngle!=720)
                                            {
                                                sprTypeContainer = tempGameObj->spriteField->get_selected_container();
                                                if( sprTypeContainer!=NULL)
                                                {
                                                    tempGameSpr = (spriteResource*) sprTypeContainer->get_held_resource();
                                                    if( tempGameSpr!=NULL && tempGameSpr->spriteInEditor!=NULL)
                                                    {
                                                        render_sprite_resized(cRender,tempGameSpr->spriteInEditor,tempGameSpr->get_preview_frame(),tempObjRect.x,tempObjRect.y, ceil(tempGameSpr->spriteInEditor->width*renderScale) ,ceil( tempGameSpr->spriteInEditor->height*renderScale) );
                                                        objectSpriteRender = true;
                                                    }
                                                }
                                            }
                                        }
                                        if( !objectSpriteRender )
                                        {
                                            objTypeContainer->render_image(cRender,tempObjRect.x,tempObjRect.y,ceil( (double)tempObjRect.w*renderScale),ceil( (double)tempObjRect.h*renderScale ),viewedSpace,cam);
                                        }
                                    }
                                    else
                                    {
                                        render_rectangle(cRender,tempObjRect.x,tempObjRect.y,tempObjRect.x+tempObjRect.w*renderScale,tempObjRect.y+tempObjRect.h*renderScale,c_maroon,false);
                                        render_new_text(cRender, tempObjRect.x, tempObjRect.y,"Unknown Game Id["+int_to_string( cGGO->objTypeId )+"]", c_red,DEFAULT_FONT,FA_LEFT,FA_TOP);

                                    }
                                }
                                else
                                {
                                    render_rectangle(cRender,tempObjRect.x,tempObjRect.y,tempObjRect.x+tempObjRect.w*renderScale,tempObjRect.y+tempObjRect.h*renderScale,c_maroon,false);
                                    render_new_text(cRender, tempObjRect.x, tempObjRect.y,"Unknown Game Object", c_red,DEFAULT_FONT,FA_LEFT,FA_TOP);
                                }
                            }
                            else
                            {
                                /*
                                //Done only on reverse iteration
                                cGGO = NULL;
                                currentLayer->layerObjects.erase(currentLayer->layerObjects.begin()+i);
                                */
                            }
                        }
                    }
                }
            }
        }

        if(currentLayer!=NULL && currentLayer->layerType==LAYER_TYPE_TILES && showEditorPreviews )
        {
            GPE_ResourceContainer * tsTypeContainer = tilesheetDropDown->get_selected_container();
            if( tsTypeContainer!=NULL)
            {
                tsRes = (tilesheetResource*) tsTypeContainer->get_held_resource();
                if( tsRes!=NULL && tsRes->tilesheetInEditor!=NULL && tsRes->tilesheetInEditor->tsImage!=NULL)
                {
                    if( get_mouse_coords(viewedSpace,cam) && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
                    {
                        int sceneTileMouseX  = sceneMouseXPos;
                        int sceneTileMouseY  = sceneMouseYPos;

                        if( showObjLines && lvlTileWidth!=0 && lvlTileHeight!=0)
                        {
                            sceneTileMouseX = sceneMouseXPos/lvlTileWidth;
                            sceneTileMouseX = (sceneTileMouseX * lvlTileWidth);
                            sceneTileMouseY = sceneMouseYPos/lvlTileHeight;
                            sceneTileMouseY = (sceneTileMouseY * lvlTileHeight);
                        }
                        sceneTileMouseX = floor( (sceneTileMouseX*renderScale+renderZone->x-sceneCamera->x*renderScale) );
                        sceneTileMouseY = floor( (sceneTileMouseY*renderScale+renderZone->y-sceneCamera->y*renderScale) );
                        if( sceneMouseXPos < 0 || sceneMouseYPos < 0 || sceneMouseXPos > sceneRect.w || sceneMouseYPos > sceneRect.h )
                        {
                            tSPreviewer->render_selection(cRender,sceneTileMouseX,sceneTileMouseY,viewedSpace,cam,forceRedraw,renderScale, c_red );
                        }
                        else
                        {
                            tSPreviewer->render_selection(cRender,sceneTileMouseX,sceneTileMouseY,viewedSpace,cam,forceRedraw,renderScale,c_white);
                        }
                    }
                }
            }
        }
    }
}

void gameSceneResource::render_self(GPE_Renderer * cRender,SDL_Rect * viewedSpace, SDL_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( forceRedraw )
    {
        render_rect(cRender,viewedSpace,GPE_MAIN_TEMPLATE->Program_Color,false);
    }
    double i = 0, j = 0, k = 0;
    int gridX1 = 0;
    int gridX2 = 0;
    int gridY1 = 0;
    int gridY2 = 0;
    //renders the level
    GPE_SceneTexture * currBg = NULL;

    //renders the BGs
    GPE_ResourceContainer * allTexturesFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE]+"s");
    GPE_ResourceContainer * allTilesheetsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TILESHEET]+"s");
    GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
    //renders the background

    int bgI = 0;
    SDL_Rect tempObjRect = {0,0,32,32};
    if( forceRedraw && allTexturesFolder!=NULL)
    {
        if( sceneBackgroundColor!=NULL)
        {
            GPE_Color * bgRenderColor = sceneBackgroundColor->get_color();
            if( bgRenderColor!=NULL)
            {
                render_rectangle(cRender,editorView.x,editorView.y,editorView.x+editorView.w,editorView.y+editorView.h,bgRenderColor,false);
                delete bgRenderColor;
                bgRenderColor = NULL;
            }
        }
        render_scene_layers(cRender,viewedSpace,cam);

        if( selectedSceneObject!=NULL)
        {
            tempObjRect.x = sceneZoomAmount*(selectedSceneObject->objRect.x-2)+editorView.x-sceneEditorViewRect.x;
            tempObjRect.y = sceneZoomAmount*(selectedSceneObject->objRect.y-2)+editorView.y-sceneEditorViewRect.y;
            tempObjRect.w = sceneZoomAmount*(selectedSceneObject->objRect.w+2);
            tempObjRect.h = sceneZoomAmount*(selectedSceneObject->objRect.h+2);
            render_rect(cRender,&tempObjRect,c_blue,false,96);

            render_rect(cRender,&tempObjRect,c_white,true);
            /*
            tempObjRect.x-=2;
            tempObjRect.y-=2;
            tempObjRect.w+=2;
            tempObjRect.h+=2;
            render_rect(cRender,&tempObjRect,c_blue,true);*/
        }

        if(currentLayer!=NULL && editorMode == LEDITOR_MODE_LAYERS )
        {
            if( currentLayer->layerType==LAYER_TYPE_OBJECTS)
            {
                if( showObjLines && objSnapX>=8 && objSnapY>=8 )
                {
                    /*
                    for( i = 0; i <= sceneRect.w/sceneZoomAmount; i+=objSnapX*sceneZoomAmount )
                    {
                        render_vertical_line_color(cRender,i+editorView.x-sceneEditorViewRect.x,editorView.y-sceneEditorViewRect.y,editorView.y-sceneEditorViewRect.y+sceneRect.h,c_maroon,255);
                    }
                    for( j =0; j <= sceneRect.h/sceneZoomAmount; j+=objSnapY*sceneZoomAmount )
                    {
                        render_horizontal_line_color(cRender,j+editorView.y-sceneEditorViewRect.y,editorView.x-sceneEditorViewRect.x,editorView.x-sceneEditorViewRect.x+sceneRect.w,c_maroon,255);
                    }
                    */
                    gridY1 = editorView.y;
                    if( sceneZoomAmount <=1)
                    {
                        gridY2 = editorView.y+std::min( (int)(sceneRect.h),(int)(sceneEditorViewRect.h/sceneZoomAmount) )*sceneZoomAmount;
                    }
                    else
                    {
                        gridY2 = editorView.y+std::max( (int)(sceneRect.h),(int)(sceneEditorViewRect.h*sceneZoomAmount) );
                    }
                    for( i = ( (int)sceneEditorViewRect.x/objSnapX)*objSnapX; i <= std::min( (int)sceneRect.w, (int)( sceneEditorViewRect.x+(sceneEditorViewRect.w+objSnapX)/sceneZoomAmount) ); i+=objSnapX )
                    {
                        gridX1 = floor( (i*sceneZoomAmount+editorView.x-sceneEditorViewRect.x*sceneZoomAmount) );
                        render_vertical_line_color(cRender,gridX1,gridY1,gridY2,c_blgray);
                    }
                    gridX1 = editorView.x;
                    if( sceneZoomAmount <=1)
                    {
                        gridX2 = editorView.x+std::min( (int)(sceneRect.w),(int)(sceneEditorViewRect.w/sceneZoomAmount) )*sceneZoomAmount;
                    }
                    else
                    {
                        gridX2 = editorView.x+std::max( (int)(sceneRect.w),(int)(sceneEditorViewRect.w*sceneZoomAmount) );
                    }
                    for( j = ( (int)sceneEditorViewRect.y/objSnapY)*objSnapY; j <= std::min( (int)sceneRect.h, (int)( sceneEditorViewRect.y+(sceneEditorViewRect.h+objSnapY)/sceneZoomAmount) ); j+=objSnapY )
                    {
                        gridY1 = floor( (j*sceneZoomAmount+editorView.y-sceneEditorViewRect.y*sceneZoomAmount) );
                        render_horizontal_line_color(cRender,gridY1,gridX1,gridX2,c_blgray);
                    }
                }

                GPE_ResourceContainer * hObj = objectInEditor->get_selected_container();

                int sceneObjMouseX  = sceneMouseXPos;
                int sceneObjMouseY  = sceneMouseYPos;

                if( showObjLines && objSnapX!=0 && objSnapY!=0)
                {
                    sceneObjMouseX = sceneMouseXPos/objSnapX;
                    sceneObjMouseX = (sceneObjMouseX * objSnapX);
                    sceneObjMouseY = sceneMouseYPos/objSnapY;
                    sceneObjMouseY = (sceneObjMouseY * objSnapY);
                }
                sceneObjMouseX = floor( (sceneObjMouseX*sceneZoomAmount+editorView.x-sceneEditorViewRect.x*sceneZoomAmount) );
                sceneObjMouseY = floor( (sceneObjMouseY*sceneZoomAmount+editorView.y-sceneEditorViewRect.y*sceneZoomAmount) );
                if( objectLeftClickModeButton->is_clicked()==false && hObj!=NULL && hObj->get_held_resource() )
                {
                    gameObjectResource * actualHGameObject= (gameObjectResource*) hObj->get_held_resource();
                    if( actualHGameObject!=NULL)
                    {
                        double renderWSize = 32*sceneZoomAmount;
                        double renderHSize = 32*sceneZoomAmount;
                        if( actualHGameObject->spriteField!=NULL && actualHGameObject->spriteField->get_selected_container()!=NULL )
                        {
                            GPE_ResourceContainer * tempSpriteContainer = actualHGameObject->spriteField->get_selected_container();
                            if( tempSpriteContainer!=NULL && tempSpriteContainer->get_held_resource() )
                            {
                                spriteResource * tempSpriteObj = (spriteResource * )tempSpriteContainer->get_held_resource();
                                if( tempSpriteObj!=NULL )
                                {
                                    if( tempSpriteObj->spriteInEditor!=NULL )
                                    {
                                        renderWSize = ceil(tempSpriteObj->spriteInEditor->width*sceneZoomAmount);
                                        renderHSize = ceil(tempSpriteObj->spriteInEditor->height*sceneZoomAmount);
                                    }
                                }
                            }
                        }
                        hObj->render_image(cRender,sceneObjMouseX,sceneObjMouseY,renderWSize, renderHSize, viewedSpace,cam, c_gray );
                    }
                }
            }
            else if( currentLayer->layerType == LAYER_TYPE_TILES )
            {
                if(viewTileGridCheckBox->is_clicked() )
                {
                    /*
                    for( i = 0; i <= sceneRect.w ;i+=lvlTileWidth )
                    {
                        render_vertical_line_color(cRender,i+editorView.x-sceneEditorViewRect.x,editorView.y-sceneEditorViewRect.y,editorView.y-sceneEditorViewRect.y+sceneRect.h,c_blue,255);
                    }
                    for( j =0; j <= sceneRect.h ;j+=lvlTileHeight )
                    {
                        render_horizontal_line_color(cRender,j+editorView.y-sceneEditorViewRect.y,editorView.x-sceneEditorViewRect.x,editorView.x-sceneEditorViewRect.x+sceneRect.w,c_blue,255);
                    }
                    */
                    gridY1 = editorView.y;
                    if( sceneZoomAmount <=1)
                    {
                        gridY2 = editorView.y+std::min( (int)(sceneRect.h),(int)(sceneEditorViewRect.h/sceneZoomAmount) )*sceneZoomAmount;
                    }
                    else
                    {
                        gridY2 = editorView.y+std::max( (int)(sceneRect.h),(int)(sceneEditorViewRect.h*sceneZoomAmount) );
                    }
                    for( i = ( (int)sceneEditorViewRect.x/lvlTileWidth)*lvlTileWidth; i <= std::min( (int)sceneRect.w, (int)( sceneEditorViewRect.x+sceneEditorViewRect.w/sceneZoomAmount) ); i+=lvlTileWidth )
                    {
                        gridX1 = floor( editorView.x+i*sceneZoomAmount-sceneEditorViewRect.x*sceneZoomAmount );
                        render_vertical_line_color(cRender,gridX1,gridY1,gridY2,c_blgray);
                    }
                    gridX1 = editorView.x;
                    if( sceneZoomAmount <=1)
                    {
                        gridX2 = editorView.x+std::min( (int)(sceneRect.w),(int)(sceneEditorViewRect.w/sceneZoomAmount) )*sceneZoomAmount;
                    }
                    else
                    {
                        gridX2 = editorView.x+std::max( (int)(sceneRect.w),(int)(sceneEditorViewRect.w*sceneZoomAmount) );
                    }
                    for( j =(int)(sceneEditorViewRect.y/lvlTileHeight)*lvlTileHeight; j <= std::min( (int)sceneRect.h, (int)( sceneEditorViewRect.y+sceneEditorViewRect.h/sceneZoomAmount) ); j+=lvlTileHeight )
                    {
                        gridY1 = floor( editorView.y+j*sceneZoomAmount-sceneEditorViewRect.y*sceneZoomAmount );
                        render_horizontal_line_color(cRender,gridY1,gridX1,gridX2,c_blgray);
                    }
                }
            }
            else if( currentLayer->layerType == LAYER_TYPE_BACKGROUND )
            {
                for( i = 0; i <= (int)(currentLayer->layerBackgrounds.size() ) ;i++ )
                {

                }
            }
        }
        render_rect(cRender,&editorPane,GPE_MAIN_TEMPLATE->Program_Color,false);

        if( editorButtonBar!=NULL)
        {
            editorButtonBar->render_self(cRender,viewedSpace,cam);
        }

        if( sceneZoomLevel!=NULL)
        {
            sceneZoomLevel->render_self(cRender,viewedSpace,cam);
        }

        if( CURRENT_PROJECT!=NULL && CURRENT_PROJECT->RESC_PROJECT_SETTINGS!=NULL )
        {
            projectPropertiesResource * tProjectProps = (projectPropertiesResource *)CURRENT_PROJECT->RESC_PROJECT_SETTINGS->get_held_resource();
            if( tProjectProps!=NULL && tProjectProps->sceneEditorPaneMode!=NULL)
            {
                tProjectProps->sceneEditorPaneMode->render_self(cRender,viewedSpace,cam);
            }
        }
    }

    if( forceRedraw )
    {
        //"Editor View:("+int_to_string(sceneEditorViewRect.x)+","+int_to_string(sceneEditorViewRect.y)+","+int_to_string(sceneEditorViewRect.w)+","+int_to_string(sceneEditorViewRect.h)+")",

        render_rect(cRender,&editorCommentPane,GPE_MAIN_TEMPLATE->Program_Color,false);
        render_rectangle(cRender,sceneYScroll->barBox.x+sceneYScroll->barBox.w,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
        render_rectangle(cRender,sceneXScroll->barBox.x,sceneXScroll->barBox.y,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);

        render_bitmap_text(cRender,editorCommentPane.x+GENERAL_GPE_PADDING,editorCommentPane.y+editorCommentPane.h/2,
                        "Mouse( "+int_to_string(sceneMouseXPos )+" , "+int_to_string(sceneMouseYPos)+")"+
                        "Camera( "+double_to_string(sceneEditorViewRect.x )+" , "+double_to_string(sceneEditorViewRect.y)+") Zoom:"+double_to_string(sceneZoomAmount),
                        GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_TEXTINPUT,FA_LEFT,FA_MIDDLE);
        if( sceneXScroll!=NULL)
        {
            sceneXScroll->render_self(cRender,viewedSpace,cam);
        }
        if( sceneYScroll!=NULL)
        {
            sceneYScroll->render_self(cRender,viewedSpace,cam);
        }
    }

    editorPaneList->render_self(cRender,viewedSpace,cam,forceRedraw);
    if( currentLayer!=NULL && editorMode==LEDITOR_MODE_LAYERS )
    {
        if( currentLayer->layerType==LAYER_TYPE_TILES )
        {
            if( forceRedraw  )
            {
                tSPreviewer->render_self(cRender,viewedSpace,cam);
            }
        }
        layerPaneList->render_self(cRender,viewedSpace,cam,forceRedraw);
    }

    //render_rect(cRender,&editorPane,GPE_MAIN_TEMPLATE->Main_Border_Color,true);
}

void gameSceneResource::reset_placement_info()
{
    lastCreatedObjTypeId = -1;
    lastCreatedObjXPos = -1;
    lastCreatedObjYPos = -1;
}

void gameSceneResource::save_resource(std::string alternatePath, int backupId)
{
    int i = 0, j = 0, k = 0, jMax = 0, kMax;
    displayMessageTitle = "Saving GameScene";
    displayMessageSubtitle = resourceName;
    displayMessageString = "...";
    display_user_messaage();
    std::string newFileIn ="";
    appendToFile(alternatePath,"blank");
    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = get_path_from_file(alternatePath);
    if( path_exists(soughtDir) )
    {
        newFileOut = alternatePath;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/scenes/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSceneFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( newSceneFile != NULL )
    {
        //makes sure the file is open
        if (newSceneFile.is_open())
        {
            GPE_SceneGameObject * tSceneObj = NULL;
            GPE_ObjectComponent * tObjCustomProperty = NULL;
            GPE_SceneTexture * tSceneBg = NULL;
            GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
            GPE_ResourceContainer * fObj = NULL;
            newSceneFile << "#    --------------------------------------------------  # \n";
            newSceneFile << "#     \n";
            newSceneFile << "#     \n";
            newSceneFile << "#    Game Pencil Engine Project GameScene File \n";
            newSceneFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newSceneFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newSceneFile << "#    If you have any problems reading this file please report it to debug@pawbyte.com . \n";
            newSceneFile << "#     \n";
            newSceneFile << "#     \n";
            newSceneFile << "#    --------------------------------------------------  # \n";
            newSceneFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            newSceneFile << "ResourceName=" << resourceName << "\n";
            if( levelTitleField!=NULL)
            {
                if( (int)levelTitleField->get_string().size() > 0)
                {
                    newSceneFile << "Title=" << levelTitleField->get_string() << "\n";
                }
            }
            if( sceneCaptionField!=NULL)
            {
                if( (int)sceneCaptionField->get_string().size() > 0)
                {
                    newSceneFile << "Caption=" << sceneCaptionField->get_string() << "\n";
                }
            }
            if( levelHintField!=NULL)
            {
                if( (int)levelHintField->get_string().size() > 0)
                {
                    newSceneFile << "SceneHint=" << levelHintField->get_string() << "\n";
                }
            }
            if( musicAudioDropDown!=NULL)
            {
                newSceneFile << "SceneMusic=" << musicAudioDropDown->get_selected_name() << "\n";
            }
            if( startAudioDropDown!=NULL)
            {
                newSceneFile << "SceneStartAudio=" << startAudioDropDown->get_selected_name() << "\n";
            }

            if( endAudioDropDown!=NULL)
            {
                newSceneFile << "SceneEndAudio=" << endAudioDropDown->get_selected_id() << "\n";
            }


            if( levelPixelWidthField!=NULL)
            {
                levelPixelWidthField->make_valid_number(4096);
                newSceneFile << "LevelWidth=" << levelPixelWidthField->get_held_number() << "\n";

            }

            if( levelPixelHeightField!=NULL)
            {
                levelPixelHeightField->make_valid_number(4096);
                newSceneFile << "LevelHeight=" << levelPixelHeightField->get_held_number() << "\n";
            }

            if( levelTileWidthField!=NULL)
            {
                levelTileWidthField->make_valid_number(32);
                newSceneFile << "TileWidth=" << levelTileWidthField->get_held_number() << "\n";
            }

            if( levelTileHeightField!=NULL)
            {
                levelTileHeightField->make_valid_number(32);
                newSceneFile << "TileHeight=" << levelTileHeightField->get_held_number() << "\n";
            }
            if( viewTileGridCheckBox!=NULL)
            {
                newSceneFile << "ViewTileGrid=" << viewTileGridCheckBox->is_clicked() << "\n";
            }

            if( preloadCheckBox!=NULL)
            {
                newSceneFile << "Preload=" << preloadCheckBox->is_clicked() << "\n";
            }
            if( isometricCheckBox!=NULL)
            {
                newSceneFile << "Isometric=" << isometricCheckBox->is_clicked() << "\n";
            }
            if( checkBoxIsContinuous!=NULL)
            {
                newSceneFile << "Continuous=" << checkBoxIsContinuous->is_clicked() << "\n";
            }

            if( sceneBackgroundColor!=NULL)
            {
                newSceneFile << "BGColor=" << sceneBackgroundColor->get_rgb_string() << "\n";
            }

            if( levelObjGridWidthField!=NULL)
            {
                levelObjGridWidthField->make_valid_number(32);
                newSceneFile << "ObjectGridWidth=" << levelObjGridWidthField->get_string() << "\n";
            }

            if( levelObjGridHeightField!=NULL)
            {
                levelObjGridHeightField->make_valid_number(32);
                newSceneFile << "ObjectGridHeight=" << levelObjGridHeightField->get_string() << "\n";
            }

            if( useObjGridCheckBox!=NULL)
            {
                newSceneFile << "DisplayObjectGrid=" << useObjGridCheckBox->is_clicked() << "\n";
            }

            sceneLayer * tempTLayer = NULL;
            GPE_SceneTile * fSceneTile = NULL;
            int foundTilePos = 0;
            int maxTilesInLayer = 0;
            for( i = 0; i < (int)sceneLayers.size(); i++)
            {
                tempTLayer = sceneLayers[i];
                if( tempTLayer!=NULL)
                {
                    maxTilesInLayer = (int)tempTLayer->mapTiles.size();
                    newSceneFile << "[GPE_Layer="+int_to_string(tempTLayer->layerType)+","+int_to_string(tempTLayer->projectLayerId)+",]" << "\n";
                    if( tempTLayer->layerType==LAYER_TYPE_TILES)
                    {
                        for(  j = 0; j < lvlTileAmountY;j++)
                        {
                            newSceneFile << "   ";
                            for(  k = 0; k < lvlTileAmountX;k++)
                            {
                                foundTilePos = k+j*lvlTileAmountX;
                                if(  maxTilesInLayer> foundTilePos)
                                {
                                    fSceneTile = tempTLayer->mapTiles.at(foundTilePos);
                                    if( fSceneTile!=NULL)
                                    {
                                        if( fSceneTile->tilesheetIndexId>=0 && fSceneTile->tileIndexId>=0)
                                        {
                                            newSceneFile << int_to_string(fSceneTile->tilesheetIndexId) << "-" << int_to_string(fSceneTile->tileIndexId) << ",";
                                        }
                                        else
                                        {
                                            newSceneFile << "0,";
                                        }
                                    }
                                    else
                                    {
                                        newSceneFile << "0,";
                                    }
                                }
                            }
                            newSceneFile << "\n";
                        }
                    }
                    else if( tempTLayer->layerType==LAYER_TYPE_BACKGROUND && (int)(tempTLayer->layerBackgrounds.size() ) > 0)
                    {
                        for( j = 0; j < (int)(tempTLayer->layerBackgrounds.size() ); j++)
                        {
                            if(tempTLayer->layerBackgrounds[j]!=NULL)
                            {
                                newSceneFile << "   GPE_Background=";
                                if( tempTLayer->layerBackgrounds[j]->backgroundInEditor!=NULL)
                                {
                                    newSceneFile << tempTLayer->layerBackgrounds[j]->backgroundInEditor->get_selected_name() << ",";
                                }
                                else
                                {
                                    newSceneFile << "-1,";
                                }
                                if( tempTLayer->layerBackgrounds[j]->bgPosXField!=NULL)
                                {
                                    tempTLayer->layerBackgrounds[j]->bgPosXField->make_valid_number(0);
                                    newSceneFile <<  tempTLayer->layerBackgrounds[j]->bgPosXField->get_held_number() << ",";
                                }
                                else
                                {
                                    newSceneFile << "-1,";
                                }
                                if( tempTLayer->layerBackgrounds[j]->bgPosYField!=NULL)
                                {
                                    tempTLayer->layerBackgrounds[j]->bgPosYField->make_valid_number(0);
                                    newSceneFile << tempTLayer->layerBackgrounds[j]->bgPosYField->get_held_number() << ",";
                                }
                                else
                                {
                                    newSceneFile << "-1,";
                                }
                                if( tempTLayer->layerBackgrounds[j]->bgHorSpeedField!=NULL)
                                {
                                    tempTLayer->layerBackgrounds[j]->bgHorSpeedField->make_valid_number(0);
                                    newSceneFile << tempTLayer->layerBackgrounds[j]->bgHorSpeedField->get_held_number() << ",";
                                }
                                else
                                {
                                    newSceneFile << "-1,";
                                }
                                if( tempTLayer->layerBackgrounds[j]->bgVertSpeedField!=NULL)
                                {
                                    tempTLayer->layerBackgrounds[j]->bgVertSpeedField->make_valid_number(0);
                                    newSceneFile << tempTLayer->layerBackgrounds[j]->bgVertSpeedField->get_held_number() << ",";
                                }
                                else
                                {
                                    newSceneFile << "-1,";
                                }
                                if( tempTLayer->layerBackgrounds[j]->checkTileHori!=NULL)
                                {
                                    newSceneFile << tempTLayer->layerBackgrounds[j]->checkTileHori->is_clicked() << ",";
                                }
                                else
                                {
                                    newSceneFile << "-1,";
                                }
                                if( tempTLayer->layerBackgrounds[j]->checkTileVert!=NULL)
                                {
                                    newSceneFile << tempTLayer->layerBackgrounds[j]->checkTileVert->is_clicked() << ",";
                                }
                                else
                                {
                                    newSceneFile << "-1,";
                                }
                                if( tempTLayer->layerBackgrounds[j]->checkStretch!=NULL)
                                {
                                    newSceneFile << tempTLayer->layerBackgrounds[j]->checkStretch->is_clicked() << ",";
                                }
                                else
                                {
                                    newSceneFile << "-1,";
                                }
                                newSceneFile << "\n";
                            }
                        }
                    }
                    else if( tempTLayer->layerType==LAYER_TYPE_OBJECTS)
                    {
                        for( j = 0; j < (int)tempTLayer->layerObjects.size(); j++)
                        {
                            tSceneObj = tempTLayer->layerObjects[j];
                            if( tSceneObj!=NULL)
                            {
                                newSceneFile << "   GPEObject=";
                                if( allObjsFolder!=NULL)
                                {
                                    fObj = allObjsFolder->find_resource_from_name(tSceneObj->objTypeName);
                                    if( fObj==NULL)
                                    {
                                        fObj = allObjsFolder->find_resource_from_id( tSceneObj->objTypeId );
                                    }
                                }

                                if( fObj!=NULL)
                                {
                                    tSceneObj->objTypeId = fObj->get_global_id();
                                    tSceneObj->objTypeName = fObj->get_name();
                                    newSceneFile << fObj->get_name() << ",";
                                }
                                else if( (int)tSceneObj->objTypeName.size() > 0)
                                {
                                    newSceneFile << tSceneObj->objTypeName << ",";
                                }
                                else
                                {
                                     newSceneFile << tSceneObj->objTypeId << ",";
                                }
                                newSceneFile << tSceneObj->objRect.x << "," << tSceneObj->objRect.y << "," ;
                                newSceneFile << tSceneObj->objAngleField->get_held_number()  << "," << tSceneObj->objXScale << "," << tSceneObj->objYScale << "," ;
                                if( tSceneObj->objColor!=NULL)
                                {
                                    newSceneFile << (int)tSceneObj->objColor->get_r() << "," << (int)tSceneObj->objColor->get_g() << "," << (int)tSceneObj->objColor->get_b() << ",";
                                }
                                else
                                {
                                    newSceneFile <<  "0,0,0,";
                                }
                                if( (int)tSceneObj->objSubName->get_string().size() >0)
                                {
                                    newSceneFile << tSceneObj->objSubName->get_string() << ",,,";
                                }
                                else
                                {
                                    newSceneFile << "unnamed,,,";
                                }
                                newSceneFile << "[GPECustomFields]";
                                kMax = (int) tSceneObj->objCustomValPairs.size();
                                if( kMax > 0)
                                {
                                    for( k = 0; k < kMax ; k++ )
                                    {
                                        tObjCustomProperty = tSceneObj->objCustomValPairs[k];
                                        if( tObjCustomProperty!=NULL)
                                        {
                                            newSceneFile << "[component]"+tObjCustomProperty->get_data()+"[/component]";
                                        }
                                    }
                                }
                                newSceneFile << "[/GPECustomFields]";
                                newSceneFile << "\n";
                            }
                        }
                    }
                    newSceneFile << "[/GPE_Layer]" << "\n";
                }
            }

            newSceneFile.close();
            if( !usingAltSaveSource)
            {
                isModified = false;
            }
        }
        else
        {
            GPE_Main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

void gameSceneResource::select_object(GPE_SceneGameObject * objToSelect)
{
    unselect_object();
    if( objToSelect!=NULL)
    {
        selectedSceneObject = objToSelect;
        if( layerPaneList!=NULL)
        {
            layerPaneList->reset_self();
        }
        if( objectEditorNameTitle!=NULL)
        {
           objectEditorNameTitle ->set_name( "["+objToSelect->objTypeName+"]" );
        }
    }
}
bool gameSceneResource::unselect_object()
{
    if( selectedSceneObject!=NULL)
    {
        selectedSceneObject->isBeingMoved = false;
        selectedSceneObject = NULL;
        if( layerPaneList!=NULL)
        {
            layerPaneList->reset_self();
        }
        if( objectEditorNameTitle!=NULL)
        {
           objectEditorNameTitle ->set_name( "None");
        }
        return true;
    }
    return false;
}

void gameSceneResource::update_box(int newX, int newY, int newW, int newH)
{

}

void gameSceneResource::update_project_layers()
{
    if( layerMenu!=NULL)
    {
        GPE_SelectBoxBasic_Option * tOption = NULL;
        for( int i = layerMenu->get_size()-1; i>=0; i--)
        {
            tOption = layerMenu->get_option(i);
            if( tOption!=NULL)
            {
                if( tOption->optionValue >=0 && tOption->optionValue < 32)
                {
                    tOption->optionName = CURRENT_PROJECT->projectLayerNames[ (int)(tOption->optionValue) ];
                }
                else
                {
                    layerMenu->remove_option(i);
                }
            }
            else
            {
                layerMenu->remove_option(i);
            }
        }
    }
}

bool gameSceneResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "scene=" << resourceName << "," << get_global_rid() << ",";
            *fileTarget << "\n";
            return true;
        }
    }
    return false;
}
