/*
pawgui_buttons.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
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
        if( buttonTexture == nullptr)
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
        if( buttonTexture!=nullptr)
        {
            return widget_type+":"+widget_name+"==|||=="+ stg_ex::int_to_string(widget_box.w)+","+buttonTexture->get_filename()+",,,"+stg_ex::int_to_string(widget_box.w)+","+stg_ex::int_to_string(widget_box.h)+","+stg_ex::int_to_string(opId);
        }
        else
        {
            return widget_type+":"+widget_name+"==|||=="+ stg_ex::int_to_string(widget_box.w)+",0,,,0,0,"+ stg_ex::int_to_string(opId);
        }
    }

    void widget_button_icon::load_data(std::string datastring)
    {
        int buttonSize = stg_ex::split_first_int(datastring,',');
        if( buttonSize <=0)
        {
            widget_box.w = buttonSize;
            widget_box.h = buttonSize;
        }
        std::string texture_file_name = stg_ex::split_first_string(datastring,",,,");
        int animationWidth = stg_ex::split_first_int(datastring,',');
        int animationHeight = stg_ex::split_first_int(datastring,',');
        widget_box.w = animationWidth;
        widget_box.h = animationHeight;
        opId = stg_ex::split_first_int(datastring,',');
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
        if( cam!=nullptr && view_space!=nullptr)
        {
            if( buttonTexture!=nullptr)
            {
                gpe::color * render_color = pawgui::theme_main->icon_font_color;

                if(isTabbed)
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->icon_box_selected_color,false);
                    render_color = pawgui::theme_main->icon_font_selected_color;
                }
                else if( isInUse)
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->icon_box_highlight_color,false);
                    render_color = pawgui::theme_main->icon_font_highlight_color;
                }

                else if(isHovered)
                {
                    gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->icon_box_highlight_color,false);
                    render_color = pawgui::theme_main->icon_font_highlight_color;
                }

                buttonTexture->render_tex_resized( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.h,widget_box.h,nullptr,render_color);
            }
            if( endsSection)
            {
                //gpe::gcanvas->render_rectangle( widget_box.x+widget_box.w-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w+1-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->icon_font_color);
            }
        }
    }

    void widget_button_icon::set_website(std::string url_string)
    {
        webUrl = url_string;
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
        new_buttonXPos = barPadding;
        isTabBar = useTabs;
        tabPosition = 0;
        widthAutoResizes = true;
    }

    widget_button_iconbar::~widget_button_iconbar()
    {
        for( int i = barOptions.size()-1; i >=0; i--)
        {
            if( barOptions[i]!=nullptr)
            {
                delete barOptions[i];
                barOptions[i] = nullptr;
                barOptions.erase(barOptions.begin()+i);
            }
        }
        barOptions.clear();
    }

    std::string widget_button_iconbar::get_data()
    {
        return "";
    }

    void widget_button_iconbar::load_data(std::string datastring)
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

        new_buttonXPos+= xPadding+widget_box.h;
        if( endsSection)
        {
            new_buttonXPos+=xPadding;
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
            if( barOptions[tabPosition]!=nullptr)
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
        widget_button_icon * c_button = nullptr;
        widget_button_icon * d_button = nullptr;
        int buttonsItr=0;
        int other_buttonsItr=0;

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
        int new_buttonXPos = widget_box.x;
        for( buttonsItr =0;  buttonsItr<(int)barOptions.size(); buttonsItr++ )
        {
            c_button = barOptions[buttonsItr];
            if(c_button!=nullptr)
            {
                if( !isTabBar)
                {
                    c_button->set_clicked(false);
                    c_button->switch_inuse(false);
                }
                c_button->set_coords( new_buttonXPos, widget_box.y );
                c_button->process_self(view_space, cam);
                if( c_button->is_clicked()  )
                {
                    selectedOption = c_button->get_id();
                    if( isTabBar)
                    {
                        tabPosition = buttonsItr;
                        for( other_buttonsItr=0; other_buttonsItr <(int)barOptions.size(); other_buttonsItr++)
                        {
                            d_button = barOptions[other_buttonsItr];
                            if( d_button!=nullptr )
                            {
                                d_button->isTabbed = false;
                            }
                        }
                        c_button->isTabbed = true;
                    }
                }
                else if( c_button->is_hovered() )
                {
                    main_overlay_system->update_tooltip( c_button->descriptionText );
                }
                new_buttonXPos+= xPadding+widget_box.h;
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
                    c_button = barOptions[buttonsItr];
                    if(c_button!=nullptr)
                    {
                        if( buttonsItr==tabPosition )
                        {
                            c_button->isTabbed =  true;
                            selectedOption = c_button->get_id();
                        }
                        else
                        {
                            c_button->isTabbed =  false;
                            c_button->set_clicked( false );
                            c_button->switch_inuse( false );
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
            widget_button_icon * c_button = nullptr;
            for(int i=0; i< (int)barOptions.size(); i++)
            {
                c_button = barOptions[i];
                if(c_button!=nullptr)
                {
                    c_button->render_self( view_space,cam);
                }
            }
            if( isInUse)
            {
                //gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->main_box_font_url_color,true);
            }
            //gpe::gcanvas->render_rectangle( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x+widget_box.w-cam->x,widget_box.y+widget_box.h-cam->y,pawgui::theme_main->popup_box_border_color,true);
        }
    }

    void widget_button_iconbar::set_coords(int x_new, int y_new)
    {
        int pastX = widget_box.x;
        int pastY = widget_box.y;
        widget_basic::set_coords(x_new,y_new);
        if( widget_box.x!=pastX || widget_box.y!=pastY)
        {
            widget_button_icon * c_button = nullptr;
            int xNewPos = widget_box.x+xPadding;
            for( int i = 0; i < (int)barOptions.size(); i++)
            {
                c_button = barOptions[i];
                if( c_button!=nullptr)
                {
                    c_button->set_coords(xNewPos, widget_box.y);
                    c_button->set_height( widget_box.h);
                    xNewPos+=xPadding+widget_box.h;
                    if( c_button->ends_section() )
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


    void widget_button_iconbar::set_height(int new_height)
    {
        int pastH = widget_box.h;
        if( widget_box.h!=new_height)
        {
            widget_box.h = new_height;
            new_buttonXPos = xPadding;
            widget_button_icon * c_button = nullptr;
            for( int i = 0; i < (int)barOptions.size(); i++)
            {
                c_button = barOptions[i];
                if( c_button!=nullptr)
                {
                    c_button->set_coords(widget_box.x+new_buttonXPos,widget_box.y);
                    c_button->set_width(new_height);
                    c_button->set_height(new_height);
                    new_buttonXPos+=widget_box.h;
                    if( c_button->ends_section() )
                    {
                        new_buttonXPos+=xPadding;
                    }
                }
            }
            recalculate_width();
        }
    }

    void widget_button_iconbar::set_selection(int newSelection)
    {
        widget_button_icon * c_button = nullptr;
        bool foundValue = false;
        for(int i=0; i<(int)barOptions.size(); i++)
        {
            c_button = barOptions[i];
            if( c_button!=nullptr )
            {
                if( c_button->get_id()==newSelection )
                {
                    tabPosition = i;
                    if( !foundValue )
                    {
                        c_button->switch_inuse( true );
                        c_button->isTabbed = true;
                        foundValue = true;
                    }
                }
                else
                {
                    c_button->switch_inuse( false );
                    c_button->isTabbed = false;
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
        if( gpe::font_default!=nullptr)
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
        if(cam!=nullptr && view_space!=nullptr)
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
            if(gpe::font_default!=nullptr)
            {
                if( isHovered)
                {
                    gpe::gfs->render_text( widget_box.x-cam->x+widget_box.w/2, widget_box.y-cam->y+widget_box.h/2,widget_name,pawgui::theme_main->button_font_highlight_color,gpe::font_default,gpe::fa_center,gpe::fa_middle);
                }
                else
                {
                    gpe::gfs->render_text( widget_box.x-cam->x+widget_box.w/2, widget_box.y-cam->y+widget_box.h/2,widget_name,pawgui::theme_main->button_font_color,gpe::font_default,gpe::fa_center,gpe::fa_middle);
                }
                /*if( widget_box.w > opstringTexture->get_width()+padding_default*2 )
                {
                    opstringTexture->render_tex( widget_box.x-cam->x+(widget_box.w-opstringTexture->get_width() )/2,widget_box.y-cam->y+padding_default,nullptr);
                }
                else
                {
                    opstringTexture->render_tex( widget_box.x-cam->x+padding_default,widget_box.y-cam->y+padding_default,nullptr);
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
        if( gpe::font_default!=nullptr)
        {
            int textW = 0;
            int textH = 0;
            gpe::font_default->get_metrics(new_name,&textW, &textH);
            //opstringTexture->loadFromRenderedText(gpe::renderer_main,new_name,pawgui::theme_main->main_box_font_color,gpe::font_default);
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
        if( font_buttons!=nullptr && widget_box.w > 32)
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
        if(view_space!=nullptr && cam!=nullptr)
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
            if(view_space!=nullptr && cam!=nullptr)
            {
                gpe::color * render_color = pawgui::theme_main->button_font_color;

                if( showBackground )
                {
                    if( isClicked )
                    {
                        render_color = pawgui::theme_main->main_box_faded_font_color;
                        gpe::gcanvas->render_roundrect_filled_color( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_box_selected_color, 255);
                    }
                    else
                    {
                        if( isHovered)
                        {
                            gpe::gcanvas->render_roundrect_filled_color( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_box_highlight_color,255);
                            gpe::gcanvas->render_roundrect_filled_color( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_border_highlight_color,255 );
                            render_color = pawgui::theme_main->button_font_highlight_color;

                        }
                        else
                        {
                            gpe::gcanvas->render_roundrect_filled_color( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_box_color,255);
                            render_color = pawgui::theme_main->button_font_color;
                        }
                    }
                }
                else
                {
                    if( isHovered)
                    {
                        render_color = pawgui::theme_main->main_box_font_url_hovered_color;
                        gpe::gcanvas->render_roundrect_filled_color( widget_box.x-cam->x,widget_box.y-cam->y,widget_box.x-cam->x+widget_box.w,widget_box.y-cam->y+widget_box.h,pawgui::theme_main->button_border_highlight_color,255 );
                    }
                    else if( wasClicked)
                    {
                        render_color = pawgui::theme_main->main_box_font_url_visited_color;
                    }
                    else
                    {
                        render_color = pawgui::theme_main->main_box_font_url_color;
                    }
                }

                int iconSize = 0;
                iconSize = std::min(widget_box.w,widget_box.h);

                if( opTexture!=nullptr)
                {
                    if( nameSize > maxCharactersToRender )
                    {
                        opTexture->render_tex_resized( widget_box.x-cam->x+iconPadding, widget_box.y-cam->y,iconSize,iconSize, nullptr, render_color);
                        gpe::gfs->render_text( widget_box.x-cam->x+iconSize+iconPadding, widget_box.y-cam->y+widget_box.h/2, stg_ex::get_substring( widget_name, 0, maxCharactersToRender -2)+"...",render_color,font_buttons,gpe::fa_left,gpe::fa_middle);
                    }
                    else
                    {
                        //abs(widget_box.w-buttonTextWidth)/2+ - iconSize
                        opTexture->render_tex_resized( widget_box.x - cam->x + iconPadding, widget_box.y-cam->y,iconSize,iconSize,nullptr,render_color);
                        if( font_buttons!=nullptr)
                        {
                            gpe::gfs->render_text( abs(widget_box.w-buttonTextWidth)/2+widget_box.x-cam->x+iconPadding, widget_box.y-cam->y+widget_box.h/2,widget_name,render_color,FONT_LABEL,gpe::fa_left,gpe::fa_middle);
                        }
                    }
                }
                else if(font_buttons!=nullptr)
                {
                    if( nameSize > maxCharactersToRender )
                    {
                        gpe::gfs->render_text( widget_box.x-cam->x+iconSize+iconPadding, widget_box.y-cam->y+widget_box.h/2,stg_ex::get_substring( widget_name, 0, maxCharactersToRender  -2)+"...",render_color,font_buttons,gpe::fa_left,gpe::fa_middle);
                    }
                    else
                    {
                        gpe::gfs->render_text( widget_box.x-cam->x+widget_box.w/2, widget_box.y-cam->y+widget_box.h/2,widget_name,render_color,font_buttons,gpe::fa_center,gpe::fa_middle);
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

    void widget_button_push::set_website(std::string url_string)
    {
        webUrl = url_string;
        wasClicked = false;
        prerender_self();
    }

    ///


    widget_button_card_vertical::widget_button_card_vertical( std::string imgLocation, std::string parsedLines, std::string strValue, int id, int buttonSize)
    {
        autoResizes = false;
        usingFlagIcon = false;
        showBackground = false;
        int ww = 0;
        lineHeight = 0;
        if(gpe::font_default !=nullptr)
        {
            gpe::font_default->get_numbered_metrics("A",&ww,&lineHeight);
        }
        widget_type = "vertical_button";
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
                std::string cbNLstring = stg_ex::split_first_string(parsedLines,'\n');
                buttonLines.push_back(cbNLstring);
                newLinePos=parsedLines.find_first_of('\n');
                while (newLinePos!=(int)std::string::npos)
                {
                    cbNLstring = stg_ex::split_first_string(parsedLines,'\n');
                    newLinePos=parsedLines.find_first_of('\n');
                    buttonLines.push_back(cbNLstring);
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
        else
        {
            buttonLines.push_back( "Project..." );
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

        if( gpe::font_default!=nullptr)
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
        if(view_space!=nullptr && cam!=nullptr)
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

            if(view_space!=nullptr && cam!=nullptr)
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



                if( opTexture!=nullptr)
                {
                    if( usingFlagIcon)
                    {
                        opTexture->render_tex_resized( widget_box.x-cam->x, widget_box.y-cam->y,widget_box.w,widget_box.w,nullptr,textRenderColor );
                    }
                    else
                    {
                        opTexture->render_tex_resized( widget_box.x-cam->x, widget_box.y-cam->y,widget_box.w,widget_box.w,nullptr, textureRenderColor );
                    }
                }


                if(gpe::font_default!=nullptr)
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
                std::string cbNLstring = stg_ex::split_first_string(new_name,'\n');
                buttonLines.push_back(cbNLstring);
                newLinePos=new_name.find_first_of('\n');
                while (newLinePos!=(int)std::string::npos)
                {
                    buttonLines.push_back(cbNLstring);
                    cbNLstring = stg_ex::split_first_string(new_name,'\n');
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

    void widget_button_card_vertical::set_website(std::string url_string)
    {
        webUrl = url_string;
        wasClicked = false;
        prerender_self();
    }
}
