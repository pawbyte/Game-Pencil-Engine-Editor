/*
paw_gui_buttons.cpp
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

#include "paw_gui_buttons.h"


GPE_ToolIconButton::GPE_ToolIconButton( std::string buttonImgFile,std::string name, int id, int buttonSize, bool lastInCol)
{
    autoResizes = false;
    webUrl = "";
    wasClicked = false;
    guiListTypeName = "iconbutton";
    endsSection = lastInCol;
    opName = name;
    descriptionText = name;
    opId = id;
    buttonTexture = paw_gui_rsm->texture_add( buttonImgFile,buttonImgFile );
    if( buttonTexture == NULL)
    {
        gpe::error_log->report("Unable to add texture with img ["+buttonImgFile+"]..." );
    }
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = buttonSize;
    widget_box.h = buttonSize;
    isTabbed = false;
    usesTabs = false;
}

GPE_ToolIconButton::~GPE_ToolIconButton()
{

}

void GPE_ToolIconButton::change_texture( std::string file_name )
{
    buttonTexture = paw_gui_rsm->texture_add_filename( file_name );
}

std::string GPE_ToolIconButton::get_data()
{
    if( buttonTexture!=NULL)
    {
        return guiListTypeName+":"+opName+"==|||=="+ stg_ex::int_to_string(widget_box.w)+","+buttonTexture->get_filename()+",,,"+stg_ex::int_to_string(widget_box.w)+","+stg_ex::int_to_string(widget_box.h)+","+stg_ex::int_to_string(opId);
    }
    else
    {
        return guiListTypeName+":"+opName+"==|||=="+ stg_ex::int_to_string(widget_box.w)+",0,,,0,0,"+ stg_ex::int_to_string(opId);
    }
}

void GPE_ToolIconButton::load_data(std::string dataString)
{
    int buttonSize = stg_ex::split_first_int(dataString,',');
    if( buttonSize <=0)
    {
        widget_box.w = buttonSize;
        widget_box.h = buttonSize;
    }
    std::string texture_file_name = stg_ex::split_first_string(dataString,",,,");
    int animationWidth = stg_ex::split_first_int(dataString,',');
    int animationHeight = stg_ex::split_first_int(dataString,',');
    widget_box.w = animationWidth;
    widget_box.h = animationHeight;
    opId = stg_ex::split_first_int(dataString,',');
    buttonTexture = paw_gui_rsm->texture_add( texture_file_name, texture_file_name );
}

bool GPE_ToolIconButton::ends_section()
{
    return endsSection;
}

int GPE_ToolIconButton::get_id()
{
    return opId;
}

void GPE_ToolIconButton::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam)
{
    set_clicked( false );
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( windowInView == false )
    {
        return;
    }

    if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
    {
        isClicked = true;
    }
    else if( clickedOutside)
    {
        isClicked = false;
    }
    if( isClicked && usesTabs )
    {
        isTabbed = true;
    }

    if( isClicked && (int)webUrl.size() > 3)
    {
        gpe::external_open_url(webUrl);
    }

    if( isHovered )
    {
        gpe::cursor_main_controller->cursor_change( "asterisk");
    }
}

void GPE_ToolIconButton::set_id(int new_id)
{
    opId = new_id;
}

void GPE_ToolIconButton::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);

    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        if( buttonTexture!=NULL)
        {
            gpe::color * renderColor = theme_main->icon_font_color;

            if(isTabbed)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->icon_box_selected_color,false);
                renderColor = theme_main->icon_font_selected_color;
            }
            else if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->icon_box_highlight_color,false);
                renderColor = theme_main->icon_font_highlight_color;
            }

            else if(isHovered)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->icon_box_highlight_color,false);
                renderColor = theme_main->icon_font_highlight_color;
            }

            buttonTexture->render_tex_resized( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.w,widget_box.h,NULL,renderColor);
        }
        if( endsSection)
        {
            //gpe::gcanvas->render_rectangle( widget_box.x+widget_box.w-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w+1-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->icon_font_color);
        }
    }
}

void GPE_ToolIconButton::set_website(std::string urlIn)
{
    webUrl = urlIn;
    wasClicked = false;
    prerender_self();
}

GPE_ToolIconButtonBar::GPE_ToolIconButtonBar( int buttonSize,bool useTabs )
{
    guiListTypeName = "iconbuttonbar";
    if( buttonSize<=0)
    {
        buttonSize = 24;
    }

    hoverOption = 0;
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = 0;
    widget_box.h = buttonSize;
    barPadding = 8;
    xPadding = GENERAL_GPE_GUI_PADDING;
    newButtonXPos = barPadding;
    isTabBar = useTabs;
    tabPosition = 0;
    widthAutoResizes = true;
}

GPE_ToolIconButtonBar::~GPE_ToolIconButtonBar()
{
    for( int i = barOptions.size()-1; i >=0; i--)
    {
        if( barOptions[i]!=NULL)
        {
            delete barOptions[i];
            barOptions[i] = NULL;
            barOptions.erase(barOptions.begin()+i);
        }
    }
    barOptions.clear();
}

std::string GPE_ToolIconButtonBar::get_data()
{
    return "";
}

void GPE_ToolIconButtonBar::load_data(std::string dataString)
{

}

void GPE_ToolIconButtonBar::limit_width( bool isLimited)
{
    widthIsLimited = isLimited;
    if( widthIsLimited)
    {
        widget_box.w = xPadding*3+( (int)barOptions.size() )*(xPadding+widget_box.h);
    }
}

void GPE_ToolIconButtonBar::set_width(int newWid)
{
    widget_box.w = newWid;
}

GPE_ToolIconButton * GPE_ToolIconButtonBar::add_option( std::string buttonImgFile,std::string name, int id, bool endsSection)
{
    int barSize = (int)barOptions.size();
    GPE_ToolIconButton * newOp = new GPE_ToolIconButton(  buttonImgFile,name,id,widget_box.h, endsSection);
    newOp->descriptionText =  name;
    newOp->usesTabs = isTabBar;
    if( isTabBar && barSize==0 )
    {
        newOp->isTabbed = true;
        tabPosition = 0;
    }

    barOptions.push_back(newOp);

    newButtonXPos+= xPadding+widget_box.h;
    if( endsSection)
    {
        newButtonXPos+=xPadding;
    }
    recalculate_width();
    return newOp;
}


int GPE_ToolIconButtonBar::get_tab_pos()
{
    return tabPosition;
}

int GPE_ToolIconButtonBar::get_tab_id()
{
    if( tabPosition >=0 && tabPosition < (int)barOptions.size() )
    {
        if( barOptions[tabPosition]!=NULL)
        {
            return barOptions[tabPosition]->get_id();
        }
    }
    return 0;
}

void GPE_ToolIconButtonBar::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    GPE_ToolIconButton * cButton = NULL;
    GPE_ToolIconButton * dButton = NULL;
    int buttonsItr=0;
    int otherButtonsItr=0;

    GPE_GeneralGuiElement::process_self(viewedSpace,cam);

    recalculate_width();
    if( isTabBar)
    {
        if( isClicked)
        {
            hasArrowkeyControl = true;
        }
        else if( clickedOutside)
        {
            hasArrowkeyControl = false;
        }
        else if( isInUse)
        {
            hasArrowkeyControl = true;
        }
        else
        {
            hasArrowkeyControl = false;
        }
    }
    else
    {
        selectedOption = -1;
        hasArrowkeyControl = false;
    }
    int newButtonXPos = widget_box.x;
    for( buttonsItr =0;  buttonsItr<(int)barOptions.size(); buttonsItr++ )
    {
        cButton = barOptions[buttonsItr];
        if(cButton!=NULL)
        {
            if( !isTabBar)
            {
                cButton->set_clicked(false);
                cButton->switch_inuse(false);
            }
            cButton->set_coords( newButtonXPos, widget_box.y );
            cButton->process_self(viewedSpace, cam);
            if( cButton->is_clicked()  )
            {
                selectedOption = cButton->get_id();
                if( isTabBar)
                {
                    tabPosition = buttonsItr;
                    for( otherButtonsItr=0; otherButtonsItr <(int)barOptions.size(); otherButtonsItr++)
                    {
                        dButton = barOptions[otherButtonsItr];
                        if( dButton!=NULL )
                        {
                            dButton->isTabbed = false;
                        }
                    }
                    cButton->isTabbed = true;
                }
            }
            else if( cButton->is_hovered() )
            {
                main_OVERLAY->update_tooltip( cButton->get_name() );
            }
            newButtonXPos+= xPadding+widget_box.h;
        }
    }

    if( isTabBar && hasArrowkeyControl)
    {
        bool tabMoved = false;
        if( tabPosition > 0 && (gpe::input->check_kb_pressed(kb_left) || gpe::input->check_kb_pressed(kb_up) ) )
        {
            tabPosition--;
            tabMoved = true;
        }
        else if(gpe::input->check_kb_pressed(kb_right) || gpe::input->check_kb_pressed(kb_down) )
        {
            if( tabPosition < (int)barOptions.size()-1 )
            {
                tabPosition++;
                tabMoved = true;
            }
        }

        if( tabMoved )
        {
            for( buttonsItr=0; buttonsItr <(int)barOptions.size(); buttonsItr++)
            {
                cButton = barOptions[buttonsItr];
                if(cButton!=NULL)
                {
                    if( buttonsItr==tabPosition )
                    {
                        cButton->isTabbed =  true;
                        selectedOption = cButton->get_id();
                    }
                    else
                    {
                        cButton->isTabbed =  false;
                        cButton->set_clicked( false );
                        cButton->switch_inuse( false );
                    }
                }
            }
        }
    }
}

void GPE_ToolIconButtonBar::recalculate_width()
{
    int expectedWidth = ( (int)barOptions.size() )*(xPadding+widget_box.h);
    if( widget_box.w < expectedWidth )
    {
        set_width( expectedWidth );
    }
}

void GPE_ToolIconButtonBar::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect *cam)
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);

    if( widget_box.h > 0)
    {
        //gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->popup_box_color,false);
        GPE_ToolIconButton * cButton = NULL;
        for(int i=0; i< (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if(cButton!=NULL)
            {
                cButton->render_self( viewedSpace,cam);
            }
        }
        if( isInUse)
        {
            //gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_url_color,true);
        }
        //gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->popup_box_border_color,true);
    }
}

void GPE_ToolIconButtonBar::set_coords(int newX, int newY)
{
    int pastX = widget_box.x;
    int pastY = widget_box.y;
    GPE_GeneralGuiElement::set_coords(newX,newY);
    if( widget_box.x!=pastX || widget_box.y!=pastY)
    {
        GPE_ToolIconButton * cButton = NULL;
        int xNewPos = widget_box.x+xPadding;
        for( int i = 0; i < (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if( cButton!=NULL)
            {
                cButton->set_coords(xNewPos, widget_box.y);
                cButton->set_height( widget_box.h);
                xNewPos+=xPadding+widget_box.h;
                if( cButton->ends_section() )
                {
                    xNewPos+=xPadding;
                }
            }
        }
    }
    if( widthAutoResizes)
    {
        recalculate_width();
    }
}


void GPE_ToolIconButtonBar::set_height(int newHeight)
{
    int pastH = widget_box.h;
    if( widget_box.h!=newHeight)
    {
        widget_box.h = newHeight;
        newButtonXPos = xPadding;
        GPE_ToolIconButton * cButton = NULL;
        for( int i = 0; i < (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if( cButton!=NULL)
            {
                cButton->set_coords(widget_box.x+newButtonXPos,widget_box.y);
                cButton->set_width(newHeight);
                cButton->set_height(newHeight);
                newButtonXPos+=widget_box.h;
                if( cButton->ends_section() )
                {
                    newButtonXPos+=xPadding;
                }
            }
        }
        recalculate_width();
    }
}

void GPE_ToolIconButtonBar::set_selection(int newSelection)
{
    GPE_ToolIconButton * cButton = NULL;
    bool foundValue = false;
    for(int i=0; i<(int)barOptions.size(); i++)
    {
        cButton = barOptions[i];
        if( cButton!=NULL )
        {
            if( cButton->get_id()==newSelection )
            {
                tabPosition = i;
                if( !foundValue )
                {
                    cButton->switch_inuse( true );
                    cButton->isTabbed = true;
                    foundValue = true;
                }
            }
            else
            {
                cButton->switch_inuse( false );
                cButton->isTabbed = false;
            }
        }
    }
}

GPE_ToolLabelButton::GPE_ToolLabelButton( std::string name, std::string description,int id )
{
    guiListTypeName = "labelbutton";
    opName = name;
    descriptionText = description;
    opId = -1;

    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = 192;
    widget_box.h = 24;
    if( font_default!=NULL)
    {
        int textW = 0;
        int textH = 0;
        font_default->get_metrics(name,&textW, &textH);
        //opStringTexture->loadFromRenderedText(gpe::renderer_main,new_name,theme_main->main_box_font_color,FONT_POPUP);
        if( textH > widget_box.h)
        {
            widget_box.h=textH+GENERAL_GPE_GUI_PADDING*2;
        }
    }
    isEnabled = true;
}

GPE_ToolLabelButton::~GPE_ToolLabelButton()
{

}



void GPE_ToolLabelButton::prerender_self( )
{

    if( font_default!=NULL)
    {
        /*
        int textW = 0;
        int textH = 0;
        font_default->get_metrics(opName,&textW, &textH);
        //widget_box.w=textW+GENERAL_GPE_GUI_PADDING*2;
        //opStringTexture->loadFromRenderedText(gpe::renderer_main,new_name,theme_main->main_box_font_color,FONT_POPUP);
        */
    }
}

void GPE_ToolLabelButton::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect  * cam )
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered)
    {
        gpe::cursor_main_controller->cursor_change("hand");
    }
    if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
    {
        isClicked = true;
    }
}

void GPE_ToolLabelButton::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect  * cam )
{
    cam = gpe::camera_find(cam);
    viewedSpace = gpe::camera_find(viewedSpace);
    if(cam!=NULL && viewedSpace!=NULL)
    {
        //Renders shadow
        if( isHovered || isInUse )
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x-4,widget_box.y-cam->y+4,widget_box.x-cam->x+widget_box.w+4,widget_box.y-cam->y+widget_box.h+4, theme_main->button_box_shadow_color,false, 64 );
        }

        if( isClicked)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->button_box_selected_color,false);
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->button_border_selected_color,true);
        }
        else if( isHovered)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->button_box_highlight_color,false);
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->button_border_highlight_color,true);
        }
        else
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->button_box_color,false);
        }
        if(font_default!=NULL)
        {
            if( isHovered)
            {
                gpe::gfs->render_text( widget_box.x-cam->x+widget_box.w/2, widget_box.y-cam->y+widget_box.h/2,opName,theme_main->button_font_highlight_color,font_default,gpe::fa_center,gpe::fa_middle);
            }
            else
            {
                gpe::gfs->render_text( widget_box.x-cam->x+widget_box.w/2, widget_box.y-cam->y+widget_box.h/2,opName,theme_main->button_font_color,font_default,gpe::fa_center,gpe::fa_middle);
            }
            /*if( widget_box.w > opStringTexture->get_width()+GENERAL_GPE_GUI_PADDING*2 )
            {
                opStringTexture->render_tex( widget_box.x-cam->x+(widget_box.w-opStringTexture->get_width() )/2,widget_box.y-cam->y+GENERAL_GPE_GUI_PADDING,NULL);
            }
            else
            {
                opStringTexture->render_tex( widget_box.x-cam->x+GENERAL_GPE_GUI_PADDING,widget_box.y-cam->y+GENERAL_GPE_GUI_PADDING,NULL);
            }*/
        }
        /*
        if( isInUse)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->button_border_highlight_color,true);
        }
        else if( isHovered)
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->button_border_highlight_color,true);
        }
        else
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_border_color,true);
        }
        */

        if( isInUse )
        {
            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,theme_main->main_border_highlight_color,true);
        }
    }
}


