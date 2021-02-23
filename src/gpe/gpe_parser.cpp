/*
gpe_parser.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_parser.h"

namespace gpe
{
    key_pair::key_pair(int idNumb, std::string str, std::string ksubstr, float kValue)
    {
        key_id = idNumb;
        key_string = str;
        key_substring = ksubstr;
        key_value = kValue;
        parent_pair = nullptr;
        id_unique_number = 0;
    }

    key_pair::~key_pair()
    {

    }

    gpe::key_pair * key_pair::add_keypair(std::string optionName, std::string optionSubStr,float optionValue )
    {
        bool optionExists = false;
        gpe::key_pair * tOption = nullptr;
        for( int i = 0; i < (int)sub_options.size(); i++)
        {
            tOption = sub_options[i];
            if( tOption!=nullptr )
            {
                if( tOption->key_string==optionName)
                {
                    optionExists = false;
                    return tOption; //returns option already that exists
                }
            }
        }
        if( !optionExists)
        {
            gpe::key_pair * kp = new gpe::key_pair( get_unique_id(),optionName,optionSubStr, optionValue);
            kp->parent_pair = this;
            sub_options.push_back(kp);
            return kp; //returns new option
        }
        return nullptr; //if everything breaks return nothing
    }

    gpe::key_pair *  key_pair::find_option_id( int kId )
    {
        if( key_id == kId)
        {
            return this;
        }
        gpe::key_pair * curKey = nullptr;
        gpe::key_pair * fKey = nullptr;
        int optionsSize = (int)sub_options.size();

        for( int i = 0; i < optionsSize; i++)
        {
            curKey = sub_options[i];
            if( curKey!=nullptr)
            {
                fKey = curKey->find_option_id( kId);
                if( fKey!=nullptr)
                {
                    return fKey;
                }
            }
        }
        return nullptr;
    }

    gpe::key_pair *  key_pair::find_option_named( std::string str )
    {
        if( key_string == str || key_substring == str)
        {
            return this;
        }
        gpe::key_pair * curKey = nullptr;
        gpe::key_pair * fKey = nullptr;
        int optionsSize = sub_options.size();

        for( int i = 0; i < optionsSize; i++)
        {
            curKey = sub_options[i];
            if( curKey!=nullptr)
            {
                fKey = curKey->find_option_named( str );
                if( fKey!=nullptr)
                {
                    return fKey;
                }
            }
        }
        return nullptr;
    }

    gpe::key_pair *  key_pair::find_option_value( float kVal )
    {
        if( key_value == kVal)
        {
            return this;
        }
        gpe::key_pair * curKey = nullptr;
        gpe::key_pair * fKey = nullptr;
        int optionsSize = sub_options.size();

        for( int i = 0; i < optionsSize; i++)
        {
            curKey = sub_options[i];
            if( curKey!=nullptr)
            {
                fKey = curKey->find_option_value( kVal );
                if( fKey!=nullptr)
                {
                    return fKey;
                }
            }
        }
        return nullptr;

    }

    int key_pair::get_unique_id()
    {
        if( parent_pair!=nullptr)
        {
            return parent_pair->get_unique_id();
        }
        else
        {
            id_unique_number++;
            return id_unique_number;
        }
    }

    bool key_pair::name_exists( std::string str)
    {
        gpe::key_pair * curKey = nullptr;
        int optionsSize = sub_options.size();

        for( int i = 0; i < optionsSize; i++)
        {
            curKey = sub_options[i];
            if( curKey!=nullptr)
            {
                if( curKey->name_exists( str) )
                {
                    return true;
                }
            }
        }

        if( key_string == str || key_substring == str)
        {
            return true;
        }
        return false;
    }

    bool key_pair::remove_option_id( int kId, bool nest_down  )
    {
        bool optionRemoved = false;
        int optionsSize = sub_options.size();
        gpe::key_pair * curKey = nullptr;
        for( int i = optionsSize -1; i >=0; i--)
        {
            curKey = sub_options[i];
            if( curKey!=nullptr)
            {
                if( nest_down )
                {
                    if( curKey->remove_option_id( kId, true) )
                    {
                        optionRemoved = true;
                    }
                }
                if( curKey->key_id == kId)
                {
                    optionRemoved =  true;
                }
                sub_options.erase( sub_options.begin() + i );
                delete curKey;
                curKey = nullptr;
            }
        }

        return optionRemoved;
    }

    bool key_pair::remove_option_named( std::string kStr, bool nest_down  )
    {
        bool optionRemoved = false;
        int optionsSize = (int)sub_options.size();
        gpe::key_pair * curKey = nullptr;

        for( int i = optionsSize -1; i >=0; i--)
        {
            curKey = sub_options[i];
            if( curKey!=nullptr)
            {
                if( nest_down )
                {
                    if( curKey->remove_option_named( kStr, true) )
                    {
                        optionRemoved = true;
                    }
                }
                if( curKey->key_string == kStr)
                {
                    optionRemoved =  true;
                }
                sub_options.erase( sub_options.begin() + i );
                delete curKey;
                curKey = nullptr;
            }
        }

        return optionRemoved;

    }

    bool key_pair::remove_option_value( float kVal, bool nest_down  )
    {

        bool optionRemoved = false;
        int optionsSize = (int)sub_options.size();
        gpe::key_pair * curKey = nullptr;

        for( int i = optionsSize -1; i >=0; i--)
        {
            curKey = sub_options[i];
            if( curKey!=nullptr)
            {
                if( nest_down )
                {
                    if( curKey->remove_option_value( kVal, true) )
                    {
                        optionRemoved = true;
                    }
                }
                if( curKey->key_value == kVal)
                {
                    optionRemoved =  true;
                }
                sub_options.erase( sub_options.begin() + i );
                delete curKey;
                curKey = nullptr;
            }
        }
        return optionRemoved;
    }

    void key_pair::remove_all()
    {
        gpe::key_pair * curKey = nullptr;
        for( int i = (int)sub_options.size()-1; i >=0; i--)
        {
            curKey = sub_options[i];
            if( curKey!=nullptr)
            {
                delete curKey;
                curKey = nullptr;
            }
        }
        sub_options.clear();
    }
}
