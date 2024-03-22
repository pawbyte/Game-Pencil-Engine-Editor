/*
gpe_font_sdl2.cpp
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

-Game Pencil Engine  - SDL2 Module <https://gamepencil.net/sdl2-module/>


*/


#include "gpe_font_sdl2.h"

namespace gpe
{
    font_pair_sdl2::font_pair_sdl2(TTF_Font * fontIn, const std::string& str_in)
    {
        lastAlphaRendered = 255;
        str = str_in;
        strTexture = NULL;
        text_width = 0;
        text_height = 0;
        if( fontIn!=NULL && (int)str_in.size() > 0)
        {
            //Render text surface
            SDL_Color sdl2_color_white = {255, 255, 255,255};
            SDL_Surface * textSurface = textSurface = TTF_RenderUTF8_Blended( fontIn, str_in.c_str(),sdl2_color_white );
            if( textSurface == NULL )
            {
                std::string ttfError = TTF_GetError();
                gpe::error_log->report( "Unable to render text surface! Error:  " + ttfError );
            }
            else
            {
                //Create texture from surface pixels
                text_width = textSurface->w;
                text_height = textSurface->h;
                strTexture = sdl2_surface_ex::create_texture_from_surface( renderer_main_sdl2->get_sdl2_renderer(), textSurface);
                if( strTexture != NULL )
                {
                    //Get image dimensions
                    SDL_QueryTexture(strTexture,NULL, NULL,&text_width, &text_height);
                    //SDL_SetTextureBlendMode(strTexture, SDL_BLENDMODE_ADD );
                }
                else
                {
                    std::string sdlError = SDL_GetError();
                    gpe::error_log->report( "Unable to create texture from rendered text! SDL Error: "+ sdlError  );
                }

                SDL_FreeSurface( textSurface );
                textSurface = NULL;
            }
        }
        else
        {
            gpe::error_log->report("Unable to pre-render text [" + str_in +"]");
        }
    }

    font_pair_sdl2::~font_pair_sdl2()
    {
        if( strTexture!=NULL)
        {
            SDL_DestroyTexture(strTexture);
            strTexture = NULL;
        }
    }

    SDL_Texture * font_pair_sdl2::get_texture()
    {
        return strTexture;
    }

