/*
gpe_particles2d.h
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

#ifndef gpe_particles_h
#define gpe_particles_h

#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "GPE_Collisions.h"
#include "gpe_globals.h"
#include "gpe_texture_base.h"
#include "gpe_runtime.h"
#include "gpe_branch.h"
#include "../other_libs/semath.h"
#include "../other_libs/stg_ex.h"

namespace gpe
{
    const int part_shape_point = 0;
    const int part_shape_square = 1;
    const int part_shape_square_outline = 2;
    const int part_shape_rectangle = 3;
    const int part_shape_rectangle_outline = 4;
    const int part_shape_circle = 5;
    const int part_shape_circle_outline = 6;
    const int part_shape_line = 7;
    const int part_shape_linecapped = 8;

    const int PARTICLE_MAX_PER_EMITTER = 3000;

    class particle
    {
        public:
            texture_base * pTex;
            bool rotateParticle;
            float xStart,  yStart;
            float pSpeed;
            float pDirection;
            float pGravity;
            float pGravityDirection;
            float xPos,  yPos;
            float xVelocity, yVelocity;
            float width, height;
            float startScale;
            float particleScale;
            int shapeType;
            float particleLife;
            float startLife;
            color * startColor;
            color * myColor;
            color * endColor;
            particle( );
            ~particle();
            void die();
            //Three different render calls to avoid the constant if-checks
            void render_particle();
            void render_particle_shape();
            void render_particle_rotated();
            void set_position(  float x, float y );
            void set_velocity(  float angle, float speed );
            void update( float delta );
    };

    class particle_emitter: public branch
    {
        private:
            std::vector< particle * > particles;
            texture_base * pTex;
            int maxParticles;
            int currentParticleCount;
            float emissionRate;
            float emissionMSRate;
            int particlesPerEmission;
            float emissionCounter;
            int fallBackShapeType;
        public:
            float angleMin, angleMax;
            float speedMin, speedMax;
            float emitXPos, emitYPos;
            float xRandom, yRandom;
            int texBlendMode;
            bool localEmitter;
            bool showBox;
            bool rotateTexture;
            float emitterXPos;
            float emitterYPos;
            bool sortByIndex;
            bool loopEmission;
            bool emissionPaused;
            float radius;
            float lifeMin, lifeMax;
            int shapeType;
            float shapeW, shapeH;
            std::string emitterName;
            std::string imageLocation;
            color * startColor;
            color * startColorVar;
            color * endColor;
            color * endColorVar;

            particle_emitter(int x, int y, bool isLocal = true, int pMax = 100 );
            ~particle_emitter();
            bool can_emit();
            void burst_emit();
            void change_texture( texture_base * newTex, int newBlendMode = blend_mode_blend );
            void clear_particles();
            int get_count();
            int get_remaining();
            void emit_particles();
            void process_emitter(  );
            void render();
            void respawn_particle( particle * part );
            void reset();
            void set_blend_mode( int newMode );
            void set_emission_rate( float emr = 30 );
            void set_fallback_shape( int newFBS );
            void set_max_particles( int newMax = 50, bool removeExtra = false );
    };
}
#endif //gpe_particles_h
