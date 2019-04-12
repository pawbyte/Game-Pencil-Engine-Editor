/*
GPE_Particles.cpp
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

#include "GPE_Engine.h"
#include "GPE_Particles.h"


GPE_Particle::GPE_Particle( )
{
    //Defaults
    rotateParticle = true;
    xPos = xStart = 0;
    yPos = yStart = 0;
    startLife = 1;
    particleLife = 0;
    startScale = particleScale = 1;
    shapeType = part_shape_circle;
    pDirection = 0;
    xVelocity = 0;
    yVelocity = 0;

    pSpeed = 0;
    pTex = NULL;
    startColor = new GPE_Color("startColor",255,255,0);
    myColor = new GPE_Color("myColor",255,255,255,0);
    endColor = new GPE_Color("endColor",0,0,0,0 );
    width = height = 32;
    pGravity = 0;
    pGravityDirection = 0;

}

GPE_Particle::~GPE_Particle()
{

}

void GPE_Particle::die()
{
    particleLife = 0;
    myColor->change_a( 0 );
}

void GPE_Particle::render_particle( GPE_Rect * cam )
{
    cam = GPE_find_camera( cam );
    if( cam!=NULL && particleLife > 0 && myColor->get_a() > 0 )
    {
        if( pTex!=NULL )
        {
            pTex->render_tex_scaled(xPos-cam->x - (width*particleScale)/2,yPos-cam->y - (height*particleScale)/2,particleScale,particleScale,NULL, myColor, myColor->get_a() );
        }
    }
}

void GPE_Particle::render_particle_shape( GPE_Rect * cam )
{
    cam = GPE_find_camera( cam );
    if( cam!=NULL && particleLife > 0 && myColor->get_a() > 0 )
    {
        if( particleScale!=0 )
        {
            switch( shapeType)
            {
                case part_shape_circle:
                    gcanvas->render_circle_color( xPos-cam->x,yPos-cam->y,particleScale * width,myColor, myColor->get_a() );
                break;

                case part_shape_rectangle:
                    gcanvas->render_rectangle( xPos-cam->x,yPos-cam->y,xPos-cam->x+particleScale*width, yPos-cam->y+ particleScale*height, myColor, false, myColor->get_a() );
                break;
                case part_shape_rectangle_outline:
                    gcanvas->render_rectangle( xPos-cam->x,yPos-cam->y,xPos-cam->x+particleScale*width, yPos-cam->y+ particleScale*height, myColor, true, myColor->get_a() );
                break;

                case part_shape_square_outline:
                //defaults to square
                    gcanvas->render_square( xPos-cam->x,yPos-cam->y,particleScale*width, myColor, true, myColor->get_a() );
                break;

                case part_shape_line:
                    gcanvas->render_line_width_color( xStart-cam->x,yStart-cam->y, xPos-cam->x, yPos- cam->y,particleScale*width, myColor, myColor->get_a() );
                break;

                case part_shape_linecapped:
                    gcanvas->render_line_capped_color( xStart-cam->x,yStart-cam->y, xPos-cam->x, yPos- cam->y,particleScale*width, myColor, myColor->get_a() );
                break;

                default:
                //defaults to square
                    gcanvas->render_square( xPos-cam->x,yPos-cam->y,particleScale*width, myColor, false, myColor->get_a() );
                break;
            }
        }
    }
}

void GPE_Particle::render_particle_rotated( GPE_Rect * cam )
{
    cam = GPE_find_camera( cam );
    if( cam!=NULL && particleLife > 0 && myColor->get_a() > 25 )
    {
        if( pTex!=NULL )
        {
            pTex->render_tex_special(xPos-cam->x,yPos-cam->y,pDirection, abs(particleScale * width),abs(particleScale * height), myColor, NULL,myColor->get_a() );
        }
    }
}

void GPE_Particle::set_position(  double x, double y )
{
    xPos = xStart = x;
    yPos = yStart = y;
}

void GPE_Particle::set_velocity(  double angle, double speed )
{
    pDirection = angle;
    double angleInRadians = pDirection * radiansMultiple;
    xVelocity = speed *cos(angleInRadians);
    yVelocity = speed * sin(angleInRadians);
    pSpeed = speed;
}

void GPE_Particle::update( int delta )
{
    particleLife -= delta;

    if( particleLife > 0 )
    {
        double ageRatio = (double)particleLife/ (double)startLife;
        if( ageRatio > 1)
        {
            GPE_Report("Invalid Age Range Found ["+ int_to_string( ageRatio)+"]" );
            ageRatio = 1;
        }
        int newR = merge_channel(endColor->get_r(),startColor->get_r(), ageRatio );
        int newG = merge_channel(endColor->get_g(),startColor->get_g(), ageRatio );
        int newB = merge_channel(endColor->get_b(),startColor->get_b(), ageRatio );
        int newA = merge_channel(endColor->get_a(),startColor->get_a(), ageRatio );
        myColor->change_and_verify_rgba( newR, newG, newB, newA );
        particleScale = startScale * ageRatio;
        //double movementDelta = FPS_CAP * delta;
        xPos += xVelocity*pSpeed/FPS_CAP; // * ceil( movementDelta/1000.0);
        yPos += yVelocity*pSpeed/FPS_CAP; // * ceil( movementDelta/1000.0);
    }
    else
    {
        myColor->change_and_verify_rgba( 255, 0, 0, 0  );
        particleScale = 0;
    }
}

GPE_ParticleEmitter::GPE_ParticleEmitter( int x, int y, bool isLocal, int pMax  )
{
    rotateTexture = true;
    shapeW = 16;
    shapeH = 16;
    emitXPos = emitYPos = 0;
    xRandom = 16;
    yRandom = 16;
    emitterXPos = x;
    emitterYPos = y;
    emissionPaused = false;
    pTex = NULL;

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

    startColor = new GPE_Color("startColor", 255,255,255,255 );
    startColorVar = new GPE_Color("startColorVar", 255,255,255,0 );

    endColor = new GPE_Color( "endColor", 0,0,0,0 );
    endColorVar = new GPE_Color("endColorVar", 0,0,0,0 );


    lifeMin = 9;
    lifeMax = 2;
    angleMin = -360;
    angleMax = 360;
    speedMin = 3;
    speedMax = 6;

    maxParticles = pMax;
    GPE_Particle * cParticle = NULL;
    for( int i = 0; i < maxParticles; i++ )
    {
        cParticle = new GPE_Particle();
        particles.push_back( cParticle );
        respawn_particle( cParticle );
    }
}

GPE_ParticleEmitter::~GPE_ParticleEmitter()
{

}

void GPE_ParticleEmitter::burst_emit()
{
    int particleGoalCount = std::min( maxParticles, (int)particles.size() );
    for( currentParticleCount = 0; currentParticleCount < particleGoalCount; currentParticleCount++ )
    {
        respawn_particle( particles[currentParticleCount] );
    }
}

bool GPE_ParticleEmitter::can_emit()
{
    return ( get_remaining() > 0 );
}

void GPE_ParticleEmitter::change_texture( GPE_Texture * newTex, int newBlendMode )
{
    texBlendMode = newBlendMode;
    pTex = newTex;
    if( newTex!=NULL )
    {
        if( newTex->get_width() > 512 || newTex->get_height() > 512 )
        {
            pTex = NULL;
        }
    }
    int maxI = (int)particles.size();
    GPE_Particle * cParticle = NULL;

    if( pTex !=NULL )
    {
        pTex->set_blend_mode( texBlendMode );
    }
    else
    {
        pTex = NULL;
        /*
        if( fallBackShapeType== part_shape_circle)
        {
            pTex = new GPE_Texture();
            pTex->prerender_circle( 16, c_white );
            pTex->set_blend_mode( texBlendMode);
        }
        else
        {
            pTex = NULL;
        }*/
    }

    for( int i = 0; i < maxI; i++)
    {
        cParticle = particles[i];
        if( cParticle !=NULL )
        {
            cParticle->pTex = pTex;
            cParticle->shapeType = fallBackShapeType;
        }
    }
}

