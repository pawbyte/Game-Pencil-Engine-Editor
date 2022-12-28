/*
game_state_shape_test.h
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

#ifndef game_state_triangle_test_h
#define game_state_triangle_test_h

#include "gpe/gpe.h"
#include "other_libs/semath.h"
#include "sdl_libs/gpe_artist_sdl.h"
#include "sdl_libs/gpe_renderer_sdl.h"


class colored_circle
{
    public:
    gpe::shape_circle * my_circle;
    gpe::color * my_color;
    colored_circle( int x, int y, int r );
    ~colored_circle();
};

class colored_triangle
{
    public:
    gpe::shape_triangle2d * my_triangle;
    gpe::color * my_color;
    colored_triangle();
    ~colored_triangle();
};

class colored_arc
{
    public:
    gpe::shape_circle * my_circle;
    gpe::color * my_color;
    float start_arc_angle;
    float end_arc_angle;
    float arc_vertices;
    int arc_width;
    int arc_alpha;
    colored_arc( int x, int y, int r, float s_angle, float e_angle, float a_verts, int a_width );
    ~colored_arc();
};


class colored_rectangle
{
    public:
    gpe::shape_rect * my_rectangle;
    gpe::color * my_color;
    colored_rectangle( int x, int y, int w, int h );
    ~colored_rectangle();
};

class shape_test_state: public gpe::program_state
{
    public:
        long current_circles, average_circles;
        long current_triangles, average_triangles;
        long current_frame;
        SDL_FPoint line_render_points[ gpe::render_points_giant_size];

        float arc_degree_ctr;
        bool show_instructions;
        int instruction_level;
        bool show_circles;
        bool show_triangles;
        bool show_rectangles;
        bool triangle_alpha_enabled;

        //Circle Variables
        std::vector< colored_arc * > random_arcs;
        int arc_seeked_count;

        std::vector< colored_circle * > random_circles;
        int circle_seeked_count;
        gpe::shape_circle * mouse_circle;
        int mouse_circle_position;

        //Triangle Variables
        std::vector< colored_triangle * > random_triangles;
        int triangle_seeked_count;
        gpe::shape_triangle2d * mouse_triangle;
        int mouse_triangle_position;

        std::vector< colored_rectangle * > random_rectangles;
        int rectangle_seeked_count;
        gpe::shape_rect * mouse_rectangle;
        int mouse_rectangle_position;

        shape_test_state(std::string s_name );
        ~shape_test_state();
        void apply_logic();
        void clean_up();

        void clear_circle();
        void clear_triangles();
        void clear_rectangles();

        colored_arc * create_random_arc();
        colored_circle * create_random_circle();
        colored_triangle * create_random_triangle();
        colored_rectangle * create_random_rectangle();

        void end_state();
        void process_input();
        void remove_triangle( int remove_count );
		void render();
        void start_state();
};

#endif // game_state_triangle_test_h
