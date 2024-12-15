/*
sdl_rederparty.h
This file is part of:
SDL2_RenderParty
https://www.pawbyte.com/sdl2_renderparty
Copyright (c) 2024 Nathan Hurde, Chase Lee.

Copyright (c) 2024 PawByte.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the �Software�), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-SDL2_RenderParty <https://www.pawbyte.com/sdl2_renderparty>


*/


#ifndef SDL2_RENDERPARTY_HEADER
#define SDL2_RENDERPARTY_HEADER

#define SDL2_RENDERPARTY_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

    #define SDL2_RENDERPARTY_VERSION_MAJOR 1
    #define SDL2_RENDERPARTY_MINOR 1
    #define SDL2_RENDERPARTY_PATCH 3

    #define SDL2_RENDERPARTY_CIRCLE_POINT_MIN 4

    #include <stdbool.h>
    #include "SDL.h"

    /*
    Requires SDL_RenderGeometry, which was introduced in SDL 2.0.18

    SDL_2.0.18 or higher is required!



    */



    #define sdl_party_max_vertex_count 999999

    extern SDL_Vertex SDL_RenderParty_Vertex_List[sdl_party_max_vertex_count];
    extern bool SDL2_Party_WasInitted;
    extern int SDL2_Party_Circle_Points; //Detailed enough for smaller screens.

    //  Effects for polygon rendering, some are experimental. Toggle to see which effect works best for your game/app.

    enum
    {
        sdl_partyeffect_full = 0,
        sdl_partyeffect_additive = 1,
        sdl_partyeffect_subtractive = 2,
        sdl_partyeffect_inverted =3,
        sdl_partyeffect_bottom_up = 4,
        sdl_partyeffect_full_inverted = 5,
        sdl_partyeffect_texture_mapped = 6, // W.I.P
        sdl_partyeffect_none = 7,
    };


    //  Math related variables

    #define sdl_renderparty_math_pi 3.14159265359
    const float sdl_renderparty_math_to_radians = 180.f/sdl_renderparty_math_pi;
    const float sdl_renderparty_math_degrees_multiplier = 180.f/sdl_renderparty_math_pi;
    const float sdl_renderparty_math_radians_multiplier = sdl_renderparty_math_pi/180.f;


    //  Function Declarations

    bool SDL2_RenderParty_Init();
    bool SDL2_RenderParty_Quit();
    float SDL2_RenderParty_LengthdirX( float length, float angle  );
    float SDL2_RenderParty_LengthdirY( float length, float angle  );
    bool SDL_RenderParty_Circle( SDL_Renderer * renderer, SDL_Texture * tex, SDL_Point pos, int circle_radius, SDL_Color * circle_colour, int alpha, float start_angle, int texture_effect );
    int SDL_RenderParty_Fans( SDL_Renderer *renderer, SDL_Texture *tex, const SDL_Vertex * vertices, int num_vertices, int texture_effect , int start_vertice_pos);
    bool SDL_RenderParty_Polygon( SDL_Renderer * renderer, SDL_Texture * tex, SDL_Point pos, int shape_length , int point_count, SDL_Color * shape_colour, int alpha, float start_angle, int texture_effect, int start_vertice_pos );
    bool SDL_RenderParty_Quad( SDL_Renderer *renderer, SDL_Texture *tex, const SDL_Vertex * vertices, bool show_colors, bool update_tex_cords );
    bool SDL_RenderParty_SetCirclePoints( int new_circle_points );
    int SDL_RenderParty_Strips(SDL_Renderer *renderer, SDL_Texture *tex, const SDL_Vertex * vertices, int num_vertices, bool is_closed, int texture_effect, int start_vertice_pos  );

#ifdef __cplusplus
}
#endif
#endif // SDL2_RENDERPARTY_HEADER


//
//Implementation Phase
//
#if defined(SDL2_RENDERPARTY_IMPLEMENTATION)

SDL_Vertex SDL_RenderParty_Vertex_List[sdl_party_max_vertex_count];
bool SDL2_Party_WasInitted = false;
int SDL2_Party_Circle_Points = 32; //Detailed enough for smaller screens.