void GPE_ToolLabelButton::set_name(std::string new_name)
{
    if( font_default!=NULL)
    {
        int textW = 0;
        int textH = 0;
        font_default->get_metrics(new_name,&textW, &textH);
        //opStringTexture->loadFromRenderedText(gpe::renderer_main,new_name,theme_main->main_box_font_color,font_default);
        widget_box.w=textW+GENERAL_GPE_GUI_PADDING*2;
        widget_box.h=textH+GENERAL_GPE_GUI_PADDING*2;
    }
    opName = new_name;
}

///
GPE_ToolPushButton::GPE_ToolPushButton( std::string imgLocation,std::string name, std::string description,int id)
{
    showBackground = true;
    guiListTypeName = "pushbutton";
    opName = name;
    descriptionText = description;
    opId = -1;
    opTexture = paw_gui_rsm->texture_add_filename( imgLocation);
    iconPadding = GENERAL_GPE_GUI_PADDING;
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = 192;
    widget_box.h = 32;
    maxCharactersToRender = 0;
    buttonTextWidth = 0;
    buttonTextHeight = 0;

    isEnabled = true;
    wasClicked = false;
    webUrl = "";
    prerender_self();
}

GPE_ToolPushButton::~GPE_ToolPushButton()
{

}

void GPE_ToolPushButton::prerender_self( )
{
    if( FONT_BUTTONS_FONT!=NULL && widget_box.w > 32)
    {
        int characterWidth = 0;
        int nameSize = (int)opName.size();
        FONT_BUTTONS_FONT->get_metrics("A",&buttonTextWidth, &buttonTextHeight);
        characterWidth = buttonTextWidth;
        if( characterWidth!=0 )
        {
            maxCharactersToRender = (widget_box.w -32) / characterWidth;
            if( nameSize > maxCharactersToRender )
            {
                buttonTextWidth = maxCharactersToRender * characterWidth;
            }
            else
            {
                buttonTextWidth = nameSize * characterWidth;
            }
        }

    }
}

