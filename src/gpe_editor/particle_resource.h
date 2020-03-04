/*
particle_resource.h
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



#ifndef GPE_PARTICLE_RESOURCES_H
#define GPE_PARTICLE_RESOURCES_H

#include "../gpe/gpe_particles2d.h"
#include "gpe_basic_resource_page.h"


class particleResource: public standardEditableGameResource
{
private:
    gpe::particle_emitter * myEmitter;
public:
    GPE_Label_Title * textureLabel;
    GPE_Label_Image * texturePreviewImgLabel;
    gpe_text_widget_string * textureLocationField;
    GPE_ToolIconButton * transformResourceButton;
    GPE_ToolIconButton * openExternalEditorButton;
    GPE_ToolIconButton * refreshResourceDataButton;
    GPE_ToolLabelButton * clearTextureButton;
    GPE_DropDown_Menu * emitterBackupTextureMenu;
    GPE_CheckBoxBasic * textureIsRotated;

    gpe_text_widget_number * particleCountField;
    gpe_text_widget_number * emissionRateField;

    gpe_text_widget_number * lifeMinField;
    gpe_text_widget_number * lifeMaxField;

    GPE_Label_Title * colorsLabel;
    gpe_widget_color_picker * sColorField;
    gpe_widget_color_picker * sVColorField;
    gpe_widget_color_picker * eColorField;
    gpe_widget_color_picker * eVColorField;

    gpe_text_widget_number * xStartPosField;
    gpe_text_widget_number * yStartPosField;
    gpe_text_widget_number * xRandomField;
    gpe_text_widget_number * yRandomField;

    GPE_Label_Title * physicsLabel;

    gpe_text_widget_number * directionMin;
    gpe_text_widget_number * directionMax;
    gpe_text_widget_number * speedMin;
    gpe_text_widget_number * speedMax;

    gpe_text_widget_number * gravityXValue;
    gpe_text_widget_number * gravityYValue;

    gpe::texture_base * emitterTexture;

    GPE_DropDown_Menu * blendTypeMenu;
    GPE_DropDown_Menu * shapeMenu;
    gpe_text_widget_number * shapeWField;
    gpe_text_widget_number * shapeHField;
    GPE_CheckBoxBasic * showDebugInfo;

    particleResource( GPE_GeneralResourceContainer * pFolder = NULL );
    ~particleResource();

    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool get_mouse_coords( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    bool include_local_files( std::string pBuildDir , int buildType );
    void handle_scrolling();
    bool is_build_ready();
    void load_image(std::string new_file_name, bool autoProcess = false);
    void load_resource(std::string file_path = "");
    void prerender_self( );
    void process_data_fields(float versionToProcess = -1);
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void revert_data_fields();
    void save_resource(std::string file_path = "", int backupId = -1);
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif // GPE_PARTICLE_RESOURCES_H