// Function Implementation

bool SDL2_RenderParty_Init()
{
    SDL2_Party_WasInitted = SDL_WasInit(0);

    if( !SDL2_Party_WasInitted )
    {
        return false;
    }
    for( int i = 0; i <sdl_party_max_vertex_count; i+= 1 )
    {
        SDL_RenderParty_Vertex_List[i].color.r = SDL_RenderParty_Vertex_List[i].color.g = SDL_RenderParty_Vertex_List[i].color.b = SDL_RenderParty_Vertex_List[i].color.a = 255;
    }
    return SDL2_Party_WasInitted;
}

bool SDL2_RenderParty_Quit()
{
    //Currently no use for this function, but would in later versions probably...
    return true;
}

//Math Related Operations

float SDL2_RenderParty_LengthdirX( float length, float angle  )
{
    return SDL_cos( angle  ) * length;
}

float SDL2_RenderParty_LengthdirY( float length, float angle )
{
    return  SDL_sin( angle  ) * length;
}

/*
Now onto the render operations that call SDL_RenderGeometry.
*/

bool SDL_RenderParty_Circle( SDL_Renderer * renderer, SDL_Texture * tex, SDL_Point pos, int circle_radius, SDL_Color * circle_colour, int alpha, float start_angle, int texture_effect )
{
    return SDL_RenderParty_Polygon( renderer, tex, pos, circle_radius* 2.f, SDL2_Party_Circle_Points, circle_colour, alpha, start_angle, texture_effect, 0  );
}

//This function assumes there are at least 4 vertices
bool SDL_RenderParty_Quad( SDL_Renderer *renderer, SDL_Texture *tex, const SDL_Vertex * vertices, bool show_colors, bool update_tex_cords  )
{
    //This function assumes there are at least 4 vertices
    SDL_Vertex vert[4];


    vert[0].position.x = vertices[0].position.x;
    vert[0].position.y = vertices[0].position.y;


    vert[1].position.x = vertices[1].position.x;
    vert[1].position.y = vertices[1].position.y;


    vert[2].position.x = vertices[2].position.x;
    vert[2].position.y = vertices[2].position.y;

    vert[3].position.x = vertices[3].position.x;
    vert[3].position.y = vertices[3].position.y;

    //draw the entire texture, otherwise use previous vertice positions
    if( update_tex_cords )
    {
        vert[0].tex_coord.x = 0;
        vert[0].tex_coord.y = 0;
        vert[1].tex_coord.x = 1;
        vert[1].tex_coord.y = 0;
        vert[2].tex_coord.x = 1;
        vert[2].tex_coord.y = 1;
        vert[3].tex_coord.x = 0;
        vert[3].tex_coord.y = 1;
    }

    if( show_colors )
    {
        for( int i_col =0; i_col < 4; i_col+= 1 )
        {
            vert[i_col].color.r = vertices[i_col].color.r;
            vert[i_col].color.g = vertices[i_col].color.g;
            vert[i_col].color.b = vertices[i_col].color.b;
            vert[i_col].color.a = vertices[i_col].color.a;
        }
    }


    int new_indices[ 6];
    new_indices[0] = 1;
    new_indices[1] = 2;
    new_indices[2] = 3;
    new_indices[3] = 0;
    new_indices[4] = 1;
    new_indices[5] = 3;

    if( update_tex_cords )
    {
        return (SDL_RenderGeometry(renderer, tex, vert, 4, new_indices, 6 ) == 0 );
    }
    else
    {
        return (SDL_RenderGeometry(renderer, tex, vertices, 4, new_indices, 6 ) == 0 );
    }
}


