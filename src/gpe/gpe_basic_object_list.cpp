/*
gpe_basic_object_list.cpp
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

#include "gpe_basic_object_list.h"

namespace gpe
{
    game_object_list::game_object_list()
    {

    }

    game_object_list::~game_object_list()
    {
        internal_list.clear();
    }

    void game_object_list::add_object( game_object * object_in)
    {
        if( object_in!=NULL)
        {
            internal_list.push_back(object_in);
        }
    }

    void game_object_list::remove_held_object( game_object * object_in)
    {
        if( object_in!=NULL )
        {
            game_object * foundHeldObject = NULL;
            for( int ii = (int)internal_list.size()-1;  ii>=0; ii--)
            {
                foundHeldObject = internal_list[ii];
                if( foundHeldObject->get_id()==object_in->get_id() )
                {
                    internal_list.erase( internal_list.begin()+ii);
                }
            }
        }
    }
}
