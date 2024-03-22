#include "gpe_animiation2d_sdl2.h"

namespace gpe
{
    animation2d_sdl2::animation2d_sdl2()
    {
        sprite_tex = NULL;
        ck_r = 255;
        ck_g = 0;
        ck_b = 255;
    }

    animation2d_sdl2::animation2d_sdl2(  const std::string& anim_name, const std::string& anim_filename, bool transparent_image, uint8_t colorkeyR, uint8_t colorkeyG , uint8_t colorkeyB   )
    {
        texture_is_transparent = transparent_image;
        ck_r = colorkeyR;
        ck_g = colorkeyG;
        ck_b = colorkeyB;
        load_image(  anim_filename, transparent_image );
    }

    animation2d_sdl2::~animation2d_sdl2()
    {
        //dtor
        sprite_quad_tex_top_left.clear();
        sprite_quad_tex_top_right.clear();
        sprite_quad_tex_bottom_left.clear();
        sprite_quad_tex_bottom_right.clear();
    }

    bool animation2d_sdl2::copy_image_source(std::string directory_output_name)
    {
        return animation2d::copy_image_source( directory_output_name);
    }

    animation2d * animation2d_sdl2::copy_self()
    {
        return new animation2d_sdl2( );
    }

    void animation2d_sdl2::clean_up()
    {
        animation2d::clean_up();
    }

    animation2d * animation2d_sdl2::create_new( const std::string& anim_name, const std::string& anim_filename, bool transparent_image, uint8_t colorkeyR, uint8_t colorkeyG , uint8_t colorkeyB )
    {
        return new animation2d_sdl2( anim_name, anim_filename, transparent_image,  colorkeyR,  colorkeyG ,  colorkeyB );
    }

    void animation2d_sdl2::load_image(  const std::string & anim_filename, bool transparent, uint8_t colorkeyR, uint8_t colorkeyG, uint8_t colorkeyB  )
    {
        clean_up();
        if( animation_texture != NULL )
        {
            delete animation_texture;
            animation_texture = NULL;
        }
        sprite_tex = new texture_sdl2();
        file_name = anim_filename;
        if( sff_ex::file_exists( anim_filename) )
        {
            sprite_tex->load_new_texture( anim_filename, -1,transparent, false,  colorkeyR, colorkeyG, colorkeyB );
            tex_width = sprite_tex->get_width();
            tex_height = sprite_tex->get_height();
        }
        animation_texture = sprite_tex;
    }