    font_sdl2_tff::font_sdl2_tff( const std::string& file_loc, int f_size, bool make_monospaced, const std::string& f_nickname,  int id_number )
    {
        font_system_type = "sdl2_ttf";
        last_used_halign = fa_left;
        last_used_valign = fa_top;
        font_id = id_number;
        mono_width = 0;
        mono_height = 0;
        number_width = 0;
        number_height = 0;
        font_is_monospaced = make_monospaced;
        heldSDLFont = NULL;
        font_family_name = "";
        font_nickname = f_nickname;



        if( (int)file_loc.size() > 0 )
        {
            if( !sff_ex::file_exists( file_loc) )
            {
                gpe::error_log->report("SDL-TTF-Font-Error: Unable to locate font file for ["+f_nickname+"] at [" + file_loc + "]"+stg_ex::int_to_string(f_size)+" pt size [Monospaced="+stg_ex::int_to_string(make_monospaced)+"]!!" );
            }
            else
            {
                gpe::error_log->report("SDL-TTF-Font-Update: Attempting to load font ["+f_nickname+"]from ["+file_loc +"] file "+stg_ex::int_to_string(f_size)+" pt size [Monospaced="+stg_ex::int_to_string(make_monospaced)+"]!!" );
            }
        }
        else
        {
            gpe::error_log->report("SDL-TTF-Font-Update: Attempting to load font [BLANK] from ["+file_loc +"] file "+stg_ex::int_to_string(f_size)+" pt size [Monospaced="+stg_ex::int_to_string(make_monospaced)+"]!!" );

        }
        heldSDLFont = TTF_OpenFontIndex(file_loc.c_str(), f_size,0);


        int i = 0;
        if( heldSDLFont!=NULL)
        {
            font_family_name = TTF_FontFaceFamilyName(heldSDLFont);
            font_family_name+=" "+ stg_ex::int_to_string(f_size)+"pt";
            std::string longCharstring = "";
            for( i = 0; i < 256; i++)
            {
                longCharstring+=i;
            }

            if( font_is_monospaced )
            {

                TTF_SetFontHinting(heldSDLFont,TTF_HINTING_NONE );
            }
            else
            {
                TTF_SetFontHinting(heldSDLFont,TTF_HINTING_NORMAL );
            }

            font_pair_sdl2 * monoSpacedPair = find_texture_sdl2("W");
            if( monoSpacedPair!=NULL)
            {
                number_width = mono_width = monoSpacedPair->get_width();
                number_height = mono_height = monoSpacedPair->get_height();
            }

            for( i = 0; i < 10; i++)
            {
                characterPairs[ stg_ex::int_to_string(i)] =  new font_pair_sdl2(heldSDLFont, stg_ex::int_to_string(i) );
            }
            characterPairs["." ] = new font_pair_sdl2(heldSDLFont,"." );
            characterPairs["," ] =  new font_pair_sdl2(heldSDLFont,"," );
            characterPairs["-" ] =  new font_pair_sdl2(heldSDLFont,"-" );
            characterPairs["(" ] =  new font_pair_sdl2(heldSDLFont,"(" );
            characterPairs[")" ] = new font_pair_sdl2(heldSDLFont,")" );
            if( make_monospaced == false)
            {
                std::map<std::string,font_pair_sdl2 *>::iterator foundIterator;
                foundIterator = characterPairs.find("9");
                if( foundIterator!= characterPairs.end() )
                {
                    font_pair_sdl2 * tempfont_pair_sdl2 = foundIterator->second;
                    if( tempfont_pair_sdl2!=NULL )
                    {
                        if( tempfont_pair_sdl2->get_width() > number_width)
                        {
                            number_width = tempfont_pair_sdl2->get_width();
                        }
                        if( tempfont_pair_sdl2->get_height() > number_height)
                        {
                            number_height = tempfont_pair_sdl2->get_height();
                        }
                    }
                }
            }
            gpe::error_log->report("SDL-TTF-Font-Update: Font loaded ["+f_nickname+"]" );
        }
        else
        {
            gpe::error_log->report("SDL-TTF-Font-Error: Unable to load ["+f_nickname+"]from ["+file_loc +"] file!");
            for( i = 0; i < 10; i++)
            {
                characterPairs[ stg_ex::int_to_string(i)] =  nullptr;
            }
        }
    }

    font_sdl2_tff::~font_sdl2_tff()
    {
        if( heldSDLFont!=NULL)
        {
            TTF_CloseFont(heldSDLFont);
            heldSDLFont = NULL;
        }
        clear_cache();
        font_pair_sdl2 * tempfont_pair_sdl2 = NULL;
        for (std::map<std::string,font_pair_sdl2 *>::iterator it=characterPairs.begin(); it!=characterPairs.end(); ++it)
        {
            tempfont_pair_sdl2 = it->second;
            if( tempfont_pair_sdl2!=NULL)
            {
                delete tempfont_pair_sdl2;
                tempfont_pair_sdl2 = NULL;
            }
        }
        characterPairs.clear();

    }

    void font_sdl2_tff::clear_cache()
    {
        font_pair_sdl2 * tempfont_pair_sdl2 = NULL;
        for (std::map<std::string,font_pair_sdl2 *>::iterator it=textPairs.begin(); it!=textPairs.end(); ++it)
        {
            tempfont_pair_sdl2 = it->second;
            if( tempfont_pair_sdl2!=NULL)
            {
                delete tempfont_pair_sdl2;
                tempfont_pair_sdl2 = NULL;
            }
        }
        textPairs.clear();
    }

    font_base * font_sdl2_tff::create_new(  const std::string& file_loc, int f_size, bool make_monospaced , const std::string& f_nickname , int id_number )
    {
        return new font_sdl2_tff( file_loc, f_size, make_monospaced, f_nickname, id_number );
    }