void GPE_ToolPushButton::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect  * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if(viewedSpace!=NULL && cam!=NULL)
    {
        GPE_GeneralGuiElement::process_self(viewedSpace,cam);
        prerender_self();
        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change("hand");
        }
        if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked && (int)webUrl.size() > 3)
        {
            gpe::external_open_url(webUrl);
        }
    }
}

void GPE_ToolPushButton::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect  * cam )
{
    if( isEnabled)
    {
        int nameSize = (int)opName.size();
        viewedSpace = gpe::camera_find(viewedSpace);
        cam = gpe::camera_find(cam);
        if(viewedSpace!=NULL && cam!=NULL)
        {
            gpe::color * renderColor = theme_main->button_font_color;

            if( showBackground )
            {
                if( isClicked )
                {
                    renderColor = theme_main->main_box_faded_font_color;
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,theme_main->button_box_selected_color,false);
                }
                else
                {
                    if( isHovered)
                    {
                        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,theme_main->button_box_highlight_color,false);
                        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,theme_main->button_border_highlight_color,true);
                        renderColor = theme_main->button_font_highlight_color;

                    }
                    else
                    {
                        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,theme_main->button_box_color,false);
                        renderColor = theme_main->button_font_color;
                    }
                }
            }
            else
            {
                if( isHovered)
                {
                    renderColor = theme_main->main_box_font_url_hovered_color;
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,theme_main->button_border_highlight_color,true);
                }
                else if( wasClicked)
                {
                    renderColor = theme_main->main_box_font_url_visited_color;
                }
                else
                {
                    renderColor = theme_main->main_box_font_url_color;
                }
            }

            int iconSize = 0;
            iconSize = std::min(widget_box.w,widget_box.h);

            if( opTexture!=NULL)
            {
                if( nameSize > maxCharactersToRender )
                {
                    opTexture->render_tex_resized( widget_box.x-cam->x+iconPadding, widget_box.y-cam->y,iconSize,iconSize, NULL, renderColor);
                    gpe::gfs->render_text( widget_box.x-cam->x+iconSize+iconPadding, widget_box.y-cam->y+widget_box.h/2, stg_ex::get_substring( opName, 0, maxCharactersToRender -2)+"...",renderColor,FONT_BUTTONS_FONT,gpe::fa_left,gpe::fa_middle);
                }
                else
                {
                    //abs(widget_box.w-buttonTextWidth)/2+ - iconSize
                    opTexture->render_tex_resized( widget_box.x - cam->x + iconPadding, widget_box.y-cam->y,iconSize,iconSize,NULL,renderColor);
                    if( FONT_BUTTONS_FONT!=NULL)
                    {
                        gpe::gfs->render_text( abs(widget_box.w-buttonTextWidth)/2+widget_box.x-cam->x+iconPadding, widget_box.y-cam->y+widget_box.h/2,opName,renderColor,FONT_LABEL,gpe::fa_left,gpe::fa_middle);
                    }
                }
            }
            else if(FONT_BUTTONS_FONT!=NULL)
            {
                if( nameSize > maxCharactersToRender )
                {
                    gpe::gfs->render_text( widget_box.x-cam->x+iconSize+iconPadding, widget_box.y-cam->y+widget_box.h/2,stg_ex::get_substring( opName, 0, maxCharactersToRender  -2)+"...",renderColor,FONT_BUTTONS_FONT,gpe::fa_left,gpe::fa_middle);
                }
                else
                {
                    gpe::gfs->render_text( widget_box.x-cam->x+widget_box.w/2, widget_box.y-cam->y+widget_box.h/2,opName,renderColor,FONT_BUTTONS_FONT,gpe::fa_center,gpe::fa_middle);
                }
            }
            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,theme_main->main_box_font_url_color,true);
            }
        }
    }
}

