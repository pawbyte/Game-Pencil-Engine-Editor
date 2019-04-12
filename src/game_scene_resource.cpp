/*
game_scene_resource.cpp
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

#include "game_scene_resource.h"
#include "gpe_editor_settings.h"
#include "animation_resource.h"
#include "project_properties.h"
#include "texture_resource.h"
#include "tilesheet_resource.h"

std::string DEFAULT_SCENE_SUBEDITOR_NAMES[5];

gameSceneResource * currentSceneToRender = NULL;

gpeEditorDockPanel * PANEL_GRID_INFO = NULL;
gpeEditorDockPanel * PANEL_TS_RESOURCE = NULL;

gameSceneResource::gameSceneResource(GPE_GeneralResourceContainer * pFolder )
{
    selectedBranchId = -1;
    draggingOffsetX = 0;
    draggingOffsetY = 0;
    clickMultipleBranches = false;
    sceneInFocus = false;
    useLighting = false;
    //Thanks YellowAfterLife
    //Used to calculate the derotated/scaled box of an resource
    lmx = 0;
    lmy = 0;
    rmx = 0;
    rmy = 0;
    //Thanks YellowAfterLife

    clickedSceneBranch  = NULL;
    objSelectedX1 = 0;
    objSelectedY1 = 0;
    objSelectedX2 = 0;
    objSelectedY2 = 0;

    branchTempRect = new GPE_Rect();
    mouseIsInScene = false;
    sceneObjMouseX  = sceneMouseXPos = 0;
    sceneObjMouseY  = sceneMouseYPos = 0;
    sceneComponents = new GPE_SceneBasicClass();
    sceneTopBarList = new GPE_GuiElementList();

    changeLayerNameButton = new GPE_ToolLabelButton( "Change Layer","Change Layer");
    renameLayerButton = new GPE_ToolLabelButton( "Rename Layer","Rename Layer");
    addTileMapButton = new GPE_ToolLabelButton( "Add TileMap","Adds a new TileMap Container to this branch");
    addGroupButton = new GPE_ToolLabelButton( "Add Group","Adds a new Group Container to this branch");
    changeGroupNameButton = new GPE_ToolLabelButton( "Rename Group","Rename Group");
    selectedSceneBranch = NULL;
    sceneResourceTree = new GPE_SpecialMenu_Tree( "Scene" );

    zoomValue = 1;
    defaultObjectLayer = NULL;
    defaultBackgroundLayer = NULL;
    isDraggingObject = false;
    areaIsScrollable = false;
    scnPostProcessed = true;
    projectParentFolder = pFolder;
    editorMode = 0;
    sceneRect.x = 0;
    sceneRect.y =0;
    sceneRect.w = 4096;
    sceneRect.h = 4096;
    editorCameraRect.x = 0;
    editorCameraRect.y = 0;
    editorCameraRect.w = 640;
    editorCameraRect.h = 480;
    showTileLines = true;
    sceneGridX = sceneGridY =16;
    useSceneGrid = true;
    saveButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/save.png","Save Changes",-1,24);
    sceneEditorSubTitle = new GPE_Label_Title("Settings");
    customComponentsTitle = new GPE_Label_Title("Components");

    editorButtonBar = new GPE_ToolIconButtonBar(24, true);
    editorButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/object-group.png","Layers", LEDITOR_MODE_LAYERS,true);
    editorButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/dashboard.png","Settings",LEDITOR_MODE_SETTINGS,false);

    shortcutButtonBar = new GPE_ToolIconButtonBar(24, true);
    shortcutButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/mouse-pointer.png","Select-Mode(Q)", SCENE_MODE_SELECT,false);
    shortcutButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/pencil.png","Place-Mode(W)", SCENE_MODE_PLACE,false);
    shortcutButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/eraser.png","Erase-Mode(E)",SCENE_MODE_ERASE,false);
    shortcutButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/rotate-left.png","Rotation-Mode(R)",SCENE_MODE_ROTATION,false);
    shortcutButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrows.png","Movement-Mode(T)",SCENE_MODE_MOVE,false);
    shortcutButtonBar->add_option(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrows-alt.png","Scale-Mode(Y)",SCENE_MODE_SCALE,false);

    gridToggleButtton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/th-large.png","Toggle Grid(G)" );
    rotationButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/random.png","Flip/ Rotate(H)");
    objectLockStateButton = new GPE_ToolIconButton(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/lock.png","Lock Branch(J)" );
    lightingStateButton = new GPE_ToolIconButton(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/sun-o.png","Lighting Off(L)" );

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
    renameBox->disable_self();
    sceneTitleField = NULL;
    sceneCaptionField = new GPE_TextInputBasic("","Scene Caption");
    sceneCaptionField->set_label("Scene Caption");
    sceneHintField = new GPE_TextInputBasic("","Scene Hint");
    sceneHintField->set_label("Scene Hint");

    levelPixelWidthField = new GPE_TextInputNumber("",true,0);
    levelPixelWidthField->set_string( int_to_string(sceneRect.w) );
    levelPixelWidthField->set_label("Width(px)");
    levelPixelWidthField->scale_width( 0.5 );

    levelPixelHeightField = new GPE_TextInputNumber("",true,0);
    levelPixelHeightField->set_string( int_to_string(sceneRect.h) );
    levelPixelHeightField->set_label("Height(px)");
    levelPixelHeightField->scale_width( 0.5 );

    defaultTileWidthField = new GPE_TextInputNumber("width",true,0);
    defaultTileWidthField->set_string("32");
    defaultTileWidthField->set_label("Default Tile Width(px)");

    defaultTileHeightField = new GPE_TextInputNumber("height",true,0);
    defaultTileHeightField->set_string("32");
    defaultTileHeightField->set_label("Default Tile Height(px)");

    musicAudioDropDown = NULL;
    endAudioDropDown = NULL;
    startAudioDropDown = NULL;

    northSceneDropDown = NULL;
    southSceneDropDown = NULL;
    westSceneDropDown = NULL;
    eastSceneDropDown = NULL;
    if( projectParentFolder!=NULL)
    {
        musicAudioDropDown = new GPE_DropDown_Resouce_Menu( "BG Music",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_AUDIO]),-1,true);

        startAudioDropDown = new GPE_DropDown_Resouce_Menu( "Start Audio",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_AUDIO]),-1,true);

        endAudioDropDown = new GPE_DropDown_Resouce_Menu( "End Audio",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_AUDIO]),-1,true);

        //
        northSceneDropDown = new GPE_DropDown_Resouce_Menu( "North",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE]+"s"),-1,true);
        northSceneDropDown->set_width(128);

        southSceneDropDown = new GPE_DropDown_Resouce_Menu( "South",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE]+"s"),-1,true);
        southSceneDropDown->set_width(128);

        eastSceneDropDown = new GPE_DropDown_Resouce_Menu( "East",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE]+"s"),-1,true);
        eastSceneDropDown->set_width(128);

        westSceneDropDown = new GPE_DropDown_Resouce_Menu( "West",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SCENE]+"s"),-1,true);
        westSceneDropDown->set_width(128);
    }

    resizeMapButton = new GPE_ToolLabelButton("Resize Map","Click to re-size and recalculate map content" );
    preloadCheckBox = new GPE_CheckBoxBasic("Preload","Check to load scene at launch of program", true);
    isometricCheckBox = new GPE_CheckBoxBasic("Isometric Scene","The Scene is rendered and processed with the isometric engine", false);
    checkBoxIsContinuous = new GPE_CheckBoxBasic("Continuous Scene","Scene Data & Objects are stored throughout game[Reccommended: Keep unchecked for most scenes]", false);
    sceneBackgroundColor = new GPE_Input_Field_Color();
    sceneBackgroundColor->set_rgb(0,0,0);
    sceneBackgroundColor->set_label("Background");
    //end Variables for settings Tab

    //variables for the backgrounds tab
    bgIdInEdit = 0;
    //variables for the objects tab
    layerInEdit = -1;
    if( projectParentFolder!=NULL)
    {
        addNewComponentDropDown = new GPE_DropDown_Menu( "Add Component",false);
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
        addNewComponentDropDown->add_menu_option("Animation","Animations-resourcedropdown",52,false);
        addNewComponentDropDown->add_menu_option("Audio","Audio-resourcedropdown",53,false);
        addNewComponentDropDown->add_menu_option("Video","Videos-resourcedropdown",54,false);
        addNewComponentDropDown->add_menu_option("Function","Functions-resourcedropdown",55,false);
        addNewComponentDropDown->add_menu_option("Object","Objects-resourcedropdown",56,false);
        addNewComponentDropDown->add_menu_option("Scene","Scenes-resourcedropdown",57,false);
        addNewComponentDropDown->add_menu_option("Font","Fonts-resourcedropdown",58,false);
        addNewComponentDropDown->set_width(128);
        animationInEditor = new GPE_DropDown_Resouce_Menu( "Change  Animation",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ANIMATION]+"s"),-1,true);
        objectInEditor = new GPE_DropDown_Resouce_Menu( "New Branch",projectParentFolder,-1,true);
        objectReplacerInEditor = new GPE_DropDown_Resouce_Menu( "Replace Object",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s"),-1,true);
        objectNewType = new GPE_DropDown_Resouce_Menu( "Change Object",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s"),-1,true);

        closeObjectProperties = new GPE_ToolLabelButton( "Close Properties","Closes Object Properties of Selected Object");

        gridWidthField = new GPE_TextInputNumber("grid_width",true,0);
        gridWidthField->set_string("32");
        gridWidthField->set_label("Grid Width");

        gridHeightField = new GPE_TextInputNumber("grid_height",true,0);
        gridHeightField->set_string("32");
        gridHeightField->set_label("Grid Height");

        gridColorField = new GPE_Input_Field_Color("Grid Color","Sets the color of the grid cells" );
        gridAlphaField = new GPE_TextInputNumber("grid_alpha",true,0);
        gridAlphaField->set_string("228");
        gridAlphaField->set_label("Grid Alpha");

        viewTileGridCheckBox = new GPE_CheckBoxBasic("View Grid","View grid.", true);
        useObjGridCheckBox = new GPE_CheckBoxBasic("Use Grid","Have future branches placed on a uniform grid.", true);
        onlyRemoveThisObjectTypeButton = new GPE_CheckBoxBasic("Limit Scope","Only select/remove this object on right-click", false);
        forceSnapButton = new GPE_ToolLabelButton( "Snap To Grid","Aligns all branches to the  grid");
        removeObjectButton = new GPE_ToolLabelButton( "Remove from Scene","Removes only this object type from the scene");
        clearObjectsButton = new GPE_ToolLabelButton( "Clear Objects","Removes all Objects from scene");
        removeObjectButton->set_width(forceSnapButton->get_width());
        clearObjectsButton->set_width(forceSnapButton->get_width());

        inheritParentComponentButton = new GPE_ToolLabelButton( "Inherit Components","Inherit components from entity's object class");
        resetComponentsButton = new GPE_ToolLabelButton( "Reset Components","Removes all components from this object");
        resetComponentsButton->set_width(forceSnapButton->get_width());
        resetComponentsButton->set_width(forceSnapButton->get_width());

        objCustomVariableSettingsButtton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/edit.png","Edit Variable");
        objCustomVariableAddButtton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/plus.png","Add Variable");
        objCustomVariableRemoveButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/minus.png", "Remove Variable" );
        objCustomVariableRefeshButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/refresh.png","Refresh Object Default Variables");

        //variables for the tiled tab
        update_rect(&tsCameraRect,0,0,0,0);

        selectedBranchLabel = new GPE_Label_Text("Scene Branch","");
        layerErrorMessage = new GPE_Label_Text("Error invalid layer type given...","ERROR");
        layerRemoveButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/remove.png", "Remove Layer",-1,32 );
        layerMoveUpButtton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrow-up.png", "Move Layer Up",-1,32);
        layerMoveDownButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/arrow-down.png","Move Layer Down",-1,32 );
        layerToggleHideButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/adjust.png","Hide/Unhide Other Layers",-1,32 );

        tilesheetRenderXPos = 0;
        tilesheetRenderYPos = 0;

    }
    else
    {
        addNewComponentDropDown = NULL;
        animationInEditor = NULL;
        objectInEditor = NULL;
        objectReplacerInEditor = NULL;
        objectNewType = NULL;
    }

    selectedSceneObject = NULL;
    customObjCreationId = 0;
    objectEditorNameTitle = new GPE_Label_Text("None","None");
    objectEditorPropertiesTitle = new GPE_Label_Text("Object Properties","Object Properties");


    lastCreatedObjXPos=-1;
    lastCreatedObjYPos=-1;
    lastCreatedObjTypeId=-1;
    editorMode = LEDITOR_MODE_SETTINGS;
    sceneXScroll = new GPE_ScrollBar_XAxis();
    update_rectangle(&sceneXScroll->contextRect,0,0,640,480);
    sceneYScroll = new GPE_ScrollBar_YAxis();
    update_rectangle(&sceneYScroll->contextRect,0,0,640,480);
    texRes = NULL;

    gridRenderRect = new GPE_Rect();
    topResourceTreeHeight = 256;

    defaultTileWidth = GPE_DEFAULT_TILEWIDTH;
    defaultTileHeight = GPE_DEFAULT_TILEHEIGHT;
    defaultTileAmountX = sceneRect.w / defaultTileWidth;
    defaultTileAmountY = sceneRect.h / defaultTileHeight;
}

gameSceneResource::~gameSceneResource()
{
    if( branchTempRect != NULL )
    {
        delete branchTempRect;
        branchTempRect = NULL;
    }
    if( sceneComponents != NULL )
    {
        delete sceneComponents;
        sceneComponents = NULL;
    }
    if( sceneTopBarList != NULL )
    {
        delete sceneTopBarList;
        sceneTopBarList = NULL;
    }
    if( gridRenderRect != NULL )
    {
        delete gridRenderRect;
        gridRenderRect = NULL;
    }
    int i = 0;

    if(animationInEditor!=NULL)
    {
        delete animationInEditor;
        animationInEditor = NULL;
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

    if(gridToggleButtton!=NULL)
    {
        delete gridToggleButtton;
        gridToggleButtton= NULL;
    }
    if(rotationButton!=NULL)
    {
        delete rotationButton;
        rotationButton= NULL;
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

    if( checkBoxIsContinuous!=NULL)
    {
        delete checkBoxIsContinuous;
        checkBoxIsContinuous = NULL;
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

    if( gridWidthField!=NULL)
    {
        delete gridWidthField;
        gridWidthField = NULL;
    }

    if( gridHeightField!=NULL)
    {
        delete gridHeightField;
        gridHeightField = NULL;
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

    if( defaultTileWidthField!=NULL)
    {
        delete defaultTileWidthField;
        defaultTileWidthField = NULL;
    }

    if( defaultTileHeightField!=NULL)
    {
        delete defaultTileHeightField;
        defaultTileHeightField = NULL;
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

    if( sceneTitleField!=NULL)
    {
        delete sceneTitleField;
        sceneTitleField = NULL;
    }

    if( sceneCaptionField!=NULL)
    {
        delete sceneCaptionField;
        sceneCaptionField = NULL;
    }

    if( sceneHintField!=NULL)
    {
        delete sceneHintField;
        sceneHintField = NULL;
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

    if( sceneResourceTree!=NULL )
    {
        delete sceneResourceTree;
        sceneResourceTree = NULL;
    }

    if( renameLayerButton!=NULL )
    {
        delete renameLayerButton;
        renameLayerButton = NULL;
    }
    if( changeLayerNameButton!=NULL )
    {
        delete changeLayerNameButton;
        changeLayerNameButton = NULL;
    }
}

sceneLayer * gameSceneResource::add_retro_layer(int layerType, int newLayerId, bool selectLayer)
{
    sceneLayer * previousFoundLayer = NULL;
    sceneLayer * newLayer = NULL;
    if( layerType >=0 && layerType <= LAYER_TYPE_MAX )
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

            GPE_SpecialMenu_Branch * fResBranch = NULL;
            sceneLayer * fLayer = NULL;
            for( availI = (int)(availableLayerIds.size() )-1; availI >=0; availI--)
            {
                layerWasUsed = false;
                for( sLayerJ = (int)(sceneResourceTree->subElements.size() )-1; sLayerJ >=0; sLayerJ--)
                {
                    fResBranch = sceneResourceTree->subElements.at(sLayerJ);
                    if( fResBranch!=NULL && fResBranch->get_type() == BRANCH_TYPE_LAYER )
                    {
                        fLayer = (sceneLayer *)fResBranch;
                        if( availableLayerIds[ availI ]==fLayer->projectLayerId )
                        {
                            layerWasUsed = true;
                        }
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
            newLayer = new sceneLayer( newLayerId, projectParentFolder);

            if( newLayer!=NULL)
            {
                sceneResourceTree->add_branch( newLayer );
                if( selectLayer )
                {
                    sceneResourceTree->select_branch( newLayer );
                    selectedSceneBranch = newLayer;
                }
                newLayer->set_name( CURRENT_PROJECT->projectLayerNames[newLayerId] );
                //Now we have the joy of adding a new tilemap
                if( layerType==LAYER_TYPE_TILES)
                {
                    GPE_SceneTileMap * newTileMap = add_tilemap( newLayer, "tilemap" );
                }
            }
        }
        else
        {
            GPE_Report("New Layer NOT CREATED of Type ["+int_to_string(layerType)+"] with id"+int_to_string(newLayerId) +"( already existed).");
        }
    }
    else
    {
        GPE_Report("Unable to create Layer of Type ["+int_to_string(layerType)+"] with id"+int_to_string(newLayerId)+"." );
    }
    return newLayer;
}

sceneLayer * gameSceneResource::add_layer( int newLayerId, bool selectLayer )
{
    sceneLayer * previousFoundLayer = NULL;
    sceneLayer * newLayer = NULL;

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
        GPE_SpecialMenu_Branch * fResBranch = NULL;
        sceneLayer * fLayer = NULL;
        for( availI = (int)(availableLayerIds.size() )-1; availI >=0; availI--)
        {
            layerWasUsed = false;
            for( sLayerJ = (int)(sceneResourceTree->subElements.size() )-1; sLayerJ >=0; sLayerJ--)
            {
                fResBranch = sceneResourceTree->subElements.at(sLayerJ);
                if( fResBranch!=NULL && fResBranch->get_type() == BRANCH_TYPE_LAYER )
                {
                    fLayer = (sceneLayer *)fResBranch;
                    if( availableLayerIds[ availI ]==fLayer->projectLayerId )
                    {
                        layerWasUsed = true;
                    }
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
        newLayer = new sceneLayer( newLayerId, projectParentFolder);
        if( newLayer!=NULL)
        {
            GPE_Main_Logs->log_other_line( CURRENT_PROJECT->projectLayerNames[newLayerId] );
            newLayer->set_name( CURRENT_PROJECT->projectLayerNames[newLayerId] );
            sceneResourceTree->add_branch(  newLayer, true );
            if( selectLayer )
            {
                sceneResourceTree->select_branch( newLayer );
                selectedSceneBranch = newLayer;
            }
        }
    }
    else
    {
        GPE_Report("New Layer NOT CREATED with id"+int_to_string(newLayerId) +"( already existed).");
    }
    return newLayer;
}

GPE_SceneBasicClass *  gameSceneResource::add_new_resource_from_menu()
{
    if( spm->layerListsDropDown!=NULL)
    {
        std::vector< int > availableLayerIds;
        int availI = 0;
        int sLayerJ = 0;
        bool layerWasUsed = false;
        for( availI = 0; availI < 32; availI++)
        {
            availableLayerIds.push_back(availI);
        }

        GPE_SpecialMenu_Branch * fResBranch = NULL;
        sceneLayer * fLayer = NULL;
        for( availI = (int)(availableLayerIds.size() )-1; availI >=0; availI--)
        {
            layerWasUsed = false;
            for( sLayerJ = (int)(sceneResourceTree->subElements.size() )-1; sLayerJ >=0; sLayerJ--)
            {
                fResBranch = sceneResourceTree->subElements.at(sLayerJ);
                if( fResBranch!=NULL && fResBranch->get_type() == BRANCH_TYPE_LAYER )
                {
                    fLayer = (sceneLayer *)fResBranch;
                    if( availableLayerIds[ availI ]==fLayer->projectLayerId )
                    {
                        layerWasUsed = true;
                    }
                }
            }
            if( layerWasUsed )
            {
                availableLayerIds.erase(availableLayerIds.begin()+availI);
            }
        }
        spm->layerListsDropDown->reset_suboptions();

        if( (int)(availableLayerIds.size() )> 0 )
        {
            for( availI = (int)(availableLayerIds.size() )-1; availI >=0; availI--)
            {
                spm->layerListsDropDown->add_menu_option(CURRENT_PROJECT->projectLayerNames[ availableLayerIds[availI] ], CURRENT_PROJECT->projectLayerNames[ availableLayerIds[availI] ],availableLayerIds[availI] );
            }
        }

        int newBranchType = spm->get_new_resource("Create New Scene Branch");

        if( newBranchType > BRANCH_TYPE_BASIC_SCENE_ELEMENT )
        {
            if( newBranchType == BRANCH_TYPE_LAYER )
            {
                if( spm->layerListsDropDown->get_selected_value() >= 0 )
                {
                    return add_layer( spm->layerListsDropDown->get_selected_value(), true );
                }
                return NULL;
            }
            else if( (int)spm->chosenName.size() > 0 )
            {
                seek_placeable_branch();

                //If the above code to dedicate a layer for this call failed, we abort
                if( selectedSceneBranch == NULL )
                {
                    return NULL;
                }

                int newBranchXPos = editorCameraRect.x + editorCameraRect.w/2;
                int newBranchYPos = editorCameraRect.y + editorCameraRect.h/2;

                switch( newBranchType )
                {
                    case BRANCH_TYPE_ANIMATION:
                    {
                        GPE_SceneAnimation * newAnimation = new GPE_SceneAnimation( projectParentFolder );
                        newAnimation->set_name( spm->chosenName );
                        newAnimation->set_position( newBranchXPos, newBranchYPos );
                        selectedSceneBranch->add_scene_branch( newAnimation, true, true );
                        return newAnimation;
                    }
                    break;


                    case BRANCH_TYPE_BACKGROUND:
                    {
                        GPE_SceneBackground * newSceneBackground = new GPE_SceneBackground( projectParentFolder );
                        newSceneBackground->set_name( spm->chosenName );
                        selectedSceneBranch->add_scene_branch( newSceneBackground, true, true );
                        return newSceneBackground;
                    }
                    break;

                    case BRANCH_TYPE_GROUP:
                    {
                        sceneBranchGroup * newBranchGroup = new sceneBranchGroup( spm->chosenName );
                        selectedSceneBranch->add_scene_branch( newBranchGroup, true, true );
                        return newBranchGroup;
                    }
                    break;

                    case BRANCH_TYPE_LIGHT:
                    {
                        GPE_SceneBasicLight * newLightSource = new GPE_SceneBasicLight( projectParentFolder );
                        newLightSource->set_name( spm->chosenName );
                        selectedSceneBranch->add_scene_branch( newLightSource, true, true );
                        return newLightSource;
                    }
                    break;

                    case BRANCH_TYPE_OBJECT:
                    {
                        GPE_SceneGameObject * newGameObject = new GPE_SceneGameObject(  );
                        newGameObject->set_name( spm->chosenName );
                        newGameObject->set_position( newBranchXPos, newBranchYPos );
                        selectedSceneBranch->add_scene_branch( newGameObject, true, true );
                        return newGameObject;
                    }
                    break;

                    case BRANCH_TYPE_PARTIClE_EMITTER:
                    {
                        GPE_SceneParticleEmitter * newParticleEmitter = new GPE_SceneParticleEmitter( projectParentFolder );
                        newParticleEmitter->set_name( spm->chosenName );
                        selectedSceneBranch->add_scene_branch( newParticleEmitter, true, true );
                        return newParticleEmitter;
                    }
                    break;

                    case BRANCH_TYPE_STEXT:
                    {
                        GPE_SceneText * newSText = new GPE_SceneText( projectParentFolder );
                        newSText->set_name( spm->chosenName );
                        newSText->set_position( newBranchXPos, newBranchYPos );
                        selectedSceneBranch->add_scene_branch( newSText, true, true );
                        return newSText;
                    }
                    break;

                    case BRANCH_TYPE_TEXT:
                    {
                        GPE_SceneMultilineText * newMText = new GPE_SceneMultilineText( projectParentFolder );
                        newMText->set_name( spm->chosenName );
                        newMText->checkWrapText->set_clicked( true );
                        newMText->set_position( newBranchXPos, newBranchYPos );
                        selectedSceneBranch->add_scene_branch( newMText, true, true );
                        return newMText;
                    }
                    break;

                    case BRANCH_TYPE_TILEMAP:
                    {
                        GPE_SceneTileMap * newTileMap = new GPE_SceneTileMap( spm->chosenName,0,0, projectParentFolder );
                        newTileMap->set_name( spm->chosenName );
                        selectedSceneBranch->add_scene_branch( newTileMap, true, true );
                        return newTileMap;
                    }
                    break;
                }
            }
        }
    }
    return NULL;
}

void gameSceneResource::adjust_object_offsets( GPE_SpecialMenu_Branch *  basicObj )
{
    //Avoids all getters and calculations if NULl or has an empty list
    if( basicObj == NULL)
    {
        return;
    }
    if( (int)basicObj->subElements.size()== 0 )
    {
        return;
    }

    int objectsAdjustedTotal = 0;
    sceneLayer * cLayer = NULL;
    GPE_SceneAnimation * cAnim = NULL;
    GPE_SceneGameObject * cObj = NULL;
    int jAnim = 0, jObj = 0;
    GPE_GeneralResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
    GPE_GeneralResourceContainer * fObjType = NULL;

    GPE_GeneralResourceContainer * allAnimationsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ANIMATION]+"s");
    GPE_GeneralResourceContainer * fAnimType = NULL;

    gameObjectResource * currentTypeObj = NULL;
    animationResource * currentTypeAnim= NULL;
    if( allObjsFolder==NULL || allAnimationsFolder==NULL)
    {
        GPE_Report("All allObjsFolder or allAnimationsFolder is null... aborting...");
        return;
    }
    GPE_SpecialMenu_Branch * iBranch = NULL;
    GPE_SceneBasicClass * iBasicClass = NULL;

    for( int iAdjust = basicObj->subElements.size()-1; iAdjust >=0; iAdjust--)
    {
        iBranch = basicObj->subElements.at(iAdjust);
        if( iBranch!=NULL && iBranch->get_type() > BRANCH_TYPE_BASIC_SCENE_ELEMENT )
        {
            if( iBranch->get_type() == BRANCH_TYPE_OBJECT )
            {
                cObj = (GPE_SceneGameObject * )iBranch;
                fObjType = allObjsFolder->find_resource_from_id( cObj->objTypeId );
                if( fObjType!=NULL && fObjType->get_held_resource()!=NULL )
                {
                    currentTypeObj = (gameObjectResource *) fObjType->get_held_resource();
                    if( currentTypeObj!=NULL && currentTypeObj->spriteIndex > 0  )
                    {
                        fAnimType = allAnimationsFolder->find_resource_from_id( currentTypeObj->spriteIndex );
                        if( fAnimType!=NULL && fAnimType->get_held_resource()!=NULL )
                        {
                            currentTypeAnim = (animationResource * )fAnimType->get_held_resource();
                            if( currentTypeAnim!=NULL && currentTypeAnim->animInEditor!=NULL && currentTypeAnim->animInEditor->colBox!=NULL )
                            {
                                 //cObj->xPos+=sceneGridX/2;
                                 //cObj->yPos+=sceneGridY/2;

                                cObj->xPos+=currentTypeAnim->animInEditor->colBox->get_center();
                                cObj->yPos+=currentTypeAnim->animInEditor->colBox->get_middle();
                                objectsAdjustedTotal++;
                            }
                        }
                        else
                        {
                            cObj->xPos+=sceneGridX/2;
                            cObj->yPos+=sceneGridY/2;
                        }
                    }
                    else
                    {
                        cObj->xPos+=sceneGridX/2;
                        cObj->yPos+=sceneGridY/2;
                    }
                    cObj->set_position( cObj->xPos,cObj->yPos );
                    //GPE_Report("New Coords ("+int_to_string(cObj->xPos)+","+int_to_string(cObj->yPos)+")..." );
                }
            }
            adjust_object_offsets( iBranch);
        }
    }
    GPE_Report("A total of ["+int_to_string(objectsAdjustedTotal)+"] objects were adjusted for ["+resourceName+"] scene." );
}

GPE_SceneTileMap *  gameSceneResource::add_tilemap( GPE_SceneBasicClass * branchHolder, std::string name, int x, int y, int w, int h,  int tW, int tH  )
{
    if( branchHolder!=NULL && branchHolder->layerParentId >=0 && branchHolder->layerParentId < 32 )
    {
        sceneLayer * layerHolder = find_layer( branchHolder->layerParentId );
        if( layerHolder!=NULL )
        {
            GPE_SceneTileMap * newMap = new GPE_SceneTileMap( name, x, y );
            newMap->tileWidth = tW;
            newMap->tileHeight = tH;
            newMap->create_new_map(w,h);
            branchHolder->add_scene_branch( newMap );
            return newMap;
        }
    }
    return NULL;
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
        //*fileTarget << nestedTabsStr << "var " << get_name() << " = " << exportBuildGlobalId << ";\n";
        //*fileTarget << nestedTabsStr << "var " << html5SceneName << " =  GPE.add_gamescene(); \n";

        *fileTarget << nestedTabsStr << "newGPEScene_000 = "+html5SceneName+";\n";

        *fileTarget << nestedTabsStr << html5SceneName << ".sceneId = " << exportBuildGlobalId << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".sceneName = \"" << get_name() << "\";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".sceneCaption = \"" << sceneCaptionField->get_string() << "\";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".sceneWidth = " << sceneRect.w << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".sceneHeight = " << sceneRect.h << ";\n";
        /*
        *fileTarget << nestedTabsStr << html5SceneName << ".defaultTileWidth  = " << ceil(defaultTileWidth) << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".defaultTileHeight = " << ceil(defaultTileHeight) << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".tileAmountX = " << floor(defaultTileAmountX) << ";\n";
        *fileTarget << nestedTabsStr << html5SceneName << ".tileAmountY = " << floor(defaultTileAmountY) << ";\n";
        */
        *fileTarget << nestedTabsStr << html5SceneName << ".bgColor = '#" << sceneBackgroundColor->get_hex_string() << "';\n";

        if(checkBoxIsContinuous!=NULL)
        {
            *fileTarget << nestedTabsStr << html5SceneName << ".sceneIsContinuous = " << checkBoxIsContinuous->is_clicked() << ";\n";
        }
        GPE_GeneralResourceContainer * fAudioId = NULL;
        if( musicAudioDropDown!=NULL)
        {
            fAudioId = musicAudioDropDown->get_selected_container();
            if( fAudioId!=NULL)
            {
                *fileTarget << nestedTabsStr << html5SceneName << ".bgMusic = " << fAudioId->exportBuildGlobalId << ";\n";
            }
        }
        if( startAudioDropDown!=NULL)
        {
            fAudioId = startAudioDropDown->get_selected_container();
            if( fAudioId!=NULL)
            {
                *fileTarget << nestedTabsStr << html5SceneName << ".startAudio = " << fAudioId->exportBuildGlobalId << ";\n";
            }
        }
        if( endAudioDropDown!=NULL)
        {
            fAudioId = endAudioDropDown->get_selected_container();
            if( fAudioId!=NULL)
            {
                *fileTarget << nestedTabsStr << html5SceneName << ".endAudio = " << fAudioId->exportBuildGlobalId << ";\n";
            }
        }
        GPE_SpecialMenu_Branch * tempBranch = NULL;
        GPE_SceneBasicClass * tempBasicSceneObj = NULL;
        sceneLayer * tempLayer = NULL;
        GPE_SceneTile* fSceneTile= NULL;
        int maxTilesInLayer = 0;
        GPE_SceneBackground * fSceneBg = NULL;
        int tempMax  = 0;

        *fileTarget << nestedTabsStr << "var newBranch = {};\n";
        for( iItr = 0; iItr < (int)sceneResourceTree->subElements.size(); iItr++)
        {
            tempBranch = sceneResourceTree->subElements[iItr];
            if( tempBranch!=NULL && tempBranch->get_type() >= BRANCH_TYPE_BASIC_SCENE_ELEMENT )
            {
                GPE_SceneBasicClass * tempBasicSceneObj = (GPE_SceneBasicClass * )tempBranch;
                tempBasicSceneObj->build_intohtml5_file(fileTarget,leftTabAmount+1,projectParentFolder );
            }
        }
        return true;
    }
    else
    {
        buildSuccessful = false;
    }
    return buildSuccessful;
}

