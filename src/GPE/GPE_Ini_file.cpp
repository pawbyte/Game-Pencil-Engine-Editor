/*
GPE_Ini_File.cpp
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

#include "GPE_Ini_File.h"
#include "GPE_Functions.h"

GPE_Ini_File::GPE_Ini_File()
{

}

GPE_Ini_File::~GPE_Ini_File()
{
    remove_all_sections();
}

GPE_KeyPair * GPE_Ini_File::add_section( std::string sectionName)
{
    if( find_section( sectionName ) !=NULL)
    {
        return NULL;
    }
    GPE_KeyPair * newSection = new GPE_KeyPair(-1, sectionName, sectionName);
    iniFileSections.push_back( newSection );
    return newSection;
}

void GPE_Ini_File::clear_all_sections()
{
    GPE_KeyPair * curSection = NULL;
    for( int i = (int)iniFileSections.size()-1; i >=0; i--)
    {
        curSection = iniFileSections[i];
        if( curSection!=NULL )
        {
            curSection->remove_all();
        }
    }
}

void GPE_Ini_File::clear_section( std::string sectionName )
{
    GPE_KeyPair * curSection = find_section( sectionName);
    if( curSection!=NULL )
    {
        curSection->remove_all();
    }
}

float GPE_Ini_File::find_float_keypair( std::string sectionName,std::string sKey)
{
    GPE_KeyPair * curSection = find_section( sectionName);
    if( curSection!=NULL )
    {
        GPE_KeyPair * curKey = NULL;
        for( int i = (int)curSection->subOptions.size()-1; i >=0; i--)
        {
            curKey = curSection->subOptions[i];
            if( curKey!=NULL)
            {
                if( curKey->keyString == sKey)
                {
                    return curKey->keyValue;
                }
            }
        }
    }
    return -1;
}

std::string GPE_Ini_File::find_string_keypair( std::string sectionName,std::string sKey)
{
    GPE_KeyPair * curSection = find_section( sectionName);
    if( curSection!=NULL )
    {
        GPE_KeyPair * curKey = NULL;
        for( int i = (int)curSection->subOptions.size()-1; i >=0; i--)
        {
            curKey = curSection->subOptions[i];
            if( curKey!=NULL)
            {
                if( curKey->keyString == sKey)
                {
                    return curKey->keySubString;
                }
            }
        }
    }
    return "";
}

GPE_KeyPair * GPE_Ini_File::find_section( std::string sectionName)
{
    GPE_KeyPair * curSection = NULL;
    for( int i = (int)iniFileSections.size()-1; i >=0; i--)
    {
        curSection = iniFileSections[i];
        if( curSection!=NULL && curSection->keyString == sectionName)
        {
            return curSection;
        }
    }
    return NULL;
}

bool GPE_Ini_File::map_key_pair( std::string sectionName,std::string sKey, std::string sValue)
{
    GPE_KeyPair * curSection = NULL;
    GPE_KeyPair * curKey = NULL;
    int j = 0;
    for( int i = (int)iniFileSections.size()-1; i >=0; i--)
    {
        curSection = iniFileSections[i];
        if( curSection!=NULL && curSection->keyString == sectionName)
        {
            for( int j = (int)curSection->subOptions.size()-1; j >=0; j--)
            {
                curKey = curSection->subOptions[i];
                if( curKey!=NULL)
                {
                    if( curKey->keyString == sKey)
                    {
                    }
                }
            }
        }
    }
}

bool GPE_Ini_File::read_ini_file(std::string fName, int lineTrimStyle )
{
    remove_all_sections();
    if( ! file_exists( fName) )
    {
        return false;
    }
    std::ifstream iniFileIn( fName.c_str() );

    std::string currInputLine = "";
    std::string currLineToBeProcessed = "";
    int equalPos = 0;
    int endBracketPos = 0;
    std::string firstChar="";
    std::string keyString="";
    std::string valString="";
    std::string subValString="";

    GPE_KeyPair * currentSection = NULL;
    while ( iniFileIn.good() )
    {
        getline (iniFileIn,currInputLine); //gets the next line of the file

        if( lineTrimStyle == 0)
        {
            currLineToBeProcessed = trim_left_inplace(currInputLine);
            currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);
        }
        else if( lineTrimStyle == -1)
        {
            currLineToBeProcessed = trim_left_inplace(currInputLine);
        }
        else if( lineTrimStyle ==1 )
        {
            currLineToBeProcessed = trim_left_inplace(currInputLine);
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
                    keyString = currLineToBeProcessed.substr(0,equalPos);
                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                    if( currentSection!=NULL)
                    {
                        currentSection->add_keypair( keyString,valString, string_to_float( valString) );
                    }
                    else
                    {
                        currentSection = new GPE_KeyPair( string_to_float(valString), keyString,  valString );
                        iniFileSections.push_back( currentSection );
                        currentSection = NULL;
                    }
                }
            }
        }
    }
    iniFileIn.close();
}

void GPE_Ini_File::remove_all_sections()
{
    GPE_KeyPair * curSection = NULL;
    for( int i = (int)iniFileSections.size()-1; i >=0; i--)
    {
        curSection = iniFileSections[i];
        if( curSection!=NULL)
        {
            delete curSection;
            curSection = NULL;
        }
    }
    iniFileSections.clear();
}

void GPE_Ini_File::remove_section( std::string sectionName )
{
    GPE_KeyPair * curSection = NULL;
    for( int i = (int)iniFileSections.size()-1; i >=0; i--)
    {
        curSection = iniFileSections[i];
        if( curSection!=NULL && curSection->keyString == sectionName)
        {
            delete curSection;
            curSection = NULL;
            iniFileSections.erase( iniFileSections.begin() + i );
        }
    }
}

bool GPE_Ini_File::write_ini_file(std::string fName)
{

}