void GPE_ToolPushButton::set_name(std::string new_name)
{
    opName = new_name;
    prerender_self();
}

void GPE_ToolPushButton::set_website(std::string urlIn)
{
    webUrl = urlIn;
    wasClicked = false;
    prerender_self();
}

///


GPE_VerticalCardButton::GPE_VerticalCardButton( std::string imgLocation, std::string parsedLines, std::string strValue, int id, int buttonSize)
{
    usingFlagIcon = false;
    showBackground = false;
    int ww = 0;
    lineHeight = 0;
    if(font_default !=NULL)
    {
        font_default->get_numbered_metrics("A",&ww,&lineHeight);
    }
    guiListTypeName = "verticalButton";
    if( buttonSize <=0)
    {
        buttonSize = 128;
    }
    opName = parsedLines;

    if( (int)parsedLines.size() > 0 )
    {
        int newLinePos=parsedLines.find_first_of("\n");
        if(newLinePos!=(int)std::string::npos)
        {
            std::string cbNLString = stg_ex::split_first_string(parsedLines,'\n');
            buttonLines.push_back(cbNLString);
            newLinePos=parsedLines.find_first_of('\n');
            while (newLinePos!=(int)std::string::npos)
            {
                cbNLString = stg_ex::split_first_string(parsedLines,'\n');
                newLinePos=parsedLines.find_first_of('\n');
                buttonLines.push_back(cbNLString);
            }
            if( (int)parsedLines.size() > 0 )
            {
                buttonLines.push_back(parsedLines);
            }
        }
        else
        {
            buttonLines.push_back(parsedLines);
        }
    }
    descriptionText = strValue;
    opId = id;
    opTexture = paw_gui_rsm->texture_add_filename( imgLocation );
    iconPadding = 8;
    widget_box.x = 0;
    widget_box.y = 0;
    widget_box.w = buttonSize;
    widget_box.h = buttonSize+(int)buttonLines.size() *(GENERAL_GPE_GUI_PADDING+lineHeight );

    isEnabled = true;
    wasClicked = false;
    webUrl = "";
    prerender_self();
}

