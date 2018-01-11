/*
GPE_Engine.h
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

#ifndef GPE_ENGINE_H
#define GPE_ENGINE_H

#include "GPE_CIncludes.h"
#include "GPE_Shared_Resources.h"
#include "GPE_Functions.h"
#include "GPE_Globals.h"

extern bool GPE_IS_LOADING;
const int GPE_MAX_CAMERAS = 8;
const int GPE_MAX_DEFAULT_TIMERS = 10;
extern int GPE_MAX_OBJECT_TYPES;

//The base class for Game Objects
class GPE_GameObject
{
    private:
        //Path Related Variables [BEGIN]
        int pathId;
        int foundPathObject;
        int pathPosition;
        int pathRelativeX;
        int pathRelativeY;
        int pathTargetX;
        int pathTargetY;
        int pathTargetPosition;
        bool pathReplayOnEnd;
        double pathSpeed;
        double pathCurrentSpeed;
        double pathCurrentDirection;
        bool pathGoingForward;
        //Path Related Variables [END]
        int gpeObjectType;
        std::string gpeObjName;

        int spriteId;
        int gpeUniqGameObjectId;

        bool isOutsideScene;
        int lastCollisionObjId;
        bool applyGravity;
        double gravityDirection;
        double gravityEffect;
        double spriteFrame;
        double imageSpeed;
        bool animationEnded;
        bool isVisible;
        double xBegin;
        double xPos;
        double x2Pos;
        double yBegin;
        double yPos;
        double y2Pos;
        double xEnd;
        double yEnd;
        double xVel;
        double yVel;
        double xPast;
        double x2Past;
        double yPast;
        double y2Past;
        double xspeed;
        double yspeed;
        double direction;
        double speed;
        double defaultSpeed;
        int animationDuration;
        bool isInit;
        bool isViewIndependent;
        bool isInView[GPE_MAX_CAMERAS];
        bool timeToCheckForInView[GPE_MAX_CAMERAS];
        GPE_Rect * collisionBox;
    public:
        bool gpeIsBeingDestroyed;
        bool prerenderedInFrame[GPE_MAX_CAMERAS];
        bool renderedInFrame[GPE_MAX_CAMERAS];
        bool renderedAboveInFrame[GPE_MAX_CAMERAS];
        bool hudrenderedInFrame[GPE_MAX_CAMERAS];

        int functionTimer[GPE_MAX_DEFAULT_TIMERS];
        int sceneLayerPos;
        int sceneLayerStartPos;
        /*Make into a MAP
        customComponents = {};
        */
        int gpeSceneStartId;
        GPE_Color * outlineTestColor;
        int objectDepth;
        int prevObjectDepth;
        int objectDepthPosition;
        bool GPE_AddedToGridSystem;
        bool gpeLogicAppliedInFrame;
        bool gpePrelogicAppliedInFrame;
        bool gpeAddedToRenderList;
        bool gpeAddedToContinuousList;
        bool hasPreRenderFunction;
        bool hasRenderFunction;
        bool hasPostRenderFunction;
        bool gpeHasTimedFunctions;
        bool hasHudRenderFunction;
        bool gpeHasStartLogicFunction;
        bool gpeHasLogicFunction;
        bool gpeSceneStartCodeUsed ;

        std::vector< int >OBJECT_COLLISION_LIST;
        std::vector< int >SPATIAL_PARTITION_LIST;
        int GPE_SearchForCollisionBegin;

        GPE_GameObject( int objectType,  int xPosIn,  int yPosIn,  int objectLayerId);
        ~GPE_GameObject();
        virtual void timedFunction0();
        virtual void timedFunction1();
        virtual void timedFunction2();
        virtual void timedFunction3();
        virtual void timedFunction4();
        virtual void timedFunction5();
        virtual void timedFunction6();
        virtual void timedFunction7();
        virtual void timedFunction8();
        virtual void timedFunction9();
        bool under_mouse( int cameraId );
        GPE_GameObject * add_object(int objType, int nX, int nY);
        void bounce_from( GPE_GameObject * otherObj);
        virtual void collide_with( GPE_GameObject * otherObj);
        void initialize();
        bool equals(GPE_GameObject * otherObj);
        void default_load();
        std::string get_component(std::string componentName);
        void gpe_constructor();
        int get_depth();
        int get_collision_height();
        int get_collision_width();

        double get_my_direction(double x2,double  y2);
        double get_my_distance(double x2, double y2);
        int get_id();
        int get_object_type();
        std::string get_name();
        int get_sprite();
        double getx();
        double getx2();
        double gety();
        double gety2();
        virtual void end_frame_logic();
        void gravity_use();
        bool is_init();
        bool is_visible();
        void init_sprite(int nextSprite );
        void make_view_dependent();
        void make_view_independent();
        void move_left(double leftMore);
        void move_right(double rightMore);
        void move_up(double upMore);
        void move_down(double downMore);
        void move_towards( double targX, double targY, double newSpeed);
        void path_begin(int path, double pathSpeed, bool continueOnEnd = true, bool relativePath = false);
        void path_start(int path, double pathSpeed, bool continueOnEnd = true, bool relativePath = false);
        bool path_move_along();
        bool path_move_towards_point(int pointId);
        void path_end();
        void path_stop();
        bool using_path();
        int get_path_id();
        //get_path_object();
        double get_path_targetx();
        double get_path_targety();
        int get_path_position();
        int get_path_original_size();
        int get_path_size();
        virtual void perform_object_logic();
        virtual void perform_object_prelogic();
        void process_gpe_movement();
        void process_timed_functions();
        virtual void render_above();
        virtual void render_under();
        virtual void render_hud();
        virtual void render_self();
        virtual void scene_end();
        virtual void scene_start();
        virtual void self_obliterate();
        void self_destruct();
        void set_sprite(int nextSprite = -1);
        void setx(double newX);
        void sety(double newY);
        void speed_move();
        void set_velocity(double newDir, double newSpeed );
        void start_countdown ( int timerId, int countDownStart);
        void set_countdown ( int timerId, int countDownStart);
        void set_timer ( int timerId, int secondsToCount);
        void start_timer ( int timerId, int secondsToCount);
        virtual void start_frame_logic();
        void update_animation();
        void update_cords( double newX,  double newY);
        bool touches_tile(int layerToCheck);
        bool touches_certain_tile(int layerToCheck, int needleTile);
};

class GPE_GameObject_List
{
    public:
        std::vector<GPE_GameObject *> internalArray;
        int objListType;
        GPE_GameObject_List();
        ~GPE_GameObject_List();
        void add_object(GPE_GameObject * objectIn);
        void remove_held_object(GPE_GameObject * objectIn);

};

class GPE_SuperSpatialPartition
{
    private:
        std::vector<GPE_GameObject * > internalArray;
        std::vector<GPE_GameObject_List* >heldObjectsArray;
        int objectsHeld;
        GPE_Rect * boxArea;
    public:
        std::string spaceName;
        GPE_SuperSpatialPartition();
        ~GPE_SuperSpatialPartition();
        void add_object(GPE_GameObject * newObject);
        void empty_list();
        int get_size();
        GPE_GameObject * get_object(int pos);
        void set_space(int newX, int newY, int newW, int newHeight);
        void remove_held_object(GPE_GameObject * objectIn);
};

class GPE_SceneCamera
{
    private:
        GPE_Rect * cameraRect;
        GPE_Rect * cameraBoundaries;

        GPE_Rect * renderRect;
        GPE_Rect * scaledRect;
        int viewTarget;
        int horiBorder;
        int vertBorder;

        double hSpeed;
        double vSpeed;

        bool isVisible;

        int maxXTilesInView;
        int maxYTilesInView;

        int cSceneWidth;
        int cSceneHeight;
    public:
        GPE_SceneCamera(double cameraX, double cameraY, double cameraW, double cameraH, int cameraIdNumber );
        ~GPE_SceneCamera();
        void center_camera ( double centerX, double centerY);
        void move_hori ( double movSpace );
        void move_vert ( double movSpace );
        void set_visible( bool visiblility );
        void setup_camera (double cameraX,double cameraY, double cameraW, double cameraH);
        void update_tile_size (int newTX, int newTY);
        void update_screen_space ( double cameraX, double cameraY, double cameraW, double cameraH);
        void update_view_space ( double cameraX, double cameraY, double cameraW, double cameraH);
};

class GPE_GameMapTile
{
    private:
        GPE_Rect * tileBox;
		int xCoord;
		int yCoord;
    public:
        int tset;
		int tileIdNumb;
		int tSetPos ;

