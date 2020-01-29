/*
GPE_Engine.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the ԓoftwareԩ, to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ԁS ISԬ WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "GPE_Engine.h"
#include "GPE_Math.h"
#include "GPE_Artist_Base.h"

bool GPE_IS_LOADING = false;
int GPE_MAX_OBJECT_TYPES = 0;


GPE_Runtime * gpe = NULL;

GPE_Runtime::GPE_Runtime()
{
    loopStarted = false;

    gameTimer = new GPE_Timer();
    gameTimer->set_fps( 30 );
    currentState = NULL;
    nextStateName = "";
    currentStateName = "";

    for(int iV=0; iV < GPE_MAX_CAMERAS; iV++)
    {
        gpeCameras[iV] = new GPE_SceneCamera(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,iV);
    }

    SDL_Cursor * cCursor = NULL;

    for( int iC = 0; iC < GPE_CURSOR_MAX; iC ++ )
    {
        defaultCursorNames[ iC ] = "";
    }

    defaultCursorNames[GPE_CURSOR_ARROW] = "arrow";
    defaultCursorNames[GPE_CURSOR_IBEAM] = "ibeam";
    defaultCursorNames[GPE_CURSOR_WAIT] = "wait";
    defaultCursorNames[GPE_CURSOR_CROSSHAIR] = "crosshair";
    defaultCursorNames[GPE_CURSOR_WAITARROW] = "waitarrow";

    defaultCursorNames[GPE_CURSOR_SIZENWSE] = "sizenwse";
    defaultCursorNames[GPE_CURSOR_SIZENESW] = "sizenesw";
    defaultCursorNames[GPE_CURSOR_SIZEWE] = "sizewe";
    defaultCursorNames[GPE_CURSOR_SIZENS] = "sizens";
    defaultCursorNames[GPE_CURSOR_SIZEALL] = "sizeall";

    defaultCursorNames[GPE_CURSOR_NO] = "no";
    defaultCursorNames[GPE_CURSOR_HAND] = "hand";


    for( int iCursor = SDL_SYSTEM_CURSOR_ARROW; iCursor < GPE_CURSOR_MAX; iCursor++ )
    {
        cCursor = SDL_CreateSystemCursor( (SDL_SystemCursor) iCursor );
        cursorMap[ defaultCursorNames[ iCursor] ] =  cCursor ;
    }

    cursorPrevious = cursorCurrent = defaultCursorNames[GPE_CURSOR_ARROW];
    cursorSDL = cursorMap[ cursorCurrent];
}

GPE_Runtime::~GPE_Runtime()
{
    GPE_Report("Removing Game Timer...");
    if( gameTimer!=NULL )
    {
        delete gameTimer;
        gameTimer = NULL;
    }

    GPE_Report("Removing program states...");
    GPE_ProgramState *  cState = NULL;
    for( int i = (int)stateLists.size() -1; i >=0; i-- )
    {
        cState = stateLists[i];
        if( cState!=NULL )
        {
            GPE_Report("Removing State["+cState->get_state_name()+"]...");
            delete cState;
            cState = NULL;
        }
    }
    stateLists.clear();
    currentState = NULL;

    GPE_Report("Removing cameras...");
    GPE_SceneCamera * tCamera = NULL;
    for(int iV= GPE_MAX_CAMERAS-1; iV >=0; iV-- )
    {
        tCamera = gpeCameras[iV];
        if( tCamera!=NULL)
        {
            delete tCamera;
            tCamera = NULL;
        }
        gpeCameras[iV] = NULL;
    }

    GPE_Report("Removing cursors...");

    SDL_Cursor * cCursor = NULL;
    std::map<std::string, SDL_Cursor *>::iterator it;

	for ( it = cursorMap.begin(); it != cursorMap.end(); it++ )
	{
        cCursor = it->second;
        if( cCursor !=NULL )
        {
            SDL_FreeCursor( cCursor );
        }
        cCursor = NULL;
    }
    cursorMap.clear();
}


void GPE_Runtime::camera_disable ( int cameraId)
{
    if( cameraId >=0 && cameraId < GPE_MAX_CAMERAS )
    {
        gpeCameras[cameraId]->set_visible( false );
    }
}

void GPE_Runtime::camera_enable ( int cameraId )
{
    if( cameraId >=0 && cameraId < GPE_MAX_CAMERAS )
    {
        gpeCameras[cameraId]->update_view_space(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
        gpeCameras[cameraId]->update_screen_space(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
        gpeCameras[cameraId]->set_visible( true );
    }
}


void GPE_Runtime::cursor_change( std::string newCursor)
{
    cursorCurrent = newCursor;
}

void GPE_Runtime::cursor_change_system( int systemId )
{
    cursor_change( cursor_system_name( systemId ) );
}

void GPE_Runtime::cursor_clear_dynamic()
{
    SDL_Cursor * cCursor = NULL;

    std::map<std::string, SDL_Cursor *>::iterator it = cursorMap.begin();
	int jCursor = 0;
	bool isSystemCursor = false;
	while (it != cursorMap.end() )
	{
        cCursor = it->second;
        isSystemCursor = false;
        if( cCursor !=NULL )
        {
            for( jCursor = 0; jCursor < GPE_CURSOR_MAX; jCursor++)
            {
                if( it->first == defaultCursorNames[jCursor] )
                {
                    isSystemCursor = true;
                }
            }

            if( isSystemCursor == false )
            {
                SDL_FreeCursor( cCursor);
                cursorMap.erase( it );
            }
        }
    }
}

bool GPE_Runtime::cursor_contains( std::string cursorName)
{
    auto cCursor = cursorMap.find( cursorName );
    if( cCursor == cursorMap.end() )
    {
        return false;
    }
    return true;
}

bool GPE_Runtime::cursor_create_from_image(std::string fName )
{
    if( file_exists( fName) == false )
    {
        GPE_Report("Unable to load cursor from ["+ fName + "]" );
        return false;
    }

    SDL_Surface * loadedCursor = SDL_SurfaceEx::load_surface_image( fName.c_str() );
    if( loadedCursor!=NULL && loadedCursor->w!=0 )
    {
        SDL_Cursor * newCursor = SDL_CreateColorCursor( loadedCursor, 0, 0 );
        fName = get_local_from_global_file( fName );
        cursorMap[ get_file_noext( fName ) ] =  newCursor ;
        GPE_Report("Loaded cursor ["+ fName + "[" );
        return true;
    }
    GPE_Report("Unable to load cursor surface from ["+ fName + "[" );
    return false;
}

int GPE_Runtime::cursor_map_size()
{
    return (int)cursorMap.size();
}

void GPE_Runtime::cursor_logic()
{
    if( cursorCurrent!=cursorPrevious )
    {
        if( cursor_contains( cursorCurrent ) )
        {
            cursorSDL = cursorMap[cursorCurrent];
            SDL_SetCursor( cursorSDL );
        }
    }
    cursorPrevious = cursorCurrent;
}

std::string GPE_Runtime::cursor_system_name( int cId )
{
    if( cId >= 0 && cId < GPE_CURSOR_MAX)
    {
        return defaultCursorNames[ cId ];
    }
    return "";
}

void GPE_Runtime::log_error( std::string errorReported)
{
    if( runtimeLog.size() > 100)
    {
        runtimeLog.erase( runtimeLog.begin() );
    }
    runtimeLog.push_back( errorReported );
}

//gameloop
int GPE_Runtime::game_loop()
{
    int neededTime = 0;

    state_handle_changes();
    //If the gameloop starts off on exit, we return false
    if( string_lower( nextStateName )== "exit" )
    {
        return -4;
    }

    if( currentState == NULL )
    {
        return -3; //no state set on first go around so we exit
    }

    Uint32 sTicks;
    Uint32 eTicks;
    //While we didnt't request gameloop to end
    while( string_lower( nextStateName ) != "exit" )
    {
        start_loop();

        //Change state if needed
        if( state_handle_changes() )
        {
            return true;
        }

        if( currentState == NULL )
        {
            return true;
        }
        else
        {
            sTicks = SDL_GetTicks();
            currentState->process_input();
            eTicks = SDL_GetTicks();
            GPE_Log_MS_Action("currentState->process_input()",eTicks - sTicks,10 );
        }


        if( GPE_MAIN_WINDOW->is_minimized() == false )
        {
            sTicks = SDL_GetTicks();
            currentState->apply_logic();
            eTicks = SDL_GetTicks();
            GPE_Log_MS_Action("currentState->apply_logic",eTicks - sTicks, 10);

            sTicks = SDL_GetTicks();

            if( gpeph!=NULL )
            {
                gpeph->clear_render_packages();
            }

            if( currentState!=NULL )
            {
                currentState->render();
            }

            eTicks = SDL_GetTicks();
            GPE_Log_MS_Action("currentState->render",eTicks - sTicks,10 );

            //Updates the screen, calculates/caps fps and more.
            end_loop();
        }
        else
        {
            if(input!=NULL)
            {
                input->reset_temp_input();
            }
            if( gameTimer!=NULL )
            {
                gameTimer->reset_timer();
            }
        }
    }
    return true; //Otherwise return true that the gameloop ended properly
}

float GPE_Runtime::get_delta_performance()
{
    return gameTimer->get_delta_performance();
}


float GPE_Runtime::get_delta_time()
{
    return gameTimer->get_delta_ticks();
}

float GPE_Runtime::get_fps()
{
    return gameTimer->get_fps();
}

float GPE_Runtime::get_needed_ticks()
{
    return gameTimer->get_needed_ticks();
}

float GPE_Runtime::get_ticks()
{
    return gameTimer->get_ticks();
}


void GPE_Runtime::start_loop()
{
    gameTimer->start_timer();
    //gets user input
    Uint32 sTicks = SDL_GetTicks();
    input->handle_input(true, false );
    if( GPE_MAIN_WINDOW->is_minimized() == false )
    {
        cursor_logic();
        SCREEN_WIDTH = GPE_MAIN_WINDOW->get_window_width();
        SCREEN_HEIGHT= GPE_MAIN_WINDOW->get_window_height();
    }
    loopStarted  = true;
    Uint32 eTicks = SDL_GetTicks();
    GPE_Log_MS_Action("GPE_Runtime::start_loop()",eTicks - sTicks,10 );
}

void GPE_Runtime::end_loop(  )
{
    loopStarted = false;
    if( GPE_MAIN_WINDOW->is_minimized() )
    {
        gameTimer->reset_timer();
        return;
    }

    if( gpeph!=NULL )
    {
        gpeph->update_render_packages();

        gameTimer->cap_fps( gpeph->is_all_windows_minimized()  );
        gameTimer->calculate_avg_fps( gpeph->is_all_windows_minimized()  );
    }
}

void GPE_Runtime::finish_loop()
{
    end_loop();
}


void GPE_Runtime::reset_loop()
{
    start_loop();
}

bool GPE_Runtime::state_add( GPE_ProgramState * newState )
{
    if( newState!=NULL )
    {
        if( state_find( newState->get_state_name() ) == NULL )
        {
            stateLists.push_back( newState );
            return true;
        }
    }
    return false;
}

GPE_ProgramState * GPE_Runtime::state_find( std::string stateName )
{
    GPE_ProgramState *  cState = NULL;
    for( int i = (int)stateLists.size() -1; i >=0; i-- )
    {
        cState = stateLists[i];
        if( cState!=NULL && cState->get_state_name() == stateName )
        {
            return cState;
        }
    }
    return NULL;
}

bool GPE_Runtime::state_handle_changes()
{
    //If the state needs to be changed
    if( (int)nextStateName.size() > 0 && currentStateName != nextStateName )
    {
        if( currentState!=NULL )
        {
            currentState->end_state();
        }
        currentState = NULL;

        GPE_ProgramState * cState  = state_find( nextStateName);
        if( cState!=NULL )
        {
            currentState = cState;
            currentState->start_state();
            currentStateName = nextStateName;
        }

        if( string_lower( nextStateName ) == "exit" )
        {
            return true;
        }
        else
        {
            nextStateName = "";
        }
    }
    return false;
}

void GPE_Runtime::state_set( std::string newState )
{
    nextStateName = newState;
}

void GPE_Runtime::set_fps( int newFPS)
{
    gameTimer->set_fps( newFPS );
}


void GPE_Report(std::string stringIn )
{
    //std::cout << stringIn+" \n";
    if( gpe!=NULL )
    {
        gpe->log_error( stringIn );
    }

    /*
    std::string errorFileName = "gpe_error_log.txt";
    std::ofstream filestr(errorFileName.c_str(), std::ios::out | std::ios::app);

    if( filestr.is_open() )
    {
        filestr << stringIn+" \n";
        filestr.close();
    }
    */
}

void GPE_Log_MS_Action(std::string stringIn, Uint32 msIn, Uint32 msRequired )
{
    if( msIn > msRequired )
    {
        stringIn += " took " + int_to_string( msIn ) + "ms";
        //std::cout << stringIn+" \n";
        if( gpe!=NULL )
        {
            gpe->log_error( stringIn );
        }

        /*
        std::string errorFileName = "gpe_error_log.txt";
        std::ofstream filestr(errorFileName.c_str(), std::ios::out | std::ios::app);

        if( filestr.is_open() )
        {
            filestr << stringIn+" \n";
            filestr.close();
        }
        */
    }
}

//GPE Render calls [ End ]
