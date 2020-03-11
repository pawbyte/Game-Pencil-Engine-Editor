/*
pawgui_parsers.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_parsers.h"
#include "pawgui_themes.h"

namespace pawgui
{

    parsed_text::parsed_text(int nStart, int nEnd)
    {
        textStart = nStart;
        textEnd = nEnd;
    }

    parsed_text::~parsed_text()
    {

    }

    syntax_line::syntax_line()
    {
        firstPos = -1;
        lastPos = -1;
    }

    syntax_line::~syntax_line()
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

    std::string syntax_line::get_parsed_line(std::string sIn)
    {
        int startParsingPos = 0;
        int maxParseSize = (int)sIn.size();
        parsed_text* tempParseContent = NULL;
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
                        rString+= stg_ex::string_repeat(" ",tempParseContent->textStart-startParsingPos)+sIn.substr(tempParseContent->textStart,tempParseContent->textEnd-tempParseContent->textStart);
                        startParsingPos = tempParseContent->textEnd;
                    }
                }
            }
            return rString;
        }
        return "";
    }

    void syntax_line::render_tokens( gpe::font_base * fontIn, std::string sIn,int xPos, int yPos, int lineStartPos, int lineMaxPos, gpe::color * renderColor, bool isNumber)
    {
        int maxParseSize = (int)foundParses.size();
        if( maxParseSize > 0)
        {
            std::string rString = "";
            parsed_text* tempParseContent = NULL;
            if( renderColor==NULL)
            {
                renderColor = pawgui::theme_main->text_box_font_color;
            }
            int fontWidth = fontIn->get_mono_width();
            for( int i = 0; i < maxParseSize; i++)
            {
                tempParseContent = foundParses[i];
                if( tempParseContent!=NULL)
                {
                    if( tempParseContent->textEnd >= lineStartPos )
                    {
                        rString = stg_ex::get_substring(sIn,tempParseContent->textStart,tempParseContent->textEnd-tempParseContent->textStart );
                        if( isNumber)
                        {
                            gpe::gfs->render_text( xPos+fontWidth*(tempParseContent->textStart-lineStartPos),yPos, rString,renderColor,fontIn,gpe::fa_left,gpe::fa_top );
                        }
                        else
                        {
                            gpe::gfs->render_text( xPos+fontWidth*(tempParseContent->textStart-lineStartPos),yPos, rString,renderColor,fontIn,gpe::fa_left,gpe::fa_top );
                        }
                    }
                }
            }
        }
    }

    void syntax_line::reset_self()
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
        if( (int)strIn.size() > main_settings->tabSpaceCount && main_settings->tabSpaceCount > 0)
        {
            for( int ii= 0; ii < main_settings->tabSpaceCount; ii++ )
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
            if( main_settings!=NULL && main_settings->tabSpaceCount > 0 )
            {
                strIn = stg_ex::get_substring(strIn, main_settings->tabSpaceCount );
            }
            else
            {
                strIn = stg_ex::get_substring(strIn,  4 );
            }
        }
        return strIn;
    }

    std::string generate_tabs(int tabCount)
    {
        if( tabCount > 0)
        {
            if( main_settings!=NULL && main_settings->tabSpaceCount > 0 )
            {
                return stg_ex::string_repeat(" ", tabCount * main_settings->tabSpaceCount );
            }
            else
            {
                return stg_ex::string_repeat("    ", tabCount);
            }
        }
        //gpe::error_log->report("0 tabs detected...");
        return "";
    }

    int get_tab_space_count()
    {
        if( main_settings!=NULL && main_settings->tabSpaceCount > 0)
        {
            return main_settings->tabSpaceCount;
        }
        return 4;
    }
}
