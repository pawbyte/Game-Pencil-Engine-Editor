/*
gpe_error_logger.cpp
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

#include "gpe_error_logger.h"

namespace gpe
{
    error_log_system * error_log = NULL;

    error_log_system::error_log_system( std::string output_file_name)
    {
        output_file = output_file_name;
        remove( output_file.c_str() );
    }

    error_log_system::~error_log_system()
    {

    }

    void error_log_system::clear_line( int line_id )
    {
        if( (int)system_log.size() > line_id )
        {
            system_log.erase( system_log.begin() + line_id );
        }
    }

    void error_log_system::clear_log()
    {
        system_log.clear();
    }

    std::string error_log_system::get_line(int line_id)
    {
        if( (int)system_log.size() > line_id )
        {
            return system_log[line_id];
        }
        return "";
    }

    int error_log_system::get_size()
    {
        return (int)system_log.size();
    }


    void error_log_system::log_ms_action( std::string str_in, int ms_in, int ms_required )
    {
        /*
        if( ms_in > ms_required )
        {
            str_in += " elapsed " + stg_ex::int_to_string( ms_in ) + "ms";
            report( str_in );
        }
        */
    }

    void error_log_system::report(std::string str_in)
    {
        system_log.push_back( str_in );
        sff_ex::append_to_file( output_file, str_in );
    }

    void error_log_system::set_output_file( std::string output_file_name )
    {
        if( output_file_name.size() <= 0 )
        {
            output_file_name = "errors.txt";
        }
        output_file = output_file_name;
    }

    void error_log_system::write_to_file( const std::string output_file_name )
    {
        int line_count = (int)system_log.size();
        for( int i = 0; i < line_count; i++ )
        {

        }
    }
}
