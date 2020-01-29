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
    GPE_Font * fontInEditor;
public:
    int fontSize;
    int fontId;
    int fontType;
    int fontWidth;
    int fontHeight;
    GPE_TextInputNumber * fontSizeField;
    GPE_TextInputBasic * fontPreviewTextField;
    std::string storedFontFileNames[FONT_FILE_TYPES];
    fontResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~fontResource();
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_css3_file(std::ofstream * fileTarget, int leftTabAmount);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool include_local_files( std::string pBuildDir , int buildType );
    void load_font(std::string newFileName, int newFontSize = 12);
    void preprocess_self(std::string alternatePath = "");
    void prerender_self( );
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    bool render_held_font( int xPos, int yPos, std::string textureText, GPE_Color * textColor = NULL,int hAlign=FA_LEFT,int vAlign=FA_TOP, float renderAngle = 0.f, float renderScale = 1, int renderAlpha = 255 );
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void save_resource(std::string alternatePath = "", int backupId = -1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif
