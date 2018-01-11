/*
gpe_syntax_handler.cpp
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
#include "gpe_syntax_handler.h"
#include "ambitious_gui_library.h"

GPE_Syntax_Highlighter * GPE_MAIN_HIGHLIGHTER = NULL;

GPE_Compiler_Term::GPE_Compiler_Term()
{
    termString = "";
    //termParameters = "";
    termDescription = "";
    termFunctionReturnType = "";
    termType = -1;
    termParametersString = "";
    termScope = "";
}

GPE_Compiler_Term::GPE_Compiler_Term(std::string tName,std::string tParameters, std::string tFunctionReturnType, std::string tDescription,int tType, std::string tScope)
{
    termScope = tScope;
    termString = tName;
    //termParameters = tParameters;
    termDescription = tDescription;
    termType = tType;
    termFunctionReturnType = tFunctionReturnType;
    termParametersString = tParameters;
    if( (int)tParameters.size() > 0 )
    {
        int commaPos = tParameters.find_first_of(",");
        std::string valString = "";
        while( commaPos!=(int)std::string::npos )
        {
            commaPos=tParameters.find_first_of(",");
            valString = split_first_string(tParameters,',');
            termParameters.push_back(valString);
        }
        termParameters.push_back( tParameters );
    }
}

GPE_Compiler_Term::~GPE_Compiler_Term()
{

}

std::string GPE_Compiler_Term::get_parameters()
{
    return termParametersString;
}

GPE_Gui_Engine_Language::GPE_Gui_Engine_Language(std::string langName, std::string langShortName, bool projectCodeLanguage,bool useDefaultSymbols)
{
    macroCommentChar = "";
    commentChar = "//";
    blockcommentStartStr = "/*";
    blockcommentEndStr = "*/";
    languageName = langName;
    languageShortName = langShortName;
    isCodingLanguage = projectCodeLanguage;
    if( useDefaultSymbols )
    {
        symbolLibrary.push_back('`');
        symbolLibrary.push_back('~');
        symbolLibrary.push_back('!');
        symbolLibrary.push_back('@');
        //symbolLibrary.push_back('#');
        symbolLibrary.push_back('$');
        symbolLibrary.push_back('%');
        symbolLibrary.push_back('^');
        symbolLibrary.push_back('&');
        symbolLibrary.push_back('*');
        symbolLibrary.push_back('(');
        symbolLibrary.push_back(')');
        symbolLibrary.push_back('-');
        symbolLibrary.push_back('_');
        symbolLibrary.push_back('=');
        symbolLibrary.push_back('+');
        symbolLibrary.push_back('\\');
        symbolLibrary.push_back('|');
        symbolLibrary.push_back(';');
        symbolLibrary.push_back(';');
        symbolLibrary.push_back(',');
        symbolLibrary.push_back('<');
        symbolLibrary.push_back('.');
        symbolLibrary.push_back('>');
        symbolLibrary.push_back('/');
        symbolLibrary.push_back('?');
    }
}

GPE_Gui_Engine_Language::~GPE_Gui_Engine_Language()
{

}


bool GPE_Gui_Engine_Language::add_language_variable(std::string nName, std::string nDescription,std::string  nScopre)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)languageVariables.size()-1; i>=0; i--)
    {
        tTerm = languageVariables[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    tTerm = new GPE_Compiler_Term(nName, "", "", nDescription, -1, nScopre);
    languageVariables.push_back( tTerm);
    return true;
}

bool GPE_Gui_Engine_Language::add_language_constant(std::string nName, std::string nDescription,std::string  nParameters)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)languageConstants.size()-1; i>=0; i--)
    {
        tTerm = languageConstants[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    tTerm = new GPE_Compiler_Term(nName, nParameters, "", nDescription, -1);
    languageConstants.push_back( tTerm);
    return true;
}

bool GPE_Gui_Engine_Language::add_language_function(std::string nName, std::string nDescription,std::string  nParameters, std::string tFunctionReturnType, std::string nScope)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)languageFunctions.size()-1; i>=0; i--)
    {
        tTerm = languageFunctions[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    if( (int)tFunctionReturnType.size()==0)
    {
        tFunctionReturnType = "void";
    }
    tTerm = new GPE_Compiler_Term(nName, nParameters, tFunctionReturnType,nDescription, CTERM_FUNCTION,nScope );
    languageFunctions.push_back( tTerm);
    return true;
}

bool GPE_Gui_Engine_Language::add_language_class(std::string nName, std::string nDescription,std::string  nParameters)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)languageClasses.size()-1; i>=0; i--)
    {
        tTerm = languageClasses[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    tTerm = new GPE_Compiler_Term(nName, nParameters, "", nDescription, -1);
    languageClasses.push_back( tTerm);
    return true;
}

bool GPE_Gui_Engine_Language::add_language_keyword(std::string nName, std::string nDescription,std::string  nScope)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)languageKeywords.size()-1; i>=0; i--)
    {
        tTerm = languageKeywords[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    tTerm = new GPE_Compiler_Term(nName, "", "",nDescription,-1,nScope);
    languageKeywords.push_back( tTerm);
    return true;
}

bool GPE_Gui_Engine_Language::charIsSymbol(char  charIn)
{
    for( int i = 0; i < (int)symbolLibrary.size(); i++)
    {
        if( charIn==symbolLibrary[i])
        {
            return true;
        }
    }
    return false;
}


int GPE_Gui_Engine_Language::firstSymbolInString(std::string inString, int pos)
{
    int returnVal = -1;

    int maxLoop = symbolLibrary.size();
    for(int i=0;i<maxLoop;i+=1)
    {
        int symPos = inString.find(symbolLibrary[i],pos);
        if(symPos!=-1)
        {
            if(returnVal==-1)
            {
                returnVal = symPos;
            }
            else if(symPos<returnVal)
            {
                returnVal = symPos;
            }
        }
    }

    return returnVal;
}

bool GPE_Gui_Engine_Language::is_compilable_word(std::string strIn)
{
    int i = 0;
    GPE_Compiler_Term * tTerm = NULL;
    for( i = (int)languageKeywords.size()-1; i>=0; i--)
    {
        tTerm = languageKeywords[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==strIn)
            {
                return false;
            }
        }
    }

    for( i = (int)languageVariables.size()-1; i>=0; i--)
    {
        tTerm = languageVariables[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==strIn)
            {
                return false;
            }
        }
    }

    for( i = (int)languageClasses.size()-1; i>=0; i--)
    {
        tTerm = languageClasses[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==strIn)
            {
                return false;
            }
        }
    }

    for( i = (int)languageDataTypes.size()-1; i>=0; i--)
    {
        tTerm = languageDataTypes[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==strIn)
            {
                return false;
            }
        }
    }
    return true;
}

bool GPE_Gui_Engine_Language::add_language_data_type(std::string nName, std::string nDescription,std::string  nParameters)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)languageDataTypes.size()-1; i>=0; i--)
    {
        tTerm = languageDataTypes[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString==nName)
            {
                return false;
            }
        }
    }
    tTerm = new GPE_Compiler_Term(nName, nParameters, "", nDescription, -1);
    languageDataTypes.push_back( tTerm);
    return true;
}

std::string GPE_Gui_Engine_Language::find_gpe_datatype(std::string strBeingChecked, int startPos)
{
    GPE_Compiler_Term * tTerm = NULL;
    int strSize = (int)strBeingChecked.size();
    if( strSize>0 && strSize > startPos)
    {
        std::string strToSearch = "";
        int tempStrSize = 0;
        int j = 0;
        bool allMatched = false;
        for( int i = (int)languageDataTypes.size()-1; i>0; i--)
        {
            tTerm= languageDataTypes[i];
            if( tTerm!=NULL)
            {
                strToSearch = tTerm->termString;
                tempStrSize = strToSearch.size();
                if(strSize >=  startPos+tempStrSize && tempStrSize>0)
                {
                    allMatched = true;
                    for( j = 0; j < tempStrSize; j++)
                    {
                        if(strBeingChecked[startPos+j]!=strToSearch[j])
                        {
                            allMatched = false;
                        }
                    }
                    if( allMatched ==true)
                    {
                        if( strSize > startPos+tempStrSize)
                        {
                            //if( char_is_alnum(strBeingChecked[startPos+tempStrSize],false,true)==false )
                            {
                                return strToSearch;
                            }
                        }
                        else
                        {
                            return strToSearch;
                        }
                    }
                }
            }
        }
    }
    return "";
}

std::string GPE_Gui_Engine_Language::find_gpe_function(std::string strBeingChecked, int startPos)
{
    GPE_Compiler_Term * tTerm = NULL;
    int strSize = (int)strBeingChecked.size();
    if( strSize>0 && strSize > startPos)
    {
        std::string strToSearch = "";
        int tempStrSize = 0;
        int j = 0;
        bool allMatched = false;
        for( int i = (int)languageFunctions.size()-1; i>=0; i--)
        {
            tTerm= languageFunctions[i];
            if( tTerm!=NULL)
            {
                strToSearch = tTerm->termString;
                tempStrSize = strToSearch.size();
                if(strSize >=  startPos+tempStrSize && tempStrSize>0)
                {
                    allMatched = true;
                    for( j = 0; j < tempStrSize; j++)
                    {
                        if(strBeingChecked[startPos+j]!=strToSearch[j])
                        {
                            allMatched = false;
                        }
                    }
                    if( allMatched ==true)
                    {
                        if( strSize > startPos+tempStrSize)
                        {
                            if( char_is_alnum(strBeingChecked[startPos+tempStrSize],false,true)==false )
                            {
                                return strToSearch;
                            }
                        }
                        else
                        {
                            return strToSearch;
                        }
                    }
                }
            }
        }
    }
    return "";
}