    int font_sdl2_tff::get_cache_count()
    {
        return (int)textPairs.size();
    }

    int font_sdl2_tff::get_font_id()
    {
        return font_id;
    }

    void font_sdl2_tff::get_metrics( const std::string& text_to_render, int * width_value, int * height_value)
    {
        int tSize = (int)text_to_render.size();
        if( tSize > 0)
        {
            if( font_is_monospaced)
            {
                *width_value = mono_width*tSize;
                *height_value = mono_height;
            }
            else if(heldSDLFont!=NULL)
            {
                font_pair_sdl2 *  newFoundPair = find_texture_sdl2( text_to_render);
                if( newFoundPair!=NULL)
                {
                    *width_value = newFoundPair->get_width();
                    *height_value = newFoundPair->get_height();
                }

            }
        }
        else
        {
            *width_value = 0;
            *height_value = 0;
        }
    }

    void font_sdl2_tff::get_numbered_metrics( const std::string& text_to_render, int * width_value, int * height_value)
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

    void font_sdl2_tff::get_wrapped_string_metrics( const std::string& str_in, int line_width, int linePadding, int * width_value, int *height_value )
    {
        if( (int)str_in.size() == 0 || line_width <=0 )
        {
            return;
        }
        std::vector < std::string > wrappedTextLine;
        stg_ex::wrap_string( str_in, wrappedTextLine, line_width);
        if( (int)wrappedTextLine.size() == 0  )
        {
            *width_value = 0;
            *height_value = 0;
        }
        else if( (int)wrappedTextLine.size() == 1 )
        {

        }
        else
        {

        }
    }

    TTF_Font * font_sdl2_tff::get_sdl2_font()
    {
        return heldSDLFont;
    }

    font_pair_base * font_sdl2_tff::find_character_texture( const std::string& id_number )
    {
        return find_character_texture_sdl2( id_number );
    }

    font_pair_sdl2 * font_sdl2_tff::find_character_texture_sdl2( const std::string& id_number )
    {
        font_pair_sdl2 * fPair = NULL;
        SDL_Texture * fSDLTexture = NULL;
        std::map<std::string,font_pair_sdl2 *>::iterator foundVal;
        foundVal = characterPairs.find( id_number );
        if( foundVal !=  characterPairs.end() )
        {
            fPair = foundVal->second;
            if( fPair!=NULL && fPair->texture_matches(id_number) )
            {
                return fPair;
            }
        }
        else
        {
            if( id_number!=" ")
            {
                fPair = new font_pair_sdl2(heldSDLFont,id_number);
                characterPairs[id_number] = fPair;
                return fPair;
            }
        }
        return NULL;
    }

    font_pair_base * font_sdl2_tff::find_texture( const std::string& text_to_render)
    {
        return find_texture_sdl2( text_to_render );
    }

    font_pair_sdl2 * font_sdl2_tff::find_texture_sdl2( const std::string& text_to_render)
    {
        font_pair_sdl2 * fPair = NULL;
        SDL_Texture * fSDLTexture = NULL;
        std::map< const std::string,font_pair_sdl2 *>::iterator foundVal;
        foundVal = textPairs.find( text_to_render );
        if( foundVal !=  textPairs.end() )
        {
            return foundVal->second;
        }
        else if( text_to_render!=" ")
        {
            fPair = new font_pair_sdl2(heldSDLFont,text_to_render);
            textPairs[text_to_render] = fPair;
            return fPair;
        }
        return NULL;
    }

