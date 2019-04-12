/*
paw_gui_buttons.cpp
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

#include "paw_gui_buttons.h"


GPE_ToolIconButton::GPE_ToolIconButton( std::string buttonImgFile,std::string name, int id, int buttonSize, bool lastInCol)
{
    webUrl = "";
    wasClicked = false;
    guiListTypeName = "iconbutton";
    endsSection = lastInCol;
    opName = name;
    descriptionText = name;
    opId = id;
    buttonTexture = guiRCM->texture_add( buttonImgFile );
    if( buttonTexture == NULL)
    {
        GPE_Report("Unable to add texture with img ["+buttonImgFile+"]..." );
    }
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = buttonSize;
    elementBox.h = buttonSize;
    isTabbed = false;
    usesTabs = false;
}

GPE_ToolIconButton::~GPE_ToolIconButton()
{

}

void GPE_ToolIconButton::change_texture( GPE_Texture * newTexture)
{
    buttonTexture = newTexture;
}

std::string GPE_ToolIconButton::get_data()
{
    if( buttonTexture!=NULL)
    {
        return guiListTypeName+":"+opName+"==|||=="+int_to_string(elementBox.w)+","+buttonTexture->get_filename()+",,,"+int_to_string(elementBox.w)+","+int_to_string(elementBox.h)+","+int_to_string(opId);
    }
    else
    {
        return guiListTypeName+":"+opName+"==|||=="+int_to_string(elementBox.w)+",0,,,0,0,"+int_to_string(opId);
    }
}

void GPE_ToolIconButton::load_data(std::string dataString)
{
    int buttonSize = split_first_int(dataString,',');
    if( buttonSize <=0)
    {
        elementBox.w = buttonSize;
        elementBox.h = buttonSize;
    }
    std::string textureFileName = split_first_string(dataString,",,,");
    int spriteWidth = split_first_int(dataString,',');
    int spriteHeight = split_first_int(dataString,',');
    elementBox.w = spriteWidth;
    elementBox.h = spriteHeight;
    opId = split_first_int(dataString,',');
    buttonTexture = guiRCM->texture_add(textureFileName);
}

bool GPE_ToolIconButton::ends_section()
{
    return endsSection;
}

int GPE_ToolIconButton::get_id()
{
    return opId;
}

void GPE_ToolIconButton::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    set_clicked( false );
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
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
        GPE_OpenURL(webUrl);
    }
}

void GPE_ToolIconButton::set_id(int newId)
{
    opId = newId;
}

void GPE_ToolIconButton::set_image( std::string buttonImgFile)
{
    /*if( buttonTexture!=NULL )
    {
        buttonTexture->load_new_texture(buttonImgFile,-1,true);
    }
    else
    {
        */buttonTexture = guiRCM->texture_add(buttonImgFile);
    //}
}

void GPE_ToolIconButton::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);

    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( forceRedraw && cam!=NULL && viewedSpace!=NULL)
    {
        if( buttonTexture!=NULL)
        {
            GPE_Color * renderColor = GPE_MAIN_THEME->Icon_Font_Color;

            if(isTabbed)
            {
                gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Icon_Box_Selected_Color,false);
                renderColor = GPE_MAIN_THEME->Icon_Font_Selected_Color;
            }
            else if( isInUse)
            {
                gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Icon_Box_Highlighted_Color,false);
                renderColor = GPE_MAIN_THEME->Icon_Font_Highlighted_Color;
            }

            else if(isHovered)
            {
                gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Icon_Box_Highlighted_Color,false);
                renderColor = GPE_MAIN_THEME->Icon_Font_Highlighted_Color;
            }

            buttonTexture->render_tex_resized( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.w,elementBox.h,NULL,renderColor);
        }
        if( endsSection)
        {
            //gcanvas->render_rectangle( elementBox.x+elementBox.w-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w+1-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Icon_Font_Color);
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
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 0;
    elementBox.h = buttonSize;
    barPadding = 8;
    xPadding = GENERAL_GPE_PADDING;
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
        elementBox.w = xPadding*3+( (int)barOptions.size() )*(xPadding+elementBox.h);
    }
}

void GPE_ToolIconButtonBar::set_width(int newWid)
{
    elementBox.w = newWid;
}

