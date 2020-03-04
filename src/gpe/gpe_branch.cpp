/*
gpe_branch.cpp
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

#include "gpe_branch.h"

namespace gpe
{
    branch::branch()
    {

    }

    branch::~branch()
    {

    }

    void branch::add_branch( branch * new_branch )
    {
        if( new_branch == NULL )
        {
            return;
        }
    }

    branch * branch::find_branch_by_name( std::string branchName , bool nestDown )
    {
        return NULL;
    }

    branch * branch::find_typed_branch_by_name( int bType, std::string branchName , bool nestDown )
    {
        return NULL;
    }

    float branch::get_angle()
    {
        return angle;
    }

    branch * branch::get_parent()
    {
        return parentBranch;
    }

    branch * branch::get_parent_tree()
    {
        return treeBranch;
    }

    float branch::getx()
    {
        return xpos;
    }

    float branch::gety()
    {
        return ypos;
    }

    int branch::get_layer_id()
    {
        return currentLayer;
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
        return branchType;
    }

    std::string branch::get_type_string()
    {

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

    bool branch::remove_branch_by_name( std::string branchName , bool nestDown )
    {

    }

    bool branch::remove_typed_branch_by_name( int bType, std::string branchName , bool nestDown )
    {

    }

    void branch::render()
    {
        branch * current_branch = NULL;
        for( int branch_itr = 0; branch_itr < (int)sub_elements.size(); branch_itr++ )
        {
            current_branch = sub_elements[branch_itr];
            if( current_branch != NULL )
            {
                current_branch->render();
            }
        }
    }

    void branch::reset_branch()
    {
        branch * current_branch = NULL;
        for( int branch_itr = 0; branch_itr < (int)sub_elements.size(); branch_itr++ )
        {
            current_branch = sub_elements[ branch_itr ];
            if( current_branch != NULL )
            {
                current_branch->reset_branch();
            }
        }
    }

    void branch::set_angle(float newAngle )
    {
        angle = newAngle;
    }

    void branch::set_coords(float newX, float newY )
    {
        xpos = newX;
        ypos = newY;
    }

    void branch::set_height(int newHeight )
    {
        height = newHeight;
    }

    void branch::set_tag( std::string newTag)
    {
        branchTag = newTag;
    }

    void branch::set_width(int newWidth )
    {
        width = newWidth;
    }

    void branch::set_size(int newWidth, int newHeight )
    {
        width = newWidth;
        height = newHeight;
    }

    void branch::set_pivots(int newXPivot, int newYPivot )
    {
        xpivot = newXPivot;
        ypivot = newYPivot;
    }

    void branch::set_scale( float newScale )
    {
        xscale = newScale;
        yscale = newScale;
    }

    void branch::setx(float newX )
    {
        xpos = newX;
    }

    void branch::set_xscale( float newScale )
    {
        xscale = newScale;
    }

    void branch::sety(float newY )
    {
        ypos = newY;
    }

    void branch::set_yscale( float newScale )
    {
        yscale = newScale;
    }

    void branch::update()
    {
        branch * current_branch = NULL;
        for( int branch_itr = 0; branch_itr < (int)sub_elements.size(); branch_itr++ )
        {
            current_branch = sub_elements[ branch_itr ];
            if( current_branch != NULL)
            {
                current_branch->update();
            }
        }
    }
}
