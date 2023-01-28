/*
gpe_audio.cpp
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

#include "gpe_audio.h"

namespace gpe
{
    audio::audio(int aId , std::string aName , std::string aFileName )
    {
        audioChunk = nullptr;
        audioMusic = nullptr;
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

    audio::~audio()
    {
        if( audioChunk!=nullptr)
        {
            Mix_FreeChunk( audioChunk );
            audioChunk = nullptr;
        }
        if( audioMusic!=nullptr)
        {
            Mix_FreeMusic(audioMusic);
            audioMusic = nullptr;
        }
    }

    void audio::audio_play( int repeatTimes  )
    {

    }

    void audio::audio_pause()
    {

    }

    void audio::audio_stop()
    {

    }

    int audio::get_length()
    {
        return audioLength;
    }

    int audio::get_position()
    {
        return currentPosition;
    }

    int audio::get_position_secs()
    {
        return currentSec;
    }

    int audio::get_position_min()
    {
        return currentMin;
    }

    int audio::get_position_hour()
    {
        return currentMin;
    }

    int audio::get_remaining_loops()
    {
        return audioLoops;
    }

    void audio::load_audio( std::string aFileName)
    {

    }

    bool audio::is_playing()
    {
        return isPlaying;
    }

}
