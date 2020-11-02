/*
game_scene_resource.h
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

#ifndef GAME_SCENE_RESOURCE_H
#define GAME_SCENE_RESOURCE_H

#include "../gpe/gpe_layer.h"
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

extern std::string DEFAULT_SCENE_SUBEDITOR_NAMES[5];
//This class will essentially manage all of the pop ups that can be created with the scene editor
//Made a global to avoid duplicating data

const float GPE_SCENE_ALIGN_VERSION = 1.30;

class gameSceneResource: public standardEditableGameResource
{
protected:
    float objselectedX1;
    float objselectedY1;
    float objselectedX2;
    float objselectedY2;
    bool mouseIsInScene;
    std::vector< GPE_SceneBasicClass * > selectedBranchList;
    gpe::shape_rect * gridRenderRect;
    gpe::shape_rect scenePane;
    gpe::shape_rect editorView;
    gpe::shape_rect editorCameraRect;
    pawgui::widget_panel_list * sceneTopBarList;

    pawgui::widget_button_iconbar * editorButtonBar;
    pawgui::widget_button_iconbar * shortcutButtonBar;
    pawgui::widget_button_icon * objectLockStateButton;
    pawgui::widget_button_icon * lightingStateButton;
    pawgui::widget_label_title * sceneEditorSubTitle;
    pawgui::widget_label_title * customComponentsTitle;

    //used for the settings/options tab
    pawgui::widget_input_text * sceneTitleField;
    pawgui::widget_input_text * sceneCaptionField;
    pawgui::widget_input_text * sceneHintField;

    pawgui::widget_drop_down_resource_menu * musicAudioDropDown;
    pawgui::widget_drop_down_resource_menu * startAudioDropDown;
    pawgui::widget_drop_down_resource_menu * endAudioDropDown;

    //
    pawgui::widget_input_number * levelPixelWidthField;
    pawgui::widget_input_number * levelPixelHeightField;
    pawgui::widget_input_number * defaultTileWidthField;
    pawgui::widget_input_number * defaultTileHeightField;

    pawgui::widget_checkbox * preloadCheckBox;
    pawgui::widget_checkbox * isometricCheckBox;
    pawgui::widget_checkbox * checkBoxIsContinuous;
    pawgui::gpe_widget_color_picker * sceneBackgroundColor;

    pawgui::widget_button_label * addGroupButton;
    pawgui::widget_button_label * addTileMapButton;
    pawgui::widget_button_label * changeGroupNameButton;

    //Used for the Layers Tab
    pawgui::widget_button_label * changeLayerNameButton;
    pawgui::widget_button_label * renameLayerButton;
    pawgui::widget_checkbox * viewTileGridCheckBox;
    bool showTileLines;

    //used for the background tab
    int bgIdInEdit;
    animationResource * animRes;
    textureResource * texRes;
    float zoomValue;

    //used for the object placement tab
    pawgui::widget_button_label * inheritParentComponentButton;
    pawgui::widget_button_label * resetComponentsButton;
    pawgui::widget_dropdown_menu * addNewComponentDropDown;

    //Object related gui elements.
    pawgui::widget_drop_down_resource_menu * objectInEditor;

    //Animation Related gui elements.
    pawgui::widget_drop_down_resource_menu * animationInEditor;

    pawgui::widget_drop_down_resource_menu * objectNewType;
    pawgui::widget_drop_down_resource_menu * objectReplacerInEditor;
    GPE_SceneGameObject * selectedSceneObject;
    int customObjCreationId;
    pawgui::widget_button_label * closeObjectProperties;

    //Grid Related Varables [ Begin ]
    pawgui::widget_input_number * gridWidthField;
    pawgui::widget_input_number * gridHeightField;
    pawgui::widget_button_label * forceSnapButton;
    pawgui::gpe_widget_color_picker * gridColorField;
    pawgui::widget_input_number * gridAlphaField;
    int sceneGridX, sceneGridY;
    pawgui::widget_checkbox * useObjGridCheckBox;
    bool useSceneGrid;
    //Grid Related Variables [ End ]

    pawgui::widget_checkbox * onlyRemoveThisObjectTypeButton;
    pawgui::widget_button_label * removeObjectButton;
    pawgui::widget_button_label * clearObjectsButton;

    pawgui::widget_label_text  * objectEditorNameTitle;
    pawgui::widget_label_text  * objectEditorPropertiesTitle;


    pawgui::widget_button_icon * objCustomVariableSettingsButtton;
    pawgui::widget_button_icon * objCustomVariableAddButtton;
    pawgui::widget_button_icon * objCustomVariableRemoveButton;
    pawgui::widget_button_icon * objCustomVariableRefeshButton;

    //used for the tile placement tab
    pawgui::widget_button_label * resizeMapButton;

    int layerInEdit;
    float sceneMouseXPos, sceneMouseYPos;
    float sceneObjMouseX, sceneObjMouseY;
    int lastCreatedObjXPos, lastCreatedObjYPos;
    int lastCreatedObjTypeId;
    gpe::shape_rect tsCameraRect;
    int tilesheetRenderXPos,tilesheetRenderYPos;
    sceneLayer * defaultObjectLayer;
    sceneLayer * defaultBackgroundLayer;
    pawgui::widget_button_icon * gridToggleButtton;
    pawgui::widget_button_icon * rotationButton;
    pawgui::widget_button_icon * layerRemoveButton;
    pawgui::widget_button_icon * layerMoveUpButtton;
    pawgui::widget_button_icon * layerMoveDownButton;
    pawgui::widget_button_icon * layerToggleHideButton;

    pawgui::widget_label_text  * selectedBranchLabel;
    pawgui::widget_label_text  * layerErrorMessage;
    bool areaIsScrollable;
    bool isDraggingObject;
    gpe::shape_rect * branchTempRect;
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
    gpe::shape_rect sceneRect;
    int selectedBranchId;
    GPE_SceneBasicClass * sceneComponents;
    GPE_SceneBasicClass * selectedSceneBranch;
    pawgui::widget_tree * sceneResourceTree;
    bool scnPostProcessed;
    pawgui::widget_scrollbar_xaxis * sceneXScroll;
    pawgui::widget_scrollbar_yaxis * sceneYScroll;

    pawgui::widget_button_icon * saveButton;
    //Used to calculate the derotated/scaled box of an resource
    float rmx;
    float rmy;
    float lmx;
    float lmy;
    gameSceneResource(pawgui::widget_resource_container * pFolder = NULL);
    ~gameSceneResource();
    sceneLayer * add_retro_layer(int layerType, int newLayerProjectLayerId, bool selectedLayer = false );
    sceneLayer * add_layer( int newLayerProjectLayerId, bool selectedLayer = false );
    GPE_SceneBasicClass *  add_new_resource_from_menu();
    GPE_SceneTileMap *  add_tilemap( GPE_SceneBasicClass * branchHolder, std::string name = "", int x = 0, int y = 0, int w = -1, int h = -1, int tW = 32, int tH = 32);
    void adjust_object_offsets( pawgui::widget_branch *  basicObj );
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    sceneLayer * find_layer(int layeToFind);
    bool find_scene_branch( GPE_SceneBasicClass * branchHolder, bool nestDown = false );
    bool find_scene_branches( GPE_SceneBasicClass * branchHolder, bool nestDown  = false );

    bool get_mouse_coords( );
    void handle_scrolling();
    bool include_local_files( std::string pBuildDir , int buildType );
    void inherit_components( GPE_SceneBasicClass * objectToInherit, standardEditableGameResource * objParent);
    bool is_build_ready();
    void manage_components( bool processComponentsToo = true );

    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_components();
    void process_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void render_grid( int xStart, int yStart, int cellW, int cellH, int xMax = -1, int yMax = -1, gpe::color * gridLineColor = NULL, int gridLineAlpha = 128 );
    void render_scene_layers( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL, gpe::shape_rect * sceneCamera = NULL, float renderScale = -1,bool showEditorPreviews = true, bool checkListDependent = true);
    void render_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void reset_placement_info();
    void save_resource(std::string file_path = "", int backupId = -1);
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

extern gameSceneResource * scene_currentToRender;
#endif