std::string GPE_Gui_Engine_Language::find_gpe_keyword(std::string strBeingChecked, int startPos)
{
    GPE_Compiler_Term * tTerm = NULL;
    int strSize = (int)strBeingChecked.size();
    if( strSize>0 && strSize > startPos)
    {
        std::string strToSearch = "";
        int tempStrSize = 0;
        int j = 0;
        bool allMatched = false;
        for( int i = (int)languageKeywords.size()-1; i>=0; i--)
        {
            tTerm= languageKeywords[i];
            if( tTerm!=NULL)
            {
                strToSearch = tTerm->termString;

                tempStrSize = strToSearch.size();
                if(strSize >=  startPos+tempStrSize && tempStrSize>0)
                {
                    allMatched = true;
                    for( j = 0; j < tempStrSize; j++)
                    {
                        if(strBeingChecked[startPos+j]!=strToSearch[j])
                        {
                            allMatched = false;
                        }
                    }
                    if( allMatched ==true)
                    {
                        if( strSize >= startPos+tempStrSize)
                        {
                            if( char_is_alnum(strBeingChecked[startPos+tempStrSize],false,true)==false )
                            {
                                return strToSearch;
                            }
                        }
                        else
                        {
                            return strToSearch;
                        }
                    }
                }
            }
        }
    }
    return "";
}

std::string GPE_Gui_Engine_Language::find_gpe_variable(std::string strBeingChecked, int startPos)
{
    GPE_Compiler_Term * tTerm = NULL;
    int strSize = (int)strBeingChecked.size();
    if( strSize>0 && strSize > startPos)
    {
        std::string strToSearch = "";
        int tempStrSize = 0;
        int j = 0;
        bool allMatched = false;
        for( int i = (int)languageVariables.size()-1; i>=0; i--)
        {
            tTerm= languageVariables[i];
            if( tTerm!=NULL)
            {
                strToSearch = tTerm->termString;

                tempStrSize = strToSearch.size();
                if(strSize >=  startPos+tempStrSize && tempStrSize>0)
                {
                    allMatched = true;
                    for( j = 0; j < tempStrSize; j++)
                    {
                        if(strBeingChecked[startPos+j]!=strToSearch[j])
                        {
                            allMatched = false;
                        }
                    }
                    if( allMatched ==true)
                    {
                        if( strSize >= startPos+tempStrSize)
                        {
                            if( char_is_alnum(strBeingChecked[startPos+tempStrSize],false,true)==false )
                            {
                                return strToSearch;
                            }
                        }
                        else
                        {
                            return strToSearch;
                        }
                    }
                }
            }
        }
    }
    return "";
}

GPE_Compiler_Term * GPE_Gui_Engine_Language::find_matching_variable(std::string nameIn)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)languageVariables.size()-1; i>=0; i--)
    {
        tTerm= languageVariables[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString == nameIn )
            {
                return tTerm;
            }
        }
    }
    return NULL;

}

