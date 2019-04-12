/*
paw_gui_checkbox.cpp
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

#include "paw_gui_checkbox.h"


GPE_CheckBoxBasic::GPE_CheckBoxBasic(std::string name, std::string descriptor, bool isChecked )
{
    guiListTypeName = "checkbox";
    guiListTypeId = 1;
    opName = name;
    descriptionText = descriptor;

    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = clickBoxW = 16;
    elementBox.h = clickBoxH = 16;
    labelBoxW = 0;
    labelBoxH = 0;
    if( (int)opName.size()>0 && GPE_DEFAULT_FONT!=NULL )
    {
        GPE_DEFAULT_FONT->get_metrics(opName,&labelBoxW, &labelBoxH);
        elementBox.w+= labelBoxW;
    }
    //elementBox.w+=+GENERAL_GPE_PADDING;
    isClicked = isChecked;
}

GPE_CheckBoxBasic::~GPE_CheckBoxBasic()
{

}

std::string GPE_CheckBoxBasic::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+int_to_string(isClicked);
}

void GPE_CheckBoxBasic::load_data(std::string dataString)
{
    isClicked = string_to_bool(dataString);
}


void GPE_CheckBoxBasic::prerender_self(  )
{
    //elementBox.w = clickBoxW;
    //elementBox.h = clickBoxH;
}

void GPE_CheckBoxBasic::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    isHovered = false;
    if(viewedSpace!=NULL && cam!=NULL)
    {
        bool isChecked = isClicked;
        GPE_GeneralGuiElement::process_self( viewedSpace, cam);
        if( isClicked )
        {
            isClicked = !isChecked;
        }
        else
        {
            isClicked = isChecked;
        }
        if( (isInUse || isHovered ) &&( input->check_keyboard_pressed( kb_enter ) || input->check_keyboard_pressed( kb_space )  ) )
        {
            isClicked = !isChecked;
        }
    }
}

void GPE_CheckBoxBasic::render_self( GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && forceRedraw )
    {
        if( isInUse )
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Button_Box_Highlighted_Color,false);
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
        }
        else if( isHovered)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Button_Box_Selected_Color,false);
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Main_Border_Color,false);
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+clickBoxW-cam->x,elementBox.y+clickBoxH-cam->y,GPE_MAIN_THEME->Main_Box_Font_Color,true);
        }
        if( isClicked && GPE_CHECKMARK_IMAGE!=NULL )
        {
            GPE_CHECKMARK_IMAGE->render_tex_resized( elementBox.x-cam->x,elementBox.y-cam->y,clickBoxW,clickBoxH,NULL, GPE_MAIN_THEME->Checkbox_Color );
        }
        gfs->render_text( elementBox.x+clickBoxW+GENERAL_GPE_PADDING-cam->x,elementBox.y+elementBox.h-cam->y,opName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_CHECKBOX,FA_LEFT,FA_BOTTOM,255);
    }
}

void GPE_CheckBoxBasic::set_checkbox_size(int nBoxSize, bool resizeelementBox)
{
    if( resizeelementBox)
    {
        elementBox.w = nBoxSize+labelBoxW;
    }
    labelBoxW = labelBoxH = nBoxSize;
}