		int tileLayer;
		int tileType;
		bool isSolid;
		bool drewBefore;
        GPE_GameMapTile();
        ~GPE_GameMapTile();
        int get_type();
        void update_box ( int xNew,  int yNew,  int wNew, int hNew);
        void render_self( GPE_Rect * cam );
        void render_self_auto( GPE_Rect * cam );
};

class GPE_Background
{
    private:
        GPE_Texture * backgroundTexture;
		double bgXOffset;
		double bgYOffset;
		double bgXSpeed;
		double bgYSpeed;
		int bgId;
		GPE_Color * bgColor;
		bool renderBgSpeed;
		bool loopBGHori;
		bool loopBGVert;
		bool stretchBG;
    public:
        GPE_Background();
        ~GPE_Background();
        int get_background_id();
        GPE_Color * get_scene_background_color();
        void render_bg_speed( int xx, int yy, GPE_Rect *  cam, GPE_Rect * renderCam);
        void render_bg( GPE_Rect * renderToArea, GPE_Rect *  cam, GPE_Rect * renderCam, int lvlW, int lvlH);
        void scroll_bg();
        void set_bgspeed( double speedX, double speedY );
        void set_bgoffsets( double nextX, double nextY );
        void set_background( int newBG);
        void set_background_color(GPE_Color * colorIn);
};

class GPE_Layer
{
    private:
        int layerType;

        int collisionPairsTotal;
		int parsed_objects;
        //Spatial Grid stuff
        bool spatialGridIsCreated;
		int spatialMapWidthSize;
		int spatialMapHeightSize;
        int spatialGridBoxSize;
		int spatialGridWidthSize;
		int spatialGridHeightSize;
        int spatialGridWidthAmount;
		int spatialGridHeightAmount;
		int objectCollisionHashesChecked;
		int spatialCollisionChecks;
		std::vector<GPE_GameObject * > layerObjects;
		std::vector<GPE_GameObject * > independentObjects;
		std::vector<GPE_Background * >layerBackgrounds;

		/*
		scnStartBackgrounds = [];
		scnStartObjects = [];
		scnStartTiles  = [];
		*/
		int tileAmountX;
		int tileAmountY;
		int layerId;
		int layerAlpha;
		std::string layerName;
		int prevTileAmountX;
		int prevTileAmountY;

		std::vector< GPE_GameMapTile *> layerTiles;

		//Collision related variables
		std::vector<GPE_SuperSpatialPartition * > collisionSpacesInView;
		std::vector<GPE_SuperSpatialPartition * > COLLISION_AREA_SPACES; //Used for spatial partitioning collision checker
		GPE_SuperSpatialPartition * COLLISION_AREA_OUTSIDE_SCENE;
		bool superDebugCollisions;


		std::map<std::string,bool> objectCollisionIsChecked;
		std::vector<GPE_GameObject * > collisionPairA;
		std::vector<GPE_GameObject * > collisionPairB;

		std::vector<GPE_GameObject * > prerenderObjectRenderList;
		std::vector<GPE_GameObject * > regularObjectRenderList;
		std::vector<GPE_GameObject * > renderAboveObjectRenderList;
		std::vector<GPE_GameObject * > renderHudObjectRenderList;
    public:
        GPE_Layer(int newLayerType, int newLayerId, double newLayerAlpha = 1);
        ~GPE_Layer();
        void add_collision_pair( GPE_GameObject *objPairA, GPE_GameObject *objPairB);
        void begin_spatialpartioning( int cSceneWidth, int cSceneHeight );
        void begin_render ( std::vector< int >&spacesInView );
        void clear_dynamic_data();
        void clear_spaces();
        void clear_tiles();
        void create_new_map ( int newTX, int newTY);
        int find_spatial_at( int xIn, int yIn);
        GPE_GameMapTile * get_tile_at ( int xIn,  int yIn );
        int get_id();
        int get_map_size();
        int get_type();
        void remove_object ( GPE_GameObject *objToRemove);
        void render_hud_objects();
        void render_objects();
        void replace_all_backgrounds ( int needleBgId, int newBgId );
        void scroll_backgrounds();
        void set_default_grid_size();
        void set_layer_background ( int layerBeingChanged, int bgId, int newBgId );
};

