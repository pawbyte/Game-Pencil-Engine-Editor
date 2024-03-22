/*
render_package.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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
    render_package * gpepm = nullptr;

    render_package::render_package( int pId, std::string pName )
    {
        packageId = pId;
        packageName = pName;
        packageTexture = nullptr;
        packageRenderer = nullptr;
        packageWindow = nullptr;
    }

    render_package::~render_package()
    {
        if( packageTexture !=nullptr)
        {
            delete packageTexture;
            packageTexture = nullptr;
        }

        if( packageRenderer !=nullptr)
        {
            delete packageRenderer;
            packageRenderer = nullptr;
        }

        if( packageWindow !=nullptr)
        {
            delete packageWindow;
            packageWindow = nullptr;
        }
    }

    animation2d * render_package::create_animation2d( const std::string& anim_name, const std::string& anim_filename , bool transparent_image)
    {
        if( packageTexture!=nullptr)
        {
            return packageAnimation2d->create_new( anim_name, anim_filename, transparent_image );
        }
        return nullptr;
    }


    texture_base * render_package::create_texture()
    {
        if( packageTexture!=nullptr)
        {
            return packageTexture->create_new();
        }
        return nullptr;
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
