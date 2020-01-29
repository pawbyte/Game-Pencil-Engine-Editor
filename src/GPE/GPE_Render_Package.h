/*
GPE_Render_Package.h
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

#ifndef RENDER_PACKAGE_H
#define RENDER_PACKAGE_H

#include "GPE_CIncludes.h"
#include "GPE_Renderer_Base.h"
#include "GPE_Texture_Base.h"
#include "GPE_Window.h"

class GPE_Render_Package
{
    protected:
        int packageId;
        std::string packageName;
    public:
        //Package properties
        GPE_Texture_Base * packageTexture;
        GPE_Renderer_Base * packageRenderer;
        GPE_Window * packageWindow;

        GPE_Render_Package( int pId, std::string pName );
        ~GPE_Render_Package();
        GPE_Texture_Base * create_texture();

        int get_package_id();
        std::string get_package_name();
};


#endif //RENDER_PACKAGE_H
