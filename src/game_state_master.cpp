/*
game_state_master.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#include "game_state_master.h"


game_master::game_master( std::string s_name )
{
    if( (int)s_name.size() > 0 )
    {
        state_name = s_name;
    }
    else
    {
        state_name = "gpe_game_master";
    }
    for( int imCam = 0; imCam < gpe::max_cameras_allowed; imCam++)
    {
        mouse_cam_x[imCam] = 0;
        mouse_cam_y[imCam] = 0;
        mouse_in_cam[imCam] = true;
    }

    scene_current = NULL;
    scene_currentId = -1;

    if(  gpe::main_spatial_grid != NULL)
    {
        delete gpe::main_spatial_grid;
        gpe::main_spatial_grid = NULL;
    }
    spatialPartitonController = new gpe::spatial_partition_controller("main_sp_grid");
}


game_master::~game_master()
{
    clean_up();
}

//Main loop functions

void game_master::apply_game_objects_logic()
{
    if( scene_current!=NULL )
    {
        scene_current->apply_logic();
    }
}

void game_master::apply_game_objects_prelogic()
{

}

void game_master::apply_logic()
{
    if( gpe::window_controller_main->window_closed || gpe::input->exit_requested  )
    {
        gpe::game_runtime->state_set( "exit" );
    }

    process_scene_movement();
    if( scene_current == NULL )
    {
        return;
    }

    if( scene_current->scene_is_ready()!=true )
    {
        scene_current->scene_init();
    }
    scene_current->process_scene();
    if( scene_current->scene_is_ready() == false )
    {
        return;
    }
    gpe::game_object * foundRegObj = NULL;
    gpe::game_object * foundTypedObj = NULL;
    int iObjectType = -1;
    int jObject = -1;
    gpe::game_object_list *  foundObjTypeHolder = NULL;
    int jj = 0;
    int kItr = 0;
    int fcSpace = 0;

    //deletes objects set for deletion
    remove_deleted_objects();

    //apply game object prelogics
    apply_game_objects_prelogic();
    //apply_spatial_game_objects_prelogics(this.collision_spaces_in_view);
    init_collision_handler();

    update_spaces_in_view();

    //process collisions
    parse_spatial_collisions();

    scene_current->apply_logic();
    if( gpe::window_controller_main->window_closed || gpe::input->exit_requested  )
    {
        gpe::game_runtime->state_set( "exit" );
    }

}

void game_master::init_collision_handler()
{
    if( scene_current!=NULL )
    {
        scene_current->init_collision_handler();
    }
}

void game_master::clean_up()
{
    scene_current = NULL;
    gpe::game_scene * cScene = NULL;
    gpe::error_log->report("Attempting to remove ["+ stg_ex::int_to_string( (int)gpeScenes.size() )+"] scenes");
    for( int iScene = (int)gpeScenes.size()-1; iScene >=0; iScene--)
    {
        cScene = gpeScenes[iScene];
        if( cScene!=NULL)
        {
            delete cScene;
            cScene = NULL;
        }
    }
    gpeScenes.clear();

    gpe::game_path2d * cPath = NULL;
    gpe::error_log->report("Attempting to remove ["+ stg_ex::int_to_string( (int)gamePaths.size() )+"] paths");

    for( int iPath = (int)gamePaths.size()-1; iPath >=0; iPath--)
    {
        cPath = gamePaths[iPath];
        if( cPath!=NULL)
        {
            delete cPath;
            cPath = NULL;
        }
    }
    gamePaths.clear();

    game_objects.clear();
    game_objects_delete_list.clear();
    game_objects_delete_types.clear();
    game_objects_continuous.clear();
    collision_spaces_in_view.clear();
    object_collision_is_checked.clear();

    gpe::error_log->report("State removed..");
}

void game_master::end_state()
{

}

void game_master::parse_spatial_collisions()
{

}

void game_master::process_input()
{
    if( gpe::window_controller_main->window_closed || gpe::input->exit_requested  )
    {
        gpe::game_runtime->state_set( "exit" );
    }
}

void game_master::process_scene_movement()
{
    if( scene_currentId!=sceneToEnter && sceneToEnter>=0 && sceneToEnter < (int)gpeScenes.size() )
    {
        int nextSceneToGo = sceneToEnter;
        int previousSceneIn = scene_currentId;
        int jj;
        int iHolderType;
        gpe::game_object_list * foundObjTypeHolder;
        gpe::game_object * foundRegObj;
        game_objects_continuous.clear();
        for( iHolderType = (int)game_objects.size()-1; iHolderType>=0; iHolderType--)
        {
            foundObjTypeHolder = game_objects.at( iHolderType );
            for( jj=(int)foundObjTypeHolder->internal_list.size()-1; jj>=0; jj--)
            {
                foundRegObj = foundObjTypeHolder->internal_list[jj];
                if( foundRegObj->gpeIsBeingDestroyed==false )
                {
                    foundRegObj->scene_end();
                }
                if( foundRegObj->isContinuousObject  )
                {
                    game_objects_continuous.push_back( foundRegObj );
                }
            }
        }
        //if( this.scene_current.sceneIsContinuous ==false )
        {
            //Restart Object and scene related things
            scene_restart_object_info();

            scene_current->scene_restart();
        }
        scene_current = gpeScenes[ nextSceneToGo ];
        sceneToEnter = nextSceneToGo;
        scene_currentId = nextSceneToGo;
        scene_current->update_tilemap();
    }
}

void game_master::render()
{
    gpe::renderer_main->reset_viewpoint();
    gpe::gcanvas->render_rectangle( 0, 0, gpe::screen_width, gpe::screen_height, gpe::c_white, false, 255 );
    if( scene_current!=NULL )
    {
        scene_current->render_scene();
        //gpe::gfs->render_text(32,32,"Current Scene = ["+scene_current->get_scene_name()+"]", gpe::c_white, gpe::font_default,  gpe::fa_left, gpe::fa_top );

    }
    else
    {
        //gpe::gfs->render_text(32,32,"Error: Current Scene = NULL", gpe::c_red, gpe::font_default,  gpe::fa_left, gpe::fa_top );
    }
    //gpe::gfs->render_fps( gpe::font_default, gpe::c_green, gpe::screen_logical_width -32, 32, gpe::fa_right, gpe::fa_top );
    gpe::gfs->render_text( 32,  32,"Screen Size = ("+ stg_ex::int_to_string( gpe::screen_width ) +" , "+stg_ex::int_to_string( gpe::screen_height ) + ")", gpe::c_black, gpe::font_default, gpe::fa_left, gpe::fa_top, 255 );
    gpe::gfs->render_text( 32,  gpe::screen_height / 2,"Mouse Coords = ("+ stg_ex::int_to_string( gpe::input->mouse_position_x ) +" , "+stg_ex::int_to_string( gpe::input->mouse_position_y ) + ")", gpe::c_black, gpe::font_default, gpe::fa_left, gpe::fa_middle, 255 );
    gpe::gfs->render_text( 32, gpe::screen_height- 32, "Window Size = ("+ stg_ex::int_to_string( gpe::window_controller_main->get_window_width() ) +" , "+stg_ex::int_to_string( gpe::window_controller_main->get_window_height() ) + ")", gpe::c_black, gpe::font_default, gpe::fa_left, gpe::fa_bottom, 255 );
}

int game_master::remove_deleted_objects()
{
    int objectsDeleted = 0;
    int jObject = 0;
    int kItr = 0;
    gpe::game_object * foundRegObj = NULL;
    gpe::game_object_list * foundObjTypeHolder = NULL;
    for( int iObjectType = (int)game_objects.size()-1; iObjectType>=0; iObjectType--)
    {
        foundObjTypeHolder = game_objects[iObjectType];
        if( foundObjTypeHolder!=NULL )
        {
            for( jObject = (int)foundObjTypeHolder->internal_list.size() -1; jObject >=0; iObjectType-- )
            {
                foundRegObj = foundObjTypeHolder->internal_list[jObject];
                if( foundRegObj->gpeIsBeingDestroyed == true )
                {
                    foundObjTypeHolder->internal_list.erase( foundObjTypeHolder->internal_list.begin() + jObject );
                    /*
                    for( kItr = foundRegObj.SPATIAL_PARTITION_LIST.length -1; kItr>=0; kItr--)
                    {
                        fcSpace = foundRegObj.SPATIAL_PARTITION_LIST[kItr];
                        if( fcSpace < this.COLLISION_AREA_SPACES.length && fcSpace >=0 )
                        {
                            spaceToDeleteObjectFrom = this.COLLISION_AREA_SPACES[fcSpace];
                            spaceToDeleteObjectFrom.remove_held_object(foundRegObj);
                        }
                        else
                        {
                            this.COLLISION_AREA_OUTOFLEVEL.remove_held_object(foundRegObj);
                        }
                    }
                    */
                    objectsDeleted++;
                }
            }
        }
    }
    return objectsDeleted;
}

