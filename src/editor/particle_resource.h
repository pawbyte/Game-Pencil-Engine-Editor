/*
particle_resource.h
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



#ifndef GPE_PARTICLE_RESOURCES_H
#define GPE_PARTICLE_RESOURCES_H

#include "GPE/GPE_Particles.h"
#include "gpe_basic_resource_page.h"


class particleResource: public standardEditableGameResource
{
private:
    GPE_ParticleEmitter * myEmitter;
public:
    GPE_Label_Title * textureLabel;
    GPE_Label_Image * texturePreviewImgLabel;
    GPE_TextInputBasic * textureLocationField;
    GPE_ToolIconButton * transformResourceButton;
    GPE_ToolIconButton * openExternalEditorButton;
    GPE_ToolIconButton * refreshResourceDataButton;
    GPE_ToolLabelButton * clearTextureButton;
    GPE_DropDown_Menu * emitterBackupTextureMenu;
    GPE_CheckBoxBasic * textureIsRotated;

    GPE_TextInputNumber * particleCountField;
    GPE_TextInputNumber * emissionRateField;

    GPE_TextInputNumber * lifeMinField;
    GPE_TextInputNumber * lifeMaxField;

    GPE_Label_Title * colorsLabel;
    GPE_Input_Field_Color * sColorField;
    GPE_Input_Field_Color * sVColorField;
    GPE_Input_Field_Color * eColorField;
    GPE_Input_Field_Color * eVColorField;

    GPE_TextInputNumber * xStartPosField;
    GPE_TextInputNumber * yStartPosField;
    GPE_TextInputNumber * xRandomField;
    GPE_TextInputNumber * yRandomField;

    GPE_Label_Title * physicsLabel;

    GPE_TextInputNumber * directionMin;
    GPE_TextInputNumber * directionMax;
    GPE_TextInputNumber * speedMin;
    GPE_TextInputNumber * speedMax;

    GPE_TextInputNumber * gravityXValue;
    GPE_TextInputNumber * gravityYValue;

    GPE_Texture_Base * emitterTexture;

    GPE_DropDown_Menu * blendTypeMenu;
    GPE_DropDown_Menu * shapeMenu;
    GPE_TextInputNumber * shapeWField;
    GPE_TextInputNumber * shapeHField;
    GPE_CheckBoxBasic * showDebugInfo;

    particleResource( GPE_GeneralResourceContainer * pFolder = NULL );
    ~particleResource();

    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    bool include_local_files( std::string pBuildDir , int buildType );
    void handle_scrolling();
    void load_image(std::string newFileName, bool autoProcess = false);
    void preprocess_self(std::string alternatePath = "");
    void prerender_self( );
    void process_data_fields(float versionToProcess = -1);
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void revert_data_fields();
    void save_resource(std::string alternatePath = "", int backupId = -1);
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif // GPE_PARTICLE_RESOURCES_H
