/*
gpe_particles2d.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2021 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2021 PawByte LLC.
Copyright (c) 2014-2021 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_artist_base.h"
#include "gpe_runtime.h"
#include "gpe_particles2d.h"

namespace gpe
{
    particle::particle( )
    {
        //Defaults
        rotate_particle = true;
        x_pos = x_start = 0;
        y_pos = y_start = 0;
        startLife = 1;
        particleLife = 0;
        start_scale = particle_scale = 1;
        shapeType = part_shape_circle;
        p_direction = 0;
        x_velocity = 0;
        y_velocity = 0;

        p_speed = 0;
        particle_texture = nullptr;
        startColor = new color("startColor",255,255,0);
        myColor = new color("myColor",255,255,255,0);
        endColor = new color("endColor",0,0,0,0 );
        width = height = 32;
        p_gravity = 0;
        p_gravity_direction = 0;

    }

    particle::~particle()
    {

    }

    void particle::die()
    {
        particleLife = 0;
        myColor->change_a( 0 );
    }

    void particle::render_particle(  )
    {
        if( particleLife > 0 && myColor->get_a() > 0 )
        {
            if( particle_texture!=nullptr )
            {
                particle_texture->render_tex_scaled(x_pos-camera_current->x - (width*particle_scale)/2,y_pos-camera_current->y - (height*particle_scale)/2,particle_scale,particle_scale,nullptr, myColor, myColor->get_a() );
            }
        }
    }

    void particle::render_particle_shape( )
    {
        if( particleLife > 0 && myColor->get_a() > 0 )
        {
            if( particle_scale!=0 )
            {
                switch( shapeType)
                {
                    case part_shape_circle:
                        gcanvas->render_circle_filled_color( x_pos-camera_current->x,y_pos-camera_current->y,particle_scale * width,myColor, myColor->get_a() );
                    break;

                    case part_shape_circle_outline:
                        gcanvas->render_circle_outline_color( x_pos-camera_current->x,y_pos-camera_current->y,particle_scale * width,myColor, myColor->get_a() );
                    break;

                    case part_shape_rectangle:
                        gcanvas->render_rectangle( x_pos-camera_current->x,y_pos-camera_current->y,x_pos-camera_current->x+particle_scale*width, y_pos-camera_current->y+ particle_scale*height, myColor, false, myColor->get_a() );
                    break;
                    case part_shape_rectangle_outline:
                        gcanvas->render_rectangle( x_pos-camera_current->x,y_pos-camera_current->y,x_pos-camera_current->x+particle_scale*width, y_pos-camera_current->y+ particle_scale*height, myColor, true, myColor->get_a() );
                    break;

                    case part_shape_square_outline:
                    //defaults to square
                        gcanvas->render_square( x_pos-camera_current->x,y_pos-camera_current->y,particle_scale*width, myColor, true, myColor->get_a() );
                    break;

                    case part_shape_line:
                        gcanvas->render_line_width_color( x_start-camera_current->x,y_start-camera_current->y, x_pos-camera_current->x, y_pos- camera_current->y,particle_scale*width, myColor, myColor->get_a() );
                    break;

                    case part_shape_linecapped:
                        gcanvas->render_line_capped_color( x_start-camera_current->x,y_start-camera_current->y, x_pos-camera_current->x, y_pos- camera_current->y,particle_scale*width, myColor, myColor->get_a() );
                    break;

                    default:
                    //defaults to square
                        gcanvas->render_square( x_pos-camera_current->x,y_pos-camera_current->y,particle_scale*width, myColor, false, myColor->get_a() );
                    break;
                }
            }
        }
    }

    void particle::render_particle_rotated( )
    {
        if( particleLife > 0 && myColor->get_a() > 25 )
        {
            if( particle_texture!=nullptr )
            {
                particle_texture->render_tex_special(x_pos-camera_current->x,y_pos-camera_current->y,p_direction, abs(particle_scale * width),abs(particle_scale * height), myColor, nullptr,myColor->get_a() );
            }
        }
    }

    void particle::set_position(  float x, float y )
    {
        x_pos = x_start = x;
        y_pos = y_start = y;
    }

    void particle::set_velocity(  float angle, float speed )
    {
        p_direction = angle;
        float angleInRadians = p_direction * semath::math_radians_multiplier;

        if( speed == 0.f )
        {
            p_speed = 0.f;
        }
        else
        {
            p_speed = speed / 100.f;
        }
        x_velocity = cos(angleInRadians);
        y_velocity = sin(angleInRadians);

    }

    void particle::update( float delta )
    {
        particleLife -= delta;

        if( particleLife > 0.f )
        {
            float ageRatio = (float)particleLife/ (float)startLife;
            if( ageRatio > 1.f )
            {
                //error_log->report("Invalid Age Range Found ["+ stg_ex::int_to_string( ageRatio)+"]" );
                ageRatio = 1;
            }
            int newR = color_system->merge_channel(endColor->get_r(),startColor->get_r(), ageRatio );
            int newG = color_system->merge_channel(endColor->get_g(),startColor->get_g(), ageRatio );
            int newB = color_system->merge_channel(endColor->get_b(),startColor->get_b(), ageRatio );
            int newA = color_system->merge_channel(endColor->get_a(),startColor->get_a(), ageRatio );
            myColor->change_and_verify_rgba( newR, newG, newB, newA );
            particle_scale = start_scale * ageRatio;

            //Moves based on MS
            x_pos += x_velocity*p_speed * delta;
            y_pos += y_velocity*p_speed * delta;
        }
        else
        {
            myColor->change_and_verify_rgba( 255, 0, 0, 0  );
            particle_scale = 0;
        }
    }

    particle_emitter::particle_emitter( int x, int y, bool isLocal, int pMax  )
    {
        rotateTexture = true;
        shapeW = 16;
        shapeH = 16;
        emitXPos = emitYPos = 0;
        xRandom = 16;
        yRandom = 16;
        emitterXPos = x;
        emitterYPos = y;
        emission_disabled = false;
        emission_paused = false;
        reverse_draw = false;
        particle_texture = nullptr;

        currentParticleCount = 0;
        texBlendMode = blend_mode_blend;
        emissionRate = 0;  //per second
        emissionMSRate = 0;
        particlesPerEmission = 1;
        emissionCounter = 0;
        set_emission_rate( 15 );
        imageLocation = "";
        sortByIndex = true;
        loopEmission = true;
        loopEmission = false;
        radius = 16;
        fallBackShapeType = part_shape_circle;
        shapeType = part_shape_circle;

        localEmitter = isLocal;
        showBox = false;

        startColor = new color("startColor", 255,255,255,255 );
        startColorVar = new color("startColorVar", 255,255,255,0 );

        endColor = new color( "endColor", 0,0,0,0 );
        endColorVar = new color("endColorVar", 0,0,0,0 );


        lifeMin = 9;
        lifeMax = 2;
        angleMin = -360;
        angleMax = 360;
        speedMin = 3.f / 1000.f;
        speedMax = 6.f / 1000.f;

        maxParticles = pMax;
        particle * cParticle = nullptr;
        for( int i = 0; i < maxParticles; i++ )
        {
            cParticle = new particle();
            particles.push_back( cParticle );
            respawn_particle( cParticle );
        }
    }

    particle_emitter::~particle_emitter()
    {

    }

    void particle_emitter::burst_emit()
    {
        int particleGoalCount = std::min( maxParticles, (int)particles.size() );
        for( currentParticleCount = 0; currentParticleCount < particleGoalCount; currentParticleCount++ )
        {
            respawn_particle( particles[currentParticleCount] );
        }
    }

    bool particle_emitter::can_emit()
    {
        return ( get_remaining() > 0 );
    }

    void particle_emitter::change_texture( texture_base * newTex, int blend_mode_new )
    {
        texBlendMode = blend_mode_new;
        particle_texture = newTex;
        if( newTex!=nullptr )
        {
            if( newTex->get_width() > 512 || newTex->get_height() > 512 )
            {
                particle_texture = nullptr;
            }
        }
        int maxI = (int)particles.size();
        particle * cParticle = nullptr;

        if( particle_texture !=nullptr )
        {
            particle_texture->set_blend_mode( texBlendMode );
        }

        for( int i = 0; i < maxI; i++)
        {
            cParticle = particles[i];
            if( cParticle !=nullptr )
            {
                cParticle->particle_texture = particle_texture;
                cParticle->shapeType = fallBackShapeType;
            }
        }
    }

    void particle_emitter::clear_particles()
    {
        currentParticleCount = 0;
    }

    void particle_emitter::emit_particles()
    {
        int remainingParticles = get_remaining();
        if( remainingParticles > 0 )
        {
            int particleGoalCount = std::min( currentParticleCount+particlesPerEmission +1 , (int)particles.size() );
            particle * cParticle = nullptr;
            while( currentParticleCount < particleGoalCount )
            {
                cParticle = particles[currentParticleCount];
                respawn_particle( cParticle );
                currentParticleCount++;
            }
        }
    }

    int particle_emitter::get_count()
    {
        return currentParticleCount;
    }

    int particle_emitter::get_remaining()
    {
        return maxParticles-currentParticleCount;
    }

    void particle_emitter::process_emitter( )
    {
        if( emission_paused || emission_disabled )
        {
            return;
        }
        particle * cParticle = nullptr;
        particle * tempParticle = nullptr;
        float delta = time_keeper->get_delta_ticks();

        if( !emission_paused && !emission_disabled  )
        {
            emissionCounter+= delta;
            if( emissionCounter >= emissionMSRate && can_emit() )
            {
                emit_particles();
                emissionCounter = 0;
            }
        }
        else
        {
            emissionCounter = 0;
            error_log->report("Emission Paused...");
        }

        for( int i = 0; i <  currentParticleCount; i++ )
        {
            cParticle = particles[i];
            if( cParticle !=nullptr )
            {
                cParticle->update( delta );
                if( cParticle->particleLife <= 0.1f  )
                {
                    cParticle->die();
                    tempParticle = particles[i];
                    cParticle = particles[currentParticleCount -1];

                    particles[i] = cParticle;
                    particles[currentParticleCount  -1 ] = tempParticle;
                    currentParticleCount--;
                    if( cParticle->particleLife <= 0.1f  )
                    {
                        respawn_particle( cParticle );
                    }
                }
            }
        }


    }

    void particle_emitter::render()
    {
        if( emission_disabled || !branch_visible )
        {
            return;
        }
        //render_text( emitterXPos - cam->x, emitterYPos - cam->y, "Particle count"+ stg_ex::int_to_string(currentParticleCount)+" / "+ stg_ex::int_to_string(maxParticles),c_white,font_default,gpe::fa_left,gpe::fa_top );

        int particleCount = std::min( currentParticleCount, (int)particles.size() );

        particle * cParticle = nullptr;
        int i = 0;
        if( particle_texture!=nullptr )
        {
            particle_texture->set_blend_mode( texBlendMode);
            if( rotateTexture)
            {
                if( reverse_draw )
                {
                    for( i = particleCount -1; i >= 0; i-- )
                    {
                        cParticle = particles[i];
                        if( cParticle !=nullptr )
                        {
                            cParticle->render_particle_rotated();
                        }
                    }
                }
                else
                {
                    for( i = 0; i < particleCount; i++ )
                    {
                        cParticle = particles[i];
                        if( cParticle !=nullptr )
                        {
                            cParticle->render_particle_rotated();
                        }
                    }
                }
            }
            else
            {
                if( reverse_draw )
                {
                    for( i = particleCount -1; i >= 0; i-- )
                    {
                        cParticle = particles[i];
                        if( cParticle !=nullptr )
                        {
                            cParticle->render_particle();
                        }
                    }
                }
                else
                {
                    for( i = 0; i < particleCount; i++ )
                    {
                        cParticle = particles[i];
                        if( cParticle !=nullptr )
                        {
                            cParticle->render_particle();
                        }
                    }
                }
            }
            particle_texture->set_blend_mode( blend_mode_blend);
        }
        else if( particleCount > 0 )
        {
            //renderer_main->set_render_blend_mode(texBlendMode );
            gcanvas->set_artist_blend_mode( texBlendMode );
            if( reverse_draw )
            {
                for( i = particleCount -1; i >= 0; i-- )
                {
                    cParticle = particles[i];
                    if( cParticle !=nullptr )
                    {
                        cParticle->render_particle_shape();
                    }
                }
            }
            else
            {
                for( i = 0; i < particleCount; i++ )
                {
                    cParticle = particles[i];
                    if( cParticle !=nullptr )
                    {
                        cParticle->render_particle_shape();
                    }
                }
            }
            gcanvas->set_artist_blend_mode( blend_mode_blend );
            renderer_main->set_render_blend_mode(blend_mode_blend );
        }

        if( showBox )
        {
            //gcanvas->render_rectangle( emitterXPos - cam->x - xRandom, emitterYPos - cam->y - yRandom,  emitterXPos - cam->x + xRandom,emitterYPos  - cam->y +yRandom,c_blgray, false, 128 );

            //gcanvas->render_rectangle( emitterXPos - cam->x - xRandom, emitterYPos - cam->y- yRandom,  emitterXPos  - cam->x + xRandom,emitterYPos  - cam->y + yRandom,startColor, true, 255 );
        }
    }

    void particle_emitter::respawn_particle( particle * part )
    {
        if( part !=nullptr )
        {
            part->set_position( emitterXPos+emitXPos+semath::random_float(-xRandom, xRandom), emitterYPos+emitYPos+semath::random_float(-yRandom, yRandom) );
            part->particleLife = part->startLife = semath::random_float( lifeMin, lifeMax ) * 1000; //* 10000ms
            part->set_velocity( semath::random_float( angleMin, angleMax) , semath::random_float( speedMin, speedMax) );
            part->particle_texture = particle_texture;
            part->rotate_particle = rotateTexture;
            part->width = shapeW;
            part->height = shapeH;
            if( particle_texture!=nullptr )
            {
                part->width = particle_texture->get_width();
                part->height = particle_texture->get_height();
            }
            //Changes the color values of the particle
            int variedRColor = semath::bound_number( startColor->get_r() + semath::random( -startColorVar->get_r(), startColorVar->get_r() ) ,0,255 );
            int variedGColor = semath::bound_number( startColor->get_g() + semath::random( -startColorVar->get_g(), startColorVar->get_g() ) ,0,255 );
            int variedBColor = semath::bound_number( startColor->get_b() + semath::random( -startColorVar->get_b(), startColorVar->get_b() ) ,0,255 );
            int variedAColor = semath::bound_number( startColor->get_a() + semath::random( -startColorVar->get_a(), startColorVar->get_a() ) ,0,255 );

            part->startColor->change_rgba( variedRColor, variedGColor, variedBColor, variedAColor );
            part->myColor->change_rgba(variedRColor, variedGColor, variedBColor, variedAColor  );


            variedRColor = semath::bound_number( endColor->get_r() + semath::random( -endColorVar->get_r(), endColorVar->get_r() ) ,0, 255 );
            variedGColor = semath::bound_number( endColor->get_g() + semath::random( -endColorVar->get_g(), endColorVar->get_g() ) ,0, 255 );
            variedBColor = semath::bound_number( endColor->get_b() + semath::random( -endColorVar->get_b(), endColorVar->get_b() ) ,0, 255 );
            variedAColor = semath::bound_number( endColor->get_a() + semath::random( -endColorVar->get_a(), endColorVar->get_a() ) ,0, 255 );
            part->endColor->change_rgba(variedRColor, variedGColor, variedBColor, variedAColor  );
            part->shapeType = fallBackShapeType;
        }
    }

    void particle_emitter::reset()
    {

    }

    void particle_emitter::set_blend_mode( int newMode )
    {
        texBlendMode = newMode;
        if( particle_texture!=nullptr )
        {
            particle_texture->set_blend_mode( newMode );
        }
    }

    void particle_emitter::set_emission_rate( float emr )
    {
        float prevRate = emissionRate;
        emissionRate = std::max( 0.f, emr );
        if( emissionRate > 1000.f )
        {
            emissionMSRate = 0.f; //We will end up emitting every frame
            particlesPerEmission = emissionRate / fps_cap;
        }
        else if( emissionRate != 0.f )
        {
            emissionMSRate = 1000.f / emissionRate;
            if( emissionRate > fps_cap )
            {
                particlesPerEmission = emissionRate / fps_cap;
            }
            else
            {
                    particlesPerEmission = 1.f;
            }
        }
        if( prevRate != emissionRate )
        {
            emissionCounter = 0.f;
        }
    }

    void particle_emitter::set_fallback_shape( int newFBS )
    {
        if( newFBS >=0 && newFBS <= part_shape_linecapped )
        {
            fallBackShapeType = newFBS;
        }
        else
        {
            fallBackShapeType = 0;
        }
        particle * cParticle = nullptr;
        int maxI = (int)particles.size();
        for( int i = 0; i < maxI; i++ )
        {
            cParticle = particles[i];
            if( cParticle !=nullptr )
            {
                cParticle->shapeType = fallBackShapeType;
            }
        }
    }

    void particle_emitter::set_max_particles( int newMax, bool removeExtra  )
    {
        newMax = std::max( 0, newMax );
        newMax = std::min( partical_max_per_emitter, newMax );
        int i = 0;
        particle * cParticle = nullptr;
        int currentSize = (int)particles.size();
        if( newMax > maxParticles )
        {
            for(  i = currentSize; i < newMax; i++ )
            {
                cParticle = new particle();
                particles.push_back( cParticle );
                respawn_particle( cParticle );
            }
        }
        else if( newMax <  maxParticles )
        {
            currentParticleCount = 0;
            if( removeExtra )
            {
                for( i = currentSize -1; i >=newMax; i-- )
                {
                    cParticle = particles[i];
                    if( cParticle!=nullptr )
                    {
                        delete cParticle;
                        cParticle = nullptr;
                    }
                    particles.erase( particles.begin()+i );
                }
            }
        }
        maxParticles = newMax;
        //else do nothing its the same size
    }
}