struct GPE_SceneLayer_Meta
{
    int lId;
    int lType;
    double lAlpha;
};

class GPE_GameScene
{
    private:
        int sceneId;
        bool sceneHasBeenActivated;
        int sceneWidth;
        int sceneHeight;
        bool sceneIsContinuous;
        bool sceneWasVisitedBefore;
        std::string sceneName;
        std::string defaultLayerName;

        //tile data info
        int tileAmountX;
        int tileAmountY;
        int tileWidth;
        int tileHeight;
        std::vector< GPE_GameObject * >sceneObjects;
        std::vector< GPE_GameObject * >persistentObjects;
        std::vector< GPE_Layer * >sceneLayers;
        std::vector< GPE_SceneLayer_Meta * >scenesStartLayers;

        //background data
        GPE_Color * bgColor;
        int sceneVersion;
        int bgMusic;
        int startAudio;
        int endAudio;

        bool sceneIsProcessed;
        bool sceneResourcesAreProcessed;
        bool sceneIsSpatiallyPartitioned;
        GPE_SceneCamera * sceneCamera[GPE_MAX_CAMERAS];
    public:
        GPE_GameScene(int sId, std::string sName);
        ~GPE_GameScene();
        GPE_Layer * add_layer( int newLayerType, int newLayerId, double newLayerAlpha );
        GPE_Layer * add_start_layer( int newLayerType, int newLayerId, double newLayerAlpha );
        void disable_camera ( int cameraId);
        void enable_camera ( int cameraId);
        void reset_scene_meta();
        void clear_layers_data();
        bool add_to_persistent_objects( GPE_GameObject * nObject);
        GPE_Layer * find_layer ( int layerIdIn, int seekedType);
        GPE_Layer * get_layer ( int layerIdIn, int newLayerType);
        int get_scene_width();
        int get_scene_height();
        int find_tile ( int layerToCheck, int xPos, int yPos );
        int find_tile_in_box( int layerToCheck, int x1,  int y1,  int x2, int y2 );
        bool find_matching_tile( int layerToCheck, int xPos,  int yPos,  int tileTypeToSearchFor);
        bool find_matching_tile_in_box( int layerToCheck, int x1,  int y1,  int x2, int y2 ,  int tileTypeToSearchFor);
        bool scene_is_ready();
        void scene_init();
        void update_scene_specs( int lvlW, int lvlH);
        void update_tilemap();
        void add_background( int bSurf, bool loopH, bool loopV, double bgX, double bgY, double bgXSpd, double bgYSpd);
        void replace_background( int layerBeingChanged, int needleBgId, int newBgId );
        void replace_all_backgrounds(  int needleBgId, int newBgId );
        void set_background( int layerBeingChanged, int needleBgId, int newBgId );

        int get_twsize();
        int get_thsize();
        int get_map_width();
        int get_map_height();
        void apply_logic();
        void apply_postlogic();
        std::string get_scene_name();
        void set_scene_name(std::string inSceneName);
        change_background( int newBG, int bgNumber);
        change_foreground( int newBG, int bgNumber);
        int snap_xto_grid(int xPos);
        int snap_yto_grid(int yPos);
        void load_game_scene(std::string sceneFileName);
        void load_and_parse_scene();
        bool process_scene();
        void render_background_color();
        void render_backgrounds();
        void render_foregrounds();
        void render_scene();
};

class GPE_MainGame
{
    private:

    public:
        GPE_MainGame();
        ~GPE_MainGame();
};

void limit_space_to_rect(GPE_Rect * rectIn = NULL,int * limitedX = NULL, int * limitedY = NULL);

class fontPair
{
    private:
        std::string str;
        SDL_Texture * strTexture;
        int textWidth, textHeight;
    public:
        int lastAlphaRendered;
        fontPair(TTF_Font * fontIn, std::string strIn);
        ~fontPair();
        bool texture_matches(std::string strCheck);
        SDL_Texture * get_texture();
        int get_width();
        int get_height();
};