	bool animation2d_sdl2::render_quad( int sub_image_to_draw, shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha , shape_rect * cam   )
	{
		   //This function assumes there are at least 4 vertices
		if( sprite_tex == nullptr || frameCount == 0 )
		{
			std::cout << "\r Sprite-Texture cant render since its null = ";
			return false;
		}
		//If sprite index is out of bounds we return and exit
		if(( sub_image_to_draw < 0 || sub_image_to_draw >= frameCount) )
		{
			std::cout << "\r Sprite-Texture cant render since its out of bounds = " << sub_image_to_draw;
			return false;
		}

		if( tex_width < 8 || tex_height < 8 )
		{
			std::cout << "/r Sprite-Texture cant render since its texture is less than 8x8px";
			return false; //For this function we need at least 16x16px...
		}

		//The 6 vertices used to draw 2 triangles to create a quad
		SDL_Vertex vert[4];


		//Testing for indices: [start]
		vert[0].position.x = p1.x;
		vert[0].position.y = p1.y;
		vert[0].tex_coord.x = sprite_quad_tex_top_left[ sub_image_to_draw ].x;
		vert[0].tex_coord.y = sprite_quad_tex_top_left[ sub_image_to_draw ].y;

		//Top right vert
		vert[1].position.x = p2.x;
		vert[1].position.y = p2.y;
		vert[1].tex_coord.x = sprite_quad_tex_top_right[ sub_image_to_draw ].x;
		vert[1].tex_coord.y = sprite_quad_tex_top_right[ sub_image_to_draw ].y;

		//Bottom left vert
		vert[2].position.x = p3.x;
		vert[2].position.y = p3.y;
		vert[2].tex_coord.x = sprite_quad_tex_bottom_right[ sub_image_to_draw ].x;
		vert[2].tex_coord.y = sprite_quad_tex_bottom_right[ sub_image_to_draw ].y;

		//Second triangle
		//Top left ( 2nd triangle) vertex
		vert[3].position.x = p4.x;
		vert[3].position.y = p4.y;
		vert[3].tex_coord.x = sprite_quad_tex_bottom_left[ sub_image_to_draw ].x;
		vert[3].tex_coord.y = sprite_quad_tex_bottom_left[ sub_image_to_draw ].y;

		//[end]

		int i_col = 0;
		if( render_color != NULL )
		{
			for(  i_col =0; i_col < 4; i_col++ )
			{
				vert[i_col].color.r = render_color->get_r();
				vert[i_col].color.g = render_color->get_g();
				vert[i_col].color.b = render_color->get_b();
				vert[i_col].color.a = alpha;
			}
		}
		else
		{
			for( i_col = 0; i_col < 4; i_col++ )
			{
				vert[i_col].color.r = 255;
				vert[i_col].color.g = 255;
				vert[i_col].color.b = 255;
				vert[i_col].color.a = 255;
			}
		}

		int new_indices[ 6];
		new_indices[0] = 0;
		new_indices[1] = 1;
		new_indices[2] = 3;
		new_indices[3] = 1;
		new_indices[4] = 2;
		new_indices[5] = 3;

    	return ( SDL_RenderGeometry( renderer_main_sdl2->get_sdl2_renderer(), sprite_tex->get_sdl2_texture(), vert, 4, new_indices, 6 ) == 0 );
    }

    void animation2d_sdl2::setup_animation( int frame_count, int aw, int ah, int sofx, int sofy, int hPad, int vPad,int animAlign )
    {
        animation2d::setup_animation( frame_count, aw, ah, sofx, sofy, hPad, vPad, animAlign );
        setup_quad_coordinates();
    }

    void animation2d_sdl2::setup_fullimg_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad,int animAlign )
    {
        animation2d::setup_fullimg_animation( aw, ah, sofx, sofy, hPad, vPad, animAlign );
        setup_quad_coordinates();
    }


    void animation2d_sdl2::setup_quad_coordinates()
    {
        sprite_quad_tex_top_left.clear();
        sprite_quad_tex_top_right.clear();
        sprite_quad_tex_bottom_left.clear();
        sprite_quad_tex_bottom_right.clear();


        if( tex_width <=0 || tex_height <= 0 )
        {
            gpe::error_log->report("Tex height/width invalid for "+ file_name + "," + name );
            return;
        }

        animation2d_frame_data * c_frame = NULL;
        for( int i_frame = 0; i_frame < frameCount; i_frame++ )
        {
            c_frame = frame_data[i_frame];
            if( c_frame != nullptr )
            {
                sprite_quad_tex_top_left.push_back( { c_frame->frame_box->get_x() / tex_width, c_frame->frame_box->get_y() / tex_height} );
                sprite_quad_tex_top_right.push_back( {  c_frame->frame_box->get_x2() /tex_width, c_frame->frame_box->get_y() /tex_height} );
                sprite_quad_tex_bottom_left.push_back( { c_frame->frame_box->get_x() /tex_width,  c_frame->frame_box->get_y2() / tex_height} );
                sprite_quad_tex_bottom_right.push_back( { c_frame->frame_box->get_x2() / tex_width, c_frame->frame_box->get_y2() / tex_height } );
            }
            else
            {
                sprite_quad_tex_top_left.push_back( {0,0} );
                sprite_quad_tex_top_right.push_back( {1,0} );
                sprite_quad_tex_bottom_left.push_back( {0,1} );
                sprite_quad_tex_bottom_right.push_back( {1,1} );
            }
        }
    }
}
