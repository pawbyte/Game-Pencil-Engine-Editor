/*
GPE_Particles.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#ifndef GPE_PARTICLES_H
#define GPE_PARTICLES_H

#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "GPE_Functions.h"
#include "GPE_Collisions.h"
#include "GPE_String_Ex.h"
#include "GPE_Globals.h"
#include "GPE_Texture_Base.h"
#include "GPE_Engine.h"
#include "GPE_Branch.h"
#include "GPE_Math.h"

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

class GPE_Particle
{
public:
    GPE_Texture_Base * pTex;
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
    GPE_Color * startColor;
    GPE_Color * myColor;
    GPE_Color * endColor;
    GPE_Particle( );
    ~GPE_Particle();
    void die();
    //Three different render calls to avoid the constant if-checks
    void render_particle();
    void render_particle_shape();
    void render_particle_rotated();
    void set_position(  float x, float y );
    void set_velocity(  float angle, float speed );
    void update( float delta );
};

class GPE_ParticleEmitter: public GPE_Branch
{
private:
    std::vector< GPE_Particle * > particles;
    GPE_Texture_Base * pTex;
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
    GPE_Color * startColor;
    GPE_Color * startColorVar;
    GPE_Color * endColor;
    GPE_Color * endColorVar;

    GPE_ParticleEmitter(int x, int y, bool isLocal = true, int pMax = 100 );
    ~GPE_ParticleEmitter();
    bool can_emit();
    void burst_emit();
    void change_texture( GPE_Texture_Base * newTex, int newBlendMode = blend_mode_blend );
    void clear_particles();
    int get_count();
    int get_remaining();
    void emit_particles();
    void process_emitter(  );
    void render();
    void respawn_particle( GPE_Particle * part );
    void reset();
    void set_blend_mode( int newMode );
    void set_emission_rate( float emr = 30 );
    void set_fallback_shape( int newFBS );
    void set_max_particles( int newMax = 50, bool removeExtra = false );
};

#endif