bool gameSceneResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void gameSceneResource::compile_cpp()
{

}

sceneLayer * gameSceneResource::find_layer( int layerIdToFind )
{
    GPE_SpecialMenu_Branch * fResBranch = NULL;
    sceneLayer * fLayer = NULL;
    for( int i = (int)sceneResourceTree->subElements.size()-1; i >= 0; i-- )
    {
        fResBranch = sceneResourceTree->subElements.at(i);
        if( fResBranch!=NULL && fResBranch->get_type() == BRANCH_TYPE_LAYER )
        {
            fLayer = (sceneLayer *)fResBranch;
            if( fLayer->projectLayerId==layerIdToFind )
            {
                return fLayer;
            }
        }
    }
    return NULL;
}

bool gameSceneResource::find_scene_branch( GPE_SceneBasicClass * branchHolder, bool nestDown )
{
    if( branchHolder!=NULL )
    {
        GPE_SceneBasicClass * tempBranch;
        for( int i = (int)branchHolder->subElements.size()-1; i >=0; i-- )
        {
            if( branchHolder->subElements[i]!=NULL )
            {
                tempBranch = ( GPE_SceneBasicClass * )branchHolder->subElements[i];
                if( tempBranch!=NULL)
                {
                    if( tempBranch->under_mouse( sceneMouseXPos, sceneMouseYPos ) )
                    {
                        clickedSceneBranch = tempBranch;
                        selectedBranchId = clickedSceneBranch->get_global_id();
                        return true;
                    }
                    else if( nestDown && (int)tempBranch->subElements.size() > 0 )
                    {
                        if( find_scene_branch( tempBranch, true) )
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool gameSceneResource::find_scene_branches( GPE_SceneBasicClass * branchHolder, bool nestDown  )
{
    if( !clickMultipleBranches)
    {
        return find_scene_branch(branchHolder, nestDown);
    }
    bool returnVal = false;
    if( branchHolder!=NULL )
    {
        GPE_SceneBasicClass * tempBranch;
        for( int i = branchHolder->subElements.size()-1; i >=0; i-- )
        {
            if( branchHolder->subElements[i]!=NULL )
            {
                tempBranch = ( GPE_SceneBasicClass * )branchHolder->subElements[i];
                if( tempBranch!=NULL)
                {
                    if( tempBranch->under_mouse( sceneMouseXPos, sceneMouseYPos ) )
                    {
                        clickedSceneBranch = tempBranch;
                        selectedBranchId = clickedSceneBranch->get_global_id();
                        clickedSceneBranches.push_back( tempBranch);
                        returnVal =  true;
                    }
                    if( nestDown && (int)tempBranch->subElements.size() > 0 )
                    {
                        if( find_scene_branches( tempBranch, true) )
                        {
                            returnVal = true;
                        }
                    }
                }
            }
        }
    }
    return returnVal;
}

bool gameSceneResource::get_mouse_coords()
{
    sceneMouseXPos = 0;
    sceneMouseYPos = 0;

    if( point_within_rect(input->mouse_x,input->mouse_y, &editorView ) )
    {
        sceneMouseXPos = (input->mouse_x-editorView.x)/zoomValue + editorCameraRect.x;
        sceneMouseYPos = (input->mouse_y-editorView.y)/zoomValue + editorCameraRect.y;
        if( spm!=NULL)
        {
            spm->mouseInScene = true;
            spm->mouseXPos = sceneMouseXPos;
            spm->mouseYPos = sceneMouseYPos;
            if( sceneXScroll!=NULL && sceneYScroll!=NULL )
            {
                if( sceneXScroll->is_scrolling() || sceneYScroll->is_scrolling( ) )
                {
                    spm->mouseInScene = false;
                }
            }
        }
        return true;
    }
    if( spm!=NULL)
    {
        spm->mouseInScene = false;
        spm->mouseXPos = -1;
        spm->mouseYPos = -1;
    }
    return false;
}

void gameSceneResource::handle_scrolling()
{
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    bool editorScrolling = false;
    if( PANEL_GENERAL_EDITOR!=NULL  )
    {
        if( PANEL_GENERAL_EDITOR->hasScrollControl )
        {
            return;
        }
        editorScrolling = PANEL_GENERAL_EDITOR->is_inuse();
    }

    if( areaIsScrollable && ( editorMode!=LEDITOR_MODE_LAYERS || (  editorMode==LEDITOR_MODE_LAYERS/* && tSPreviewer->hasScrollControl==false*/)  ) )
    {
        bool scrollModeAllowed = true;
        if( shortcutButtonBar->get_tab_id()==SCENE_MODE_SCALE || shortcutButtonBar->get_tab_id()==SCENE_MODE_ROTATION )
        {
            scrollModeAllowed = false;
        }

        if( scrollModeAllowed )
        {
            if( input->check_keyboard_down(kb_ctrl)  )
            {
                //Zoom In
                if( input->mouseScrollingDown > 0 )
                {
                    zoomValue -= 0.0625;
                }
                else if( input->mouseScrollingUp)
                {
                    //zoom out
                    zoomValue += 0.0625;
                }
                if( zoomValue < ZOOM_LEVEL_MIN)
                {
                    zoomValue = ZOOM_LEVEL_MIN;
                }
                else if( zoomValue > ZOOM_LEVEL_MAX )
                {
                    zoomValue = ZOOM_LEVEL_MAX;
                }
            }
            else if( input->shiftKeyIsPressed)
            {
                if( input->mouseScrollingUp > 0 )
                {
                    //moves camera left
                    xScrollHappened = true;
                    editorCameraRect.x-=(editorCameraRect.w/16)*zoomValue;
                }
                else if( input->mouseScrollingDown)
                {
                    //moves camera right
                    xScrollHappened = true;
                    editorCameraRect.x+=(editorCameraRect.w/16)*zoomValue;
                }
            }
            else
            {
                if( input->mouseScrollingUp )
                {
                    yScrollHappened = true;
                    editorCameraRect.y-=(editorCameraRect.h/16)*zoomValue;
                }
                else if( input->mouseScrollingDown)
                {
                    yScrollHappened = true;
                    editorCameraRect.y+=(editorCameraRect.h/16)*zoomValue;
                }
                else if( editorScrolling = false )
                {
                    //arrow scrolling
                    if( input->check_keyboard_down(kb_up) )
                    {
                        yScrollHappened = true;
                        editorCameraRect.y-=(editorCameraRect.h/32)*zoomValue;
                    }
                    else if( input->check_keyboard_down(kb_down) )
                    {
                        yScrollHappened = true;
                        editorCameraRect.y+=(editorCameraRect.h/32)*zoomValue;
                    }
                    if( input->check_keyboard_down(kb_left) )
                    {
                        if( editorCameraRect.x > (editorCameraRect.w/32)*zoomValue )
                        {
                            xScrollHappened = true;
                            editorCameraRect.x-=(editorCameraRect.w/32)*zoomValue;
                        }
                        else
                        {
                            editorCameraRect.x = 0;
                            xScrollHappened = true;
                        }
                    }
                    else if( input->check_keyboard_down(kb_right) )
                    {
                        if( (editorCameraRect.x +editorCameraRect.w/32)*zoomValue < sceneRect.w*zoomValue )
                        {
                            xScrollHappened = true;
                            editorCameraRect.x+=(editorCameraRect.w/32)*zoomValue;
                        }
                    }
                }
            }
        }
    }


    if( editorCameraRect.x+editorCameraRect.w/zoomValue > sceneRect.w )
    {
        xScrollHappened = true;
        editorCameraRect.x = sceneRect.w-editorCameraRect.w/zoomValue;
    }

    if( editorCameraRect.y+editorCameraRect.h/zoomValue > sceneRect.h )
    {
        yScrollHappened = true;
        editorCameraRect.y = sceneRect.h-editorCameraRect.h/zoomValue;
    }

    if( editorCameraRect.x <= 0)
    {
        xScrollHappened = true;
        editorCameraRect.x = 0;
    }

    if( editorCameraRect.y <= 0)
    {
        yScrollHappened = true;
        editorCameraRect.y = 0;
    }

    if( xScrollHappened)
    {
        sceneXScroll->contextRect.x = editorCameraRect.x;
        sceneXScroll->process_self(NULL,NULL);
    }
    if( yScrollHappened)
    {
        sceneYScroll->contextRect.y = editorCameraRect.y;
        sceneYScroll->process_self(NULL,NULL);
    }
}

void gameSceneResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
    if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self( );
    }

    if( isometricCheckBox!=NULL)
    {
        isometricCheckBox->prerender_self( );
    }
    if( checkBoxIsContinuous!=NULL)
    {
        checkBoxIsContinuous->prerender_self( );
    }
    /*for( int i = 0; i < MAX_BACKGROUND_NUMBER; i++)
    {
        if( checkForeground[i]!=NULL)
        {
            checkForeground[i]->prerender_self( );
        }
        if( checkTileHori[i]!=NULL)
        {
            checkTileHori[i]->prerender_self( );
        }
        if( checkTileVert[i]!=NULL)
        {
            checkTileVert[i]->prerender_self( );
        }
        if( checkStretch[i]!=NULL)
        {
            checkStretch[i]->prerender_self( );
        }
    }
    */
    if( useObjGridCheckBox!=NULL)
    {
        useObjGridCheckBox->prerender_self( );
    }
    if( forceSnapButton!=NULL)
    {
        forceSnapButton->prerender_self( );
    }
}

void gameSceneResource::preprocess_self(std::string alternatePath)
{
    if( scnPostProcessed ==false || file_exists(alternatePath) )
    {
        editorCameraRect.x = 0;
        editorCameraRect.y = 0;
        editorCameraRect.w = 640;
        editorCameraRect.h = 480;
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Game Scene", resourceName );
        }

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
        //GPE_Report("Loading scene - "+newScnFilename);
        //If the level file could be loaded
        double foundFileVersion = 0;

        if( !gameScnFileIn.fail() )
        {
            //makes sure the file is open
            if (gameScnFileIn.is_open())
            {
                double foundXPos = 0;
                double foundYPos = 0;
                int equalPos = 0;
                int commaPos = 0;
                int newSceneLayerWantedId = 0;
                std::string firstChar="";
                std::string keyString="";
                std::string valString="";

                std::string objCustomKeyString="";
                std::string objCustomValString="";
                std::string subValString="";
                std::string currLine="";

                std::string customObjAllFieldData = "";
                std::string customObjEntryTag = "";
                std::string currLineToBeProcessed;
                int foundBgId = 0;
                int foundNumberToRead = 0;
                std::string foundBgName = "";
                std::string foundObjName = "";
                std::string foundNickname= "";
                std::string foundHexColor= "";
                int foundR = 0;
                int foundG = 0;
                int foundB = 0;
                bool olderVersionImportBegan = false;
                int foundLayerType = 0;
                int foundLayerId = 0;
                std::string foundLayerName = "";
                GPE_SceneBasicClass * recentObject = NULL;
                GPE_SceneBasicClass * newestObjectMade = NULL;
                GPE_SceneGameObject * newGameObject = NULL;
                GPE_SceneTile * sceneTileToEdit = NULL;
                GPE_SceneTileMap * mapToEdit = NULL;
                sceneBranchGroup * newSceneGroup = NULL;
                sceneLayer * newSceneLayer = NULL;
                sceneLayer * newBackgroundLayer = NULL;
                GPE_SceneBackground * newSceneBg = NULL;
                bool tileMapFillScene = false;
                int tileLayerXPos = 0, tileLayerYPos = 0, foundTilePos = 0;
                int foundTilesheetId = 0, foundTileSheetPos = 0;
                int foundTileWidth = 0, foundTileHeight = 0, foundTileAmountX = 0, foundTileAmountY = 0;
                std::string objectComponentField = "";
                int beginObjectComponentPos = -1;
                int endObjectComponentPos = -1;
                std::string componentTag = "[component]";
                int componentTagSize = (int)componentTag.size();
                GPE_GeneralGuiElement * newObjectComponent = NULL;
                GPE_ObjectComponent * tempObjectComponent = NULL;
                bool uniqeComponentsOpen = false;
                //GPE_TextAreaInputBasic * textAreaToLoad = NULL;
                GPE_GeneralResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
                GPE_GeneralResourceContainer * foundObjContainer = NULL;


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
                                        if( !compare_doubles(foundFileVersion, GPE_VERSION_DOUBLE_NUMBER) && foundFileVersion < GPE_VERSION_DOUBLE_NUMBER && !olderVersionImportBegan )
                                        {
                                            if( defaultBackgroundLayer == NULL)
                                            {
                                                defaultBackgroundLayer = add_retro_layer(LAYER_TYPE_BACKGROUND,0);
                                            }
                                            if( defaultObjectLayer == NULL)
                                            {
                                                defaultObjectLayer = add_retro_layer(LAYER_TYPE_OBJECTS,1);
                                            }
                                            olderVersionImportBegan = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion <= 2)
                    {
                        //Begin processing the file.
                        if(!currLineToBeProcessed.empty() )
                        {
                            //Processes the components found via the scene file
                            if( uniqeComponentsOpen )
                            {
                                if( currLineToBeProcessed=="[/GPECustomFields]")
                                {
                                    uniqeComponentsOpen = false;
                                }
                                else
                                {
                                    subValString = split_first_string(currLineToBeProcessed,"[/component]");

                                    beginObjectComponentPos=subValString.find_first_of(componentTag);
                                    if( beginObjectComponentPos!=(int)std::string::npos )
                                    {
                                        //if the beginObjectComponentPos is present, then parse on through and carryon
                                        objCustomValString = subValString.substr(beginObjectComponentPos+componentTagSize,subValString.length());
                                        newObjectComponent = add_gui_component(objCustomValString);
                                        tempObjectComponent = NULL;
                                        if( newObjectComponent!=NULL)
                                        {
                                            newObjectComponent->set_max_width(192);
                                            if( newestObjectMade!=NULL )
                                            {
                                                tempObjectComponent = newestObjectMade->add_variable(newObjectComponent);
                                            }
                                            else if( sceneComponents!=NULL )
                                            {
                                                tempObjectComponent = sceneComponents->add_variable(newObjectComponent);
                                            }
                                            if( tempObjectComponent==NULL)
                                            {
                                                //duplicate key added, therefore erase newly created gui element.
                                                delete newObjectComponent;
                                                newObjectComponent = NULL;
                                            }
                                        }
                                    }
                                }
                            }
                            else
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
                                    else if( keyString=="LevelWidth" || keyString=="SceneWidth" )
                                    {
                                        foundNumberToRead = string_to_int(valString,4096);
                                        if( levelPixelWidthField!=NULL)
                                        {
                                            levelPixelWidthField->set_string( int_to_string(foundNumberToRead) );
                                        }
                                        sceneRect.w = foundNumberToRead;
                                    }
                                    else if( keyString=="LevelHeight" || keyString=="SceneHeight" )
                                    {
                                        foundNumberToRead = string_to_int(valString,4096);
                                        if( levelPixelHeightField!=NULL)
                                        {
                                            levelPixelHeightField->set_string( int_to_string(foundNumberToRead) );
                                        }
                                        sceneRect.h = foundNumberToRead;
                                    }
                                    else if( keyString=="TileWidth"  || keyString=="DefaultTileWidth" )
                                    {
                                        foundNumberToRead = string_to_int(valString,32);
                                        if( defaultTileWidthField!=NULL)
                                        {
                                            defaultTileWidthField->set_string(int_to_string(foundNumberToRead) );
                                        }
                                        defaultTileWidth= foundNumberToRead;
                                        if(defaultTileWidth!=0)
                                        {
                                            defaultTileAmountX = ceil( std::abs( (double)sceneRect.w/defaultTileWidth) );
                                        }
                                    }
                                    else if( keyString=="TileHeight" || keyString=="DefaultTileHeight" )
                                    {
                                        foundNumberToRead = string_to_int(valString,32);
                                        if( defaultTileHeightField!=NULL)
                                        {
                                            defaultTileHeightField->set_string(int_to_string(foundNumberToRead) );
                                        }
                                        defaultTileHeight = foundNumberToRead;
                                        if(defaultTileHeight!=0)
                                        {
                                            defaultTileAmountY = ceil( std::abs( (double)sceneRect.h/defaultTileHeight) );
                                        }
                                    }
                                    else if( keyString=="Preload" && preloadCheckBox!=NULL)
                                    {
                                        preloadCheckBox->set_clicked( string_to_bool(valString) );
                                    }
                                    else if( keyString=="ViewTileGrid" && viewTileGridCheckBox!=NULL)
                                    {
                                        viewTileGridCheckBox->set_clicked( string_to_bool(valString) );
                                    }
                                    else if(keyString=="Isometric" && isometricCheckBox!=NULL)
                                    {
                                        isometricCheckBox->set_clicked( string_to_bool(valString) );
                                    }
                                    else if(keyString=="Continuous" && checkBoxIsContinuous!=NULL)
                                    {
                                        checkBoxIsContinuous->set_clicked( string_to_bool(valString) );
                                    }
                                    else if(keyString=="BGColor" && sceneBackgroundColor!=NULL)
                                    {
                                        sceneBackgroundColor->set_color_from_rgb( valString );
                                    }
                                   else if( keyString=="GridWidth" || keyString=="ObjectGridWidth" )
                                    {
                                        foundNumberToRead = string_to_int(valString,32);
                                        if( gridWidthField!=NULL)
                                        {
                                            gridWidthField->set_string(int_to_string(foundNumberToRead) );
                                        }
                                        sceneGridX= foundNumberToRead;
                                    }
                                    else if( keyString=="GridHeight" || keyString=="ObjectGridHeight")
                                    {
                                        foundNumberToRead = string_to_int(valString,32);
                                        if( gridHeightField!=NULL)
                                        {
                                            gridHeightField->set_string(int_to_string(foundNumberToRead) );
                                        }
                                        sceneGridY = foundNumberToRead;
                                    }
                                    else if( keyString=="GridColor" )
                                    {
                                        if( gridColorField!=NULL)
                                        {
                                            gridColorField->set_color_from_hex(valString );
                                        }
                                    }
                                    else if( keyString=="GridAlpha" )
                                    {
                                        foundNumberToRead = string_to_int(valString,228);
                                        if( gridAlphaField!=NULL)
                                        {
                                            gridAlphaField->set_string(int_to_string(foundNumberToRead) );
                                        }
                                    }
                                    else if( keyString=="[GPE_Layer")
                                    {
                                        newestObjectMade = NULL;
                                        //Version 1-> 1.21 method [ OUTDATED, but will load via backward compatibility ]
                                        foundLayerType = split_first_int(valString,',');
                                        foundLayerName = split_first_string(valString,',');
                                        if( (int)foundLayerName.size() > 0)
                                        {
                                            foundLayerId = string_to_int(foundLayerName, -1);
                                            if( foundLayerId >=0 && foundLayerId < 32)
                                            {
                                                newSceneLayer = add_retro_layer( foundLayerType,  foundLayerId );
                                                if( newSceneLayer!=NULL )
                                                {
                                                    recentObject = newSceneLayer;
                                                    newestObjectMade = newSceneLayer;
                                                    tileLayerXPos = 0;
                                                    tileLayerYPos = 0;
                                                }
                                                else
                                                {
                                                    GPE_Report("Unknown [GPE_Layer] tag Detected ["+foundLayerName+"] for "+resourceName+"...");
                                                }
                                            }
                                            else
                                            {
                                                GPE_Report("Invalid [GPE_Layer] tag Detected ["+foundLayerName+"] for "+resourceName+"...");
                                            }
                                            /*
                                            else if( CURRENT_PROJECT!=NULL)
                                            {

                                            }
                                            */
                                        }
                                        /*else
                                        {
                                            newSceneLayer = find_layer( string_to_int(subValString) );
                                            if( newSceneLayer!=NULL)
                                            {
                                                if(newSceneLayer->layerType!= foundLayerType)
                                                {
                                                    newSceneLayer = NULL;
                                                }
                                            }
                                        }*/

                                    }
                                    else if( keyString=="[GPE_LayerContainer")
                                    {
                                        newestObjectMade = NULL;
                                        //Version 1.3X and up method
                                        foundLayerId = split_first_int(valString,',');
                                        if( foundLayerId >=0 && foundLayerId < 32)
                                        {
                                            newSceneLayer = add_layer( foundLayerId );
                                            if( newSceneLayer!=NULL)
                                            {
                                                recentObject = newSceneLayer;
                                                newestObjectMade = newSceneLayer;
                                                tileLayerXPos = 0;
                                                tileLayerYPos = 0;
                                            }
                                            else
                                            {
                                                GPE_Report("Unknown Layer Detected ["+foundLayerName+"] for "+resourceName+"...");
                                            }
                                        }
                                        else
                                        {
                                            GPE_Report("Invalid Layer Detected ["+foundLayerName+"] for "+resourceName+"...");
                                        }
                                        /*
                                        else if( CURRENT_PROJECT!=NULL)
                                        {

                                        }
                                        */
                                    }
                                    else if( keyString=="[sceneLayer" || keyString=="[SceneTileLayer")
                                    {
                                        newestObjectMade = NULL;
                                        /*
                                        newSceneLayerWantedId = split_first_int(valString,']');
                                        newSceneLayer = find_layer(newSceneLayerWantedId);
                                        if( newSceneLayer==NULL)
                                        {
                                            newSceneLayer = add_retro_layer(LAYER_TYPE_TILES,newSceneLayerWantedId);
                                        }
                                        tileLayerXPos = 0;
                                        tileLayerYPos = 0;
                                        */
                                        newSceneLayer = add_retro_layer(LAYER_TYPE_TILES,-1);
                                        if( newSceneLayer != NULL )
                                        {
                                            recentObject = newSceneLayer;
                                            newestObjectMade = newSceneLayer;
                                        }
                                        tileLayerXPos = 0;
                                        tileLayerYPos = 0;
                                    }
                                    if( newSceneLayer!=NULL)
                                    {
                                        if( keyString=="GPEBackground" || keyString=="GPE_Background"  )
                                        {
                                            foundBgName = split_first_string(valString,',');
                                            newSceneBg = new GPE_SceneBackground(projectParentFolder);
                                            if( newSceneBg->backgroundInEditor!=NULL)
                                            {
                                                newSceneBg->backgroundInEditor->set_selected_target( foundBgName );
                                                newSceneBg->textureId = newSceneBg->backgroundInEditor->get_selected_id();
                                            }
                                            else
                                            {
                                                split_first_int(valString,',');
                                            }
                                            foundXPos =  split_first_int(valString,',');
                                            foundYPos =  split_first_int(valString,',');
                                            newSceneBg->set_position( foundXPos, foundYPos );
                                            newSceneBg->bgHorSpeedField->set_string(split_first_string(valString,','));
                                            newSceneBg->bgVertSpeedField->set_string(split_first_string(valString,','));
                                            newSceneBg->checkTileHori->set_clicked( string_to_bool( split_first_string(valString,',') ) );
                                            newSceneBg->checkTileVert->set_clicked( string_to_bool(split_first_string(valString,',') ) );
                                            newSceneBg->checkStretch->set_clicked( string_to_bool( split_first_string(valString,',') ) );
                                            newSceneBg->process_elements();//ReturnToLater

                                            //Added as of 1.22 to give backgrounds unique naming if wanted.
                                            foundNickname = split_first_string(valString,',');
                                            newSceneBg->set_name( foundNickname );
                                            if( recentObject!=NULL )
                                            {
                                                recentObject->add_scene_branch( newSceneBg, true, false );
                                            }
                                            else
                                            {
                                                newSceneLayer->add_scene_branch( newSceneBg, true, false );
                                            }
                                            newestObjectMade = newSceneBg;
                                        }
                                        else if( keyString=="GPE_SceneObject" )
                                        {
                                            //Version 1.22 and up object importing
                                            foundObjName = split_first_string(valString,',');
                                            newGameObject = new GPE_SceneGameObject();
                                            //newGameObject->localMenuBranch =
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
                                                GPE_Report("Unable to locate object["+foundObjName+"].");
                                            }
                                            newGameObject->set_name( newGameObject->objTypeName );
                                            foundXPos =  split_first_int(valString,',');
                                            foundYPos =  split_first_int(valString,',');
                                            newGameObject->set_position( foundXPos, foundYPos );
                                            newGameObject->set_angle( split_first_int(valString,',') );
                                            newGameObject->xScale = split_first_int(valString,',');
                                            newGameObject->yScale = split_first_int(valString,',');

                                            foundHexColor = split_first_string(valString,"," );
                                            newGameObject->branchColor->set_color_from_hex( foundHexColor );
                                            foundNickname = split_first_string(valString,",,,");
                                            newGameObject->branchNameField->set_string( foundNickname );
                                            if( (int)foundNickname.size( ) > 0 )
                                            {
                                                if( foundNickname!="unnamed")
                                                {
                                                    newGameObject->set_name( foundNickname );
                                                }
                                            }

                                            newGameObject->customObjId = customObjCreationId;
                                            customObjCreationId++;

                                            if( recentObject!=NULL)
                                            {
                                                recentObject->add_scene_branch( newGameObject );
                                            }
                                            else
                                            {
                                                //GPE_Report("Unable to add object["+foundObjName+"].");
                                                newSceneLayer->add_scene_branch( newGameObject );
                                            }
                                            newestObjectMade = newGameObject;
                                        }
                                        else if( keyString=="GPEObject" || keyString=="GPE_Object" )
                                        {
                                            //Version 1.21 and below object importing
                                            foundObjName = split_first_string(valString,',');
                                            newGameObject = new GPE_SceneGameObject();
                                            //newGameObject->localMenuBranch =
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
                                                GPE_Report("Unable to locate object["+foundObjName+"].");
                                            }
                                            newGameObject->set_name( newGameObject->objTypeName );
                                            foundXPos =  split_first_int(valString,',');
                                            foundYPos =  split_first_int(valString,',');
                                            newGameObject->set_position( foundXPos, foundYPos );
                                            newGameObject->set_angle( split_first_int(valString,',') );
                                            newGameObject->xScale = split_first_int(valString,',');
                                            newGameObject->yScale = split_first_int(valString,',');
                                            foundR = bound_number( split_first_int(valString,','), 0,255);
                                            foundG = bound_number( split_first_int(valString,','), 0,255);
                                            foundB = bound_number( split_first_int(valString,','), 0,255);
                                            newGameObject->branchColor->set_rgb(foundR,foundG,foundB);
                                            foundNickname = split_first_string(valString,",,,");
                                            newGameObject->branchNameField->set_string( foundNickname );
                                            if( (int)foundNickname.size( ) > 0 )
                                            {
                                                if( foundNickname!="unnamed")
                                                {
                                                    newGameObject->set_name( foundNickname );
                                                }
                                            }
                                            customObjAllFieldData = split_first_string(valString,"[/GPECustomFields]");

                                            customObjEntryTag = split_first_string(customObjAllFieldData,"[GPECustomFields]");
                                            while( (int)customObjAllFieldData.size() > 0)
                                            {
                                                subValString = split_first_string(customObjAllFieldData,"[/component]");

                                                beginObjectComponentPos=subValString.find_first_of(componentTag);
                                                if( beginObjectComponentPos!=(int)std::string::npos )
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
                                            if( recentObject!=NULL)
                                            {
                                                recentObject->add_scene_branch( newGameObject );
                                            }
                                            else
                                            {
                                                GPE_Report("Unable to add object["+foundObjName+"].");
                                                delete newGameObject;
                                                newGameObject = NULL;
                                            }
                                        }
                                        else if( keyString=="GameObject")
                                        {
                                            //Version 1.0 and below object importing
                                            /*if( foundGPEVersion==0.2)
                                            {
                                                foundNumberToRead = split_first_int(valString,',');
                                                if( foundNumberToRead> 0)
                                                {
                                                    newGameObject = new GPE_SceneGameObject();
                                                    newGameObject->objTypeId = foundNumberToRead;
                                                    newGameObject->xPos = split_first_int(valString,',');
                                                    newGameObject->yPos = split_first_int(valString,',');
                                                    newGameObject->angle = split_first_int(valString,',');
                                                    newGameObject->objXScale = split_first_int(valString,',');
                                                    newGameObject->objYScale = split_first_int(valString,',');
                                                    foundR = bound_number( split_first_int(valString,','), 0,255);
                                                    foundG = bound_number( split_first_int(valString,','), 0,255);
                                                    foundB = bound_number( split_first_int(valString,','), 0,255);
                                                    newGameObject->branchColor->change_rgba(foundR,foundG,foundB);
                                                    newGameObject->branchNameField >set_string(  split_first_string(valString,',') );
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
                                                    foundXPos =  split_first_int(valString,',');
                                                    foundYPos =  split_first_int(valString,',');
                                                    newGameObject->set_position( foundXPos, foundYPos );
                                                    newGameObject->set_angle( split_first_int(valString,',') );
                                                    newGameObject->xScale = split_first_int(valString,',');
                                                    newGameObject->yScale = split_first_int(valString,',');
                                                    foundR = bound_number( split_first_int(valString,','), 0,255);
                                                    foundG = bound_number( split_first_int(valString,','), 0,255);
                                                    foundB = bound_number( split_first_int(valString,','), 0,255);
                                                    newGameObject->branchColor->set_rgb(foundR,foundG,foundB);
                                                    newGameObject->branchNameField->set_string( split_first_string(valString,",,,") );
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
                                                        defaultObjectLayer = add_layer( 1);
                                                    }
                                                    if( defaultObjectLayer!=NULL)
                                                    {
                                                        newGameObject->layerParentId = defaultObjectLayer->layerParentId;
                                                        defaultObjectLayer->add_scene_branch( newGameObject );
                                                    }
                                                    else
                                                    {
                                                        delete newGameObject;
                                                        newGameObject = NULL;
                                                    }
                                                }
                                            }
                                        }
                                        else if(keyString=="Background" || keyString=="SceneBackground")
                                        {
                                            foundNumberToRead = split_first_int(valString,',');
                                            if( foundNumberToRead >=0  && foundNumberToRead < 8 )
                                            {
                                                foundBgId = split_first_int(valString,',');
                                                if( foundBgId >= 0 && defaultBackgroundLayer!=NULL )
                                                {
                                                    newSceneBg = new GPE_SceneBackground(projectParentFolder);
                                                    if( newSceneBg->backgroundInEditor!=NULL)
                                                    {
                                                        newSceneBg->backgroundInEditor->set_selection( foundBgId);
                                                        newSceneBg->textureId = newSceneBg->backgroundInEditor->get_selected_id();
                                                    }
                                                    else
                                                    {
                                                        split_first_int(valString,',');
                                                    }
                                                    foundXPos =  split_first_int(valString,',');
                                                    foundYPos =  split_first_int(valString,',');
                                                    newSceneBg->set_position( foundXPos, foundYPos );
                                                    newSceneBg->bgHorSpeedField->set_string(split_first_string(valString,','));
                                                    newSceneBg->bgVertSpeedField->set_string(split_first_string(valString,','));
                                                    split_first_string(valString,','); //previously used for checkForeground
                                                    newSceneBg->checkTileHori->set_clicked( string_to_bool( split_first_string(valString,',') ) );
                                                    newSceneBg->checkTileVert->set_clicked( string_to_bool(split_first_string(valString,',') ) );
                                                    newSceneBg->checkStretch->set_clicked( string_to_bool( split_first_string(valString,',') ) );
                                                    newSceneBg->process_elements();
                                                    defaultBackgroundLayer->add_branch(newSceneBg);
                                                }
                                                else
                                                {
                                                    GPE_Report("Background Detected out of range ["+int_to_string(foundBgId)+"]" );
                                                }
                                            }
                                            else
                                            {
                                                GPE_Report("Invalid Scene ID "+int_to_string(foundNumberToRead)+"Given for "+resourceName+".");
                                            }
                                        }
                                        else if( keyString=="[GPE_TileMap")
                                        {
                                            /*
                                            #todo
                                            recentObject = newTileMap;
                                            */
                                            foundTileWidth = split_first_int(valString,',');
                                            foundTileHeight = split_first_int(valString,',');
                                            foundTileAmountX = split_first_int(valString,',');
                                            foundTileAmountY = split_first_int(valString,',');
                                            tileLayerXPos= split_first_int(valString,',');
                                            tileLayerYPos = split_first_int(valString,',');

                                            tileMapFillScene = string_to_bool(split_first_string(valString,',') );
                                            foundNickname = split_first_string(valString,',');


                                            mapToEdit = new GPE_SceneTileMap( foundNickname, tileLayerXPos, tileLayerYPos , projectParentFolder);
                                            mapToEdit->fillScene->set_clicked( tileMapFillScene );
                                            mapToEdit->tileWidth = foundTileWidth;
                                            mapToEdit->tileHeight = foundTileHeight;
                                            mapToEdit->create_new_map( foundTileAmountX, foundTileAmountY );
                                            mapToEdit->set_name( foundNickname );

                                            if( recentObject!=NULL)
                                            {
                                                recentObject->add_scene_branch( mapToEdit );
                                            }
                                            else
                                            {
                                                //GPE_Report("Unable to add object["+foundObjName+"].");
                                                newSceneLayer->add_scene_branch( mapToEdit );
                                            }
                                            newestObjectMade = mapToEdit;
                                        }
                                        else if( keyString=="GPE_SingleTile" && mapToEdit!=NULL )
                                        {
                                            foundTilePos = split_first_int(valString,',');
                                            if( (int)mapToEdit->mapTiles.size() > foundTilePos )
                                            {
                                                sceneTileToEdit = mapToEdit->mapTiles.at(foundTilePos);
                                                if( sceneTileToEdit!=NULL)
                                                {
                                                    foundTilesheetId = split_first_int(valString,',');
                                                    foundTileSheetPos = split_first_int(valString,',');
                                                    sceneTileToEdit->tilesheetIndexId = foundTilesheetId;
                                                    sceneTileToEdit->tileIndexId = foundTileSheetPos;
                                                    sceneTileToEdit->tileTypeId = split_first_int(valString,',');
                                                }
                                                else
                                                {
                                                    GPE_Report("NULL Tile found @"+int_to_string(foundTilePos)+"..." );
                                                }
                                            }
                                            else
                                            {
                                                GPE_Report("Invalid Tile-Position given for map @"+int_to_string(foundTilePos)+"..." );
                                            }
                                        }
                                        else if( keyString=="[GPE_Group=")
                                        {
                                            /*
                                            #todo
                                            recentObject = newGroup;
                                            */
                                            foundNickname = split_first_string( valString, ",,,");
                                            newSceneGroup = new sceneBranchGroup(foundNickname);
                                            if(recentObject!=NULL && recentObject->get_type()==BRANCH_TYPE_LAYER || recentObject->get_type()==BRANCH_TYPE_GROUP )
                                            {
                                                recentObject->add_scene_branch( newSceneGroup);
                                            }
                                            else if( newSceneLayer!=NULL )
                                            {
                                                newSceneLayer->add_scene_branch( newSceneGroup);
                                            }
                                            recentObject = newSceneGroup;
                                            newestObjectMade = newSceneGroup;
                                        }
                                    }
                                }
                                else if( newSceneLayer!=NULL)
                                {
                                    if( currLineToBeProcessed=="[/sceneLayer]" || currLineToBeProcessed=="[/GPE_LayerContainer]" )
                                    {
                                        newestObjectMade = NULL;
                                        recentObject = NULL;
                                        newSceneLayer = NULL;
                                        tileLayerXPos = 0;
                                        tileLayerYPos = 0;
                                    }
                                    else if( currLineToBeProcessed=="[/GPE_TileMap]")
                                    {
                                        //#todo
                                        newestObjectMade = NULL;
                                        mapToEdit = NULL;
                                    }
                                    else if( currLineToBeProcessed=="[/GPE_Group]")
                                    {
                                        //#todo
                                        newestObjectMade = NULL;
                                        if( newSceneGroup!=NULL)
                                        {
                                            newSceneGroup->parentBranch;
                                        }
                                        else
                                        {
                                            recentObject = newSceneLayer;
                                        }
                                        newSceneGroup = NULL;
                                    }
                                    else if( currLineToBeProcessed=="[GPECustomFields]" )
                                    {
                                        uniqeComponentsOpen = true;
                                    }
                                    if( currLineToBeProcessed=="[/GPECustomFields]")
                                    {
                                        uniqeComponentsOpen = false;
                                    }
                                    else if( mapToEdit!=NULL )
                                    {
                                        tileLayerXPos = 0;
                                        commaPos=currLineToBeProcessed.find_first_of(",");
                                        while(commaPos!=(int)std::string::npos)
                                        {
                                            foundTilePos = tileLayerXPos+tileLayerYPos*mapToEdit->get_sizex();
                                            commaPos=currLineToBeProcessed.find_first_of(",");
                                            valString = split_first_string(currLineToBeProcessed,',');
                                            if( (int)mapToEdit->mapTiles.size() > foundTilePos && tileLayerXPos < mapToEdit->get_sizex() )
                                            {
                                                sceneTileToEdit = mapToEdit->mapTiles.at(foundTilePos);
                                                if( sceneTileToEdit!=NULL)
                                                {
                                                    if( (int)valString.size() > 2)
                                                    {
                                                        sceneTileToEdit->tilesheetIndexId = split_first_int(valString,'-');
                                                        sceneTileToEdit->tileIndexId = string_to_int(valString, -1 );
                                                    }
                                                }
                                            }
                                            tileLayerXPos += 1;
                                        }
                                        tileLayerYPos+=1;
                                    }
                                }
                                else if( currLineToBeProcessed=="[SceneGameObjects]" && defaultObjectLayer == NULL )
                                {
                                    defaultObjectLayer = add_retro_layer(LAYER_TYPE_OBJECTS,1);
                                }
                            }
                        }
                        else if( newSceneLayer!=NULL)
                        {
                            newSceneLayer = NULL;
                            tileLayerXPos = 0;
                            tileLayerYPos = 0;
                        }
                    }
                    else
                    {
                        GPE_Report("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }

            }
            gameScnFileIn.close();
        }
        if( foundFileVersion < GPE_SCENE_ALIGN_VERSION )
        {
            GPE_Report("Old Version found adjusting offsets...");
            adjust_object_offsets( sceneResourceTree );
        }
        scnPostProcessed = true;
        //GPE_Report("Scene loaded successfully...");
    }
}


void gameSceneResource::inherit_components( GPE_SceneBasicClass * objectToInherit, standardEditableGameResource * objParent)
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


void gameSceneResource::manage_components( bool processComponentsToo )
{
    if( PANEL_INSPECTOR!=NULL )
    {
        GPE_SceneBasicClass * classHolder = NULL;
        PANEL_INSPECTOR->add_gui_element( customComponentsTitle );
        if( editorMode==LEDITOR_MODE_SETTINGS )
        {
            classHolder = sceneComponents;
        }
        else if( selectedSceneBranch!=NULL)
        {
            classHolder = selectedSceneBranch;
        }

        if( classHolder!=NULL )
        {
            GPE_ObjectComponent * tComponent = NULL;

            for( int icv = 0; icv < (int)classHolder->objCustomValPairs.size(); icv++)
            {
                tComponent = classHolder->objCustomValPairs.at(icv);
                if( tComponent!=NULL)
                {
                    if( tComponent->get_component()!=NULL && tComponent->get_gear()!=NULL)
                    {
                        PANEL_INSPECTOR->add_gui_element( tComponent->get_component(),false );
                        PANEL_INSPECTOR->add_gui_element( tComponent->get_gear(),true );
                    }
                }
            }
            PANEL_INSPECTOR->add_gui_element( addNewComponentDropDown,true );
        }
        if( processComponentsToo )
        {
            PANEL_INSPECTOR->process_self();
            process_components();
        }
    }
}

void gameSceneResource::process_components()
{
    GPE_SceneBasicClass * classHolder = NULL;
    if( editorMode==LEDITOR_MODE_SETTINGS )
    {
        classHolder = sceneComponents;
    }
    else if( selectedSceneBranch!=NULL)
    {
        classHolder = selectedSceneBranch;
    }

    if( classHolder!=NULL )
    {
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
                        for( int iCompo = 0; iCompo < (int)classHolder->objCustomValPairs.size(); iCompo++)
                        {
                            myTempCustomComponent = classHolder->objCustomValPairs.at(iCompo);
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
                                    classHolder->objCustomValPairs.push_back(newComponent);
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
                input->reset_all_input();
            }
        }

        GPE_ObjectComponent * tempCustomComponent = NULL;
        GPE_ObjectComponent * otherTempCustomComponent = NULL;
        int iRealCompo = 0;
        int jRealCompo = 0;
        for(  iRealCompo = (int)classHolder->objCustomValPairs.size()-1; iRealCompo>=0; iRealCompo--)
        {
            tempCustomComponent = classHolder->objCustomValPairs.at(iRealCompo);
            if( tempCustomComponent!=NULL && tempCustomComponent->get_gear()!=NULL &&  tempCustomComponent->get_gear()->is_clicked() )
            {
                GPE_open_context_menu(-1,-1,256);
                MAIN_CONTEXT_MENU->add_menu_option("Rename Component",0,NULL,-1,NULL,true,true);
                MAIN_CONTEXT_MENU->add_menu_option("Delete Component",1,NULL,-1,NULL,false,true);
                if( tempCustomComponent->get_type()=="dropdown" || tempCustomComponent->get_type()=="selectbox")
                {
                    MAIN_CONTEXT_MENU->add_menu_option("Add Option",2,NULL,-1,NULL,false,true);
                    MAIN_CONTEXT_MENU->add_menu_option("Remove Option",3,NULL,-1,NULL,false,true);
                }

                MAIN_CONTEXT_MENU->add_menu_option("Exit Menu",4,NULL,-1,NULL,true,true);
                int menuSelection = GPE_Get_Context_Result();
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
                    if( GPE_Display_Basic_Prompt("[WARNING]Object Component deletion?","Are you sure you will like to delete the ["+tempCustomComponent->get_name()+"] component? This action is irreversible!")==DISPLAY_QUERY_YES)
                    {
                        //delete
                        delete tempCustomComponent;
                        tempCustomComponent = NULL;
                        classHolder->objCustomValPairs.erase( classHolder->objCustomValPairs.begin()+iRealCompo);
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
                //Rests input and manages the object's components where needed
                input->reset_all_input();
                break;
            }
        }
    }
}

void gameSceneResource::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    saveResourceButton->disable_self();
    preprocess_self();
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam==NULL || viewedSpace==NULL || sceneXScroll==NULL && sceneYScroll==NULL )
    {
        return;
    }
    spm->sWidth = sceneRect.w;
    spm->sHeight = sceneRect.h;
    mouseIsInScene = get_mouse_coords();

    //Checks for resources being dragged
    if( RESOURCE_TO_DRAG!=NULL )
    {
        if( RESOURCE_TO_DRAG->is_folder()==false  && RESOURCE_TO_DRAG->projectParentFileName.compare(CURRENT_PROJECT_NAME)==0 )
        {
            if( mouseIsInScene && input->check_mouse_released( mb_left) )
            {
                //Enter place mode, since the user seems to be in a placey mood...
                editorButtonBar->set_selection( LEDITOR_MODE_LAYERS );
                shortcutButtonBar->set_selection( SCENE_MODE_PLACE );

                if( RESOURCE_TO_DRAG->is_folder()==false && RESOURCE_TO_DRAG->projectParentFileName.compare(CURRENT_PROJECT_NAME)==0 )
                {
                    seek_placeable_branch();
                    if( RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_ANIMATION )
                    {
                        MAIN_OVERLAY->update_tooltip("Placing Animation");
                        GPE_SceneAnimation * newPlacedAnim = new GPE_SceneAnimation( projectParentFolder );
                        lastCreatedObjXPos = sceneObjMouseX;
                        lastCreatedObjYPos = sceneObjMouseY;
                        newPlacedAnim->set_position( sceneObjMouseX, sceneObjMouseY );
                        newPlacedAnim->animationId = RESOURCE_TO_DRAG->get_global_id();
                        newPlacedAnim->set_name(  RESOURCE_TO_DRAG->get_name() );

                        selectedSceneBranch->add_scene_branch( newPlacedAnim );
                        lastCreatedObjTypeId=RESOURCE_TO_DRAG->get_global_id();
                        selectedSceneObject = NULL;

                        if( objectEditorNameTitle!=NULL)
                        {
                            objectEditorNameTitle ->set_name( "None" );
                        }
                        //objectInEditor->set_selection(RESOURCE_TO_DRAG->get_global_id() );

                        animationResource* newAniRes = (animationResource*)RESOURCE_TO_DRAG->get_held_resource();
                        /*
                        if( newAniRes!=NULL )
                        {
                            inherit_components(newPlacedAnim,newAniRes);
                        }
                        newPlacedAnim->customObjId = customObjCreationId;
                        customObjCreationId++;*/

                        RESOURCE_TO_DRAG = NULL;
                    }
                    else if( RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_FONT )
                    {
                        MAIN_OVERLAY->update_tooltip("Placing Custom Text");
                        GPE_SceneText * newPlacedText = new GPE_SceneText();
                        lastCreatedObjXPos = sceneObjMouseX;
                        lastCreatedObjYPos = sceneObjMouseY;
                        newPlacedText->set_position( sceneObjMouseX, sceneObjMouseY );
                        newPlacedText->set_name( "Hello World");

                        selectedSceneBranch->add_scene_branch( newPlacedText );
                        lastCreatedObjTypeId=RESOURCE_TO_DRAG->get_global_id();
                        selectedSceneObject = NULL;

                        if( objectEditorNameTitle!=NULL)
                        {
                            objectEditorNameTitle ->set_name( "None" );
                        }
                        //objectInEditor->set_selection(RESOURCE_TO_DRAG->get_global_id() );

                        fontResource * newFontRes = (fontResource*)RESOURCE_TO_DRAG->get_held_resource();
                        newPlacedText->fontId = RESOURCE_TO_DRAG->get_global_id();
                        /*
                        if( newAniRes!=NULL )
                        {
                            inherit_components(newPlacedAnim,newAniRes);
                        }
                        newPlacedAnim->customObjId = customObjCreationId;
                        customObjCreationId++;*/

                        RESOURCE_TO_DRAG = NULL;
                    }
                    else if( RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_TILESHEET )
                    {

                    }
                    else if( RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_OBJECT )
                    {
                        MAIN_OVERLAY->update_tooltip("Placing Object");
                        GPE_SceneGameObject * newPlacedObject = new GPE_SceneGameObject();
                        lastCreatedObjXPos = sceneObjMouseX;
                        lastCreatedObjYPos = sceneObjMouseY;
                        newPlacedObject->set_position( sceneObjMouseX, sceneObjMouseY );
                        newPlacedObject->objTypeId = RESOURCE_TO_DRAG->get_global_id();
                        newPlacedObject->objTypeName = RESOURCE_TO_DRAG->get_name();

                        selectedSceneBranch->add_scene_branch( newPlacedObject );
                        lastCreatedObjTypeId=RESOURCE_TO_DRAG->get_global_id();
                        selectedSceneObject = NULL;

                        if( objectEditorNameTitle!=NULL)
                        {
                            objectEditorNameTitle ->set_name( "None" );
                        }
                        //objectInEditor->set_selection(RESOURCE_TO_DRAG->get_global_id() );

                        gameObjectResource* newObjRes = (gameObjectResource*)RESOURCE_TO_DRAG->get_held_resource();
                        if( newObjRes!=NULL )
                        {
                            inherit_components(newPlacedObject,newObjRes);
                        }
                        newPlacedObject->customObjId = customObjCreationId;
                        customObjCreationId++;
                        RESOURCE_TO_DRAG = NULL;
                    }
                    else if( RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_PATH )
                    {

                    }
                    else if( RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_TEXTURE )
                    {
                        MAIN_OVERLAY->update_tooltip("Placing Background");
                        GPE_SceneBackground * newPlacedBg = new GPE_SceneBackground( projectParentFolder );
                        newPlacedBg->backgroundInEditor->set_selection( RESOURCE_TO_DRAG->get_global_id() );
                        lastCreatedObjXPos = sceneObjMouseX;
                        lastCreatedObjYPos = sceneObjMouseY;
                        newPlacedBg->set_position( sceneObjMouseX, sceneObjMouseY );
                        newPlacedBg->set_name( RESOURCE_TO_DRAG->get_name() );

                        selectedSceneBranch->add_scene_branch( newPlacedBg );
                        lastCreatedObjTypeId=RESOURCE_TO_DRAG->get_global_id();
                        selectedSceneObject = NULL;

                        if( objectEditorNameTitle!=NULL)
                        {
                            objectEditorNameTitle ->set_name( "None" );
                        }
                        //objectInEditor->set_selection(RESOURCE_TO_DRAG->get_global_id() );

                        textureResource* newBGRes = (textureResource*)RESOURCE_TO_DRAG->get_held_resource();
                        /*
                        if( newAniRes!=NULL )
                        {
                            inherit_components(newPlacedAnim,newAniRes);
                        }
                        newPlacedAnim->customObjId = customObjCreationId;
                        customObjCreationId++;*/
                        RESOURCE_TO_DRAG = NULL;
                    }
                    else if( RESOURCE_TO_DRAG->get_resource_type()==RESOURCE_TYPE_TILESHEET )
                    {

                    }
                }
                else
                {
                    GPE_Report("Unable to place dragged resource ["+RESOURCE_TO_DRAG->projectParentFileName+"] with ["+CURRENT_PROJECT_NAME+"]...");
                }
                RESOURCE_TO_DRAG = NULL;
            }
        }
    }
    if( input->check_mouse_released( mb_anybutton)  )
    {
        sceneInFocus = mouseIsInScene;
    }

    GPE_SpecialMenu_Branch * fBranch = NULL;
    GPE_SceneBasicClass * fBasicElement = NULL;
    sceneLayer * fSceneLayer  = NULL;

    //Top Bar Edits and Mode Toggles
    sceneTopBarList->set_coords(0, 0 );
    sceneTopBarList->set_width( viewedSpace->w );
    sceneTopBarList->set_height( 32 );
    sceneTopBarList->barXPadding = GENERAL_GPE_PADDING*2;
    sceneTopBarList->barYPadding = 0;
    sceneTopBarList->barXMargin = 0;
    sceneTopBarList->barYMargin = 0;
    sceneTopBarList->set_horizontal_align( FA_LEFT );
    sceneTopBarList->clear_list();

    int prevEditorMode = editorMode;
    sceneTopBarList->add_gui_element(editorButtonBar, false );

    if( editorMode == LEDITOR_MODE_LAYERS )
    {
        sceneTopBarList->add_gui_element(shortcutButtonBar, false );
        //Arranged alphabetically by type
        sceneTopBarList->add_gui_element( gridToggleButtton, false );
        sceneTopBarList->add_gui_element( rotationButton, false );

        sceneTopBarList->add_gui_element( objectLockStateButton, false );
    }
    else
    {
        sceneTopBarList->add_gui_element( gridToggleButtton, false );
    }
    sceneTopBarList->add_gui_element( lightingStateButton, false );

    objectLockStateButton->set_clicked( false );
    rotationButton->set_clicked( false );
    lightingStateButton->set_clicked( false );
    sceneTopBarList->process_self(viewedSpace, cam );
    bool openBranchRoationMenu = false;
    if( gridToggleButtton->is_clicked() )
    {
        useSceneGrid = !useSceneGrid;
        useObjGridCheckBox->set_clicked( useSceneGrid );
    }
    else if( rotationButton->is_clicked() )
    {
        openBranchRoationMenu = true;
    }
    else if( sceneInFocus && !input->check_keyboard_down(kb_ctrl)  )
    {
        if( input->check_keyboard_released(kb_g) )
        {
            useSceneGrid = !useSceneGrid;
            useObjGridCheckBox->set_clicked( useSceneGrid );
        }
        else if( input->check_keyboard_released(kb_h) )
        {
            openBranchRoationMenu = true;
        }
        else if( input->check_keyboard_released(kb_l) )
        {
            swap_lighting( );
        }
        else if( input->check_keyboard_released(kb_q) )
        {
            shortcutButtonBar->set_selection( SCENE_MODE_SELECT );
        }
        else if( input->check_keyboard_released(kb_w) )
        {
            shortcutButtonBar->set_selection( SCENE_MODE_PLACE );
        }
        else if( input->check_keyboard_released(kb_e) )
        {
            shortcutButtonBar->set_selection( SCENE_MODE_ERASE );
        }
        else if( input->check_keyboard_released(kb_r) )
        {
            shortcutButtonBar->set_selection( SCENE_MODE_ROTATION);
        }
        else if( input->check_keyboard_released(kb_t) )
        {
            shortcutButtonBar->set_selection( SCENE_MODE_MOVE );
        }
        else if( input->check_keyboard_released(kb_y) )
        {
            shortcutButtonBar->set_selection( SCENE_MODE_SCALE );
        }
    }
    if( spm!=NULL )
    {
        spm->editMode = shortcutButtonBar->get_tab_id();
    }
    gpeEditorDockPanel * gridSettingsPanel = NULL;
    if( GPE_DOCK!=NULL)
    {
        gridSettingsPanel = GPE_DOCK->find_panel("Grid Settings");
    }
    if( gridSettingsPanel!=NULL )
    {
        gridSettingsPanel->add_gui_element(useObjGridCheckBox, true);
        gridSettingsPanel->add_gui_element(gridWidthField, true);
        gridSettingsPanel->add_gui_element(gridHeightField, true);
        gridSettingsPanel->add_gui_element(gridColorField, true);
        gridSettingsPanel->add_gui_element(gridAlphaField, true);
        gridSettingsPanel->add_gui_element(forceSnapButton, true);
        gridSettingsPanel->process_self();
        if( useObjGridCheckBox!=NULL)
        {
            useSceneGrid = useObjGridCheckBox->is_clicked();
        }
    }

    if( openBranchRoationMenu && selectedSceneBranch!=NULL)
    {
        GPE_open_context_menu();
        MAIN_CONTEXT_MENU->add_menu_option("Rotate Left 90 Degrees",1);
        MAIN_CONTEXT_MENU->add_menu_option("Rotate Right 90 Degrees",2);
        MAIN_CONTEXT_MENU->add_menu_option("Rotate Left 180 Degrees",3);
        MAIN_CONTEXT_MENU->add_menu_option("Rotate Right 180 Degrees",4);
        MAIN_CONTEXT_MENU->add_menu_option("Flip Horizontal",5);
        MAIN_CONTEXT_MENU->add_menu_option("Flip Vertical",6);
        int rotationOption = GPE_Get_Context_Result(NULL, true );
        if( rotationOption == 1 )
        {
            selectedSceneBranch->set_angle( selectedSceneBranch->angle + 90 );
        }
        else if( rotationOption == 2 )
        {
            selectedSceneBranch->set_angle( selectedSceneBranch->angle - 90 );
        }
        else if( rotationOption == 3 )
        {
            selectedSceneBranch->set_angle( selectedSceneBranch->angle - 180 );
        }
        else if( rotationOption == 4 )
        {
            selectedSceneBranch->set_angle( selectedSceneBranch->angle + 180 );
        }
        else if( rotationOption == 5 )
        {
            selectedSceneBranch->xScale*=-1.d;
        }
        else if( rotationOption==6 )
        {
            selectedSceneBranch->yScale*=-1.d;
        }
        GPE_close_context_menu();
    }
    scenePane.x = 0;
    scenePane.y = sceneTopBarList->get_height();

    editorView.x = viewedSpace->x+scenePane.x;
    editorView.y = viewedSpace->y+sceneTopBarList->get_height();

    editorCameraRect.w = scenePane.w = (int)(viewedSpace->w -sceneYScroll->get_box_width() );
    editorView.w = scenePane.w;

    scenePane.h = viewedSpace->h;
    editorCameraRect.h = editorView.h = scenePane.h  - sceneXScroll->get_height();

    if( editorButtonBar!=NULL)
    {
        editorMode = editorButtonBar->get_tab_id();
    }

    //If the editor mode changed reset the panels and such
    if( prevEditorMode!=editorMode)
    {
        input->reset_all_input();
        if( PANEL_INSPECTOR!=NULL )
        {
            PANEL_INSPECTOR->clear_panel();
            PANEL_INSPECTOR->process_self();
        }
        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            PANEL_GENERAL_EDITOR->clear_panel();
            PANEL_GENERAL_EDITOR->process_self();
        }
        if( editorMode >=0 && editorMode < 5)
        {
            sceneEditorSubTitle->set_name(DEFAULT_SCENE_SUBEDITOR_NAMES[editorMode]);
        }
        unselect_object();
    }



    editorCameraRect.h = editorView.h = viewedSpace->h-shortcutButtonBar->get_height() - sceneXScroll->get_height();


    //Checks if the mouse is in the scene for all editor related tasks below
    mouseIsInScene = get_mouse_coords();

    if( editorMode == LEDITOR_MODE_LAYERS )
    {
        //Sets the x/y coordinates to snap to based on the default grid width/heights
        sceneGridX = std::max( 8, gridWidthField->get_held_int() );
        sceneGridY = std::max( 8, gridHeightField->get_held_int() );

        sceneGridX = std::min( 512, gridWidthField->get_held_int() );
        sceneGridY = std::min( 512, gridHeightField->get_held_int() );

        if( mouseIsInScene )
        {
            sceneObjMouseX  = sceneMouseXPos;
            sceneObjMouseY  = sceneMouseYPos;

            if( useObjGridCheckBox!=NULL)
            {
                useSceneGrid = useObjGridCheckBox->is_clicked();
            }

            if( useSceneGrid && sceneGridX!=0 && sceneGridY!=0)
            {
                sceneObjMouseX = sceneObjMouseX/sceneGridX;
                sceneObjMouseX = floor( sceneObjMouseX );
                sceneObjMouseX = sceneObjMouseX * sceneGridX+ sceneGridX/2;
                sceneObjMouseY = sceneObjMouseY/sceneGridY;
                sceneObjMouseY = floor( sceneObjMouseY );
                sceneObjMouseY = sceneObjMouseY * sceneGridY + sceneGridY/2;
            }
        }

        if( PANEL_INSPECTOR!=NULL )
        {
            PANEL_INSPECTOR->panelGuiList->set_horizontal_align(FA_LEFT);
        }

        //Clicks on object in scene if possible
        clickedSceneBranch = NULL;
        clickedSceneBranches.clear();
        if( shortcutButtonBar->get_tab_id() == SCENE_MODE_SELECT  )
        {
            if( mouseIsInScene && ( input->check_mouse_pressed( mb_left ) || input->check_mouse_pressed( mb_right ) ) )
            {
                int i = 0;
                int maxElements = (int)sceneResourceTree->subElements.size();
                for( i = 0; i < maxElements; i++)
                {
                    fBranch = sceneResourceTree->subElements.at(i);
                    if( fBranch!=NULL && fBranch->get_type() >= BRANCH_TYPE_BASIC_SCENE_ELEMENT )
                    {
                        fBasicElement = (GPE_SceneBasicClass *)fBranch;
                        if( clickMultipleBranches )
                        {
                            find_scene_branches( fBasicElement, true);
                        }
                        else
                        {
                            if( find_scene_branch( fBasicElement, true ) )
                            {
                                 break;
                            }
                        }
                    }
                }

                if( sceneResourceTree!=NULL && clickedSceneBranch!=NULL )
                {
                    sceneResourceTree->select_branch( clickedSceneBranch );
                }
            }
        }

        //Handles resource tree
        int previousSelectedId = sceneResourceTree->selectedSubOption;
        bool selectionChanged = false;
        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            PANEL_GENERAL_EDITOR->add_gui_element_fullsize( sceneResourceTree );
            PANEL_GENERAL_EDITOR->process_self(NULL, NULL);
            if( previousSelectedId!= sceneResourceTree->selectedSubOption )
            {
                selectionChanged = true;
            }
            if( sceneResourceTree->add_button_clicked() )
            {
                GPE_SceneBasicClass * newResource = add_new_resource_from_menu();
            }
            else if( sceneResourceTree->rightClickedId > 0 && sceneResourceTree->selectedBranch!=NULL )
            {
                GPE_open_context_menu();
                MAIN_CONTEXT_MENU->add_menu_option("Add Group", 1);
                MAIN_CONTEXT_MENU->add_menu_option("Add Sub-Branch", 2);
                MAIN_CONTEXT_MENU->add_menu_option("Duplicate Branch ( disabled )", 3);
                MAIN_CONTEXT_MENU->add_menu_option("Delete Branch", 4);
                MAIN_CONTEXT_MENU->add_menu_option("Change Branch Type ( disabled )", 5);
                if( sceneResourceTree->selectedBranch->get_type() !=BRANCH_TYPE_LAYER )
                {
                    MAIN_CONTEXT_MENU->add_menu_option("Rename Branch", 6);
                }
                MAIN_CONTEXT_MENU->add_menu_option("Move Up", 7 );
                MAIN_CONTEXT_MENU->add_menu_option("Move Down", 8 );
                MAIN_CONTEXT_MENU->add_menu_option("Branch Properties", 9 );
                int menuResult = GPE_Get_Context_Result();
                GPE_close_context_menu();

                switch( menuResult )
                {
                    case 6:
                        std::string newBranchName =get_string_from_popup("Rename branching...","Enter new name below", sceneResourceTree->selectedBranch->get_name() );
                        if( (int)newBranchName.size() > 0 )
                        {
                            sceneResourceTree->selectedBranch->set_name( newBranchName );
                        }
                    break;
                }
            }
            sceneResourceTree->rightClickedId = -1;
        }

        if( sceneResourceTree->selectedBranch!=NULL )
        {
            selectedSceneBranch = ( GPE_SceneBasicClass* )sceneResourceTree->selectedBranch;
        }
        else
        {
            selectedSceneBranch = NULL;
        }

        if( selectedSceneBranch!=NULL )
        {
            sceneLayer * currentLayer = find_layer( selectedSceneBranch->layerParentId );

            selectedBranchLabel->set_name( selectedSceneBranch->get_name() );
            if( addGroupButton!=NULL )
            {
                addGroupButton->set_clicked( false );
            }
            if( addTileMapButton!=NULL )
            {
                addTileMapButton->set_clicked( false );
            }

            GPE_SceneTileMap * selectedLayerMap = NULL;
            GPE_SceneText * selectedTextEntity = NULL;
            GPE_SceneMultilineText * selectedMLTextEntity = NULL;
            GPE_SceneAnimation * selectedAnimHolder = NULL;
            GPE_SceneBackground * selectedBgHolder = NULL;
            sceneLayer * selectedLayerHolder = NULL;
            sceneBranchGroup * selectedGroupHolder = NULL;
            bool isLayerOrGroup = false;
            if( selectedSceneBranch->get_type() == BRANCH_TYPE_LAYER || selectedSceneBranch->get_type() == BRANCH_TYPE_GROUP )
            {
                isLayerOrGroup = true;
            }
            //Swaps lock button-state
            if( !selectionChanged && objectLockStateButton!=NULL && objectLockStateButton->is_clicked() )
            {
                selectedSceneBranch->isLocked = !selectedSceneBranch->isLocked;
            }
            swap_lock( selectedSceneBranch->isLocked  );

            if( PANEL_INSPECTOR!=NULL)
            {
                if( selectedSceneBranch->get_type() > BRANCH_TYPE_BASIC_SCENE_ELEMENT && selectedSceneBranch->get_type() < BRANCH_TYPE_MAX_TYPE )
                {
                    PANEL_INSPECTOR->clear_panel();
                    selectedSceneBranch->add_basic_elements();
                    selectedSceneBranch->add_typed_elements();
                    manage_components( true );
                }
                //Process different branch types.
                else
                {
                    objectLockStateButton->set_clicked( false );
                    //Shows error message if the scene branch has a weird type...
                    PANEL_INSPECTOR->clear_panel();
                    PANEL_INSPECTOR->add_gui_element(layerErrorMessage, true );
                    PANEL_INSPECTOR->process_self(viewedSpace, cam);
                    //means that something is simply broken in editor code here
                }
            }
            else
            {
                objectLockStateButton->set_clicked( false );
            }
            if( selectedSceneBranch->get_type() > BRANCH_TYPE_BASIC_SCENE_ELEMENT && selectedSceneBranch->get_type() < BRANCH_TYPE_MAX_TYPE )
            {
                selectedSceneBranch->process_elements();
            }
            //Now lets play around in the scene some
            if( mouseIsInScene && RESOURCE_TO_DRAG!=NULL )
            {
                if( shortcutButtonBar->get_tab_id()== SCENE_MODE_SELECT && isLayerOrGroup  )
                {

                }
                else if( shortcutButtonBar->get_tab_id()== SCENE_MODE_PLACE && isLayerOrGroup && currentLayer!=NULL )
                {
                    int objTypeBeingPlaced = objectInEditor->get_selected_id();
                    if( objTypeBeingPlaced > 0)
                    {
                        if( input->check_mouse_down( mb_left ) )
                        {
                            seek_placeable_branch();
                            //Makes sure we are not stacking identical objects on each other
                            if( ( lastCreatedObjXPos < 0 || lastCreatedObjYPos < 0 || lastCreatedObjTypeId < 0) || sceneObjMouseX!=lastCreatedObjXPos && lastCreatedObjXPos|| sceneObjMouseY!=lastCreatedObjYPos || lastCreatedObjTypeId!=objTypeBeingPlaced )
                            {
                                MAIN_OVERLAY->update_tooltip("Placing Object");
                                GPE_SceneGameObject * newPlacedObject = new GPE_SceneGameObject();
                                newPlacedObject->set_position( sceneObjMouseX, sceneObjMouseY);
                                lastCreatedObjXPos = sceneObjMouseX;
                                lastCreatedObjYPos = sceneObjMouseY;
                                newPlacedObject->objTypeId = objTypeBeingPlaced;

                                selectedSceneBranch->add_scene_branch( newPlacedObject );

                                lastCreatedObjTypeId=objTypeBeingPlaced;
                                selectedSceneObject = NULL;
                                if( objectEditorNameTitle!=NULL)
                                {
                                    objectEditorNameTitle ->set_name( "None");
                                }
                                GPE_GeneralResourceContainer * objContainer = objectInEditor->get_selected_container();
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
                            }
                        }
                    }
                }
                else if( !selectedSceneBranch->isLocked && shortcutButtonBar->get_tab_id()== SCENE_MODE_ROTATION && !isLayerOrGroup  )
                {

                    //Temporarily disabled until I fix this calculation
                    if( input->check_mouse_down( mb_left ) )
                    {
                        selectedSceneBranch->set_angle( round( gpe->get_direction( selectedSceneBranch->xPos,selectedSceneBranch->yPos,sceneMouseXPos,sceneMouseYPos ) ) );
                    }
                    else
                    {
                        if( input->mouseScrollingUp )
                        {
                            selectedSceneBranch->set_angle( selectedSceneBranch->angle + 5 );
                        }
                        else if( input->mouseScrollingDown )
                        {
                            selectedSceneBranch->set_angle( selectedSceneBranch->angle - 5 );
                        }
                    }
                }
                else if( !selectedSceneBranch->isLocked && shortcutButtonBar->get_tab_id()== SCENE_MODE_SCALE && !isLayerOrGroup  )
                {
                    bool scaleChanged = false;
                    if( input->mouseScrollingUp )
                    {
                        selectedSceneBranch->xScale-=0.0625;
                        selectedSceneBranch->yScale-=0.0625;
                        scaleChanged = true;
                    }
                    else if( input->mouseScrollingDown )
                    {
                        selectedSceneBranch->xScale +=0.0625;
                        selectedSceneBranch->yScale +=0.0625;
                        scaleChanged = true;
                    }
                    if( scaleChanged )
                    {
                        if( selectedSceneBranch->xScale > 16.d )
                        {
                            selectedSceneBranch->xScale = 16.d;
                        }
                        if( selectedSceneBranch->xScale < -16.d )
                        {
                            selectedSceneBranch->xScale = -16.d;
                        }

                        if( selectedSceneBranch->yScale > 16.d )
                        {
                            selectedSceneBranch->yScale = 16.d;
                        }
                        if( selectedSceneBranch->yScale < -16.d)
                        {
                            selectedSceneBranch->yScale = -16.d;
                        }
                        selectedSceneBranch->xScaleField->set_number( selectedSceneBranch->xScale );
                        selectedSceneBranch->yScaleField->set_number( selectedSceneBranch->yScale );
                    }
                }
                else if( shortcutButtonBar->get_tab_id()== SCENE_MODE_MOVE  && !selectedSceneBranch->isLocked && !isLayerOrGroup  )
                {
                    if( input->check_mouse_released( mb_left) )
                    {
                        isDraggingObject = false;
                        selectedSceneBranch->isBeingMoved = false;
                        draggingOffsetX = draggingOffsetY = 0;
                    }
                    else if( selectedSceneBranch->isBeingMoved )
                    {
                        if( input->check_mouse_down( mb_left ) )
                        {
                            if( useSceneGrid)
                            {
                                selectedSceneBranch->set_position( sceneObjMouseX , sceneObjMouseY  );
                            }
                            else
                            {
                                selectedSceneBranch->set_position( sceneObjMouseX + draggingOffsetX, sceneObjMouseY + draggingOffsetY );
                            }
                            isDraggingObject = true;
                            selectedSceneBranch->isBeingMoved = true;
                        }
                    }
                    else if( input->check_mouse_pressed( mb_left ) )
                    {
                        if( selectedSceneBranch->under_mouse(sceneMouseXPos,sceneMouseYPos ) )
                        {
                            isDraggingObject = true;
                            selectedSceneBranch->isBeingMoved = true;
                            draggingOffsetX = selectedSceneBranch->xPos - sceneMouseXPos;
                            draggingOffsetY = selectedSceneBranch->yPos - sceneMouseYPos;
                            /*
                            selectedSceneBranch->xPos = sceneObjMouseX - selectedSceneBranch->xOffset;
                            selectedSceneBranch->yPos = sceneObjMouseX - selectedSceneBranch->xOffset;
                            selectedObjXPos->set_number(selectedSceneBranch->xPos );
                            selectedObjYPos->set_number(yPos );
                            */
                        }
                    }
                }
                else if( shortcutButtonBar->get_tab_id()== SCENE_MODE_ERASE  )
                {
                    //delete objscts
                    if( input->check_mouse_down( mb_right ) && sceneXScroll->is_scrolling()==false && sceneYScroll->is_scrolling()==false )
                    {
                        //reset_placement_info();
                    }
                }
            }

            if( addGroupButton!=NULL && addGroupButton->is_clicked() )
            {
                std::string newBranchName =get_string_from_popup("Create a new Group Branch","Branch name","" );
                if( newBranchName.size() > 0 )
                {
                    sceneBranchGroup * newBranchGroup = new sceneBranchGroup( newBranchName );
                    selectedSceneBranch->add_scene_branch( newBranchGroup, true, true );
                }
            }
            else if( addTileMapButton!=NULL && addTileMapButton->is_clicked() )
            {
                std::string newBranchName =get_string_from_popup("Create a new TileMap","Branch name","" );
                if( newBranchName.size() > 0 )
                {
                    GPE_SceneTileMap * newTileMap = new GPE_SceneTileMap( newBranchName, 0, 0, projectParentFolder );
                    selectedSceneBranch->add_scene_branch( newTileMap, true, true  );
                }
            }
        }


        if( mouseIsInScene && input->check_mouse_released( mb_right) )
        {
            //Reverts to place mode on right click
            if( shortcutButtonBar->get_tab_id() != SCENE_MODE_SELECT && shortcutButtonBar->get_tab_id() != SCENE_MODE_ERASE )
            {
                //shortcutButtonBar->set_selection( SCENE_MODE_SELECT );
            }
            else if( shortcutButtonBar->get_tab_id() != SCENE_MODE_ERASE )
            {
                //Opens options for scene
                GPE_open_context_menu();
                if( clickedSceneBranch!=NULL )
                {
                    MAIN_CONTEXT_MENU->add_menu_option("Edit Resource["+int_to_string( clickedSceneBranch->get_type() )+"]", 1 );
                }
                MAIN_CONTEXT_MENU->add_menu_option("Create New Scene Branch",5,NULL,5,NULL,true,true);
                MAIN_CONTEXT_MENU->add_menu_option("Help", 6 );
                MAIN_CONTEXT_MENU->add_menu_option("Exit Menu",7,NULL,-1,NULL,true,true);
                int menuSelection = GPE_Get_Context_Result();
                if( menuSelection == 5 )
                {
                    GPE_SceneBasicClass *  newResource = add_new_resource_from_menu();
                }
            }
        }
    }
    else
    {
        if( mouseIsInScene )
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_NO );
            MAIN_OVERLAY->update_tooltip("Select Layer-Mode to edit layer");
        }
        //Edits the settings of the scene
        objectLockStateButton->set_clicked( false );
        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            PANEL_GENERAL_EDITOR->clear_panel();
            renameBox->enable_self();
            confirmResourceButton->enable_self();

            cancelResourceButton->enable_self();

            PANEL_GENERAL_EDITOR->add_gui_element(sceneEditorSubTitle,true);
            PANEL_GENERAL_EDITOR->add_gui_auto(renameBox );
            PANEL_GENERAL_EDITOR->add_gui_auto(sceneCaptionField );
            PANEL_GENERAL_EDITOR->add_gui_auto(sceneHintField );
            PANEL_GENERAL_EDITOR->add_gui_auto(levelPixelWidthField );
            PANEL_GENERAL_EDITOR->add_gui_element(levelPixelHeightField, true );
            PANEL_GENERAL_EDITOR->add_gui_element(defaultTileWidthField, true  );
            PANEL_GENERAL_EDITOR->add_gui_auto(defaultTileHeightField);
            PANEL_GENERAL_EDITOR->add_gui_auto(musicAudioDropDown );
            PANEL_GENERAL_EDITOR->add_gui_auto(startAudioDropDown );
            PANEL_GENERAL_EDITOR->add_gui_auto(endAudioDropDown );
            //PANEL_GENERAL_EDITOR->add_gui_auto(isometricCheckBox );
            PANEL_GENERAL_EDITOR->add_gui_auto(checkBoxIsContinuous );
            PANEL_GENERAL_EDITOR->add_gui_auto(sceneBackgroundColor );
            if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->renderSceneBGColor!=NULL)
            {
                PANEL_GENERAL_EDITOR->add_gui_auto(MAIN_EDITOR_SETTINGS->renderSceneBGColor );
            }
            PANEL_GENERAL_EDITOR->add_gui_auto(confirmResourceButton );
            PANEL_GENERAL_EDITOR->add_gui_auto(cancelResourceButton );
            //PANEL_GENERAL_EDITOR->set_maxed_out_width();
            PANEL_GENERAL_EDITOR->process_self(NULL, NULL);

            manage_components( true );
            if( confirmResourceButton->is_clicked() )
            {
                save_resource();
            }
            else if( cancelResourceButton->is_clicked() )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you will like to revert scene?","This will load in data from save-file and remove all unsaved changes!", true )== DISPLAY_QUERY_YES )
                {
                    resourcePostProcessed = false;
                    preprocess_self();
                }
            }
        }
    }

    if( lightingStateButton!=NULL && lightingStateButton->is_clicked() )
    {
        swap_lighting( );
    }
    //if( PANEL_GENERAL_EDITOR->hasScrollControl==false && PANEL_INSPECTOR->hasScrollControl==false )
    {
        //Horizontal scrolling
        sceneXScroll->set_coords( scenePane.x,scenePane.h - sceneXScroll->get_height() );
        sceneXScroll->set_width( editorView.w );
        update_rectangle(&sceneXScroll->fullRect,0,0,(double)sceneRect.w, (double)sceneRect.h );
        update_rectangle(&sceneXScroll->contextRect,(double)editorCameraRect.x,(double)editorCameraRect.y, (double)editorCameraRect.w/zoomValue, (double)editorCameraRect.h/zoomValue );
        sceneXScroll->process_self(viewedSpace,cam );
        //if( sceneXScroll->has_moved() )
        {
            editorCameraRect.x = (double)(sceneXScroll->contextRect.x);
        }

        //Vertical Scrolling
        sceneYScroll->set_coords( scenePane.x+scenePane.w, scenePane.y );
        sceneYScroll->set_height( scenePane.h);
        update_rectangle(&sceneYScroll->fullRect,0,0,(double)sceneRect.w, (double)sceneRect.h );
        update_rectangle(&sceneYScroll->contextRect,(double)editorCameraRect.x,(double)editorCameraRect.y, (double)editorCameraRect.w/zoomValue, (double)editorCameraRect.h/zoomValue );
        //sceneYScroll->contextRect.h = scenePane.h;
        sceneYScroll->process_self(viewedSpace,cam );
        //if( sceneYScroll->has_moved() )
        {
            editorCameraRect.y = double(sceneYScroll->contextRect.y);
        }
    }

    //if( input->check_mouse_pressed(0) ||  input->check_mouse_pressed(1) || input->check_mouse_pressed(2) )
    {
        if( mouseIsInScene )
        {
            areaIsScrollable = true;
            if( PANEL_GENERAL_EDITOR!=NULL )
            {
                PANEL_GENERAL_EDITOR->hasScrollControl = false;
            }
        }
        else
        {
            areaIsScrollable = false;
        }
    }

    if( input->check_mouse_down( mb_left ) )
    {
        if( mouseIsInScene )
        {
            if( sceneMouseXPos-editorCameraRect.x < 0 )
            {
                editorCameraRect.x = 0;
            }

            if( sceneMouseYPos-editorCameraRect.y < 0 )
            {
                editorCameraRect.y = 0;
            }
        }
    }

    handle_scrolling();

    if( GPE_Main_Statusbar!=NULL)
    {
        GPE_Main_Statusbar->set_codestring( "Mouse( "+int_to_string(sceneMouseXPos )+" , "+int_to_string(sceneMouseYPos)+")"+
                                            "Camera( "+double_to_string(editorCameraRect.x )+" , "+double_to_string(editorCameraRect.y)+") Zoom:"+double_to_string(zoomValue) );
    }
    clickedSceneBranch = NULL;
}

