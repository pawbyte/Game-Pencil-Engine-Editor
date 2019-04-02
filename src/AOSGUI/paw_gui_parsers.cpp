/*
paw_gui_parsers.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

#include "paw_gui_parsers.h"
#include "paw_gui_themes.h"


GPE_KeyPair::GPE_KeyPair(double idNumb, std::string str, std::string ksubstr)
{
    keyString = str;
    keyValue = idNumb;
    keySubString = ksubstr;
}

GPE_KeyPair::~GPE_KeyPair()
{

}

GPE_ParsedText::GPE_ParsedText(int nStart, int nEnd)
{
    textStart = nStart;
    textEnd = nEnd;
}

GPE_ParsedText::~GPE_ParsedText()
{

}

GPE_SyntaxLine::GPE_SyntaxLine()
{
    firstPos = -1;
    lastPos = -1;
}

GPE_SyntaxLine::~GPE_SyntaxLine()
{
    for( int i = (int)foundParses.size()-1; i >=0; i--)
    {
        if( foundParses[i]!=NULL)
        {
            delete foundParses[i];
            foundParses[i] = NULL;
        }
    }
    foundParses.clear();
}

std::string GPE_SyntaxLine::get_parsed_line(std::string sIn)
{
    int startParsingPos = 0;
    int maxParseSize = (int)sIn.size();
    GPE_ParsedText* tempParseContent = NULL;
    if( maxParseSize>0 && (int)foundParses.size()>0)
    {
        std::string rString = "";
        for( int i = 0; i < (int)foundParses.size(); i++)
        {
            tempParseContent = foundParses[i];
            if( tempParseContent!=NULL)
            {
                if( tempParseContent->textStart < 0 || tempParseContent->textStart > maxParseSize )
                {
                    tempParseContent->textStart = 0;
                }
                if( tempParseContent->textStart < tempParseContent->textEnd && tempParseContent->textEnd >0 && tempParseContent->textEnd <= maxParseSize && tempParseContent->textStart >= startParsingPos  && tempParseContent->textEnd > startParsingPos )
                {
                    rString+=string_repeat(" ",tempParseContent->textStart-startParsingPos)+sIn.substr(tempParseContent->textStart,tempParseContent->textEnd-tempParseContent->textStart);
                    startParsingPos = tempParseContent->textEnd;
                }
            }
        }
        return rString;
    }
    return "";
}

void GPE_SyntaxLine::render_tokens(GPE_Font * fontIn, std::string sIn,int xPos, int yPos, int lineStartPos, int lineMaxPos, GPE_Color * renderColor, bool isNumber)
{
    int maxParseSize = (int)foundParses.size();
    if( maxParseSize > 0)
    {
        std::string rString = "";
        GPE_ParsedText* tempParseContent = NULL;
        if( renderColor==NULL)
        {
            renderColor = GPE_MAIN_THEME->Text_Box_Font_Color;
        }
        int fontWidth = fontIn->get_mono_width();
        for( int i = 0; i < maxParseSize; i++)
        {
            tempParseContent = foundParses[i];
            if( tempParseContent!=NULL)
            {
                if( tempParseContent->textEnd >= lineStartPos )
                {
                    rString = get_substring(sIn,tempParseContent->textStart,tempParseContent->textEnd-tempParseContent->textStart );
                    if( isNumber)
                    {
                        gfs->render_bitmap_text( xPos+fontWidth*(tempParseContent->textStart-lineStartPos),yPos, rString,renderColor,fontIn,FA_LEFT,FA_TOP );
                    }
                    else
                    {
                        gfs->render_text( xPos+fontWidth*(tempParseContent->textStart-lineStartPos),yPos, rString,renderColor,fontIn,FA_LEFT,FA_TOP );
                    }
                }
            }
        }
    }
}

void GPE_SyntaxLine::reset_self()
{
    for( int i = (int)foundParses.size()-1; i >=0; i--)
    {
        if( foundParses[i]!=NULL)
        {
            delete foundParses[i];
            foundParses[i] = NULL;
        }
    }
    foundParses.clear();
    firstPos = -1;
    lastPos = -1;
}


bool has_early_tab(std::string strIn)
{
    if( (int)strIn.size() > MAIN_GUI_SETTINGS->tabSpaceCount && MAIN_GUI_SETTINGS->tabSpaceCount > 0)
    {
        for( int ii= 0; ii < MAIN_GUI_SETTINGS->tabSpaceCount; ii++ )
        {
            if( strIn[ii]!=' ')
            {
                return false;
            }
        }
        return true;
    }
    else if( strIn.size() >= 4 )
    {
        if( strIn[0]==' ' && strIn[1]==' ' && strIn[2]==' ' && strIn[3]==' ')
        {
            return true;
        }
    }
    return false;
}

std::string untab_string(std::string strIn)
{
    if( has_early_tab(strIn) )
    {
        if( MAIN_GUI_SETTINGS!=NULL && MAIN_GUI_SETTINGS->tabSpaceCount > 0 )
        {
            strIn = get_substring(strIn, MAIN_GUI_SETTINGS->tabSpaceCount );
        }
        else
        {
            strIn = get_substring(strIn,  4 );
        }
    }
    return strIn;
}

std::string generate_tabs(int tabCount)
{
    if( tabCount > 0)
    {
        if( MAIN_GUI_SETTINGS!=NULL && MAIN_GUI_SETTINGS->tabSpaceCount > 0 )
        {
            return string_repeat(" ", tabCount * MAIN_GUI_SETTINGS->tabSpaceCount );
        }
        else
        {
            return string_repeat("    ", tabCount);
        }
    }
    //GPE_Report("0 tabs detected...");
    return "";
}

int get_tab_space_count()
{
    if( MAIN_GUI_SETTINGS!=NULL && MAIN_GUI_SETTINGS->tabSpaceCount > 0)
    {
        return MAIN_GUI_SETTINGS->tabSpaceCount;
    }
    return 4;
}
