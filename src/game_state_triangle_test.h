/*
game_state_triangle_test.h
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

#ifndef game_state_triangle_test_h
#define game_state_triangle_test_h

#include "gpe/gpe.h"
#include "other_libs/semath.h"

class colored_triangle
{
    public:
    gpe::shape_triangle2d * my_triangle;
    gpe::color * my_color;
    colored_triangle();
    ~colored_triangle();
};

class triangle_test_state: public gpe::program_state
{
    public:
        std::vector< colored_triangle * > random_triangles;
        int triangle_seeked_count;
        gpe::shape_triangle2d * mouse_triangle;
        int mouse_triangle_position;

        triangle_test_state(std::string s_name );
        ~triangle_test_state();
        void apply_logic();
        void clean_up();

        void clear_triangles();
        colored_triangle * create_random_triangle();

        void end_state();
        void process_input();
        void remove_triangle( int remove_count );
		void render();
        void start_state();
};

#endif // game_state_triangle_test_h