void gameSceneResource::render_grid( int xStart, int yStart, int cellW, int cellH, int xMax, int yMax, GPE_Color * gridLineColor, int gridLineAlpha )
{

    if( cellW < 4 && cellH < 4 || gridRenderRect==NULL || gridLineAlpha <=0 )
    {
        return;
    }

    int i = 0, j = 0;
    if( gridLineColor == NULL )
    {
        gridLineColor = c_ltgray;
    }
    //gridRenderRect->x = ( ( (int)editorCameraRect.x  )/cellW )* cellW;
    gridRenderRect->x = xStart * zoomValue;
    gridRenderRect->x -= editorCameraRect.x* zoomValue;
    i = gridRenderRect->x;

    //gridRenderRect->y = ( ( (int)editorCameraRect.y  )/cellH )* cellH;
    gridRenderRect->y = yStart * zoomValue;
    gridRenderRect->y -= editorCameraRect.y* zoomValue;
    j = gridRenderRect->y;

    double jStart = j;
    gridRenderRect->w = cellW * zoomValue;
    gridRenderRect->h = cellH * zoomValue;

    if( xMax < 0 )
    {
        xMax = sceneRect.w;
    }
    if( yMax < 0 )
    {
        yMax = sceneRect.h;
    }

    while( i <= editorCameraRect.x + (editorCameraRect.w+cellW)/zoomValue  && i < xMax )
    {
        gridRenderRect->y = jStart;
        j = jStart;
        while( j <= editorCameraRect.y +  (editorCameraRect.h+cellH)/zoomValue && j < yMax )
        {
            gcanvas->render_rect( gridRenderRect,  gridLineColor, true, gridLineAlpha );
            j += cellH;
            gridRenderRect->y += cellH * zoomValue;
        }
        i += cellW;
        gridRenderRect->x += cellW * zoomValue;
    }
}

