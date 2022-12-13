/*
gpe_render_package.h
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

#ifndef gpe_render_package_h
#define gpe_render_package_h

#include "gpe_common_includes.h"
#include "gpe_renderer_base.h"
#include "gpe_animation2d.h"
#include "gpe_texture_base.h"
#include "gpe_window_controller_base.h"

namespace gpe
{
    class render_package
    {
        protected:
            int packageId;
            std::string packageName;
        public:
            //Package properties
            renderer_base * packageRenderer;
            texture_base * packageTexture;
            animation2d * packageAnimation2d;

            window_controller_base * packageWindow;

            render_package( int pId, std::string pName );
            ~render_package();
            animation2d * create_animation2d( const std::string& anim_name, const std::string& anim_filename , bool transparent_image=false );
            texture_base * create_texture();

            int get_package_id();
            std::string get_package_name();
            void set_vysnc( bool vs_on );
    };
}

#endif //gpe_render_package_h
