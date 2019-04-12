/*
game_scene_resource.h
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

#ifndef GAME_SCENE_RESOURCE_H
#define GAME_SCENE_RESOURCE_H

#include "GPE_Engine/GPE_Layers.h"
#include "animation_resource.h"
#include "font_resource.h"
#include "game_object_resource.h"
#include "project_properties.h"
#include "texture_resource.h"
#include "tilesheet_resource.h"

#include "gpe_scene_animation_class.h"
#include "gpe_scene_background_class.h"
#include "gpe_scene_basic_class.h"
#include "gpe_scene_background_class.h"
#include "gpe_scene_group_class.h"
#include "gpe_scene_helper_class.h"
#include "gpe_scene_layer_class.h"
#include "gpe_scene_light_class.h"
#include "gpe_scene_object_class.h"
#include "gpe_scene_particle_class.h"
#include "gpe_scene_text_class.h"
#include "gpe_scene_tilemap_class.h"

const int LEDITOR_MODE_SETTINGS = 0;
const int LEDITOR_MODE_LAYERS = 1;

const int LAYER_TYPE_TILES = 0;
const int LAYER_TYPE_BACKGROUND = 1;
const int LAYER_TYPE_OBJECTS = 2;
const int LAYER_TYPE_RESOURCES = 3;
const int LAYER_TYPE_MAX = 2;

const int LEDITOR_MODE_META = 4;

const int MAX_BACKGROUND_NUMBER = 7;

const int BRANCH_TYPE_MAX_TYPE = 12;

extern std::string DEFAULT_SCENE_SUBEDITOR_NAMES[5];
//This class will essentially manage all of the pop ups that can be created with the scene editor
//Made a global to avoid duplicating data

const double GPE_SCENE_ALIGN_VERSION = 1.30;

class gameSceneResource: public standardEditableGameResource
{
protected:
    double objSelectedX1;
    double objSelectedY1;
    double objSelectedX2;
    double objSelectedY2;
    bool mouseIsInScene;
    std::vector< GPE_SceneBasicClass * > selectedBranchList;
    GPE_Rect * gridRenderRect;
    GPE_Rect scenePane;
    GPE_Rect editorView;
    GPE_Rect editorCameraRect;
    GPE_GuiElementList * sceneTopBarList;

    GPE_ToolIconButtonBar * editorButtonBar;
    GPE_ToolIconButtonBar * shortcutButtonBar;
    GPE_ToolIconButton * objectLockStateButton;
    GPE_ToolIconButton * lightingStateButton;
    GPE_Label_Title * sceneEditorSubTitle;
    GPE_Label_Title * customComponentsTitle;

    //used for the settings/options tab
    GPE_TextInputBasic * sceneTitleField;
    GPE_TextInputBasic * sceneCaptionField;
    GPE_TextInputBasic * sceneHintField;

    GPE_DropDown_Resouce_Menu * musicAudioDropDown;
    GPE_DropDown_Resouce_Menu * startAudioDropDown;
    GPE_DropDown_Resouce_Menu * endAudioDropDown;

    //
    GPE_DropDown_Resouce_Menu * northSceneDropDown;
    GPE_DropDown_Resouce_Menu * southSceneDropDown;
    GPE_DropDown_Resouce_Menu * eastSceneDropDown;
    GPE_DropDown_Resouce_Menu * westSceneDropDown;
    //
    GPE_TextInputNumber * levelPixelWidthField;
    GPE_TextInputNumber * levelPixelHeightField;
    GPE_TextInputNumber * defaultTileWidthField;
    GPE_TextInputNumber * defaultTileHeightField;

    GPE_CheckBoxBasic * preloadCheckBox;
    GPE_CheckBoxBasic * isometricCheckBox;
    GPE_CheckBoxBasic * checkBoxIsContinuous;
    GPE_Input_Field_Color * sceneBackgroundColor;

    GPE_ToolLabelButton * addGroupButton;
    GPE_ToolLabelButton * addTileMapButton;
    GPE_ToolLabelButton * changeGroupNameButton;

    //Used for the Layers Tab
    GPE_ToolLabelButton * changeLayerNameButton;
    GPE_ToolLabelButton * renameLayerButton;
    GPE_CheckBoxBasic * viewTileGridCheckBox;
    bool showTileLines;

    //used for the background tab
    int bgIdInEdit;
    animationResource * animRes;
    textureResource * texRes;
    double zoomValue;

    //used for the object placement tab
    GPE_ToolLabelButton * inheritParentComponentButton;
    GPE_ToolLabelButton * resetComponentsButton;
    GPE_DropDown_Menu * addNewComponentDropDown;

    //Object related gui elements.
    GPE_DropDown_Resouce_Menu * objectInEditor;

    //Animation Related gui elements.
    GPE_DropDown_Resouce_Menu * animationInEditor;


    GPE_DropDown_Resouce_Menu * objectNewType;
    GPE_DropDown_Resouce_Menu * objectReplacerInEditor;
    GPE_SceneGameObject * selectedSceneObject;
    int customObjCreationId;
    GPE_ToolLabelButton * closeObjectProperties;

    //Grid Related Varables [ Begin ]
    GPE_TextInputNumber * gridWidthField;
    GPE_TextInputNumber * gridHeightField;
    GPE_ToolLabelButton * forceSnapButton;
    GPE_Input_Field_Color * gridColorField;
    GPE_TextInputNumber * gridAlphaField;
    int sceneGridX, sceneGridY;
    GPE_CheckBoxBasic * useObjGridCheckBox;
    bool useSceneGrid;
    //Grid Related Variables [ End ]

    GPE_CheckBoxBasic * onlyRemoveThisObjectTypeButton;
    GPE_ToolLabelButton * removeObjectButton;
    GPE_ToolLabelButton * clearObjectsButton;

    GPE_Label_Text * objectEditorNameTitle;
    GPE_Label_Text * objectEditorPropertiesTitle;


    GPE_ToolIconButton * objCustomVariableSettingsButtton;
    GPE_ToolIconButton * objCustomVariableAddButtton;
    GPE_ToolIconButton * objCustomVariableRemoveButton;
    GPE_ToolIconButton * objCustomVariableRefeshButton;

    //used for the tile placement tab
    GPE_ToolLabelButton * resizeMapButton;



    int layerInEdit;
    double sceneMouseXPos, sceneMouseYPos;
    double sceneObjMouseX, sceneObjMouseY;
    int lastCreatedObjXPos, lastCreatedObjYPos;
    int lastCreatedObjTypeId;
    GPE_Rect tsCameraRect;
    int tilesheetRenderXPos,tilesheetRenderYPos;
    sceneLayer * defaultObjectLayer;
    sceneLayer * defaultBackgroundLayer;
    GPE_ToolIconButton * gridToggleButtton;
    GPE_ToolIconButton * rotationButton;
    GPE_ToolIconButton * layerRemoveButton;
    GPE_ToolIconButton * layerMoveUpButtton;
    GPE_ToolIconButton * layerMoveDownButton;
    GPE_ToolIconButton * layerToggleHideButton;

    GPE_Label_Text * selectedBranchLabel;
    GPE_Label_Text * layerErrorMessage;
    bool areaIsScrollable;
    bool isDraggingObject;
    GPE_Rect * branchTempRect;
    GPE_SceneBasicClass * clickedSceneBranch;
    std::vector< GPE_SceneBasicClass * >clickedSceneBranches;
    int draggingOffsetX, draggingOffsetY;
    int defaultTileWidth,defaultTileHeight;
    int defaultTileAmountX,defaultTileAmountY;
public:
    bool sceneInFocus;
    bool useLighting;
    bool clickMultipleBranches;
    int topResourceTreeHeight;
    GPE_Rect sceneRect;
    int selectedBranchId;
    GPE_SceneBasicClass * sceneComponents;
    GPE_SceneBasicClass * selectedSceneBranch;
    GPE_SpecialMenu_Tree * sceneResourceTree;
    bool scnPostProcessed;
    GPE_ScrollBar_XAxis * sceneXScroll;
    GPE_ScrollBar_YAxis * sceneYScroll;

    GPE_ToolIconButton * saveButton;
    //Used to calculate the derotated/scaled box of an resource
    double rmx;
    double rmy;
    double lmx;
    double lmy;
    gameSceneResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~gameSceneResource();
    sceneLayer * add_retro_layer(int layerType, int newLayerProjectLayerId, bool selectedLayer = false );
    sceneLayer * add_layer( int newLayerProjectLayerId, bool selectedLayer = false );
    GPE_SceneBasicClass *  add_new_resource_from_menu();
    GPE_SceneTileMap *  add_tilemap( GPE_SceneBasicClass * branchHolder, std::string name = "", int x = 0, int y = 0, int w = -1, int h = -1, int tW = 32, int tH = 32);
    void adjust_object_offsets( GPE_SpecialMenu_Branch *  basicObj );
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    sceneLayer * find_layer(int layeToFind);
    bool find_scene_branch( GPE_SceneBasicClass * branchHolder, bool nestDown = false );
    bool find_scene_branches( GPE_SceneBasicClass * branchHolder, bool nestDown  = false );

    bool get_mouse_coords( );
    void handle_scrolling();
    void inherit_components( GPE_SceneBasicClass * objectToInherit, standardEditableGameResource * objParent);
    void manage_components( bool processComponentsToo = true );

    void prerender_self( );
    void preprocess_self(std::string alternatePath = "");
    void process_components();
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_grid( int xStart, int yStart, int cellW, int cellH, int xMax = -1, int yMax = -1, GPE_Color * gridLineColor = NULL, int gridLineAlpha = 128 );
    void render_scene_layers(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, GPE_Rect * sceneCamera = NULL, double renderScale = -1,bool showEditorPreviews = true, bool checkListDependent = true,  bool forceRedraw = true);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void reset_placement_info();
    void save_resource(std::string alternatePath = "", int backupId = -1);
    bool seek_placeable_branch();
    void select_object(GPE_SceneGameObject * objToSelect);
    void swap_lighting( );
    void swap_lock( bool lockState );
    bool unselect_object( bool selectParent = false );
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    void update_project_layers();
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gpeEditorDockPanel * PANEL_GRID_INFO;

extern gameSceneResource * currentSceneToRender;
#endif