int SDL_RenderParty_Fans( SDL_Renderer *renderer, SDL_Texture *tex, const SDL_Vertex *vertices, int num_vertices, int texture_effect, int start_vertice_pos )
{
    //triangles are made of 3 vertices minium
    if( num_vertices < 3)
    {
        return 0;
    }

    SDL_FPoint tex_coords_default[4];

    tex_coords_default[0].x = 0;
    tex_coords_default[0].y = 0;


    tex_coords_default[1].x = 1;
    tex_coords_default[1].y = 0;

    tex_coords_default[2].x = 1;
    tex_coords_default[2].y = 1;

    tex_coords_default[3].x = 0;
    tex_coords_default[3].y = 1;

    int quad_position = 0;;


    int i_vert_place = 0;

    while(num_vertices >= sdl_party_max_vertex_count/3 )
    {
       SDL_RenderParty_Fans( renderer, tex,  vertices, sdl_party_max_vertex_count -1, texture_effect, i_vert_place  );
       num_vertices -= sdl_party_max_vertex_count-1;
       i_vert_place += sdl_party_max_vertex_count -1;
    }

    int vert_pos_offset = start_vertice_pos;

    float found_max_w = 0;
    float found_max_h = 0;

    //Expensive to do a double-loop and comparision perhaps...
    if( texture_effect == sdl_partyeffect_texture_mapped )
    {
        for( int i_tex_vert = 0; i_tex_vert < num_vertices; i_tex_vert+= 1 )
        {
            if( vertices[vert_pos_offset].position.x > found_max_w )
            {
                found_max_w = vertices[vert_pos_offset].position.x;
            }

            if( vertices[vert_pos_offset].position.y > found_max_h )
            {
                found_max_h = vertices[vert_pos_offset].position.y;
            }
            vert_pos_offset+= 1;
        }

    }

    vert_pos_offset = start_vertice_pos;
    for( int i_vert = 0; i_vert < num_vertices; i_vert+= 1 )
    {
        SDL_RenderParty_Vertex_List[i_vert].position.x = vertices[vert_pos_offset].position.x;
        SDL_RenderParty_Vertex_List[i_vert].position.y = vertices[vert_pos_offset].position.y;

        SDL_RenderParty_Vertex_List[i_vert].color.r = vertices[vert_pos_offset].color.r;
        SDL_RenderParty_Vertex_List[i_vert].color.g = vertices[vert_pos_offset].color.g;
        SDL_RenderParty_Vertex_List[i_vert].color.b = vertices[vert_pos_offset].color.b;
        SDL_RenderParty_Vertex_List[i_vert].color.a = vertices[vert_pos_offset].color.a;

        SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = vertices[vert_pos_offset].tex_coord.x ;
        SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = vertices[vert_pos_offset].tex_coord.y ;

        //SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = 1.f - (float)i_vert/(float)num_vertices;//tex_coords_default[i_vert_place].x ;
        //SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = 1.f - (float)i_vert/(float)num_vertices;//tex_coords_default[i_vert_place].y ;
        if( tex != NULL )
        {
            switch( texture_effect )
            {
                case sdl_partyeffect_additive:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = (float)(i_vert)/(float)(num_vertices);
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = (float)(i_vert)/(float)(num_vertices);
                break;

                case sdl_partyeffect_subtractive:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = 1.f - (float)i_vert/(float)num_vertices;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = 1.f -(float)i_vert/(float)num_vertices;
                break;

                case sdl_partyeffect_bottom_up:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x =  (float)i_vert/(float)num_vertices;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = 1.f -(float)i_vert/(float)num_vertices;
                break;

                case sdl_partyeffect_inverted:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x =  1.f-(float)i_vert/(float)num_vertices;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = (float)i_vert/(float)num_vertices;
                break;

                case sdl_partyeffect_full_inverted:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = 1.f - tex_coords_default[i_vert_place].x ;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = 1.f - tex_coords_default[ i_vert_place].y ;
                break;

                case sdl_partyeffect_full:
                    if( ( num_vertices-2% 4) == 0 )
                    {
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = tex_coords_default[quad_position].x ;
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = tex_coords_default[quad_position].y ;
                        quad_position+= 1;
                        if( quad_position >= 4 )
                        {
                            quad_position = 0;
                        }
                    }
                    else
                    {
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = (float)(i_vert)/(float)(num_vertices);
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = (float)(i_vert)/(float)(num_vertices);
                    }
                break;
                case sdl_partyeffect_texture_mapped:
                    if( i_vert_place == 0 )
                    {
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = 0.5f;
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = 0.5f;
                    }
                    else
                    {
                        if( found_max_w == 0)
                        {
                            SDL_RenderParty_Vertex_List[i_vert].tex_coord.x =  0.5f;
                        }
                        else
                        {
                            SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = (float)(vertices[i_vert_place].position.x / found_max_w);
                        }
                        if( found_max_h == 0)
                        {
                            SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = 0.5f;
                        }
                        else
                        {
                            SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = (float)(vertices[i_vert_place].position.y / found_max_h);
                        }
                    }
                break;
                default:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = vertices[i_vert].tex_coord.x ;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = vertices[i_vert].tex_coord.y ;
                break;
            }
        }
        i_vert_place+= 1;
        vert_pos_offset+= 1;
        if( i_vert_place >=3 )
        {
            i_vert_place = 0;
        }
    }

    if( num_vertices == 3)
    {
        //if its only 3 vertices save time from doing for loop and just draw the 1 triangle
        if( tex != NULL )
        {
            SDL_RenderGeometry(renderer, tex, SDL_RenderParty_Vertex_List, num_vertices, NULL, 0 );
            return 3;
        }
        SDL_RenderGeometry(renderer, NULL, SDL_RenderParty_Vertex_List, num_vertices, NULL, 0);
        return 3;
    }
    else if( num_vertices == 4 && tex != NULL )
    {
        if( SDL_RenderParty_Quad( renderer, tex, SDL_RenderParty_Vertex_List, true, false ) )
        {
            return 4;
        }
        return 0;
    }

    //The following lines are used for vertices of 4 or more and will all connect to the 1st, plus last 2 verts
    unsigned int estimated_indices = (num_vertices-2)*3;
    int new_indices[ estimated_indices];
    new_indices[0] = 0;
    new_indices[1] = 1;
    new_indices[2] = 2;
    int i_vert = 3; //used to loop through remaining vertices
    int index_pos = 3; //used to create each 3 indices
    while(  i_vert < num_vertices )
    {
        new_indices[index_pos] = 0;
        new_indices[index_pos+1] = i_vert -1;
        new_indices[index_pos+2] = i_vert;

        i_vert+= 1;
        index_pos+= 3;
    }
    if( tex != NULL )
    {
        SDL_RenderGeometry(renderer, tex, SDL_RenderParty_Vertex_List, num_vertices, new_indices, estimated_indices );
    }
    else
    {
        SDL_RenderGeometry(renderer, NULL, SDL_RenderParty_Vertex_List, num_vertices, new_indices, estimated_indices );
    }
    return estimated_indices;
}

