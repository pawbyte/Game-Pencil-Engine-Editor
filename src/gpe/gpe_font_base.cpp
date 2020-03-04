/*
gpe_font.cpp
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

#include "gpe_artist_base.h"
#include "gpe_runtime.h"
#include "gpe_font_base.h"

namespace gpe
{
    font_base * font_default = NULL;
    font_system_controller * gfs = NULL;

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

    bool font_pair_base::texture_matches(std::string strCheck)
    {
        if( str==strCheck)
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
        fontFamilyName = "";
        fontNickName = "";
        last_used_halign = fa_left;
        last_used_valign = fa_top;
        customFontId = -1;
        monoWidth = 0;
        monoHeight = 0;
        numberWidth = 0;
        numberHeight = 0;
        fontIsMonoSpaced = false;
    }

    font_base::~font_base()
    {
        clear_cache();
    }

    void font_base::clear_cache()
    {

    }

    font_base * font_base::create_new(std::string fFileLocation, int fSize, bool isMonospaced, const std::string fNickName, int fontIdNumb )
    {
        return NULL;
    }

    int font_base::get_cache_count()
    {
        return 0;
    }

    int font_base::get_font_id()
    {
        return customFontId;
    }

    void font_base::get_metrics(std::string textToRender, int * wVal, int * hVal)
    {
        int tSize = (int)textToRender.size();
        if( tSize > 0)
        {
            *wVal = monoWidth*tSize;
            *hVal = monoHeight;
        }
        else
        {
            *wVal = 0;
            *hVal = 0;
        }
    }

    void font_base::get_numbered_metrics(std::string textToRender, int * wVal, int * hVal)
    {
        int tSize = (int)textToRender.size();
        if( tSize > 0)
        {
            *wVal = numberWidth*tSize;
            *hVal = numberHeight;
        }
        else
        {
            *wVal = 0;
            *hVal = 0;
        }
    }

    int font_base::get_mono_height()
    {
        return monoHeight;
    }

    int font_base::get_mono_width()
    {
        return monoWidth;
    }

    std::string font_base::get_family_name()
    {
        return fontFamilyName;
    }

    font_pair_base * font_base::find_character_texture( const std::string numbId )
    {
        return NULL;
    }

    font_pair_base * font_base::find_texture( const std::string textToRender)
    {
        return NULL;
    }

    void font_base::render_bitmapped_text( int xPos, int yPos, std::string numberToRender, color * textColor, int hAlign,int vAlign, int renderAlpha )
    {
    }

    void font_base::render_text( int xPos, int yPos, std::string textureText, color * textColor, int hAlign,int vAlign, int renderAlpha )
    {
    }

    void font_base::render_text_scaled( int xPos, int yPos, std::string textureText, color * textColor, float textScale, int hAlign,int vAlign, int renderAlpha )
    {

    }

    void font_base::render_text_resized( int xPos, int yPos, std::string textureText, color * textColor, int hAlign,int vAlign, int rendWid, int rendHeight, int renderAlpha )
    {
    }

    void font_base::render_text_boxed( int xPos, int yPos, std::string textureText, color * textColor,color * boxColor,int hAlign,int vAlign, int renderAlpha )
    {
    }

    void font_base::render_text_rotated( int xPos, int yPos, std::string textureText, color * textColor, float textAngle, int renderAlpha )
    {
    }

    bool font_base::render_text_special( int xPos, int yPos, std::string textureText, color * textColor, int hAlign,int vAlign, float renderAngle , float renderScale, int renderAlpha )
    {
        return false;
    }

    font_system_controller::font_system_controller()
    {
        newFontIdNumber = 0;
        fontTotalCount = 0;
        lastFontUsed = NULL;
    }

    font_system_controller::~font_system_controller()
    {

    }

    font_base * font_system_controller::open_font(std::string fontLocation, int fontSize, bool isMonospaced, std::string fNickName, int dynamicId )
    {
        if( dynamicId < 0)
        {
            dynamicId = newFontIdNumber;
        }
        if( font_template != NULL )
        {
            font_base *  returnVal = font_template->create_new( fontLocation.c_str(),fontSize, isMonospaced, fNickName,dynamicId );
            loadedFonts.push_back(returnVal);
            newFontIdNumber++;
            fontTotalCount++;
            return returnVal;
        }
        return NULL;
    }

    void font_system_controller::close_font(font_base * fontIn)
    {
        if( fontIn!=NULL)
        {
            font_base * cFont = NULL;
            for( int i = (int)loadedFonts.size()-1; i >=0; i--)
            {
                cFont = loadedFonts[i];
                if( cFont!=NULL)
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
            fontIn = NULL;
        }
    }

    void font_system_controller::clear_font_cache()
    {
        gpe::error_log->report("Clearing Font Cache...");
        font_base * cFont = NULL;
        for( int i = (int)loadedFonts.size()-1; i >=0; i--)
        {
            cFont = loadedFonts[i];
            if( cFont!=NULL)
            {
                cFont->clear_cache();
            }
        }
        gpe::error_log->report("Font Cache total: ["+ stg_ex::int_to_string(get_font_cache_size() )+"]...");
    }

    int font_system_controller::get_font_cache_size(bool recordCache)
    {
        font_base * cFont = NULL;
        int cacheCount = 0;
        int i = 0;
        int foundCache = 0;
        if( recordCache)
        {
            gpe::error_log->report("\nCalculating Font Cache...");
            for( i = (int)loadedFonts.size()-1; i >=0; i--)
            {
                cFont = loadedFonts[i];
                if( cFont!=NULL)
                {
                    foundCache =cFont->get_cache_count();
                    cacheCount+=foundCache;
                    gpe::error_log->report("      Font["+cFont->fontNickName+"] Cache total: ["+ stg_ex::int_to_string(foundCache )+"]...");
                }
            }
            gpe::error_log->report("Font Cache total: ["+ stg_ex::int_to_string(cacheCount )+"]...");
        }
        else
        {
            for( i = (int)loadedFonts.size()-1; i >=0; i--)
            {
                cFont = loadedFonts[i];
                if( cFont!=NULL)
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

    bool font_system_controller::render_bitmap_text( int xPos, int yPos, std::string textureText, color * textColor, font_base * textFont, int hAlign, int vAlign, int renderAlpha)
    {
        if( textFont==NULL)
        {
            textFont = lastFontUsed;
        }
        if( textFont!=NULL)
        {
            textFont->render_bitmapped_text( xPos,yPos,textureText,textColor,hAlign,vAlign,renderAlpha);
            return true;
        }
        else
        {
            gpe::error_log->report( "Unable to render number ("+ textureText + ")");
        }
        return false;
    }

    void font_system_controller::render_fps( font_base * textFont, color * color,  int xPos, int yPos, int hAlign , int vAlign  )
    {
        //if( fps_show_counter )
        {
            if( color == NULL )
            {
                color = c_lime;
            }

            if( window_controller_main->is_minimized() )
            {
                color =  c_red;
            }
            if( xPos < 0 )
            {
                xPos = gpe::screen_width;
            }
            if( yPos < 0 )
            {
                yPos = 0;
            }
            render_text( xPos, yPos, stg_ex::float_to_string(fps_current)+" / "+ stg_ex::float_to_string(fps_cap)+" fps | "+ stg_ex::float_to_string( delta_current )+" / " + stg_ex::float_to_string( time_keeper->get_needed_ticks() ) + " ms ",color,textFont,hAlign, vAlign );

        }
    }

    bool font_system_controller::render_only_text( int xPos, int yPos, std::string textureText, color * textColor, font_base * textFont, int hAlign, int vAlign, int renderAlpha)
    {
        if( textFont==NULL)
        {
            textFont = lastFontUsed;
        }
        if( (int)textureText.size() > 0)
        {
            if( textFont!=NULL)
            {
                textFont->render_text( xPos,yPos,textureText,textColor,hAlign,vAlign,renderAlpha);
                return true;
            }
        }
        return false;
    }

    bool font_system_controller::render_text( int xPos, int yPos, std::string textureText, color * textColor, font_base * textFont, int hAlign, int vAlign, int renderAlpha)
    {
        if( textFont==NULL)
        {
            textFont = lastFontUsed;
        }
        if( (int)textureText.size() > 0 && textFont!=NULL )
        {
            textFont->render_text( xPos,yPos,textureText,textColor,hAlign,vAlign,renderAlpha);
            return true;
        }
        else if( textFont!=NULL )
        {
            // std::cout << "Unable to render text (" << textureText << ").\n";
        }
        return false;
    }

    bool font_system_controller::render_text_boxed( int xPos, int yPos, std::string textureText, color * textColor,color * boxColor, font_base * textFont, int hAlign,int vAlign, int renderAlpha)
    {
        if( textFont==NULL)
        {
            textFont = lastFontUsed;
        }
        if( textFont!=NULL)
        {
            textFont->render_text_boxed( xPos,yPos,textureText,textColor,boxColor,hAlign,vAlign,renderAlpha);
            return true;
        }
        return false;
    }

    bool font_system_controller::render_text_resized( int xPos, int yPos, std::string textureText, color * textColor, font_base * textFont, int hAlign, int vAlign, int rendWid, int rendHeight, int renderAlpha )
    {
        if( textFont==NULL)
        {
            textFont = lastFontUsed;
        }
        if( textFont!=NULL)
        {
            textFont->render_text_resized( xPos,yPos,textureText,textColor,hAlign,vAlign,rendWid,rendHeight,renderAlpha);
            return true;
        }
        else if( textFont!=NULL )
        {
            std::cout << "Unable to render EXT text (" << textureText << ").\n";
        }
        return false;
    }

    bool font_system_controller::render_text_rotated( int xPos, int yPos, std::string textureText, color * textColor, font_base * textFont, float textAngle, int renderAlpha )
    {
        if( textFont==NULL)
        {
            textFont = lastFontUsed;
        }
        if( textFont!=NULL)
        {
            textFont->render_text_rotated( xPos,yPos,textureText,textColor,textAngle,renderAlpha);
            return true;
        }
        return false;
    }

    bool font_system_controller::render_text_scaled( int xPos, int yPos, std::string textureText,color * textColor, float scaleValue, font_base * textFont, int hAlign,int vAlign, int renderAlpha )
    {
        if( textFont!=NULL)
        {
            textFont->render_text_scaled( xPos, yPos, textureText, textColor, scaleValue, hAlign, vAlign, renderAlpha );
        }
        return false;
    }

    bool font_system_controller::render_text_special( int xPos, int yPos, std::string textureText, color * textColor, font_base * textFont, int hAlign,int vAlign, float renderAngle, float renderScale, int renderAlpha )
    {
        if( textFont!=NULL)
        {
            return textFont->render_text_special( xPos, yPos, textureText, textColor, hAlign, vAlign, renderAngle, renderScale, renderAlpha );
        }
        return false;
    }


    bool init_font_system()
    {
        //If we already started the font system do nothing.
        if( gfs!=NULL )
        {
            return true;
        }

        if( gfs==NULL )
        {
            gfs = new font_system_controller();
        }
        return true;
    }

    void quit_font_system()
    {
        if( gfs!=NULL )
        {
            delete gfs;
            gfs = NULL;
        }
        gpe::error_log->report("font_System Quit...");
    }

}