void gameSceneResource::render_scene_layers(GPE_Rect * viewedSpace, GPE_Rect * cam, GPE_Rect * sceneCamera,double renderScale, bool showEditorPreviews,  bool checkListDependent, bool forceRedraw )
{
    if( forceRedraw)
    {
        viewedSpace = GPE_find_camera( viewedSpace );
        cam = GPE_find_camera( cam );
        if( renderScale <=0 )
        {
            renderScale = zoomValue;
        }
        else
        {
            zoomValue = renderScale;
        }
        currentSceneToRender = this;
        /*
        renderZone = GPE_find_camera( renderZone );
        if( renderZone==NULL)
        {
            renderZone = &editorView;
        }

        MAIN_RENDERER->reset_viewpoint();
        MAIN_RENDERER->set_viewpoint( viewedSpace );
        */

        if( sceneCamera==NULL)
        {
            sceneCamera = &editorCameraRect;
        }
        spm->currentCamera->update_box( sceneCamera->x, sceneCamera->y, sceneCamera->w, sceneCamera->h );
        spm->cSceneAnimtList = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ANIMATION]+"s", false );
        spm->cSceneObjList = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s", false );
        spm->cSceneTexList = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE]+"s", false );
        spm->cSceneTstList = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TILESHEET]+"s", false );

        /*if( input->check_keyboard_down(kb_ctrl) )
        {
            if( input->check_keyboard_released(kb_r) )
            {
                GPE_Report("Editor View:("+int_to_string(sceneCamera->x)+","+int_to_string(sceneCamera->y)+","+int_to_string(sceneCamera->x+sceneCamera.w)+","+int_to_string(sceneCamera->y+sceneCamera.h)+")");
                GPE_Report("Tiles View:("+int_to_string(tileXStartPos)+","+int_to_string(tileYStartPos)+","+int_to_string(tileXEndPos)+","+int_to_string(tileYEndPos)+")");
            }
        }
        */

        int tempMax = 0;
        spm->zoomValue = zoomValue;
        //GPE_Report("Attempting to render scene...");
        spm->cameraFloorXPos = floor( sceneCamera->x );
        spm->cameraFloorYPos = floor( sceneCamera->y );
        int layerCount = (int)sceneResourceTree->subElements.size();
        GPE_SpecialMenu_Branch * cBranch = NULL;
        for(  int layerItr = 0; layerItr < layerCount; layerItr++ )
        {
            cBranch = sceneResourceTree->subElements[layerItr];
            if( cBranch!=NULL)
            {
                cBranch->render_branch();
            }
        }
    }
}

void gameSceneResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    double i = 0, j = 0, k = 0;
    int gridX1 = 0;
    int gridX2 = 0;
    int gridY1 = 0;
    int gridY2 = 0;
    //renders the level
    GPE_SceneBackground * currBg = NULL;

    //renders the BGs
    GPE_GeneralResourceContainer * allTexturesFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TEXTURE]+"s");
    GPE_GeneralResourceContainer * allTilesheetsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_TILESHEET]+"s");
    GPE_GeneralResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
    //renders the background

    int bgI = 0;
    if( forceRedraw && allTexturesFolder!=NULL)
    {
        CURRENT_RENDERER->reset_viewpoint(  );
        CURRENT_RENDERER->set_viewpoint( &editorView );

        if( sceneBackgroundColor!=NULL)
        {
            GPE_Color * bgRenderColor = sceneBackgroundColor->get_color();
            if( bgRenderColor!=NULL)
            {
                gcanvas->render_rectangle( 0,0,editorView.w,editorView.h,bgRenderColor,false);
            }
        }

        //Renders the transparent background texture below the layers
        if(  editorMode==0 && GPE_TEXTURE_TRANSPARENT_BG!=NULL)
        {
            if( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->renderSceneBGColor!=NULL && MAIN_EDITOR_SETTINGS->renderSceneBGColor->is_clicked()==false )
            {
                for(int iPV= 0; iPV<= editorView.w; iPV+=GPE_TEXTURE_TRANSPARENT_BG->get_width() )
                {
                    for(int jPV= 0; jPV<= editorView.h; jPV+=GPE_TEXTURE_TRANSPARENT_BG->get_height() )
                    {
                        //GPE_TEXTURE_TRANSPARENT_BG->render_tex( iPV,jPV,NULL);
                    }
                }
            }
        }

        //Renders the scene layers( tiles, backgrounds, objects, etc)
        render_scene_layers( viewedSpace, cam, &editorCameraRect,zoomValue,true, true, forceRedraw );

        int mousePreviewXPos = floor( (sceneMouseXPos*zoomValue-editorCameraRect.x*zoomValue) );
        int mousePreviewYPos = floor( (sceneMouseYPos*zoomValue-editorCameraRect.y*zoomValue) );

        if( useLighting && forceRedraw )
        {
            gcanvas->resize_ligting_overlay( editorView.w, editorView.h );
            gcanvas->switch_ligting_overlay( true );
            CURRENT_RENDERER->reset_viewpoint( );
            gcanvas->render_rectangle(0,0,editorView.w, editorView.h, c_black, false, 255 );
            spm->lightCircleTexture->render_tex_resized( editorView.w/2-128, editorView.h/2-128, 256,256, NULL, c_white, 128 );

            gcanvas->render_rectangle(mousePreviewXPos-64,mousePreviewYPos-64,mousePreviewXPos+64,mousePreviewYPos+64, c_orangered, false, 192 );
            //gcanvas->render_circle_color( editorView.w/2, editorView.h/2, 256, 128, 128, 255, 255 );
            gcanvas->switch_ligting_overlay( false );
            CURRENT_RENDERER->reset_viewpoint( );
            CURRENT_RENDERER->set_viewpoint( &editorView );

            gcanvas->render_ligting_overlay( 0,0 );
        }
        //MAIN_RENDERER->reset_viewpoint();
        //MAIN_RENDERER->set_viewpoint( &editorView );

        //Renders the selected scene object above the scene at mouse coordinates
        //###SELECTED_SCENE_RENDER ###

        int draggedResourceX = floor( (sceneMouseXPos*zoomValue-editorCameraRect.x*zoomValue) );
        int draggedResourceY = floor( (sceneMouseYPos*zoomValue-editorCameraRect.y*zoomValue) );

        if( selectedSceneBranch!=NULL && editorMode == LEDITOR_MODE_LAYERS )
        {
            bool isLayerOrGroup = false;
            branchTempRect->x = zoomValue*(selectedSceneBranch->xPos) - zoomValue*editorCameraRect.x;
            branchTempRect->y = zoomValue*(selectedSceneBranch->yPos) - zoomValue*editorCameraRect.y;
            branchTempRect->w = abs( zoomValue*(selectedSceneBranch->width * selectedSceneBranch->xScale ) );
            branchTempRect->h = abs( zoomValue*(selectedSceneBranch->height * selectedSceneBranch->yScale) );

            if( selectedSceneBranch->get_type() == BRANCH_TYPE_TILEMAP  )
            {
                GPE_SceneTileMap * currentTileMap = ( GPE_SceneTileMap * )selectedSceneBranch;
                if( viewTileGridCheckBox->is_clicked()  )
                {
                    render_grid( currentTileMap->xPos, currentTileMap->yPos, currentTileMap->tileWidth, currentTileMap->tileHeight, currentTileMap->get_xmax(), currentTileMap->get_ymax() );
                }
                //###Previews tilesheet in scene
            }
            else
            {
                if( useSceneGrid && sceneGridX>=8 && sceneGridY>=8 )
                {
                    render_grid( 0, 0, sceneGridX, sceneGridY,-1,-1, gridColorField->get_color(), gridAlphaField->get_held_int() );
                }
                if( selectedSceneBranch->get_type() == BRANCH_TYPE_LAYER || selectedSceneBranch->get_type() == BRANCH_TYPE_GROUP )
                {
                    isLayerOrGroup = true;

                    int objRenderXPos = draggedResourceX = floor( (sceneObjMouseX*zoomValue-editorCameraRect.x*zoomValue) );
                    int objRenderYPos = draggedResourceY = floor( (sceneObjMouseY*zoomValue-editorCameraRect.y*zoomValue) );

                    if( shortcutButtonBar->get_tab_id() == SCENE_MODE_PLACE && mouseIsInScene && RESOURCE_TO_DRAG==NULL)
                    {
                        GPE_GeneralResourceContainer * hObj = objectInEditor->get_selected_container();
                        if( hObj!=NULL && hObj->get_held_resource() )
                        {
                            generalGameResource * actualHGameObject= (gameObjectResource*) hObj->get_held_resource();

                            if( actualHGameObject!=NULL)
                            {
                                /*
                                double renderWSize = 32*zoomValue;
                                double renderHSize = 32*zoomValue;
                                if( actualHGameObject->spriteField!=NULL && actualHGameObject->spriteField->get_selected_container()!=NULL )
                                {
                                    GPE_GeneralResourceContainer * tempSpriteContainer = actualHGameObject->spriteField->get_selected_container();
                                    if( tempSpriteContainer!=NULL && tempSpriteContainer->get_held_resource() )
                                    {
                                        animationResource * tempSpriteObj = (animationResource * )tempSpriteContainer->get_held_resource();
                                        if( tempSpriteObj!=NULL )
                                        {
                                            if( tempSpriteObj->animInEditor!=NULL )
                                            {
                                                renderWSize = ceil(tempSpriteObj->animInEditor->get_width()*zoomValue);
                                                renderHSize = ceil(tempSpriteObj->animInEditor->get_height()*zoomValue);
                                            }
                                        }
                                    }
                                }
                                */
                                if( hObj->get_resource_type() == RESOURCE_TYPE_OBJECT || hObj->get_resource_type() == RESOURCE_TYPE_ANIMATION )
                                {
                                    hObj->render_image_scaled( objRenderXPos, objRenderYPos, zoomValue, zoomValue, NULL, NULL, c_gray );
                                }
                                else if( sceneGridX>=32 && sceneGridY>=32  )
                                {
                                    hObj->render_image( objRenderXPos, objRenderYPos, sceneGridX*zoomValue, sceneGridY*zoomValue, NULL, NULL, c_gray );
                                }
                                else
                                {
                                    hObj->render_image( objRenderXPos, objRenderYPos,32*zoomValue, 32*zoomValue, NULL, NULL, c_gray );
                                }
                            }
                        }
                    }
                }
                else if( selectedSceneBranch->get_type() == BRANCH_TYPE_OBJECT )
                {
                    if( shortcutButtonBar->get_tab_id()!= SCENE_MODE_ERASE && shortcutButtonBar->get_tab_id()!= SCENE_MODE_PLACE )
                    {
                        GPE_SceneGameObject * currentObject = ( GPE_SceneGameObject * )selectedSceneBranch;
                    }
                }
            }

            //Temporarily disabled until I fix this calculation
            if( shortcutButtonBar->get_tab_id()== SCENE_MODE_ROTATION && !isLayerOrGroup  )
            {
                if( input->check_mouse_down( mb_left ) && mouseIsInScene )
                {
                    int objPivotX = floor( (selectedSceneBranch->xPos*zoomValue-editorCameraRect.x*zoomValue) );
                    int objPivotY = floor( (selectedSceneBranch->yPos*zoomValue-editorCameraRect.y*zoomValue) );
                    gcanvas->render_line_color( objPivotX, objPivotY,draggedResourceX,draggedResourceY, c_red  );
                    gcanvas->render_circle_color( draggedResourceX, draggedResourceY,4, c_red,255 );
                    gcanvas->render_circle_color( objPivotX, objPivotY,4, c_red,255 );
                }
            }

            if( selectedSceneBranch!=NULL && !isLayerOrGroup )
            {
                if( spm->highlightRect!=NULL )
                {
                    spm->highlightRect->render_tex_special(branchTempRect->x-2,branchTempRect->y-2,selectedSceneBranch->angle,branchTempRect->w+2,branchTempRect->h+2, c_white, NULL,192 );
                    spm->highlightRect->render_tex_special(branchTempRect->x,branchTempRect->y,selectedSceneBranch->angle,branchTempRect->w,branchTempRect->h, c_blue, NULL, 192 );
                }
                gfs->render_text_boxed(branchTempRect->x, branchTempRect->y,selectedSceneBranch->get_name(), c_white, c_black, FONT_LABEL, FA_LEFT, FA_BOTTOM );
            }

            if( mouseIsInScene && shortcutButtonBar->get_tab_id() == SCENE_MODE_ERASE && GPE_MAIN_GUI!=NULL && GPE_MAIN_GUI->eraserAnimation!=NULL )
            {
                GPE_MAIN_GUI->eraserAnimation->render_scaled( 0, draggedResourceX, draggedResourceY, zoomValue, zoomValue );
            }
            else if( RESOURCE_TO_DRAG!=NULL && mouseIsInScene )
            {
                if( RESOURCE_TO_DRAG->get_resource_type() == RESOURCE_TYPE_OBJECT || RESOURCE_TO_DRAG->get_resource_type() == RESOURCE_TYPE_ANIMATION )
                {
                    RESOURCE_TO_DRAG->render_image_scaled( draggedResourceX, draggedResourceY, zoomValue, zoomValue, NULL, NULL, c_gray );
                }
                else if( sceneGridX>=8 && sceneGridY>=8  )
                {
                    RESOURCE_TO_DRAG->render_image( draggedResourceX, draggedResourceY, sceneGridX*zoomValue, sceneGridY*zoomValue, NULL, NULL, c_gray );
                }
                else
                {
                    RESOURCE_TO_DRAG->render_image( draggedResourceX, draggedResourceY,32*zoomValue, 32*zoomValue, NULL, NULL, c_gray );
                }
            }
        }
        else if( useSceneGrid && sceneGridX>=8 && sceneGridY>=8 )
        {
            render_grid( 0, 0, sceneGridX, sceneGridY, -1,-1, gridColorField->get_color(), gridAlphaField->get_held_int() );
        }
        //Clears the editor pane then renders panel and scrollbars

        //"Editor View:("+int_to_string(editorCameraRect.x)+","+int_to_string(editorCameraRect.y)+","+int_to_string(editorCameraRect.w)+","+int_to_string(editorCameraRect.h)+")",
        //gcanvas->render_rectangle( sceneYScroll->elementBox.x+sceneYScroll->elementBox.w,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_THEME->Program_Color,false);
        //gcanvas->render_rectangle( sceneXScroll->elementBox.x,sceneXScroll->elementBox.y,viewedSpace->w,viewedSpace->h,GPE_MAIN_THEME->Program_Color,false);
        CURRENT_RENDERER->reset_viewpoint(  );
        CURRENT_RENDERER->set_viewpoint( viewedSpace );
    }

    if( forceRedraw )
    {
        if( sceneXScroll!=NULL)
        {
            sceneXScroll->render_self( viewedSpace,cam);
        }
        if( sceneYScroll!=NULL)
        {
            sceneYScroll->render_self( viewedSpace,cam);
        }

        if( sceneTopBarList!=NULL)
        {
            sceneTopBarList->render_self( viewedSpace,cam);
        }
    }
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
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Game Scene", resourceName );
    }
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
    if( !newSceneFile.fail() )
    {
        //makes sure the file is open
        if (newSceneFile.is_open())
        {
            GPE_SceneGameObject * tSceneObj = NULL;
            GPE_SceneBackground * tSceneBg = NULL;
            GPE_GeneralResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
            GPE_GeneralResourceContainer * fObj = NULL;
            write_header_on_file(&newSceneFile);
            if( sceneTitleField!=NULL)
            {
                if( (int)sceneTitleField->get_string().size() > 0)
                {
                    newSceneFile << "Title=" << sceneTitleField->get_string() << "\n";
                }
            }
            if( sceneCaptionField!=NULL)
            {
                if( (int)sceneCaptionField->get_string().size() > 0)
                {
                    newSceneFile << "Caption=" << sceneCaptionField->get_string() << "\n";
                }
            }
            if( sceneHintField!=NULL)
            {
                if( (int)sceneHintField->get_string().size() > 0)
                {
                    newSceneFile << "SceneHint=" << sceneHintField->get_string() << "\n";
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

            if( defaultTileWidthField!=NULL)
            {
                defaultTileWidthField->make_valid_number(32);
                newSceneFile << "DefaultTileWidth=" << defaultTileWidthField->get_held_number() << "\n";
            }

            if( defaultTileHeightField!=NULL)
            {
                defaultTileHeightField->make_valid_number(32);
                newSceneFile << "DefaultTileHeight=" << defaultTileHeightField->get_held_number() << "\n";
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

            if( gridWidthField!=NULL)
            {
                gridWidthField->make_valid_number(32);
                newSceneFile << "GridWidth=" << gridWidthField->get_string() << "\n";
            }

            if( gridHeightField!=NULL)
            {
                gridHeightField->make_valid_number(32);
                newSceneFile << "GridHeight=" << gridHeightField->get_string() << "\n";
            }
            if( gridColorField!=NULL)
            {
                newSceneFile << "GridColor=" << gridColorField->get_hex_string() << "\n";
            }
            if( gridAlphaField!=NULL)
            {
                gridAlphaField->make_valid_number(22);
                newSceneFile << "GridAlpha=" << gridAlphaField->get_string() << "\n";
            }

            if( useObjGridCheckBox!=NULL)
            {
                newSceneFile << "DisplayObjectGrid=" << useObjGridCheckBox->is_clicked() << "\n";
            }

            //Saves the scene's components
            if( sceneComponents !=NULL )
            {
                sceneComponents->save_branch_data( &newSceneFile, 0 );
            }

            //Saves the individual scene branches after the scene's components
            if( sceneResourceTree !=NULL )
            {
                sceneResourceTree->save_branch_data( &newSceneFile, 0 );
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

bool gameSceneResource::seek_placeable_branch()
{
    //We will add this branch to the last layer
    if( selectedSceneBranch == NULL )
    {
        if( sceneResourceTree == NULL)
        {
            return false;
        }
        //Look for last layer IN TREE
        sceneResourceTree->select_branch( sceneResourceTree->get_last_branch() );
        if( sceneResourceTree->selectedBranch!=NULL )
        {
            selectedSceneBranch = ( GPE_SceneBasicClass* )sceneResourceTree->selectedBranch;
        }
        else
        {
            selectedSceneBranch = add_layer( 0 , true );
        }
        return true;
    }
    return false;
}
void gameSceneResource::select_object( GPE_SceneGameObject * objToSelect )
{
    unselect_object();
    if( objToSelect!=NULL)
    {
        sceneResourceTree->select_branch( objToSelect);
        selectedSceneBranch = ( GPE_SceneBasicClass * ) objToSelect;
        selectedSceneObject = objToSelect;
        if( PANEL_INSPECTOR!=NULL)
        {
            PANEL_INSPECTOR->clear_panel();
        }

        GPE_GeneralResourceContainer * foundSelectedObjContainer = projectParentFolder->find_resource_from_id(selectedSceneObject->objTypeId);
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
    }
}

void gameSceneResource::swap_lighting( )
{
    useLighting = !useLighting;
    if( lightingStateButton!=NULL )
    {
        if( useLighting )
        {
            lightingStateButton->descriptionText = "Turn Off Lighting(L)";
            lightingStateButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/sun-o.png" );
        }
        else
        {
            lightingStateButton->descriptionText = "Turn On Lighting(L)";
            lightingStateButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/moon-o.png" );
        }
    }
}
void gameSceneResource::swap_lock( bool lockState )
{
    if( objectLockStateButton!=NULL )
    {
        if( lockState )
        {
            objectLockStateButton->descriptionText = "Unlock Branch(J)";
            objectLockStateButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/unlock.png" );
        }
        else
        {
            objectLockStateButton->descriptionText = "Lock Branch(J)";
            objectLockStateButton->set_image( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/lock.png" );
        }
    }
}

bool gameSceneResource::unselect_object( bool selectParent )
{
    selectedSceneObject = NULL;
    if( selectedSceneBranch!=NULL )
    {
        selectedSceneBranch->isBeingMoved = false;
        draggingOffsetX = draggingOffsetY = 0;
        if( sceneResourceTree!=NULL && selectParent )
        {
            sceneResourceTree->select_branch( selectedSceneBranch->parentBranch );
            if( sceneResourceTree->selectedBranch!=NULL )
            {
                selectedSceneBranch = (GPE_SceneBasicClass*)sceneResourceTree->selectedBranch;
            }
            else
            {
                selectedSceneBranch = NULL;
            }
        }
        else
        {
            if( sceneResourceTree!=NULL )
            {
                sceneResourceTree->select_branch( NULL );
            }
            selectedSceneBranch = NULL;
        }
        return true;
    }
    else
    {

    }
    if( PANEL_INSPECTOR!=NULL)
    {
        PANEL_INSPECTOR->clear_panel();
    }
    if( objectEditorNameTitle!=NULL)
    {
        objectEditorNameTitle ->set_name( "None");
    }
    return false;
}

void gameSceneResource::update_box(int newX, int newY, int newW, int newH)
{

}

void gameSceneResource::update_project_layers()
{
    GPE_SpecialMenu_Branch * cBranch;

    sceneLayer * tLayer = NULL;
    for( int i = (int)sceneResourceTree->subElements.size()-1; i>=0; i-- )
    {
        cBranch = sceneResourceTree->subElements[i];
        if( cBranch!=NULL && cBranch->get_type() == BRANCH_TYPE_LAYER )
        {
            tLayer  = (sceneLayer *)cBranch;
            if( tLayer->projectLayerId >=0 && tLayer->projectLayerId < 32)
            {
                tLayer->set_name( CURRENT_PROJECT->projectLayerNames[ (int)(tLayer->projectLayerId) ] );
            }
            else
            {
                //Out of bounds
            }
        }
        else
        {
            //Erase?
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
