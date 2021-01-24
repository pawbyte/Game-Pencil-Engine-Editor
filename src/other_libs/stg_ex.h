/*
stg_ex.h
This file is part of:
stg_ex
https://pawbyte.com/stg_ex
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.

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

-stg_ex <https://pawbyte.com/stg_ex>


*/

#ifndef stg_ex_h
#define stg_ex_h

//The headers
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>


namespace stg_ex
{
    const int version_major = 1;
    const int version_minor = 2;
    const int version_patch_level = 0;

    bool char_is_alnum(char c, bool allowSpaces = false, bool allowUnderscores = false);
    bool char_is_alpha(char c, bool allowSpaces = false, bool allowUnderscores = false);

    std::vector <std::string> divide_string_into_lines( const std::string str,int numbLines);
    std::string double_to_string(double in);

    bool file_is_audio(const std::string str_file_name);
    bool file_is_font(const std::string str_file_name);
    bool file_is_image(const std::string str_file_name);
    bool file_is_video(const std::string str_file_name);

    std::string file_to_dir(const std::string new_file_name);
    std::string float_to_string(float in);

    std::string get_2digit_value_string(int numberIn);

    std::string get_file_ext(const std::string str_file_name);
    std::string get_file_ext_last_dot(const std::string str_file_name);
    std::string get_file_noext(const std::string str_file_name);

    int get_leading_space_count(const std::string strIn);
    std::string get_local_from_global_file(const std::string str);
    std::string get_path_from_file(const std::string str);
    std::string get_short_filename(const std::string new_file_name, bool show_extension=false);
    std::string get_substring(const std::string strIn, int cStart = 0, int cLength =-1);
    int get_trailing_space_count( const std::string strIn);

    std::string int_to_string(int in);
    bool is_alnum(const std::string str, bool allowSpaces = false, bool allowUnderscores = false);

    int split_first_int(std::string& s, char separator);
    std::string split_first_string(std::string& s, char separator);
    std::string split_first_string(std::string& s, std::string separator);

    bool string_contains(const std::string& hay, const std::string& needle);
    int string_count(const std::string str, std::string needle );
    std::string string_digits(const std::string str);
    bool string_ends(const std::string& hay, const std::string& needle);
    std::string string_letters(const std::string str);
    std::string string_lettersdigits(const std::string str);
    std::string string_lower( const std::string str);
    std::string string_upper(const std::string str);
    std::string string_repeat( const std::string str,int repeatCount );
    std::string string_replace_all( const std::string str, std::string substr, std::string newstr);
    bool string_starts(const std::string& hay, const std::string& needle);

    bool string_to_bool( const std::string value);
    float string_to_float( const std::string& s, float default_value = 0 );
    int string_to_int(const std::string& s, int default_value = 0);

    std::string trim_inplace(std::string&       s,const std::string& delimiters = " \f\n\r\t\v" );
    std::string trim_left_inplace(std::string       s,const std::string& delimiters = " \f\n\r\t\v" );
    std::string trim_right_inplace(std::string       s,const std::string& delimiters = " \f\n\r\t\v" );
    std::string trim_string(const std::string s, char c);

    bool wrap_string( const std::string strIn,std::vector < std::string > &strVector, int lineWidth = 256, int maxLines = -1);
}

#endif //stg_ex_h
