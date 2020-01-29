/*
GPE_String_Ex.h
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

#ifndef STRING_EX_H
#define STRING_EX_H

//The headers
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <ctime>
#include <cstdlib>

bool char_is_alnum(char c, bool allowSpaces = false, bool allowUnderscores = false);
bool char_is_alpha(char c, bool allowSpaces = false, bool allowUnderscores = false);
bool is_alnum(const std::string str, bool allowSpaces = false, bool allowUnderscores = false);

bool string_contains(const std::string& hay, const std::string& needle);
bool string_starts(const std::string& hay, const std::string& needle);
bool string_ends(const std::string& hay, const std::string& needle);

std::string double_to_string(double in);
std::string float_to_string(float in);
std::string int_to_string(int in);

std::string get_substring(std::string strIn, int cStart = 0, int cLength =-1);
int get_leading_space_count(std::string strIn);
int get_trailing_space_count( std::string strIn);

int string_count(std::string str, std::string needle );
std::string string_digits(std::string str);
std::string string_letters(std::string str);
std::string string_lettersdigits(std::string str);
std::string string_lower(std::string str);
std::string string_upper(std::string str);
std::string string_repeat(std::string str,int repeatCount );
std::string string_replace_all(std::string str, std::string substr, std::string newstr);

std::vector <std::string> divide_string_into_lines(std::string stringIn,int numbLines);
std::string get_2digit_value_string(int numberIn);
std::string get_short_filename(std::string fileNameIn, bool showExtension=false);
std::string file_to_dir(std::string fileNameIn);

int split_first_int(std::string& s, char separator);
std::string split_first_string(std::string& s, char separator);
std::string split_first_string(std::string& s, std::string separator);
bool string_to_bool(std::string value);
int string_to_int(const std::string& s, int default_value = 0);
float string_to_float( const std::string& s, float default_value = 0 );

std::string trim_inplace(std::string&       s,const std::string& delimiters = " \f\n\r\t\v" );
std::string trim_left_inplace(std::string       s,const std::string& delimiters = " \f\n\r\t\v" );
std::string trim_right_inplace(std::string       s,const std::string& delimiters = " \f\n\r\t\v" );
std::string trim_string(const std::string s, char c);

bool wrap_string( std::string strIn,std::vector < std::string > &strVector, int lineWidth = 256, int maxLines = -1);

#endif
