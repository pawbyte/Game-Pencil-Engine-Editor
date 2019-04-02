/*
animation_resource.h
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

#ifndef GPE_ANIMATION_RESOURCES_H
#define GPE_ANIMATION_RESOURCES_H

#include "gpe_basic_resource_page.h"

const double ZOOM_LEVEL_MIN = 0.0625;
const double ZOOM_LEVEL_MAX = 16;

const int SPRITE_DATA_FIELD_COUNT = 7;

extern std::string spriteDataLabels[SPRITE_DATA_FIELD_COUNT];
extern int spriteDataLabelWidth;


const double GPE_ANIM_FCOUNT_VERSION = 1.04;

class animationResource: public standardEditableGameResource
{
public:
    bool areaIsScrollable;
    double animationSpeed;
    bool isPreloaded;
    double editor0ZoomValue;
    double editor1ZoomValue;
    double minZoomValue;
    double maxZoomValue;
    int autoAnimationPos;
    GPE_TabBar * animationModeToolBar;
    GPE_Slider_XAxis * animationTrackBar;
    GPE_Animation * animInEditor;

    //
    GPE_Rect animCameraRect;
    GPE_Rect animRect;

    GPE_ScrollBar_XAxis * animXScroll;
    GPE_ScrollBar_YAxis * animYScroll;
    //Right pane
    GPE_Rect  * spritePreviewCam;
    GPE_GuiElementList * topPanel;
    GPE_Label_Text * animationSpeedLabel;
    GPE_Slider_XAxis * animationSpeedSlider;
    GPE_CheckBoxBasic * previewZoomLevel;
    GPE_CheckBoxBasic * previewSubImageNumbers;
    //Left pane
    GPE_Label_Text * labelSpriteDimensions;
    GPE_Label_Text * labelFrameInfo;
    GPE_Label_Text * labelSpriteMessage;
    GPE_ToolIconButton * editResourceButton;
    GPE_ToolIconButton * transformResourceButton;
    GPE_ToolIconButton * playPauseResourceButton;
    GPE_ToolIconButton * openExternalEditorButton;
    GPE_ToolIconButton * refreshResourceDataButton;
    GPE_CheckBoxBasic * preloadCheckBox;
    GPE_CheckBoxBasic * showCollisionShapeCheckBox;
    GPE_CheckBoxBasic * showAnimationCheckBox;

    GPE_TextInputNumber * spriteDataFields[SPRITE_DATA_FIELD_COUNT];

    GPE_TextInputNumber * spriteCollisionRectX;
    GPE_TextInputNumber * spriteCollisionRectY;
    GPE_TextInputNumber * spriteCollisionRectW;
    GPE_TextInputNumber * spriteCollisionRectH;

    GPE_TextInputNumber * spriteCollisionCircleX;
    GPE_TextInputNumber * spriteCollisionCircleY;
    GPE_TextInputNumber * spriteCollisionCircleR;

    GPE_ToolLabelButton * spriteCenterCollisionButton;

    GPE_DropDown_Menu * spriteCollisionShapeMenu;

    GPE_ToolIconButton * previousSubImageButton;
    GPE_ToolIconButton * nextSubImageButton;
    GPE_TextInputNumber * subImageNumberField;
    GPE_TextInputNumber * subImageEditorPreviewId;
    int subImagePreviewNumber;
    double subImageMiniAnimationNumber;
    GPE_Label_Text * labelInfoMaxTextureSize;

    animationResource(GPE_GeneralResourceContainer * pFolder = NULL);
    ~animationResource();
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    bool get_mouse_coords(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    int get_preview_frame();
    void handle_scrolling();
    void load_image(std::string newFileName, bool autoProcess = false);
    void preprocess_self(std::string alternatePath = "");
    void prerender_self( );
    void process_collision_box();
    void process_data_fields(double versionToProcess = -1);
    void process_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL);
    void render_self(GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void revert_data_fields();
    void save_resource(std::string alternatePath = "", int backupId = -1);
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};


#endif // GPE_ANIMATION_RESOURCES_H
