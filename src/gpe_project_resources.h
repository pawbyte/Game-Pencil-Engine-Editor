/*
ambitious_gui_library.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef GPE_PROJECT_RESOURCES_H
#define GPE_PROJECT_RESOURCES_H

#include "gpe_editor.h"

class standardEditableGameResource: public generalGameResource
{
    protected:
        GPE_ToolIconButton * loadResourceButton;
        GPE_ToolIconButton * exportResourceButton;
        GPE_ToolIconButton * saveResourceButton;
        GPE_ToolPushButton * confirmResourceButton;
        GPE_ToolPushButton * cancelResourceButton;
        GPE_TextInputBasic * renameBox;
        GPE_GuiElementList * editorPaneList;
    public:
        bool resourcePostProcessed;
        int commonButtonAlignment;
        GPE_ResourceContainer * projectParentFolder;
        standardEditableGameResource(GPE_ResourceContainer * ppFolder = NULL);
        virtual ~standardEditableGameResource() = 0;
        std::string get_current_name();
        GPE_GeneralGuiElement * add_gui_component(std::string componentData);
        void process_export();
        virtual void integrate_into_syntax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        virtual void preprocess_self(std::string alternatePath = "" );
        virtual void prerender_self( );
        virtual void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        virtual void process_resource(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_resource(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        virtual void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void seek_parent_name();
        void set_name(std::string newName);
        void set_parent_name(std::string newName);
        virtual int search_for_string(std::string needle);
        virtual int search_and_replace_string(std::string needle, std::string newStr = "");
        bool write_header_on_file(std::ofstream * fileTarget);
};

const int aacFileName = 0;
const int mp3FileName = 1;
const int oggFileName = 2;
const int wavFileName = 3;

const int SUPPORTED_AUDIO_FORMAT_COUNT = 4;

extern std::string SUPPORTED_AUDIO_EXT[SUPPORTED_AUDIO_FORMAT_COUNT];

class audioResource: public standardEditableGameResource
{
    public:
        bool isPreloaded;
        int audioId;
        int audioType;
        Mix_Chunk * soundVal;
        Mix_Music * musicVal;
        GPE_ToolIconButton * playButton;
        GPE_ToolIconButton * openExternalEditorButton;
        GPE_ToolIconButton * refreshResourceDataButton;
        GPE_RadioButtonControllerBasic * audioTypeButtonController;
        std::string audioFileName[SUPPORTED_AUDIO_FORMAT_COUNT];
        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_TextInputBasic * audioGroupName;

        GPE_Label_Text * volumeLabel;
        GPE_TextInputNumber * defaultVolume;
        GPE_Slider_XAxis * volumeSlider;
        GPE_Label_Text * audioEditorMainNote;
        audioResource(GPE_ResourceContainer * pFolder = NULL);
        ~audioResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        bool copy_audio_source(std::string outDirectoryName);
        void load_audio(std::string newFileName);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self( );
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const int mp4FileName = 0;
const int webmFileName = 1;
const int oggVideoFileName = 2;

const int SUPPORTED_VIDEO_FORMAT_COUNT = 3;

extern std::string SUPPORTED_VIDEO_EXT[SUPPORTED_VIDEO_FORMAT_COUNT];

class videoResource: public standardEditableGameResource
{
    public:
        int videoId;
        int videoType;
        GPE_RadioButtonControllerBasic * audioTypeButtonController;
        std::string videoFileName[SUPPORTED_VIDEO_FORMAT_COUNT];
        GPE_Label_Text * videoEditorMainNote;
        GPE_TextInputBasic * videoGroupName;
        GPE_TextInputNumber * defaultVolume;
        GPE_ToolIconButton * openExternalEditorButton;
        GPE_ToolIconButton * refreshResourceDataButton;
        videoResource(GPE_ResourceContainer * pFolder = NULL);
        ~videoResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        bool copy_video_source(std::string outDirectoryName);
        void load_video(std::string newFileName);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self( );
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const int FONT_FILE_TYPES = 5;
const int FONT_EOT = 0;
const int FONT_SVG = 1;
const int FONT_TTF = 2;
const int FONT_WOFF = 3;
const int FONT_WOFF2 = 4;

extern std::string SUPPORTED_FONT_EXT[FONT_FILE_TYPES];

class fontResource: public standardEditableGameResource
{
    private:
        GPE_RadioButtonControllerBasic * fontTypeButtonController;
        GPE_ToolIconButton * openExternalEditorButton;
        std::string fontInEditorFileName;
        std::string fontFamilyName;
        GPE_Font * fontInEditor;
    public:
        int fontSize;
        int fontId;
        int fontType;
        int fontWidth;
        int fontHeight;
        GPE_TextInputNumber * fontSizeField;
        GPE_TextInputBasic * fontPreviewTextField;
        std::string storedFontFileNames[FONT_FILE_TYPES];
        fontResource(GPE_ResourceContainer * pFolder = NULL);
        ~fontResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        bool build_css3_file(std::ofstream * fileTarget, int leftTabAmount);
        void load_font(std::string newFileName, int newFontSize = 12);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self( );
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class functionResource: public standardEditableGameResource
{
    public:
        GPE_TextInputBasic * parametersField;
        GPE_TextInputBasic * functionReturnType;
        GPE_TextAreaInputBasic * functionCode;
        GPE_ToolIconButton * saveButton;
        functionResource(GPE_ResourceContainer * pFolder = NULL);
        ~functionResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        void integrate_into_syntax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void preprocess_self(std::string alternatePath = "");
        void prerender_self( );
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class classResource: public standardEditableGameResource
{
    public:
        GPE_TextAreaInputBasic * classHeaderCode;
        GPE_TextAreaInputBasic * classSourceCode;
        GPE_TabBar * codeCategoryTabs;
        GPE_ToolIconButton * saveButton;
        classResource(GPE_ResourceContainer * pFolder = NULL);
        ~classResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        void integrate_into_syntax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void preprocess_self(std::string alternatePath = "");
        void prerender_self( );
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const double ZOOM_LEVEL_MIN = 0.0625;
const double ZOOM_LEVEL_MAX = 16;
class animationResource: public standardEditableGameResource
{
    public:
        bool areaIsScrollable;
        double animationSpeed;
        bool isPreloaded;
        double editor0ZoomValue;
        double editor1ZoomValue;
        double minZoomValue;
        double maxZoomValue;
        int autoAnimationPos;
        GPE_Animation * spriteInEditor;

        //
        GPE_Rect animCameraRect;
        GPE_Rect animRect;

        GPE_ScrollBar_XAxis * animXScroll;
        GPE_ScrollBar_YAxis * animYScroll;
        //Right pane
        GPE_Rect  * spritePreviewCam;
        GPE_GuiElementList * rightPanel;
        GPE_Label_Text * animationSpeedLabel;
        GPE_Slider_XAxis * animationSpeedSlider;
        GPE_CheckBoxBasic * previewZoomLevel;
        GPE_CheckBoxBasic * previewSubImageNumbers;
        //Left pane
        GPE_Label_Text * labelSpriteDimensions;
        GPE_Label_Text * labelFrameInfo;
        GPE_Label_Text * labelSpriteMessage;
        GPE_ToolIconButton * editResourceButton;
        GPE_ToolIconButton * transformResourceButton;
        GPE_ToolIconButton * playPauseResourceButton;
        GPE_ToolIconButton * openExternalEditorButton;
        GPE_ToolIconButton * refreshResourceDataButton;
        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_CheckBoxBasic * showCollisionShapeCheckBox;
        GPE_CheckBoxBasic * showAnimationCheckBox;

        GPE_TextInputNumber * spriteDataFields[SPRITE_DATA_FIELD_COUNT];

        GPE_TextInputNumber * spriteCollisionRectX;
        GPE_TextInputNumber * spriteCollisionRectY;
        GPE_TextInputNumber * spriteCollisionRectW;
        GPE_TextInputNumber * spriteCollisionRectH;

        GPE_TextInputNumber * spriteCollisionCircleX;
        GPE_TextInputNumber * spriteCollisionCircleY;
        GPE_TextInputNumber * spriteCollisionCircleR;

        GPE_ToolLabelButton * spriteCenterCollisionButton;

        GPE_DropDown_Menu * spriteCollisionShapeMenu;

        GPE_ToolIconButton * previousSubImageButton;
        GPE_ToolIconButton * nextSubImageButton;
        GPE_TextInputNumber * subImageNumberField;
        GPE_TextInputNumber * subImageEditorPreviewId;
        int subImagePreviewNumber;
        double subImageMiniAnimationNumber;

        animationResource(GPE_ResourceContainer * pFolder = NULL);
        ~animationResource();
        GPE_Label_Text * labelInfoMaxTextureSize;
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        int get_preview_frame();
        void handle_scrolling();
        void load_image(std::string newFileName, bool autoProcess = false);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self( );
        void process_data_fields(float versionToProcess = -1);
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class textureResource: public standardEditableGameResource
{
    public:
        bool isPreloaded;
        GPE_Texture * textureInEditor;
        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_Label_Text * labelImageDimensions;
        GPE_Label_Text * labelTextureMessage;
        GPE_ToolIconButton * transformResourceButton;
        GPE_ToolIconButton * openExternalEditorButton;
        GPE_ToolIconButton * refreshResourceDataButton;
        textureResource(GPE_ResourceContainer * pFolder = NULL);
        ~textureResource();
        GPE_Label_Text * labelInfoMaxTextureSize;
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        int load_image(std::string newFileName);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self( );
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class tilesheetPreviewer: public GPE_GeneralGuiElement
{
    protected:
        GPE_Rect tsEditorViewRect;
        GPE_Rect tsCameraRect;
        GPE_Rect tsImageCameraRect;
        GPE_Rect tsRect;
        GPE_Rect tsSelectedArea;
        GPE_ScrollBar_XAxis * previewXScroll;
        GPE_ScrollBar_YAxis * previewYScroll;
        bool areaIsScrollable;
    public:
        double zoomValue;
        double minZoomValue;
        double maxZoomValue;
        double areaMouseXPos, areaMouseYPos;

        GPE_Label_Text * labelImageDimensions;
        GPE_Label_Text * labelFrameInfo;
        GPE_Label_Text * labelSpriteMessage;
        int tileToPrevX1,tileToPrevY1, tileToPrevX2,tileToPrevY2;
        int tilesToPlacePerRow;
        GPE_Tilesheet * tileSheetToPreview;
        std::vector< int > tilesIdsInPreview;
        bool allowDuplicates;
        bool showGrid;
        tilesheetPreviewer();
        ~tilesheetPreviewer();
        GPE_Label_Text * labelInfoMaxTextureSize;
        bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void handle_scrolling();
        void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL);
        void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
        void render_selection( int xPos = 0, int yPos = 0, GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true, double scaleSize = 1, GPE_Color * fColor = NULL);
        void reset_preview(bool moveCamera);
};

class tilesheetResource: public standardEditableGameResource
{
    public:
        bool isPreloaded;
        tilesheetPreviewer * tsPreviewer;
        GPE_Tilesheet * tilesheetInEditor;
        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_Label_Text * tilesheetDimensionsStr;
        GPE_ToolIconButton * transformResourceButton;
        GPE_ToolIconButton * openExternalEditorButton;
        GPE_ToolIconButton * refreshResourceDataButton;
        GPE_TextInputNumber * tsDataFields[TILESHEET_DATA_FIELD_COUNT];
        GPE_Label_Text * labelInfoMaxTextureSize;
        tilesheetResource(GPE_ResourceContainer * pFolder = NULL);
        ~tilesheetResource();
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        void load_image( std::string newFileName);
        void preprocess_self(std::string alternatePath = "");
        void prerender_self( );
        void process_data_fields();
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class GPE_NewsScreen: public generalGameResource
{

};

class GPE_WelcomeScreen: public generalGameResource
{

};

class collisionContainer
{
    public:
        int otherObjectType;
        std::string otherObjectName;
        GPE_TextAreaInputBasic * textEditor;
        collisionContainer();
        ~collisionContainer();

};

class customFunctionContainer
{
    public:
        std::string functionName;
        std::string functionReturnType;
        std::string functionParameters;
        GPE_TextInputBasic * returnTypeField;
        GPE_TextInputBasic * parametersField;
        GPE_TextAreaInputBasic * textEditor;
        customFunctionContainer();
        ~customFunctionContainer();

};

const int DEFAULT_OBJECT_FUNCTIONS_COUNT = 16;
const int DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT = 10;
const int DEFAULT_SPECIAL_FUNCTIONS_COUNT= 6;


const int objectFunctionTypes = 5; //normal, timed, collisions, user added, special funcions
extern std::string DEFAULT_OBJECT_FUNCTIONS_NAMES[DEFAULT_OBJECT_FUNCTIONS_COUNT];
extern std::string DEFAULT_OBJECT_SPECIALS_NAMES[DEFAULT_SPECIAL_FUNCTIONS_COUNT];

extern std::string DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[DEFAULT_OBJECT_FUNCTIONS_COUNT];
extern std::string DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[DEFAULT_SPECIAL_FUNCTIONS_COUNT];


const int FUNC_ID_PRELOGIC = 2;
const int FUNC_ID_LOGIC = 3;
const int FUNC_ID_ENDLOGIC = 5;


const int FUNC_ID_PRERENDER = 6;
const int FUNC_ID_RENDER = 7;
const int FUNC_ID_POSTRENDER = 8;
const int FUNC_ID_HUD_RENDER = 9;

class objectResParentChainLink
{
    public:
        int chainObjectType;
        objectResParentChainLink( int origObjectType);
        ~objectResParentChainLink();
        std::vector< int> heldChains;
        void grab_chain( objectResParentChainLink * chainIn = NULL);
        bool is_inchain(int chainId);
};

class GPE_ObjectComponent
{
    private:
        GPE_GeneralGuiElement * component;
        GPE_ToolIconButton * settingsGear;
    public:
        GPE_ObjectComponent();
        ~GPE_ObjectComponent();

        void load_data(std::string dataString);
        void remove_data(std::string dataString);
        void rename_component(std::string newName);
        std::string get_data();
        std::string get_name();
        std::string get_plain_string();
        std::string get_type();
        GPE_GeneralGuiElement * get_component();
        GPE_ToolIconButton * get_gear();
        void set_component(GPE_GeneralGuiElement * newComponent);
};

const int OBJ_EDITOR_MODE_COMPONENTS = 0;
const int OBJ_EDITOR_MODE_BASIC = 1;
const int OBJ_EDITOR_MODE_TIMED = 2;
const int OBJ_EDITOR_MODE_COLLISIONS = 3;
const int OBJ_EDITOR_MODE_CUSTOM = 4;
const int OBJ_EDITOR_MODE_SPECIAL = 5;

class gameObjectResource: public standardEditableGameResource
{
    public:
        GPE_Label_Title * resourceNameLabel;
        GPE_ToolIconButtonBar * editorButtonBar;
        GPE_Rect rightPanelBox;
        GPE_SelectBoxBasic * basicFunctionsSelector;
        GPE_SelectBoxBasic * timedFunctionsSelector;
        GPE_SelectBoxBasic * colliderFunctionsSelector;
        GPE_SelectBoxBasic * customFunctionsSelector;
        GPE_SelectBoxBasic * specialFunctionsSelector;
        GPE_CheckBoxBasic * checkBoxNeedsCamera;
        GPE_CheckBoxBasic * checkBoxIsMoveable;
        GPE_CheckBoxBasic * checkBoxIsVisible;
        GPE_CheckBoxBasic * checkBoxIsContinuous;
        GPE_TextAreaInputBasic * classDeclarationBox;
        GPE_TextAreaInputBasic * objectFunctions [DEFAULT_OBJECT_FUNCTIONS_COUNT];
        GPE_TextAreaInputBasic * timedObjectFunctions [DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT];
        GPE_TextAreaInputBasic * specialObjectFunctions [DEFAULT_SPECIAL_FUNCTIONS_COUNT];
        int objectFunctionInEdit[objectFunctionTypes];
        std::vector< collisionContainer * > collisionFunctions;
        std::vector< customFunctionContainer * > customFunctions;
        int spriteIndex;
        int imageIndex;
        int codeSection;
        int parentObjectId;
        GPE_Animation * spriteInEditor;
        GPE_DropDown_Resouce_Menu * spriteField;
        GPE_DropDown_Resouce_Menu * parentObjectField;
        GPE_DropDown_Resouce_Menu * newObjectToCollideDropDown;
        GPE_DropDown_Resouce_Menu * changeObjectToCollideDropDown;
        GPE_ToolPushButton * removeCollideFunctionButton;
        GPE_ToolIconButton * newCustomObjectFunctionButton;
        GPE_ToolIconButton * newCustomObjectFunctionSettingsButton;
        GPE_ToolIconButton * removeCustomObjectFunctionButton;

        //Component Related Variables
        std::vector < GPE_ObjectComponent * > customComponentRealList;
        GPE_GuiElementList * customComponentsGuiList;
        GPE_Label_Text * componentsMainGuiLabel;
        GPE_Label_Text * componentsGuiLabel;
        GPE_ToolPushButton * inheritParentComponentButton;
        GPE_ToolPushButton * resetComponentsButton;
        GPE_DropDown_Menu * addNewComponentDropDown;
        GPE_ToolIconButton * removeComponentButton;
        GPE_ToolIconButton * editCompnentButton;
        GPE_ToolIconButton * componentSettingsButton;

        gameObjectResource(GPE_ResourceContainer * pFolder = NULL);
        ~gameObjectResource();
        void add_component(std::string newComponentData);
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        collisionContainer *  find_collision_event( int otherObjId);
        customFunctionContainer * find_custom_function( std::string functionNameIn, bool createNewOne = true);
        GPE_TextAreaInputBasic * find_function_textarea(std::string functionNameIn,int functionTypeIn = 0);
        GPE_ResourceContainer * get_parent_resource();
        void integrate_into_syntax();
        void manage_components(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const int LEDITOR_MODE_SETTINGS = 0;
const int LEDITOR_MODE_LAYERS = 1;

const int LAYER_TYPE_TILES = 0;
const int LAYER_TYPE_BACKGROUND = 1;
const int LAYER_TYPE_OBJECTS = 2;
const int LAYER_TYPE_RESOURCES = 3;
const int LAYER_TYPE_MAX = 2;

const int LEDITOR_MODE_META = 4;

const int MAX_BACKGROUND_NUMBER = 7;

class GPE_SceneGameObject
{
    public:
        bool isBeingMoved;
        int customObjId;
        int objTypeId;
        std::string objTypeName;
        gameObjectResource * objBeingPlaced;
        GPE_Rect objRect;
        GPE_Input_Field_Color * objColor;
        GPE_TextInputBasic * objSubName;
        GPE_TextInputNumber * objAngleField;
        std::string objCustomVariables;
        std::vector< GPE_ObjectComponent * >objCustomValPairs;
        int objAngle;
        float objXScale;
        float objYScale;
        GPE_SceneGameObject();
        ~GPE_SceneGameObject();
        GPE_ObjectComponent *  add_variable(GPE_GeneralGuiElement * newVariable);
        void reset_components();
};

class GPE_SceneTexture
{
    public:
        GPE_ResourceContainer * projectParentFolder;
        GPE_DropDown_Resouce_Menu * backgroundInEditor;
        GPE_TextInputNumber * bgPosXField;
        GPE_TextInputNumber * bgPosYField;
        GPE_TextInputNumber * bgHorSpeedField;
        GPE_TextInputNumber * bgVertSpeedField;

        GPE_CheckBoxBasic * checkTileHori;
        GPE_CheckBoxBasic * checkTileVert;
        GPE_CheckBoxBasic * checkStretch;
        int textureId;
        textureResource * texRes;
        GPE_Rect bgRect;
        bool inFront;
        bool tileHori;
        bool tileVert;
        bool stretchBG;
        int bgXSpeed;
        int bgYSpeed;
        GPE_SceneTexture(GPE_ResourceContainer *pFolder);
        ~GPE_SceneTexture();
        void process_background_data();
};

class GPE_SceneTile
{
    public:

        int tileTypeId;
        int tileIndexId;
        int tilesheetIndexId;
        GPE_Rect tileRect;
        GPE_SceneTile();
        ~GPE_SceneTile();
};

class sceneLayer
{
    public:
        GPE_ResourceContainer * projectParentFolder;
        int layerType;
        int tileAmountX, tileAmountY;
        //int layerId;
        int layerAlpha;
        int projectLayerId;
        int prevTileAmountX, prevTileAmountY;
        std::vector< GPE_SceneTexture *> layerBackgrounds;
        std::vector <GPE_SceneTile*> mapTiles;
        std::vector< GPE_SceneGameObject * > layerObjects;
        GPE_SelectBoxBasic * layerBGSelectBox;
        sceneLayer(int newLayerType, int foundProjectLayerId, GPE_ResourceContainer *pFolder = NULL);
        ~sceneLayer();
        void add_background(GPE_SceneTexture * newBG);
        GPE_SceneTile* get_tile_at(int x, int y);
        void clear_all_objects();
        void clear_objects(int objectTypeToClear);
        void create_new_map(int newTX=8, int newTY=8, int ntileType=0);
        void resize_tilemap(int newTX=8, int newTY=8, int ntileType=0);
        int get_map_size();
};

extern std::string DEFAULT_SCENE_SUBEDITOR_NAMES[5];

class gameSceneResource: public standardEditableGameResource
{
    protected:
        int rightPaneWidth;
        GPE_Rect editorPane;
        GPE_Rect rightEditorPane;
        GPE_Rect editorView;
        GPE_Rect editorCameraRect;
        GPE_ToolIconButtonBar * editorButtonBar;
        GPE_Label_Title * sceneEditorSubTitle;
        //used for the settings/options tab
        GPE_TextInputBasic * levelTitleField;
        GPE_TextInputBasic * sceneCaptionField;
        GPE_TextInputBasic * levelHintField;

        GPE_DropDown_Resouce_Menu * musicAudioDropDown;
        GPE_DropDown_Resouce_Menu * startAudioDropDown;
        GPE_DropDown_Resouce_Menu * endAudioDropDown;

        //
        sceneLayer * currentLayer;
        GPE_DropDown_Resouce_Menu * northSceneDropDown;
        GPE_DropDown_Resouce_Menu * southSceneDropDown;
        GPE_DropDown_Resouce_Menu * eastSceneDropDown;
        GPE_DropDown_Resouce_Menu * westSceneDropDown;
        //
        GPE_TextInputNumber * levelPixelWidthField;
        GPE_TextInputNumber * levelPixelHeightField;
        GPE_TextInputNumber * levelTileWidthField;
        GPE_TextInputNumber * levelTileHeightField;

        GPE_CheckBoxBasic * preloadCheckBox;
        GPE_CheckBoxBasic * isometricCheckBox;
        GPE_CheckBoxBasic * checkBoxIsContinuous;
        GPE_Input_Field_Color * sceneBackgroundColor;
        //used for the background tab
        int bgIdInEdit;
        GPE_SelectBoxBasic * backgroundToSelect;

        textureResource * texRes;
        GPE_DropDown_Menu * sceneZoomLevel;
        double zoomValue;
        //used for the object placement tab
        GPE_ToolLabelButton * inheritParentComponentButton;
        GPE_ToolLabelButton * resetComponentsButton;
        GPE_DropDown_Menu * addNewComponentDropDown;

        GPE_DropDown_Resouce_Menu * objectInEditor;
        GPE_DropDown_Resouce_Menu * objectNewType;
        GPE_DropDown_Resouce_Menu * objectReplacerInEditor;
        GPE_SceneGameObject * selectedSceneObject;
        int customObjCreationId;
        GPE_ToolLabelButton * closeObjectProperties;
        GPE_TextInputNumber * levelObjGridWidthField;
        GPE_TextInputNumber * levelObjGridHeightField;
        GPE_CheckBoxBasic * useObjGridCheckBox;
        GPE_CheckBoxBasic * viewTileGridCheckBox;
        GPE_ToolLabelButton * forceSnapButton;
        GPE_CheckBoxBasic * objectLeftClickModeButton;
        GPE_CheckBoxBasic * onlyRemoveThisObjectTypeButton;
        GPE_ToolLabelButton * removeObjectButton;
        GPE_ToolLabelButton * clearObjectsButton;

        GPE_Label_Text * objectEditorNameTitle;
        GPE_Label_Text * objectEditorPropertiesTitle;
        int objSnapX, objSnapY;
        bool showObjLines;
        GPE_TextInputNumber * selectedObjXPos;
        GPE_TextInputNumber * selectedObjYPos;

        GPE_ToolIconButton * objCustomVariableSettingsButtton;
        GPE_ToolIconButton * objCustomVariableAddButtton;
        GPE_ToolIconButton * objCustomVariableRemoveButton;
        GPE_ToolIconButton * objCustomVariableRefeshButton;
        //used for the tile placement tab
        bool showTileLines;
        float lvlTileWidth, lvlTileHeight;
        float lvlTileAmountX, lvlTileAmountY;
        float tileToPlaceX1,tileToPlaceY1, tileToPlaceX2,tileToPlaceY2;
        int tilesToPlacePerRow;
        GPE_Rect tsPlacementArea;
        GPE_SelectBoxBasic * layerMenu;
        GPE_DropDown_Resouce_Menu * tilesheetDropDown;

        int layerInEdit;
        int tileIdPlace;
        float sceneMouseXPos, sceneMouseYPos;
        int lastCreatedObjXPos, lastCreatedObjYPos;
        int lastCreatedObjTypeId;
        GPE_Rect tsCameraRect;
        tilesheetPreviewer * tSPreviewer;
        int tilesheetRenderXPos,tilesheetRenderYPos;
        tilesheetResource * tsRes;
        std::vector <sceneLayer* >  sceneLayers;
        sceneLayer * defaultObjectLayer;
        sceneLayer * defaultBackgroundLayer;
        std::vector< int > tileIdsToPlace;
        GPE_ToolIconButton * layerSettingsButtton;
        GPE_ToolIconButton * layerAddButtton;
        GPE_ToolIconButton * layerRemoveButton;
        GPE_ToolIconButton * layerMoveUpButtton;
        GPE_ToolIconButton * layerMoveDownButton;
        GPE_ToolIconButton * layerToggleHideButton;

        GPE_ToolIconButton * layerBackgroundSettingsButtton;
        GPE_ToolIconButton * layerBackgroundAddButtton;
        GPE_ToolIconButton * layerBackgroundRemoveButton;
        GPE_ToolIconButton * layerBackgroundMoveUpButtton;
        GPE_ToolIconButton * layerBackgroundMoveDownButton;

        GPE_Label_Text * layerErrorMessage;
        bool areaIsScrollable;
        bool isDraggingObject;
    public:
        GPE_Rect sceneRect;
        GPE_GuiElementList * layerPaneList;
        bool scnPostProcessed;
        GPE_ScrollBar_XAxis * sceneXScroll;
        GPE_ScrollBar_YAxis * sceneYScroll;

        GPE_ToolIconButton * saveButton;
        gameSceneResource(GPE_ResourceContainer * pFolder = NULL);
        ~gameSceneResource();
        sceneLayer * add_layer(int layerType, int newLayerProjectLayerId );
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);

        sceneLayer * find_layer(int layeToFind);

        bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void handle_scrolling();
        void inherit_components(GPE_SceneGameObject * objectToInherit,gameObjectResource * objParent);
        void manage_components(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_scene_layers(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, GPE_Rect * renderZone = NULL, GPE_Rect * sceneCamera = NULL, double renderScale = -1,bool showEditorPreviews = true, bool checkListDependent = true,  bool forceRedraw = true);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void reset_placement_info();
        void save_resource(std::string alternatePath = "", int backupId = -1);
        void select_object(GPE_SceneGameObject * objToSelect);
        bool unselect_object();
        void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
        void update_project_layers();
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};


class gamePathResource: public standardEditableGameResource
{
    public:
        int pointPos;
        int selectedPointPos;
        GPE_PathPoint * selectedPathPoint;
        GPE_TextInputNumber * currentPointX;
        GPE_TextInputNumber * currentPointY;
        GPE_TextInputNumber * currentPointSpeed;
        GPE_Rect sceneEditorView;
        GPE_Rect scenePreviewRect;
        GPE_Rect sceneRect;
        GPE_Rect editorPane;
        GPE_Rect editorCommentPane;
        GPE_DropDown_Resouce_Menu * sceneToPreview;
        GPE_DropDown_Menu * sceneZoomLevel;
        double zoomValue;
        std::vector <GPE_PathPoint * >  pathPoints;
        GPE_SelectBoxBasic * pathOptions;
        GPE_ToolIconButton * pointSettingsButtton;
        GPE_ToolIconButton * pointRemoveButton;
        GPE_ToolIconButton * pointMoveUpButtton;
        GPE_ToolIconButton * pointMoveDownButton;

        GPE_Color * pathLineColor;
        GPE_Color * pathPointColor;
        GPE_CheckBoxBasic * pathTypeIsClosed;
        GPE_RadioButtonControllerBasic * pathShapeType;
        bool scnPostProcessed;
        GPE_ScrollBar_XAxis * sceneXScroll;
        GPE_ScrollBar_YAxis * sceneYScroll;
        bool areaIsScrollable;
        float sceneMouseXPos, sceneMouseYPos;
        GPE_GuiElementList * bottomPaneList;
        gamePathResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePathResource();
        GPE_PathPoint * add_point( int pointX, int pointY, float pointSpeed = 1);
        bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
        void clear_points();
        bool export_and_play_native( bool launchProgram = true);
        bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void handle_scrolling();
        void integrate_into_syntax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        bool remove_point( int pointId );
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void update_project_layers();
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

class definitionResource
{
    public:
        std::string myKey;
        std::string myValue;
        GPE_TextInputBasic * keyField;
        GPE_TextInputBasic * valueField;
        GPE_ToolIconButton * removeSelfButton;
        definitionResource(std::string key, std::string value);
        ~definitionResource();
};

class dictionaryResource: public standardEditableGameResource
{
    public:
        std::vector <definitionResource * >  myDefinitions;
        GPE_ToolPushButton * addDefinitionButton;
        GPE_ToolPushButton * clearDictionaryButton;
        dictionaryResource(GPE_ResourceContainer * pFolder = NULL);
        ~dictionaryResource();
        definitionResource * add_definition( std::string key, std::string value ="");
        void clear_dictionary();
        bool export_and_play_native( bool launchProgram = true);
        bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void integrate_into_syntax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void update_project_layers();
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
        bool word_exists( std::string needleWord, int allowedPos = -1);
};

class projectPropertiesResource: public standardEditableGameResource
{
    public:
        //Icon data and such
        std::string iconFileName;
        GPE_Label_Text * projectIconLabel;
        GPE_Label_Image * projectIconImg;
        GPE_ToolLabelButton * browseIconButton;
        //Meta information to be built into the project
        GPE_TextAreaInputBasic * projectGameMacros;
        GPE_TextAreaInputBasic * projectGameNotes;
        GPE_CheckBoxBasic * showMetaData;

        //Export Settings Tab
        GPE_SelectBoxBasic * exportSettingsBar;
        //Export Settings [ HTML5]
        GPE_TabBar * projectHTML5SettingsTabBar;

        GPE_TextAreaInputBasic * projectCSSCode;
        GPE_TextAreaInputBasic * projectHtmlCode;
        GPE_TextAreaInputBasic * projectHtmlHeaderCode;

        GPE_CheckBoxBasic * obfuscateCodeCheckbox;
        GPE_CheckBoxBasic * minifyCodeCheckbox;
        GPE_CheckBoxBasic * warnOnCloseCheckbox;
        GPE_CheckBoxBasic * headerCodeBeforeGPECheckbox;

        GPE_ToolPushButton * exportPushButton;
        GPE_ToolPushButton * exportAndPlayPushButton;
        GPE_ToolPushButton * cleanBuildFolderPushButton;
        GPE_ToolPushButton * cleanBuildAllFolderPushButton;
        GPE_ToolPushButton * playProgramPushButton;

        //[Export Settings Desktop]
        GPE_TabBar * desktopSettingsTabBar;
        GPE_Label_Text * exportApplicationLabel;
        GPE_Label_Text * exportWindowsLabel;
        GPE_Label_Text * exportOSXLabel;
        GPE_Label_Text * exportLinuxLabel;
        GPE_CheckBoxBasic * win64BitCheckbox;
        GPE_CheckBoxBasic * osx64BitCheckbox;
        GPE_CheckBoxBasic * linux64BitCheckbox;

        GPE_CheckBoxBasic * html5DebugModeCheckbox;
        GPE_CheckBoxBasic * wiiuDebugModeCheckbox;
        GPE_CheckBoxBasic * winDebugModeCheckbox;
        GPE_CheckBoxBasic * osxDebugModeCheckbox;
        GPE_CheckBoxBasic * linuxDebugModeCheckbox;

        GPE_RadioButtonControllerBasic * html5nativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * windowsNativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * osxNativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * linuxNativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * wiiuNativeBuildTypeOptions;
        GPE_RadioButtonControllerBasic * switchNativeBuildTypeOptions;
        GPE_Label_Text * electronCompileNote;
        GPE_TextURL * electronCompileUrl;

        GPE_Label_Title * comingSoonTitle;
        GPE_Label_Text * wiiuCompileNote;
        GPE_TextURL * wiiuCompileUrl;

        GPE_Label_Paragraph * switchComingSoonNote;
        GPE_Label_Paragraph * nSwitchSideNote;
        GPE_Label_Paragraph * nSwitchThirdNote;
        GPE_TextURL * nSwitchContactUrl;

        GPE_Label_Paragraph * ps4ComingSoonNote;
        GPE_Label_Paragraph * ps4SideNote;
        GPE_TextURL * ps4ContactUrl;

        GPE_Label_Paragraph * xboxOneComingSoonNote;
        GPE_Label_Paragraph * xboxOneSideNote;
        GPE_TextURL * xboxOneContactUrl;

        //Settings Tab
        GPE_TextInputNumber * projectScreenWidthField;
        GPE_TextInputNumber * projectScreenHeightField;
        GPE_TextInputNumber * projectGameVersionField;
        GPE_TextInputBasic * projectGameSubVersionField;
        GPE_RadioButtonControllerBasic * projectScaleSettings;
        GPE_Label_Text * projectSettingsFPSRateLabel;
        GPE_DropDown_Menu * projectSettingsFPSRate;
        //Publisher Setting
        GPE_Label_Title * sectionTitlePublisher;
        GPE_Label_Title * sectionTitleGameSettings;
        GPE_TextInputBasic * projectGameTitleField;
        GPE_TextInputBasic * projectGameShortTitleField;
        GPE_TextInputBasic * projectGameDateField;
        GPE_TextInputBasic * projectGameDescriptionField;
        GPE_TextInputBasic * projectGamePublisherField;
        GPE_TextInputBasic * projectGameDeveloperField;
        GPE_TextInputBasic * projectGameCopyrightField;
        GPE_TextInputBasic * projectGameWebsiteField;
        GPE_TextInputBasic * projectGameEmailField;
        GPE_CheckBoxBasic * checkBoxShowPublisherInfo;

        //Colors Tab
        GPE_Label_Title * sectionTitleGameColors;
        GPE_Input_Field_Color * projectBorderColor;
        GPE_Input_Field_Color * projectGameBackgroundColor;
        GPE_Input_Field_Color * projectWebsiteBackgroundColor;
        GPE_Input_Field_Color * projectTextHeaderColor;
        GPE_Input_Field_Color * projectTextParagraphColor;
        GPE_Input_Field_Color * projectTextLinkColor;
        GPE_Input_Field_Color * projectTextLinkActiveColor;
        GPE_Input_Field_Color * projectTextLinkHoverColor;
        GPE_Input_Field_Color * projectTextLinkVisitedColor;
        //Extras Tab
        GPE_ToolPushButton * transformImageDirectoryButton;
        GPE_Rect subViewedSpace;
        GPE_TabBar * projectSettingsTabBar;
        GPE_ToolIconButtonBar * sceneEditorPaneMode;

        GPE_GuiElementList * projectSettingsList;
        GPE_CheckBoxBasic * projectCollisionLayerMatrix[1024];
        GPE_Label_90Degree * projectLayerNamesColumnTitles[32];
        GPE_Label_Text * projectLayerNamesRowTitles[32];
        GPE_Label_Text * projectLayerEmptyTitle;
        GPE_ToolLabelButton * selectAllCollisionBoxes;
        GPE_ToolLabelButton * unselectAllCollisionBoxes;

        projectPropertiesResource(GPE_ResourceContainer * pFolder = NULL);
        ~projectPropertiesResource();
        bool export_and_play_native( bool launchProgram = true);
        void integrate_into_syntax();
        void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        int search_for_string(std::string needle);
        int search_and_replace_string(std::string needle, std::string newStr = "");
        void update_project_layers();
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

const int GPE_EXTERNAL_EDITOR_MAX = 4;

const int GPE_EXTERNAL_EDITOR_IMG = 0;
const int GPE_EXTERNAL_EDITOR_AUD = 1;
const int GPE_EXTERNAL_EDITOR_VID = 2;
const int GPE_EXTERNAL_EDITOR_FONT = 3;

class GPE_Theme_Holder
{
    public:
        std::string themeName;
        std::string templateFileName;
        std::string templateBgFileName;
        GPE_TextInputBasic * nameInEditor;
        bool nonDefaultTheme;
        std::vector < GPE_Input_Field_Color * > colorsInEditor;
        GPE_Label_Image * themeIconImg;
        GPE_TextInputBasic * themeBgFileLocation;
        GPE_Theme_Holder( std::string tName, std::string tFileName, bool isCustomTheme = false  );
        ~GPE_Theme_Holder();
        void clear_colors();
        bool copy_theme( GPE_Theme * systemTheme, bool copyToSelf = true );
        bool load_background( std::string bgTextureLocation, bool allOrNothing = false);
};

class gamePencilEditorSettingsResource: public standardEditableGameResource
{
    public:
        //Compiler Settings
        GPE_CheckBoxBasic * showShortProjectNames;
        //Added as of 1.13 [ BEGIN ]
        GPE_CheckBoxBasic * renderSceneBGColor;
        //Added as of 1.13 [ END ]

        //General Editor Settings
        GPE_CheckBoxBasic * autoSaveScreenshots;
        GPE_CheckBoxBasic * makeMetaScreenshots;

        //FPS Related Items
        GPE_Label_Title * editorGuiSettingsLabel;
        GPE_Label_Title * ideSettingsFPSRateLabel;
        GPE_DropDown_Menu * ideSettingsFPSRate;
        GPE_Label_Text * ideFPSRatioLabel;
        GPE_Label_Text *    ideButtonBarSizeLabel;
        GPE_CheckBoxBasic * showFPSOnEditor;

        GPE_DropDown_Menu * ideButtonBarSize;
        GPE_RadioButtonControllerBasic * ideButtonBarAlignment;
        GPE_TextInputBasic * pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_MAX];
        GPE_ToolIconButton * pencilExternalEditorsLoadButton[GPE_EXTERNAL_EDITOR_MAX];
        GPE_SelectBoxBasic * subEditorSelectBox;

        GPE_TextInputNumber * editorTextAreaDelayTime;
        GPE_TextInputNumber * editorTextInputDelayTime;
        GPE_TextInputNumber * editorNormalDelayTime;
        GPE_TextInputNumber * editorScrollSpeed;

        GPE_Label_Title * editorStatupTipsLabel;
        GPE_CheckBoxBasic * showStatupTipsBox;
        GPE_CheckBoxBasic * launchLastProjectBox;
        GPE_DropDown_Menu * projectAutoSaveRate;
        GPE_Label_Text * projectAutoSaveRateLabel;

        //Tab Section
        GPE_Label_Title * tabSectionLabel;
        GPE_CheckBoxBasic * mouseAutoFindTabs;
        GPE_TextInputNumber * tabSpaceSize;

        //Themes Section
        GPE_Label_Title * themesLabel;
        GPE_DropDown_Menu * themePicker;
        GPE_ToolIconButton * themeLoadButton;
        GPE_ToolIconButton * themeAddButton;
        GPE_ToolIconButton * themeRemoveButton;
        GPE_ToolLabelButton * themeSetDefaultButton;
        GPE_TextURL * itchLinkForThemes;
        std::vector< GPE_Theme_Holder * > themeHolders;
        int defaultTemplateEndPos;
        int currentThemeInEdit;

        GPE_Label_Text * themeBgLabel;
        GPE_ToolLabelButton * themeBgBrowseButton;
        //Advanced Section
        GPE_Label_Title * advancedAreaLabel;
        GPE_CheckBoxBasic * showHiddenFilesInBrowser;
        GPE_CheckBoxBasic * forceFrameRedraw;
        GPE_ToolPushButton * clearCacheButton;

        //Editor Related variables
        GPE_Rect gpeLogoSpace;
        GPE_Rect subViewedSpace;
        GPE_TabBar * editorPageTabBar;
        GPE_GuiElementList * editorPageList;
        std::string projectFolderListLocation;
        gamePencilEditorSettingsResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilEditorSettingsResource();
        void load_themes_from_folder( std::string themeFolder );
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gamePencilEditorSettingsResource * MAIN_EDITOR_SETTINGS;

class gameJSCompilerSettingsResource: public standardEditableGameResource
{
    public:
        //Compiler Settings
        GPE_CheckBoxBasic * stopCompileOnError;
        //Added as of 1.15 [ BEGIN ]
        GPE_CheckBoxBasic * minifyCode;
        GPE_CheckBoxBasic * pluginConstantValues;
        GPE_CheckBoxBasic * obfuscateCode;
        GPE_TextInputBasic * obfuscatorDirectoryField;
        GPE_ToolIconButton * obfuscatorDirectoryLoadButton;
        GPE_TextInputBasic * googleClosureCompilerFile;
        GPE_ToolIconButton * googleClosureCompilerLoadButton;
        //Added as of 1.15 [ END ]
        GPE_Rect gpeLogoSpace;
        GPE_Rect subViewedSpace;
        GPE_TabBar * editorPageTabBar;
        GPE_GuiElementList * editorPageList;
        std::string projectFolderListLocation;
        gameJSCompilerSettingsResource(GPE_ResourceContainer * pFolder = NULL);
        ~gameJSCompilerSettingsResource();
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gameJSCompilerSettingsResource * GPE_JS_COMPILER_SETTINGS;

class cppCompilerManager
{

};

class gameCPPCompilerSettingsResource: public standardEditableGameResource
{
    public:
        //Compiler Settings
        GPE_DropDown_Menu * compilerInUse;
        GPE_ToolLabelButton * defaultCompilerButton;
        GPE_ToolLabelButton * copyCompilerButton;
        GPE_ToolLabelButton * renameCompilerButton;
        GPE_ToolLabelButton * resetDefaultsCompilerButton;
        GPE_CheckBoxBasic * stopCompileOnError;
        GPE_Rect gpeLogoSpace;
        GPE_Rect subViewedSpace;
        GPE_TabBar * editorPageTabBar;
        GPE_GuiElementList * editorPageList;
        std::string projectFolderListLocation;
        gameCPPCompilerSettingsResource(GPE_ResourceContainer * pFolder = NULL);
        ~gameCPPCompilerSettingsResource();
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

extern gameCPPCompilerSettingsResource * GPE_CPP_COMPILER_SETTINGS;


class gamePencilAboutPageResource: public standardEditableGameResource
{
    public:
        //Meta information to be built into the project
        GPE_TextAreaInputBasic * pencilCredits;
        GPE_TextAreaInputBasic * pencilDescription;
        GPE_TextAreaInputBasic * pencilInformation;
        GPE_TextAreaInputBasic * pencilLicense;
        GPE_TextAreaInputBasic * pencilMissionStatement;
        GPE_Rect subViewedSpace;
        GPE_TabBar * aboutPageTabBar;
        GPE_GuiElementList * aboutPageList;
        gamePencilAboutPageResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilAboutPageResource();
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilAboutPageResource * MAIN_ABOUT_PAGE;

class gamePencilHelpPageResource: public standardEditableGameResource
{
    public:
        //General Help Tab
        GPE_Label_Title * helpfulButtonsTitle;
        GPE_Label_Title * helpfulLinksTitle;
        GPE_Label_Title * socialMediaTitle;
        std::vector< GPE_ToolPushButton * > helpfulButtons;
        std::vector< GPE_TextURL * > helpfulLinks;
        std::vector< GPE_ToolIconButton * > socialMediaSites;

        GPE_TextAreaInputBasic * pencilInformation;
        GPE_TextAreaInputBasic * pencilChangelog;
        GPE_TextAreaInputBasic * pencilShortkeys;
        GPE_ToolPushButton * updateCheckerButton;
        GPE_Rect subViewedSpace;
        GPE_TabBar * helpPageTabBar;
        GPE_GuiElementList * helpPageList;
        gamePencilHelpPageResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilHelpPageResource();
        void add_helpfulbutton(GPE_ToolPushButton * newButton);
        void add_helpfullink(GPE_TextURL * newLink);
        void add_socialmedia_url( GPE_ToolIconButton * newLink);
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilHelpPageResource * MAIN_HELP_PAGE;

class gamePencilStartPageResource: public standardEditableGameResource
{
    public:
        //Meta information to be built into the project
        GPE_Rect subViewedSpace;
        GPE_WrappedTextArea * testText;
        GPE_Label_Title * labelEditorShortcuts;
        GPE_Label_Title * labelPatreon;
        GPE_Label_Image * labelPatreonLogo;
        GPE_Label_Text * labelPatreonDescription;
        GPE_TextURL * urlPatreonPage;

        GPE_Label_Title * labelHelpfulLinks;
        GPE_Label_Image * labelImgGPELogo;
        GPE_Label_Text * labelVersionInfo;
        GPE_Label_Text * labelCopyrightInfo;
        //Social Media
        GPE_Label_Title * labelSocialMedia;
        GPE_ToolPushButton * socialFacebookButton;
        GPE_ToolPushButton * socialGooglePlusButton;
        GPE_ToolPushButton * socialTwitterButton;
        GPE_ToolPushButton * socialTilweButton;
        GPE_ToolPushButton * socialYouTubeButton;
        GPE_TabBar * startPageTabBar;
        std::vector< GPE_TextURL * > editorShortcuts;
        std::vector< GPE_TextURL * > helpfulWebLinks;
        GPE_ToolPushButton * projectBrowserButton;
        GPE_ToolPushButton * forumPageButton;
        GPE_ToolPushButton * helpPageButton;
        GPE_ToolPushButton * tipOfTheDayButton;

        GPE_GuiElementList * startPageList;
        gamePencilStartPageResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilStartPageResource();
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);

};

extern gamePencilStartPageResource * MAIN_START_PAGE;

class quickProjectReader
{
    public:
        int resourcesInProject;
        std::string detectedName;
        std::string detectedIcon;
        std::string detectedLanguage;
        std::string detectedLocation;
        std::string detectedSaveTime;
        std::string detectedVersion;
        quickProjectReader();
        ~quickProjectReader();
        bool review_project(std::string projectFileName);
};

class gamePencilProjectBrowserResource: public standardEditableGameResource
{
    public:
        bool processedFirst;
        //Meta information to be built into the project
        GPE_Label_Title * projectsBrowserTitle;
        GPE_Label_Text * projectFolderLocationLabel;
        GPE_Label_Text * projectsTitle;
        GPE_Label_Text * recentProjectsTitle;
        GPE_GuiElementList * projectBrowserList;
        GPE_ToolPushButton * scanProjectFolderButton;
        GPE_ToolPushButton * newProjectButton;
        GPE_ToolPushButton * loadProjectButton;
        GPE_ToolPushButton * refreshButton;
        GPE_Label_Text * emptyProjectsListLabel;
        std::string foundProjectsDirectory;
        std::vector< GPE_ToolPushButtonMultiLine * > foundProjectsList;
        //Added as of Version 1.1.3 [ BEGIN ]
        GPE_GuiElementList * headerPageList;
        GPE_TabBar * projectsCategoryTabs;
        std::vector< GPE_ToolPushButtonMultiLine * > examplesProjectsList;
        GPE_Rect headerViewedSpace;
        GPE_Rect subViewedSpace;
        int headerSpaceHeight;
        GPE_Label_Text * exampleProjectsTitle;
        //Added as of Version 1.1.3 [ END ]
        std::vector< GPE_ToolPushButtonMultiLine * > recentProjectsList;
        gamePencilProjectBrowserResource(GPE_ResourceContainer * pFolder = NULL);
        ~gamePencilProjectBrowserResource();
        void prerender_self( );
        void preprocess_self(std::string alternatePath = "");
        void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
        void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
        void save_resource(std::string alternatePath = "", int backupId = -1);
        bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
        void load_example_projects_folder( );
        void load_projecs_folder( std::string projectsDirectory);
};

extern gamePencilProjectBrowserResource * PROJECT_BROWSER_PAGE;

class GPE_Editor_State : public GPE_ProgramState
{
    private:
        //Intro message
        GPE_Texture * message;
        GPE_Toolbar * mainToolBar;
        GPE_ToolIconButtonBar * mainButtonBar;
        GPE_ResourceManagementBar * mainResourceBar;
    protected:
        //Intro background
        GPE_Texture *background;
    public:
        GPE_Texture * country_language_image;
        SDL_version SDL_compiled_version;
        std::string SDL_VersionText;
        //Loads intro resources
        GPE_Editor_State();
        //Frees intro resources
        ~GPE_Editor_State();

        //Main loop functions
        virtual void process_input();
        virtual void apply_logic();
        virtual void clean_up();
        virtual void render();
};

#endif