GPE_Compiler_Term * GPE_Gui_Engine_Language::find_matching_function(std::string nameIn)
{
    GPE_Compiler_Term * tTerm = NULL;
    for( int i = (int)languageFunctions.size()-1; i>=0; i--)
    {
        tTerm= languageFunctions[i];
        if( tTerm!=NULL)
        {
            if( tTerm->termString == nameIn )
            {
                return tTerm;
            }
        }
    }
    return NULL;
}
GPE_Syntax_Highlighter::GPE_Syntax_Highlighter()
{
    newParametersString = "";
    parametersAreValid = false;

    GPE_Gui_Engine_Language * CPPLang  = add_programming_language("C++","CPP",true,true,false );
    CPPLang->macroCommentChar = "#";
    GPE_Gui_Engine_Language * JSLang = defaultLanguage = add_programming_language("JavaScript","JS",true,true,true );
    //JS constants
    JSLang->add_language_constant("IS_NULL");
    JSLang->add_language_constant("GPE_SETTINGS_ENTRY_LEVEL_LOCATION");
    JSLang->add_language_constant("pi");
    JSLang->add_language_constant("PI");

    //JS classes
    JSLang->add_language_class("activeObject");
    JSLang->add_language_class("GPE_String");
    JSLang->add_language_class("SuperVector");
    JSLang->add_language_class("window");
    JSLang->add_language_class("GPE_JSFile");
    JSLang->add_language_class("GPE_FileReader");
    JSLang->add_language_class("GPE_InputManager_GameController");
    JSLang->add_language_class("GPE_InputManager");
    JSLang->add_language_class("GPE_LogoDisplay");
    JSLang->add_language_class("GPE_MainGame");
    JSLang->add_language_class("GPE_GameScene");
    JSLang->add_language_class("ResourceController");
    JSLang->add_language_class("GPE_ResourceLoader");
    JSLang->add_language_class("GPE_MainMenuOption");
    JSLang->add_language_class("GPE_MainMenu");
    JSLang->add_language_class("GPE_GameController");
    JSLang->add_language_class("GPE_Rect");
    JSLang->add_language_class("GPE_GameSprite");
    JSLang->add_language_class("GPE_Tilesheet");
    JSLang->add_language_class("GPE_Texture");
    JSLang->add_language_class("GPE_Background");
    JSLang->add_language_class("GPE_GameMapTile");
    JSLang->add_language_class("GPE_TileLayer");
    JSLang->add_language_class("GPE_LevelView");
    JSLang->add_language_class("GPE_GameScene");

    /* JS Functions */
    //Audio functions
    JSLang->add_language_function("audio_loop","Loops audio whose id is audioId","audioId","void","gpe");
    JSLang->add_language_function("audio_pause","Pauses audio whose id is audioId","audioId","void","gpe");
    JSLang->add_language_function("audio_pause_all","Pauses all audio from playing","","void","gpe");
    JSLang->add_language_function("audio_pause_group","Pauses all audio whose group-id is groupId","groupId","void","gpe");

    JSLang->add_language_function("audio_playing","Checks if audio whose id is audioId is playing","audioId","bool","gpe");
    JSLang->add_language_function("audio_play","Plays audio whose id is audioId","audioId","void","gpe");
    JSLang->add_language_function("audio_stop_all","Stops all audio from playing","","void","gpe");
    JSLang->add_language_function("audio_stop_group","Stops all audio whose group-id is groupId","groupId","void","gpe");
    JSLang->add_language_function("audio_stop","Stops audio whose id is audioId","audioId","void","gpe");
    //math / collision functions
    JSLang->add_language_function("abs","Returns 0 or a positive number","x","double","gpe");
    JSLang->add_language_function("add_object","Returns a new object on the same layer as the current object","objectType, x, y","gameObject","GameObject");
    JSLang->add_language_function("add_new_object","Returns a new object","objectType, x, y, objectLayerId","gameObject","gpe");
    JSLang->add_language_function("bound_number","Bounds x between min and max","minN, maxN","double","gpe");
    JSLang->add_language_function("camera_on","Returns true or false if the camera is on or -1 if the camera is invalid","camId","int","gpe");
    JSLang->add_language_function("center_camera","Centers camera at (x,y)","(camId, x,y","void","gpe");
    JSLang->add_language_function("check_collision_rect","Checks if two rectangles collide","rectA, rectB","bool","gpe");
    JSLang->add_language_function("check_collision_with_rect","Checks if a rectangle collides with a box","boxX, boxY, boxW, boxH, rectA","bool","gpe");
    JSLang->add_language_function("degree","Converts an angle in radians to degrees","x","float","gpe");
    JSLang->add_language_function("radian","Converts an angle in degrees to radians","x","float","gpe");
    JSLang->add_language_function("sign","Gets the sign of the variable","x","int","gpe");
    //input functions

    JSLang->add_language_function("gamepad_connected","Checks if the gamepad is conected","controllerPos","bool","gpe");
    JSLang->add_language_function("gamepad_name","Returns the gamepad's name","controllerPos,","string","gpe");
    JSLang->add_language_function("gamepad_object","Returns the gamepad object","controllerPos,","GPE_GamePad","gpe");
    JSLang->add_language_function("gamepad_mapping","Returns the gamepad's mapping","controllerPos","string","gpe");
    JSLang->add_language_function("check_gamepad","Checks if the gamepad's button is down","controllerPos, buttonId","int","gpe");
    JSLang->add_language_function("check_gamepad_down","Checks if the gamepad's buttonId is down","controllerPos, buttonId","int","gpe");
    JSLang->add_language_function("check_gamepad_pressed","Checks if the gamepad's buttonId was pressed","controllerPos, buttonId","int","gpe");
    JSLang->add_language_function("check_gamepad_released","Checks if the gamepad's buttonId was released","controllerPos, buttonId","int","gpe");

    JSLang->add_language_function("check_keyboard_down","Checks if the keyboard button is down","keyId","bool","gpe");
    JSLang->add_language_function("check_keyboard_pressed","Checks if the keyboard button was pressed","keyId","bool","gpe");
    JSLang->add_language_function("check_keyboard_released","Checks if the keyboard button was released","keyId","bool","gpe");
    JSLang->add_language_function("check_keyboard","Checks if the keyboard button is down","keyId","bool","gpe");

    JSLang->add_language_function("check_mouse_down","Checks if the mouse button is down","buttonId","bool","gpe");
    JSLang->add_language_function("check_mouse_pressed","Checks if the mouse button is down","buttonId","bool","gpe");
    JSLang->add_language_function("check_mouse_released","Checks if the mouse button is down","buttonId","bool","gpe");
    JSLang->add_language_function("check_mouse","Checks if the keyboard mouse is down","buttonId","bool","gpe");
    // JS processing functions
    JSLang->add_language_function("disable_camera","Disables a camera","cameraId","void","gpe");
    JSLang->add_language_function("disable_canvas","Disables a canvas","canvasId","void","gpe");
    JSLang->add_language_function("disable_outofsightmode","Disables optimized sight mode","","void","gpe");
    JSLang->add_language_function("enable_outofsightmode","Enables optimized sight mode","","void","gpe");
    JSLang->add_language_function("enable_camera","Enables a camera","cameraId","void","gpe");
    JSLang->add_language_function("enable_canvas","Disables a canvas","canvasId","void","gpe");
    JSLang->add_language_function("equals","Checks if this object equals otherObject","otherObject","bool","gpe");
    JSLang->add_language_function("erase_data","Erases data pack","dataIdentifier, dataTitle","void","gpe");
    JSLang->add_language_function("exit_video_view_mode","Ends current video playing","","void","gpe");

    JSLang->add_language_function("find_tile","Checks for the tileset of tile at (x,y)","layerToCheck, xPos, yPosk","int","gpe");
    JSLang->add_language_function("find_tile_in_box","Checks if tile is located in box","layerToCheck, x1,  y1,  x2, y2, layerIdToCheck","int","gpe");
    JSLang->add_language_function("find_matching_tile","Checks if tile with MATCHING tileset exists at (x,y)","layerToCheck,  xPos,  yPos,  tileTypeToSearchFor","bool","gpe");
    JSLang->add_language_function("find_matching_tile_in_box","Checks if tile with MATCHING tileset exist in box","layerToCheck,  x1,  y1,  x2, y2,  tileTypeToSearchFor","bool","gpe");

    JSLang->add_language_function("find_furthest_object","Finds the furthest object away from (x,y)","theObjectType, xIn, yIn, avoidObjId, layerIdToCheck","GameObject","gpe");
    JSLang->add_language_function("find_nearest_object","Finds the nearest object away from (x,y)","theObjectType, xIn, yIn, avoidObjId, layerIdToCheck","GameObject","gpe");
    JSLang->add_language_function("get_camera_height","Get the height of the camera","cameraId","int","gpe");
    JSLang->add_language_function("get_camera_info","Get the height of the camera","cameraId","CameraData","gpe");
    JSLang->add_language_function("get_camera_width","Get the height of the camera","cameraId","int","gpe");
    JSLang->add_language_function("get_camera_x","Get the x-value of the camera","cameraId","int","gpe");
    JSLang->add_language_function("get_camera_y","Get the y-value of the camera","cameraId","int","gpe");
    JSLang->add_language_function("get_camera_screen_height","Get the height of the camera on screen","cameraId","int","gpe");
    JSLang->add_language_function("get_camera_screen_width","Get the height of the camera on screen","cameraId","int","gpe");
    JSLang->add_language_function("get_camera_screen_x","Get the height of the camera on screen","cameraId","int","gpe");
    JSLang->add_language_function("get_camera_screen_y","Get the height of the camera on screen","cameraId","int","gpe");
    JSLang->add_language_function("get_current_camera","Gets the id of the current camera being shown","","int","gpe");
    JSLang->add_language_function("get_current_camera_info","Get an object containing information of the camera","","CameraData","gpe");
    JSLang->add_language_function("get_background","Gets the background texture id","bgId","int","gpe");
    JSLang->add_language_function("get_direction","Returns the direction from (x1,y1) and (x2,y2)","x1, y1, x2, y2","int","gpe");
    JSLang->add_language_function("get_distance","Returns the distance between (x1,y1) and (x2,y2)","x1, y1, x2, y2","int","gpe");
    JSLang->add_language_function("get_mouse_x","Gets the global mouse x-position","","double","gpe");
    JSLang->add_language_function("get_mouse_y","Gets the global mouse y-position","","double","gpe");
    JSLang->add_language_function("load_data","Loads data pack","dataIdentifier, dataTitle","string","gpe");
    JSLang->add_language_function("move_camera","Moves camera to (x,y)","(camId, x,y","void","gpe");
    JSLang->add_language_function("setup_camera","Moves camera to (x,y) with sizing (w,h)","camId, x,y, w, h","void","gpe");
    JSLang->add_language_function("split_screen","Splits the scren up to 8 cameras","screenTotal, verticallySplit, useBlanks","bool","gpe");

    JSLang->add_language_function("font_is_monospaced","Checks if the font assigned to fontId is monospaced. Returns a blank string if out of bounds.","fontId","String","gpe");
    JSLang->add_language_function("get_font_family","Get the FamilyName of the font assigned to fontId. Returns a blank string if out of bounds.","fontId","String","gpe");
    JSLang->add_language_function("get_font_height","Get the estimated height of the font assigned to fontId. Returns a blank string if out of bounds.","fontId,words","String","gpe");
    JSLang->add_language_function("get_font_size","Get the size(pt) of the font assigned to fontId. Returns a blank string if out of bounds.","fontId","String","gpe");
    JSLang->add_language_function("get_font_width","Get the estimated width of the font assigned to fontId. Returns a blank string if out of bounds.","fontId,words","String","gpe");

    JSLang->add_language_function("get_sprite_width","Get the width of the sprite assigned to spriteId. Returns 0 if out of bounds.","spriteId","int","gpe");
    JSLang->add_language_function("get_sprite_height","Get the height of the sprite assigned to spriteId. Returns 0 if out of bounds","spriteId","int","gpe");
    JSLang->add_language_function("get_sprite_length","Get the length of the sprite assigned to spriteId. Returns 0 if out of bounds","spriteId","int","gpe");
    JSLang->add_language_function("get_sprite_object","Gets the actual sprite object attached to spriteId. Returns IS_NULL if out of bounds","spriteId","GameSprite","gpe");

    JSLang->add_language_function("get_texture_width","Get the width of the texture assigned to textureId. Returns 0 if out of bounds.","textureId","int","gpe");
    JSLang->add_language_function("get_texture_height","Get the height of the texture assigned to textureId. Returns 0 if out of bounds","textureId","int","gpe");
    JSLang->add_language_function("get_texture_object","Get the HTML5 Image of the texture assigned to textureId. Returns 0 if out of bounds","textureId","Image","gpe");

    JSLang->add_language_function("disable_camera","Disables a camera","cameraId","void","gpe");

    JSLang->add_language_function("game_reset","Resets the game game","","void","gpe");
    JSLang->add_language_function("get_camera_mouse_x","Gets the camera relative mouse x-position","cameraId","int","gpe");
    JSLang->add_language_function("get_camera_mouse_y","Gets the camera relative mouse y-position","cameraId","int","gpe");
    JSLang->add_language_function("get_fps","Returns frame rate","","int","gpe");
    JSLang->add_language_function("get_object_count","Returns the amount of objects within the scene","objectType","int","gpe");
    JSLang->add_language_function("get_object_count_all","Returns the amount of objects belonging to the family in the scene","familyId","int","gpe");
    JSLang->add_language_function("set_background","Changes the background texture","layerBeingChanged, bgToChange, newBgId","void","gpe");
    //JS Object processing functions
    JSLang->add_language_function("get_component","Gets component value","componentName","ANYTYPE","GameObject");
    JSLang->add_language_function("get_collision_height","Returns object collision height","","int","GameObject");
    JSLang->add_language_function("get_collision_width","Returns object collision width","","int","GameObject");
    JSLang->add_language_function("get_depth","Returns object depth","","int","GameObject");
    JSLang->add_language_function("get_my_direction","Returns the direction between object and (x2,y2)","x2, y2","int","GameObject");
    JSLang->add_language_function("get_my_distance","Returns the distance between object and (x2,y2)","x2, y2","int","GameObject");
    JSLang->add_language_function("get_sprite","Returns sprite-id of game object","","int","GameObject");
    JSLang->add_language_function("getx","Returns x-position of game object","","double","GameObject");
    JSLang->add_language_function("getx2","Returns x2-position of game object","","double","GameObject");
    JSLang->add_language_function("gety","Returns y-position of game object","","double","GameObject");
    JSLang->add_language_function("gety2","Returns y2-position of game object","","double","GameObject");
    JSLang->add_language_function("gravity_use","Activates built in gravity","","void", "GameObject");
    JSLang->add_language_function("init_sprite","Initiates the game object's sprite on creation","nextSprite","void","GameObject");
    JSLang->add_language_function("make_view_dependent","Makes this game object camera dependent","","void","GameObject");
    JSLang->add_language_function("make_view_independent","Makes this game object camera independent","","void","GameObject");
    JSLang->add_language_function("move_left","Moves this object left by m pixels","m","void","GameObject");
    JSLang->add_language_function("move_right","Moves this object right by m pixels","m","void","GameObject");
    JSLang->add_language_function("move_up","Moves this object up by m pixels","m","void","GameObject");
    JSLang->add_language_function("move_down","Moves this object down by m pixels","m","void","GameObject");
    JSLang->add_language_function("move_towards","Moves this towards the point (x,y) at speed","x,y,speed","void","GameObject");
    JSLang->add_language_function("under_mouse","Checks if this object is under the mouse via the cameraId(defaults to 0).","cameraId","void","GameObject");
    JSLang->add_language_function("set_sprite","Sets the sprite of this object","newSprite","void","GameObject");

    //Added as of Version 1.12 [ BEGIN ]
    JSLang->add_language_function("path_begin","Makes the object begin a path","path, pathSpeed, continueOnEnd, relativePath","void","GameObject");
    JSLang->add_language_function("path_start","Makes the object start a path","path, pathSpeed, continueOnEnd, relativePath","void","GameObject");
    JSLang->add_language_function("path_end","Ends the path the object is on","","void","GameObject");
    JSLang->add_language_function("path_stop","Stops the path the object is on","","void","GameObject");
    //Added as of Version 1.12 [ END ]
    JSLang->add_language_function("set_countdown","Sets a countdown of this object's timer","timerId, countDownStart","void","GameObject");
    JSLang->add_language_function("set_timer","Sets a countdown of this object's timer in seconds","timerId, secondsInTimer","void","GameObject");

    JSLang->add_language_function("set_velocity","Moves at direction at the speed rate","direction,speed","void","GameObject");

    JSLang->add_language_function("setx","Sets the x-position of this object","x","void","GameObject");
    JSLang->add_language_function("sety","Sets the y-position of this object","y","void","GameObject");
    JSLang->add_language_function("start_countdown","Sets a countdown of this object's timer","timerId, countDownStart","void","GameObject");
    JSLang->add_language_function("start_timer","Sets a countdown of this object's timer in seconds","timerId, countDownStart","void","GameObject");
    JSLang->add_language_function("update_cords","Sets the (x,y) position of this object","x,y","void","GameObject");

    //More GPE Functions
    JSLang->add_language_function("get_past_scene","Returns the past scene id visited. Returns -1 if there is no past scene.","","int","gpe");
    JSLang->add_language_function("get_past_scene_name","Returns the past scene's name visited. Returns an empty string if there is no past scene.","","int","gpe");
    JSLang->add_language_function("get_random","Returns a random number between min and max","min,max","int","gpe");
    JSLang->add_language_function("get_scene_width","Returns scene width","sceneIdNumberIn","int","gpe");
    JSLang->add_language_function("get_scene_height","Returns scene height","sceneIdNumberIn","int","gpe");
    JSLang->add_language_function("get_scene_name","Returns scene name","sceneIdNumberIn","string","gpe");
    JSLang->add_language_function("get_current_scene_id","Returns the id of the current scene","","int","gpe");
    JSLang->add_language_function("get_current_scene_name","Returns the name of current scene","","string","gpe");
    JSLang->add_language_function("getfps","Returns frame rate","","int","gpe");
    JSLang->add_language_function("get_fps","Returns frame rate","","int","gpe");
    JSLang->add_language_function("is_audio_playing","Returns true or false if audio is playing","audioIdIn","bool","gpe");
    JSLang->add_language_function("is_video_playing","Returns true or false if video is playing","videoIdIn","bool","gpe");

    JSLang->add_language_function("loop_audio","Loops audio whose id is audioId","audioId","void","gpe");
    JSLang->add_language_function("pause_audio","Pauses audio whose id is audioId","audioId","void","gpe");
    JSLang->add_language_function("play_audio","Play audio whose id is audioId","audioId","void","gpe");
    JSLang->add_language_function("position_collides","Returns true or false if objectOne collides at (x,y)","objOne,  xIn,  yIn,  otherObjectType, layerId ","bool","gpe");
    JSLang->add_language_function("stop_all_audio","Stops all audio","","void","gpe");
    JSLang->add_language_function("stop_audio_group","Stops all audio whose groupId is groupName","groupName","void","gpe");
    JSLang->add_language_function("stop_audio","Stops audio whose id is audioId","audioId","void","gpe");

    JSLang->add_language_function("loop_video","Loops video whose id is videoId","videoId","void","gpe");
    JSLang->add_language_function("pause_video","Pauses video whose id is videoId","videoId","void","gpe");
    JSLang->add_language_function("play_video","Play video whose id is videoId","videoId","void","gpe");
    JSLang->add_language_function("stop_all_video","Stops all video","","void","gpe");
    JSLang->add_language_function("stop_video_group","Stops all video whose groupId is groupName","groupName","void","gpe");
    JSLang->add_language_function("stop_video","Stops video whose id is videoId","videoId","void","gpe");

    //rendering functions
    JSLang->add_language_function("render_circle","Renders a circle","x, y, radius, circleColor, isOutline, oulineWidth, renderImmediately","void","gpe");
    JSLang->add_language_function("render_line","Renders a line","x1, y1, x2, y2, lineColor, lineWidth, renderImmediately","void","gpe");
    JSLang->add_language_function("render_rectangle","Renders a rectangle","x1, y1, x2, y2,rectColor, outline,rectLineWidth,renderImmediately","void","gpe");
    JSLang->add_language_function("render_self","Renders this object onto scene","","void","gpe");
    JSLang->add_language_function("render_sprite_rotated","Renders a rotated sprite","spriteIdIn, frameNumb, xOn, yOn, rotationAngle,scaleX, scaleY","void","gpe");

    JSLang->add_language_function("render_sprite","Renders a sprite","spriteIdIn, frameNumb, xOn, yOn, scaleX, scaleY","void","gpe");
    JSLang->add_language_function("render_square","Renders a square","xOn, yOn, squareSize,squareColor, outline,squareLineWidth,renderImmediately","void","gpe");
    JSLang->add_language_function("render_text","Renders text","fontIdIn, xOn, yOn, textToRender, fontRenderColor, fontHalign, fontValign","void","gpe");
    JSLang->add_language_function("render_texture_rotated","Renders a rotated texture","texureIdIn, xDraw, yDraw, width, height, angle","void","gpe");
    JSLang->add_language_function("render_texture_scaled","Renders a scaled texture","texureIdIn, xDraw, yDraw, width, height","void","gpe");
    JSLang->add_language_function("render_texture","Renders a texture","texureIdIn,  xDraw, yDraw","void","gpe");
    JSLang->add_language_function("render_triangle","Renders a triangle","x1, y1, x2, y2, x3, y3, triangleColor, outline, outlineSize, renderImmediately","void","gpe");
    JSLang->add_language_function("render_wrapped_text","Renders a wrapped text","fontIdIn, xOn, yOn, textToRender,fontRenderColor, maxWidth, lineHeight","void","gpe");
    JSLang->add_language_function("scene_enter_previous","Goes to the previous scene","","void","gpe");
    JSLang->add_language_function("scene_enter_named","Goes to the scene named sceneId","sceneId","void","gpe");
    JSLang->add_language_function("scene_enter_next","Goes to the next scene","","void","gpe");
    JSLang->add_language_function("scene_enter","Goes to the scene named sceneId","sceneId","void","gpe");
    JSLang->add_language_function("scene_reset","Resets the current game scene","","void","gpe");
    JSLang->add_language_function("self_destruct","Destroys this object","","void","gpe");
    JSLang->add_language_function("set_render_alpha","Sets the canvas alpha level from 0-255","alphaValue","void","gpe");

    //Collision / Touching Functionality
    JSLang->add_language_function("touches_object","Returns first object that collides at (x,y)","objOne,  xIn,  yIn,  otherObjectType","bool","gpe");
    JSLang->add_language_function("touches_family","Returns first object that collides at (x,y) with a member of the otherObjectType family","objOne,  xIn,  yIn,  otherObjectType","bool","gpe");
    JSLang->add_language_function("touches_tile","Returns first tile id touching object","layerToCheck","int","GameObject");
    JSLang->add_language_function("touches_certain_tile","Returns if object is touching a specific tile type","layerToCheck, needleTile","bool","GameObject");

    //GPE Video Functions
    JSLang->add_language_function("video_loop","Loops video whose id is videoId","videoId","void","gpe");
    JSLang->add_language_function("video_pause","Pauses video whose id is videoId","videoId","void","gpe");
    JSLang->add_language_function("video_play","Play video whose id is videoId","videoId","void","gpe");
    JSLang->add_language_function("video_stop_all","Stops all video","","void","gpe");
    JSLang->add_language_function("video_stop","Stops video whose id is videoId","videoId","void","gpe");

     //Console functions
    JSLang->add_language_function("clear","Clears the console window.","text","void","console");
    JSLang->add_language_function("error","Writes an error message into the console window.","message","void","console");
    JSLang->add_language_function("log","Writes text into the console window.","text","void","console");
    JSLang->add_language_function("warn","Writes a warning message into the console window.","text","void","console");

     //Math functions
    JSLang->add_language_function("acos","Returns the arccosine of x.","radians x","double","Math");
    JSLang->add_language_function("asin","Returns the arcsine  of x.","radians x","double","Math");
    JSLang->add_language_function("atan","Returns the arctangent of x as a numeric value between -PI/2 and PI/2 radians","x","double","Math");
    JSLang->add_language_function("atan2","Returns the arctangent of the quotient of its arguments","y,x","double","Math");

    JSLang->add_language_function("exp","Returns E ^X","x","double","Math");
    JSLang->add_language_function("pow","Returns X ^Y","x,y","double","Math");
    JSLang->add_language_function("log","Returns the natural logarithm (base E) of x","x","int","Math");
    JSLang->add_language_function("cos","Returns cos( (radians)x )","(radians)x","double","Math");
    JSLang->add_language_function("sin","Returns sin( (radians)x )","(radians)x","double","Math");
    JSLang->add_language_function("tan","Returns tan( (angle)x )","x,y","double","Math");
    JSLang->add_language_function("min","Returns the smallest NUMBER found","x,y,z...N","double","Math");
    JSLang->add_language_function("max","Returns the highest NUMBER found","x,y,z...N","double","Math");
    JSLang->add_language_function("sqrt","Returns the square root of x","x","double","Math");
    JSLang->add_language_function("random","Returns a random number from 0 to 1","","double","Math");
    JSLang->add_language_function("round","Rounds the number","","int","Math");
    JSLang->add_language_function("floor","Rounds the number downward","","int","Math");
    JSLang->add_language_function("ceil","Rounds the number upward","","int","Math");

    //JS keywords
    JSLang->add_language_keyword("break");
    JSLang->add_language_keyword("case");
    JSLang->add_language_keyword("const");
    JSLang->add_language_keyword("continue");
    JSLang->add_language_keyword("default");
    JSLang->add_language_keyword("delete");
    JSLang->add_language_keyword("do");
    JSLang->add_language_keyword("else");
    JSLang->add_language_keyword("FALSE");
    JSLang->add_language_keyword("false");
    JSLang->add_language_keyword("fa_left");
    JSLang->add_language_keyword("fa_center");
    JSLang->add_language_keyword("fa_right");
    JSLang->add_language_keyword("fa_top");
    JSLang->add_language_keyword("fa_middle");
    JSLang->add_language_keyword("fa_bottom");
    JSLang->add_language_keyword("for");
    JSLang->add_language_keyword("gpe");
    JSLang->add_language_keyword("GPE");
    JSLang->add_language_keyword("gpr");
    JSLang->add_language_keyword("GPR");
    JSLang->add_language_keyword("if");
    JSLang->add_language_keyword("Infinity");
    JSLang->add_language_keyword("isnull");
    JSLang->add_language_keyword("is_null");
    JSLang->add_language_keyword("IS_NULL");
    JSLang->add_language_keyword("NaN");
    JSLang->add_language_keyword("new");
    JSLang->add_language_keyword("null");
    JSLang->add_language_keyword("NULL");
    JSLang->add_language_keyword("other");
    JSLang->add_language_keyword("pencil");
    JSLang->add_language_keyword("return");
    JSLang->add_language_keyword("switch");
    JSLang->add_language_keyword("this");
    JSLang->add_language_keyword("true");
    JSLang->add_language_keyword("TRUE");
    JSLang->add_language_keyword("undefined");
    JSLang->add_language_keyword("UNDEFINED");
    JSLang->add_language_keyword("while");

    //JS Keyboard (kb_ ) constants
    JSLang->add_language_keyword("kb_0");
    JSLang->add_language_keyword("kb_1");
    JSLang->add_language_keyword("kb_2");
    JSLang->add_language_keyword("kb_3");
    JSLang->add_language_keyword("kb_4");
    JSLang->add_language_keyword("kb_5");
    JSLang->add_language_keyword("kb_6");
    JSLang->add_language_keyword("kb_7");
    JSLang->add_language_keyword("kb_8");
    JSLang->add_language_keyword("kb_9");

    JSLang->add_language_keyword("kb_a");
    JSLang->add_language_keyword("kb_b");
    JSLang->add_language_keyword("kb_c");
    JSLang->add_language_keyword("kb_d");
    JSLang->add_language_keyword("kb_e");
    JSLang->add_language_keyword("kb_f");
    JSLang->add_language_keyword("kb_g");
    JSLang->add_language_keyword("kb_h");
    JSLang->add_language_keyword("kb_i");
    JSLang->add_language_keyword("kb_j");
    JSLang->add_language_keyword("kb_k");
    JSLang->add_language_keyword("kb_l");
    JSLang->add_language_keyword("kb_m");
    JSLang->add_language_keyword("kb_n");
    JSLang->add_language_keyword("kb_o");
    JSLang->add_language_keyword("kb_p");
    JSLang->add_language_keyword("kb_q");
    JSLang->add_language_keyword("kb_r");
    JSLang->add_language_keyword("kb_s");
    JSLang->add_language_keyword("kb_t");
    JSLang->add_language_keyword("kb_u");
    JSLang->add_language_keyword("kb_v");
    JSLang->add_language_keyword("kb_w");
    JSLang->add_language_keyword("kb_x");
    JSLang->add_language_keyword("kb_y");
    JSLang->add_language_keyword("kb_z");

    JSLang->add_language_keyword("kb_np0");
    JSLang->add_language_keyword("kb_np1");
    JSLang->add_language_keyword("kb_np2");
    JSLang->add_language_keyword("kb_np3");
    JSLang->add_language_keyword("kb_np4");
    JSLang->add_language_keyword("kb_np5");
    JSLang->add_language_keyword("kb_np6");
    JSLang->add_language_keyword("kb_np7");
    JSLang->add_language_keyword("kb_np8");
    JSLang->add_language_keyword("kb_np9");

    JSLang->add_language_keyword("kb_f1");
    JSLang->add_language_keyword("kb_f2");
    JSLang->add_language_keyword("kb_f3");
    JSLang->add_language_keyword("kb_f4");
    JSLang->add_language_keyword("kb_f5");
    JSLang->add_language_keyword("kb_f6");
    JSLang->add_language_keyword("kb_f7");
    JSLang->add_language_keyword("kb_f8");
    JSLang->add_language_keyword("kb_f9");
    JSLang->add_language_keyword("kb_f10");
    JSLang->add_language_keyword("kb_f11");
    JSLang->add_language_keyword("kb_f12");

    JSLang->add_language_keyword("kb_backspace");
    JSLang->add_language_keyword("kb_tab");
    JSLang->add_language_keyword("kb_enter");
    JSLang->add_language_keyword("kb_pause");
    JSLang->add_language_keyword("kb_caps");
    JSLang->add_language_keyword("kb_esc");
    JSLang->add_language_keyword("kb_space");
    JSLang->add_language_keyword("kb_page_up");
    JSLang->add_language_keyword("kb_page_down");
    JSLang->add_language_keyword("kb_end");
    JSLang->add_language_keyword("kb_home");
    JSLang->add_language_keyword("kb_left");
    JSLang->add_language_keyword("kb_up");
    JSLang->add_language_keyword("kb_right");
    JSLang->add_language_keyword("kb_down");
    JSLang->add_language_keyword("kb_insert");
    JSLang->add_language_keyword("kb_delete");
    JSLang->add_language_keyword("kb_shift");
    JSLang->add_language_keyword("kb_ctrl");
    JSLang->add_language_keyword("kb_alt");
    JSLang->add_language_keyword("kb_plus");
    JSLang->add_language_keyword("kb_period");
    JSLang->add_language_keyword("kb_minus");
    JSLang->add_language_keyword("kb_period");

    JSLang->add_language_keyword("kb_multiply");
    JSLang->add_language_keyword("kb_add");
    JSLang->add_language_keyword("kb_substract");
    JSLang->add_language_keyword("kb_decimal");
    JSLang->add_language_keyword("kb_divide");
    JSLang->add_language_keyword("kb_fslash");
    JSLang->add_language_keyword("kb_gaccent");
    JSLang->add_language_keyword("kb_obracket");
    JSLang->add_language_keyword("kb_bslash");
    JSLang->add_language_keyword("kb_cbracket");
    JSLang->add_language_keyword("kb_squote");

    //JS Keyboard (gp_ ) constants
    JSLang->add_language_keyword("gp_0");
    JSLang->add_language_keyword("gp_1");
    JSLang->add_language_keyword("gp_2");
    JSLang->add_language_keyword("gp_3");
    JSLang->add_language_keyword("gp_4");
    JSLang->add_language_keyword("gp_5");
    JSLang->add_language_keyword("gp_6");
    JSLang->add_language_keyword("gp_7");
    JSLang->add_language_keyword("gp_8");
    JSLang->add_language_keyword("gp_9");

    JSLang->add_language_keyword("gp_np0");
    JSLang->add_language_keyword("gp_np1");
    JSLang->add_language_keyword("gp_np2");
    JSLang->add_language_keyword("gp_np3");
    JSLang->add_language_keyword("gp_np4");
    JSLang->add_language_keyword("gp_np5");
    JSLang->add_language_keyword("gp_np6");
    JSLang->add_language_keyword("gp_np7");
    JSLang->add_language_keyword("gp_np8");
    JSLang->add_language_keyword("gp_np9");

    JSLang->add_language_keyword("gp_f1");
    JSLang->add_language_keyword("gp_f2");
    JSLang->add_language_keyword("gp_f3");
    JSLang->add_language_keyword("gp_f4");
    JSLang->add_language_keyword("gp_f5");
    JSLang->add_language_keyword("gp_f6");
    JSLang->add_language_keyword("gp_f7");
    JSLang->add_language_keyword("gp_f8");
    JSLang->add_language_keyword("gp_f9");
    JSLang->add_language_keyword("gp_f10");
    JSLang->add_language_keyword("gp_f11");
    JSLang->add_language_keyword("gp_f12");

    JSLang->add_language_keyword("gp_backspace");
    JSLang->add_language_keyword("gp_tab");
    JSLang->add_language_keyword("gp_enter");
    JSLang->add_language_keyword("gp_pause");
    JSLang->add_language_keyword("gp_caps");
    JSLang->add_language_keyword("gp_esc");
    JSLang->add_language_keyword("gp_space");
    JSLang->add_language_keyword("gp_page_up");
    JSLang->add_language_keyword("gp_page_down");
    JSLang->add_language_keyword("gp_end");
    JSLang->add_language_keyword("gp_home");
    JSLang->add_language_keyword("gp_left");
    JSLang->add_language_keyword("gp_up");
    JSLang->add_language_keyword("gp_right");
    JSLang->add_language_keyword("gp_down");
    JSLang->add_language_keyword("gp_insert");
    JSLang->add_language_keyword("gp_delete");
    JSLang->add_language_keyword("gp_shift");
    JSLang->add_language_keyword("gp_ctrl");
    JSLang->add_language_keyword("gp_alt");
    JSLang->add_language_keyword("gp_plus");
    JSLang->add_language_keyword("gp_comma");
    JSLang->add_language_keyword("gp_minus");
    JSLang->add_language_keyword("gp_period");

    JSLang->add_language_keyword("gp_multiply");
    JSLang->add_language_keyword("gp_add");
    JSLang->add_language_keyword("gp_substract");
    JSLang->add_language_keyword("gp_decimal");
    JSLang->add_language_keyword("gp_divide");
    JSLang->add_language_keyword("gp_fslash");
    JSLang->add_language_keyword("gp_gaccent");
    JSLang->add_language_keyword("gp_obracket");
    JSLang->add_language_keyword("gp_bslash");
    JSLang->add_language_keyword("gp_cbracket");
    JSLang->add_language_keyword("gp_squote");

    //JS Mouse Contants
    JSLang->add_language_keyword("mb_center");
    JSLang->add_language_keyword("mb_left");
    JSLang->add_language_keyword("mb_middle");
    JSLang->add_language_keyword("mb_right");

    //JS GameController constants(gc_) [ BEGIN ]
    JSLang->add_language_keyword("gc_anycontroller");
    JSLang->add_language_keyword("gc_nocontroller");
    JSLang->add_language_keyword("gc_anybutton");
    JSLang->add_language_keyword("gc_nobutton");

    JSLang->add_language_keyword("gc_joystick_left_x");
    JSLang->add_language_keyword("gc_joystick_left_y");
    JSLang->add_language_keyword("gc_joystick_right_x");
    JSLang->add_language_keyword("gc_joystick_right_y");
    JSLang->add_language_keyword("gc_joystick_dpad_x");
    JSLang->add_language_keyword("gc_joystick_dpad_y");

    JSLang->add_language_keyword("gc_left");
    JSLang->add_language_keyword("gc_right");
    JSLang->add_language_keyword("gc_up");
    JSLang->add_language_keyword("gc_down");
    JSLang->add_language_keyword("gkb_c0");
    JSLang->add_language_keyword("gkb_c1");
    JSLang->add_language_keyword("gkb_c2");
    JSLang->add_language_keyword("gkb_c3");
    JSLang->add_language_keyword("gc_l1");
    JSLang->add_language_keyword("gc_l2");
    JSLang->add_language_keyword("gc_l3");
    JSLang->add_language_keyword("gc_r1");
    JSLang->add_language_keyword("gc_r2");
    JSLang->add_language_keyword("gc_r3");

    JSLang->add_language_keyword("gc_home");
    JSLang->add_language_keyword("gc_select");
    JSLang->add_language_keyword("gc_share");
    JSLang->add_language_keyword("gc_start");
    //GameController constants(gc_) [ END ]

    //Legacy Keyboard constants(gp_) [ BEGIN ]
    JSLang->add_language_keyword("gp_a");
    JSLang->add_language_keyword("gp_b");
    JSLang->add_language_keyword("gp_c");
    JSLang->add_language_keyword("gp_d");
    JSLang->add_language_keyword("gp_e");
    JSLang->add_language_keyword("gp_f");
    JSLang->add_language_keyword("gp_g");
    JSLang->add_language_keyword("gp_h");
    JSLang->add_language_keyword("gp_i");
    JSLang->add_language_keyword("gp_j");
    JSLang->add_language_keyword("gp_k");
    JSLang->add_language_keyword("gp_l");
    JSLang->add_language_keyword("gp_m");
    JSLang->add_language_keyword("gp_n");
    JSLang->add_language_keyword("gp_o");
    JSLang->add_language_keyword("gp_p");
    JSLang->add_language_keyword("gp_q");
    JSLang->add_language_keyword("gp_r");
    JSLang->add_language_keyword("gp_s");
    JSLang->add_language_keyword("gp_t");
    JSLang->add_language_keyword("gp_u");
    JSLang->add_language_keyword("gp_v");
    JSLang->add_language_keyword("gp_w");
    JSLang->add_language_keyword("gp_x");
    JSLang->add_language_keyword("gp_y");
    JSLang->add_language_keyword("gp_z");

    //Legacy Keyboard constants(gp_) [ END ]

    //javascript only words(allowed on html5 building)
    JSLang->add_language_keyword("console","The console window used to display errors in important information","JavaScript Global");
    JSLang->add_language_keyword("Date","Used to get the time/date of the user","JavaScript Global");
    JSLang->add_language_keyword("Math","The built in Math class","JavaScript Global");
    JSLang->add_language_keyword("debuger","The debugger console window used to display errors","JavaScript Global");
    JSLang->add_language_keyword("extends","","JavaScript Global");
    JSLang->add_language_keyword("export","","JavaScript Global");
    JSLang->add_language_keyword("finally","","JavaScript Global");
    JSLang->add_language_keyword("function","","JavaScript Global");
    JSLang->add_language_keyword("import","","JavaScript Global");

    JSLang->add_language_keyword("Math","","JavaScript Global");
    JSLang->add_language_keyword("super","","JavaScript Global");
    JSLang->add_language_keyword("throw","","JavaScript Global");
    JSLang->add_language_keyword("try","","JavaScript Global");
    JSLang->add_language_keyword("typeof","","JavaScript Global");
    JSLang->add_language_keyword("var","","JavaScript Global");
    JSLang->add_language_keyword("yield","","JavaScript Global");

    //Object key variables
    JSLang->add_language_variable("customComponents","An object's array of custom components created in editor", "GameObject" );
    JSLang->add_language_variable("direction","An objects direction in radians", "GameObject" );
    JSLang->add_language_variable("speed","An objects speed in pixels", "GameObject" );
    JSLang->add_language_variable("imageSpeed","An object's imageSpeed per game frame", "GameObject" );
    JSLang->add_language_variable("isVisible","A boolean if the object is visible", "GameObject" );
    JSLang->add_language_variable("spriteFrame","The current frame of the object's current sprite", "GameObject" );
    JSLang->add_language_variable("xPast","The object's x-coordinate from last frame", "GameObject" );
    JSLang->add_language_variable("yPast","The object's y-coordinate from last frame", "GameObject" );

    JSLang->add_language_variable("GPE_SETTINGS_APP_LOGO_LOCATION");
    JSLang->add_language_variable("GPE_SETTINGS_SCREEN_WIDTH");
    JSLang->add_language_variable("GPE_SETTINGS_SCREEN_HEIGHT");
    JSLang->add_language_variable("GPE_SETTINGS_PROG_NAME");
    JSLang->add_language_variable("GPE_MAX_OBJECT_TYPES");
    JSLang->add_language_variable("GPE_SETTINGS_SYSTEM_OS");
    JSLang->add_language_variable("GPE_GAME_PUBLISHER");
    JSLang->add_language_variable("GPE_GAME_DEVELOPER");
    JSLang->add_language_variable("GPE_GAME_OBJECTS_COUNT");
    JSLang->add_language_variable("GPE_SETTINGS_IS_DEBUGGING");
    JSLang->add_language_variable("GPE_SETTINGS_SHOW_FPS");
    JSLang->add_language_variable("GPE_SETTINGS_ENTRY_LEVEL_LOCATION");

    JSLang->add_language_variable("programStates");
    JSLang->add_language_variable("canvas");
    JSLang->add_language_variable("context");
    JSLang->add_language_variable("inputController");
    JSLang->add_language_variable("maxViews");
    JSLang->add_language_variable("EXTRA_CANVASES");
    JSLang->add_language_variable("preGuiCamvas");
    JSLang->add_language_variable("fileReader");
    JSLang->add_language_variable("programStateId");
    JSLang->add_language_variable("currentState");

	//actual datatypes used/allowed
    JSLang->add_language_data_type("char");
    JSLang->add_language_data_type("double");
    JSLang->add_language_data_type("float");
    JSLang->add_language_data_type("grid");
    JSLang->add_language_data_type("int");
    JSLang->add_language_data_type("string");
    JSLang->add_language_data_type("vector");

    //Adds CSS language syntax and such
    GPE_Gui_Engine_Language * CSSLang = add_programming_language("CSS","CSS", true, false,false);
    CSSLang->add_language_keyword("a:active");
    CSSLang->add_language_keyword("a:hover");
    CSSLang->add_language_keyword("a:link");
    CSSLang->add_language_keyword("a:visited");
    CSSLang->add_language_keyword("a");
    CSSLang->add_language_keyword("body");
    CSSLang->add_language_keyword("button");
    CSSLang->add_language_keyword("canvas");
    CSSLang->add_language_keyword("div");
    CSSLang->add_language_keyword("p");
    CSSLang->add_language_keyword("h1");
    CSSLang->add_language_keyword("h2");
    CSSLang->add_language_keyword("h3");
    CSSLang->add_language_keyword("h4");
    CSSLang->add_language_keyword("h5");
    CSSLang->add_language_keyword("h6");
    CSSLang->add_language_keyword("h7");
    CSSLang->add_language_keyword("h8");
    CSSLang->add_language_keyword("h9");
    CSSLang->add_language_keyword("html");
    CSSLang->add_language_keyword("hr");
    CSSLang->add_language_keyword("input");
    CSSLang->add_language_keyword("li");
    CSSLang->add_language_keyword("ol");
    CSSLang->add_language_keyword("table");
    CSSLang->add_language_keyword("th");
    CSSLang->add_language_keyword("th");
    CSSLang->add_language_keyword("tr");
    CSSLang->add_language_keyword("ul");

    CSSLang->add_language_keyword("align");
    CSSLang->add_language_keyword("background");
    CSSLang->add_language_keyword("backface");
    CSSLang->add_language_keyword("bottom");
    CSSLang->add_language_keyword("border");
    CSSLang->add_language_keyword("box");
    CSSLang->add_language_keyword("clear");
    CSSLang->add_language_keyword("collapse");
    CSSLang->add_language_keyword("color");
    CSSLang->add_language_keyword("cursor");
    CSSLang->add_language_keyword("display");
    CSSLang->add_language_keyword("family");
    CSSLang->add_language_keyword("filter");
    CSSLang->add_language_keyword("float");
    CSSLang->add_language_keyword("font");
    CSSLang->add_language_keyword("height");
    CSSLang->add_language_keyword("horizontal");
    CSSLang->add_language_keyword("image");
    CSSLang->add_language_keyword("left");
    CSSLang->add_language_keyword("line-height");
    CSSLang->add_language_keyword("line-width");
    CSSLang->add_language_keyword("list-style-type");
    CSSLang->add_language_keyword("list-style-position");
    CSSLang->add_language_keyword("list-style-type");

    CSSLang->add_language_keyword("margin-left");
    CSSLang->add_language_keyword("margin-top");
    CSSLang->add_language_keyword("margin-bottom");
    CSSLang->add_language_keyword("margin-right");
    CSSLang->add_language_keyword("margin");
    CSSLang->add_language_keyword("max-width");
    CSSLang->add_language_keyword("max-height");
    CSSLang->add_language_keyword("ms-transform");

    CSSLang->add_language_keyword("min-height");
    CSSLang->add_language_keyword("min-width");
    CSSLang->add_language_keyword("opacity");
    CSSLang->add_language_keyword("outline-color");
    CSSLang->add_language_keyword("outline-style");
    CSSLang->add_language_keyword("outline-width");
    CSSLang->add_language_keyword("outline");
    CSSLang->add_language_keyword("overflow-x");
    CSSLang->add_language_keyword("overflow-y");
    CSSLang->add_language_keyword("overflow");

    CSSLang->add_language_keyword("padding");
    CSSLang->add_language_keyword("position");
    CSSLang->add_language_keyword("right");
    CSSLang->add_language_keyword("shadow");
    CSSLang->add_language_keyword("size");
    CSSLang->add_language_keyword("text");
    CSSLang->add_language_keyword("text");
    CSSLang->add_language_keyword("top");
    CSSLang->add_language_keyword("transition");
    CSSLang->add_language_keyword("vertical");
    CSSLang->add_language_keyword("visibility");
    CSSLang->add_language_keyword("webkit");
    CSSLang->add_language_keyword("width");

    //For code highlights and suggested text

    highlightedTerm = NULL;
    suggestedCompilerTerms.clear();
    highlightedTermXPos = 0;
    highlightedTermYPos = 0;
    documentationIsBeingShown = false;
    codeBeingSuggested = false;
    iSuggestedStartPos = 0;
    iSuggestionPos = 0;
    suggestedTextMaxInViewCount = 0;
    maxSuggestedTextWidth = 0;
    //End of suggested text/code highlights related variables
}