void game_master::scene_restart_object_info()
{

}

void game_master::start_state()
{
    gpe::cursor_main_controller->cursor_change( gpe::cursor_main_controller->cursor_system_name( gpe::cursor_default_type::arrow ) );
}

void game_master::update_spaces_in_view()
{
    collision_spaces_in_view.clear();
    int cSceneViewFound = 0;
    int icameraX = 0;
    int jcameraY = 0;
    int viewPartitionSpace = 0;
    int currentSpace = gpe::is_null;
    if( scene_current->scene_is_ready() )
    {
        for( int iV=0; iV < gpe::max_cameras_allowed; iV++)
        {
            /*
            cSceneViewFound = scene_current->sceneView[iV];
            if( cSceneViewFound->is_visible)
            {
                for( icameraX = cSceneViewFound.camera_rect.x_pos; icameraX <= cSceneViewFound.camera_rect.x2Pos+spatial_grid_width_size;icameraX+=spatial_grid_width_size)
                {
                    for(  jcameraY = cSceneViewFound.camera_rect.y_pos; jcameraY <= cSceneViewFound.camera_rect.y2Pos+spatial_grid_height_size;jcameraY+=spatial_grid_height_size)
                    {
                        viewPartitionSpace = (( icameraX / spatial_grid_width_size)|0) * spatial_grid_y_amount;
                        viewPartitionSpace+= ( jcameraY / spatial_grid_height_size)|0;
                        if( viewPartitionSpace < COLLISION_AREA_SPACES.length )
                        {
                            collision_spaces_in_view.push_back(viewPartitionSpace);
                        }
                    }
                }
            }
            */
        }
    }
}

