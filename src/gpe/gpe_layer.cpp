/*
gpe_layer.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_layer.h"
#include "gpe_runtime.h"

namespace gpe
{
    scene_layer::scene_layer(  int newLayerId, float newLayerAlpha )
    {
        spatial_grid_box_size = 128;
        spatial_grid_width_size = 128;
        spatial_grid_height_size = 128;
        spatial_grid_x_amount = 2;
        spatial_grid_y_amount = 2;
        layer_id = newLayerId;
        layer_alpha = newLayerAlpha;

        name = "";
        spatial_grid_created = false;
        spatial_map_width_size = 0;
        spatial_map_height_size = 0;

        spatial_grid_x_amount = 2;
        spatial_grid_y_amount = 2;
        entity_collisions_hashes_checked = 0;
        spatial_collisions_checked_counter = 0;

        collision_pairs_total = 0;
        parsed_objects = 0;
    }

    scene_layer::~scene_layer()
    {
        clear_spaces();

        layer_entities.clear();
        independent_entities.clear();
    }

    void scene_layer::add_branch( branch * new_branch )
    {
        if( new_branch == nullptr )
        {
            return;
        }
    }

    layer_tile_map *  scene_layer::add_tilemap( std::string name, int x,  int y, int w, int h  )
    {
        return nullptr; //WIPNOTFUNCTIONALYET
    }

    void scene_layer::add_collision_pair( game_object *objPairA, game_object *objPairB)
    {

    }

    void scene_layer::init_collision_handler( int camera_scene_width, int camera_scene_height )
    {
        if(spatial_grid_created && spatial_map_width_size == camera_scene_width && spatial_map_height_size == camera_scene_height)
        {
            return;
        }

        if( camera_scene_width>0 && camera_scene_height>0 )
        {
            if( main_spatial_grid!=nullptr )
            {
                main_spatial_grid->init_system( camera_scene_width, camera_scene_height );
            }
        }

    }

    void scene_layer::begin_render ( std::vector< int >&spacesInView )
    {
        render_list_prerender_entities.clear();
        render_list_regular_entities.clear();
        render_list_front_entities.clear();
        render_list_hud_entities.clear();
        game_object *  foundGameObject = nullptr;
        int jObjectHolderParse = 0;
        spatial_partition *  currentSpace = nullptr;
        int foundCollisionSpaceId = 0;
        int jLoop = 0;
        //Adds camera independent objects of this layer into render list
        for( jLoop = (int)independent_entities.size() -1; jLoop >=0; jLoop--)
        {
            foundGameObject = independent_entities[jLoop];

            if( foundGameObject->is_init() && foundGameObject->is_visible() && !foundGameObject->gpeIsBeingDestroyed  )
            {
                //if(foundGameObject->hasPreRenderFunction)
                //    render_list_prerender_entities.push_back(foundGameObject);

                //if( foundGameObject.hasRenderFunction)
                render_list_regular_entities.push_back(foundGameObject);
                foundGameObject->gpeAddedToRenderList = true;
            }
        }
    }

    void scene_layer::clear_spaces()
    {
        if( main_spatial_grid !=nullptr )
        {
            main_spatial_grid->deactivate_layer( layer_id );
        }
    }

    int scene_layer::get_id()
    {
        return layer_id;
    }

    void scene_layer::remove_object ( game_object * objToRemove)
    {
        game_object *  tObject = nullptr;
        for( int iItr = (int)layer_entities.size()-1; iItr >=0; iItr--)
        {
            tObject = layer_entities[iItr];
            if( tObject->get_id()==objToRemove->get_id() )
            {
                layer_entities.erase( layer_entities.begin()+iItr );
            }
        }
    }

    void scene_layer::render()
    {

    }

    void scene_layer::render_objects()
    {
        game_object * foundGameObject = nullptr;
        int iLoop = 0;

        gpe::gcanvas->set_render_alpha(255);

        //renders the game objects
        int listSize = (int)layer_entities.size();
        for( iLoop = 0; iLoop < listSize; iLoop++ )
        {
            foundGameObject = layer_entities[iLoop];
            //if(  foundGameObject.renderedAboveInFrame[gTemp]==false )
            {
                foundGameObject->render();
                //foundGameObject.renderedAboveInFrame[gTemp] = true;
            }
        }
        gpe::gcanvas->set_render_alpha(255);
    }


    void scene_layer::reset_branch()
    {
        branch::reset_branch();
        /*independent_entities = [];
        layer_entities = [];
        layer_backgrounds = [];*/
        clear_spaces();
    }

    void scene_layer::scroll_backgrounds()
    {
        for (int iTemp = (int)layer_backgrounds.size()-1; iTemp >=0; iTemp--)
        {
            if( layer_backgrounds[iTemp]!= nullptr )
                layer_backgrounds[iTemp]->scroll_bg();
        }
    }

    //Layer New Background functions added as of Version 1.12 [ BEGIN ]
    void scene_layer::replace_all_backgrounds ( int needleBgId, int newBgId )
    {
        for( int i = (int)layer_backgrounds.size()-1; i >=0; i-- )
        {
            if( layer_backgrounds[i]->get_background_id() == needleBgId )
            {
                layer_backgrounds[i]->set_background( newBgId );
            }
        }
    }

    void scene_layer::set_layer_background ( int layerBeingChanged, int bg_id, int newBgId )
    {
        if( layerBeingChanged >=0 && layerBeingChanged < (int)layer_backgrounds.size() )
        {
            layer_backgrounds[layerBeingChanged]->set_background( newBgId );
        }
    }

    void scene_layer::update()
    {

    }

    void scene_layer::set_default_grid_size()
    {
        spatial_grid_box_size = 256;
        spatial_grid_width_size = 256;
        spatial_grid_height_size = 256;
    }
}
