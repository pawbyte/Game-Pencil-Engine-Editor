/*
pawgui_syntax_handler.cpp
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
#include "../syntax_handler/pawgui_syntax_handler.h"
#include "../themes/pawgui_themes.h"

namespace pawgui
{
    syntax_highlighter * main_syntax_highlighter = nullptr;

    syntax_compiler_term::syntax_compiler_term()
    {
        termstring = "";
        //termParameters = "";
        termDescription = "";
        termFunctionReturnType = "";
        termType = -1;
        termParametersstring = "";
        termScope = "";
    }

    syntax_compiler_term::syntax_compiler_term(std::string tName,std::string tParameters, std::string tFunctionReturnType, std::string tDescription,int tType, std::string tScope)
    {
        termScope = tScope;
        termstring = tName;
        //termParameters = tParameters;
        termDescription = tDescription;
        termType = tType;
        termFunctionReturnType = tFunctionReturnType;
        termParametersstring = tParameters;
        if( (int)tParameters.size() > 0 )
        {
            int commaPos = tParameters.find_first_of(",");
            std::string valstring = "";
            while( commaPos!=(int)std::string::npos )
            {
                commaPos=tParameters.find_first_of(",");
                valstring = stg_ex::split_first_string(tParameters,',');
                termParameters.push_back(valstring);
            }
            termParameters.push_back( tParameters );
        }
    }

    syntax_compiler_term::~syntax_compiler_term()
    {

    }

    std::string syntax_compiler_term::get_parameters()
    {
        return termParametersstring;
    }

    syntax_language::syntax_language(std::string langName, std::string langShortName, int langId, bool projectCodeLanguage, bool useDefaultSymbols)
    {
        languageId = langId;
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

    syntax_language::~syntax_language()
    {

    }


    bool syntax_language::add_language_variable(std::string nName, std::string nDescription,std::string  nScopre)
    {
        syntax_compiler_term * tTerm = nullptr;
        for( int i = (int)languageVariables.size()-1; i>=0; i--)
        {
            tTerm = languageVariables[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==nName)
                {
                    return false;
                }
            }
        }
        tTerm = new syntax_compiler_term(nName, "", "", nDescription, -1, nScopre);
        languageVariables.push_back( tTerm);
        return true;
    }

    bool syntax_language::add_language_constant(std::string nName, std::string nDescription,std::string  nParameters)
    {
        syntax_compiler_term * tTerm = nullptr;
        for( int i = (int)languageConstants.size()-1; i>=0; i--)
        {
            tTerm = languageConstants[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==nName)
                {
                    return false;
                }
            }
        }
        tTerm = new syntax_compiler_term(nName, nParameters, "", nDescription, -1);
        languageConstants.push_back( tTerm);
        return true;
    }

    bool syntax_language::add_language_function(std::string nName, std::string nDescription,std::string  nParameters, std::string tFunctionReturnType, std::string nScope)
    {
        syntax_compiler_term * tTerm = nullptr;
        for( int i = (int)languageFunctions.size()-1; i>=0; i--)
        {
            tTerm = languageFunctions[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==nName)
                {
                    return false;
                }
            }
        }
        if( (int)tFunctionReturnType.size()==0)
        {
            tFunctionReturnType = "void";
        }
        tTerm = new syntax_compiler_term(nName, nParameters, tFunctionReturnType,nDescription, cterm_function,nScope );
        languageFunctions.push_back( tTerm);
        return true;
    }

    bool syntax_language::add_language_class(std::string nName, std::string nDescription,std::string  nParameters)
    {
        syntax_compiler_term * tTerm = nullptr;
        for( int i = (int)languageClasses.size()-1; i>=0; i--)
        {
            tTerm = languageClasses[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==nName)
                {
                    return false;
                }
            }
        }
        tTerm = new syntax_compiler_term(nName, nParameters, "", nDescription, -1);
        languageClasses.push_back( tTerm);
        return true;
    }

    bool syntax_language::add_language_keyword(std::string nName, std::string nDescription,std::string  nScope)
    {
        syntax_compiler_term * tTerm = nullptr;
        for( int i = (int)languageKeywords.size()-1; i>=0; i--)
        {
            tTerm = languageKeywords[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==nName)
                {
                    return false;
                }
            }
        }
        tTerm = new syntax_compiler_term(nName, "", "",nDescription,-1,nScope);
        languageKeywords.push_back( tTerm);
        return true;
    }

    bool syntax_language::charIsSymbol(char  charIn)
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


    int syntax_language::firstSymbolInstring(std::string instring, int pos)
    {
        int returnVal = -1;

        int maxLoop = symbolLibrary.size();
        for(int i=0; i<maxLoop; i+=1)
        {
            int symPos = instring.find(symbolLibrary[i],pos);
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

    bool syntax_language::is_compilable_word(std::string str_in)
    {
        int i = 0;
        syntax_compiler_term * tTerm = nullptr;
        for( i = (int)languageKeywords.size()-1; i>=0; i--)
        {
            tTerm = languageKeywords[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==str_in)
                {
                    return false;
                }
            }
        }

        for( i = (int)languageVariables.size()-1; i>=0; i--)
        {
            tTerm = languageVariables[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==str_in)
                {
                    return false;
                }
            }
        }

        for( i = (int)languageClasses.size()-1; i>=0; i--)
        {
            tTerm = languageClasses[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==str_in)
                {
                    return false;
                }
            }
        }

        for( i = (int)languageDataTypes.size()-1; i>=0; i--)
        {
            tTerm = languageDataTypes[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==str_in)
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool syntax_language::add_language_data_type(std::string nName, std::string nDescription,std::string  nParameters)
    {
        syntax_compiler_term * tTerm = nullptr;
        for( int i = (int)languageDataTypes.size()-1; i>=0; i--)
        {
            tTerm = languageDataTypes[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring==nName)
                {
                    return false;
                }
            }
        }
        tTerm = new syntax_compiler_term(nName, nParameters, "", nDescription, -1);
        languageDataTypes.push_back( tTerm);
        return true;
    }

    std::string syntax_language::find_gpe_datatype(std::string strBeingChecked, int startPos)
    {
        syntax_compiler_term * tTerm = nullptr;
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
                if( tTerm!=nullptr)
                {
                    strToSearch = tTerm->termstring;
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
                                //if( stg_ex::char_is_alnum(strBeingChecked[startPos+tempStrSize],false,true)==false )
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

    std::string syntax_language::find_gpe_function(std::string strBeingChecked, int startPos)
    {
        syntax_compiler_term * tTerm = nullptr;
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
                if( tTerm!=nullptr)
                {
                    strToSearch = tTerm->termstring;
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
                                if( stg_ex::char_is_alnum(strBeingChecked[startPos+tempStrSize],false,true)==false )
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

    std::string syntax_language::find_gpe_keyword(std::string strBeingChecked, int startPos)
    {
        syntax_compiler_term * tTerm = nullptr;
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
                if( tTerm!=nullptr)
                {
                    strToSearch = tTerm->termstring;

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
                                if( stg_ex::char_is_alnum(strBeingChecked[startPos+tempStrSize],false,true)==false )
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

    std::string syntax_language::find_gpe_variable(std::string strBeingChecked, int startPos)
    {
        syntax_compiler_term * tTerm = nullptr;
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
                if( tTerm!=nullptr)
                {
                    strToSearch = tTerm->termstring;

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
                                if( stg_ex::char_is_alnum(strBeingChecked[startPos+tempStrSize],false,true)==false )
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

    syntax_compiler_term * syntax_language::find_matching_variable(std::string name_param)
    {
        syntax_compiler_term * tTerm = nullptr;
        for( int i = (int)languageVariables.size()-1; i>=0; i--)
        {
            tTerm= languageVariables[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring == name_param )
                {
                    return tTerm;
                }
            }
        }
        return nullptr;

    }

    syntax_compiler_term * syntax_language::find_matching_function(std::string name_param)
    {
        syntax_compiler_term * tTerm = nullptr;
        for( int i = (int)languageFunctions.size()-1; i>=0; i--)
        {
            tTerm= languageFunctions[i];
            if( tTerm!=nullptr)
            {
                if( tTerm->termstring == name_param )
                {
                    return tTerm;
                }
            }
        }
        return nullptr;
    }
    syntax_highlighter::syntax_highlighter()
    {
        newParametersstring = "";
        parametersAreValid = false;

        syntax_language * JSLang = defaultLanguage = add_programming_language("JavaScript","JS",pawgui::program_language_js, true,true,true );
        syntax_language * CPPLang  = add_programming_language("C++","CPP",pawgui::program_language_cpp, true,true,false );
        CPPLang->macroCommentChar = "#";


        //JS classes
        JSLang->add_language_class("activeObject");
        JSLang->add_language_class("GPE_string");
        JSLang->add_language_class("SuperVector");
        JSLang->add_language_class("window");
        JSLang->add_language_class("GPE_JSFile");
        JSLang->add_language_class("file_objectReader");
        JSLang->add_language_class("GPE_gamepad");
        JSLang->add_language_class("GPE_InputManager");
        JSLang->add_language_class("GPE_LogoDisplay");
        JSLang->add_language_class("GPE_MainGame");
        JSLang->add_language_class("game_scene");
        JSLang->add_language_class("asset_manager");
        JSLang->add_language_class("GPE_ResourceLoader");
        JSLang->add_language_class("GPE_MainMenuOption");
        JSLang->add_language_class("GPE_MainMenu");
        JSLang->add_language_class("GPE_gamepad");
        JSLang->add_language_class("gpe::shape_rect");
        JSLang->add_language_class("GPE_Gameanimation");
        JSLang->add_language_class("tilesheet");
        JSLang->add_language_class("GPE_Texture");
        JSLang->add_language_class("background");
        JSLang->add_language_class("layer_game_tile");
        JSLang->add_language_class("GPE_TileLayer");
        JSLang->add_language_class("GPE_LevelView");
        JSLang->add_language_class("game_scene");

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
        JSLang->add_language_function("abs","Returns 0 or a positive number","x","float","gpe");
        JSLang->add_language_function("add_object","Returns a new object on the same layer as the current object","objectType, x, y","gameObject","GameObject");
        JSLang->add_language_function("add_new_object","Returns a new object","objectType, x, y, object_layer_id","gameObject","gpe");
        JSLang->add_language_function("bound_number","Bounds x between min and max","minN, maxN","float","gpe");
        JSLang->add_language_function("camera_on","Returns true or false if the camera is on or -1 if the camera is invalid","camId","int","gpe");
        JSLang->add_language_function("center_camera","Centers camera at (x,y)","(camId, x,y","void","gpe");
        JSLang->add_language_function("check_collision_rect","Checks if two rectangles collide","rectA, rectB","bool","gpe");
        JSLang->add_language_function("check_collision_with_rect","Checks if a rectangle collides with a box","boxX, boxY, boxW, boxH, rectA","bool","gpe");
        JSLang->add_language_function("degree","Converts an angle in radians to degrees","x","float","gpe");
        JSLang->add_language_function("radian","Converts an angle in degrees to radians","x","float","gpe");
        JSLang->add_language_function("sign","Gets the sign of the variable","x","int","gpe");

        //input functions
        JSLang->add_language_function("gamepad_connected","Checks if the gamepad is conected","gamepad_id","bool","input");
        JSLang->add_language_function("gamepad_name","Returns the gamepad's name","gamepad_id,","string","input");
        JSLang->add_language_function("gamepad_get_object","Returns the gamepad object","gamepad_id,","GPE_gamepad","input");
        JSLang->add_language_function("gamepad_mapping","Returns the gamepad's mapping","gamepad_id","string","input");
        JSLang->add_language_function("gamepad_check_button","Checks if the gamepad's button is down","gamepad_id, button_id","int","input");
        JSLang->add_language_function("gamepad_check_button_down","Checks if the gamepad's button_id is down","gamepad_id, button_id","int","input");
        JSLang->add_language_function("gamepad_check_button_pressed","Checks if the gamepad's button_id was pressed","gamepad_id, button_id","int","input");
        JSLang->add_language_function("gamepad_check_button_released","Checks if the gamepad's button_id was released","gamepad_id, button_id","int","input");

        JSLang->add_language_function("check_kb_down","Checks if the keyboard button is down","key_id","bool","input");
        JSLang->add_language_function("check_kb_pressed","Checks if the keyboard button was pressed","key_id","bool","input");
        JSLang->add_language_function("check_kb_released","Checks if the keyboard button was released","key_id","bool","input");
        JSLang->add_language_function("check_keyboard","Checks if the keyboard button is down","key_id","bool","input");

        JSLang->add_language_function("check_mouse_down","Checks if the mouse button is down","button_id","bool","input");
        JSLang->add_language_function("check_mouse_pressed","Checks if the mouse button is down","button_id","bool","input");
        JSLang->add_language_function("check_mouse_released","Checks if the mouse button is down","button_id","bool","input");
        JSLang->add_language_function("check_mouse","Checks if the keyboard mouse is down","button_id","bool","input");

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

        JSLang->add_language_function("find_tile","Checks for the tileset of tile at (x,y)","layerToCheck, x_pos, y_posk","int","gpe");
        JSLang->add_language_function("find_tile_in_box","Checks if tile is located in box","layerToCheck, x1,  y1,  x2, y2, layerIdToCheck","int","gpe");
        JSLang->add_language_function("find_matching_tile","Checks if tile with MATCHING tileset exists at (x,y)","layerToCheck,  x_pos,  y_pos,  tileTypeToSearchFor","bool","gpe");
        JSLang->add_language_function("find_matching_tile_in_box","Checks if tile with MATCHING tileset exist in box","layerToCheck,  x1,  y1,  x2, y2,  tileTypeToSearchFor","bool","gpe");

        JSLang->add_language_function("find_furthest_object","Finds the furthest object away from (x,y)","theObjectType, xIn, yIn, avoidObjId, layerIdToCheck","GameObject","gpe");
        JSLang->add_language_function("find_nearest_object","Finds the nearest object away from (x,y)","theObjectType, xIn, yIn, avoidObjId, layerIdToCheck","GameObject","gpe");
        JSLang->add_language_function("get_camera_height","Get the height of the camera","cameraId","int","gpe");
        JSLang->add_language_function("get_camera_info","Get the height of the camera","cameraId","CameraData","gpe");
        JSLang->add_language_function("get_camera_width","Get the height of the camera","cameraId","int","gpe");
        JSLang->add_language_function("get_camera_x","Get the x-value of the camera","cameraId","int","gpe");
        JSLang->add_language_function("get_camera_y","Get the y-value of the camera","cameraId","int","gpe");
        JSLang->add_language_function("get_camera_gpe::screen_height","Get the height of the camera on screen","cameraId","int","gpe");
        JSLang->add_language_function("get_camera_gpe::screen_width","Get the height of the camera on screen","cameraId","int","gpe");
        JSLang->add_language_function("get_camera_screen_x","Get the height of the camera on screen","cameraId","int","gpe");
        JSLang->add_language_function("get_camera_screen_y","Get the height of the camera on screen","cameraId","int","gpe");
        JSLang->add_language_function("get_current_camera","Gets the id of the current camera being shown","","int","gpe");
        JSLang->add_language_function("get_current_camera_info","Get an object containing information of the camera","","CameraData","gpe");
        JSLang->add_language_function("get_background","Gets the background texture id","bg_id","int","gpe");
        JSLang->add_language_function("get_direction","Returns the direction from (x1,y1) and (x2,y2)","x1, y1, x2, y2","int","gpe");
        JSLang->add_language_function("get_distance","Returns the distance between (x1,y1) and (x2,y2)","x1, y1, x2, y2","int","gpe");
        JSLang->add_language_function("get_mouse_position_x","Gets the global mouse x-position","","float","gpe");
        JSLang->add_language_function("get_mouse_position_y","Gets the global mouse y-position","","float","gpe");
        JSLang->add_language_function("load_data","Loads data pack","dataIdentifier, dataTitle","string","gpe");
        JSLang->add_language_function("move_camera","Moves camera to (x,y)","(camId, x,y","void","gpe");
        JSLang->add_language_function("setup_camera","Moves camera to (x,y) with sizing (w,h)","camId, x,y, w, h","void","gpe");
        JSLang->add_language_function("split_screen","Splits the scren up to 8 cameras","screenTotal, verticallySplit, useBlanks","bool","gpe");

        JSLang->add_language_function("font_is_monospaced","Checks if the font assigned to fontId is monospaced. Returns a blank string if out of bounds.","fontId","string","gpe");
        JSLang->add_language_function("get_font_family","Get the FamilyName of the font assigned to fontId. Returns a blank string if out of bounds.","fontId","string","gpe");
        JSLang->add_language_function("get_font_height","Get the estimated height of the font assigned to fontId. Returns a blank string if out of bounds.","fontId,words","string","gpe");
        JSLang->add_language_function("get_font_size","Get the size(pt) of the font assigned to fontId. Returns a blank string if out of bounds.","fontId","string","gpe");
        JSLang->add_language_function("get_font_width","Get the estimated width of the font assigned to fontId. Returns a blank string if out of bounds.","fontId,words","string","gpe");

        JSLang->add_language_function("get_animation_width","Get the width of the animation assigned to animation_id. Returns 0 if out of bounds.","animation_id","int","gpe");
        JSLang->add_language_function("get_animation_height","Get the height of the animation assigned to animation_id. Returns 0 if out of bounds","animation_id","int","gpe");
        JSLang->add_language_function("get_animation_length","Get the length of the animation assigned to animation_id. Returns 0 if out of bounds","animation_id","int","gpe");
        JSLang->add_language_function("get_animation_object","Gets the actual animation object attached to animation_id. Returns is_null if out of bounds","animation_id","Gameanimation","gpe");

        JSLang->add_language_function("get_texture_width","Get the width of the texture assigned to textureId. Returns 0 if out of bounds.","textureId","int","gpe");
        JSLang->add_language_function("get_texture_height","Get the height of the texture assigned to textureId. Returns 0 if out of bounds","textureId","int","gpe");
        JSLang->add_language_function("get_texture_object","Get the HTML5 Image of the texture assigned to textureId. Returns 0 if out of bounds","textureId","Image","gpe");

        JSLang->add_language_function("disable_camera","Disables a camera","cameraId","void","gpe");

        JSLang->add_language_function("game_reset","Resets the game game","","void","gpe");
        JSLang->add_language_function("get_camera_mouse_position_x","Gets the camera relative mouse x-position","cameraId","int","gpe");
        JSLang->add_language_function("get_camera_mouse_position_y","Gets the camera relative mouse y-position","cameraId","int","gpe");
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
        JSLang->add_language_function("get_animation","Returns animation-id of game object","","int","GameObject");
        JSLang->add_language_function("getx","Returns x-position of game object","","float","GameObject");
        JSLang->add_language_function("getx2","Returns x2-position of game object","","float","GameObject");
        JSLang->add_language_function("gety","Returns y-position of game object","","float","GameObject");
        JSLang->add_language_function("gety2","Returns y2-position of game object","","float","GameObject");
        JSLang->add_language_function("gravity_use","Activates built in gravity","","void", "GameObject");
        JSLang->add_language_function("init_animation","Initiates the game object's animation on creation","nextanimation","void","GameObject");
        JSLang->add_language_function("make_view_dependent","Makes this game object camera dependent","","void","GameObject");
        JSLang->add_language_function("make_view_independent","Makes this game object camera independent","","void","GameObject");
        JSLang->add_language_function("move_left","Moves this object left by m pixels","m","void","GameObject");
        JSLang->add_language_function("move_right","Moves this object right by m pixels","m","void","GameObject");
        JSLang->add_language_function("move_up","Moves this object up by m pixels","m","void","GameObject");
        JSLang->add_language_function("move_down","Moves this object down by m pixels","m","void","GameObject");
        JSLang->add_language_function("move_towards","Moves this towards the point (x,y) at speed","x,y,speed","void","GameObject");
        JSLang->add_language_function("under_mouse","Checks if this object is under the mouse via the cameraId(defaults to 0).","cameraId","void","GameObject");
        JSLang->add_language_function("set_animation","Sets the animation of this object","new_animation","void","GameObject");

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
        JSLang->add_language_function("position_collides","Returns true or false if objectOne collides at (x,y)","objOne,  xIn,  yIn,  otherObjectType, layer_id ","bool","gpe");
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
        JSLang->add_language_function("gpe::gcanvas->render_line","Renders a line","x1, y1, x2, y2, lineColor, line_width, renderImmediately","void","gpe");
        JSLang->add_language_function("render_rectangle","Renders a rectangle","x1, y1, x2, y2,rectColor, outline,rectLineWidth,renderImmediately","void","gpe");
        JSLang->add_language_function("render_self","Renders this object onto scene","","void","gpe");
        JSLang->add_language_function("render_animation_rotated","Renders a rotated animation","animation_idIn, frameNumb, xOn, yOn, rotationAngle,scaleX, scaleY","void","gpe");

        JSLang->add_language_function("render_animation","Renders a animation","animation_idIn, frameNumb, xOn, yOn, scaleX, scaleY","void","gpe");
        JSLang->add_language_function("render_square","Renders a square","xOn, yOn, squareSize,squareColor, outline,squareLineWidth,renderImmediately","void","gpe");
        JSLang->add_language_function("render_text","Renders text","fontIdIn, xOn, yOn, text_to_render, fontRenderColor, fontHalign, fontValign","void","gpe");
        JSLang->add_language_function("render_texture_rotated","Renders a rotated texture","texureIdIn, xDraw, yDraw, width, height, angle","void","gpe");
        JSLang->add_language_function("render_texture_scaled","Renders a scaled texture","texureIdIn, xDraw, yDraw, width, height","void","gpe");
        JSLang->add_language_function("render_texture","Renders a texture","texureIdIn,  xDraw, yDraw","void","gpe");
        JSLang->add_language_function("render_triangle","Renders a triangle","x1, y1, x2, y2, x3, y3, triangleColor, outline, outlineSize, renderImmediately","void","gpe");
        JSLang->add_language_function("render_wrapped_text","Renders a wrapped text","fontIdIn, xOn, yOn, text_to_render,fontRenderColor, maxWidth, lineHeight","void","gpe");
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
        JSLang->add_language_function("acos","Returns the arccosine of x.","radians x","float","Math");
        JSLang->add_language_function("asin","Returns the arcsine  of x.","radians x","float","Math");
        JSLang->add_language_function("atan","Returns the arctangent of x as a numeric value between -PI/2 and PI/2 radians","x","float","Math");
        JSLang->add_language_function("atan2","Returns the arctangent of the quotient of its arguments","y,x","float","Math");

        JSLang->add_language_function("exp","Returns E ^X","x","float","Math");
        JSLang->add_language_function("pow","Returns X ^Y","x,y","float","Math");
        JSLang->add_language_function("log","Returns the natural logarithm (base E) of x","x","int","Math");
        JSLang->add_language_function("cos","Returns cos( (radians)x )","(radians)x","float","Math");
        JSLang->add_language_function("sin","Returns sin( (radians)x )","(radians)x","float","Math");
        JSLang->add_language_function("tan","Returns tan( (angle)x )","x,y","float","Math");
        JSLang->add_language_function("min","Returns the smallest NUMBER found","x,y,z...N","float","Math");
        JSLang->add_language_function("max","Returns the highest NUMBER found","x,y,z...N","float","Math");
        JSLang->add_language_function("sqrt","Returns the square root of x","x","float","Math");
        JSLang->add_language_function("random","Returns a random number from 0 to 1","","float","Math");
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
        JSLang->add_language_keyword("gpe::fa_left");
        JSLang->add_language_keyword("gpe::fa_center");
        JSLang->add_language_keyword("gpe::fa_right");
        JSLang->add_language_keyword("gpe::fa_top");
        JSLang->add_language_keyword("gpe::fa_middle");
        JSLang->add_language_keyword("gpe::fa_bottom");
        JSLang->add_language_keyword("for");
        JSLang->add_language_keyword("gpe");
        JSLang->add_language_keyword("GPE");
        JSLang->add_language_keyword("gpr");
        JSLang->add_language_keyword("GPR");
        JSLang->add_language_keyword("if");
        JSLang->add_language_keyword("Infinity");
        JSLang->add_language_keyword("input");
        JSLang->add_language_keyword("Input");
        JSLang->add_language_keyword("new");

        JSLang->add_language_keyword("other");
        JSLang->add_language_keyword("pencil");
        JSLang->add_language_keyword("return");
        JSLang->add_language_keyword("switch");
        JSLang->add_language_keyword("this");
        JSLang->add_language_keyword("undefined");
        JSLang->add_language_constant("UNDEFINED");
        JSLang->add_language_keyword("while");

        //JS constants
        JSLang->add_language_constant("is_null");
        JSLang->add_language_constant("GPE_SETTINGS_ENTRY_LEVEL_LOCATION");
        JSLang->add_language_constant("pi");
        JSLang->add_language_constant("PI");
        JSLang->add_language_constant("isNULL");
        JSLang->add_language_constant("is_null");
        JSLang->add_language_constant("is_null");
        JSLang->add_language_constant("NaN");
        JSLang->add_language_constant("nullptr");
        JSLang->add_language_constant("nullptr");
        JSLang->add_language_constant("true");
        JSLang->add_language_constant("TRUE");
        //JS Keyboard (kb_ ) constants
        JSLang->add_language_constant("kb_0");
        JSLang->add_language_constant("kb_1");
        JSLang->add_language_constant("kb_2");
        JSLang->add_language_constant("kb_3");
        JSLang->add_language_constant("kb_4");
        JSLang->add_language_constant("kb_5");
        JSLang->add_language_constant("kb_6");
        JSLang->add_language_constant("kb_7");
        JSLang->add_language_constant("kb_8");
        JSLang->add_language_constant("kb_9");

        JSLang->add_language_constant("kb_a");
        JSLang->add_language_constant("kb_b");
        JSLang->add_language_constant("kb_c");
        JSLang->add_language_constant("kb_d");
        JSLang->add_language_constant("kb_e");
        JSLang->add_language_constant("kb_f");
        JSLang->add_language_constant("kb_g");
        JSLang->add_language_constant("kb_h");
        JSLang->add_language_constant("kb_i");
        JSLang->add_language_constant("kb_j");
        JSLang->add_language_constant("kb_k");
        JSLang->add_language_constant("kb_l");
        JSLang->add_language_constant("kb_m");
        JSLang->add_language_constant("kb_n");
        JSLang->add_language_constant("kb_o");
        JSLang->add_language_constant("kb_p");
        JSLang->add_language_constant("kb_q");
        JSLang->add_language_constant("kb_r");
        JSLang->add_language_constant("kb_s");
        JSLang->add_language_constant("kb_t");
        JSLang->add_language_constant("kb_u");
        JSLang->add_language_constant("kb_v");
        JSLang->add_language_constant("kb_w");
        JSLang->add_language_constant("kb_x");
        JSLang->add_language_constant("kb_y");
        JSLang->add_language_constant("kb_z");

        JSLang->add_language_constant("kb_np0");
        JSLang->add_language_constant("kb_np1");
        JSLang->add_language_constant("kb_np2");
        JSLang->add_language_constant("kb_np3");
        JSLang->add_language_constant("kb_np4");
        JSLang->add_language_constant("kb_np5");
        JSLang->add_language_constant("kb_np6");
        JSLang->add_language_constant("kb_np7");
        JSLang->add_language_constant("kb_np8");
        JSLang->add_language_constant("kb_np9");

        JSLang->add_language_constant("kb_f1");
        JSLang->add_language_constant("kb_f2");
        JSLang->add_language_constant("kb_f3");
        JSLang->add_language_constant("kb_f4");
        JSLang->add_language_constant("kb_f5");
        JSLang->add_language_constant("kb_f6");
        JSLang->add_language_constant("kb_f7");
        JSLang->add_language_constant("kb_f8");
        JSLang->add_language_constant("kb_f9");
        JSLang->add_language_constant("kb_f10");
        JSLang->add_language_constant("kb_f11");
        JSLang->add_language_constant("kb_f12");

        JSLang->add_language_constant("kb_backspace");
        JSLang->add_language_constant("kb_tab");
        JSLang->add_language_constant("kb_enter");
        JSLang->add_language_constant("kb_pause");
        JSLang->add_language_constant("kb_caps");
        JSLang->add_language_constant("kb_esc");
        JSLang->add_language_constant("kb_space");
        JSLang->add_language_constant("kb_page_up");
        JSLang->add_language_constant("kb_page_down");
        JSLang->add_language_constant("kb_end");
        JSLang->add_language_constant("kb_home");
        JSLang->add_language_constant("kb_left");
        JSLang->add_language_constant("kb_up");
        JSLang->add_language_constant("kb_right");
        JSLang->add_language_constant("kb_down");
        JSLang->add_language_constant("kb_insert");
        JSLang->add_language_constant("kb_delete");
        JSLang->add_language_constant("kb_shift");
        JSLang->add_language_constant("kb_ctrl");
        JSLang->add_language_constant("kb_alt");
        JSLang->add_language_constant("kb_plus");
        JSLang->add_language_constant("kb_period");
        JSLang->add_language_constant("kb_minus");
        JSLang->add_language_constant("kb_period");

        JSLang->add_language_constant("kb_multiply");
        JSLang->add_language_constant("kb_add");
        JSLang->add_language_constant("kb_substract");
        JSLang->add_language_constant("kb_decimal");
        JSLang->add_language_constant("kb_divide");
        JSLang->add_language_constant("kb_fslash");
        JSLang->add_language_constant("kb_gaccent");
        JSLang->add_language_constant("kb_obracket");
        JSLang->add_language_constant("kb_bslash");
        JSLang->add_language_constant("kb_cbracket");
        JSLang->add_language_constant("kb_squote");

        //JS Keyboard (gp_ ) constants
        JSLang->add_language_constant("gp_0");
        JSLang->add_language_constant("gp_1");
        JSLang->add_language_constant("gp_2");
        JSLang->add_language_constant("gp_3");
        JSLang->add_language_constant("gp_4");
        JSLang->add_language_constant("gp_5");
        JSLang->add_language_constant("gp_6");
        JSLang->add_language_constant("gp_7");
        JSLang->add_language_constant("gp_8");
        JSLang->add_language_constant("gp_9");

        JSLang->add_language_constant("gp_np0");
        JSLang->add_language_constant("gp_np1");
        JSLang->add_language_constant("gp_np2");
        JSLang->add_language_constant("gp_np3");
        JSLang->add_language_constant("gp_np4");
        JSLang->add_language_constant("gp_np5");
        JSLang->add_language_constant("gp_np6");
        JSLang->add_language_constant("gp_np7");
        JSLang->add_language_constant("gp_np8");
        JSLang->add_language_constant("gp_np9");

        JSLang->add_language_constant("gp_f1");
        JSLang->add_language_constant("gp_f2");
        JSLang->add_language_constant("gp_f3");
        JSLang->add_language_constant("gp_f4");
        JSLang->add_language_constant("gp_f5");
        JSLang->add_language_constant("gp_f6");
        JSLang->add_language_constant("gp_f7");
        JSLang->add_language_constant("gp_f8");
        JSLang->add_language_constant("gp_f9");
        JSLang->add_language_constant("gp_f10");
        JSLang->add_language_constant("gp_f11");
        JSLang->add_language_constant("gp_f12");

        JSLang->add_language_constant("gp_backspace");
        JSLang->add_language_constant("gp_tab");
        JSLang->add_language_constant("gp_enter");
        JSLang->add_language_constant("gp_pause");
        JSLang->add_language_constant("gp_caps");
        JSLang->add_language_constant("gp_esc");
        JSLang->add_language_constant("gp_space");
        JSLang->add_language_constant("gp_page_up");
        JSLang->add_language_constant("gp_page_down");
        JSLang->add_language_constant("gp_end");
        JSLang->add_language_constant("gp_home");
        JSLang->add_language_constant("gp_left");
        JSLang->add_language_constant("gp_up");
        JSLang->add_language_constant("gp_right");
        JSLang->add_language_constant("gp_down");
        JSLang->add_language_constant("gp_insert");
        JSLang->add_language_constant("gp_delete");
        JSLang->add_language_constant("gp_shift");
        JSLang->add_language_constant("gp_ctrl");
        JSLang->add_language_constant("gp_alt");
        JSLang->add_language_constant("gp_plus");
        JSLang->add_language_constant("gp_comma");
        JSLang->add_language_constant("gp_minus");
        JSLang->add_language_constant("gp_period");

        JSLang->add_language_constant("gp_multiply");
        JSLang->add_language_constant("gp_add");
        JSLang->add_language_constant("gp_substract");
        JSLang->add_language_constant("gp_decimal");
        JSLang->add_language_constant("gp_divide");
        JSLang->add_language_constant("gp_fslash");
        JSLang->add_language_constant("gp_gaccent");
        JSLang->add_language_constant("gp_obracket");
        JSLang->add_language_constant("gp_bslash");
        JSLang->add_language_constant("gp_cbracket");
        JSLang->add_language_constant("gp_squote");

        //JS Mouse Contants
        JSLang->add_language_constant("mb_center");
        JSLang->add_language_constant("mb_left");
        JSLang->add_language_constant("mb_middle");
        JSLang->add_language_constant("mb_right");

        //JS gamepad constants(gp_) [ BEGIN ]
        JSLang->add_language_constant("gp_anycontroller");
        JSLang->add_language_constant("gp_nocontroller");
        JSLang->add_language_constant("gp_anybutton");
        JSLang->add_language_constant("gp_nobutton");

        JSLang->add_language_constant("gp_joystick_left_x");
        JSLang->add_language_constant("gp_joystick_left_y");
        JSLang->add_language_constant("gp_joystick_right_x");
        JSLang->add_language_constant("gp_joystick_right_y");
        JSLang->add_language_constant("gp_joystick_dpad_x");
        JSLang->add_language_constant("gp_joystick_dpad_y");

        JSLang->add_language_constant("gp_left");
        JSLang->add_language_constant("gp_right");
        JSLang->add_language_constant("gp_up");
        JSLang->add_language_constant("gp_down");
        JSLang->add_language_constant("gkb_c0");
        JSLang->add_language_constant("gkb_c1");
        JSLang->add_language_constant("gkb_c2");
        JSLang->add_language_constant("gkb_c3");
        JSLang->add_language_constant("gp_l1");
        JSLang->add_language_constant("gp_l2");
        JSLang->add_language_constant("gp_l3");
        JSLang->add_language_constant("gp_r1");
        JSLang->add_language_constant("gp_r2");
        JSLang->add_language_constant("gp_r3");

        JSLang->add_language_constant("gp_home");
        JSLang->add_language_constant("gp_select");
        JSLang->add_language_constant("gp_share");
        JSLang->add_language_constant("gp_start");
        //gamepad constants(gp_) [ END ]

        //Legacy Keyboard constants(gp_) [ BEGIN ]
        JSLang->add_language_constant("gp_a");
        JSLang->add_language_constant("gp_b");
        JSLang->add_language_constant("gp_c");
        JSLang->add_language_constant("gp_d");
        JSLang->add_language_constant("gp_e");
        JSLang->add_language_constant("gp_f");
        JSLang->add_language_constant("gp_g");
        JSLang->add_language_constant("gp_h");
        JSLang->add_language_constant("gp_i");
        JSLang->add_language_constant("gp_j");
        JSLang->add_language_constant("gp_k");
        JSLang->add_language_constant("gp_l");
        JSLang->add_language_constant("gp_m");
        JSLang->add_language_constant("gp_n");
        JSLang->add_language_constant("gp_o");
        JSLang->add_language_constant("gp_p");
        JSLang->add_language_constant("gp_q");
        JSLang->add_language_constant("gp_r");
        JSLang->add_language_constant("gp_s");
        JSLang->add_language_constant("gp_t");
        JSLang->add_language_constant("gp_u");
        JSLang->add_language_constant("gp_v");
        JSLang->add_language_constant("gp_w");
        JSLang->add_language_constant("gp_x");
        JSLang->add_language_constant("gp_y");
        JSLang->add_language_constant("gp_z");
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
        JSLang->add_language_keyword("yield","","JavaScript Global");

        //Object key variables
        JSLang->add_language_variable("customComponents","An object's array of custom components created in editor", "GameObject" );
        JSLang->add_language_variable("direction","An objects direction in radians", "GameObject" );
        JSLang->add_language_variable("speed","An objects speed in pixels", "GameObject" );
        JSLang->add_language_variable("imageSpeed","An object's imageSpeed per game frame", "GameObject" );
        JSLang->add_language_variable("is_visible","A boolean if the object is visible", "GameObject" );
        JSLang->add_language_variable("animationFrame","The current frame of the object's current animation", "GameObject" );
        JSLang->add_language_variable("xPast","The object's x-coordinate from last frame", "GameObject" );
        JSLang->add_language_variable("yPast","The object's y-coordinate from last frame", "GameObject" );

        JSLang->add_language_variable("GPE_SETTINGS_APP_LOGO_LOCATION");
        JSLang->add_language_variable("GPE_SETTINGS_gpe::screen_width");
        JSLang->add_language_variable("GPE_SETTINGS_gpe::screen_height");
        JSLang->add_language_variable("GPE_SETTINGS_PROG_NAME");
        JSLang->add_language_variable("object_max_types");
        JSLang->add_language_variable("GPE_SETTINGS_SYSTEM_OS");
        JSLang->add_language_variable("GPE_GAME_PUBLISHER");
        JSLang->add_language_variable("GPE_GAME_DEVELOPER");
        JSLang->add_language_variable("GPE_game_objects_COUNT");
        JSLang->add_language_variable("GPE_SETTINGS_IS_DEBUGGING");
        JSLang->add_language_variable("GPE_SETTINGS_SHOW_FPS");
        JSLang->add_language_variable("GPE_SETTINGS_ENTRY_LEVEL_LOCATION");

        JSLang->add_language_variable("program_states");
        JSLang->add_language_variable("canvas");
        JSLang->add_language_variable("context");
        JSLang->add_language_variable("input","Global object for handling input calls","global");
        JSLang->add_language_variable("maxViews");
        JSLang->add_language_variable("EXTRA_CANVASES");
        JSLang->add_language_variable("preGuiCamvas");
        JSLang->add_language_variable("fileReader");
        JSLang->add_language_variable("program_stateId");
        JSLang->add_language_variable("state_current");

        //actual datatypes used/allowed
        JSLang->add_language_data_type("char");
        JSLang->add_language_data_type("float");
        JSLang->add_language_data_type("float");
        JSLang->add_language_data_type("grid");
        JSLang->add_language_data_type("int");
        JSLang->add_language_data_type("string");
        JSLang->add_language_data_type("vector");
        JSLang->add_language_data_type("var","","JavaScript Global");

        //Adds CSS language syntax and such
        syntax_language * CSSLang = add_programming_language("CSS","CSS", -1, true, false,false);
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

        highlightedTerm = nullptr;
        suggestedCompilerTerms.clear();
        highlightedTermXPos = 0;
        highlightedTermYPos = 0;
        documentationIsBeingShown = false;
        codeBeingSuggested = false;
        iSuggestedStartPos = 0;
        iSuggestionPos = 0;
        suggestedTextMaxInViewCount = 7;
        maxSuggestedTextWidth = 0;
        //End of suggested text/code highlights related variables
    }

    syntax_highlighter::~syntax_highlighter()
    {
        highlightedTerm = nullptr;
        suggestedCompilerTerms.clear();
    }

    syntax_language * syntax_highlighter::add_programming_language(std::string langName, std::string langShortName, int langId, bool useDefaultSymbols, bool isCodingLanguage,bool isDefaultCodeLanguage)
    {
        syntax_language * newLangauge = new syntax_language(langName, langShortName,langId, isCodingLanguage, useDefaultSymbols);
        editorLanguages.push_back(newLangauge);
        return newLangauge;
    }

    bool syntax_highlighter::is_reserved_word(std::string str_in)
    {
        return true;
    }

    bool syntax_highlighter::is_compilable_word(std::string str_in)
    {
        syntax_compiler_term * tTerm = nullptr;
        int i = 0;
        if( defaultLanguage!=nullptr )
        {
            return defaultLanguage->is_compilable_word(str_in);
        }
        return true;
    }


    bool syntax_highlighter::charIsSymbol(char  charIn)
    {
        if( defaultLanguage!=nullptr )
        {
            return defaultLanguage->charIsSymbol(charIn);
        }
        return false;
    }

    int syntax_highlighter::firstSymbolInstring(std::string instring, int pos)
    {
        int returnVal = -1;
        if( defaultLanguage!=nullptr )
        {
            return defaultLanguage->firstSymbolInstring(instring, pos);

        }
        return returnVal;
    }

    std::string syntax_highlighter::find_project_function(std::string strBeingChecked, int startPos)
    {
        syntax_compiler_term * tTerm = nullptr;
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
                if( tTerm!=nullptr)
                {
                    strToSearch = tTerm->termstring;

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
                                //if( stg_ex::char_is_alnum(strBeingChecked[startPos+tempStrSize],false,false)==false )
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

    std::string syntax_highlighter::find_project_keyword(std::string strBeingChecked, int startPos)
    {
        syntax_compiler_term * tTerm = nullptr;
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
                if( tTerm!=nullptr)
                {
                    strToSearch = tTerm->termstring;

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
                                if( stg_ex::char_is_alnum(strBeingChecked[startPos+tempStrSize],false,false)==false )
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

    std::string syntax_highlighter::find_gpe_datatype(std::string strBeingChecked, int startPos)
    {
        if( defaultLanguage!=nullptr )
        {
            return defaultLanguage->find_gpe_datatype( strBeingChecked, startPos);
        }
        return "";
    }


    std::string syntax_highlighter::find_gpe_function(std::string strBeingChecked, int startPos)
    {
        if( defaultLanguage!=nullptr )
        {
            return defaultLanguage->find_gpe_function( strBeingChecked, startPos);
        }
        return "";
    }


    std::string syntax_highlighter::find_gpe_keyword(std::string strBeingChecked, int startPos)
    {
        if( defaultLanguage!=nullptr )
        {
            return defaultLanguage->find_gpe_keyword( strBeingChecked, startPos);
        }
        return "";
    }


    std::string syntax_highlighter::find_gpe_variable(std::string strBeingChecked, int startPos)
    {
        if( defaultLanguage!=nullptr )
        {
            return defaultLanguage->find_gpe_variable( strBeingChecked, startPos);
        }
        return "";
    }

    syntax_compiler_term * syntax_highlighter::find_matching_variable(std::string name_param)
    {
        if( defaultLanguage!=nullptr )
        {
            return defaultLanguage->find_matching_variable( name_param );
        }
        return nullptr;
    }

    syntax_compiler_term * syntax_highlighter::find_matching_function(std::string name_param)
    {
        if( defaultLanguage!=nullptr )
        {
            return defaultLanguage->find_matching_function( name_param );
        }
        return nullptr;
    }

    int syntax_highlighter::get_language_count()
    {
        return (int)editorLanguages.size();
    }

    int syntax_highlighter::get_language_id_from_name( std::string name_param)
    {
        syntax_language * cLang = nullptr;
        for( int i = 0; i < (int)editorLanguages.size(); i++)
        {
            cLang = editorLanguages[i];
            if( cLang !=nullptr )
            {
                if( cLang->languageName == name_param || cLang->languageShortName == name_param )
                {
                    return cLang->languageId;
                }
            }
        }
        return -1;
    }

    syntax_language * syntax_highlighter::get_language_object( int langPosition)
    {
        if( langPosition >=0 && langPosition < (int)editorLanguages.size() )
        {
            return editorLanguages[langPosition];
        }
        return nullptr;
    }

    syntax_language * syntax_highlighter::get_language_object_from_id( int langId )
    {
        if( langId < 0)
        {
            return nullptr;
        }
        syntax_language * cLang = nullptr;
        for( int i = 0; i < (int)editorLanguages.size(); i++)
        {
            cLang = editorLanguages[i];
            if( cLang !=nullptr )
            {
                if( cLang->languageId == langId )
                {
                    return cLang;
                }
            }
        }
        return nullptr;
    }

    std::string syntax_highlighter::get_language_name_from_id( int langId )
    {
        syntax_language * cLang = nullptr;
        for( int i = 0; i < (int)editorLanguages.size(); i++)
        {
            cLang = editorLanguages[i];
            if( cLang !=nullptr )
            {
                if( cLang->languageId == langId )
                {
                    return cLang->languageName;
                }
            }
        }
        return "";
    }

    std::string syntax_highlighter::get_language_shortname_from_id( int langId)
    {
        syntax_language * cLang = nullptr;
        for( int i = 0; i < (int)editorLanguages.size(); i++)
        {
            cLang = editorLanguages[i];
            if( cLang !=nullptr )
            {
                if( cLang->languageId == langId )
                {
                    return cLang->languageShortName;
                }
            }
        }
        return "";
    }

    bool syntax_highlighter::process_parameters_string( std::string paramToProcess)
    {
        foundParameters.clear();
        defaultParameterLines.clear();
        functionParameterErrors.clear();
        newParametersstring = "";
        parametersAreValid = true;
        std::string paramToCompile = "";
        int paramStrSize = (int)paramToProcess.size();
        if( paramStrSize > 0)
        {
            bool isinSingleQuote = false;
            bool isInfloatQuote = false;
            int i = 0;
            int lastParamPos = 0;
            int foundEqualPos = -1;
            int lastParamThatHAsDefaultValue = -1;
            std::string foundParameterStr= "";
            for(  i = 0; i < paramStrSize; i++)
            {
                if( !isinSingleQuote && paramToProcess[i]=='"' )
                {
                    isInfloatQuote = !isInfloatQuote;
                }
                else if(!isInfloatQuote && paramToProcess[i]=='\'')
                {
                    isinSingleQuote = !isinSingleQuote;
                }
                else if( !isInfloatQuote && !isinSingleQuote)
                {
                    if( paramToProcess[i]==',')
                    {
                        if( i!=lastParamPos)
                        {
                            foundParameterStr = stg_ex::get_substring(paramToProcess,lastParamPos,i-lastParamPos );
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
                foundParameterStr = stg_ex::get_substring(paramToProcess,lastParamPos );
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
                            parameterKey = stg_ex::get_substring(paramToCompile,0,foundEqualPos );
                            parameterKey = stg_ex::trim_left_inplace(parameterKey);
                            parameterKey = stg_ex::trim_right_inplace(parameterKey);

                            parameterValue = stg_ex::get_substring(paramToCompile,foundEqualPos+1 );
                            parameterValue = stg_ex::trim_left_inplace(parameterValue);
                            parameterValue = stg_ex::trim_right_inplace(parameterValue);

                            if( stg_ex::is_alnum(parameterKey,false,true) && (int)parameterKey.size() >0 )
                            {
                                if( (int)parameterValue.size() >0 )
                                {
                                    if( is_compilable_word(parameterKey) )
                                    {
                                        if( i==0)
                                        {
                                            newParametersstring = parameterKey+newParametersstring;
                                        }
                                        else
                                        {
                                            newParametersstring = ","+parameterKey+newParametersstring;
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
                            functionParameterErrors.push_back(  "Default Parameter value not allowed at position ["+ stg_ex::int_to_string(i)+"] for ["+paramToCompile+"]" );
                            break;
                        }
                    }
                    else if(lastParamThatHAsDefaultValue!=-1 && lastParamThatHAsDefaultValue!=i+1)
                    {
                        parametersAreValid = false;
                        functionParameterErrors.push_back(  "Default Parameter value NEEDED @ position ["+stg_ex::int_to_string(i)+"] for ["+paramToCompile+"]" );
                        break;
                    }
                    else
                    {
                        if( is_compilable_word(paramToCompile) )
                        {
                            newParametersstring = parameterKey+","+paramToCompile;
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
                newParametersstring = "";
            }
        }
        if( !parametersAreValid)
        {
            foundParameters.clear();
            defaultParameterLines.clear();
            newParametersstring = "";
        }
        return parametersAreValid;
    }

    void syntax_highlighter::begin_compiling()
    {
        compilerErrors.clear();
        compilerWarnings.clear();
        defaultParameterLines.clear();
        newParametersstring = "";
        parametersAreValid = true;
    }

    void syntax_highlighter::clear_all()
    {
        clear_suggestions();
        clear_highlights();
    }

    void syntax_highlighter::clear_highlights()
    {
        highlightedTerm = nullptr;
        highlightedTermXPos = 0;
        highlightedTermYPos = 0;
        documentationIsBeingShown = false;
    }

    void syntax_highlighter::clear_suggestions()
    {
        highlightedTermXPos = 0;
        highlightedTermYPos = 0;
        codeBeingSuggested = false;
        iSuggestedStartPos = 0;
        iSuggestionPos = 0;
        suggestedCompilerTerms.clear();
    }

    void syntax_highlighter::render_code_highlights( )
    {
        if( highlightedTerm!=nullptr)
        {
            gpe::renderer_main->set_viewpoint( nullptr);
            gpe::renderer_main->reset_viewpoint( );
            std::string fullPhraseToRender;
            std::string fullTermScope = highlightedTerm->termScope;
            if( (int)fullTermScope.size()>1 )
            {
                fullTermScope = "Scope: "+fullTermScope;
            }
            if( highlightedTerm->termType==cterm_function)
            {
                fullPhraseToRender = highlightedTerm->termFunctionReturnType+" "+highlightedTerm->termstring+"("+highlightedTerm->get_parameters()+")";
            }
            else
            {
                fullPhraseToRender = highlightedTerm->termstring;
            }

            int highlightedTermWidthh  = 0, highlightedTermHeight =0;
            int wideststringSize = std::max( (int)fullPhraseToRender.size(), (int)highlightedTerm->termDescription.size() );
            wideststringSize = std::max( (int)fullTermScope.size(), wideststringSize);

            font_textinput->get_metrics("A",&highlightedTermWidthh, &highlightedTermHeight);
            highlightedTermWidthh*=wideststringSize;

            if( highlightedTermXPos+32+highlightedTermWidthh > gpe::screen_width)
            {
                highlightedTermXPos = gpe::screen_width - 64-highlightedTermWidthh;
            }
            if( highlightedTermXPos< 0)
            {
                highlightedTermXPos = 0;
            }

            gpe::gcanvas->render_rectangle(  highlightedTermXPos, highlightedTermYPos,
                                    highlightedTermXPos+highlightedTermWidthh+64, highlightedTermYPos+(default_line_height*3),pawgui::theme_main->popup_box_color,false);

            gpe::gfs->render_text( highlightedTermXPos+32, highlightedTermYPos,fullPhraseToRender,pawgui::theme_main->popup_box_font_color,font_term_name,gpe::fa_left,gpe::fa_top,255 );
            gpe::gfs->render_text( highlightedTermXPos+32, highlightedTermYPos+default_line_height,highlightedTerm->termDescription,pawgui::theme_main->popup_box_font_color,font_term_description,gpe::fa_left,gpe::fa_top,255 );

            if( (int)fullTermScope.size()>1 )
            {
                gpe::gfs->render_text( highlightedTermXPos+32, highlightedTermYPos+default_line_height*2,fullTermScope,pawgui::theme_main->popup_box_font_color,font_term_scope,gpe::fa_left,gpe::fa_top,255 );
            }

            gpe::gcanvas->render_rectangle(  highlightedTermXPos, highlightedTermYPos,
                                    highlightedTermXPos+highlightedTermWidthh+64, highlightedTermYPos+default_line_height*3,pawgui::theme_main->popup_box_border_color,true);
        }
    }


    void syntax_highlighter::render_code_suggestions(  )
    {
        if( (int)suggestedCompilerTerms.size() > 0 )
        {
            //gpe::renderer_main->reset_viewpoint();
            int iRendSuggestion = 0;
            syntax_compiler_term * cTerm = nullptr;
            std::string fullPhraseToRender = "";
            maxSuggestedTextWidth = gpe::screen_width - highlightedTermXPos;
            int currentYRenderPos = highlightedTermYPos;
            for( int iSuggestedEntry = iSuggestedStartPos; iSuggestedEntry < (int)suggestedCompilerTerms.size() && iSuggestedEntry <  iSuggestedStartPos+suggestedTextMaxInViewCount; iSuggestedEntry++ )
            {
                cTerm = suggestedCompilerTerms[iSuggestedEntry];
                if( cTerm!=nullptr)
                {
                    if( cTerm->termType==cterm_function)
                    {
                        if( (int)cTerm->termScope.size() > 0 && cTerm->termScope!="User Global" )
                        {
                            fullPhraseToRender = cTerm->termFunctionReturnType+" "+cTerm->termScope+"."+cTerm->termstring+"("+cTerm->get_parameters()+")";
                        }
                        else
                        {
                            fullPhraseToRender = cTerm->termFunctionReturnType+" "+cTerm->termstring+"("+cTerm->get_parameters()+")";
                        }
                    }
                    else if( (int)cTerm->termScope.size() > 0 && cTerm->termScope!="User Global" )
                    {
                        fullPhraseToRender = cTerm->termScope+"."+cTerm->termstring;
                    }
                    else
                    {
                        fullPhraseToRender = cTerm->termstring;
                    }
                    if( iSuggestedEntry==iSuggestionPos)
                    {
                        gpe::gcanvas->render_rectangle(  highlightedTermXPos, currentYRenderPos,highlightedTermXPos+maxSuggestedTextWidth, currentYRenderPos+default_line_height,pawgui::theme_main->popup_box_highlight_color,false);
                        gpe::gfs->render_only_text( highlightedTermXPos+32, currentYRenderPos,fullPhraseToRender,pawgui::theme_main->popup_box_highlight_font_color,font_textinput,gpe::fa_left,gpe::fa_top,255 );
                    }
                    else
                    {
                        gpe::gcanvas->render_rectangle(  highlightedTermXPos, currentYRenderPos,highlightedTermXPos+maxSuggestedTextWidth, currentYRenderPos+default_line_height,pawgui::theme_main->popup_box_color,false);
                        gpe::gfs->render_only_text(highlightedTermXPos+32, currentYRenderPos,fullPhraseToRender,pawgui::theme_main->popup_box_font_color,font_textinput,gpe::fa_left,gpe::fa_top,255 );
                    }
                    iRendSuggestion++;
                }
                currentYRenderPos += default_line_height;
            }
            gpe::gcanvas->render_rectangle(  highlightedTermXPos, highlightedTermYPos,highlightedTermXPos+maxSuggestedTextWidth, currentYRenderPos,pawgui::theme_main->popup_box_border_color,true);
        }
    }
}
