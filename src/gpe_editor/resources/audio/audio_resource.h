/*
audio_resource.h
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#ifndef GPE_AUDIO_RESOURCES_H
#define GPE_AUDIO_RESOURCES_H

#include "../../resources/basic/gpe_basic_resource_page.h"


class audioResource: public standardEditableGameResource
{
public:
    bool isPreloaded;
    int audioId;
    int audioType;
    gpe::sound_base * soundVal;
    gpe::music_base * musicVal;
    pawgui::widget_button_icon * play_button;
    pawgui::widget_button_icon * openExternalEditor_button;
    pawgui::widget_button_icon * refreshResourceData_button;
    pawgui::widget_radio_button_controller * audioType_buttonController;
    std::string audioFileName[ gpe::sound_format_max ];
    pawgui::widget_checkbox * preloadCheckBox;
    pawgui::widget_input_text * audioGroupName;

    pawgui::widget_label_text  * volumeLabel;
    pawgui::widget_input_number * defaultVolume;
    pawgui::widget_slide_xaxis * volumeSlider;
    pawgui::widget_slide_xaxis * positionSlider;
    pawgui::widget_label_text  * audioEditorMainNote;
    audioResource(pawgui::widget_resource_container * pFolder = nullptr);
    ~audioResource();
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool copy_audio_source(std::string directory_output_name);
    bool include_local_files( std::string pBuildDir , int buildType );
    bool is_build_ready();
    void load_audio(std::string new_file_name);
    void load_resource(std::string file_path = "");
    void prerender_self( );
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};



#endif
