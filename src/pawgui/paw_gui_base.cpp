/*
paw_gui_base.cpp
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

#include "paw_gui_base.h"

GPE_GUI_Settings * main_GUI_SETTINGS = NULL;
gpe::texture_base * GPE_CHECKMARK_IMAGE = NULL;
gpe::texture_base  * GPE_DROPDOWN_ARROW = NULL;
gpe::texture_base  * GPE_EYE_DROPPER = NULL;

int POPUP_FONT_SIZE_WIDTH = 12;
int POPUP_FONT_SIZE_HEIGHT = 12;
int POPUP_MENU_VALUE = -1;

GPE_GUI_Settings::GPE_GUI_Settings()
{
    defaultElementWidth = 192;
    defaultElementHeight = 24;
    useShortProjectNames = false;
    textAreaFindBox.x = 0;
    textAreaFindBox.y = 0;
    textAreaFindBox.w = 32;
    textAreaFindBox.h = 32;
    autoFindMouseTabs = true;

    textAreaDelayTime = 50;
    textInputDelayTime = 50;
    normalInputDelayTime = 100;
    cursorBlinkTime = 300;

    tabSpaceCount = 4;

    defaultLineCountWidth = 48;
    showTextEditorLineCount = true;
    showTextEditorSyntaxHightlight = true;


    fileOpenAudioDir = "";
    fileOpenanimationDir = "";
    fileOpenTextureDir = "";
    fileOpenTilesheetDir = "";
    fileOpenFontDir = "";
    fileOpenFunctionDir = "";
    fileOpenTextFileDir = "";
    fileOpenImportFileDir = "";
    fileSaveExportFileDir = "";
    fileSaveTextFileDir = "";
    fileOpenDefaultDir = "";
    fileSaveDefaultDir = "";
}

GPE_GUI_Settings::~GPE_GUI_Settings()
{


}


GPE_GeneralGuiElement::GPE_GeneralGuiElement()
{
    renderPackageName = "";
    hAlign = gpe::fa_left;
    vAlign = gpe::fa_top;
    dynamicId = -1;
    needsNewLine = false;
    isFullWidth = false;
    indentationLevel = 0;
    widget_box.x =0;
    widget_box.y = 0;
    widget_box.w = 0;
    widget_box.h = 0;
    maxWidth = 0;
    maxHeight = 0;
    autoResizes = true;
    clickedOutside = false;
    clickedOutside = false;
    hasScrollControl = false;
    hasArrowkeyControl = false;
    isClicked = false;
    isPressed = false;
    isRightClicked = false;
    isEnabled = true;
    isHovered = false;
    guiListTypeName = "";
    guiListTypeId = 0;
    opName = "";
    descriptionText = "";
    showBorderBox = true;
    hasLineBreak = false;
    isModified = false;
    isInUse = false;
}

GPE_GeneralGuiElement::~GPE_GeneralGuiElement()
{

}

bool GPE_GeneralGuiElement::is_modified()
{
    return isModified;
}

bool GPE_GeneralGuiElement::is_inuse()
{
    return isInUse;
}

void GPE_GeneralGuiElement::switch_inuse(bool newUse)
{
    isInUse = newUse;
    if( newUse == false )
    {
        hasScrollControl = false;
        hasArrowkeyControl = false;
    }
}

void GPE_GeneralGuiElement::update_box( float nx, float ny, float nw, float nh)
{
    set_coords( nx, ny);
    set_width( nw);
    set_height( nh );
}

void GPE_GeneralGuiElement::update_paragraph( int foundMaxWidth )
{

}

void GPE_GeneralGuiElement::disable_self()
{
    isEnabled = false;
}

void GPE_GeneralGuiElement::enable_self()
{
    isEnabled = true;
}

int GPE_GeneralGuiElement::get_height()
{
    return widget_box.h;
}

int GPE_GeneralGuiElement::get_max_height()
{
    return maxHeight;
}

int GPE_GeneralGuiElement::get_max_width()
{
    return maxWidth;
}

std::string GPE_GeneralGuiElement::get_name()
{
    return opName;
}

std::string GPE_GeneralGuiElement::get_element_type()
{
    return guiListTypeName;
}

std::string GPE_GeneralGuiElement::get_data()
{
    return "";
}

std::string GPE_GeneralGuiElement::get_plain_string()
{
    return "\""+opName+"\"";
}

void GPE_GeneralGuiElement::load_data(std::string dataString)
{
    dataString = dataString;
}

void GPE_GeneralGuiElement::remove_data(std::string dataString)
{
    dataString = dataString;
}

int GPE_GeneralGuiElement::get_width()
{
    return widget_box.w;
}

int GPE_GeneralGuiElement::get_xpos()
{
    return widget_box.x;

}

int GPE_GeneralGuiElement::get_x2pos()
{
    return widget_box.x+widget_box.w;
}

int GPE_GeneralGuiElement::get_ypos()
{
    return widget_box.y;
}

int GPE_GeneralGuiElement::get_y2pos()
{
    return widget_box.y+widget_box.h;
}

bool GPE_GeneralGuiElement::is_clicked()
{
    return isClicked;
}

bool GPE_GeneralGuiElement::is_rightclicked()
{
    return isRightClicked;
}


bool GPE_GeneralGuiElement::is_enabled()
{
    return isEnabled;
}

bool GPE_GeneralGuiElement::is_full_width()
{
    return isFullWidth;
}

bool GPE_GeneralGuiElement::is_hovered()
{
    return isHovered;
}

void GPE_GeneralGuiElement::move_pos(int newX, int newY)
{
    if( newX!=0)
    {
        widget_box.x += newX;
    }
    if(newY!=0)
    {
        widget_box.y += newY;
    }
}

void GPE_GeneralGuiElement::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam)
{
    bool wasHovered = isHovered;
    windowInView = false;
    isClicked = false;
    isPressed = false;
    isRightClicked = false;
    isHovered = false;
    clickedOutside = false;
    if( !isEnabled)
    {
        return;
    }
    if( isFullWidth )
    {
        needsNewLine = true;
    }

    gpe::render_package * guiRenderPackage = gpe::rph->get_render_package_from_name( renderPackageName );
    if( guiRenderPackage !=NULL && guiRenderPackage->packageWindow!=NULL )
    {
        windowInView = guiRenderPackage->packageWindow->has_focus();
    }

    if( !windowInView )
    {
        return;
    }

    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if(viewedSpace!=NULL && cam!=NULL)
    {
        if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,viewedSpace->x,viewedSpace->y,viewedSpace->x+viewedSpace->w,viewedSpace->y+viewedSpace->h) )
        {
            if ( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x+viewedSpace->x-cam->x,widget_box.y+viewedSpace->y-cam->y,widget_box.x+widget_box.w+viewedSpace->x-cam->x,widget_box.y+widget_box.h+viewedSpace->y-cam->y) )
            {
                isHovered = true;
                if( main_OVERLAY!=NULL)
                {
                    if( (int)descriptionText.size()>0 )
                    {
                        main_OVERLAY->update_tooltip(descriptionText);
                    }
                    else
                    {
                        main_OVERLAY->update_tooltip(opName);
                    }
                }
                if( gpe::input->check_mouse_pressed(0) )
                {
                    isPressed = true;
                    isInUse = true;
                }
                else if( gpe::input->check_mouse_released( mb_left))
                {
                    isClicked = true;
                    isInUse = true;
                }
                else if( gpe::input->check_mouse_pressed(1) )
                {
                    isRightClicked = true;
                    isInUse = true;
                }
            }
            else if( gpe::input->check_mouse_pressed( mb_anybutton ) || gpe::input->check_mouse_released( mb_anybutton ) )
            {
                clickedOutside = true;
                isInUse = false;
                hasScrollControl = false;
                hasArrowkeyControl = false;
            }
        }
        else if( gpe::input->check_mouse_pressed( mb_anybutton )  || gpe::input->check_mouse_released(  mb_anybutton ) )
        {
            clickedOutside = true;
            isInUse = false;
            hasScrollControl = false;
            hasArrowkeyControl = false;
        }
    }
}

void GPE_GeneralGuiElement::prerender_self( )
{

}

void GPE_GeneralGuiElement::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{

}

bool GPE_GeneralGuiElement::requires_newline()
{
    return needsNewLine;
}

void GPE_GeneralGuiElement::scale_height(float scaleH)
{
    set_height( widget_box.h * scaleH);
}

void GPE_GeneralGuiElement::scale_width(float scaleW)
{
    set_width( widget_box.w* scaleW);
}

void GPE_GeneralGuiElement::scale_object(float scaleValue )
{
    scale_width( scaleValue);
    scale_height( scaleValue);
}

void GPE_GeneralGuiElement::set_full_width( bool makeFullWidth )
{
    if( makeFullWidth)
    {
        needsNewLine = true;
    }
    isFullWidth = makeFullWidth;
}

void GPE_GeneralGuiElement::set_name(std::string new_name)
{
    opName = new_name;
}

void GPE_GeneralGuiElement::set_coords(int newX, int newY)
{
    if( newX!=-1)
    {
        widget_box.x = newX;
    }
    if(newY!=-1)
    {
        widget_box.y = newY;
    }
}

void GPE_GeneralGuiElement::set_max_width( int nMW)
{
    maxWidth = nMW;
    if( maxWidth > 0 && widget_box.w > maxWidth)
    {
        widget_box.w = maxWidth;
    }
}

void GPE_GeneralGuiElement::set_max_height( int nMH)
{
    maxHeight = nMH;
    if( maxHeight > 0 && widget_box.h > maxHeight)
    {
        widget_box.h = maxHeight;
    }
}

void GPE_GeneralGuiElement::set_clicked( bool clickedVal)
{
    isClicked = clickedVal;
}


void GPE_GeneralGuiElement::set_rightclicked( bool clickedVal)
{
    isRightClicked = clickedVal;
}

void GPE_GeneralGuiElement::set_height(int newHeight)
{
    widget_box.h = newHeight;
    if(  newHeight < -1 )
    {
        widget_box.h = -1;
    }
    widget_box.set_h( newHeight);
    /*if(widget_box.y+widget_box.h > gpe::screen_height)
    {
        widget_box.h = gpe::screen_height-widget_box.y;
    }*/
}

void GPE_GeneralGuiElement::set_hovered(bool hoverVal)
{
    isHovered = hoverVal;
}

void GPE_GeneralGuiElement::set_width(int newWid )
{
    widget_box.w = newWid;
    if(  newWid < -1 )
    {
        widget_box.w = -1;
    }
    widget_box.set_w( newWid);
}

void update_action_message(std::string newMessage)
{
    GPE_Action_Message = newMessage;
}

void update_popup_info(std::string newMessage, int selectedOpId, bool popIsContext)
{
    GPE_Action_Message = newMessage;
    GPE_Action_ID = selectedOpId;
    GPE_ACTION_IS_CONTEXT = popIsContext;
}
