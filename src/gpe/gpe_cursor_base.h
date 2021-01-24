/*
gpe_cursor_base.h
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

#ifndef gpe_cursor_base_h
#define gpe_cursor_base_h

#include <string>
#include <vector>



namespace gpe
{
    enum cursor_default_type
    {
        arrow,     /**< Arrow */
        ibeam,     /**< I-beam */
        wait,      /**< Wait */
        crosshair, /**< Crosshair */
        wait_arrow, /**< Small wait cursor (or Wait if not available) */
        sizenwse,  /**< float arrow pointing northwest and southeast */
        sizenesw,  /**< float arrow pointing northeast and southwest */
        sizewe,    /**< float arrow pointing west and east */
        sizens,    /**< float arrow pointing north and south */
        sizeall,   /**< Four pointed arrow pointing north, south, east, and west */
        no,        /**< Slashed circle or crossbones */
        hand,      /**< Hand */
        max_default_cursor
    };

    extern int cursor_controller_counter;

    class cursor_controller_base
    {
        private:
            int cursor_controller_id;
        protected:
            std::string cursor_previous;
            std::string cursor_current;
            std::vector<std::string >cursor_default_names;
            std::string cursor_controller_type;
            int cursor_window_id;
        public:
            cursor_controller_base();
            virtual ~cursor_controller_base();

            //Cursor logic
            virtual void cursor_change( std::string newCursor );
            virtual void cursor_change_system( int system_cursor_id );
            virtual void cursor_clear_dynamic();
            virtual bool cursor_contains( std::string cursorName );
            virtual bool cursor_create_from_image(std::string fName );
            virtual int cursor_map_size();
            virtual std::string cursor_system_name( int cId );

            int equals( cursor_controller_base * other );
            std::string get_type();
            int get_id();
            int get_window_id();

            virtual void process_cursors();
    };

    extern cursor_controller_base * cursor_main_controller;
}

#endif //gpe_cursor_base_h
