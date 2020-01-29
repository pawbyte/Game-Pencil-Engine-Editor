/*
GPE_Parser.h
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

#ifndef GPE_PARSER
#define GPE_PARSER

#include <string>
#include <vector>

class GPE_KeyPair
{
public:
    int uniqueIdNumb;
    int keyId;
    float keyValue;
    std::string keyString;
    std::string keySubString;
    std::vector <GPE_KeyPair *> subOptions;
    GPE_KeyPair * parentPair;
    GPE_KeyPair(int idNumb, std::string str, std::string ksubstr = "", float kValue = -1);
    ~GPE_KeyPair();
    GPE_KeyPair * add_keypair(std::string optionName, std::string optionSubStr= "",float optionValue = 0 );
    GPE_KeyPair *  find_option_id( int kId );
    GPE_KeyPair *  find_option_named( std::string str );
    GPE_KeyPair *  find_option_value( float kVal );
    int get_unique_id();
    bool name_exists( std::string str);
    bool remove_option_id( int kId, bool nestDown = true );
    bool remove_option_named( std::string str, bool nestDown = true );
    bool remove_option_value( float kVal, bool nestDown = true );
    void remove_all();
};

#endif //GPE_PARSER
