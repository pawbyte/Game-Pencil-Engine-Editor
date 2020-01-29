/*
GPE_Audio.cpp
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

#include "GPE_Audio.h"

GPE_Audio::GPE_Audio(int aId , std::string aName , std::string aFileName )
{
    audioChunk = NULL;
    audioMusic = NULL;
    audioId = aId;
    audioName = aName;
    audioFileName = aFileName;

    audioLoops = 0;
    isPlaying = false;
    currentHour = 0;
    currentMin = 0;
    currentSec = 0;
    currentPosition = 0;
    audioLength = 0;
    load_audio( audioFileName);
}

GPE_Audio::~GPE_Audio()
{
    if( audioChunk!=NULL)
    {
        Mix_FreeChunk( audioChunk );
        audioChunk = NULL;
    }
    if( audioMusic!=NULL)
    {
        Mix_FreeMusic(audioMusic);
        audioMusic = NULL;
    }
}

void GPE_Audio::audio_play( int repeatTimes  )
{

}

void GPE_Audio::audio_pause()
{

}

void GPE_Audio::audio_stop()
{

}

int GPE_Audio::get_length()
{
    return audioLength;
}

int GPE_Audio::get_position()
{
    return currentPosition;
}

int GPE_Audio::get_position_secs()
{
    return currentSec;
}

int GPE_Audio::get_position_min()
{
    return currentMin;
}

int GPE_Audio::get_position_hour()
{
    return currentMin;
}

int GPE_Audio::get_remaining_loops()
{
    return audioLoops;
}

void GPE_Audio::load_audio( std::string aFileName)
{

}

bool GPE_Audio::is_playing()
{
    return isPlaying;
}