void GPE_ParticleEmitter::clear_particles()
{
    currentParticleCount = 0;
}

void GPE_ParticleEmitter::emit_particles()
{
    int remainingParticles = get_remaining();
    if( remainingParticles > 0 )
    {
        int particleGoalCount = std::min( currentParticleCount+particlesPerEmission +1 , (int)particles.size() );
        GPE_Particle * cParticle = NULL;
        while( currentParticleCount < particleGoalCount )
        {
            cParticle = particles[currentParticleCount];
            respawn_particle( cParticle );
            currentParticleCount++;
        }
    }
}

int GPE_ParticleEmitter::get_count()
{
    return currentParticleCount;
}

int GPE_ParticleEmitter::get_remaining()
{
    return maxParticles-currentParticleCount;
}

void GPE_ParticleEmitter::process_emitter( )
{
    if( emissionPaused )
    {
        return;
    }
    GPE_Particle * cParticle = NULL;
    GPE_Particle * tempParticle = NULL;
    int delta = gpe->get_delta_time();

    bool particleIsAlive = true;

    if( !emissionPaused )
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
        GPE_Report("Emission Paused...");
    }

    for( int i = 0; i <  currentParticleCount; i++ )
    {
        particleIsAlive = true;
        cParticle = particles[i];
        if( cParticle !=NULL )
        {
            cParticle->update( delta );
            if( cParticle->particleLife <= 0  )
            {
                cParticle->die();
                tempParticle = particles[i];
                cParticle = particles[currentParticleCount -1];

                particles[i] = cParticle;
                particles[currentParticleCount  -1 ] = tempParticle;
                currentParticleCount--;
                if( cParticle->particleLife <= 0  )
                {
                    respawn_particle( cParticle );
                }
                particleIsAlive = false;
            }
        }
    }


}

