/*
pawgui_parsers.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/

#include "../parsers/pawgui_parsers.h"
#include "../themes/pawgui_themes.h"

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
            if( foundParses[i]!=nullptr)
            {
                delete foundParses[i];
                foundParses[i] = nullptr;
            }
        }
        foundParses.clear();
    }

    std::string syntax_line::get_parsed_line(std::string sIn)
    {
        int startParsingPos = 0;
        int maxParseSize = (int)sIn.size();
        parsed_text* tempParseContent = nullptr;
        if( maxParseSize>0 && (int)foundParses.size()>0)
        {
            std::string rstring = "";
            for( int i = 0; i < (int)foundParses.size(); i++)
            {
                tempParseContent = foundParses[i];
                if( tempParseContent!=nullptr)
                {
                    if( tempParseContent->textStart < 0 || tempParseContent->textStart > maxParseSize )
                    {
                        tempParseContent->textStart = 0;
                    }
                    if( tempParseContent->textStart < tempParseContent->textEnd && tempParseContent->textEnd >0 && tempParseContent->textEnd <= maxParseSize && tempParseContent->textStart >= startParsingPos  && tempParseContent->textEnd > startParsingPos )
                    {
                        rstring+= stg_ex::string_repeat(" ",tempParseContent->textStart-startParsingPos)+sIn.substr(tempParseContent->textStart,tempParseContent->textEnd-tempParseContent->textStart);
                        startParsingPos = tempParseContent->textEnd;
                    }
                }
            }
            return rstring;
        }
        return "";
    }

    void syntax_line::render_tokens( gpe::font_base * fontIn, std::string sIn,int x_pos, int y_pos, int lineStartPos, int lineMax_pos, gpe::color * render_color, bool isNumber)
    {
        int maxParseSize = (int)foundParses.size();
        if( maxParseSize > 0)
        {
            std::string rstring = "";
            parsed_text* tempParseContent = nullptr;
            if( render_color==nullptr)
            {
                render_color = pawgui::theme_main->text_box_font_color;
            }
            int fontWidth = fontIn->get_mono_width();
            for( int i = 0; i < maxParseSize; i++)
            {
                tempParseContent = foundParses[i];
                if( tempParseContent!=nullptr)
                {
                    if( tempParseContent->textEnd >= lineStartPos )
                    {
                        rstring = stg_ex::get_substring(sIn,tempParseContent->textStart,tempParseContent->textEnd-tempParseContent->textStart );
                        if( isNumber)
                        {
                            gpe::gfs->render_text( x_pos+fontWidth*(tempParseContent->textStart-lineStartPos),y_pos, rstring,render_color,fontIn,gpe::fa_left,gpe::fa_top );
                        }
                        else
                        {
                            gpe::gfs->render_text( x_pos+fontWidth*(tempParseContent->textStart-lineStartPos),y_pos, rstring,render_color,fontIn,gpe::fa_left,gpe::fa_top );
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
            if( foundParses[i]!=nullptr)
            {
                delete foundParses[i];
                foundParses[i] = nullptr;
            }
        }
        foundParses.clear();
        firstPos = -1;
        lastPos = -1;
    }


    bool has_early_tab(std::string str_in)
    {
        if( (int)str_in.size() > main_settings->tabSpaceCount && main_settings->tabSpaceCount > 0)
        {
            for( int ii= 0; ii < main_settings->tabSpaceCount; ii++ )
            {
                if( str_in[ii]!=' ')
                {
                    return false;
                }
            }
            return true;
        }
        else if( str_in.size() >= 4 )
        {
            if( str_in[0]==' ' && str_in[1]==' ' && str_in[2]==' ' && str_in[3]==' ')
            {
                return true;
            }
        }
        return false;
    }

    std::string untab_string(std::string str_in)
    {
        if( has_early_tab(str_in) )
        {
            if( main_settings!=nullptr && main_settings->tabSpaceCount > 0 )
            {
                str_in = stg_ex::get_substring(str_in, main_settings->tabSpaceCount );
            }
            else
            {
                str_in = stg_ex::get_substring(str_in,  4 );
            }
        }
        return str_in;
    }

    std::string generate_tabs(int tabCount)
    {
        if( tabCount > 0)
        {
            if( main_settings!=nullptr && main_settings->tabSpaceCount > 0 )
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
        if( main_settings!=nullptr && main_settings->tabSpaceCount > 0)
        {
            return main_settings->tabSpaceCount;
        }
        return 4;
    }
}
