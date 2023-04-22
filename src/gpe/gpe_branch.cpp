/*
gpe_branch.cpp
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

#include "gpe_branch.h"

namespace gpe
{
    branch_factory * branch_factory_master = nullptr;

    branch::branch()
    {
        default_branch_constructor();
    }

    branch::branch( int branch_layer_id, float x_pos_in,  float y_pos_in, float z_pos_in )
    {
        default_branch_constructor();
        current_layer = branch_layer_id;
        xpos = x_pos_in;
        ypos = y_pos_in;
        zpos = z_pos_in;
    }

    branch::~branch()
    {

    }


    void branch::add_branch( branch * new_branch )
    {
        if( new_branch == nullptr )
        {
            return;
        }
    }

    bool branch::being_deleted()
    {
        return branch_being_removed;
    }

    branch * branch::branch_create( )
    {
        return nullptr;
    }

    branch * branch::branch_create_and_init( int branch_layer_id, float x_pos_in,  float y_pos_in, float z_pos_in )
    {
        return nullptr;
    }

    void branch::branch_destroy()
    {

    }

    void branch::branch_init( int branch_layer_id, float x_pos_in,  float y_pos_in, float z_pos_in )
    {

    }

    bool branch::branch_inited()
    {
        return branch_was_inited;
    }

    void branch::default_branch_constructor()
    {
        branch_visible = true;
        frames_til_deletion = -1;
        uses_render_start = uses_render_hud = uses_render_end = false;
        branch_parent = nullptr;
        branch_tree = nullptr;
        name = "";
        branch_tag = "";
        xpos = 0;
        ypos = 0;
        xscale = 1;
        yscale = 1;
        angle = 0;
        xpivot = 0;
        ypivot = 0;
        width = 32;
        height = 32;
        current_layer = 0;
        branch_visible = true;
        branch_parent = nullptr;

        branch_was_inited = false;
        branch_being_removed = false;
        branch_is_permanent = false;
        branch_tree = nullptr;

        scene_id = -1;
        scene_orign_id = -1;
        scene_unique_id = -1;
    }

    branch * branch::find_branch_by_name( std::string branch_name , bool nest_down )
    {
        return nullptr;
    }

    branch * branch::find_typed_branch_by_name( int branch_type, std::string branch_name , bool nest_down )
    {
        return nullptr;
    }

    float branch::get_angle()
    {
        return angle;
    }

    branch * branch::get_parent()
    {
        return branch_parent;
    }

    branch * branch::get_parent_tree()
    {
        return branch_tree;
    }

    float branch::getx()
    {
        return xpos;
    }

    float branch::gety()
    {
        return ypos;
    }

    uint8_t branch::get_layer_id()
    {
        return current_layer;
    }

    float branch::get_width()
    {
        return width;
    }

    float branch::get_height()
    {
        return height;
    }


    branch_type branch::get_type()
    {
        return branch_type_id;
    }

    std::string branch::get_type_string()
    {
        return branch_tag;
    }

    float branch::get_xpivot()
    {
        return xpivot;
    }

    float branch::get_xscale()
    {
        return yscale;
    }

    float branch::get_ypivot()
    {
        return ypivot;
    }

    float branch::get_yscale()
    {
        return yscale;
    }

    bool branch::is_visible()
    {
        return branch_visible;
    }

    bool branch::remove_branch_by_name( std::string branch_name , bool nest_down )
    {
        return false; //WIPNOTFUNCTIONALYET
    }

    bool branch::remove_typed_branch_by_name( int branch_type_id, std::string branch_name , bool nest_down )
    {
        return false; //WIPNOTFUNCTIONALYET
    }

    void branch::render()
    {
        branch * current_branch = nullptr;
        for( int branch_itr = 0; branch_itr < (int)sub_elements.size(); branch_itr++ )
        {
            current_branch = sub_elements[branch_itr];
            if( current_branch != nullptr )
            {
                current_branch->render();
            }
        }
    }

    void branch::render_end()
    {

    }

    void branch::render_start()
    {

    }

    void branch::render_hud()
    {

    }

    void branch::render_hud_end()
    {

    }

    void branch::render_hud_start()
    {

    }

    void branch::reset_branch()
    {
        branch * current_branch = nullptr;
        for( int branch_itr = 0; branch_itr < (int)sub_elements.size(); branch_itr++ )
        {
            current_branch = sub_elements[ branch_itr ];
            if( current_branch != nullptr )
            {
                current_branch->reset_branch();
            }
        }
    }

    bool branch::self_destruct()
    {
        return true;
    }

    void branch::set_angle(float new_angle )
    {
        angle = new_angle;
    }

    void branch::set_coords(float x_new, float y_new )
    {
        xpos = x_new;
        ypos = y_new;
    }

    void branch::set_frames_before_deleted( int frames_remaining )
    {
        if( frames_remaining > 0 )
        {
            frames_til_deletion = true;
            branch_being_removed = true;
        }
    }

    void branch::set_height(int new_height )
    {
        height = new_height;
    }

    void branch::set_tag( std::string tag_new)
    {
        branch_tag = tag_new;
    }

    void branch::set_width(int new_width )
    {
        width = new_width;
    }

    void branch::set_size(int new_width, int new_height )
    {
        width = new_width;
        height = new_height;
    }

    void branch::set_pivots(int new_x_pivot, int new_y_pivot )
    {
        xpivot = new_x_pivot;
        ypivot = new_y_pivot;
    }

    void branch::set_scale( float new_scale )
    {
        xscale = new_scale;
        yscale = new_scale;
    }

    void branch::setx(float x_new )
    {
        xpos = x_new;
    }

    void branch::set_xscale( float new_scale )
    {
        xscale = new_scale;
    }

    void branch::sety(float y_new )
    {
        ypos = y_new;
    }

    void branch::set_yscale( float new_scale )
    {
        yscale = new_scale;
    }

    void branch::update(float delta_time )
    {
        branch * current_branch = nullptr;
        for( int branch_itr = 0; branch_itr < (int)sub_elements.size(); branch_itr++ )
        {
            current_branch = sub_elements[ branch_itr ];
            if( current_branch != nullptr)
            {
                current_branch->update( delta_time );
            }
        }
    }
}