GPE_Syntax_Highlighter::~GPE_Syntax_Highlighter()
{

}

GPE_Gui_Engine_Language * GPE_Syntax_Highlighter::add_programming_language(std::string langName, std::string langShortName, bool useDefaultSymbols, bool isCodingLanguage,bool isDefaultCodeLanguage)
{
    GPE_Gui_Engine_Language * newLangauge = new GPE_Gui_Engine_Language(langName, langShortName,isCodingLanguage, useDefaultSymbols);
    editorLanguages.push_back(newLangauge);
    return newLangauge;
}

bool GPE_Syntax_Highlighter::is_reserved_word(std::string strIn)
{
    return true;
}

bool GPE_Syntax_Highlighter::is_compilable_word(std::string strIn)
{
    GPE_Compiler_Term * tTerm = NULL;
    int i = 0;
    if( defaultLanguage!=NULL )
    {
        return defaultLanguage->is_compilable_word(strIn);
    }
    return true;
}


bool GPE_Syntax_Highlighter::charIsSymbol(char  charIn)
{
    if( defaultLanguage!=NULL )
    {
        return defaultLanguage->charIsSymbol(charIn);
    }
    return false;
}

int GPE_Syntax_Highlighter::firstSymbolInString(std::string inString, int pos)
{
    int returnVal = -1;
    if( defaultLanguage!=NULL )
    {
        return defaultLanguage->firstSymbolInString(inString, pos);

    }
    return returnVal;
}