GPE_ToolIconButton * GPE_ToolIconButtonBar::add_option( std::string buttonImgFile,std::string name, int id, bool endsSection)
{
    int barSize = (int)barOptions.size();
    GPE_ToolIconButton * newOp = new GPE_ToolIconButton(  buttonImgFile,name,id,elementBox.h, endsSection);
    newOp->descriptionText =  name;
    newOp->usesTabs = isTabBar;
    if( isTabBar && barSize==0 )
    {
        newOp->isTabbed = true;
        tabPosition = 0;
    }

    barOptions.push_back(newOp);

    newButtonXPos+= xPadding+elementBox.h;
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

void GPE_ToolIconButtonBar::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam)
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
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
    int newButtonXPos = elementBox.x;
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
            cButton->set_coords( newButtonXPos, elementBox.y );
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
                MAIN_OVERLAY->update_tooltip( cButton->get_name() );
            }
            newButtonXPos+= xPadding+elementBox.h;
        }
    }

    if( isTabBar && hasArrowkeyControl)
    {
        bool tabMoved = false;
        if( tabPosition > 0 && (input->check_keyboard_pressed(kb_left) || input->check_keyboard_pressed(kb_up) ) )
        {
            tabPosition--;
            tabMoved = true;
        }
        else if(input->check_keyboard_pressed(kb_right) || input->check_keyboard_pressed(kb_down) )
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
    int expectedWidth = ( (int)barOptions.size() )*(xPadding+elementBox.h);
    if( elementBox.w < expectedWidth )
    {
        set_width( expectedWidth );
    }
}

void GPE_ToolIconButtonBar::render_self( GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);

    if( forceRedraw && elementBox.h > 0)
    {
        //gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->PopUp_Box_Color,false);
        GPE_ToolIconButton * cButton = NULL;
        for(int i=0; i< (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if(cButton!=NULL)
            {
                cButton->render_self( viewedSpace, cam,forceRedraw);
            }
        }
        if( isInUse)
        {
            //gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
        }
        //gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->PopUp_Box_Border_Color,true);
    }
}

void GPE_ToolIconButtonBar::set_coords(int newX, int newY)
{
    int pastX = elementBox.x;
    int pastY = elementBox.y;
    GPE_GeneralGuiElement::set_coords(newX,newY);
    if( elementBox.x!=pastX || elementBox.y!=pastY)
    {
        GPE_ToolIconButton * cButton = NULL;
        int xNewPos = elementBox.x+xPadding;
        for( int i = 0; i < (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if( cButton!=NULL)
            {
                cButton->set_coords(xNewPos, elementBox.y);
                cButton->set_height( elementBox.h);
                xNewPos+=xPadding+elementBox.h;
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
    int pastH = elementBox.h;
    if( elementBox.h!=newHeight)
    {
        elementBox.h = newHeight;
        newButtonXPos = xPadding;
        GPE_ToolIconButton * cButton = NULL;
        for( int i = 0; i < (int)barOptions.size(); i++)
        {
            cButton = barOptions[i];
            if( cButton!=NULL)
            {
                cButton->set_coords(elementBox.x+newButtonXPos,elementBox.y);
                cButton->set_width(newHeight);
                cButton->set_height(newHeight);
                newButtonXPos+=elementBox.h;
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

    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    elementBox.h = 24;
    if( GPE_DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        GPE_DEFAULT_FONT->get_metrics(name,&textW, &textH);
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_POPUP);
        if( textH > elementBox.h)
        {
            elementBox.h=textH+GENERAL_GPE_PADDING*2;
        }
    }
    isEnabled = true;
}

GPE_ToolLabelButton::~GPE_ToolLabelButton()
{

}



void GPE_ToolLabelButton::prerender_self( )
{

    if( GPE_DEFAULT_FONT!=NULL)
    {
        /*
        int textW = 0;
        int textH = 0;
        GPE_DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        //elementBox.w=textW+GENERAL_GPE_PADDING*2;
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_POPUP);
        */
    }
}

void GPE_ToolLabelButton::process_self(GPE_Rect * viewedSpace, GPE_Rect  * cam )
{
    GPE_GeneralGuiElement::process_self(viewedSpace,cam);
    if( isHovered)
    {
        GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
    }
    if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
    {
        isClicked = true;
    }
}

void GPE_ToolLabelButton::render_self( GPE_Rect * viewedSpace, GPE_Rect  * cam,bool forceRedraw )
{
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if(cam!=NULL && viewedSpace!=NULL && forceRedraw )
    {
        //Renders shadow
        if( isHovered || isInUse )
        {
            gcanvas->render_rectangle( elementBox.x-cam->x-4,elementBox.y-cam->y+4,elementBox.x-cam->x+elementBox.w+4,elementBox.y-cam->y+elementBox.h+4, GPE_MAIN_THEME->Button_Box_Shadow_Color,false, 64 );
        }

        if( isClicked)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Box_Selected_Color,false);
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Border_Selected_Color,true);
        }
        else if( isHovered)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Box_Highlighted_Color,false);
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Box_Color,false);
        }
        if(GPE_DEFAULT_FONT!=NULL)
        {
            if( isHovered)
            {
                gfs->render_text( elementBox.x-cam->x+elementBox.w/2, elementBox.y-cam->y+elementBox.h/2,opName,GPE_MAIN_THEME->Button_Font_Highlighted_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
            }
            else
            {
                gfs->render_text( elementBox.x-cam->x+elementBox.w/2, elementBox.y-cam->y+elementBox.h/2,opName,GPE_MAIN_THEME->Button_Font_Color,GPE_DEFAULT_FONT,FA_CENTER,FA_MIDDLE);
            }
            /*if( elementBox.w > opStringTexture->get_width()+GENERAL_GPE_PADDING*2 )
            {
                opStringTexture->render_tex( elementBox.x-cam->x+(elementBox.w-opStringTexture->get_width() )/2,elementBox.y-cam->y+GENERAL_GPE_PADDING,NULL);
            }
            else
            {
                opStringTexture->render_tex( elementBox.x-cam->x+GENERAL_GPE_PADDING,elementBox.y-cam->y+GENERAL_GPE_PADDING,NULL);
            }*/
        }
        /*
        if( isInUse)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
        }
        else if( isHovered)
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Border_Color,true);
        }
        */

        if( isInUse )
        {
            gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Main_Border_Highlighted_Color,true);
        }
    }
}


