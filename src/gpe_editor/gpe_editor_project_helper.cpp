/*
gpe_editor_project_helper.cpp
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

#include "gpe_editor_project_helper.h"

void setup_project_directory(std::string newProjectDir)
{
    if( (int)newProjectDir.size() > 3)
    {
        sff_ex::create_directory(newProjectDir);
        sff_ex::create_directory(newProjectDir+"/gpe_project");
        sff_ex::create_directory(newProjectDir+"/gpe_project/builds");

        sff_ex::create_directory(newProjectDir+"/gpe_project/data");

        sff_ex::create_directory(newProjectDir+"/gpe_project/gpebackups");

        sff_ex::create_directory(newProjectDir+"/gpe_project/resources");
        sff_ex::create_directory(newProjectDir+"/gpe_project/resources/achievements");

        for( int i_res_type = 0; i_res_type < gpe::resource_type_max; i_res_type++ )
        {
            sff_ex::create_directory(newProjectDir+"/gpe_project/resources/" + stg_ex::string_lower( gpe::resource_type_names_plural[ i_res_type] ) );
        }

        sff_ex::create_directory(newProjectDir+"/gpe_project/source");
        sff_ex::create_directory(newProjectDir+"/gpe_project/source/GPE");
        sff_ex::create_directory(newProjectDir+"/gpe_project/source/AOSGUI");
    }
}

std::string setup_cpp_folder(std::string buildDirectory,int buildType, int buildBits, bool inDebugMode)
{
    if( (int)buildDirectory.size() < 3)
    {
        return "";
    }
    std::string newBuildDirectory = buildDirectory;

    sff_ex::create_directory(buildDirectory);
    if( sff_ex::path_exists(buildDirectory) )
    {
        if( ( buildType==gpe::system_os_windows || buildType== gpe::system_os_linux ) )
        {
            sff_ex::create_directory(buildDirectory+"/gamedata");
            sff_ex::create_directory(buildDirectory+"/resources");
            if( sff_ex::path_exists( buildDirectory+"/resources") )
            {
                newBuildDirectory = buildDirectory+"/resources";
                sff_ex::create_directory(newBuildDirectory+"/animations");
                sff_ex::create_directory(newBuildDirectory+"/audio");
                sff_ex::create_directory(newBuildDirectory+"/fonts");
                sff_ex::create_directory(newBuildDirectory+"/lights");
                sff_ex::create_directory(newBuildDirectory+"/particles");
                sff_ex::create_directory(newBuildDirectory+"/animations");
                sff_ex::create_directory(newBuildDirectory+"/textures");
                sff_ex::create_directory(newBuildDirectory+"/tilesheets");
                sff_ex::create_directory(newBuildDirectory+"/videos");
            }
        }
        return newBuildDirectory;
    }
    return "";
}

std::string setup_js_folder(std::string buildDirectory,int buildType, int buildBits, bool inDebugMode, int nativeBuildType)
{
    if( (int)buildDirectory.size() < 3)
    {
        return "";
    }

    /*if( buildType!=system_os_html5 )
    {
        if( buildBits==64)
        {
            buildDirectory+="_64";
        }
        else
        {
            buildDirectory+="_32";
        }
    }
    if( inDebugMode)
    {
        //buildDirectory+="_debug";
    }
    */
    std::string newBuildDirectory = buildDirectory;

    sff_ex::create_directory(buildDirectory);
    if( sff_ex::path_exists(buildDirectory) )
    {
        if( ( buildType==gpe::system_os_windows || buildType== gpe::system_os_linux ) && nativeBuildType==native_option_electron)
        {
            sff_ex::create_directory(buildDirectory+"/locales");
            sff_ex::create_directory(buildDirectory+"/resources");
            sff_ex::create_directory(buildDirectory+"/resources/app");
            if( sff_ex::path_exists( buildDirectory+"/resources/app") )
            {
                newBuildDirectory = buildDirectory+"/resources/app";
            }
        }
        else if( buildType== gpe::system_os_mac && nativeBuildType==native_option_electron)
        {
            sff_ex::create_directory(buildDirectory+"/Electron.app");
            sff_ex::create_directory(buildDirectory+"/Electron.app/Contents");
            sff_ex::create_directory(buildDirectory+"/Electron.app/Contents/Resources");
            if( sff_ex::path_exists( buildDirectory+"/Electron.app/Contents/Resources") )
            {
                newBuildDirectory = buildDirectory+"/Electron.app/Contents/Resources";
            }
        }

        sff_ex::create_directory(newBuildDirectory+"/css");
        sff_ex::create_directory(newBuildDirectory+"/gamedata");
        sff_ex::create_directory(newBuildDirectory+"/js");
        sff_ex::create_directory(newBuildDirectory+"/js/lib");

        sff_ex::create_directory(newBuildDirectory+"/res");

        sff_ex::create_directory(newBuildDirectory+"/resources");
        sff_ex::create_directory(newBuildDirectory+"/resources/animations");
        sff_ex::create_directory(newBuildDirectory+"/resources/audio");
        sff_ex::create_directory(newBuildDirectory+"/resources/fonts");
        sff_ex::create_directory(newBuildDirectory+"/resources/lights");
        sff_ex::create_directory(newBuildDirectory+"/resources/particles");
        sff_ex::create_directory(newBuildDirectory+"/resources/animations");
        sff_ex::create_directory(newBuildDirectory+"/resources/textures");
        sff_ex::create_directory(newBuildDirectory+"/resources/tilesheets");
        sff_ex::create_directory(newBuildDirectory+"/resources/videos");

        if( nativeBuildType ==native_option_electron )
        {
            if( buildType == gpe::system_os_windows)
            {
                if( main_gpe_splash_page != nullptr )
                {
                    main_gpe_splash_page->update_messages( "Copying Electron-Windows Files", "Please wait","Do NOT CLOSE" );
                }
                if( buildBits==64)
                {
                    if( main_gpe_splash_page!=nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [game.exe and dlls] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/win64build_electron",buildDirectory);

                    if( main_gpe_splash_page!=nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [game.exe and dlls] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/win64build_electron/locales",buildDirectory+"/locales");


                    if( main_gpe_splash_page!=nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [resources] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/win64build_electron/resources",buildDirectory+"/resources", true);
                }
                else
                {
                    if( main_gpe_splash_page!=nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [game.exe and dlls] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/win32build_electron",buildDirectory);

                    if( main_gpe_splash_page!=nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [locales] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/win32build_electron/locales",buildDirectory+"/locales");

                    if( main_gpe_splash_page!=nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [resources] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/win32build_electron/resources",buildDirectory+"/resources", true);
                }
            }
            else if( buildType == gpe::system_os_linux)
            {
                if( main_gpe_splash_page != nullptr )
                {
                    main_gpe_splash_page->update_messages( "Copying Electron-Linux Files", "Please wait","Do NOT CLOSE" );
                }

                if( buildBits==64)
                {
                    if( main_gpe_splash_page != nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [game and SO files] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/linux64build_electron",buildDirectory);

                    if( main_gpe_splash_page != nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [locales] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/linux64build_electron/locales",buildDirectory+"/locales");


                    if( main_gpe_splash_page != nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [resources] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/linux64build_electron/resources",buildDirectory+"/resources", true);
                }
                else
                {
                    if( main_gpe_splash_page != nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [game and SO files] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/linux32build_electron",buildDirectory);

                    if( main_gpe_splash_page != nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [locales] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/linux32build_electron/locales",buildDirectory+"/locales");

                    if( main_gpe_splash_page != nullptr )
                    {
                        main_gpe_splash_page->update_submessages( "Copying [resources] folder", "Please Wait..." );
                    }
                    gpe::copy_folder( gpe::app_directory_name+"build_files/linux32build_electron/resources",buildDirectory+"/resources", true);
                }
            }
        }
        return newBuildDirectory;
    }
    return "";
}
