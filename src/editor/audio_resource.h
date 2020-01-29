/*
audio_resource.h
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

#ifndef GPE_AUDIO_RESOURCES_H
#define GPE_AUDIO_RESOURCES_H

#include "gpe_basic_resource_page.h"


const int aacFileName = 0;
const int mp3FileName = 1;
const int oggFileName = 2;
const int wavFileName = 3;

const int SUPPORTED_AUDIO_FORMAT_COUNT = 4;

extern std::string SUPPORTED_AUDIO_EXT[SUPPORTED_AUDIO_FORMAT_COUNT];

class audioResource: public standardEditableGameResource
{
public:
    bool isPreloaded;
    int audioId;
    int audioType;
    Mix_Chunk * soundVal;
    Mix_Music * musicVal;
    GPE_ToolIconButton * playButton;
    GPE_ToolIconButton * openExternalEditorButton;
    GPE_ToolIconButton * refreshResourceDataButton;
    GPE_RadioButtonControllerBasic * audioTypeButtonController;
    std::string audioFileName[SUPPORTED_AUDIO_FORMAT_COUNT];
    GPE_CheckBoxBasic * preloadCheckBox;
    GPE_TextInputBasic * audioGroupName;

    GPE_Label_Text * volumeLabel;
    GPE_TextInputNumber * defaultVolume;
    GPE_Slider_XAxis * volumeSlider;
    GPE_Label_Text * audioEditorMainNote;
    audioResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~audioResource();
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool copy_audio_source(std::string outDirectoryName);
    bool include_local_files( std::string pBuildDir , int buildType );
    void load_audio(std::string newFileName);
    void preprocess_self(std::string alternatePath = "");
    void prerender_self( );
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void save_resource(std::string alternatePath = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};



#endif
