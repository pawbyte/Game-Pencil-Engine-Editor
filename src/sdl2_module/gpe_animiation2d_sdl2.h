#ifndef GPE_ANIMIATION2D_sdl2_h
#define GPE_ANIMIATION2D_sdl2_h

#include "../gpe/gpe_animation2d.h"
#include "gpe_renderer_sdl2.h"
#include "gpe_texture_sdl2.h"

namespace gpe
{

    class animation2d_sdl2: public animation2d
    {
        protected:
            /*
            2.Dish triangle/quad functions will be left up to the child class, since implementation MAY vary per backend...
            This is simply precautionary in case texture coordinates are not between 0 and 1  floats
            By storing the FPoints for each sprite inside of the sprite, this "should" save us the on the fly calculation for the animation
            */
            std::vector < SDL_FPoint > sprite_quad_tex_top_left;
            std::vector < SDL_FPoint > sprite_quad_tex_top_right;
            std::vector < SDL_FPoint > sprite_quad_tex_bottom_left;
            std::vector < SDL_FPoint > sprite_quad_tex_bottom_right;
        private:
            texture_sdl2 * sprite_tex; //we will store the actual SDL texture of the sprite to avoid typecasting for our quad function and "if later added" other functions
        public:
            animation2d_sdl2();
             animation2d_sdl2(  const std::string &anim_name, const std::string& anim_filename, bool transparent_image = false, uint8_t colorkeyR = 255, uint8_t colorkeyG = 0, uint8_t colorkeyB = 255);
            virtual ~animation2d_sdl2();

            virtual bool copy_image_source(std::string directory_output_name);
            virtual animation2d * copy_self();
            virtual void clean_up();
            virtual animation2d * create_new( const std::string& anim_name, const std::string& anim_filename, bool transparent_image, uint8_t colorkeyR = 255, uint8_t colorkeyG = 0, uint8_t colorkeyB = 255 );

            virtual void load_image(  const std::string &anim_filename, bool transparent = true, uint8_t colorkeyR = 255, uint8_t colorkeyG = 0, uint8_t colorkeyB = 255  );

            //So far is the only render function that needs to be virtual in case implementation differs too greatly per backend
            bool render_quad( int sub_image_to_draw, shape_point2d p1 , shape_point2d p2, shape_point2d p3, shape_point2d p4, color * render_color, int alpha = 255, shape_rect * cam = nullptr  );

            virtual void setup_animation( int frame_count, int aw, int ah, int sofx, int sofy, int hPad, int vPad,int animAlign = dir_top_left );
            virtual void setup_fullimg_animation(  int aw, int ah, int sofx, int sofy, int hPad, int vPad,int animAlign = dir_top_left );
            void setup_quad_coordinates();

    };
}

#endif // GPE_ANIMIATION2D_sdl2_h
