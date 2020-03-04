/*
audio_resource.h
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
#ifndef GPE_FONT_RESOURCES_H
#define GPE_FONT_RESOURCES_H

#include "gpe_basic_resource_page.h"

const int FONT_FILE_TYPES = 6;
const int FONT_EOT = 0;
const int FONT_OTF = 1;
const int FONT_SVG = 2;
const int FONT_TTF = 3;
const int FONT_WOFF = 4;
const int FONT_WOFF2 = 5;

extern std::string SUPPORTED_FONT_EXT[FONT_FILE_TYPES];

class fontResource: public standardEditableGameResource
{
private:
    GPE_RadioButtonControllerBasic * fontTypeButtonController;
    GPE_ToolIconButton * openExternalEditorButton;
    std::string fontInEditorFileName;
    std::string fontFamilyName;
    gpe::font_base * fontInEditor;
public:
    int fontSize;
    int fontId;
    int fontType;
    int fontWidth;
    int fontHeight;
    gpe_text_widget_number * fontSizeField;
    gpe_text_widget_string * fontPreviewTextField;
    std::string storedFontFileNames[FONT_FILE_TYPES];
    fontResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~fontResource();
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_css3_file(std::ofstream * fileTarget, int leftTabAmount);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool include_local_files( std::string pBuildDir , int buildType );
    bool is_build_ready();
    void load_font(std::string new_file_name, int newFontSize = 12);
    void load_resource(std::string file_path = "");
    void prerender_self( );
    void process_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    bool render_held_font( int xPos, int yPos, std::string textureText, gpe::color * textColor = NULL,int hAlign = gpe::fa_left,int vAlign = gpe::fa_top, float renderAngle = 0.f, float renderScale = 1, int renderAlpha = 255 );
    void render_self( gpe::shape_rect * viewedSpace = NULL, gpe::shape_rect * cam = NULL);
    void save_resource(std::string file_path = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
