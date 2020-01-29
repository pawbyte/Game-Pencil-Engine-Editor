/*
GPE_Engine.h
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

#ifndef GPE_ENGINE_H
#define GPE_ENGINE_H

#include "GPE_CIncludes.h"
#include "GPE_Shared_Resources.h"
#include "SDL_SurfaceEx.h"
#include "GPE_Functions.h"
#include "GPE_Globals.h"
#include "GPE_Program_State.h"
#include "GPE_Camera.h"
#include "GPE_Render_Package.h"
#include "GPE_Spatial_Partition_Controller.h"

extern bool GPE_IS_LOADING;

enum
{
   GPE_CURSOR_ARROW,     /**< Arrow */
   GPE_CURSOR_IBEAM,     /**< I-beam */
   GPE_CURSOR_WAIT,      /**< Wait */
   GPE_CURSOR_CROSSHAIR, /**< Crosshair */
   GPE_CURSOR_WAITARROW, /**< Small wait cursor (or Wait if not available) */
   GPE_CURSOR_SIZENWSE,  /**< float arrow pointing northwest and southeast */
   GPE_CURSOR_SIZENESW,  /**< float arrow pointing northeast and southwest */
   GPE_CURSOR_SIZEWE,    /**< float arrow pointing west and east */
   GPE_CURSOR_SIZENS,    /**< float arrow pointing north and south */
   GPE_CURSOR_SIZEALL,   /**< Four pointed arrow pointing north, south, east, and west */
   GPE_CURSOR_NO,        /**< Slashed circle or crossbones */
   GPE_CURSOR_HAND,      /**< Hand */
   GPE_CURSOR_MAX
} gpe_cursors;

//The Master class that keeps the world going
class GPE_Runtime
{
private:
    GPE_Timer * gameTimer;
    bool loopStarted;
    GPE_ProgramState * currentState;
    std::vector < GPE_ProgramState * > stateLists;
    std::string currentStateName;
    std::string nextStateName;
    GPE_SceneCamera * gpeCameras[GPE_MAX_CAMERAS];

    std::string cursorPrevious;
    std::string cursorCurrent;
    SDL_Cursor * cursorSDL;
    std::string defaultCursorNames[GPE_CURSOR_MAX];
    std::map <std::string, SDL_Cursor *> cursorMap;
public:
    std::vector < std::string > runtimeLog;
    GPE_Runtime();
    ~GPE_Runtime();


    //Camera functions
    void camera_disable ( int cameraId);
    void camera_enable ( int cameraId);

    //Cursor logic
    void cursor_change( std::string newCursor );
    void cursor_change_system( int systemId );
    void cursor_clear_dynamic();
    bool cursor_contains( std::string cursorName );
    bool cursor_create_from_image(std::string fName );
    int cursor_map_size();
    std::string cursor_system_name( int cId );

    void cursor_logic();

    void log_error( std::string errorReported);

    //Loop related functions
    int game_loop();

    //Time related functions
    float get_delta_time();
    float get_delta_performance();
    float get_fps();
    float get_needed_ticks();
    float get_ticks();


    void start_loop();
    void reset_loop();

    void end_loop( );
    void finish_loop();
    void set_fps( int newFPS);

    bool state_add( GPE_ProgramState * newState );
    bool state_handle_changes();
    GPE_ProgramState * state_find(  std::string stateName );
    void state_set( std::string newState );

};

extern GPE_Runtime * gpe;


#endif //GPE_ENGINE_H