GPE_VerticalCardButton::~GPE_VerticalCardButton()
{

}

int GPE_VerticalCardButton::get_id()
{
    return opId;
}

void GPE_VerticalCardButton::prerender_self( )
{
    /*
    int prevWidth = 0;

    if( font_default!=NULL)
    {
        int textW = 0;
        int textH = 0;
        font_default->get_metrics(opName,&textW, &textH);
        widget_box.w = iconPadding*3+widget_box.h+textW;
    }
    */
}

void GPE_VerticalCardButton::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect  * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if(viewedSpace!=NULL && cam!=NULL)
    {
        GPE_GeneralGuiElement::process_self(viewedSpace,cam);
        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change("hand");
        }
        if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked && (int)webUrl.size() > 3)
        {
            gpe::external_open_url(webUrl);
        }

    }
}

void GPE_VerticalCardButton::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect  * cam )
{
    if( isEnabled)
    {
        viewedSpace = gpe::camera_find(viewedSpace);
        cam = gpe::camera_find(cam);

        if(viewedSpace!=NULL && cam!=NULL)
        {
            gpe::color * textRenderColor = theme_main->button_font_color;
            gpe::color * bgRenderColor = theme_main->button_box_color;
            gpe::color * textureRenderColor = gpe::c_white;
            if( isHovered || isInUse)
            {
                textRenderColor = theme_main->button_font_highlight_color;
            }

            if( isClicked )
            {
                bgRenderColor = theme_main->button_box_selected_color;
                textRenderColor= theme_main->button_font_highlight_color;
            }
            else if( isHovered)
            {
                bgRenderColor = theme_main->button_box_highlight_color;
                textRenderColor= theme_main->button_font_highlight_color;
            }
            if( showBackground )
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,bgRenderColor,false);
            }



            if( opTexture!=NULL)
            {
                if( usingFlagIcon)
                {
                    opTexture->render_tex_resized( widget_box.x-cam->x, widget_box.y-cam->y,widget_box.w,widget_box.w,NULL,textRenderColor );
                }
                else
                {
                    opTexture->render_tex_resized( widget_box.x-cam->x, widget_box.y-cam->y,widget_box.w,widget_box.w,NULL, textureRenderColor );
                }
            }


            if(font_default!=NULL)
            {
                for( int ipLine = 0; ipLine < (int)buttonLines.size(); ipLine++)
                {
                    gpe::gfs->render_text( widget_box.x+widget_box.w/2-cam->x, widget_box.y-cam->y + widget_box.w+GENERAL_GPE_GUI_PADDING+(ipLine * (GENERAL_GPE_GUI_PADDING+lineHeight) ), buttonLines[ipLine], textRenderColor, font_default, gpe::fa_center, gpe::fa_top );
                }
            }

            if(  isInUse )
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,theme_main->button_border_selected_color,true);
            }
            else if(isHovered)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,theme_main->button_border_highlight_color,true);
            }
        }
    }
}

