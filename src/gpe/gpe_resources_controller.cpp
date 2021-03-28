/*
gpe_resources_controller.cpp
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

#include "gpe_resources_controller.h"

namespace gpe
{
    std::string resource_type_names[resource_type_max];
    std::string resource_type_names_plural[resource_type_max];

    asset_manager::asset_manager(render_package * start_package, std::string start_name)
    {
        rr_package = start_package;
        managerName = start_name;
    }

    asset_manager::~asset_manager()
    {
        //loops and deletes music
        clean_up();
    }

    //adds a new game animation with one row
    animaton2d* asset_manager::animation_add(std::string resource_name, std::string resource_file_name,int imgnumb,bool transparent,int xorig,int yorig, bool add_mirror)
    {
        return nullptr;
        gpe::error_log->report("Attempting to add animation["+resource_name+"+,"+resource_file_name+"]" );

        animaton2d * newAnimation = get_animation(resource_name);
        if(newAnimation!=nullptr)
            return newAnimation;
        //makes sure the xorig and yorig is not negative
        if(( xorig>=0)&&(yorig>=0) )
        {
            //error_log->report("Loading "+resource_file_name+" animation with "+ stg_ex::int_to_string(imgnumb)+" needed images.");
            //loads in the animation's texture
            newAnimation = new animaton2d( rr_package, resource_name, resource_file_name, transparent );
            atlas_animations[resource_name] = newAnimation;
            last_animation_loaded = resource_name;
            return newAnimation;
        }
        else
        {
            std::stringstream outstring;
            outstring << "Unable to load animation from file location: (" << resource_file_name << ") due to negative dimensions given";
            error_log->report( outstring.str());
            return nullptr;
        }
    }

    animaton2d* asset_manager::animation_add_collision(std::string resource_name,std::string resource_file_name,int imgnumb,bool transparent,int xorig,int yorig, int cx, int cy, int cw, int ch, bool add_mirror)
    {
        gpe::error_log->report("Attempting to add animation_collision["+resource_name+"+,"+resource_file_name+"]" );
        animaton2d * new_animation = get_animation(resource_file_name);
        if(new_animation!=nullptr)
            return new_animation;
        //makes sure the xorig and yorig is not negative
        if(( xorig>=0)&&(yorig>=0) )
        {
            error_log->report("Loading" +resource_file_name+" animation...");
            //loads in the animation's texture
            new_animation = new animaton2d( rr_package, resource_file_name,resource_file_name, transparent);
            new_animation->edit_collision_box(cx, cy, cw, ch);
            atlas_animations[resource_name] = new_animation;
            last_animation_loaded = resource_name;
            return new_animation;
        }
        else
        {
            std::stringstream outstring;
            outstring << "Unable to load animation from file location: (" << resource_file_name << ") due to negative dimensions given";
            error_log->report( outstring.str());
            return nullptr;
        }
    }

    animaton2d* asset_manager::animation_addsheet(std::string resource_name, std::string resource_file_name, bool transparent, int width, int height)
    {
        gpe::error_log->report("Attempting to add animation_sheet["+resource_name+"+,"+resource_file_name+"]" );
        animaton2d * newAnimation = get_animation(resource_file_name);
        if(newAnimation!=nullptr)
            return newAnimation;
        if(( height>0)&&(width>0) )
        {
            newAnimation = new animaton2d(rr_package, resource_file_name,resource_file_name, transparent);
            atlas_animations[resource_name] = newAnimation;
            last_animation_loaded = resource_name;
            return newAnimation;
        }
        else
        {
            std::stringstream outstring;
            outstring << "Unable to load animation from file location: (" << resource_file_name << " due to negative dimensions given)";
            error_log->report( outstring.str());
            return nullptr;
        }
    }

    animaton2d* asset_manager::animation_addsheet_ext( std::string resource_name, std::string resource_file_name, bool transparent, unsigned short imgnumb, unsigned short imgPerRow, unsigned short width, unsigned short height, unsigned short cell_offx, unsigned short cell_offy, unsigned short pixel_offx, unsigned short pixel_offy, unsigned short hsep_px, unsigned short vsep_px)
    {
        gpe::error_log->report("Attempting to add animation_sheet_Ext["+resource_name+"+,"+resource_file_name+"]" );
        animaton2d * newAnimation = get_animation(resource_file_name);
        if(newAnimation!=nullptr)
            return newAnimation;
        if(( height!=0)&&(width!=0) )
        {
            newAnimation = new animaton2d(rr_package, resource_file_name, resource_file_name, transparent);
            atlas_animations[resource_name] = newAnimation;
            last_animation_loaded = resource_name;
            return newAnimation;
        }
        else
        {
            std::stringstream outstring;
            outstring << "Unable to load animation from file location: (" << resource_file_name << " due to negative dimensions given)";
            error_log->report( outstring.str());
            return nullptr;
        }
    }

    music_base * music_add( std::string s_name, std::string s_file,int group_id_number =-1 )
    {
        return nullptr;
    }

    sound_base * sound_add(int s_id, std::string s_name, std::string s_file,int group_id_number =-1 )
    {
        return nullptr;
    }

    texture_base * asset_manager::texture_add(std::string resource_name, std::string texture_file_name)
    {
        if( (int)resource_name.size() == 0)
        {
            gpe::error_log->report("Error: Attempting to add Texture with ["+texture_file_name+"] with a blank name" );
            return nullptr;
        }
        texture_base * newTexture = get_texture(resource_name);
        if( newTexture!=nullptr )
        {
            return newTexture;
        }
        else if( main_file_url_manager->file_exists( texture_file_name) && !main_file_url_manager->folder_exists( texture_file_name) )
        {
            if( resource_name!= texture_file_name )
            {
                gpe::error_log->report("Attempting to add Texture["+resource_name+","+texture_file_name+"]" );
            }
            else
            {
                gpe::error_log->report("Attempting to add Texture["+texture_file_name+"]" );
            }
            //loads in the animation's texture
            newTexture = rr_package->create_texture();
            if( newTexture!=nullptr)
            {
                newTexture->load_new_texture( rr_package->packageRenderer,texture_file_name, -1,true );
                if(newTexture->get_width() > 0)
                {
                    error_log->report( "Texture_Add "+stg_ex::get_local_from_global_file( texture_file_name )+ "["+ stg_ex::int_to_string( (int)atlas_textures.size() )+"]");
                    atlas_textures[resource_name] = newTexture;
                    last_texture_loaded = resource_name;
                    return newTexture;
                }
                else
                {
                    delete newTexture;
                    newTexture = nullptr;
                }
            }
        }
        return nullptr;
    }

    texture_base * asset_manager::texture_add_filename( std::string texture_file_name)
    {
        return texture_add( texture_file_name, texture_file_name );
    }

    animaton2d* asset_manager::get_animation(std::string resource_name )
    {
        auto pairExists = atlas_animations.find( resource_name );
        if (pairExists == atlas_animations.end() )
        {
            return nullptr;
        }
        return atlas_animations[resource_name];
    }

    std::string asset_manager::get_last_animation_name()
    {
        return last_animation_loaded;
    }

    std::string asset_manager::get_last_music_name()
    {
        return last_music_loaded;;
    }

    std::string asset_manager::get_last_sound_name()
    {
        return last_sound_loaded;
    }

    std::string asset_manager::get_last_texture_name()
    {
        return last_texture_loaded;
    }

    std::string asset_manager::get_last_tilesheet_name()
    {
        return last_tilesheet_loaded;
    }

    std::string asset_manager::get_name()
    {
        return managerName;
    }

    int asset_manager::get_size_animations()
    {
        return (int)atlas_animations.size();
    }

    int asset_manager::get_size_music()
    {
        return (int)atlas_music.size();
    }

    int asset_manager::get_size_sounds()
    {
        return (int)atlas_sounds.size();
    }

    int asset_manager::get_size_textures()
    {
        return (int)atlas_textures.size();
    }

    int asset_manager::get_size_tilesheets()
    {
        return (int)atlas_tilesheets.size();
    }


    music_base * asset_manager::get_music(std::string  resource_name)
    {
        auto pairExists = atlas_music.find( resource_name );
        if (pairExists == atlas_music.end() )
        {
            return nullptr;
        }
        return atlas_music[resource_name];
    }

    sound_base * asset_manager::get_sound(std::string  resource_name)
    {
        auto pairExists = atlas_sounds.find( resource_name );
        if (pairExists == atlas_sounds.end() )
        {
            return nullptr;
        }
        return atlas_sounds[resource_name];
    }

    texture_base * asset_manager::get_texture(std::string resource_name )
    {
        auto pairExists = atlas_textures.find( resource_name );
        if (pairExists == atlas_textures.end() )
        {
            return nullptr;
        }
        return atlas_textures[resource_name];
    }

    void asset_manager::play_music( std::string resource_name, int loops )
    {

    }

    void asset_manager::play_sound( std::string resource_name, int loops )
    {

    }

    void asset_manager::remove_animation( std::string resource_name )
    {

    }

    void asset_manager::remove_texture( std::string resource_name )
    {
        /*
        if( tex!=nullptr )
        {
            texture_base * tTex;
            for( int i= (int)atlas_textures.size()-1; i>=0; i-- )
            {
                tTex = atlas_textures[i];
                if ( tTex!= nullptr)
                {
                    if( tTex->get_name()==tex->get_name() || tTex->get_filename()==tex->get_filename() )
                    {
                        delete tTex;
                        tTex = nullptr;
                        atlas_textures.erase( atlas_textures.begin()+i );
                    }
                }
            }
        }
        */
    }


    bool asset_manager::load_files()
    {
        //If everything loaded fine
        return true;
    }

    void asset_manager::clean_up()
    {
        std::map< std::string, animaton2d *>::iterator itAnim;
        animaton2d * temp_anim = nullptr;
        for( itAnim = atlas_animations.begin(); itAnim != atlas_animations.end(); ++itAnim )
        {
            temp_anim = itAnim->second;

            if( temp_anim !=nullptr)
            {
                delete temp_anim;
                temp_anim = nullptr;
            }
        }
        atlas_animations.clear();

        std::map< std::string, music_base *>::iterator it_music;
        music_base * temp_music = nullptr;
        for( it_music = atlas_music.begin(); it_music != atlas_music.end(); ++it_music )
        {
            temp_music = it_music->second;

            if( temp_music !=nullptr)
            {
                delete temp_music;
                temp_music =nullptr;
            }
        }
        atlas_music.clear();

        std::map<std::string, sound_base *>::iterator it_sounds;
        sound_base * temp_sound = nullptr;
        for( it_sounds = atlas_sounds.begin(); it_sounds != atlas_sounds.end();  ++it_sounds )
        {
            temp_sound = it_sounds->second;
            if( temp_sound !=nullptr)
            {
                delete temp_sound;
                temp_sound = nullptr;
            }
        }
        atlas_sounds.clear();

        std::map< std::string, texture_base *>::iterator it_textures;
        texture_base * temp_texture = nullptr;
        for( it_textures = atlas_textures.begin(); it_textures != atlas_textures.end(); ++it_textures )
        {
            temp_texture = it_textures->second;

            if( temp_texture !=nullptr)
            {
                delete temp_texture;
                temp_texture = nullptr;
            }
        }
        atlas_textures.clear();

        std::map< std::string, tilesheet *>::iterator it_tilesheets;
        tilesheet * temp_tilesheet = nullptr;
        for( it_tilesheets = atlas_tilesheets.begin(); it_tilesheets != atlas_tilesheets.end(); ++it_tilesheets )
        {
            temp_tilesheet = it_tilesheets->second;

            if( temp_tilesheet !=nullptr)
            {
                delete temp_tilesheet;
                temp_tilesheet = nullptr;
            }
        }
        atlas_tilesheets.clear();
    }
}
