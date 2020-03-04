/*
texture_base.h
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

#ifndef gpe_texture_base_h
#define gpe_texture_base_h

#include <string>
#include "gpe_color.h"
#include "gpe_renderer_base.h"
#include "gpe_shapes.h"

namespace gpe
{
    class texture_base
    {
        protected:
            std::string fileLocation;
            bool isTransparent;
            int texWid, texHeight;
            std::string name;
            std::string textureType;
            int texId;
            int currentBlendMode;
            int currentR, currentG, currentB;
        public:
            int lastAlphaRendered;
            texture_base();
            virtual ~texture_base();
            virtual void change_alpha( uint8_t alpha );
            virtual void change_color( color * newColor);
            virtual void change_color( uint8_t red, uint8_t green, uint8_t blue );
            virtual bool copy_image_source(std::string outDirectoryName);


            std::string get_filename();
            int get_height();
            int get_id();
            std::string get_name();
            std::string get_texture_type();
            int get_width();

            //Loader functions
            virtual void load_new_texture( renderer_base * renderer, std::string fileName, int id = -1, bool transparent = true, bool useLinearScaling = false );

            //pre-render functions...
            virtual void prerender_circle( renderer_base * renderer, int rad, color * circleColor,  uint8_t alpha = 255,int id = -1, bool transparent = true, bool useLinearScaling = true, bool isOutline = false  );
            virtual void prerender_rectangle( renderer_base * renderer, int w, int h, color * newColor, int id = -1, bool transparent = true, bool useLinearScaling = true , bool isOutline = false);

            //
            virtual texture_base * create_new();

            virtual void render_align(  int x, int y, int hAlign, int vAlign, shape_rect* clip = NULL, color * rendColor = NULL, int alpha = 255  );
            virtual void render_align_resized(  int x, int y,int newWidth, int newHeight, int hAlign, int vAlign, shape_rect* clip = NULL, color * rendColor = NULL, int alpha = 255 );
            virtual void render_tex(  int x, int y, shape_rect* clip = NULL, int alpha = 255  );
            virtual void render_tex_colored(  int x, int y, color * rendColor, int alpha = 255, shape_rect* clip = NULL  );
            virtual void render_tex_resized(  int x, int y,float newWidth, float newHeight, shape_rect* clip = NULL, color * rendColor = NULL, int alpha = 255 );
            virtual void render_tex_scaled(  int x, int y,float xScale, float yScale, shape_rect* clip = NULL, color * rendColor = NULL, int alpha = 255 );
            virtual void render_tex_rotated(  int x, int y, float renderAngle,  color * renderColor = NULL, shape_rect* clip = NULL , int alpha = 255 );
            virtual void render_tex_rotated_at_point(  int x, int y, float renderAngle, int pointX, int pointY, color * renderColor = NULL, shape_rect* clip = NULL , int alpha = 255 );
            virtual void render_tex_special(  int x, int y, float renderAngle, int newWidth = -1, int newHeight = -1, color * renderColor = NULL, shape_rect* clip = NULL , int alpha = 255 );
            virtual void render_tex_special_at_point(  int x, int y, float renderAngle, int pointX, int pointY,int newWidth = -1, int newHeight = -1, color * renderColor = NULL, shape_rect* clip = NULL , int alpha = 255 );

            virtual void set_alpha( int alpha);
            virtual void set_blend_mode( int newBlendMode);
    };
}
#endif //gpe_texture_base_h
