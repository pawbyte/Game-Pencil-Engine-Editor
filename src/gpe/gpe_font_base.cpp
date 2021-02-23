/*
gpe_font_base.cpp
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

#include "gpe_artist_base.h"
#include "gpe_runtime.h"
#include "gpe_font_base.h"

namespace gpe
{
    font_base * font_default = nullptr;
    font_system_controller * gfs = nullptr;

    font_pair_base::font_pair_base()
    {
        alpha_last_rendered = 255;
        str = "";
        text_width = 0;
        text_height = 0;
    }

    font_pair_base::~font_pair_base()
    {

    }

    bool font_pair_base::texture_matches(std::string str_check)
    {
        if( str==str_check)
        {
            return true;
        }
        return false;
    }

    int font_pair_base::get_width()
    {
        return text_width;
    }

    int font_pair_base::get_height()
    {
        return text_height;
    }

    font_base::font_base()
    {
        font_system_type = "base";
        font_family_name = "";
        font_nickname = "";
        last_used_halign = fa_left;
        last_used_valign = fa_top;
        font_id = -1;
        mono_width = 0;
        mono_height = 0;
        number_width = 0;
        number_height = 0;
        font_is_monospaced = false;
    }

    font_base::~font_base()
    {
        clear_cache();
    }

    void font_base::clear_cache()
    {

    }

    font_base * font_base::create_new(std::string file_loc, int f_size, bool make_monospaced, const std::string f_nickname, int id_number )
    {
        return new font_base();
    }

        font_pair_base * font_base::find_character_texture( const std::string id_number )
    {
        return nullptr;
    }

    font_pair_base * font_base::find_texture( const std::string text_to_render)
    {
        return nullptr;
    }

    int font_base::get_cache_count()
    {
        return 0;
    }

    int font_base::get_font_id()
    {
        return font_id;
    }

    void font_base::get_metrics(std::string text_to_render, int * width_value, int * height_value)
    {
        int tSize = (int)text_to_render.size();
        if( tSize > 0)
        {
            *width_value = mono_width*tSize;
            *height_value = mono_height;
        }
        else
        {
            *width_value = 0;
            *height_value = 0;
        }
    }

    void font_base::get_numbered_metrics(std::string text_to_render, int * width_value, int * height_value)
    {
        int tSize = (int)text_to_render.size();
        if( tSize > 0)
        {
            *width_value = number_width*tSize;
            *height_value = number_height;
        }
        else
        {
            *width_value = 0;
            *height_value = 0;
        }
    }

    void font_base::get_wrapped_string_metrics( const std::string str_in, int line_width, int linePadding, int * width_value, int *height_value )
    {

    }

    int font_base::get_mono_height()
    {
        return mono_height;
    }

    int font_base::get_mono_width()
    {
        return mono_width;
    }

    std::string font_base::get_nickname()
    {
        return font_nickname;
    }

    std::string font_base::get_family_name()
    {
        return font_family_name;
    }

    int font_base::get_font_size()
    {
        return font_size;
    }

    std::string font_base::get_font_type()
    {
        return font_system_type;
    }

    bool font_base::is_monospaced()
    {
        return font_is_monospaced;
    }

    void font_base::render_bitmapped_text( int x_pos, int y_pos, std::string number_to_render, color * text_color, int alignment_h,int alignment_v, int render_alpha )
    {
        error_log->report("Base class rendering font bitmapped!");
    }

    void font_base::render_text( int x_pos, int y_pos, std::string text_to_render, color * text_color, int alignment_h,int alignment_v, int render_alpha )
    {
        error_log->report("Base class rendering font!");
    }

    void font_base::render_text_scaled( int x_pos, int y_pos, std::string text_to_render, color * text_color, float text_scale, int alignment_h,int alignment_v, int render_alpha )
    {
        error_log->report("Base class rendering font scaled!");
    }

    void font_base::render_text_resized( int x_pos, int y_pos, std::string text_to_render, color * text_color, int alignment_h,int alignment_v, int render_width, int render_height, int render_alpha )
    {
        error_log->report("Base class rendering font resized!");
    }

    void font_base::render_text_boxed( int x_pos, int y_pos, std::string text_to_render, color * text_color,color * boxColor,int alignment_h,int alignment_v, int render_alpha )
    {
        error_log->report("Base class rendering font boxed!");
    }

    void font_base::render_text_rotated( int x_pos, int y_pos, std::string text_to_render, color * text_color, float textAngle, int render_alpha )
    {
        error_log->report("Base class rendering font rotated!");
    }

    bool font_base::render_text_special( int x_pos, int y_pos, std::string text_to_render, color * text_color, int alignment_h,int alignment_v, float render_angle , float render_scale, int render_alpha )
    {
        error_log->report("Base class rendering font special!");
        return false;
    }

    font_system_controller::font_system_controller()
    {
        newid_numberer = 0;
        fontTotalCount = 0;
        lastFontUsed = nullptr;
        font_template = new font_base();
    }

    font_system_controller::~font_system_controller()
    {
        if( font_template!=nullptr )
        {
            delete font_template;
            font_template = nullptr;
        }
    }

    font_base *  font_system_controller::copy_font( font_base * parent_font, std::string font_nickname , int dynamic_id )
    {
        if( parent_font == nullptr )
        {
            return nullptr;
        }
        if( dynamic_id < 0)
        {
            dynamic_id = newid_numberer;
        }
        font_base *  returnVal = font_template->create_new( parent_font->get_family_name(),parent_font->get_font_size(), parent_font->is_monospaced(), font_nickname, newid_numberer );
        loadedFonts.push_back(returnVal);
        newid_numberer++;
        fontTotalCount++;
        return returnVal;
    }


    font_base * font_system_controller::open_font(std::string fontLocation, int font_size, bool make_monospaced, std::string f_nickname, int dynamic_id )
    {
        if( dynamic_id < 0)
        {
            dynamic_id = newid_numberer;
        }
        if( font_template != nullptr )
        {
            font_base *  returnVal = font_template->create_new( fontLocation.c_str(),font_size, make_monospaced, f_nickname,dynamic_id );
            loadedFonts.push_back(returnVal);
            newid_numberer++;
            fontTotalCount++;
            return returnVal;
        }
        return nullptr;
    }

    void font_system_controller::close_font(font_base * fontIn)
    {
        if( fontIn!=nullptr)
        {
            font_base * cFont = nullptr;
            for( int i = (int)loadedFonts.size()-1; i >=0; i--)
            {
                cFont = loadedFonts[i];
                if( cFont!=nullptr)
                {
                    if( cFont->get_font_id()==fontIn->get_font_id() )
                    {
                        loadedFonts.erase( loadedFonts.begin()+i);
                        fontTotalCount--;
                    }
                }
            }
            fontIn->clear_cache();
            delete fontIn;
            fontIn = nullptr;
        }
    }

    void font_system_controller::clear_font_cache()
    {
        gpe::error_log->report("Clearing Font Cache...");
        font_base * cFont = nullptr;
        for( int i = (int)loadedFonts.size()-1; i >=0; i--)
        {
            cFont = loadedFonts[i];
            if( cFont!=nullptr)
            {
                cFont->clear_cache();
            }
        }
        gpe::error_log->report("Font Cache total: ["+ stg_ex::int_to_string(get_font_cache_size() )+"]...");
    }

    int font_system_controller::get_font_cache_size(bool recordCache)
    {
        font_base * cFont = nullptr;
        int cacheCount = 0;
        int i = 0;
        int foundCache = 0;
        if( recordCache)
        {
            gpe::error_log->report("\nCalculating Font Cache...");
            for( i = (int)loadedFonts.size()-1; i >=0; i--)
            {
                cFont = loadedFonts[i];
                if( cFont!=nullptr)
                {
                    foundCache =cFont->get_cache_count();
                    cacheCount+=foundCache;
                    gpe::error_log->report("      Font["+cFont->font_nickname+"] Cache total: ["+ stg_ex::int_to_string(foundCache )+"]...");
                }
            }
            gpe::error_log->report("Font Cache total: ["+ stg_ex::int_to_string(cacheCount )+"]...");
        }
        else
        {
            for( i = (int)loadedFonts.size()-1; i >=0; i--)
            {
                cFont = loadedFonts[i];
                if( cFont!=nullptr)
                {
                    cacheCount+=cFont->get_cache_count();
                }
            }
        }

        return cacheCount;
    }

    int font_system_controller::get_font_count()
    {
        return fontTotalCount;
    }

    bool font_system_controller::render_bitmap_text( int x_pos, int y_pos, std::string text_to_render, color * text_color, font_base * text_font, int alignment_h, int alignment_v, int render_alpha)
    {
        if( text_font==nullptr)
        {
            text_font = lastFontUsed;
        }
        if( text_font!=nullptr)
        {
            text_font->render_bitmapped_text( x_pos,y_pos,text_to_render,text_color,alignment_h,alignment_v,render_alpha);
            return true;
        }
        else
        {
            gpe::error_log->report( "Unable to render number ("+ text_to_render + ")");
        }
        return false;
    }

    void font_system_controller::render_fps( font_base * text_font, color * color,  int x_pos, int y_pos, int alignment_h , int alignment_v  )
    {
        //if( fps_show_counter )
        {
            if( color == nullptr )
            {
                color = c_lime;
            }

            if( window_controller_main->is_minimized() )
            {
                color =  c_red;
            }
            if( x_pos < 0 )
            {
                x_pos = gpe::screen_width;
            }
            if( y_pos < 0 )
            {
                y_pos = 0;
            }
            render_text( x_pos, y_pos, stg_ex::float_to_string(fps_current)+" / "+ stg_ex::float_to_string(fps_cap)+" fps | "+ stg_ex::float_to_string( delta_current )+" / " + stg_ex::float_to_string( time_keeper->get_needed_ticks() ) + " ms ",color,text_font,alignment_h, alignment_v );

        }
    }

    bool font_system_controller::render_only_text( int x_pos, int y_pos, std::string text_to_render, color * text_color, font_base * text_font, int alignment_h, int alignment_v, int render_alpha)
    {
        if( text_font==nullptr)
        {
            text_font = lastFontUsed;
        }
        if( (int)text_to_render.size() > 0)
        {
            if( text_font!=nullptr)
            {
                text_font->render_text( x_pos,y_pos,text_to_render,text_color,alignment_h,alignment_v,render_alpha);
                return true;
            }
        }
        return false;
    }

    bool font_system_controller::render_text( int x_pos, int y_pos, std::string text_to_render, color * text_color, font_base * text_font, int alignment_h, int alignment_v, int render_alpha)
    {
        if( text_font==nullptr)
        {
            text_font = lastFontUsed;
        }
        if( (int)text_to_render.size() > 0 && text_font!=nullptr )
        {
            //error_log->report("Rendering Text["+ text_to_render+"]");
            text_font->render_text( x_pos,y_pos,text_to_render,text_color,alignment_h,alignment_v,render_alpha);
            return true;
        }
        else if( (int)text_to_render.size() > 0 )
        {
            // std::cout << "Unable to render text (" << text_to_render << ").\n";
            error_log->report("Unable to render text [" + text_to_render + "]");
            return false;
        }

        return false;
    }

    bool font_system_controller::render_text_boxed( int x_pos, int y_pos, std::string text_to_render, color * text_color,color * boxColor, font_base * text_font, int alignment_h,int alignment_v, int render_alpha)
    {
        if( text_font==nullptr)
        {
            text_font = lastFontUsed;
        }
        if( text_font!=nullptr)
        {
            text_font->render_text_boxed( x_pos,y_pos,text_to_render,text_color,boxColor,alignment_h,alignment_v,render_alpha);
            return true;
        }
        return false;
    }

    bool font_system_controller::render_text_resized( int x_pos, int y_pos, std::string text_to_render, color * text_color, font_base * text_font, int alignment_h, int alignment_v, int render_width, int render_height, int render_alpha )
    {
        if( text_font==nullptr)
        {
            text_font = lastFontUsed;
        }
        if( text_font!=nullptr)
        {
            text_font->render_text_resized( x_pos,y_pos,text_to_render,text_color,alignment_h,alignment_v,render_width,render_height,render_alpha);
            return true;
        }
        else if( text_font!=nullptr )
        {
            std::cout << "Unable to render EXT text (" << text_to_render << ").\n";
        }
        return false;
    }

    bool font_system_controller::render_text_rotated( int x_pos, int y_pos, std::string text_to_render, color * text_color, font_base * text_font, float textAngle, int render_alpha )
    {
        if( text_font==nullptr)
        {
            text_font = lastFontUsed;
        }
        if( text_font!=nullptr)
        {
            text_font->render_text_rotated( x_pos,y_pos,text_to_render,text_color,textAngle,render_alpha);
            return true;
        }
        return false;
    }

    bool font_system_controller::render_text_scaled( int x_pos, int y_pos, std::string text_to_render,color * text_color, float scaleValue, font_base * text_font, int alignment_h,int alignment_v, int render_alpha )
    {
        if( text_font!=nullptr)
        {
            text_font->render_text_scaled( x_pos, y_pos, text_to_render, text_color, scaleValue, alignment_h, alignment_v, render_alpha );
        }
        return false;
    }

    bool font_system_controller::render_text_special( int x_pos, int y_pos, std::string text_to_render, color * text_color, font_base * text_font, int alignment_h,int alignment_v, float render_angle, float render_scale, int render_alpha )
    {
        if( text_font!=nullptr)
        {
            return text_font->render_text_special( x_pos, y_pos, text_to_render, text_color, alignment_h, alignment_v, render_angle, render_scale, render_alpha );
        }
        return false;
    }


    bool init_font_system()
    {
        //If we already started the font system do nothing.
        if( gfs!=nullptr )
        {
            return true;
        }

        if( gfs==nullptr )
        {
            gfs = new font_system_controller();
        }
        return true;
    }

    void quit_font_system()
    {
        if( gfs!=nullptr )
        {
            delete gfs;
            gfs = nullptr;
        }
        gpe::error_log->report("font_System Quit...");
    }

}
