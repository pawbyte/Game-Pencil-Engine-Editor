/*
gpe_render_package_handler.cpp
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

#include "gpe_render_package_handler.h"

namespace gpe
{
    render_package_handler * rph = NULL;

    render_package_handler::render_package_handler()
    {
        r_packageCount = 0;
    }

    render_package_handler::~render_package_handler()
    {

    }

    render_package * render_package_handler::add_render_package( std::string packageName)
    {
        if( (int)packageName.size() == 0)
        {
            return NULL;
        }

        if( get_render_package_from_name( packageName) != NULL )
        {
            //can't add duplicates
            return NULL;
        }
        render_package * newPackage = new render_package(r_packageCount, packageName);
        r_packages.push_back( newPackage );
        r_packageCount++;
        return newPackage;
    }

    void render_package_handler::clear_render_packages()
    {
        int packageMax = (int)r_packages.size();
        render_package * tempPackage = NULL;
        for( int i = packageMax -1; i >= 0; i--)
        {
            tempPackage = r_packages[i];
            if( tempPackage!=NULL && tempPackage->packageRenderer!=NULL && tempPackage->packageWindow!=NULL )
            {
                tempPackage->packageRenderer->clear_renderer( tempPackage->packageWindow->is_minimized() );
            }
        }
    }

    render_package * render_package_handler::get_default_render_package()
    {
        return get_render_package_from_name( defaultr_packageName);
    }

    render_package * render_package_handler::get_render_package_from_id( int packageId )
    {
        return NULL;
    }

    render_package * render_package_handler::get_render_package_from_name( std::string packageName )
    {
        if( (int)packageName.size() == 0 )
        {
            packageName = defaultr_packageName;
        }
        int packageMax = (int)r_packages.size();
        render_package * tempPackage = NULL;
        for( int i = packageMax -1; i >= 0; i--)
        {
            tempPackage = r_packages[i];
            if( tempPackage !=NULL && tempPackage->get_package_name() == packageName )
            {
                return tempPackage;
            }
        }
        return NULL;
    }

    texture_base * render_package_handler::get_new_texture()
    {
        return get_new_texture_from_name( defaultr_packageName );
    }

    texture_base * render_package_handler::get_new_texture_from_id( int packageId )
    {
        return NULL;
    }

    texture_base * render_package_handler::get_new_texture_from_package( render_package * r_package)
    {
        if( r_package == NULL)
        {
            return NULL;
        }
        return r_package->create_texture();
    }

    texture_base * render_package_handler::get_new_texture_from_name( std::string packageName )
    {
        render_package * fPackage = get_render_package_from_name( packageName);
        if( fPackage == NULL)
        {
            return NULL;
        }
        return fPackage->create_texture();
    }

    bool render_package_handler::is_all_windows_minimized()
    {
        int packageMax = (int)r_packages.size();
        render_package * tempPackage = NULL;
        for( int i = packageMax -1; i >= 0; i--)
        {
            tempPackage = r_packages[i];
            if( tempPackage!=NULL && tempPackage->packageWindow!=NULL )
            {
                if( tempPackage->packageWindow->is_minimized() == false )
                {
                    return false;
                }
            }
        }
        return true;
    }

    void render_package_handler::process_event( input_event_container * e )
    {
        int packageMax = (int)r_packages.size();
        render_package * tempPackage = NULL;
        for( int i = packageMax -1; i >= 0; i--)
        {
            tempPackage = r_packages[i];
            if( tempPackage!=NULL && tempPackage->packageWindow!=NULL )
            {
                tempPackage->packageWindow->process_event( e );
            }
        }
    }

    int render_package_handler::window_in_focus()
    {
        int packageMax = (int)r_packages.size();
        render_package * tempPackage = NULL;
        for( int i = packageMax -1; i >= 0; i--)
        {
            tempPackage = r_packages[i];
            if(  tempPackage->packageWindow!=NULL )
            {
                if( tempPackage->packageWindow->has_focus() )
                {
                    return tempPackage->packageWindow->get_window_id();
                }
            }
        }
        return -1;
    }

    bool render_package_handler::remove_all_render_packages(  bool deletePackage  )
    {
        if( deletePackage )
        {
            int packageMax = (int)r_packages.size();
            render_package * tempPackage = NULL;
            for( int i = packageMax -1; i >= 0; i--)
            {
                tempPackage = r_packages[i];
                if( tempPackage !=NULL)
                {
                    delete tempPackage;
                    tempPackage =NULL;
                }
            }
        }
        r_packages.clear();
    }

    bool render_package_handler::remove_render_package( std::string packageName, bool deletePackage )
    {
        int packageMax = (int)r_packages.size();
        render_package * tempPackage = NULL;
        for( int i = packageMax -1; i >= 0; i--)
        {
            tempPackage = r_packages[i];
            if( tempPackage !=NULL && tempPackage->get_package_name() == packageName )
            {
                if( deletePackage )
                {
                    delete tempPackage;
                    tempPackage =NULL;
                }
                r_packages.erase( r_packages.begin() + i );
            }
        }
    }

    void render_package_handler::update_render_packages()
    {
        int packageMax = (int)r_packages.size();
        render_package * tempPackage = NULL;
        for( int i = packageMax -1; i >= 0; i--)
        {
            tempPackage = r_packages[i];

            if( tempPackage!=NULL && tempPackage->packageRenderer!=NULL && tempPackage->packageWindow!=NULL )
            {
                tempPackage->packageRenderer->update_renderer( tempPackage->packageWindow->is_minimized() );
            }
        }
    }
}
