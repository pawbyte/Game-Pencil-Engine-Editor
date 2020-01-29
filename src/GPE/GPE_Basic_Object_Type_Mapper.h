/*
GPE_Basic_Object_Type_Mapper.h
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

#ifndef GPE_BASIC_OBJECT_TYPE_MAPPER_H
#define GPE_BASIC_OBJECT_TYPE_MAPPER_H

#include "GPE_Basic_Object.h"

class GPE_ObjectMapper
{
    private:
        std::map <std::string, GPE_GameObject*(*)(int xPosIn,  int yPosIn,  int objectLayerId)> objMap;
    public:
        GPE_ObjectMapper();
        ~GPE_ObjectMapper();
        virtual bool add_to_map(std::string objName, GPE_GameObject * ( *mFunction)(int xPosIn,  int yPosIn,  int objectLayerId) );
        virtual void clear_map();
        virtual GPE_GameObject * create_object(std::string objName, int xPosIn,  int yPosIn,  int objectLayerId);
};


#endif //GPE_BASIC_OBJECT_TYPE_MAPPER_H