std::string GPE_Syntax_Highlighter::find_project_function(std::string strBeingChecked, int startPos)
{
    GPE_Compiler_Term * tTerm = NULL;
    int strSize = (int)strBeingChecked.size();
    if( strSize>0 && strSize > startPos)
    {
        std::string strToSearch = "";
        int tempStrSize = 0;
        int j = 0;
        bool allMatched = false;

        for( int i = (int)activeProjectFunctions.size()-1; i>=0; i--)
        {
            tTerm= activeProjectFunctions[i];
            if( tTerm!=NULL)
            {
                strToSearch = tTerm->termString;

                tempStrSize = strToSearch.size();
                if(strSize >=  startPos+tempStrSize && tempStrSize>0)
                {
                    allMatched = true;
                    for( j = 0; j < tempStrSize; j++)
                    {
                        if(strBeingChecked[startPos+j]!=strToSearch[j])
                        {
                            allMatched = false;
                        }
                    }
                    if( allMatched ==true)
                    {
                        if( strSize >= startPos+tempStrSize)
                        {
                            //if( char_is_alnum(strBeingChecked[startPos+tempStrSize],false,false)==false )
                            {
                                return strToSearch;
                            }
                        }
                    }
                }
            }
        }
    }
    return "";
}

std::string GPE_Syntax_Highlighter::find_project_keyword(std::string strBeingChecked, int startPos)
{
    GPE_Compiler_Term * tTerm = NULL;
    int strSize = (int)strBeingChecked.size();
    if( strSize>0 && strSize > startPos)
    {
        std::string strToSearch = "";
        int tempStrSize = 0;
        int j = 0;
        bool allMatched = false;
        for( int i = (int)activeProjectKeywords.size()-1; i>=0; i--)
        {
            tTerm= activeProjectKeywords[i];
            if( tTerm!=NULL)
            {
                strToSearch = tTerm->termString;

                tempStrSize = strToSearch.size();
                if(strSize >=  startPos+tempStrSize && tempStrSize>0)
                {
                    allMatched = true;
                    for( j = 0; j < tempStrSize; j++)
                    {
                        if(strBeingChecked[startPos+j]!=strToSearch[j])
                        {
                            allMatched = false;
                        }
                    }
                    if( allMatched ==true)
                    {
                        if( strSize >= startPos+tempStrSize)
                        {
                            if( char_is_alnum(strBeingChecked[startPos+tempStrSize],false,false)==false )
                            {
                                return strToSearch;
                            }
                        }
                    }
                }
            }
        }
    }
    return "";
}

