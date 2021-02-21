/*
gpe_branch.h
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

#ifndef gpe_branch_h
#define gpe_branch_h

#include "gpe_common_includes.h"
#include "gpe_globals.h"

namespace gpe
{
    enum class branch_type
    {
        //Basic Scene Types
        BASIC_SCENE_ELEMENT = 0,
        DEFAULT = 0,
        SCENE = 1,
        LAYER = 2,
        GROUP = 3,
        TILEMAP = 4,
        TILE = 5,
        BACKGROUND = 6,
        TEXTURE = 7,
        ANIMATION = 8,
        OBJECT = 9,
        PATH = 10,
        GEOMETRY = 11,
        CIRCLE = 12,
        RECTANGLE = 13,

        TEXT = 14,
        STEXT = 15,
        PARTIClE_EMITTER = 16,
        PARTIClE_EFFECT = 17,
        LIGHT = 18,
        LIGHT_AMBIENT = 19,
        LIGHT_DIRECTION = 20,
        LIGHT_POINT = 21,
        SHADER = 22,
        MAX_TYPE = 23,
        END = 23
    };

    class branch
    {
        private:
            int frames_til_deletion;
            bool branch_being_removed;
        protected:
            branch * branch_parent;
            branch * branch_tree;
            std::vector< branch * > sub_elements;
            std::string name, branch_tag;
            float xpos, ypos;
            float xscale, yscale;
            float angle;
            float xpivot, ypivot;
            int width, height;
            branch_type branch_type_id;
            int current_layer;
            bool branch_visible;

        public:
            branch();
            virtual ~branch();
            virtual void add_branch( branch * new_branch );
            void being_deleted();
            int get_frames_before_deleted();
            branch * find_branch_by_name( std::string branch_name , bool nest_down = false );
            branch * find_typed_branch_by_name( int branch_type, std::string branch_name , bool nest_down = false  );
            float get_angle();
            std::string get_name();
            branch * get_parent();
            branch * get_parent_tree();
            std::string get_tag();
            float getx();
            float gety();
            int get_layer_id();
            float get_width();
            float get_height();
            int get_size();
            branch_type get_type();
            std::string get_type_string();

            float get_xpivot();
            float get_xscale();
            float get_ypivot();
            float get_yscale();

            bool is_visible();

            virtual void update();

            bool remove_branch_by_name( std::string branch_name , bool nest_down = true );
            bool remove_typed_branch_by_name( int branch_type_id, std::string branch_name , bool nest_down = true );

            virtual void render();
            virtual void reset_branch();
            virtual bool self_destruct();

            virtual void set_angle(float new_angle );
            virtual void set_coords(float x_new, float y_new );
            virtual void set_frames_before_deleted( int frames_remaining );

            virtual void set_height(int new_height );
            void set_tag( std::string tag_new);
            virtual void set_width(int new_width );
            virtual void set_size(int new_width, int new_height );
            virtual void set_pivots(int new_x_pivot, int new_y_pivot );
            virtual void set_scale( float new_scale );
            virtual void setx(float x_new );
            virtual void set_xscale( float new_scale );
            virtual void sety(float y_new );
            virtual void set_yscale( float new_scale );
    };


    class branch_type_id: public branch
    {
        protected:
            int branch_category;
        public:
            branch_type_id( int branch_catetory_start );
            ~branch_type_id();
    };
}

#endif //gpe_branch_h