    void font_sdl2_tff::render_bitmapped_text( int x_pos, int y_pos, const std::string& number_to_render, color * text_color, int alignment_h,int alignment_v, int render_alpha )
    {
        int numberSize = (int)number_to_render.size();
        if( render_alpha > 0 && numberSize >0 )
        {
            switch(alignment_h)
            {
            case gpe::fa_center:
                x_pos=x_pos-(number_width*numberSize)/2;
                //last_used_halign=gpe::fa_center;
                break;

            case gpe::fa_right:
                x_pos=x_pos-(number_width*numberSize);
                // last_used_halign=gpe::fa_right;
                break;

            //rendering left will be the default
            default:
                // last_used_halign=gpe::fa_left;
                break;
            }
            switch(alignment_v)
            {
            case gpe::fa_middle:
                y_pos=y_pos-number_height/2;
                // last_used_valign=gpe::fa_middle;
                break;

            case gpe::fa_bottom:
                y_pos=y_pos-number_height;
                // last_used_valign=gpe::fa_middle;
                break;

            //rendering left will be the default
            default:
                //last_used_valign=gpe::fa_top;
                break;
            }
            font_pair_sdl2 * strTex = NULL;
            SDL_Texture * fPairTex = NULL;
            int i = 0;
            std::string currCharacter = "";
            for( i = 0; i < numberSize; i++)
            {
                currCharacter = stg_ex::get_substring(number_to_render,i,1);
                strTex = find_character_texture_sdl2( currCharacter );
                if( strTex!=NULL)
                {
                    fPairTex = strTex->get_texture();
                    if( render_alpha > 255)
                    {
                        render_alpha = 255;
                    }
                    if( fPairTex!=NULL && render_alpha >0)
                    {
                        int tex_width = 0;
                        int tex_height =0;
                        SDL_SetTextureColorMod( fPairTex, text_color->get_r(), text_color->get_g(), text_color->get_b() );

                        if( strTex->lastAlphaRendered!=render_alpha )
                        {
                            SDL_SetTextureAlphaMod(fPairTex,render_alpha);
                            strTex->lastAlphaRendered = render_alpha;
                        }
                        //Get image dimensions
                        tex_width = strTex->get_width();
                        tex_height = strTex->get_height();

                        if(alignment_h<gpe::fa_left || alignment_h<0)
                        {
                            alignment_h=last_used_halign;
                        }
                        if(alignment_v <gpe::fa_top || alignment_v<0)
                        {
                            alignment_v=last_used_valign;
                        }
                        SDL_Rect clip = {x_pos, y_pos, tex_width,tex_height};
                        SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),fPairTex, NULL, &clip);
                        SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                    }
                }
                x_pos+=number_width;
            }
        }
    }

    void font_sdl2_tff::render_text( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, int alignment_h,int alignment_v,int render_alpha )
    {
        if( render_alpha > 0 && text_color!=NULL)
        {
            //SDL_SetRenderDrawColor( renderer_main_sdl2->get_sdl2_renderer(), text_color->get_r(), text_color->get_g(), text_color->get_b(), 255 );
            font_pair_sdl2 * strTex = find_texture_sdl2(text_to_render);
            SDL_Texture * fPairTex=  NULL;
            if( strTex!=NULL)
            {
                fPairTex = strTex->get_texture();

                if( fPairTex!=NULL && render_alpha >0)
                {
                    if( render_alpha >= 255)
                    {
                        render_alpha = 255;
                    }
                    int tex_width = 0;
                    int tex_height = 0;
                    SDL_SetTextureColorMod( fPairTex, text_color->get_r(), text_color->get_g(), text_color->get_b() );

                    if( strTex->lastAlphaRendered!=render_alpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,render_alpha);
                        strTex->lastAlphaRendered = render_alpha;
                    }
                    //Get image dimensions
                    tex_width = strTex->get_width();
                    tex_height = strTex->get_height();

                    if(alignment_h<gpe::fa_left || alignment_h > gpe::fa_right )
                    {
                        alignment_h=last_used_halign;
                    }

                    if(alignment_v < gpe::fa_top || alignment_v > gpe::fa_bottom )
                    {
                        alignment_v=last_used_valign;
                    }
                    switch(alignment_h)
                    {
                    case gpe::fa_center:
                        x_pos=x_pos-tex_width/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        x_pos=x_pos-tex_width;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(alignment_v)
                    {
                        case gpe::fa_middle:
                            y_pos=y_pos-tex_height/2;
                            // last_used_valign=gpe::fa_middle;
                        break;

                        case gpe::fa_bottom:
                            y_pos=y_pos-tex_height;
                            // last_used_valign=gpe::fa_middle;
                        break;

                        //rendering left will be the default
                        default:
                            //last_used_valign=gpe::fa_top;
                        break;
                        }
                    SDL_Rect clip = {x_pos, y_pos, tex_width,tex_height};
                    SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),fPairTex, NULL, &clip);
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
                else
                {
                    gpe::error_log->report("Unable to find sdl2_text for ["+text_to_render+"]...");
                }
            }
            else
            {
                gpe::error_log->report("Unable to render ["+text_to_render+"]...");
            }
        }
        else
        {
            gpe::error_log->report("Unable to render text  ["+text_to_render+"]...");
        }
    }

    void font_sdl2_tff::render_text_scaled( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, float text_scale, int alignment_h,int alignment_v, int render_alpha )
    {

    }


    void font_sdl2_tff::render_text_boxed( int x_pos, int y_pos, const std::string& text_to_render, color * text_color,color * boxColor,int alignment_h,int alignment_v, int render_alpha )
    {
        if( render_alpha > 0)
        {
            font_pair_sdl2 * strTex = find_texture_sdl2(text_to_render);
            SDL_Texture * fPairTex=  NULL;
            if( strTex!=NULL)
            {
                fPairTex = strTex->get_texture();
                if( render_alpha > 255)
                {
                    render_alpha = 255;
                }
                if( fPairTex!=NULL && render_alpha >0)
                {
                    int tex_width = 0;
                    int tex_height =0;
                    SDL_SetTextureColorMod( fPairTex, text_color->get_r(), text_color->get_g(), text_color->get_b() );

                    if( strTex->lastAlphaRendered!=render_alpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,render_alpha);
                        strTex->lastAlphaRendered = render_alpha;
                    }
                    //Get image dimensions
                    tex_width = strTex->get_width();
                    tex_height = strTex->get_height();

                    if(alignment_h<gpe::fa_left || alignment_h > gpe::fa_right )
                    {
                        alignment_h=last_used_halign;
                    }

                    if(alignment_v < gpe::fa_top || alignment_v > gpe::fa_bottom )
                    {
                        alignment_v=last_used_valign;
                    }
                    switch(alignment_h)
                    {
                    case gpe::fa_center:
                        x_pos=x_pos-tex_width/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        x_pos=x_pos-tex_width;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(alignment_v)
                    {
                    case gpe::fa_middle:
                        y_pos=y_pos-tex_height/2;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    case gpe::fa_bottom:
                        y_pos=y_pos-tex_height;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    //rendering left will be the default
                    default:
                        //last_used_valign=gpe::fa_top;
                        break;
                    }
                    gpe::shape_rect gpeClip;
                    gpeClip.x = x_pos;
                    gpeClip.y = y_pos;
                    gpeClip.w = tex_width;
                    gpeClip.h = tex_height;
                    SDL_Rect sdlClip = {x_pos, y_pos, tex_width,tex_height};
                    gpe::gcanvas->render_rect(  &gpeClip,boxColor,false,render_alpha );
                    SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),fPairTex, NULL, &sdlClip);
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
        }
    }

    void font_sdl2_tff::render_text_clipped( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, int alignment_h,int alignment_v, int render_width, int render_height, int render_alpha )
    {
        if( render_alpha > 0)
        {
            font_pair_sdl2 * strTex = find_texture_sdl2(text_to_render);
            if( strTex!=NULL)
            {
                SDL_Texture * fPairTex = strTex->get_texture();
                if( fPairTex!=NULL)
                {
                    int tex_width = 0;
                    int tex_height =0;
                    SDL_SetTextureColorMod( fPairTex, text_color->get_r(), text_color->get_g(), text_color->get_b() );
                    if( strTex->lastAlphaRendered!=render_alpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,render_alpha);
                        strTex->lastAlphaRendered = render_alpha;
                    }
                    //Get image dimensions
                    tex_width = strTex->get_width();
                    tex_height = strTex->get_height();

                    //Get image dimensions
                    if( render_width < 0  || render_width > strTex->get_width())
                    {
                        tex_width = strTex->get_width();
                    }
                    else
                    {
                        tex_width = render_width;
                    }

                    if( render_height< 0 || render_height > strTex->get_height() )
                    {
                        tex_height = strTex->get_height();
                    }
                    else
                    {
                        tex_height = render_height;
                    }

                    if(alignment_h<gpe::fa_left || alignment_h > gpe::fa_right )
                    {
                        alignment_h=last_used_halign;
                    }

                    if(alignment_v < gpe::fa_top || alignment_v > gpe::fa_bottom )
                    {
                        alignment_v=last_used_valign;
                    }
                    switch(alignment_h)
                    {
                    case gpe::fa_center:
                        x_pos=x_pos-tex_width/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        x_pos=x_pos-tex_width;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(alignment_v)
                    {
                    case gpe::fa_middle:
                        y_pos=y_pos-tex_height/2;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    case gpe::fa_bottom:
                        y_pos=y_pos-tex_height;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    //rendering left will be the default
                    default:
                        //last_used_valign=gpe::fa_top;
                        break;
                    }

                    SDL_Rect clip = {0, 0, tex_width, tex_height};
                    SDL_Rect render_rect = {x_pos, y_pos, tex_width,tex_height};
                    SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),fPairTex, &clip, &render_rect );
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
        }
    }


    void font_sdl2_tff::render_text_resized( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, int alignment_h,int alignment_v, int render_width, int render_height, int render_alpha )
    {
        if( render_alpha > 0)
        {
            font_pair_sdl2 * strTex = find_texture_sdl2(text_to_render);
            if( strTex!=NULL)
            {
                SDL_Texture * fPairTex = strTex->get_texture();
                if( fPairTex!=NULL)
                {
                    int tex_width = 0;
                    int tex_height =0;
                    SDL_SetTextureColorMod( fPairTex, text_color->get_r(), text_color->get_g(), text_color->get_b() );
                    if( strTex->lastAlphaRendered!=render_alpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,render_alpha);
                        strTex->lastAlphaRendered = render_alpha;
                    }
                    //Get image dimensions
                    tex_width = strTex->get_width();
                    tex_height = strTex->get_height();

                    //Get image dimensions
                    if( render_width < 0 )
                    {
                        render_width = tex_width;
                    }

                    if( render_height < 0 )
                    {
                        render_height = tex_height;
                    }


                    if(alignment_h<gpe::fa_left || alignment_h > gpe::fa_right )
                    {
                        alignment_h=last_used_halign;
                    }

                    if(alignment_v < gpe::fa_top || alignment_v > gpe::fa_bottom )
                    {
                        alignment_v=last_used_valign;
                    }
                    switch(alignment_h)
                    {
                    case gpe::fa_center:
                        x_pos=x_pos-tex_width/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        x_pos=x_pos-tex_width;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(alignment_v)
                    {
                    case gpe::fa_middle:
                        y_pos=y_pos-tex_height/2;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    case gpe::fa_bottom:
                        y_pos=y_pos-tex_height;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    //rendering left will be the default
                    default:
                        //last_used_valign=gpe::fa_top;
                        break;
                    }

                    if( render_width != 0 && render_height != 0 )
                    {
                        SDL_Rect clip = {0, 0, tex_width, tex_height};
                        SDL_Rect render_rect = {x_pos, y_pos, render_width,render_height};
                        SDL_RenderCopy( renderer_main_sdl2->get_sdl2_renderer(),fPairTex, &clip, &render_rect);
                        SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                    }
                }
            }
        }
    }

    void font_sdl2_tff::render_text_rotated( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, float textAngle, int render_alpha )
    {
        if( render_alpha > 0)
        {
            font_pair_sdl2 * strTex = find_texture_sdl2(text_to_render);
            SDL_Texture * fPairTex=  NULL;
            if( strTex!=NULL)
            {
                fPairTex = strTex->get_texture();
                if( render_alpha > 255)
                {
                    render_alpha = 255;
                }
                if( fPairTex!=NULL && render_alpha >0)
                {
                    int tex_width = 0;
                    int tex_height =0;
                    SDL_SetTextureColorMod( fPairTex, text_color->get_r(), text_color->get_g(), text_color->get_b() );

                    if( strTex->lastAlphaRendered!=render_alpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,render_alpha);
                        strTex->lastAlphaRendered = render_alpha;
                    }
                    //Get image dimensions
                    tex_width = strTex->get_width();
                    tex_height = strTex->get_height();

                    SDL_Rect sdlDstrect = {x_pos-tex_width, y_pos-tex_height, tex_width,tex_height};
                    SDL_Point sdlCenterPoint = {tex_width, tex_height};
                    SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),fPairTex, NULL,&sdlDstrect,textAngle,&sdlCenterPoint, SDL_FLIP_NONE );
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
        }
    }

    bool font_sdl2_tff::render_text_special( int x_pos, int y_pos, const std::string& text_to_render, color * text_color, int alignment_h,int alignment_v, float render_angle , float render_scale, int render_alpha )
    {
        if( render_alpha > 0)
        {
            font_pair_sdl2 * strTex = find_texture_sdl2(text_to_render);
            if( strTex!=NULL)
            {
                SDL_Texture * fPairTex = strTex->get_texture();
                if( fPairTex!=NULL)
                {
                    int tex_width = 0;
                    int tex_height =0;
                    SDL_SetTextureColorMod( fPairTex, text_color->get_r(), text_color->get_g(), text_color->get_b() );
                    if( strTex->lastAlphaRendered!=render_alpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,render_alpha);
                        strTex->lastAlphaRendered = render_alpha;
                    }
                    //Get image dimensions
                    tex_width = (float)strTex->get_width() * render_scale;
                    tex_height = (float)strTex->get_height() * render_scale;

                    //Get image dimensions
                    if( tex_width < 0)
                    {
                        tex_width *=-1;
                        render_angle *=-1;
                    }

                    if( tex_height<0 )
                    {
                        tex_height *=-1;
                        render_angle *=-1;
                    }

                    if(alignment_h<gpe::fa_left || alignment_h > gpe::fa_right )
                    {
                        alignment_h=last_used_halign;
                    }

                    if(alignment_v < gpe::fa_top || alignment_v > gpe::fa_bottom )
                    {
                        alignment_v=last_used_valign;
                    }
                    switch(alignment_h)
                    {
                    case gpe::fa_center:
                        x_pos=x_pos-tex_width/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        x_pos=x_pos-tex_width;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(alignment_v)
                    {
                    case gpe::fa_middle:
                        y_pos=y_pos-tex_height/2;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    case gpe::fa_bottom:
                        y_pos=y_pos-tex_height;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    //rendering left will be the default
                    default:
                        //last_used_valign=gpe::fa_top;
                        break;
                    }

                    SDL_Rect render_rect = {x_pos, y_pos, tex_width,tex_height};
                    SDL_RenderCopyEx( renderer_main_sdl2->get_sdl2_renderer(),fPairTex, NULL,  &render_rect, -render_angle,  NULL, SDL_FLIP_NONE );
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                    return true;
                }
            }
        }
        return false;
    }

    bool init_sdl2_font_system()
    {
        //If we already started the font system do nothing.
        if( gfs!=NULL )
        {
            delete gfs;
            gfs = NULL;
        }
        //Initialize SDL_ttf
        gpe::error_log->report("Starting SDL_TFF font system...");
        if( TTF_Init() == -1 )
        {
            gpe::error_log->report("        Error loading SDL_TTF");
            return false;
        }

        gfs = new font_system_controller();
        gfs->font_template = new font_sdl2_tff("",0,false,"",-1 );
        gpe::error_log->report("SDL_TFF font system started...");
        return true;
    }

    void quit_sdl2_font_system()
    {
        if( gfs!=NULL )
        {
            delete gfs;
            gfs = NULL;
        }
        gpe::error_log->report("Quitting SDL_TFF....");
        gpe::error_log->report("font_System Quit...");
        TTF_Quit();
    }
}
