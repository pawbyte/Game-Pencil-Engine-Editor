/*
particle_resource.h
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



#ifndef GPE_PARTICLE_RESOURCES_H
#define GPE_PARTICLE_RESOURCES_H

#include "../gpe/gpe_particles2d.h"
#include "../basic/gpe_basic_resource_page.h"



class particleResource: public standardEditableGameResource
{
private:
    gpe::particle_emitter * myEmitter;
public:
    pawgui::widget_label_title * textureLabel;
    pawgui::widget_label_image * texturePreviewImgLabel;
    pawgui::widget_input_text * textureLocationField;
    pawgui::widget_button_icon * openExternalEditor_button;
    pawgui::widget_button_icon * refreshResourceData_button;
    pawgui::widget_button_label * clearTexture_button;
    pawgui::widget_dropdown_menu * emitterBackupTextureMenu;
    pawgui::widget_checkbox * textureIsRotated;

    pawgui::widget_input_number * particleCountField;
    pawgui::widget_input_number * emissionRateField;

    pawgui::widget_checkbox * emissionDisabled;
    pawgui::widget_checkbox * emissionPaused;
    pawgui::widget_checkbox * emissionDrawReversed;

    pawgui::widget_input_number * lifeMinField;
    pawgui::widget_input_number * lifeMaxField;

    pawgui::widget_label_title * colorsLabel;
    pawgui::gpe_widget_color_picker * sColorField;
    pawgui::gpe_widget_color_picker * sVColorField;
    pawgui::gpe_widget_color_picker * eColorField;
    pawgui::gpe_widget_color_picker * eVColorField;

    pawgui::widget_input_number * xStartPosField;
    pawgui::widget_input_number * yStartPosField;
    pawgui::widget_input_number * xRandomField;
    pawgui::widget_input_number * yRandomField;

    pawgui::widget_label_title * physicsLabel;

    pawgui::widget_input_number * directionMin;
    pawgui::widget_input_number * directionMax;
    pawgui::widget_input_number * speedMin;
    pawgui::widget_input_number * speedMax;

    pawgui::widget_input_number * gravityXValue;
    pawgui::widget_input_number * gravityYValue;

    gpe::texture_base * emitterTexture;

    pawgui::widget_dropdown_menu * blendTypeMenu;
    pawgui::widget_dropdown_menu * shapeMenu;
    pawgui::widget_input_number * shapeWField;
    pawgui::widget_input_number * shapeHField;
    pawgui::widget_checkbox * showDebugInfo;

    particleResource( pawgui::widget_resource_container * pFolder = nullptr );
    ~particleResource();

    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool include_local_files( std::string pBuildDir , int buildType );
    void handle_scrolling();
    bool is_build_ready();
    void load_image(std::string new_file_name, bool autoProcess = false);
    void load_resource(std::string file_path = "");
    void prerender_self( );
    void process_data_fields(float versionToProcess = -1);
    void process_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void render_self( gpe::shape_rect * view_space = nullptr, gpe::shape_rect * cam = nullptr);
    void revert_data_fields();
    void save_resource(std::string file_path = "", int backupId = -1);
    void update_box(int x_new=-1, int y_new=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif // GPE_PARTICLE_RESOURCES_H
