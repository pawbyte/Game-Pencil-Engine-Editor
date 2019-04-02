/*
GPE.h
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

#ifndef GPE_MAIN
#define GPE_MAIN

#include "GPE_Engine.h"
#include "GPE_Fonts.h"
#include "GPE_Particles.h"
#include "GPE_Paths.h"
#include "GPE_Layers.h"
#include "GPE_Scene.h"
#include "GPE_File_System.h"
#include "GPE_Program_State.h"
#include "GPE_State_Manager.h"

class GPE_MainGame
{
private:

public:
    GPE_MainGame();
    ~GPE_MainGame();
};

class GPE_GameMaster: public GPE_ProgramState
{
private:
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
    double MOUSE_CAM_X[GPE_MAX_CAMERAS];
    double MOUSE_CAM_Y[GPE_MAX_CAMERAS];
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
public:
    int currentCameraInView;
    GPE_GameMaster();
    ~GPE_GameMaster();
    //Main loop functions
    void process_input();
    void apply_logic();
    void clean_up();
    void render();
};

#endif