bool SDL_RenderParty_Polygon( SDL_Renderer * renderer, SDL_Texture * tex, SDL_Point pos, int shape_length , int point_count, SDL_Color * shape_colour, int alpha, float start_angle, int texture_effect, int start_vertice_pos  )
{
    if( renderer == NULL || shape_length < 1 || point_count < 3 )
    {
        return false;
    }

    start_angle -= 90.f;
    float end_angle = start_angle + 360.f;

    start_angle = start_angle * sdl_renderparty_math_radians_multiplier;
    end_angle = end_angle * sdl_renderparty_math_radians_multiplier;

    float step = (end_angle - start_angle) / (float)(point_count);

    float theta = start_angle;

    float vx  = 0,  vy = 0;

    int arc_i = 0;
    point_count+=2;
    SDL_Vertex line_render_points[ point_count ];

    vx = pos.x;
    vy = pos.y;

    float tex_coord_angle = 0; //0.f;
    line_render_points[ 0 ].position.x = pos.x;
    line_render_points[ 0 ].position.y = pos.y;

    if( shape_colour != NULL )
    {
        line_render_points[ 0 ].color.r = shape_colour->r;
        line_render_points[ 0 ].color.g = shape_colour->g;
        line_render_points[ 0 ].color.b = shape_colour->b;
    }
    else
    {
        line_render_points[0].color.r = 255;
        line_render_points[0].color.g = 255;
        line_render_points[0].color.b = 255;
    }

    line_render_points[ 0 ].color.a = alpha;
    line_render_points[ 0 ].tex_coord.x = 0.5f;
    line_render_points[ 0 ].tex_coord.y = 0.5f;

    theta+=step;
    tex_coord_angle += step;

    for( arc_i = 0; arc_i < point_count; arc_i+= 1)
    {
        vx = pos.x + SDL2_RenderParty_LengthdirX( shape_length/2.f, theta);
        vy = pos.y + SDL2_RenderParty_LengthdirY( shape_length/2.f, theta);
        line_render_points[arc_i].position.x = vx;
        line_render_points[arc_i].position.y = vy;
        if( shape_colour != NULL )
        {
            line_render_points[arc_i].color.r = shape_colour->r;
            line_render_points[arc_i].color.g = shape_colour->g;
            line_render_points[arc_i].color.b = shape_colour->b;
        }
        else
        {
            line_render_points[arc_i].color.r = 255;
            line_render_points[arc_i].color.g = 255;
            line_render_points[arc_i].color.b = 255;
        }
        line_render_points[arc_i].color.a = alpha;

        line_render_points[arc_i].tex_coord.x = 0.5f + SDL2_RenderParty_LengthdirX( 0.5f, tex_coord_angle);
        line_render_points[arc_i].tex_coord.y = 0.5f + SDL2_RenderParty_LengthdirY( 0.5f, tex_coord_angle);
        theta += step;
        tex_coord_angle += step;
    }
    SDL_RenderParty_Fans( renderer, tex, line_render_points, point_count, texture_effect, 0 );
    return true;
}

