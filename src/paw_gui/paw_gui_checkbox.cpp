/*
paw_gui_checkbox.cpp
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

#include "paw_gui_checkbox.h"


GPE_CheckBoxBasic::GPE_CheckBoxBasic(std::string name, std::string descriptor, bool already_checked )
{
    guiListTypeName = "checkbox";
    guiListTypeId = 1;
    opName = name;
    descriptionText = descriptor;

    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = clickBoxW = 24;
    widget_box.h = clickBoxH = 24;
    labelBoxW = 0;
    labelBoxH = 0;
    if( (int)opName.size()>0 && font_default!=NULL )
    {
        font_default->get_metrics(opName,&labelBoxW, &labelBoxH);
        widget_box.w+= labelBoxW;
    }
    //widget_box.w+=+GENERAL_GPE_GUI_PADDING;
    isChecked = already_checked;
}

GPE_CheckBoxBasic::~GPE_CheckBoxBasic()
{

}

std::string GPE_CheckBoxBasic::get_data()
{
    return guiListTypeName+":"+opName+"==|||=="+ stg_ex::int_to_string(isClicked);
}

bool GPE_CheckBoxBasic::is_checked()
{
    return isChecked;
}

void GPE_CheckBoxBasic::load_data(std::string dataString)
{
    isClicked = stg_ex::string_to_bool(dataString);
}


void GPE_CheckBoxBasic::prerender_self(  )
{
    //widget_box.w = clickBoxW;
    //widget_box.h = clickBoxH;
}

void GPE_CheckBoxBasic::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    isHovered = false;

    if(viewedSpace!=NULL && cam!=NULL)
    {
        GPE_GeneralGuiElement::process_self( viewedSpace, cam);
        if( windowInView == false )
        {
            return;
        }
        if( isClicked )
        {
            isChecked = !isChecked;
        }
        if( (isInUse || isHovered ) &&( gpe::input->check_kb_pressed( kb_enter ) || gpe::input->check_kb_pressed( kb_space )  ) )
        {
            isChecked = !isChecked;
        }
    }
}

void GPE_CheckBoxBasic::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( cam!=NULL)
    {
        if( isInUse )
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,theme_main->button_box_highlight_color,false);
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,theme_main->button_border_highlight_color,true);
        }
        else if( isHovered)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,theme_main->button_box_selected_color,false);
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,theme_main->button_border_highlight_color,true);
        }
        else
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,theme_main->main_border_color,false);
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+clickBoxW-cam->x,widget_box.y+clickBoxH-cam->y,theme_main->main_box_font_color,true);
        }
        if( isChecked && GPE_CHECKMARK_IMAGE!=NULL )
        {
            GPE_CHECKMARK_IMAGE->render_tex_resized( widget_box.x-cam->x,widget_box.y-cam->y,clickBoxW,clickBoxH,NULL, theme_main->checkbox_color );
        }
        gpe::gfs->render_text( widget_box.x+clickBoxW+GENERAL_GPE_GUI_PADDING-cam->x,widget_box.y+widget_box.h/2-cam->y,opName,theme_main->main_box_font_color,FONT_LABEL,gpe::fa_left,gpe::fa_middle,255);
    }
}

void GPE_CheckBoxBasic::set_checked( bool newCheckState )
{
    isChecked = newCheckState;
}

void GPE_CheckBoxBasic::set_checkbox_size(int nBoxSize, bool resizewidget_box)
{
    if( resizewidget_box)
    {
        widget_box.w = nBoxSize+labelBoxW;
    }
    labelBoxW = labelBoxH = nBoxSize;
}
