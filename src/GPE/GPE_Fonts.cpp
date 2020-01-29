/*
GPE_Fonts.cpp
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

#include "GPE_Artist_Base.h"
#include "GPE_Fonts.h"
#include "GPE_Engine.h"

GPE_Font * GPE_DEFAULT_FONT = NULL;
GPE_Font_Controller * GFS = NULL;
GPE_Font_Controller * gfs = NULL;

fontPair::fontPair(TTF_Font * fontIn,std::string strIn)
{
    lastAlphaRendered = 255;
    str = strIn;
    strTexture = NULL;
    textWidth = 0;
    textHeight = 0;
    if( fontIn!=NULL && (int)strIn.size() > 0)
    {
        //Render text surface
        SDL_Surface * textSurface = NULL;
        textSurface = TTF_RenderUTF8_Blended( fontIn, strIn.c_str(),c_white->get_sdl_color() );
        if( textSurface == NULL )
        {
            std::string ttfError = TTF_GetError();
            GPE_Report( "Unable to render text surface! Error:  " + ttfError );
        }
        else
        {
            //Create texture from surface pixels
            strTexture = SDL_SurfaceEx::create_texture_from_surface( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(), textSurface );
            if( strTexture != NULL )
            {
                //Get image dimensions
                textWidth = textSurface->w;
                textHeight = textSurface->h;
                SDL_QueryTexture(strTexture,NULL, NULL,&textWidth, &textHeight);
                SDL_FreeSurface(textSurface);
                //SDL_SetTextureBlendMode(strTexture, SDL_BLENDMODE_ADD );
            }
            else
            {
                std::string sdlError = SDL_GetError();
                GPE_Report( "Unable to create texture from rendered text! SDL Error: "+ sdlError  );
            }
        }
    }
}

fontPair::~fontPair()
{
    if( strTexture!=NULL)
    {
        SDL_DestroyTexture(strTexture);
        strTexture = NULL;
    }
}

bool fontPair::texture_matches(std::string strCheck)
{
    if( str==strCheck)
    {
        return true;
    }
    return false;
}

SDL_Texture * fontPair::get_texture()
{
    return strTexture;
}

int fontPair::get_width()
{
    return textWidth;
}

int fontPair::get_height()
{
    return textHeight;
}

GPE_Font::GPE_Font(std::string fFileLocation, int fSize, bool isMonospaced, std::string fNickName,  int fontIdNumb )
{
    customFontId = fontIdNumb;
    monoWidth = 0;
    monoHeight = 0;
    numberWidth = 0;
    numberHeight = 0;
    fontIsMonoSpaced = isMonospaced;
    if( !file_exists( fFileLocation) )
    {
        GPE_Report("Font-Error: Unable to located [" + fFileLocation + "]!" );
    }
    else
    {
        GPE_Report("Font-Update: Attempting to load font from ["+fFileLocation +"] file!" );
    }
    heldSDLFont = TTF_OpenFontIndex(fFileLocation.c_str(), fSize,0);
    fontFamilyName = "";
    fontNickName = fNickName;
    int i = 0;
    if( heldSDLFont!=NULL)
    {
        fontFamilyName = TTF_FontFaceFamilyName(heldSDLFont);
        fontFamilyName+=" "+int_to_string(fSize)+"pt";
        std::string longCharString = "";
        for( i = 0; i < 256; i++)
        {
            longCharString+=i;
        }

        if( isMonospaced)
        {
            TTF_SetFontHinting(heldSDLFont,TTF_HINTING_MONO);
        }
        else
        {
            TTF_SetFontHinting(heldSDLFont,TTF_HINTING_LIGHT );
        }

        fontPair * monoSpacedPair = find_texture("W");
        if( monoSpacedPair!=NULL)
        {
            numberWidth = monoWidth = monoSpacedPair->get_width();
            numberHeight = monoHeight = monoSpacedPair->get_height();
        }

        for( i = 0; i < 10; i++)
        {
            characterPairs[ int_to_string(i)] =  new fontPair(heldSDLFont,int_to_string(i) );
        }
        characterPairs["." ] = new fontPair(heldSDLFont,"." );
        characterPairs["," ] =  new fontPair(heldSDLFont,"," );
        characterPairs["-" ] =  new fontPair(heldSDLFont,"-" );
        characterPairs["(" ] =  new fontPair(heldSDLFont,"(" );
        characterPairs[")" ] = new fontPair(heldSDLFont,")" );
        if( isMonospaced == false)
        {
            std::map<std::string,fontPair *>::iterator foundIterator;
            foundIterator = characterPairs.find("9");
            if( foundIterator!= characterPairs.end() )
            {
                fontPair * tempFontPair = foundIterator->second;
                if( tempFontPair!=NULL )
                {
                    if( tempFontPair->get_width() > numberWidth)
                    {
                        numberWidth = tempFontPair->get_width();
                    }
                    if( tempFontPair->get_height() > numberHeight)
                    {
                        numberHeight = tempFontPair->get_height();
                    }
                }
            }
        }
    }
    else
    {
        GPE_Report("Font-Error: Unable to load [" + fFileLocation + "]!" );
    }
}

GPE_Font::~GPE_Font()
{
    if( heldSDLFont!=NULL)
    {
        TTF_CloseFont(heldSDLFont);
        heldSDLFont = NULL;
    }
    clear_cache();
    fontPair * tempFontPair = NULL;
    for (std::map<std::string,fontPair *>::iterator it=characterPairs.begin(); it!=characterPairs.end(); ++it)
    {
        tempFontPair = it->second;
        if( tempFontPair!=NULL)
        {
            delete tempFontPair;
            tempFontPair = NULL;
        }
    }
    characterPairs.clear();

}

void GPE_Font::clear_cache()
{
    fontPair * tempFontPair = NULL;
    for (std::map<std::string,fontPair *>::iterator it=textPairs.begin(); it!=textPairs.end(); ++it)
    {
        tempFontPair = it->second;
        if( tempFontPair!=NULL)
        {
            delete tempFontPair;
            tempFontPair = NULL;
        }
    }
    textPairs.clear();
}

int GPE_Font::get_cache_count()
{
    return (int)textPairs.size();
}

int GPE_Font::get_font_id()
{
    return customFontId;
}

void GPE_Font::get_metrics(std::string textToRender, int * wVal, int * hVal)
{
    int tSize = (int)textToRender.size();
    if( tSize > 0)
    {
        if( fontIsMonoSpaced)
        {
            *wVal = monoWidth*tSize;
            *hVal = monoHeight;
        }
        else if(heldSDLFont!=NULL)
        {
            fontPair *  newFoundPair = find_texture( textToRender);
            if( newFoundPair!=NULL)
            {
                *wVal = newFoundPair->get_width();
                *hVal = newFoundPair->get_height();
            }

        }
    }
    else
    {
        *wVal = 0;
        *hVal = 0;
    }
}

void GPE_Font::get_numbered_metrics(std::string textToRender, int * wVal, int * hVal)
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

int GPE_Font::get_mono_height()
{
    return monoHeight;
}

int GPE_Font::get_mono_width()
{
    return monoWidth;
}

std::string GPE_Font::get_family_name()
{
    return fontFamilyName;
}

TTF_Font * GPE_Font::get_sdl_font()
{
    return heldSDLFont;
}

fontPair * GPE_Font::find_character_texture( std::string numbId)
{
    fontPair * fPair = NULL;
    SDL_Texture * fSDLTexture = NULL;
    std::map<std::string,fontPair *>::iterator foundVal;
    foundVal = characterPairs.find( numbId );
    if( foundVal !=  characterPairs.end() )
    {
        fPair = foundVal->second;
        if( fPair!=NULL && fPair->texture_matches(numbId) )
        {
            return fPair;
        }
    }
    else
    {
        if( numbId!=" ")
        {
            fPair = new fontPair(heldSDLFont,numbId);
            characterPairs[numbId] = fPair;
            return fPair;
        }
    }
    return NULL;
}

fontPair * GPE_Font::find_texture( std::string textToRender)
{
    fontPair * fPair = NULL;
    SDL_Texture * fSDLTexture = NULL;
    std::map<std::string,fontPair *>::iterator foundVal;
    foundVal = textPairs.find( textToRender );
    if( foundVal !=  textPairs.end() )
    {
        fPair = foundVal->second;
        //if( fPair!=NULL && fPair->texture_matches( textToRender ) )
        {
            return fPair;
        }
    }
    else
    {
        if( textToRender!=" ")
        {
            fPair = new fontPair(heldSDLFont,textToRender);
            textPairs[textToRender] = fPair;
            return fPair;
        }
    }
    return NULL;
}

void GPE_Font::render_bitmapped_text( int xPos, int yPos, std::string numberToRender, GPE_Color * textColor, int hAlign,int vAlign, int renderAlpha )
{
    int numberSize = (int)numberToRender.size();
    if( renderAlpha > 0 && numberSize >0 )
    {
        switch(hAlign)
        {
        case FA_CENTER:
            xPos=xPos-(numberWidth*numberSize)/2;
            //LAST_USED_HALIGNMENT=FA_CENTER;
            break;

        case FA_RIGHT:
            xPos=xPos-(numberWidth*numberSize);
            // LAST_USED_HALIGNMENT=FA_RIGHT;
            break;

        //rendering left will be the default
        default:
            // LAST_USED_HALIGNMENT=FA_LEFT;
            break;
        }
        switch(vAlign)
        {
        case FA_MIDDLE:
            yPos=yPos-numberHeight/2;
            // LAST_USED_VALIGNMENT=FA_MIDDLE;
            break;

        case FA_BOTTOM:
            yPos=yPos-numberHeight;
            // LAST_USED_VALIGNMENT=FA_MIDDLE;
            break;

        //rendering left will be the default
        default:
            //LAST_USED_VALIGNMENT=FA_TOP;
            break;
        }
        fontPair * strTex = NULL;
        SDL_Texture * fPairTex = NULL;
        int i = 0;
        std::string currCharacter = "";
        for( i = 0; i < numberSize; i++)
        {
            currCharacter = get_substring(numberToRender,i,1);
            strTex = find_character_texture( currCharacter );
            if( strTex!=NULL)
            {
                fPairTex = strTex->get_texture();
                if( renderAlpha > 255)
                {
                    renderAlpha = 255;
                }
                if( fPairTex!=NULL && renderAlpha >0)
                {
                    int texWid = 0;
                    int texHeight =0;
                    SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                    if( strTex->lastAlphaRendered!=renderAlpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                        strTex->lastAlphaRendered = renderAlpha;
                    }
                    //Get image dimensions
                    texWid = strTex->get_width();
                    texHeight = strTex->get_height();

                    if(hAlign<FA_LEFT || hAlign<0)
                    {
                        hAlign=LAST_USED_HALIGNMENT;
                    }
                    if(vAlign <FA_TOP || vAlign<0)
                    {
                        vAlign=LAST_USED_VALIGNMENT;
                    }
                    SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                    SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),fPairTex, NULL, &clip);
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
            xPos+=numberWidth;
        }
    }
}

void GPE_Font::render_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign,int vAlign, int renderAlpha )
{
    if( renderAlpha > 0 && textColor!=NULL)
    {
        SDL_SetRenderDrawColor( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(), textColor->get_r(), textColor->get_g(), textColor->get_b(), 255 );
        fontPair * strTex = find_texture(textureText);
        SDL_Texture * fPairTex=  NULL;
        if( strTex!=NULL)
        {
            fPairTex = strTex->get_texture();

            if( fPairTex!=NULL && renderAlpha >0)
            {
                if( renderAlpha >= 255)
                {
                    renderAlpha = 255;
                }
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                if(hAlign<FA_LEFT || hAlign > FA_RIGHT )
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }

                if(vAlign < FA_TOP || vAlign > FA_BOTTOM )
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                case FA_CENTER:
                    xPos=xPos-texWid/2;
                    //LAST_USED_HALIGNMENT=FA_CENTER;
                    break;

                case FA_RIGHT:
                    xPos=xPos-texWid;
                    // LAST_USED_HALIGNMENT=FA_RIGHT;
                    break;

                //rendering left will be the default
                default:
                    // LAST_USED_HALIGNMENT=FA_LEFT;
                    break;
                }
                switch(vAlign)
                {
                case FA_MIDDLE:
                    yPos=yPos-texHeight/2;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                case FA_BOTTOM:
                    yPos=yPos-texHeight;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                //rendering left will be the default
                default:
                    //LAST_USED_VALIGNMENT=FA_TOP;
                    break;
                }
                SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),fPairTex, NULL, &clip);
                //SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );

            }
        }
    }
}

void GPE_Font::render_text_scaled( int xPos, int yPos, std::string textureText, GPE_Color * textColor, float textScale, int hAlign,int vAlign, int renderAlpha )
{

}

void GPE_Font::render_text_resized( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign,int vAlign, int rendWid, int rendHeight, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        if( strTex!=NULL)
        {
            SDL_Texture * fPairTex = strTex->get_texture();
            if( fPairTex!=NULL)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );
                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                //Get image dimensions
                if( rendWid<=0 || rendWid > strTex->get_width())
                {
                    texWid = strTex->get_width();
                }
                else
                {
                    texWid = rendWid;
                }

                if( rendHeight<=0 || rendHeight > strTex->get_height() )
                {
                    texHeight = strTex->get_height();
                }
                else
                {
                    texHeight = rendHeight;
                }

                if(hAlign<FA_LEFT || hAlign > FA_RIGHT )
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }

                if(vAlign < FA_TOP || vAlign > FA_BOTTOM )
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                case FA_CENTER:
                    xPos=xPos-texWid/2;
                    //LAST_USED_HALIGNMENT=FA_CENTER;
                    break;

                case FA_RIGHT:
                    xPos=xPos-texWid;
                    // LAST_USED_HALIGNMENT=FA_RIGHT;
                    break;

                //rendering left will be the default
                default:
                    // LAST_USED_HALIGNMENT=FA_LEFT;
                    break;
                }
                switch(vAlign)
                {
                case FA_MIDDLE:
                    yPos=yPos-texHeight/2;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                case FA_BOTTOM:
                    yPos=yPos-texHeight;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                //rendering left will be the default
                default:
                    //LAST_USED_VALIGNMENT=FA_TOP;
                    break;
                }

                SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                SDL_Rect renderRect = {0, 0, texWid, texHeight};
                SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),fPairTex, &renderRect, &clip);
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
            }
        }
    }
}

void GPE_Font::render_text_boxed( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor,int hAlign,int vAlign, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        SDL_Texture * fPairTex=  NULL;
        if( strTex!=NULL)
        {
            fPairTex = strTex->get_texture();
            if( renderAlpha > 255)
            {
                renderAlpha = 255;
            }
            if( fPairTex!=NULL && renderAlpha >0)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                if(hAlign<FA_LEFT || hAlign > FA_RIGHT )
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }

                if(vAlign < FA_TOP || vAlign > FA_BOTTOM )
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                case FA_CENTER:
                    xPos=xPos-texWid/2;
                    //LAST_USED_HALIGNMENT=FA_CENTER;
                    break;

                case FA_RIGHT:
                    xPos=xPos-texWid;
                    // LAST_USED_HALIGNMENT=FA_RIGHT;
                    break;

                //rendering left will be the default
                default:
                    // LAST_USED_HALIGNMENT=FA_LEFT;
                    break;
                }
                switch(vAlign)
                {
                case FA_MIDDLE:
                    yPos=yPos-texHeight/2;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                case FA_BOTTOM:
                    yPos=yPos-texHeight;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                //rendering left will be the default
                default:
                    //LAST_USED_VALIGNMENT=FA_TOP;
                    break;
                }
                GPE_Rect gpeClip;
                gpeClip.x = xPos;
                gpeClip.y = yPos;
                gpeClip.w = texWid;
                gpeClip.h = texHeight;
                SDL_Rect sdlClip = {xPos, yPos, texWid,texHeight};
                gcanvas->render_rect(  &gpeClip,boxColor,false,renderAlpha );
                SDL_RenderCopy( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),fPairTex, NULL, &sdlClip);
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
            }
        }
    }
}

void GPE_Font::render_text_rotated( int xPos, int yPos, std::string textureText, GPE_Color * textColor, float textAngle, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        SDL_Texture * fPairTex=  NULL;
        if( strTex!=NULL)
        {
            fPairTex = strTex->get_texture();
            if( renderAlpha > 255)
            {
                renderAlpha = 255;
            }
            if( fPairTex!=NULL && renderAlpha >0)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = strTex->get_width();
                texHeight = strTex->get_height();

                SDL_Rect sdlDstrect = {xPos-texWid, yPos-texHeight, texWid,texHeight};
                SDL_Point sdlCenterPoint = {texWid, texHeight};
                SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),fPairTex, NULL,&sdlDstrect,textAngle,&sdlCenterPoint, SDL_FLIP_NONE );
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
            }
        }
    }
}

bool GPE_Font::render_text_special( int xPos, int yPos, std::string textureText, GPE_Color * textColor, int hAlign,int vAlign, float renderAngle , float renderScale, int renderAlpha )
{
    if( renderAlpha > 0)
    {
        fontPair * strTex = find_texture(textureText);
        if( strTex!=NULL)
        {
            SDL_Texture * fPairTex = strTex->get_texture();
            if( fPairTex!=NULL)
            {
                int texWid = 0;
                int texHeight =0;
                SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );
                if( strTex->lastAlphaRendered!=renderAlpha )
                {
                    SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                    strTex->lastAlphaRendered = renderAlpha;
                }
                //Get image dimensions
                texWid = (float)strTex->get_width() * renderScale;
                texHeight = (float)strTex->get_height() * renderScale;

                //Get image dimensions
                if( texWid < 0)
                {
                    texWid *=-1;
                    renderAngle *=-1;
                }

                if( texHeight<0 )
                {
                    texHeight *=-1;
                    renderAngle *=-1;
                }

                if(hAlign<FA_LEFT || hAlign > FA_RIGHT )
                {
                    hAlign=LAST_USED_HALIGNMENT;
                }

                if(vAlign < FA_TOP || vAlign > FA_BOTTOM )
                {
                    vAlign=LAST_USED_VALIGNMENT;
                }
                switch(hAlign)
                {
                case FA_CENTER:
                    xPos=xPos-texWid/2;
                    //LAST_USED_HALIGNMENT=FA_CENTER;
                    break;

                case FA_RIGHT:
                    xPos=xPos-texWid;
                    // LAST_USED_HALIGNMENT=FA_RIGHT;
                    break;

                //rendering left will be the default
                default:
                    // LAST_USED_HALIGNMENT=FA_LEFT;
                    break;
                }
                switch(vAlign)
                {
                case FA_MIDDLE:
                    yPos=yPos-texHeight/2;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                case FA_BOTTOM:
                    yPos=yPos-texHeight;
                    // LAST_USED_VALIGNMENT=FA_MIDDLE;
                    break;

                //rendering left will be the default
                default:
                    //LAST_USED_VALIGNMENT=FA_TOP;
                    break;
                }

                SDL_Rect renderRect = {xPos, yPos, texWid,texHeight};
                SDL_RenderCopyEx( GPE_MAIN_RENDERER_SDL->get_sdl_renderer(),fPairTex, NULL,  &renderRect, -renderAngle,  NULL, SDL_FLIP_NONE );
                SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                return true;
            }
        }
    }
    return false;
}

GPE_Font_Controller::GPE_Font_Controller()
{
    newFontIdNumber = 0;
    fontTotalCount = 0;
    lastFontUsed = NULL;
}

GPE_Font_Controller::~GPE_Font_Controller()
{

}

GPE_Font * GPE_Font_Controller::open_font(std::string fontLocation, int fontSize, bool isMonospaced, std::string fNickName, int dynamicId )
{
    if( dynamicId < 0)
    {
        dynamicId = newFontIdNumber;
    }
    GPE_Font *  returnVal = new GPE_Font(fontLocation.c_str(),fontSize, isMonospaced, fNickName,dynamicId );
    loadedFonts.push_back(returnVal);
    newFontIdNumber++;
    fontTotalCount++;
    return returnVal;
}

void GPE_Font_Controller::close_font(GPE_Font * fontIn)
{
    if( fontIn!=NULL)
    {
        GPE_Font * cFont = NULL;
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

void GPE_Font_Controller::clear_font_cache()
{
    GPE_Report("Clearing Font Cache...");
    GPE_Font * cFont = NULL;
    for( int i = (int)loadedFonts.size()-1; i >=0; i--)
    {
        cFont = loadedFonts[i];
        if( cFont!=NULL)
        {
            cFont->clear_cache();
        }
    }
    GPE_Report("Font Cache total: ["+int_to_string(get_font_cache_size() )+"]...");
}

int GPE_Font_Controller::get_font_cache_size(bool recordCache)
{
    GPE_Font * cFont = NULL;
    int cacheCount = 0;
    int i = 0;
    int foundCache = 0;
    if( recordCache)
    {
        GPE_Report("\nCalculating Font Cache...");
        for( i = (int)loadedFonts.size()-1; i >=0; i--)
        {
            cFont = loadedFonts[i];
            if( cFont!=NULL)
            {
                foundCache =cFont->get_cache_count();
                cacheCount+=foundCache;
                GPE_Report("      Font["+cFont->fontNickName+"] Cache total: ["+int_to_string(foundCache )+"]...");
            }
        }
        GPE_Report("Font Cache total: ["+int_to_string(cacheCount )+"]...");
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

int GPE_Font_Controller::get_font_count()
{
    return fontTotalCount;
}

bool GPE_Font_Controller::render_bitmap_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = lastFontUsed;
    }
    if( textFont!=NULL)
    {
        textFont->render_bitmapped_text( xPos,yPos,mainTranslator->get_str(textureText ),textColor,hAlign,vAlign,renderAlpha);
        return true;
    }
    else
    {
        GPE_Report( "Unable to render number ("+ textureText + ")");
    }
    return false;
}

void GPE_Font_Controller::render_fps( GPE_Font * textFont, GPE_Color * color,  int xPos, int yPos, int hAlign , int vAlign  )
{
    //if( SHOW_FPS_COUNTER )
    {
        if( color == NULL )
        {
            color = c_lime;
        }

        if( GPE_MAIN_WINDOW->is_minimized() )
        {
            color =  c_red;
        }
        if( xPos < 0 )
        {
            xPos = SCREEN_WIDTH;
        }
        if( yPos < 0 )
        {
            yPos = 0;
        }
        render_text( xPos, yPos, float_to_string(CURRENT_FPS)+" / "+float_to_string(FPS_CAP)+" fps | "+float_to_string( CURRENT_DELTA )+" / " + float_to_string( gpe->get_needed_ticks() ) + " ms ",color,textFont,hAlign, vAlign );

    }
}

bool GPE_Font_Controller::render_only_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int renderAlpha)
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

bool GPE_Font_Controller::render_text( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = lastFontUsed;
    }
    if( (int)textureText.size() > 0 && textFont!=NULL )
    {
        textFont->render_text( xPos,yPos,mainTranslator->get_str(textureText ),textColor,hAlign,vAlign,renderAlpha);
        return true;
    }
    else if( textFont!=NULL )
    {
        // std::cout << "Unable to render text (" << textureText << ").\n";
    }
    return false;
}

bool GPE_Font_Controller::render_text_boxed( int xPos, int yPos, std::string textureText, GPE_Color * textColor,GPE_Color * boxColor, GPE_Font * textFont, int hAlign,int vAlign, int renderAlpha)
{
    if( textFont==NULL)
    {
        textFont = lastFontUsed;
    }
    if( textFont!=NULL)
    {
        textFont->render_text_boxed( xPos,yPos,mainTranslator->get_str(textureText ),textColor,boxColor,hAlign,vAlign,renderAlpha);
        return true;
    }
    return false;
}

bool GPE_Font_Controller::render_text_resized( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign, int vAlign, int rendWid, int rendHeight, int renderAlpha )
{
    if( textFont==NULL)
    {
        textFont = lastFontUsed;
    }
    if( textFont!=NULL)
    {
        textFont->render_text_resized( xPos,yPos,mainTranslator->get_str(textureText ),textColor,hAlign,vAlign,rendWid,rendHeight,renderAlpha);
        return true;
    }
    else if( textFont!=NULL )
    {
        std::cout << "Unable to render EXT text (" << textureText << ").\n";
    }
    return false;
}

bool GPE_Font_Controller::render_text_rotated( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, float textAngle, int renderAlpha )
{
    if( textFont==NULL)
    {
        textFont = lastFontUsed;
    }
    if( textFont!=NULL)
    {
        textFont->render_text_rotated( xPos,yPos,mainTranslator->get_str(textureText ),textColor,textAngle,renderAlpha);
        return true;
    }
    return false;
}

bool GPE_Font_Controller::render_text_scaled( int xPos, int yPos, std::string textureText,GPE_Color * textColor, float scaleValue, GPE_Font * textFont, int hAlign,int vAlign, int renderAlpha )
{
    if( textFont!=NULL)
    {
        textFont->render_text_scaled( xPos, yPos, textureText, textColor, scaleValue, hAlign, vAlign, renderAlpha );
    }
    return false;
}

bool GPE_Font_Controller::render_text_special( int xPos, int yPos, std::string textureText, GPE_Color * textColor, GPE_Font * textFont, int hAlign,int vAlign, float renderAngle, float renderScale, int renderAlpha )
{
    if( textFont!=NULL)
    {
        return textFont->render_text_special( xPos, yPos, textureText, textColor, hAlign, vAlign, renderAngle, renderScale, renderAlpha );
    }
    return false;
}


bool GPE_Font_Init()
{
    //If we already started the font system do nothing.
    if( gfs!=NULL )
    {
        return true;
    }
    //Initialize SDL_ttf
    GPE_Report("GPE_Font_System Started...");
    if( TTF_Init() == -1 )
    {
        GPE_Report("        Error loading SDL_TTF");
        return false;
    }
    if( gfs==NULL )
    {
        GFS = gfs = new GPE_Font_Controller();
    }
    return true;
}

void GPE_Font_Quit()
{
    if( gfs!=NULL )
    {
        delete gfs;
        GFS = gfs = NULL;
    }
    GPE_Report("Quitting SDL_TFF....");
    GPE_Report("GPE_Font_System Quit...");
    TTF_Quit();
}