bool SDL_RenderParty_SetCirclePoints( int new_circle_points )
{
    if( new_circle_points < SDL2_RENDERPARTY_CIRCLE_POINT_MIN )
    {
        return false;
    }
    SDL2_Party_Circle_Points = new_circle_points;
    return true;
}

//int returns the amount of indices were rendered
int SDL_RenderParty_Strips(SDL_Renderer *renderer, SDL_Texture *tex, const SDL_Vertex * vertices, int num_vertices, bool is_closed , int texture_effect, int start_vertice_pos  )
{
    if( start_vertice_pos < 0 )
    {
        start_vertice_pos = 0;
    }

    if( num_vertices < 3)
    {
        return 0;
    }
    else if( num_vertices == 3)
    {
        SDL_RenderGeometry(renderer, tex, vertices, num_vertices, NULL, 0);
        return 3;
    }

    unsigned int estimated_indices = 3 + (num_vertices-3)*3;
    if( is_closed)
    {
        //add 3 more indices for connecting triangle
        estimated_indices += 3;
    }
    int new_indices[ estimated_indices];

    SDL_FPoint tex_coords_default[4];
    tex_coords_default[0].x = 0;
    tex_coords_default[0].y = 0;


    tex_coords_default[1].x = 1;
    tex_coords_default[1].y = 0;

    tex_coords_default[2].x = 1;
    tex_coords_default[2].y = 1;

    tex_coords_default[3].x = 0;
    tex_coords_default[3].y = 1;

    int i_vert_place = 0;



    while(num_vertices >= sdl_party_max_vertex_count/3 )
    {
       SDL_RenderParty_Strips( renderer, tex,  vertices, sdl_party_max_vertex_count -1, is_closed , texture_effect, i_vert_place  );
       num_vertices -= sdl_party_max_vertex_count-1;
       i_vert_place += sdl_party_max_vertex_count -1;
    }

    int vert_pos_offset = start_vertice_pos;


    float found_max_w = 0;
    float found_max_h = 0;

    //Expensive to do a double-loop and comparision perhaps...
    if( texture_effect == sdl_partyeffect_texture_mapped )
    {
        for( int i_tex_vert = 0; i_tex_vert < num_vertices; i_tex_vert+= 1 )
        {
            if( vertices[vert_pos_offset].position.x > found_max_w )
            {
                found_max_w = vertices[vert_pos_offset].position.x;
            }

            if( vertices[vert_pos_offset].position.y > found_max_h )
            {
                found_max_h = vertices[vert_pos_offset].position.y;
            }
            vert_pos_offset+= 1;
        }

    }

    vert_pos_offset = start_vertice_pos;
    for( int i_vert = 0; i_vert < num_vertices; i_vert += 1 )
    {
        SDL_RenderParty_Vertex_List[i_vert].color.r = vertices[vert_pos_offset].color.r;
        SDL_RenderParty_Vertex_List[i_vert].color.g = vertices[vert_pos_offset].color.g;
        SDL_RenderParty_Vertex_List[i_vert].color.b = vertices[vert_pos_offset].color.b;
        SDL_RenderParty_Vertex_List[i_vert].color.a = vertices[vert_pos_offset].color.a;
        SDL_RenderParty_Vertex_List[i_vert].position.x = vertices[vert_pos_offset].position.x;
        SDL_RenderParty_Vertex_List[i_vert].position.y = vertices[vert_pos_offset].position.y;

        if( tex != NULL )
        {
            switch( texture_effect )
            {
                case sdl_partyeffect_additive:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = (float)i_vert/(float)num_vertices;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = (float)i_vert/(float)num_vertices;
                break;

                case sdl_partyeffect_subtractive:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = 1.f - (float)i_vert/(float)num_vertices;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = 1.f -(float)i_vert/(float)num_vertices;
                break;

                case sdl_partyeffect_bottom_up:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x =  (float)i_vert/(float)num_vertices;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = 1.f -(float)i_vert/(float)num_vertices;
                break;

                case sdl_partyeffect_inverted:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x =  1.f-(float)i_vert/(float)num_vertices;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = (float)i_vert/(float)num_vertices;
                break;

                case sdl_partyeffect_full_inverted:
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = tex_coords_default[ num_vertices - i_vert_place -1].x ;
                    SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = tex_coords_default[ num_vertices - i_vert_place -1].y ;
                break;

                case sdl_partyeffect_texture_mapped:
                    if( found_max_w == 0)
                    {
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = tex_coords_default[i_vert_place].x ;

                    }
                    else
                    {
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = vertices[vert_pos_offset].position.x / found_max_w;
                    }
                    if( found_max_h == 0)
                    {
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = tex_coords_default[i_vert_place].y ;
                    }
                    else
                    {
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = vertices[vert_pos_offset].position.y / found_max_h;
                    }
                break;

                default:
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = tex_coords_default[i_vert_place].x ;
                        SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = tex_coords_default[i_vert_place].y ;
                break;
            }
        }
        else
        {
            SDL_RenderParty_Vertex_List[i_vert].tex_coord.x = 0;
            SDL_RenderParty_Vertex_List[i_vert].tex_coord.y = 0;
        }
        i_vert_place+= 1;
        vert_pos_offset+= 1 ;
        if( i_vert_place >=3 )
        {
            i_vert_place = 0;
        }
    }

    int i_vert = 3;


    new_indices[0] = 0;
    new_indices[1] = 1;
    new_indices[2] = 2;

    int index_pos = 3;
    while(  i_vert < num_vertices )
    {
        new_indices[index_pos] = i_vert ;
        new_indices[index_pos+1] = i_vert -1;
        new_indices[index_pos+2] = i_vert -2;
        i_vert+= 1 ;
        index_pos += 3;
    }
    if( is_closed )
    {
        new_indices[index_pos] = num_vertices  -2;
        new_indices[index_pos+1] = num_vertices -1;
        new_indices[index_pos+2] = 0;
    }
    if( tex != NULL )
    {
        SDL_RenderGeometry(renderer, tex, SDL_RenderParty_Vertex_List, num_vertices, new_indices, estimated_indices );
    }
    else
    {
        SDL_RenderGeometry(renderer, NULL, SDL_RenderParty_Vertex_List, num_vertices,  new_indices, estimated_indices );
    }
    return estimated_indices;
}

#endif // SDL2_RENDERPARTY_IMPLEMENTATION