const int GPE_MonoNumberPairCount = 15;
class GPE_Font
{
    private:
        std::string fontFileLocation;
        std::string fontFamilyName;
        int fontSize;
        bool fontIsMonoSpaced;
        int monoWidth;
        int monoHeight;
        int numberWidth;
        int numberHeight;
        std::vector < fontPair * > textPairs;
        std::vector <fontPair * > characterPairs;
        TTF_Font * heldSDLFont;
    public:
        std::string fontNickName;
        int customFontId;
        GPE_Font(std::string fFileLocation, int fSize, bool isMonospaced = false, std::string fNickName = "");
        ~GPE_Font();
        void clear_cache();
        void get_metrics(std::string textToRender, int * wVal, int *hVal);
        void get_numbered_metrics(std::string textToRender, int * wVal, int *hVal);
        int get_mono_height();
        int get_mono_width();
        std::string get_family_name();
        TTF_Font * get_sdl_font();
        int get_cache_count();
        fontPair * find_character_texture( std::string numbId);
        fontPair * find_texture( std::string textToRender);
        void render_bitmapped_text( int xPos, int yPos, std::string numberToRender, GPE_Color * textColor, int hAlign=-1,int vAlign=-1,int renderAlpha = 255);
        void render_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign=-1,int vAlign=-1,int renderAlpha = 255);
        void render_text_ext( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign=-1,int vAlign=-1, int rendWid = -1, int rendHeight = -1,int renderAlpha = 255);
        void render_text_rotated( int xPos, int yPos, std::string textureText, GPE_Color * textColor, double textAngle = 0,int renderAlpha = 255);
        void render_text_boxed( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor,int hAlign=-1,int vAlign=-1,int renderAlpha = 255);
};

extern GPE_Font * LAST_USED_FONT;

class GPE_PathPoint
{
    public:

        float xPos;
        float yPos;
        float pointSpeed;
        GPE_PathPoint(int pointX, int pointY, float speed = 1);
        ~GPE_PathPoint();
};

class GPE_GamePath
{
    private:
        int pathId;
        bool isClosedPath;
        std::vector< GPE_PathPoint * > pathPoints;
        std::vector< GPE_PathPoint * > pathOriginalPoints;
        int pathLength;
        int originalPathLength;
    public:
        GPE_GamePath(int pId, bool pIsClosed);
        ~GPE_GamePath();
};

//The Master class that keeps the world going
class GPE_Runtime
{
    private:
        int font_total_count;
        std::vector < GPE_Font * > loadedGPEFonts;
        GPE_Rect * HUD_RECT;
		int sceneWidth;
		int sceneHeight;
		std::string stateName;
		//Path related functions/objects/variables
        std::vector < GPE_GamePath * > gamePaths;

		//Scene Variables / Data
        std::vector < GPE_GameScene * > gpeScenes;
		GPE_GameScene * currentScene;
		int gpeScenePos;
		int currentSceneId;
		int pastSceneId;
		std::string pastSceneName;
		bool sceneWasReset;
		int sceneToEnter;

		//Used for handling mouse inputs
		float MOUSE_CAM_X[GPE_MAX_CAMERAS];
		float MOUSE_CAM_Y[GPE_MAX_CAMERAS];
		bool MOUSE_IN_CAM[GPE_MAX_CAMERAS];
		//currentScene.load_game_scene(GPE_SETTINGS_START_SCENE,'text');

		//Used to keep track of game objects
		std::vector < GPE_GameObject* > GAME_OBJECTS; //listed version of game objects based on object types
		std::vector < GPE_GameObject* > GAME_OBJECTS_TO_BE_DESTROYED;
		std::vector < GPE_GameObject* > GAME_OBJECTS_TO_DELETE_TYPES;
		std::vector < GPE_GameObject* > GAME_OBJECTS_CONTINUOUS;
		bool needToDeleteObjects;
		//GAME_OBJECTS_FAMILIES = [];
		int OBJECT_COUNT;
		bool MAIN_GAME_CODE_IS_READ;
		//Collision related variables
		std::vector < GPE_SuperSpatialPartition* > collisionSpacesInView;
		bool spatialGridIsCreated;
		int spatialMapWidthSize;
		int spatialMapHeightSize;
		bool superDebugCollisions;

		std::vector < bool >objectCollisionIsChecked;
		/*collisionPairA = [];
		collisionPairB = [];  */
		int collisionPairsTotal;

		int parsed_objects;
		int spatialGridWidthAmount;
		int spatialGridHeightAmount;
		int objectCollisionHashesChecked;
		int spatialCollisionChecks;

