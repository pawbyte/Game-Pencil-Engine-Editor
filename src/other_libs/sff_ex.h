/*
sff_ex.h
This file is part of:
Simple File Functions Extension
https://pawbyte.com/sff_ex/
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

-Simple File Functions Extension <https://www.pawbyte.com/sff_ex/>


*/

#ifndef sff_ex_h
#define sff_ex_h

#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
//#include <direct.h>

namespace sff_ex
{
    const int version_major = 1;
    const int version_minor = 2;
    const int version_patch_level = 0;

    //The headers
    void append_to_file(std::string file_name, std::string output_string );
    bool file_copy(std::string source_file_name, std::string destination_file_name, bool overwrite_existing = true);
    int create_directory( std::string new_path_name);
    bool file_exists(std::string new_file_name);
    bool path_exists(std::string path_name);
}

#endif //sff_ex_h
