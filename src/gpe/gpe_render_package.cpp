/*
render_package.h
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

#include "gpe_render_package.h"
namespace gpe
{
    render_package * gpepm = NULL;

    render_package::render_package( int pId, std::string pName )
    {
        packageId = pId;
        packageName = pName;
        packageTexture = NULL;
        packageRenderer = NULL;
        packageWindow = NULL;
    }

    render_package::~render_package()
    {
        if( packageTexture !=NULL)
        {
            delete packageTexture;
            packageTexture = NULL;
        }

        if( packageRenderer !=NULL)
        {
            delete packageRenderer;
            packageRenderer = NULL;
        }

        if( packageWindow !=NULL)
        {
            delete packageWindow;
            packageWindow = NULL;
        }
    }

    texture_base * render_package::create_texture()
    {
        if( packageTexture!=NULL)
        {
            return packageTexture->create_new();
        }
        return NULL;
    }

    int render_package::get_package_id()
    {
        return packageId;
    }

    std::string render_package::get_package_name()
    {
        return packageName;
    }
}
