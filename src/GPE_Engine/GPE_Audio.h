/*
GPE_Audio.h
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

#ifndef GPE_AUDIO_H
#define GPE_AUDIO_H

#include <string>
#include <cmath>
#include <sstream>
#include "GPE_Functions.h"
#include "GPE_String_Ex.h"
#include "GPE_Globals.h"

class GPE_Audio
{
    private:
        Mix_Chunk * audioChunk;
        Mix_Music * audioMusic;
        int audioLoops;
        bool isPlaying;
        int currentHour;
        int currentMin;
        int currentSec;
        int currentPosition;
        int audioLength;
        int audioId;
        std::string audioName;
        std::string audioFileName;
    public:
        GPE_Audio(int aId = -1, std::string aName = "", std::string aFileName = "" );
        ~GPE_Audio();
        void audio_play( int repeatTimes = 0 );
        void audio_pause();
        void audio_stop();
        int get_id();
        int get_length();
        int get_position();
        int get_position_secs();
        int get_position_min();
        int get_position_hour();
        std::string get_file_name();
        std::string get_name();
        int get_remaining_loops();
        bool is_playing();
        void load_audio( std::string aFileName );
};

#endif
