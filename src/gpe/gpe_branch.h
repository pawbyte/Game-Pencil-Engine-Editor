/*
gpe_branch.h
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

        protected:
            branch * parentBranch;
            branch * treeBranch;
            std::vector< branch * > sub_elements;
            std::string name, branchTag;
            float xpos, ypos;
            float xscale, yscale;
            float angle;
            float xpivot, ypivot;
            int width, height;
            branch_type branchType;
            int currentLayer;
        public:
            branch();
            virtual ~branch();
            virtual void add_branch( branch * new_branch );
            branch * find_branch_by_name( std::string branchName , bool nestDown = false );
            branch * find_typed_branch_by_name( int bType, std::string branchName , bool nestDown = false  );
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
            virtual void update();


            bool remove_branch_by_name( std::string branchName , bool nestDown = true );
            bool remove_typed_branch_by_name( int bType, std::string branchName , bool nestDown = true );

            virtual void render();
            virtual void reset_branch();
            virtual void set_angle(float newAngle );
            virtual void set_coords(float newX, float newY );
            virtual void set_height(int newHeight );
            void set_tag( std::string newTag);
            virtual void set_width(int newWidth );
            virtual void set_size(int newWidth, int newHeight );
            virtual void set_pivots(int newXPivot, int newYPivot );
            virtual void set_scale( float newScale );
            virtual void setx(float newX );
            virtual void set_xscale( float newScale );
            virtual void sety(float newY );
            virtual void set_yscale( float newScale );
    };


    class branchType: public branch
    {
        protected:
            int branchCategory;
        public:
            branchType( int bCategory );
            ~branchType();
    };
}

#endif //gpe_branch_h