std::string GPE_Syntax_Highlighter::find_gpe_datatype(std::string strBeingChecked, int startPos)
{
    if( defaultLanguage!=NULL )
    {
        return defaultLanguage->find_gpe_datatype( strBeingChecked, startPos);
    }
    return "";
}


std::string GPE_Syntax_Highlighter::find_gpe_function(std::string strBeingChecked, int startPos)
{
    if( defaultLanguage!=NULL )
    {
        return defaultLanguage->find_gpe_function( strBeingChecked, startPos);
    }
    return "";
}


std::string GPE_Syntax_Highlighter::find_gpe_keyword(std::string strBeingChecked, int startPos)
{
    if( defaultLanguage!=NULL )
    {
        return defaultLanguage->find_gpe_keyword( strBeingChecked, startPos);
    }
    return "";
}


std::string GPE_Syntax_Highlighter::find_gpe_variable(std::string strBeingChecked, int startPos)
{
    if( defaultLanguage!=NULL )
    {
        return defaultLanguage->find_gpe_variable( strBeingChecked, startPos);
    }
    return "";
}

GPE_Compiler_Term * GPE_Syntax_Highlighter::find_matching_variable(std::string nameIn)
{
    if( defaultLanguage!=NULL )
    {
        return defaultLanguage->find_matching_variable( nameIn );
    }
    return NULL;
}

