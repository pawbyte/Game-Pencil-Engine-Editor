/*
gpe_ini_file.cpp
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

#include "gpe_ini_file.h"

namespace gpe
{
    gpe_ini_file::gpe_ini_file()
    {

    }

    gpe_ini_file::~gpe_ini_file()
    {
        remove_all_sections();
    }

    gpe::key_pair * gpe_ini_file::add_section( std::string section_name)
    {
        if( find_section( section_name ) !=nullptr)
        {
            return nullptr;
        }
        gpe::key_pair * newSection = new gpe::key_pair(-1, section_name, section_name);
        ini_sections.push_back( newSection );
        return newSection;
    }

    void gpe_ini_file::clear_all_sections()
    {
        gpe::key_pair * curSection = nullptr;
        for( int i = (int)ini_sections.size()-1; i >=0; i--)
        {
            curSection = ini_sections[i];
            if( curSection!=nullptr )
            {
                curSection->remove_all();
            }
        }
    }

    void gpe_ini_file::clear_section( std::string section_name )
    {
        gpe::key_pair * curSection = find_section( section_name);
        if( curSection!=nullptr )
        {
            curSection->remove_all();
        }
    }

    float gpe_ini_file::find_float_keypair( std::string section_name,std::string section_key)
    {
        gpe::key_pair * curSection = find_section( section_name);
        if( curSection!=nullptr )
        {
            gpe::key_pair * curKey = nullptr;
            for( int i = (int)curSection->sub_options.size()-1; i >=0; i--)
            {
                curKey = curSection->sub_options[i];
                if( curKey!=nullptr)
                {
                    if( curKey->key_string == section_key)
                    {
                        return curKey->key_value;
                    }
                }
            }
        }
        return -1;
    }

    std::string gpe_ini_file::find_string_keypair( std::string section_name,std::string section_key)
    {
        gpe::key_pair * curSection = find_section( section_name);
        if( curSection!=nullptr )
        {
            gpe::key_pair * curKey = nullptr;
            for( int i = (int)curSection->sub_options.size()-1; i >=0; i--)
            {
                curKey = curSection->sub_options[i];
                if( curKey!=nullptr)
                {
                    if( curKey->key_string == section_key)
                    {
                        return curKey->key_substring;
                    }
                }
            }
        }
        return "";
    }

    gpe::key_pair * gpe_ini_file::find_section( std::string section_name)
    {
        gpe::key_pair * curSection = nullptr;
        for( int i = (int)ini_sections.size()-1; i >=0; i--)
        {
            curSection = ini_sections[i];
            if( curSection!=nullptr && curSection->key_string == section_name)
            {
                return curSection;
            }
        }
        return nullptr;
    }

    bool gpe_ini_file::map_key_pair( std::string section_name,std::string section_key, std::string section_value)
    {
        gpe::key_pair * curSection = nullptr;
        gpe::key_pair * curKey = nullptr;
        int j = 0;
        for( int i = (int)ini_sections.size()-1; i >=0; i--)
        {
            curSection = ini_sections[i];
            if( curSection!=nullptr && curSection->key_string == section_name)
            {
                for( int j = (int)curSection->sub_options.size()-1; j >=0; j--)
                {
                    curKey = curSection->sub_options[i];
                    if( curKey!=nullptr)
                    {
                        if( curKey->key_string == section_key)
                        {
                        }
                    }
                }
            }
        }
        return false; //WIPNOTFUNCTIONALYET
    }

    bool gpe_ini_file::read_ini_file(std::string f_name, int lineTrimStyle )
    {
        remove_all_sections();
        if( ! sff_ex::file_exists( f_name) )
        {
            return false;
        }
        std::ifstream iniFileIn( f_name.c_str() );

        std::string currInputLine = "";
        std::string currLineToBeProcessed = "";
        int equalPos = 0;
        int endBracketPos = 0;
        std::string firstChar="";
        std::string key_string="";
        std::string valstring="";
        std::string subValstring="";

        gpe::key_pair * currentSection = nullptr;
        while ( iniFileIn.good() )
        {
            getline (iniFileIn,currInputLine); //gets the next line of the file

            if( lineTrimStyle == 0)
            {
                currLineToBeProcessed = stg_ex::trim_left_inplace(currInputLine);
                currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);
            }
            else if( lineTrimStyle == -1)
            {
                currLineToBeProcessed = stg_ex::trim_left_inplace(currInputLine);
            }
            else if( lineTrimStyle ==1 )
            {
                currLineToBeProcessed = stg_ex::trim_left_inplace(currInputLine);
            }
            else
            {
                currLineToBeProcessed = currInputLine;
            }
            //Empty Line skipping is only allowed at the top of the file
            if(!currLineToBeProcessed.empty() )
            {
                if( currLineToBeProcessed[0]== '#' || currLineToBeProcessed[0]!='/'   || currLineToBeProcessed[0]!=';'  )
                {
                    //skips comment lines
                }
                else if( currLineToBeProcessed[0]== '[')
                {
                    //adds a section
                    endBracketPos = currLineToBeProcessed.find_first_of("]");
                    if( endBracketPos > 2)
                    {
                        currentSection = add_section( currLineToBeProcessed.substr(1,endBracketPos) );
                    }
                }
                else
                {
                    //looks for keys for sections
                    equalPos=currLineToBeProcessed.find_first_of("=");
                    if(equalPos!=(int)std::string::npos)
                    {
                        //if the equalPos is present, then parse on through and carryon
                        key_string = currLineToBeProcessed.substr(0,equalPos);
                        valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                        if( currentSection!=nullptr)
                        {
                            currentSection->add_keypair( key_string,valstring, stg_ex::string_to_float( valstring) );
                        }
                        else
                        {
                            currentSection = new gpe::key_pair( stg_ex::string_to_float(valstring), key_string,  valstring );
                            ini_sections.push_back( currentSection );
                            currentSection = nullptr;
                        }
                    }
                }
            }
        }
        iniFileIn.close();
        return true;
    }

    void gpe_ini_file::remove_all_sections()
    {
        gpe::key_pair * curSection = nullptr;
        for( int i = (int)ini_sections.size()-1; i >=0; i--)
        {
            curSection = ini_sections[i];
            if( curSection!=nullptr)
            {
                delete curSection;
                curSection = nullptr;
            }
        }
        ini_sections.clear();
    }

    void gpe_ini_file::remove_section( std::string section_name )
    {
        gpe::key_pair * curSection = nullptr;
        for( int i = (int)ini_sections.size()-1; i >=0; i--)
        {
            curSection = ini_sections[i];
            if( curSection!=nullptr && curSection->key_string == section_name)
            {
                delete curSection;
                curSection = nullptr;
                ini_sections.erase( ini_sections.begin() + i );
            }
        }
    }

    bool gpe_ini_file::write_ini_file(std::string f_name)
    {
        return false; //WIPNOTFUNCTIONALYET
    }
}