		//Debugging variables
		int collisionsCheckedCount;
		int colisionObjectLoops;
		//
    public:
        int currentCameraInView;
        GPE_Runtime();
        ~GPE_Runtime();

        //Font Functions
        GPE_Font * open_font(std::string fontLocation, int fontSize = 12, bool isMonoSpaced = false, std::string fNickName = "" );
        void close_font(GPE_Font * fontIn);
        void clear_font_cache();
        int get_font_cache_size(bool recordCache = false);
        //Rendering Functions
        void render_fps(GPE_Font * fontToUse);
        bool render_circle_filled_rgba( Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void render_gradient_horizontal(  GPE_Rect * rendRect = NULL, GPE_Color * rendColor = NULL);
        void render_gradient_vertical(  GPE_Rect * rendRect = NULL, GPE_Color * rendColor = NULL);
        void render_line(  int x1, int y1, int x2, int y2,  GPE_Color *rendColor, int alphaChannel = 255);
        void render_line_width(  int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel = 255);

        void render_horizontal_line(int y, int x1, int x2);
        void render_horizontal_line_color( int y, int x1, int x2, GPE_Color *rendColor, int alphaChannel = 255);
        void render_rect(  GPE_Rect * rendRect,GPE_Color * rendColor,bool outline = false, int alphaChannel = 255);
        void render_rectangle( int x1, int y1, int x2, int y2,  GPE_Color *rendColor,bool outline = false, int alphaChannel = 255);

        void render_rounded_rectangle(int x1, int y1, int x2, int y2, int rad, GPE_Color * rendColor = NULL);
        void render_rounded_rectangle_rgba( int x1, int y1,int x2, int y2, int rad, GPE_Color * rendColor = NULL, Uint8 a = 255);

        void render_vertical_line( int x, int y1, int y2);
        void render_vertical_line_color( int x, int y1, int y2, GPE_Color *rendColor, int alphaChannel = 255);

        //Sprite Rendering Functions
        void render_animation(int spriteId, int subImageToDraw, int xPos, int yPos, GPE_Texture *destination= NULL, GPE_Rect * cam= NULL);
        void render_animation_resized(int spriteId, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Texture *destination= NULL, GPE_Rect * cam = NULL);
        void render_animation_ext(int spriteId, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,GPE_Color * renderColor,GPE_Texture *destination= NULL, GPE_Rect * cam= NULL);
        void render_animation_rotated(int spriteId, int subImageToDraw, int xPos, int yPos, double newAngle, int newWidth, int newHeight, GPE_Texture *destination= NULL, GPE_Rect * cam= NULL);
        int sprite_get_number(GPE_Animation* spriteIn);

        //Texture Rendering Functions
        void render_texture( int textureId, int x, int y,GPE_Rect* clip = NULL, GPE_Texture* destination= NULL, int hAlign = -1, int vAlign = -1 );
        void render_texture_resized( int textureId, int x, int y,int newWidth, int newHeight,GPE_Rect* clip = NULL, GPE_Texture* destination= NULL, int hAlign = -1, int vAlign = -1, GPE_Color * renderColor=NULL );
        void render_texture_rotated( int textureId, int x, int y,double newAngle,int newWidth, int newHeight,GPE_Rect* clip = NULL, GPE_Texture* destination= NULL );
        void set_render_alpha( int newAlpha = 255);
};

extern GPE_Runtime * gpe;
extern GPE_Runtime * GPE;

//Auto-translates
bool render_bitmap_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Font * textFont=NULL, int hAlign=-1,int vAlign=-1, int renderAlpha = 255);
bool render_new_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Font * textFont=NULL, int hAlign=-1,int vAlign=-1, int renderAlpha = 255);
bool render_new_text_ext( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont=NULL, int hAlign=-1,int vAlign=-1, int rendWid = -1, int rendHeight = -1, int renderAlpha = 255);
bool render_new_boxed( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor, GPE_Font * textFont=NULL, int hAlign=-1,int vAlign=-1, int renderAlpha = 255);
bool render_new_text_rotated( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, double textAngle = 0, int renderAlpha  = 255);
//Doesnt translate ( useful for textfields and code/text editors)
bool render_only_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Font * textFont=NULL, int hAlign=-1,int vAlign=-1, int renderAlpha = 255);

bool GPE_Init();
bool GPE_Quit();
#endif