GPE_Compiler_Term * GPE_Syntax_Highlighter::find_matching_function(std::string nameIn)
{
    if( defaultLanguage!=NULL )
    {
        return defaultLanguage->find_matching_function( nameIn );
    }
    return NULL;
}

bool GPE_Syntax_Highlighter::process_parameters_string( std::string paramToProcess)
{
    foundParameters.clear();
    defaultParameterLines.clear();
    functionParameterErrors.clear();
    newParametersString = "";
    parametersAreValid = true;
    std::string paramToCompile = "";
    int paramStrSize = (int)paramToProcess.size();
    if( paramStrSize > 0)
    {
        bool isinSingleQuote = false;
        bool isInDoubleQuote = false;
        int i = 0;
        int lastParamPos = 0;
        int foundEqualPos = -1;
        int lastParamThatHAsDefaultValue = -1;
        std::string foundParameterStr= "";
        for(  i = 0; i < paramStrSize; i++)
        {
            if( !isinSingleQuote && paramToProcess[i]=='"' )
            {
                isInDoubleQuote = !isInDoubleQuote;
            }
            else if(!isInDoubleQuote && paramToProcess[i]=='\'')
            {
                isinSingleQuote = !isinSingleQuote;
            }
            else if( !isInDoubleQuote && !isinSingleQuote)
            {
                if( paramToProcess[i]==',')
                {
                    if( i!=lastParamPos)
                    {
                        foundParameterStr = get_substring(paramToProcess,lastParamPos,i-lastParamPos );
                        foundParameters.push_back(foundParameterStr);
                        lastParamPos = i+1;
                    }
                    else
                    {
                        parametersAreValid = false;
                        functionParameterErrors.push_back( "Invalid Comma found at position 0 of parameters string");
                    }
                }
            }
        }
        if( lastParamPos < (int)paramToProcess.size() )
        {
            foundParameterStr = get_substring(paramToProcess,lastParamPos );
            foundParameters.push_back(foundParameterStr);
        }

        if( parametersAreValid)
        {
            std::string parameterKey = "";
            std::string parameterValue = "";
            std::string newParameterLine = "";
            lastParamThatHAsDefaultValue = (int)foundParameters.size();
            for( i = (int)foundParameters.size()-1; i >=0; i--)
            {
                paramToCompile = foundParameters[i];
                foundEqualPos = paramToCompile.find("=",0);
                if( foundEqualPos > 0)
                {
                    if( lastParamThatHAsDefaultValue==i+1)
                    {
                        parameterKey = get_substring(paramToCompile,0,foundEqualPos );
                        parameterKey = trim_left_inplace(parameterKey);
                        parameterKey = trim_right_inplace(parameterKey);

                        parameterValue = get_substring(paramToCompile,foundEqualPos+1 );
                        parameterValue = trim_left_inplace(parameterValue);
                        parameterValue = trim_right_inplace(parameterValue);

                       if( is_alnum(parameterKey,false,true) && (int)parameterKey.size() >0 )
                       {
                            if( (int)parameterValue.size() >0 )
                            {
                                if( is_compilable_word(parameterKey) )
                                {
                                    if( i==0)
                                    {
                                        newParametersString = parameterKey+newParametersString;
                                    }
                                    else
                                    {
                                        newParametersString = ","+parameterKey+newParametersString;
                                    }
                                    newParameterLine = "if("+parameterKey+"==undefined) "+parameterKey+" = "+parameterValue+";\n";
                                    defaultParameterLines.insert(defaultParameterLines.begin()+0,newParameterLine);
                                    lastParamThatHAsDefaultValue = i;
                                }
                                else
                                {
                                    functionParameterErrors.push_back(  "Illegal parameter entered is a GPE engine variable ["+parameterKey+"]");
                                    parametersAreValid = false;
                                    break;
                                }
                            }
                            else
                            {
                                functionParameterErrors.push_back(  "Illegal parameter value ["+parameterValue+"] listed");
                                parametersAreValid = false;
                                break;
                            }
                        }
                        else
                        {
                            functionParameterErrors.push_back(  "Illegal parameter ["+parameterKey+"] listed");
                            parametersAreValid = false;
                            break;
                        }
                    }
                    else
                    {
                        parametersAreValid = false;
                        functionParameterErrors.push_back(  "Default Parameter value not allowed at position ["+int_to_string(i)+"] for ["+paramToCompile+"]" );
                        break;
                    }
                }
                else if(lastParamThatHAsDefaultValue!=-1 && lastParamThatHAsDefaultValue!=i+1)
                {
                    parametersAreValid = false;
                    functionParameterErrors.push_back(  "Default Parameter value NEEDED @ position ["+int_to_string(i)+"] for ["+paramToCompile+"]" );
                    break;
                }
                else
                {
                    if( is_compilable_word(paramToCompile) )
                    {
                        newParametersString = parameterKey+","+paramToCompile;
                        lastParamThatHAsDefaultValue = i;
                    }
                    else
                    {
                        functionParameterErrors.push_back( "Illegal parameter entered is a GPE engine variable ["+paramToCompile+"]" );
                        parametersAreValid = false;
                        break;
                    }
                }
            }
        }
        else
        {
            newParametersString = "";
        }
    }
    if( !parametersAreValid)
    {
        foundParameters.clear();
        defaultParameterLines.clear();
        newParametersString = "";
    }
    return parametersAreValid;
}

