/*
paw_gui_syntax_handler.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#ifndef GPE_SYNTHAX_HANDLER_H
#define GPE_SYNTHAX_HANDLER_H

#include "../gpe/gpe_file_system.h"

const int PROGRAM_LANGUAGE_JS = 0;
const int PROGRAM_LANGUAGE_CPP = 1;
const int PROGRAM_LANGUAGE_CS = 2;
const int PROGRAM_LANGUAGE_TST = 3;
const int PROGRAM_LANGUAGE_HAXE = 4;
const int PROGRAM_LANGUAGE_LUA = 5;
const int PROGRAM_LANGUAGE_MAX = 6;

typedef enum
{
    CTERM_VARIABLE,
    CTERM_CONSTANT,
    CTERM_KEYWORD,
    CTERM_FUNCTION,
    CTERM_CLASS
} compileTermTypes;

class GPE_Compiler_Term
{
private:
    std::string termParametersString;
public:
    std::string termString;
    std::vector < std::string > termParameters;
    std::string termFunctionReturnType;
    std::string termDescription;
    std::string termScope;
    int termType;
    GPE_Compiler_Term();
    GPE_Compiler_Term(std::string tName,std::string tParameters, std::string tFunctionReturnType,std::string tDescription, int tType = -1, std::string tScope="");
    ~GPE_Compiler_Term();
    std::string get_parameters();
};

class GPE_Gui_Engine_Language
{
public:
    std::vector <GPE_Compiler_Term *> languageVariables;
    std::vector <GPE_Compiler_Term *> languageConstants;
    std::vector <GPE_Compiler_Term *> languageFunctions;
    std::vector <GPE_Compiler_Term *> languageClasses;
    std::vector <GPE_Compiler_Term *> languageKeywords;
    std::vector <GPE_Compiler_Term *> languageDataTypes;
    std::vector <char> symbolLibrary;
    std::string macroCommentChar;
    std::string commentChar;
    std::string blockcommentStartStr;
    std::string blockcommentEndStr;
    std::string languageName;
    std::string languageShortName;
    bool isCodingLanguage;
    int languageId;
    GPE_Gui_Engine_Language(std::string langName, std::string langShortName, int langId, bool projectCodeLanguage = true, bool useDefaultSymbols = true);
    ~GPE_Gui_Engine_Language();
    bool add_language_variable(std::string nName, std::string nDescription = "", std::string nScopre = "");
    bool add_language_constant(std::string nName, std::string nDescription = "", std::string nParameters = "");
    bool add_language_function(std::string nName, std::string nDescription, std::string nParameters, std::string tFunctionReturnType, std::string nScope);
    bool add_language_class(std::string nName, std::string nDescription = "", std::string nParameters = "");
    bool add_language_keyword(std::string nName, std::string nDescription = "", std::string nScope = "");
    bool add_language_data_type(std::string nName, std::string nDescription = "", std::string nParameters = "");

    void erase_language_variable(std::string nName);
    void erase_language_constant(std::string nName);
    void erase_language_function(std::string nName);
    void erase_language_class(std::string nName);
    void erase_language_keyword(std::string nName);
    void erase_language_data_type(std::string nName);

    std::string find_gpe_datatype(std::string strIn, int startPos = 0);
    std::string find_gpe_function(std::string strIn, int startPos = 0);
    std::string find_gpe_keyword(std::string strIn, int startPos = 0);
    std::string find_gpe_variable(std::string strIn, int startPos = 0);

    bool is_reserved_word(std::string strIn);
    bool is_compilable_word(std::string strIn);

    bool charIsSymbol( char  charIn);
    int firstSymbolInString(std::string inString, int pos=0);
    GPE_Compiler_Term * find_matching_variable(std::string name_param);
    GPE_Compiler_Term * find_matching_function(std::string name_param);
};

class GPE_Syntax_Highlighter
{
private:
        std::vector <GPE_Gui_Engine_Language * > editorLanguages;
public:
    std::vector <std::string> foundParameters;
    std::vector <std::string> defaultParameterLines;
    std::string newParametersString;
    std::vector <std::string> functionParameterErrors;
    bool parametersAreValid;
    std::vector <std::string> compilerErrors;
    std::vector <std::string> compilerWarnings;
    GPE_Gui_Engine_Language * defaultLanguage;
    //creates the vector for all of the symbols

    //For code highlights and suggested text
    std::vector <GPE_Compiler_Term *> activeProjectFunctions;
    std::vector <GPE_Compiler_Term *> activeProjectKeywords;
    //For code highlights and suggested text
    std::vector <GPE_Compiler_Term *> suggestedCompilerTerms;
    GPE_Compiler_Term * highlightedTerm;
    int highlightedTermXPos, highlightedTermYPos;
    bool documentationIsBeingShown;
    bool codeBeingSuggested;
    int iSuggestedStartPos;
    int iSuggestionPos;
    int suggestedTextMaxInViewCount;
    int maxSuggestedTextWidth;
    //End of suggested text/code highlights related variables
    GPE_Syntax_Highlighter();
    ~GPE_Syntax_Highlighter();

    GPE_Gui_Engine_Language * add_programming_language(std::string langName, std::string langShortName, int langId, bool useDefaultSymbols = true,bool  isCodingLanguage = true, bool  isDefaultCodeLanguage= false);
    bool is_reserved_word(std::string strIn);
    bool is_compilable_word(std::string strIn);

    bool charIsSymbol( char  charIn);
    int firstSymbolInString(std::string inString, int pos=0);
    std::string find_project_function(std::string strIn, int startPos = 0);
    std::string find_project_keyword(std::string strIn, int startPos = 0);
    std::string find_gpe_datatype(std::string strIn, int startPos = 0);
    std::string find_gpe_function(std::string strIn, int startPos = 0);
    std::string find_gpe_keyword(std::string strIn, int startPos = 0);
    std::string find_gpe_variable(std::string strIn, int startPos = 0);
    GPE_Compiler_Term * find_matching_variable(std::string name_param);
    GPE_Compiler_Term * find_matching_function(std::string name_param);
    int get_language_count();
    int get_language_id_from_name( std::string name_param);
    GPE_Gui_Engine_Language * get_language_object( int langPosition);
    GPE_Gui_Engine_Language * get_language_object_from_id( int langId);
    std::string get_language_name_from_id( int langId );
    std::string get_language_shortname_from_id( int langId);
    bool process_parameters_string( std::string paramToProcess);
    void begin_compiling();
    void clear_all();
    void clear_highlights();
    void clear_suggestions();
    void render_code_highlights( );
    void render_code_suggestions( );
};

extern GPE_Syntax_Highlighter * GPE_main_HIGHLIGHTER;

#endif