void GPE_ToolLabelButton::set_name(std::string newName)
{
    if( GPE_DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        GPE_DEFAULT_FONT->get_metrics(newName,&textW, &textH);
        //opStringTexture->loadFromRenderedText(MAIN_RENDERER,newName,GPE_MAIN_THEME->Main_Box_Font_Color,GPE_DEFAULT_FONT);
        elementBox.w=textW+GENERAL_GPE_PADDING*2;
        elementBox.h=textH+GENERAL_GPE_PADDING*2;
    }
    opName = newName;
}

///
GPE_ToolPushButton::GPE_ToolPushButton( std::string imgLocation,std::string name, std::string description,int id)
{
    showBackground = true;
    guiListTypeName = "pushbutton";
    opName = name;
    descriptionText = description;
    opId = -1;
    opTexture = guiRCM->texture_add(imgLocation);
    iconPadding = GENERAL_GPE_PADDING;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = 192;
    elementBox.h = 24;
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

void GPE_ToolPushButton::change_texture( GPE_Texture * newTexture)
{
    opTexture = newTexture;
}

void GPE_ToolPushButton::prerender_self( )
{
    if( FONT_BUTTONS_FONT!=NULL && elementBox.w > 32)
    {
        int characterWidth = 0;
        int nameSize = (int)opName.size();
        FONT_BUTTONS_FONT->get_metrics("A",&buttonTextWidth, &buttonTextHeight);
        characterWidth = buttonTextWidth;
        if( characterWidth!=0 )
        {
            maxCharactersToRender = (elementBox.w -32) / characterWidth;
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

void GPE_ToolPushButton::process_self(GPE_Rect * viewedSpace, GPE_Rect  * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(viewedSpace!=NULL && cam!=NULL)
    {
        GPE_GeneralGuiElement::process_self(viewedSpace,cam);
        if( isHovered)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
        }
        if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked && (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }
    }
}

void GPE_ToolPushButton::render_self( GPE_Rect * viewedSpace, GPE_Rect  * cam,bool forceRedraw )
{
    if( forceRedraw && isEnabled)
    {
        int nameSize = (int)opName.size();
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);
        if(viewedSpace!=NULL && cam!=NULL)
        {
            GPE_Color * renderColor = GPE_MAIN_THEME->Button_Font_Color;

            if( showBackground )
            {
                if( isClicked )
                {
                    renderColor = GPE_MAIN_THEME->Main_Box_Faded_Font_Color;
                    gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Box_Selected_Color,false);
                }
                else
                {
                    if( isHovered)
                    {
                        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Box_Highlighted_Color,false);
                        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
                        renderColor = GPE_MAIN_THEME->Button_Font_Highlighted_Color;

                    }
                    else
                    {
                        gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Box_Color,false);
                        renderColor = GPE_MAIN_THEME->Button_Font_Color;
                    }
                }
            }
            else
            {
                if( isHovered)
                {
                    renderColor = GPE_MAIN_THEME->Main_Box_Font_URL_Hovered_Color;
                    gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
                }
                else if( wasClicked)
                {
                    renderColor = GPE_MAIN_THEME->Main_Box_Font_URL_Visited_Color;
                }
                else
                {
                    renderColor = GPE_MAIN_THEME->Main_Box_Font_URL_Color;
                }
            }

            int iconSize = 0;
            iconSize = std::min(elementBox.w,elementBox.h);

            if( opTexture!=NULL)
            {
                if( nameSize > maxCharactersToRender )
                {
                    opTexture->render_tex_resized( elementBox.x-cam->x+iconPadding, elementBox.y-cam->y,iconSize,iconSize, NULL, renderColor);
                    gfs->render_text( elementBox.x-cam->x+iconSize+iconPadding, elementBox.y-cam->y+elementBox.h/2,get_substring( opName, 0, maxCharactersToRender -2)+"...",renderColor,FONT_BUTTONS_FONT,FA_LEFT,FA_MIDDLE);
                }
                else
                {
                    //abs(elementBox.w-buttonTextWidth)/2+ - iconSize
                    opTexture->render_tex_resized( elementBox.x - cam->x + iconPadding, elementBox.y-cam->y,iconSize,iconSize,NULL,renderColor);
                    if( FONT_BUTTONS_FONT!=NULL)
                    {
                        gfs->render_text( abs(elementBox.w-buttonTextWidth)/2+elementBox.x-cam->x+iconPadding, elementBox.y-cam->y+elementBox.h/2,opName,renderColor,FONT_BUTTONS_FONT,FA_LEFT,FA_MIDDLE);
                    }
                }
            }
            else if(FONT_BUTTONS_FONT!=NULL)
            {
                if( nameSize > maxCharactersToRender )
                {
                    gfs->render_text( elementBox.x-cam->x+iconSize+iconPadding, elementBox.y-cam->y+elementBox.h/2,get_substring( opName, 0, maxCharactersToRender  -2)+"...",renderColor,FONT_BUTTONS_FONT,FA_LEFT,FA_MIDDLE);
                }
                else
                {
                    gfs->render_text( elementBox.x-cam->x+elementBox.w/2, elementBox.y-cam->y+elementBox.h/2,opName,renderColor,FONT_BUTTONS_FONT,FA_CENTER,FA_MIDDLE);
                }
            }
            if( isInUse)
            {
                gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x+elementBox.w-cam->x,elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Main_Box_Font_URL_Color,true);
            }
        }
    }
}