void GPE_Syntax_Highlighter::begin_compiling()
{
    compilerErrors.clear();
    compilerWarnings.clear();
    defaultParameterLines.clear();
    newParametersString = "";
    parametersAreValid = true;
}


void GPE_Syntax_Highlighter::render_code_highlights(GPE_Renderer * renderTarget,bool forceRedraw)
{
    if( highlightedTerm!=NULL)
    {
        if( renderTarget==NULL)
        {
            renderTarget = MAIN_RENDERER;
        }
        if( renderTarget==NULL)
        {
            return;
        }
        SDL_RenderSetViewport(renderTarget->get_renderer(),NULL);
        renderTarget->reset_viewpoint();
        std::string fullPhraseToRender;
        std::string fullTermScope = GPE_MAIN_HIGHLIGHTER->highlightedTerm->termScope;
        if( (int)fullTermScope.size()>1 )
        {
            fullTermScope = "Scope: "+fullTermScope;
        }
        if( GPE_MAIN_HIGHLIGHTER->highlightedTerm->termType==CTERM_FUNCTION)
        {
            fullPhraseToRender = GPE_MAIN_HIGHLIGHTER->highlightedTerm->termFunctionReturnType+" "+GPE_MAIN_HIGHLIGHTER->highlightedTerm->termString+"("+GPE_MAIN_HIGHLIGHTER->highlightedTerm->get_parameters()+")";
        }
        else
        {
            fullPhraseToRender = GPE_MAIN_HIGHLIGHTER->highlightedTerm->termString;
        }

        int highlightedTermWidth  = 0, highlightedTermHeight =0;
        int widestStringSize = std::max( (int)fullPhraseToRender.size(), (int)GPE_MAIN_HIGHLIGHTER->highlightedTerm->termDescription.size() );
        widestStringSize = std::max( (int)fullTermScope.size(), widestStringSize);

        FONT_TEXTINPUT->get_metrics("A",&highlightedTermWidth, &highlightedTermHeight);
        highlightedTermWidth*=widestStringSize;

        if( GPE_MAIN_HIGHLIGHTER->highlightedTermXPos+32+highlightedTermWidth > SCREEN_WIDTH)
        {
            GPE_MAIN_HIGHLIGHTER->highlightedTermXPos = SCREEN_WIDTH - 64-highlightedTermWidth;
        }
        if( GPE_MAIN_HIGHLIGHTER->highlightedTermXPos< 0)
        {
            GPE_MAIN_HIGHLIGHTER->highlightedTermXPos = 0;
        }

        render_rectangle(renderTarget, GPE_MAIN_HIGHLIGHTER->highlightedTermXPos, GPE_MAIN_HIGHLIGHTER->highlightedTermYPos,
                         GPE_MAIN_HIGHLIGHTER->highlightedTermXPos+highlightedTermWidth+64, GPE_MAIN_HIGHLIGHTER->highlightedTermYPos+(GPE_AVERAGE_LINE_HEIGHT*3),GPE_MAIN_TEMPLATE->PopUp_Box_Color,false);

        render_new_text(renderTarget,GPE_MAIN_HIGHLIGHTER->highlightedTermXPos+32, GPE_MAIN_HIGHLIGHTER->highlightedTermYPos,fullPhraseToRender,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_TERM_NAME,FA_LEFT,FA_TOP,255 );
        render_new_text(renderTarget,GPE_MAIN_HIGHLIGHTER->highlightedTermXPos+32, GPE_MAIN_HIGHLIGHTER->highlightedTermYPos+GPE_AVERAGE_LINE_HEIGHT,GPE_MAIN_HIGHLIGHTER->highlightedTerm->termDescription,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_TERM_DESCRIPTION,FA_LEFT,FA_TOP,255 );

        if( (int)fullTermScope.size()>1 )
        {
            render_new_text(renderTarget,highlightedTermXPos+32, highlightedTermYPos+GPE_AVERAGE_LINE_HEIGHT*2,fullTermScope,GPE_MAIN_TEMPLATE->PopUp_Box_Font_Color,FONT_TERM_SCOPE,FA_LEFT,FA_TOP,255 );
        }

        render_rectangle(renderTarget, highlightedTermXPos, highlightedTermYPos,
                         highlightedTermXPos+highlightedTermWidth+64, highlightedTermYPos+GPE_AVERAGE_LINE_HEIGHT*3,GPE_MAIN_TEMPLATE->PopUp_Box_Border_Color,true);
    }
}
