/*
gpe_error_logger.h
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

#ifndef gpe_error_logger_h
#define gpe_error_logger_h

#include <string>
#include <vector>
#include "../other_libs/sff_ex.h"
#include "../other_libs/stg_ex.h"

namespace gpe
{
    class error_log_system
    {
        protected:
            std::string output_file;
            std::vector < std::string > system_log;
        public:
            error_log_system( std::string output_file_name );
            ~error_log_system();
            void clear_line( int line_id );
            void clear_log();
            std::string get_line(int line_id);
            int get_size();
            void log_ms_action(std::string str_in, int ms_in, int ms_required = 1 );
            void report(std::string str_in );
            void set_output_file( std::string output_file_name );
            void write_to_file( const std::string output_file_name );
    };

    extern error_log_system * error_log;
}

#endif //gpe_error_logger_h