void GPE_ToolPushButton::set_name(std::string newName)
{
    opName = newName;
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
    if(GPE_DEFAULT_FONT !=NULL)
    {
        GPE_DEFAULT_FONT->get_numbered_metrics("A",&ww,&lineHeight);
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
            std::string cbNLString = split_first_string(parsedLines,'\n');
            buttonLines.push_back(cbNLString);
            newLinePos=parsedLines.find_first_of('\n');
            while (newLinePos!=(int)std::string::npos)
            {
                cbNLString = split_first_string(parsedLines,'\n');
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
    opTexture = guiRCM->texture_add(imgLocation);
    iconPadding = 8;
    elementBox.x = 0;
    elementBox.y = 0;
    elementBox.w = buttonSize;
    elementBox.h = buttonSize+(int)buttonLines.size() *(GENERAL_GPE_PADDING+lineHeight );

    isEnabled = true;
    wasClicked = false;
    webUrl = "";
    prerender_self();
}

GPE_VerticalCardButton::~GPE_VerticalCardButton()
{

}

void GPE_VerticalCardButton::change_texture( GPE_Texture * newTexture)
{
    opTexture = newTexture;
}

int GPE_VerticalCardButton::get_id()
{
    return opId;
}

void GPE_VerticalCardButton::prerender_self( )
{
    /*
    int prevWidth = 0;

    if( GPE_DEFAULT_FONT!=NULL)
    {
        int textW = 0;
        int textH = 0;
        GPE_DEFAULT_FONT->get_metrics(opName,&textW, &textH);
        elementBox.w = iconPadding*3+elementBox.h+textW;
    }
    */
}

void GPE_VerticalCardButton::process_self(GPE_Rect * viewedSpace, GPE_Rect  * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(viewedSpace!=NULL && cam!=NULL)
    {
        GPE_GeneralGuiElement::process_self(viewedSpace,cam);
        if( isHovered)
        {
            GPE_change_cursor(SDL_SYSTEM_CURSOR_HAND);
        }
        if( isInUse &&( input->check_keyboard_down( kb_enter ) || input->check_keyboard_down( kb_space )  ) )
        {
            isClicked = true;
        }
        if( isClicked && (int)webUrl.size() > 3)
        {
            GPE_OpenURL(webUrl);
        }

    }
}

void GPE_VerticalCardButton::render_self( GPE_Rect * viewedSpace, GPE_Rect  * cam,bool forceRedraw )
{
    if( forceRedraw && isEnabled)
    {
        viewedSpace = GPE_find_camera(viewedSpace);
        cam = GPE_find_camera(cam);

        if(viewedSpace!=NULL && cam!=NULL)
        {
            GPE_Color * textRenderColor = GPE_MAIN_THEME->Button_Font_Color;
            GPE_Color * bgRenderColor = GPE_MAIN_THEME->Button_Box_Color;
            GPE_Color * textureRenderColor = c_white;
            if( isHovered || isInUse)
            {
                textRenderColor = GPE_MAIN_THEME->Button_Font_Highlighted_Color;
            }

            if( isClicked )
            {
                bgRenderColor = GPE_MAIN_THEME->Button_Box_Selected_Color;
                textRenderColor= GPE_MAIN_THEME->Button_Font_Highlighted_Color;
            }
            else if( isHovered)
            {
                bgRenderColor = GPE_MAIN_THEME->Button_Box_Highlighted_Color;
                textRenderColor= GPE_MAIN_THEME->Button_Font_Highlighted_Color;
            }
            if( showBackground )
            {
                gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,bgRenderColor,false);
            }



            if( opTexture!=NULL)
            {
                if( usingFlagIcon)
                {
                    opTexture->render_tex_resized( elementBox.x-cam->x, elementBox.y-cam->y,elementBox.w,elementBox.w,NULL,textRenderColor );
                }
                else
                {
                    opTexture->render_tex_resized( elementBox.x-cam->x, elementBox.y-cam->y,elementBox.w,elementBox.w,NULL, textureRenderColor );
                }
            }


            if(GPE_DEFAULT_FONT!=NULL)
            {
                for( int ipLine = 0; ipLine < (int)buttonLines.size(); ipLine++)
                {
                    gfs->render_text( elementBox.x+elementBox.w/2-cam->x, elementBox.y-cam->y + elementBox.w+GENERAL_GPE_PADDING+(ipLine * (GENERAL_GPE_PADDING+lineHeight) ), buttonLines[ipLine], textRenderColor, GPE_DEFAULT_FONT, FA_CENTER, FA_TOP );
                }
            }

            if(  isInUse )
            {
                gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Border_Selected_Color,true);
            }
            else if(isHovered)
            {
                gcanvas->render_rectangle( elementBox.x-cam->x,elementBox.y-cam->y,elementBox.x-cam->x+elementBox.w,elementBox.y-cam->y+elementBox.h,GPE_MAIN_THEME->Button_Border_Highlighted_Color,true);
            }
        }
    }
}

