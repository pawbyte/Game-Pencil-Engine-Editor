/*
light_resource.h
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

#ifndef GPE_LIGHT_RESOURCE_H
#define GPE_LIGHT_RESOURCE_H


#include "GPE_Engine/GPE_Lights.h"
#include "gpe_basic_resource_page.h"


class lightResource: public standardEditableGameResource
{
private:
    GPE_BasicLight * myAmbientLight;
    GPE_Directionight * myDirectionLight;
    GPE_PointLight * myPointLight;
    double areaMouseXPos, areaMouseYPos;
public:
    int tempAngleExtra;
    GPE_RadioButtonControllerBasic * lightType;
    GPE_Input_Field_Color * lightColorField;
    GPE_TextInputNumber * lightIntensityField;

    //Point Light Related Variables [ BEGIN ]
    GPE_TextInputNumber * lightRadiusField;
    GPE_TextInputNumber * lightSecondRadiusField;
    GPE_TextInputNumber * lightThirdRadiusField;
    GPE_TextInputNumber * lightSecondIntensityField;
    GPE_TextInputNumber * lightThirdIntensityField;
    //Point Light Related Variables [ END ]

    //Direction Light Related Variables [ END ]
    GPE_TextInputNumber * lightLengthField;
    GPE_TextInputNumber * lightWidthField;
    GPE_TextInputNumber * lightDirectionField;
     //Texture related variables
    GPE_Texture * directionLightTexture;
    GPE_Label_Text * textureLabel;
    GPE_Label_Image * texturePreviewImgLabel;
    GPE_TextInputBasic * textureLocationField;
    GPE_ToolLabelButton * browseTextureButton;
    GPE_ToolLabelButton * clearTextureButton;
    GPE_CheckBoxBasic * autorotateAngle;
    //Direction Light Related Variables [ END ]


    //Light Flickering variables
    GPE_CheckBoxBasic * lightUseFlicker;
    GPE_TextInputNumber * lightFlickerTimeField;
    GPE_TextInputNumber * lightFlickerAmountField;

    lightResource( GPE_GeneralResourceContainer * pFolder = NULL );
    ~lightResource();

    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void handle_scrolling();
    void load_image(std::string newFileName, bool autoProcess = false);
    void preprocess_self(std::string alternatePath = "");
    void prerender_self( );
    void process_data_fields(double versionToProcess = -1);
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void revert_data_fields();
    void save_resource(std::string alternatePath = "", int backupId = -1);
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif // GPE_LIGHT_RESOURCE_H

