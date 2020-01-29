/*
GPE_Render_Package_Handler.h
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

#ifndef RENDER_PACKAGE_HANDLER_H
#define RENDER_PACKAGE_HANDLER_H

#include "GPE_Render_Package.h"

class GPE_Render_Package_Handler
{
    protected:
        std::vector <GPE_Render_Package *> renderPackages;
        int renderPackageCount;
    public:
        std::string defaultRenderPackageName;
        GPE_Render_Package_Handler();
        ~GPE_Render_Package_Handler();

        //Adding
        GPE_Render_Package * add_render_package( std::string packageName );

        //Clearing render packages
        void clear_render_packages();

        //Package /Texture getters
        GPE_Render_Package * get_default_render_package();
        GPE_Render_Package * get_render_package_from_id( int packageId );
        GPE_Render_Package * get_render_package_from_name( std::string packageName );
        GPE_Texture_Base * get_new_texture();
        GPE_Texture_Base * get_new_texture_from_package( GPE_Render_Package * renderPackage);
        GPE_Texture_Base * get_new_texture_from_id( int packageId );
        GPE_Texture_Base * get_new_texture_from_name( std::string packageName );

        //Processing all of the windows
        bool is_all_windows_minimized();
        int window_in_focus();
        void process_window_events( SDL_Event& e );

        //Removals
        bool remove_all_render_packages(  bool deletePackage = true );
        bool remove_render_package( std::string packageName, bool deletePackage = true );

        //Update renders
        void update_render_packages();
};

extern GPE_Render_Package_Handler * gpeph;

#endif //RENDER_PACKAGE_HANDLER_H
