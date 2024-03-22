/*
gpe_branch.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#include <memory>

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
        SEQUENCE = 23,
        _3D_MODEL = 24,
        _3D_ANIMATION = 25,
        _3D_LIGHT = 26,
        _3D_OTHER = 27,
        _3D_OTHER2 = 28,
        _3D_OTHER3 = 29,
        MAX_TYPE = 30,
        END = 30,
    };

    //SEQUENCES, and 3D related branches is an InProgress branch type added for 1.6 LTS and newer, making the MAX_TYPE = END = 30 currently


    class branch_factory;

    class branch
    {
        private:
            int frames_til_deletion;
            bool branch_being_removed;
            void default_branch_constructor(); //used by both constructors to set every variable
        protected:
            branch * branch_parent;
            branch * branch_tree;

            int branch_id; //All branches hav e this on creation
            bool branch_was_inited;
            bool branch_is_permanent;

            int scene_id; //
            int scene_orign_id;
            int scene_unique_id;

            std::vector< branch * > sub_elements;
            std::string name, branch_tag;
            float xpos, ypos, zpos;
            float xscale, yscale;
            float angle;
            float xpivot, ypivot;
            int width, height;
            branch_type branch_type_id;
            int current_layer;
            bool branch_visible;

            bool uses_render_start;
            bool uses_render_end;
            bool uses_render_hud;
            bool uses_render_hud_start;
            bool uses_render_hud_end;
        public:
            branch();
            branch( int branch_layer_id, float x_pos_in,  float y_pos_in, float z_pos_in );
            virtual ~branch();
            virtual void add_branch( branch * new_branch );
            bool being_deleted();

            virtual branch * branch_create( );
            virtual branch * branch_create_and_init( int branch_layer_id, float x_pos_in,  float y_pos_in, float z_pos_in = 0);
            virtual void branch_destroy();
            bool branch_inited();
            virtual void branch_init( int branch_layer_id, float x_pos_in,  float y_pos_in, float z_pos_in = 0);

            bool compare_zpos( branch * other );
            branch * find_branch_by_name( std::string branch_name , bool nest_down = false );
            branch * find_typed_branch_by_name( int branch_type, std::string branch_name , bool nest_down = false  );
            float get_angle();
            int get_frames_before_deleted();
            std::string get_name();
            branch * get_parent();
            branch * get_parent_tree();
            std::string get_tag();
            float getx();
            float gety();
            uint8_t get_layer_id();
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

            virtual void update( float delta_time );

            bool remove_branch_by_name( std::string branch_name , bool nest_down = true );
            bool remove_typed_branch_by_name( int branch_type_id, std::string branch_name , bool nest_down = true );

            virtual void render();
            virtual void render_start(); //Useful for rendering on start(i.e, floors and shadows)
            virtual void render_end(); //Useful for post-rendering(i.e  effects and texts)
            virtual void render_hud(); //Useful for rendering on the HUD level
            virtual void render_hud_start(); //Useful for rendering at start the HUD level
            virtual void render_hud_end(); //Useful for rendering on last layer of HUD level

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

    bool compare_braches_zpos( branch * a, branch * b );


    class branch_type_id: public branch
    {
        protected:
            int branch_category;
        public:
            branch_type_id( int branch_catetory_start );
            ~branch_type_id();
    };

    extern branch_factory * branch_factory_master;
}

#endif //gpe_branch_h