void GPE_VerticalCardButton::set_name(std::string newName)
{
    if( (int)newName.size() > 0 )
    {
        buttonLines.clear();
        int newLinePos=newName.find_first_of("\n");
        if(newLinePos!=(int)std::string::npos)
        {
            std::string cbNLString = split_first_string(newName,'\n');
            buttonLines.push_back(cbNLString);
            newLinePos=newName.find_first_of('\n');
            while (newLinePos!=(int)std::string::npos)
            {
                buttonLines.push_back(cbNLString);
                cbNLString = split_first_string(newName,'\n');
                newLinePos=newName.find_first_of('\n');
            }
            if( (int)newName.size() > 0 )
            {
                buttonLines.push_back(newName);
            }
        }
        else
        {
            buttonLines.push_back(newName);
        }
    }
    //elementBox.h = GENERAL_GPE_PADDING*4+( (int)buttonLines.size() ) * (GENERAL_GPE_PADDING+lineHeight);
    //elementBox.h = GENERAL_GPE_PADDING+( (int)buttonLines.size() *(lineHeight+GENERAL_GPE_PADDING) );
    prerender_self();
}

void GPE_VerticalCardButton::set_website(std::string urlIn)
{
    webUrl = urlIn;
    wasClicked = false;
    prerender_self();
}
