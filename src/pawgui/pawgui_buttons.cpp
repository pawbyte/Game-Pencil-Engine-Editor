/*
pawgui_buttons.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_buttons.h"

namespace pawgui
{
    widget_button_icon::widget_button_icon( std::string buttonImgFile,std::string name, int id, int buttonSize, bool lastInCol)
    {
        autoResizes = false;
        webUrl = "";
        wasClicked = false;
        widget_type = "iconbutton";
        endsSection = lastInCol;
        widget_name = name;
        descriptionText = name;
        opId = id;
        buttonTexture = rsm_gui->texture_add( buttonImgFile,buttonImgFile );
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

    widget_button_icon::~widget_button_icon()
    {

    }

    void widget_button_icon::change_texture( std::string file_name )
    {
        buttonTexture = rsm_gui->texture_add_filename( file_name );
    }

    std::string widget_button_icon::get_data()
    {
        if( buttonTexture!=NULL)
        {
            return widget_type+":"+widget_name+"==|||=="+ stg_ex::int_to_string(widget_box.w)+","+buttonTexture->get_filename()+",,,"+stg_ex::int_to_string(widget_box.w)+","+stg_ex::int_to_string(widget_box.h)+","+stg_ex::int_to_string(opId);
        }
        else
        {
            return widget_type+":"+widget_name+"==|||=="+ stg_ex::int_to_string(widget_box.w)+",0,,,0,0,"+ stg_ex::int_to_string(opId);
        }
    }

    void widget_button_icon::load_data(std::string dataString)
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
        buttonTexture = rsm_gui->texture_add( texture_file_name, texture_file_name );
    }

    bool widget_button_icon::ends_section()
    {
        return endsSection;
    }

    int widget_button_icon::get_id()
    {
        return opId;
    }

    void widget_button_icon::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
    {
        set_clicked( false );
        widget_basic::process_self(view_space,cam);
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

    void widget_button_icon::set_id(int new_id)
    {
        opId = new_id;
    }

    void widget_button_icon::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);

        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);
        if( cam!=NULL && view_space!=NULL)
        {
            if( buttonTexture!=NULL)
            {
                gpe::color * renderColor = pawgui::theme_main->icon_font_color;

                if(isTabbed)
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->icon_box_selected_color,false);
                    renderColor = pawgui::theme_main->icon_font_selected_color;
                }
                else if( isInUse)
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->icon_box_highlight_color,false);
                    renderColor = pawgui::theme_main->icon_font_highlight_color;
                }

                else if(isHovered)
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->icon_box_highlight_color,false);
                    renderColor = pawgui::theme_main->icon_font_highlight_color;
                }

                buttonTexture->render_tex_resized( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.w,widget_box.h,NULL,renderColor);
            }
            if( endsSection)
            {
                //gpe::gcanvas->render_rectangle( widget_box.x+widget_box.w-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w+1-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->icon_font_color);
            }
        }
    }

    void widget_button_icon::set_website(std::string urlIn)
    {
        webUrl = urlIn;
        wasClicked = false;
        prerender_self();
    }

    widget_button_iconbar::widget_button_iconbar( int buttonSize,bool useTabs )
    {
        widget_type = "iconbuttonbar";
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
        xPadding = padding_default;
        newButtonXPos = barPadding;
        isTabBar = useTabs;
        tabPosition = 0;
        widthAutoResizes = true;
    }

    widget_button_iconbar::~widget_button_iconbar()
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

    std::string widget_button_iconbar::get_data()
    {
        return "";
    }

    void widget_button_iconbar::load_data(std::string dataString)
    {

    }

    void widget_button_iconbar::limit_width( bool isLimited)
    {
        widthIsLimited = isLimited;
        if( widthIsLimited)
        {
            widget_box.w = xPadding*3+( (int)barOptions.size() )*(xPadding+widget_box.h);
        }
    }

    void widget_button_iconbar::set_width(int newWid)
    {
        widget_box.w = newWid;
    }

    widget_button_icon * widget_button_iconbar::add_option( std::string buttonImgFile,std::string name, int id, bool endsSection)
    {
        int barSize = (int)barOptions.size();
        widget_button_icon * newOp = new widget_button_icon(  buttonImgFile,name,id,widget_box.h, endsSection);
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


    int widget_button_iconbar::get_tab_pos()
    {
        return tabPosition;
    }

    int widget_button_iconbar::get_tab_id()
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

    void widget_button_iconbar::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);
        widget_button_icon * cButton = NULL;
        widget_button_icon * dButton = NULL;
        int buttonsItr=0;
        int otherButtonsItr=0;

        widget_basic::process_self(view_space,cam);

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
                cButton->process_self(view_space, cam);
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
                    main_overlay_system->update_tooltip( cButton->get_name() );
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

    void widget_button_iconbar::recalculate_width()
    {
        int expectedWidth = ( (int)barOptions.size() )*(xPadding+widget_box.h);
        if( widget_box.w < expectedWidth )
        {
            set_width( expectedWidth );
        }
    }

    void widget_button_iconbar::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);

        if( widget_box.h > 0)
        {
            //gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->popup_box_color,false);
            widget_button_icon * cButton = NULL;
            for(int i=0; i< (int)barOptions.size(); i++)
            {
                cButton = barOptions[i];
                if(cButton!=NULL)
                {
                    cButton->render_self( view_space,cam);
                }
            }
            if( isInUse)
            {
                //gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_color,true);
            }
            //gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->popup_box_border_color,true);
        }
    }

    void widget_button_iconbar::set_coords(int newX, int newY)
    {
        int pastX = widget_box.x;
        int pastY = widget_box.y;
        widget_basic::set_coords(newX,newY);
        if( widget_box.x!=pastX || widget_box.y!=pastY)
        {
            widget_button_icon * cButton = NULL;
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


    void widget_button_iconbar::set_height(int newHeight)
    {
        int pastH = widget_box.h;
        if( widget_box.h!=newHeight)
        {
            widget_box.h = newHeight;
            newButtonXPos = xPadding;
            widget_button_icon * cButton = NULL;
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

    void widget_button_iconbar::set_selection(int newSelection)
    {
        widget_button_icon * cButton = NULL;
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

    widget_button_label::widget_button_label( std::string name, std::string description,int id )
    {
        widget_type = "labelbutton";
        widget_name = name;
        descriptionText = description;
        opId = -1;

        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 192;
        widget_box.h = 24;
        if( gpe::font_default!=NULL)
        {
            int textW = 0;
            int textH = 0;
            gpe::font_default->get_metrics(name,&textW, &textH);
            if( textH > widget_box.h)
            {
                widget_box.h=textH+padding_default*2;
            }
        }
        isEnabled = true;
    }

    widget_button_label::~widget_button_label()
    {

    }



    void widget_button_label::prerender_self( )
    {

    }

    void widget_button_label::process_self( gpe::shape_rect * view_space, gpe::shape_rect  * cam )
    {
        widget_basic::process_self(view_space,cam);
        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change("hand");
        }
        if( isInUse &&( gpe::input->check_kb_down( kb_enter ) || gpe::input->check_kb_down( kb_space )  ) )
        {
            isClicked = true;
        }
    }

    void widget_button_label::render_self( gpe::shape_rect * view_space, gpe::shape_rect  * cam )
    {
        cam = gpe::camera_find(cam);
        view_space = gpe::camera_find(view_space);
        if(cam!=NULL && view_space!=NULL)
        {
            //Renders shadow
            if( isHovered || isInUse )
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x-4,widget_box.y-cam->y+4,widget_box.x-cam->x+widget_box.w+4,widget_box.y-cam->y+widget_box.h+4, pawgui::theme_main->button_box_shadow_color,false, 64 );
            }

            if( isClicked)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->button_box_selected_color,false);
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->button_border_selected_color,true);
            }
            else if( isHovered)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->button_box_highlight_color,false);
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->button_border_highlight_color,true);
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->button_box_color,false);
            }
            if(gpe::font_default!=NULL)
            {
                if( isHovered)
                {
                    gpe::gfs->render_text( widget_box.x-cam->x+widget_box.w/2, widget_box.y-cam->y+widget_box.h/2,widget_name,pawgui::theme_main->button_font_highlight_color,gpe::font_default,gpe::fa_center,gpe::fa_middle);
                }
                else
                {
                    gpe::gfs->render_text( widget_box.x-cam->x+widget_box.w/2, widget_box.y-cam->y+widget_box.h/2,widget_name,pawgui::theme_main->button_font_color,gpe::font_default,gpe::fa_center,gpe::fa_middle);
                }
                /*if( widget_box.w > opStringTexture->get_width()+padding_default*2 )
                {
                    opStringTexture->render_tex( widget_box.x-cam->x+(widget_box.w-opStringTexture->get_width() )/2,widget_box.y-cam->y+padding_default,NULL);
                }
                else
                {
                    opStringTexture->render_tex( widget_box.x-cam->x+padding_default,widget_box.y-cam->y+padding_default,NULL);
                }*/
            }
            /*
            if( isInUse)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->button_border_highlight_color,true);
            }
            else if( isHovered)
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->button_border_highlight_color,true);
            }
            else
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_border_color,true);
            }
            */

            if( isInUse )
            {
                gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->main_border_highlight_color,true);
            }
        }
    }


    void widget_button_label::set_name(std::string new_name)
    {
        if( gpe::font_default!=NULL)
        {
            int textW = 0;
            int textH = 0;
            gpe::font_default->get_metrics(new_name,&textW, &textH);
            //opStringTexture->loadFromRenderedText(gpe::renderer_main,new_name,pawgui::theme_main->main_box_font_color,gpe::font_default);
            widget_box.w=textW+padding_default*2;
            widget_box.h=textH+padding_default*2;
        }
        widget_name = new_name;
    }

    ///
    widget_button_push::widget_button_push( std::string imgLocation,std::string name, std::string description,int id)
    {
        showBackground = true;
        widget_type = "pushbutton";
        widget_name = name;
        descriptionText = description;
        opId = -1;
        opTexture = rsm_gui->texture_add_filename( imgLocation);
        iconPadding = padding_default;
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

    widget_button_push::~widget_button_push()
    {

    }

    void widget_button_push::prerender_self( )
    {
        if( font_buttons!=NULL && widget_box.w > 32)
        {
            int characterWidth = 0;
            int nameSize = (int)widget_name.size();
            font_buttons->get_metrics("A",&buttonTextWidth, &buttonTextHeight);
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

    void widget_button_push::process_self( gpe::shape_rect * view_space, gpe::shape_rect  * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if(view_space!=NULL && cam!=NULL)
        {
            widget_basic::process_self(view_space,cam);
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

    void widget_button_push::render_self( gpe::shape_rect * view_space, gpe::shape_rect  * cam )
    {
        if( isEnabled)
        {
            int nameSize = (int)widget_name.size();
            view_space = gpe::camera_find(view_space);
            cam = gpe::camera_find(cam);
            if(view_space!=NULL && cam!=NULL)
            {
                gpe::color * renderColor = pawgui::theme_main->button_font_color;

                if( showBackground )
                {
                    if( isClicked )
                    {
                        renderColor = pawgui::theme_main->main_box_faded_font_color;
                        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_box_selected_color,false);
                    }
                    else
                    {
                        if( isHovered)
                        {
                            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_box_highlight_color,false);
                            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_border_highlight_color,true);
                            renderColor = pawgui::theme_main->button_font_highlight_color;

                        }
                        else
                        {
                            gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_box_color,false);
                            renderColor = pawgui::theme_main->button_font_color;
                        }
                    }
                }
                else
                {
                    if( isHovered)
                    {
                        renderColor = pawgui::theme_main->main_box_font_url_hovered_color;
                        gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_border_highlight_color,true);
                    }
                    else if( wasClicked)
                    {
                        renderColor = pawgui::theme_main->main_box_font_url_visited_color;
                    }
                    else
                    {
                        renderColor = pawgui::theme_main->main_box_font_url_color;
                    }
                }

                int iconSize = 0;
                iconSize = std::min(widget_box.w,widget_box.h);

                if( opTexture!=NULL)
                {
                    if( nameSize > maxCharactersToRender )
                    {
                        opTexture->render_tex_resized( widget_box.x-cam->x+iconPadding, widget_box.y-cam->y,iconSize,iconSize, NULL, renderColor);
                        gpe::gfs->render_text( widget_box.x-cam->x+iconSize+iconPadding, widget_box.y-cam->y+widget_box.h/2, stg_ex::get_substring( widget_name, 0, maxCharactersToRender -2)+"...",renderColor,font_buttons,gpe::fa_left,gpe::fa_middle);
                    }
                    else
                    {
                        //abs(widget_box.w-buttonTextWidth)/2+ - iconSize
                        opTexture->render_tex_resized( widget_box.x - cam->x + iconPadding, widget_box.y-cam->y,iconSize,iconSize,NULL,renderColor);
                        if( font_buttons!=NULL)
                        {
                            gpe::gfs->render_text( abs(widget_box.w-buttonTextWidth)/2+widget_box.x-cam->x+iconPadding, widget_box.y-cam->y+widget_box.h/2,widget_name,renderColor,FONT_LABEL,gpe::fa_left,gpe::fa_middle);
                        }
                    }
                }
                else if(font_buttons!=NULL)
                {
                    if( nameSize > maxCharactersToRender )
                    {
                        gpe::gfs->render_text( widget_box.x-cam->x+iconSize+iconPadding, widget_box.y-cam->y+widget_box.h/2,stg_ex::get_substring( widget_name, 0, maxCharactersToRender  -2)+"...",renderColor,font_buttons,gpe::fa_left,gpe::fa_middle);
                    }
                    else
                    {
                        gpe::gfs->render_text( widget_box.x-cam->x+widget_box.w/2, widget_box.y-cam->y+widget_box.h/2,widget_name,renderColor,font_buttons,gpe::fa_center,gpe::fa_middle);
                    }
                }
                if( isInUse)
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_color,true);
                }
            }
        }
    }

    void widget_button_push::set_name(std::string new_name)
    {
        widget_name = new_name;
        prerender_self();
    }

    void widget_button_push::set_website(std::string urlIn)
    {
        webUrl = urlIn;
        wasClicked = false;
        prerender_self();
    }

    ///


    widget_button_card_vertical::widget_button_card_vertical( std::string imgLocation, std::string parsedLines, std::string strValue, int id, int buttonSize)
    {
        usingFlagIcon = false;
        showBackground = false;
        int ww = 0;
        lineHeight = 0;
        if(gpe::font_default !=NULL)
        {
            gpe::font_default->get_numbered_metrics("A",&ww,&lineHeight);
        }
        widget_type = "verticalButton";
        if( buttonSize <=0)
        {
            buttonSize = 128;
        }
        widget_name = parsedLines;

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
        opTexture = rsm_gui->texture_add_filename( imgLocation );
        iconPadding = 8;
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = buttonSize;
        widget_box.h = buttonSize+(int)buttonLines.size() *(padding_default+lineHeight );

        isEnabled = true;
        wasClicked = false;
        webUrl = "";
        prerender_self();
    }

    widget_button_card_vertical::~widget_button_card_vertical()
    {

    }

    int widget_button_card_vertical::get_id()
    {
        return opId;
    }

    void widget_button_card_vertical::prerender_self( )
    {
        /*
        int prevWidth = 0;

        if( gpe::font_default!=NULL)
        {
            int textW = 0;
            int textH = 0;
            gpe::font_default->get_metrics(widget_name,&textW, &textH);
            widget_box.w = iconPadding*3+widget_box.h+textW;
        }
        */
    }

    void widget_button_card_vertical::process_self( gpe::shape_rect * view_space, gpe::shape_rect  * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if(view_space!=NULL && cam!=NULL)
        {
            widget_basic::process_self(view_space,cam);
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

    void widget_button_card_vertical::render_self( gpe::shape_rect * view_space, gpe::shape_rect  * cam )
    {
        if( isEnabled)
        {
            view_space = gpe::camera_find(view_space);
            cam = gpe::camera_find(cam);

            if(view_space!=NULL && cam!=NULL)
            {
                gpe::color * textRenderColor = pawgui::theme_main->button_font_color;
                gpe::color * bgRenderColor = pawgui::theme_main->button_box_color;
                gpe::color * textureRenderColor = gpe::c_white;
                if( isHovered || isInUse)
                {
                    textRenderColor = pawgui::theme_main->button_font_highlight_color;
                }

                if( isClicked )
                {
                    bgRenderColor = pawgui::theme_main->button_box_selected_color;
                    textRenderColor= pawgui::theme_main->button_font_highlight_color;
                }
                else if( isHovered)
                {
                    bgRenderColor = pawgui::theme_main->button_box_highlight_color;
                    textRenderColor= pawgui::theme_main->button_font_highlight_color;
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


                if(gpe::font_default!=NULL)
                {
                    for( int ipLine = 0; ipLine < (int)buttonLines.size(); ipLine++)
                    {
                        gpe::gfs->render_text( widget_box.x+widget_box.w/2-cam->x, widget_box.y-cam->y + widget_box.w+padding_default+(ipLine * (padding_default+lineHeight) ), buttonLines[ipLine], textRenderColor, gpe::font_default, gpe::fa_center, gpe::fa_top );
                    }
                }

                if(  isInUse )
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_border_selected_color,true);
                }
                else if(isHovered)
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_border_highlight_color,true);
                }
            }
        }
    }

    void widget_button_card_vertical::set_name(std::string new_name)
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
        //widget_box.h = padding_default*4+( (int)buttonLines.size() ) * (padding_default+lineHeight);
        //widget_box.h = padding_default+( (int)buttonLines.size() *(lineHeight+padding_default) );
        prerender_self();
    }

    void widget_button_card_vertical::set_website(std::string urlIn)
    {
        webUrl = urlIn;
        wasClicked = false;
        prerender_self();
    }
}