void GPE_ParticleEmitter::render_emitter( GPE_Rect * cam )
{
    cam = GPE_find_camera( cam );
    //render_text( emitterXPos - cam->x, emitterYPos - cam->y, "Particle count"+int_to_string(currentParticleCount)+" / "+int_to_string(maxParticles),c_white,GPE_DEFAULT_FONT,FA_LEFT,FA_TOP );

    int particleCount = std::min( currentParticleCount, (int)particles.size() );

    GPE_Particle * cParticle = NULL;
    int i = 0;
    if( pTex!=NULL )
    {
        pTex->set_blend_mode( texBlendMode);
        if( rotateTexture)
        {
            for( i = 0; i < particleCount; i++ )
            {
                cParticle = particles[i];
                if( cParticle !=NULL )
                {
                    cParticle->render_particle_rotated( cam );
                }
            }
        }
        else
        {
            for( i = 0; i < particleCount; i++ )
            {
                cParticle = particles[i];
                if( cParticle !=NULL )
                {
                    cParticle->render_particle( cam );
                }
            }
        }
        pTex->set_blend_mode( blend_mode_blend);
    }
    else if( particleCount > 0 )
    {
        CURRENT_RENDERER->set_render_blend_mode(texBlendMode );
        gcanvas->set_artist_blend_mode( texBlendMode );
        for( i = 0; i < particleCount; i++ )
        {
            cParticle = particles[i];
            if( cParticle !=NULL )
            {
                cParticle->render_particle_shape( cam );
            }
        }
        gcanvas->set_artist_blend_mode( blend_mode_blend );
        CURRENT_RENDERER->set_render_blend_mode(blend_mode_blend );

    }

    if( showBox )
    {
        gcanvas->render_rectangle( emitterXPos - cam->x - xRandom, emitterYPos - cam->y - yRandom,  emitterXPos - cam->x + xRandom,emitterYPos  - cam->y +yRandom,c_blgray, false, 128 );

        gcanvas->render_rectangle( emitterXPos - cam->x - xRandom, emitterYPos - cam->y- yRandom,  emitterXPos  - cam->x + xRandom,emitterYPos  - cam->y + yRandom,startColor, true, 255 );

    }
}