void GPE_VerticalCardButton::set_name(std::string new_name)
{
    if( (int)new_name.size() > 0 )
    {
        buttonLines.clear();
        int newLinePos=new_name.find_first_of("\n");
        if(newLinePos!=(int)std::string::npos)
        {
            std::string cbNLString = stg_ex::split_first_string(new_name,'\n');
            buttonLines.push_back(cbNLString);
            newLinePos=new_name.find_first_of('\n');
            while (newLinePos!=(int)std::string::npos)
            {
                buttonLines.push_back(cbNLString);
                cbNLString = stg_ex::split_first_string(new_name,'\n');
                newLinePos=new_name.find_first_of('\n');
            }
            if( (int)new_name.size() > 0 )
            {
                buttonLines.push_back(new_name);
            }
        }
        else
        {
            buttonLines.push_back(new_name);
        }
    }
    //widget_box.h = GENERAL_GPE_GUI_PADDING*4+( (int)buttonLines.size() ) * (GENERAL_GPE_GUI_PADDING+lineHeight);
    //widget_box.h = GENERAL_GPE_GUI_PADDING+( (int)buttonLines.size() *(lineHeight+GENERAL_GPE_GUI_PADDING) );
    prerender_self();
}

void GPE_VerticalCardButton::set_website(std::string urlIn)
{
    webUrl = urlIn;
    wasClicked = false;
    prerender_self();
}
