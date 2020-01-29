/*
GPE_Render_Package_Handler.cpp
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

#include "GPE_Render_Package_Handler.h"

GPE_Render_Package_Handler * gpeph = NULL;

GPE_Render_Package_Handler::GPE_Render_Package_Handler()
{
    renderPackageCount = 0;
}

GPE_Render_Package_Handler::~GPE_Render_Package_Handler()
{

}

GPE_Render_Package * GPE_Render_Package_Handler::add_render_package( std::string packageName)
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
    GPE_Render_Package * newPackage = new GPE_Render_Package(renderPackageCount, packageName);
    renderPackages.push_back( newPackage );
    renderPackageCount++;
    return newPackage;
}

void GPE_Render_Package_Handler::clear_render_packages()
{
    int packageMax = (int)renderPackages.size();
    GPE_Render_Package * tempPackage = NULL;
    for( int i = packageMax -1; i >= 0; i--)
    {
        tempPackage = renderPackages[i];
        if( tempPackage!=NULL && tempPackage->packageRenderer!=NULL && tempPackage->packageWindow!=NULL )
        {
            tempPackage->packageRenderer->clear_renderer( tempPackage->packageWindow->is_minimized() );
        }
    }
}

GPE_Render_Package * GPE_Render_Package_Handler::get_default_render_package()
{
    return get_render_package_from_name( defaultRenderPackageName);
}

GPE_Render_Package * GPE_Render_Package_Handler::get_render_package_from_id( int packageId )
{
    return NULL;
}

GPE_Render_Package * GPE_Render_Package_Handler::get_render_package_from_name( std::string packageName )
{
    if( (int)packageName.size() == 0 )
    {
        packageName = defaultRenderPackageName;
    }
    int packageMax = (int)renderPackages.size();
    GPE_Render_Package * tempPackage = NULL;
    for( int i = packageMax -1; i >= 0; i--)
    {
        tempPackage = renderPackages[i];
        if( tempPackage !=NULL && tempPackage->get_package_name() == packageName )
        {
            return tempPackage;
        }
    }
    return NULL;
}

GPE_Texture_Base * GPE_Render_Package_Handler::get_new_texture()
{
    return get_new_texture_from_name( defaultRenderPackageName );
}

GPE_Texture_Base * GPE_Render_Package_Handler::get_new_texture_from_id( int packageId )
{
    return NULL;
}

GPE_Texture_Base * GPE_Render_Package_Handler::get_new_texture_from_package( GPE_Render_Package * renderPackage)
{
    if( renderPackage == NULL)
    {
        return NULL;
    }
    return renderPackage->create_texture();
}

GPE_Texture_Base * GPE_Render_Package_Handler::get_new_texture_from_name( std::string packageName )
{
    GPE_Render_Package * fPackage = get_render_package_from_name( packageName);
    if( fPackage == NULL)
    {
        return NULL;
    }
    return fPackage->create_texture();
}

bool GPE_Render_Package_Handler::is_all_windows_minimized()
{
    int packageMax = (int)renderPackages.size();
    GPE_Render_Package * tempPackage = NULL;
    for( int i = packageMax -1; i >= 0; i--)
    {
        tempPackage = renderPackages[i];
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

int GPE_Render_Package_Handler::window_in_focus()
{
    int packageMax = (int)renderPackages.size();
    GPE_Render_Package * tempPackage = NULL;
    for( int i = packageMax -1; i >= 0; i--)
    {
        tempPackage = renderPackages[i];
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

void GPE_Render_Package_Handler::process_window_events( SDL_Event& e )
{
    int packageMax = (int)renderPackages.size();
    GPE_Render_Package * tempPackage = NULL;
    for( int i = packageMax -1; i >= 0; i--)
    {
        tempPackage = renderPackages[i];
        if( tempPackage!=NULL && tempPackage->packageWindow!=NULL )
        {
            tempPackage->packageWindow->process_events( e );
        }
    }
}

bool GPE_Render_Package_Handler::remove_all_render_packages(  bool deletePackage  )
{
    if( deletePackage )
    {
        int packageMax = (int)renderPackages.size();
        GPE_Render_Package * tempPackage = NULL;
        for( int i = packageMax -1; i >= 0; i--)
        {
            tempPackage = renderPackages[i];
            if( tempPackage !=NULL)
            {
                delete tempPackage;
                tempPackage =NULL;
            }
        }
    }
    renderPackages.clear();
}

bool GPE_Render_Package_Handler::remove_render_package( std::string packageName, bool deletePackage )
{
    int packageMax = (int)renderPackages.size();
    GPE_Render_Package * tempPackage = NULL;
    for( int i = packageMax -1; i >= 0; i--)
    {
        tempPackage = renderPackages[i];
        if( tempPackage !=NULL && tempPackage->get_package_name() == packageName )
        {
            if( deletePackage )
            {
                delete tempPackage;
                tempPackage =NULL;
            }
            renderPackages.erase( renderPackages.begin() + i );
        }
    }
}

void GPE_Render_Package_Handler::update_render_packages()
{
    int packageMax = (int)renderPackages.size();
    GPE_Render_Package * tempPackage = NULL;
    for( int i = packageMax -1; i >= 0; i--)
    {
        tempPackage = renderPackages[i];
        if( tempPackage!=NULL && tempPackage->packageRenderer!=NULL && tempPackage->packageWindow!=NULL )
        {
            tempPackage->packageRenderer->update_renderer( tempPackage->packageWindow->is_minimized() );
        }
    }
}