void GPE_ParticleEmitter::respawn_particle( GPE_Particle * part )
{
    if( part !=NULL )
    {
        part->set_position( emitterXPos+emitXPos+random(-xRandom, xRandom), emitterYPos+emitYPos+random(-yRandom, yRandom) );
        part->particleLife = part->startLife = random( lifeMin, lifeMax ) * 1000; //* 10000ms
        part->set_velocity( random( angleMin, angleMax) , random_double( speedMin, speedMax) );
        part->pTex = pTex;
        part->rotateParticle = rotateTexture;
        part->width = shapeW;
        part->height = shapeH;
        if( pTex!=NULL )
        {
            part->width = pTex->get_width();
            part->height = pTex->get_height();
        }
        //Changes the color values of the particle
        int variedRColor = bound_number( startColor->get_r() + random( -startColorVar->get_r(), startColorVar->get_r() ) ,0,255 );
        int variedGColor = bound_number( startColor->get_g() + random( -startColorVar->get_g(), startColorVar->get_g() ) ,0,255 );
        int variedBColor = bound_number( startColor->get_b() + random( -startColorVar->get_b(), startColorVar->get_b() ) ,0,255 );
        int variedAColor = bound_number( startColor->get_a() + random( -startColorVar->get_a(), startColorVar->get_a() ) ,0,255 );

        part->startColor->change_rgba( variedRColor, variedGColor, variedBColor, variedAColor );
        part->myColor->change_rgba(variedRColor, variedGColor, variedBColor, variedAColor  );


        variedRColor = bound_number( endColor->get_r() + random( -endColorVar->get_r(), endColorVar->get_r() ) ,0, 255 );
        variedGColor = bound_number( endColor->get_g() + random( -endColorVar->get_g(), endColorVar->get_g() ) ,0, 255 );
        variedBColor = bound_number( endColor->get_b() + random( -endColorVar->get_b(), endColorVar->get_b() ) ,0, 255 );
        variedAColor = bound_number( endColor->get_a() + random( -endColorVar->get_a(), endColorVar->get_a() ) ,0, 255 );
        part->endColor->change_rgba(variedRColor, variedGColor, variedBColor, variedAColor  );
        part->shapeType = fallBackShapeType;
    }
}

void GPE_ParticleEmitter::reset()
{

}

void GPE_ParticleEmitter::set_blend_mode( int newMode )
{
    texBlendMode = newMode;
    if( pTex!=NULL )
    {
        pTex->set_blend_mode( newMode );
    }
}

void GPE_ParticleEmitter::set_emission_rate( int emr )
{
    int prevRate = emissionRate;
    emissionRate = std::max( 0,emr );
    if( emissionRate > 1000 )
    {
        emissionMSRate = 0; //We will end up emitting every frame
        particlesPerEmission = emissionRate / FPS_CAP;
    }
    else if( emissionRate != 0 )
    {
        emissionMSRate = 1000 / emissionRate;
        if( emissionRate > FPS_CAP )
        {
            particlesPerEmission = emissionRate / FPS_CAP;
        }
        else
        {
                particlesPerEmission = 1;
        }
    }
    if( prevRate != emissionRate )
    {
        emissionCounter = 0;
    }
}

void GPE_ParticleEmitter::set_fallback_shape( int newFBS )
{
    if( newFBS >=0 && newFBS <= part_shape_linecapped )
    {
        fallBackShapeType = newFBS;
    }
    else
    {
        fallBackShapeType = 0;
    }
    GPE_Particle * cParticle = NULL;
    int maxI = (int)particles.size();
    for( int i = 0; i < maxI; i++ )
    {
        cParticle = particles[i];
        if( cParticle !=NULL )
        {
            cParticle->shapeType = fallBackShapeType;
        }
    }
}

void GPE_ParticleEmitter::set_max_particles( int newMax, bool removeExtra  )
{
    newMax = std::max( 0, newMax );
    newMax = std::min( PARTICLE_MAX_PER_EMITTER, newMax );
    int i = 0;
    GPE_Particle * cParticle = NULL;
    int currentSize = (int)particles.size();
    if( newMax > maxParticles )
    {
        for(  i = currentSize; i < newMax; i++ )
        {
            cParticle = new GPE_Particle();
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
                if( cParticle!=NULL )
                {
                    delete cParticle;
                    cParticle = NULL;
                }
                particles.erase( particles.begin()+i );
            }
        }
    }
    maxParticles = newMax;
    //else do nothing its the same size
}
