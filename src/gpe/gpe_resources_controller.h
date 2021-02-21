/*
gpe_resources_controller.h
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

#ifndef gpe_resources_controller_h
#define gpe_resources_controller_h

#include "gpe_common_includes.h"
#include "gpe_constants.h"
#include "gpe_globals.h"
#include "gpe_texture_base.h"
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include "gpe_color_manager.h"

#include "gpe_animation2d.h"
#include "gpe_tilesheet.h"
#include "gpe_audio_base.h"
#include "../other_libs/stg_ex.h"

namespace gpe
{
    extern std::string resource_type_names[resource_type_max];
    extern std::string resource_type_names_plural[resource_type_max];
    //Controls and handles all of the game's resources( animations & Backgrounds and maybe sounds and music
    class asset_manager
    {
        private:
            std::string managerName;
            render_package * rr_package;

            std::string last_animation_loaded;
            std::map<std::string, animaton2d*> atlas_animations; //map of every game animation

            std::string last_music_loaded;
            std::map<std::string, music_base *> atlas_music; //map of every game music object

            std::string last_sound_loaded;
            std::map<std::string, sound_base *> atlas_sounds; //map of every game sound

            std::string last_texture_loaded;
            std::map<std::string, texture_base *> atlas_textures; //map of every game texture

            std::string last_tilesheet_loaded;
            std::map<std::string, tilesheet*> atlas_tilesheets; //map of every game tilesheet

        public:
            asset_manager(render_package * start_package, std::string start_name); // constructor
            ~asset_manager();  //deconstructor
            //adds a animation with only one row of subimages

            animaton2d* animation_add( std::string resource_name, std::string resource_file_name,int imgnumb,bool transparent=true,int xorig=0,int yorig=0, bool add_mirror=false);
            animaton2d* animation_add_collision(std::string resource_name, std::string resource_file_name,int imgnumb,bool transparent=true,int xorig=0,int yorig=0, int cx=0, int cy=0, int cw=-1, int ch=-1, bool add_mirror=false);
            //animaton2d* animation_add_centered(std::string filename,int imgnumb,bool transparent,int xorig,int yorig, bool add_mirror);
            animaton2d* animation_addsheet( std::string resource_name, std::string resource_file_name, bool transparent, int width, int height);
            animaton2d* animation_addsheet_ext( std::string resource_name, std::string resource_file_name, bool transparent, unsigned short imgnumb, unsigned short imgPerRow, unsigned short width, unsigned short height, unsigned short cell_offx, unsigned short cell_offy, unsigned short pixel_offx, unsigned short pixel_offy, unsigned short hsep_px, unsigned short vsep_px);

            music_base * music_add(int s_id, std::string s_name, std::string s_file,int group_id_number =-1 );
            sound_base * sound_add(int s_id, std::string s_name, std::string s_file,int group_id_number =-1 );

            //Clean up
            void clean_up();

            //Getters
            animaton2d * get_animation( std::string resource_name );

            std::string get_last_animation_name();
            std::string get_last_music_name();
            std::string get_last_sound_name();
            std::string get_last_texture_name();
            std::string get_last_tilesheet_name();

            std::string get_name();

            int get_size_animations();
            int get_size_music();
            int get_size_sounds();
            int get_size_textures();
            int get_size_tilesheets();

            texture_base * get_texture(std::string texture_file_name);

            music_base * get_music(std::string name_param);
            sound_base * get_sound(std::string name_param);

            //File loading
            bool load_files();

            void play_music( std::string resource_name, int loops=-1);
            void play_sound( std::string resource_name, int loops=-1);

            void remove_animation( std::string resource_name );
            void remove_texture( std::string resource_name );

            texture_base * texture_add(std::string resource_name, std::string texture_file_name);
            texture_base * texture_add_filename( std::string texture_file_name);
    };
}

#endif //gpe_resources_controller_h
