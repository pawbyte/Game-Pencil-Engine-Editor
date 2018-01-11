/*
GPE_Translator.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "GPE_Translator.h"

GPE_Translator::GPE_Translator(std::string startLang)
{
    if( (int)startLang.size() >=1 )
    {
        myLanguage = startLang;
    }
    else
    {
        myLanguage = "";
    }
}

GPE_Translator::~GPE_Translator()
{

}


void GPE_Translator::clear_translations()
{
    myTranslations.clear();
}


bool GPE_Translator::export_to_po(std::string fName)
{
    return false;
}

std::string GPE_Translator::get_language( )
{
    return myLanguage;
}

std::string GPE_Translator::get_str(std::string message)
{
    std::string foundTranslation = myTranslations[message];
	if (foundTranslation == "") foundTranslation = message;
	return message;
}


bool GPE_Translator::translate_file(std::string fName)
{
    std::string expectedType = get_file_ext_last_dot(fName);
    if( expectedType == "po" )
    {
        return translate_po_file(fName);
    }
    return false;
}

//Incomplte function will finish it at a later time...
bool GPE_Translator::translate_po_file(std::string fName)
{
    std::ifstream langFile( fName.c_str() );

    record_error("Loading language file - "+fName );
    //If the level file could be loaded
    if( !langFile.fail() )
    {
        //makes sure the file is open
        if (langFile.is_open())
        {
            int keyPos = 0;
            std::string firstChar="";
            std::string section="";
            std::string cur_layer="";
            std::string data_format="";
            std::string msgid = "";
            std::string msgstr = "";
            std::string keyString ="";
            std::string valString = "";
            std::string currLine = "";
            std::string currLineToBeProcessed;
            float foundFileVersion = 0;
            int fCursorX = 0;
            int fCursorY = 0;
            while ( langFile.good() )
            {
                getline (langFile,currLine); //gets the next line of the file
                currLineToBeProcessed = trim_left_inplace(currLine);
                currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);
                if( !currLineToBeProcessed.empty() )
                {
                    //Comment skipping is only allowed at the top of the file
                    if( currLineToBeProcessed[0]== '#' )
                    {

                    }
                    else
                    {
                        //Seeks out msgstr
                        if( !msgid.empty() )
                        {
                            if (currLineToBeProcessed.find("msgstr ") == 0)
                            {
                                msgstr += split_first_string(currLineToBeProcessed,"msgstr ");
                            }
                            else if( !msgid.empty() )
                            {

                            }
                            else
                            {

                            }
                        }
                        else
                        {
                            if (currLineToBeProcessed.find("msgid ") == 0)
                            {
                                // grab only what's contained in the quotes
                                msgid+= split_first_string(currLineToBeProcessed,"msgid ");
                                msgstr = "";
                            }
                            else
                            {
                                //Looks for file meta-data
                                //searches for an equal character and parses through the variable
                                keyPos=currLineToBeProcessed.find_first_of(":");
                                if(keyPos!=(int)std::string::npos)
                                {
                                    //if the equalPos is present, then parse on through and carryon
                                    keyString = currLineToBeProcessed.substr(0,keyPos);
                                    valString = currLineToBeProcessed.substr(keyPos+1,currLineToBeProcessed.length());
                                }
                            }
                        }
                    }
                }
            }
            langFile.close();
        }
    }
    return false;
}

