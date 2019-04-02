/*
GPE_Engine.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef GPE_ENGINE_H
#define GPE_ENGINE_H

#include "GPE_CIncludes.h"
#include "GPE_Shared_Resources.h"
#include "GPE_Surface_Modifier.h"
#include "GPE_Functions.h"
#include "GPE_Globals.h"

extern bool GPE_IS_LOADING;


void limit_space_to_rect(GPE_Rect * rectIn = NULL,int * limitedX = NULL, int * limitedY = NULL);

const int GPE_ARTIST_PRERENDER_SHAPE_COUNT = 10;
//The Master class to render things to textures/the screen
class GPE_Artist
{
private:
    GPE_Rect * tempCipRect;
    int currentArtistBlendMode;
    GPE_Color * currentColor;
    GPE_Color * tempColor;
    int currentAlpha;
    int defaultLineWidth;
    GPE_Texture * prerenderedSquare;
    GPE_TargetTexture * lightingOverlayTexture;
    GPE_Texture * prerenderedCircles[GPE_ARTIST_PRERENDER_SHAPE_COUNT];
    GPE_Texture * prerenderedCirclesOutlines[GPE_ARTIST_PRERENDER_SHAPE_COUNT];
public:
    GPE_Artist();
    ~GPE_Artist();

    //Circle and SemiCircles and Ovals Rendering Functions
    bool render_circle( int x, int y, int rad, bool renderOutLine = false);
    bool render_circle_color( int x, int y, int rad, GPE_Color * rendColor = NULL, int alphaChannel = 255,bool renderOutLine = false);
    bool render_oval( int x, int y, int w, int h,bool renderOutLine = false);
    bool render_oval_color( int x, int y, int w, int h, GPE_Color *rendColor, int alphaChannel = 255, bool renderOutLine = false);
    bool render_semi_circle( int x, int y, int rad, int direction = 0, bool renderOutLine = false);
    bool render_semi_circle_color( int x, int y, int rad, int direction = 0, GPE_Color *rendColor = NULL, int alphaChannel = 255, bool renderOutLine = false);

    //Gradients Rendering [ BEGIN ]
    void render_gradient_circle(  int radius, GPE_Rect * rendRect = NULL, GPE_Color * rendColor = NULL);
    void render_gradient_horizontal(  GPE_Rect * rendRect = NULL, GPE_Color * rendColor = NULL);
    void render_gradient_vertical(  GPE_Rect * rendRect = NULL, GPE_Color * rendColor = NULL);
    //Gradients Rendering [ END ]

    //Line Rendering [ BEGIN ]
    void render_line(  int x1, int y1, int x2, int y2 );
    void render_line_color(  int x1, int y1, int x2, int y2,  GPE_Color *rendColor, int alphaChannel = 255);
    void render_line_capped(  int x1, int y1, int x2, int y2, int lineWidth );
    void render_line_capped_color(  int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel = 255);
    void render_line_width(  int x1, int y1, int x2, int y2, int lineWidth);
    void render_line_width_color(  int x1, int y1, int x2, int y2, int lineWidth, GPE_Color *rendColor, int alphaChannel = 255);

    void render_horizontal_line(int y, int x1, int x2);
    void render_horizontal_line_color( int y, int x1, int x2, GPE_Color *rendColor, int alphaChannel = 255);

    void render_vertical_line( int x, int y1, int y2);
    void render_vertical_line_color( int x, int y1, int y2, GPE_Color *rendColor, int alphaChannel = 255);
    //Line Rendering [ END ]

    //Points Rendering [ BEGIN ]
    void render_point( int x, int y);
    void render_point_color( int x, int y, GPE_Color *rendColor, int alphaChannel = 255);
     //Points Rendering [ END ]

    //Rectangles Rendering [ BEGIN ]
    void render_rect(  GPE_Rect * rendRect,GPE_Color * rendColor,bool outline = false, int alphaChannel = 255);
    void render_rectangle( int x1, int y1, int x2, int y2,  GPE_Color *rendColor,bool outline = false, int alphaChannel = 255);
    void render_square( int x, int y, int squareSize,  GPE_Color *rendColor,bool outline = false, int alphaChannel = 255);
    void render_rotated_rectangle(int xCenter, int yCenter, int w, int h, int angle, GPE_Color * rendColor = NULL, int alphaChannel = 255);
    void render_roundrect(int x1, int y1, int x2, int y2, int rad, bool outline = false);
    void render_roundrect_color( int x1, int y1,int x2, int y2, int rad, bool outline = false,GPE_Color * rendColor = NULL, int alphaChannel  = 255);
    //Rectangles Rendering [ END ]

     //Triangle Rendering [ BEGIN ]
    void render_triangle( int x1, int y1, int x2, int y2, int x3, int y3, bool isOutlne = false, int lineWidth = 1);
    void render_triangle_color( int x1, int y1, int x2, int y2, int x3, int y3, GPE_Color * rendColor = NULL, int alphaChannel = 255, bool isOutlne = false, int lineWidth = 1);
    void render_triangle_flatbottom( double x1, double y1, double x2, double y2, double x3, double y3, GPE_Color * rendColor = NULL, int alphaChannel = 255);
    void render_triangle_flattop( double x1, double y1, double x2, double y2, double x3, double y3, GPE_Color * rendColor = NULL, int alphaChannel = 255);
    //Triangle Rendering [ END ]

    //Animation/Sprite Rendering Functions [ BEGIN ]
    void render_animation(int animId, int subImageToDraw, int xPos, int yPos,  GPE_Rect * cam= NULL);
    void render_animation_resized(int animId, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, GPE_Rect * cam = NULL);
    void render_animation_special(int animId, int subImageToDraw, int xPos, int yPos,  int newWidth, int newHeight,double newAngle,GPE_Color * renderColor, int alpha = 255, GPE_Rect * cam= NULL);
    void render_animation_rotated(int animId, int subImageToDraw, int xPos, int yPos,  double newAngle,int newWidth, int newHeight, GPE_Rect * cam= NULL);

    void render_animation(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos,  GPE_Rect * cam= NULL);
    void render_animation_resized(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight, GPE_Rect * cam = NULL);
    void render_animation_special(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, int newWidth, int newHeight,double newAngle,GPE_Color * renderColor, int alpha = 255,GPE_Rect * cam= NULL);
    void render_animation_rotated(GPE_Animation* spriteToDraw, int subImageToDraw, int xPos, int yPos, double newAngle, int newWidth, int newHeight,  GPE_Rect * cam= NULL);

    int sprite_get_number(GPE_Animation* spriteIn);
    //Animation/Sprite  Rendering [ END ]

    //Texture Rendering Functions [ BEGIN ]
    void render_texture( int textureId, int x, int y,GPE_Rect* clip = NULL, int hAlign = -1, int vAlign = -1 );
    void render_texture_resized( int textureId, int x, int y,int newWidth, int newHeight,GPE_Rect* clip = NULL, int hAlign = -1, int vAlign = -1, GPE_Color * renderColor=NULL );
    void render_texture_rotated( int textureId, int x, int y,double newAngle,int newWidth, int newHeight,GPE_Rect* clip = NULL );
    //Texture Rendering Functions[ END ]

    //Render Mode Related Functions [ BEGIN ]
    int get_artist_blend_mode();
    void set_color( GPE_Color * color);
    void set_color( int r, int g, int b );
    void set_render_alpha( int newAlpha = 255);
    void set_line_width( int newWidth = 1);
    void set_artist_blend_mode( int newBlendMode );
    //Render Mode Related Functions [ END ]

    //Lighting Overlay [ BEGIN ]
    void render_ligting_overlay( int x, int y);
    void render_ligting_overlay_scaled( int x, int y, double scaleSize);
    void resize_ligting_overlay( int w, int h);
    void switch_ligting_overlay(  bool on  = true );
    //Lighting Overlay [ END ]

    //Other Rendering Functions

};

extern GPE_Artist * gcanvas; //game pencil artist?
extern GPE_Artist * GCANVAS; //game pencil artist?

//The Master class that keeps the world going
class GPE_Runtime
{
private:
    GPE_Timer * gameTimer;
    bool loopStarted;
    //
public:
    std::vector < std::string > runtimeLog;
    GPE_Runtime();
    ~GPE_Runtime();

    //Math/Logic related functions
    double get_direction( double x1, double y1, double x2, double y2 );
    double get_distance( double x1, double y1, double x2, double y2 );

    //Length/Direction Functions
    double lengthdir_x( double length, double angle);
    double lengthdir_y( double length, double angle);
    void log_error( std::string errorReported);
    //Loop related function
    int get_delta_time();
    int get_fps();
    int get_ticks();
    void start_loop();
    void reset_loop();
    void end_loop( bool capFPS = true, bool updateRenderer = true );
    void finish_loop();
    void set_fps( int newFPS);
};

extern GPE_Runtime * gpe;
extern GPE_Runtime * GPE;

bool GPE_Init( int argc, char* args[], std::string publisher = "", std::string title = "" );
bool GPE_Quit();

#endif
