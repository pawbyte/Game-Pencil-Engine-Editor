/*
pawgui_syntax_handler.h
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

#ifndef PAWGUI_SYNTHAX_HANDLER_H
#define PAWGUI_SYNTHAX_HANDLER_H

#include "../gpe/gpe_file_system.h"

namespace pawgui
{
    const int program_language_js = 0;
    const int program_language_cpp = 1;
    const int program_language_cs = 2;
    const int program_language_tst = 3;
    const int program_language_haxe = 4;
    const int program_language_lua = 5;
    const int program_language_max = 6;

    typedef enum
    {
        cterm_variable,
        cterm_constant,
        cterm_keyword,
        cterm_function,
        cterm_class
    } compileTermTypes;

    class syntax_compiler_term
    {
        private:
            std::string termParametersstring;
        public:
            std::string termstring;
            std::vector < std::string > termParameters;
            std::string termFunctionReturnType;
            std::string termDescription;
            std::string termScope;
            int termType;
            syntax_compiler_term();
            syntax_compiler_term(std::string tName,std::string tParameters, std::string tFunctionReturnType,std::string tDescription, int tType = -1, std::string tScope="");
            ~syntax_compiler_term();
            std::string get_parameters();
    };

    class syntax_language
    {
        public:
            std::vector <syntax_compiler_term *> languageVariables;
            std::vector <syntax_compiler_term *> languageConstants;
            std::vector <syntax_compiler_term *> languageFunctions;
            std::vector <syntax_compiler_term *> languageClasses;
            std::vector <syntax_compiler_term *> languageKeywords;
            std::vector <syntax_compiler_term *> languageDataTypes;
            std::vector <char> symbolLibrary;
            std::string macroCommentChar;
            std::string commentChar;
            std::string blockcommentStartStr;
            std::string blockcommentEndStr;
            std::string languageName;
            std::string languageShortName;
            bool isCodingLanguage;
            int languageId;
            syntax_language(std::string langName, std::string langShortName, int langId, bool projectCodeLanguage = true, bool useDefaultSymbols = true);
            ~syntax_language();
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

            std::string find_gpe_datatype(std::string str_in, int startPos = 0);
            std::string find_gpe_function(std::string str_in, int startPos = 0);
            std::string find_gpe_keyword(std::string str_in, int startPos = 0);
            std::string find_gpe_variable(std::string str_in, int startPos = 0);

            bool is_reserved_word(std::string str_in);
            bool is_compilable_word(std::string str_in);

            bool charIsSymbol( char  charIn);
            int firstSymbolInstring(std::string instring, int pos=0);
            syntax_compiler_term * find_matching_variable(std::string name_param);
            syntax_compiler_term * find_matching_function(std::string name_param);
    };

    class syntax_highlighter
    {
        private:
                std::vector <syntax_language * > editorLanguages;
        public:
            std::vector <std::string> foundParameters;
            std::vector <std::string> defaultParameterLines;
            std::string newParametersstring;
            std::vector <std::string> functionParameterErrors;
            bool parametersAreValid;
            std::vector <std::string> compilerErrors;
            std::vector <std::string> compilerWarnings;
            syntax_language * defaultLanguage;
            //creates the vector for all of the symbols

            //For code highlights and suggested text
            std::vector <syntax_compiler_term *> activeProjectFunctions;
            std::vector <syntax_compiler_term *> activeProjectKeywords;
            //For code highlights and suggested text
            std::vector <syntax_compiler_term *> suggestedCompilerTerms;
            syntax_compiler_term * highlightedTerm;
            int highlightedTermXPos, highlightedTermYPos;
            bool documentationIsBeingShown;
            bool codeBeingSuggested;
            int iSuggestedStartPos;
            int iSuggestionPos;
            int suggestedTextMaxInViewCount;
            int maxSuggestedTextWidth;
            //End of suggested text/code highlights related variables
            syntax_highlighter();
            ~syntax_highlighter();

            syntax_language * add_programming_language(std::string langName, std::string langShortName, int langId, bool useDefaultSymbols = true,bool  isCodingLanguage = true, bool  isDefaultCodeLanguage= false);
            bool is_reserved_word(std::string str_in);
            bool is_compilable_word(std::string str_in);

            bool charIsSymbol( char  charIn);
            int firstSymbolInstring(std::string instring, int pos=0);
            std::string find_project_function(std::string str_in, int startPos = 0);
            std::string find_project_keyword(std::string str_in, int startPos = 0);
            std::string find_gpe_datatype(std::string str_in, int startPos = 0);
            std::string find_gpe_function(std::string str_in, int startPos = 0);
            std::string find_gpe_keyword(std::string str_in, int startPos = 0);
            std::string find_gpe_variable(std::string str_in, int startPos = 0);
            syntax_compiler_term * find_matching_variable(std::string name_param);
            syntax_compiler_term * find_matching_function(std::string name_param);
            int get_language_count();
            int get_language_id_from_name( std::string name_param);
            syntax_language * get_language_object( int langPosition);
            syntax_language * get_language_object_from_id( int langId);
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
    extern syntax_highlighter * main_syntax_highlighter;
}

#endif //PAWGUI_SYNTHAX_HANDLER_H
