/*
gpe_render_package_handler.h
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

#ifndef gpe_render_package_handler_h
#define gpe_render_package_handler_h

#include "gpe_render_package.h"

namespace gpe
{
    class render_package_handler
    {
        protected:
            std::vector <render_package *> renderPackages;
            int renderPackageCount;
        public:
            std::string defaultRenderPackageName;
            render_package_handler();
            ~render_package_handler();

            //Adding
            render_package * add_render_package( std::string packageName );

            //Clearing render packages
            void clear_render_packages();

            //Package /Texture getters
            render_package * get_default_render_package();
            render_package * get_render_package_from_id( int packageId );
            render_package * get_render_package_from_name( std::string packageName );
            texture_base * get_new_texture();
            texture_base * get_new_texture_from_package( render_package * renderPackage);
            texture_base * get_new_texture_from_id( int packageId );
            texture_base * get_new_texture_from_name( std::string packageName );

            //Processing all of the windows
            bool is_all_windows_minimized();
            void process_event( input_event_container * event_holder );
            int window_in_focus();

            //Removals
            bool remove_all_render_packages(  bool deletePackage = true );
            bool remove_render_package( std::string packageName, bool deletePackage = true );

            //Update renders
            void update_render_packages();
    };
    extern render_package_handler * rph;
}
#endif //gpe_render_package_handler_h
