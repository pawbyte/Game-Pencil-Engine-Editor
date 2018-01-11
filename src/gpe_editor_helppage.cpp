/*
gpe_editor_helppage.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2018 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2018 PawByte.
Copyright (c) 2014-2018 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_editor_helppage.h"

gamePencilHelpPageResource::gamePencilHelpPageResource(GPE_ResourceContainer * pFolder)
{
    resourceFileName = "";
    resourceName = "Help Page";
    parentProjectName = "";
    parentProjectDirectory = "";
    //GPE_Animation * mainMenuSprite = rsm->sprite_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/gpe_main_icons_big.png",70,true,0,0,false);
    projectParentFolder = pFolder;
    renameBox->disable_self();
    confirmResourceButton->disable_self();
    cancelResourceButton->disable_self();
    loadResourceButton->disable_self();
    saveResourceButton->disable_self();

    helpPageList = new GPE_GuiElementList();
    helpPageTabBar = new GPE_TabBar();
    helpPageTabBar->add_new_tab("General Help");
    helpPageTabBar->add_new_tab("Version Info");
    helpPageTabBar->add_new_tab("Changelog");
    helpPageTabBar->add_new_tab("Shortkeys");
    helpPageTabBar->open_tab(0);

    //Information Tab
    pencilChangelog = new GPE_TextAreaInputBasic();
    pencilChangelog->set_read_only();
    pencilChangelog->isCodeEditor = false;
    pencilChangelog->clear_all_lines();
    pencilChangelog->import_text(APP_DIRECTORY_NAME+"changelog.txt");

    pencilInformation = new GPE_TextAreaInputBasic();
    pencilInformation->set_read_only();
    pencilInformation->isCodeEditor = false;
    pencilInformation->clear_all_lines();
    pencilInformation->add_line("Name:                          Game Pencil Engine");
    std::string versionInfoLine = "";
    if( GPE_VERSION_UPDATE_NUMBER==0)
    {
        versionInfoLine = "Version:                       "+float_to_string(GPE_VERSION_DOUBLE_NUMBER)+"0";
    }
    else
    {
        versionInfoLine = "Version:                       "+float_to_string(GPE_VERSION_DOUBLE_NUMBER);
    }
    if( GPE_MAIN_GUI!=NULL)
    {
        if( GPE_MAIN_GUI->editorReleaseType==RELEASE_TYPE_RC)
        {
            versionInfoLine+="[Release Candidate]";
        }
        else if( GPE_MAIN_GUI->editorReleaseType==RELEASE_TYPE_BETA)
        {
            versionInfoLine+="[Beta Version]";
        }
    }
    pencilInformation->add_line( versionInfoLine+"." );
    pencilInformation->add_line("SDL Version:                   "+int_to_string(SDL_MAJOR_VERSION)+"."+int_to_string(SDL_MINOR_VERSION)+"."+int_to_string(SDL_PATCHLEVEL)+"." );
    pencilInformation->add_line("PGUI Version:                  "+int_to_string(PGUI_MAJOR_VERSION)+"."+int_to_string(PGUI_MINOR_VERSION)+"."+int_to_string(PGUI_PATCHLEVEL)+"." );
    pencilInformation->add_line("GPE(runtime) Version:          "+int_to_string(GPR_MAJOR_VERSION)+"."+int_to_string(GPR_MINOR_VERSION)+"."+int_to_string(GPR_PATCHLEVEL)+"." );
    pencilInformation->add_line("Release Date:                  01-05-2018");
    pencilInformation->add_line("Author:                        PawByte");
    pencilInformation->add_line("Lead Developer:                Nathan Hurde aka theweirdn8");
    pencilInformation->add_line("Email:                         help@pawbyte.com");
    pencilInformation->add_line("Website:                       gamepencil.pawbyte.com");
    pencilInformation->isCodeEditor = false;

    updateCheckerButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/download.png","Download new updates...","Compares latest version to your copy");
    updateCheckerButton->set_website("http://gamepencil.pawbyte.com/get-game-pencil-engine/");

    pencilShortkeys= new GPE_TextAreaInputBasic();
    pencilShortkeys->set_read_only();
    pencilShortkeys->isCodeEditor = false;

    pencilShortkeys->clear_all_lines();
    pencilShortkeys->import_text(APP_DIRECTORY_NAME+"shortkeys.txt");
    pencilShortkeys->set_height(640);


    helpPageList->barXPadding = GENERAL_GPE_PADDING;
    helpPageList->barXMargin = 0;

    helpfulButtonsTitle = new GPE_Label_Title("Quick Finds","PawByte social media sites to stay in the loop");
    helpfulLinksTitle = new GPE_Label_Title("Helpful Links","PawByte social media sites to stay in the loop");
    socialMediaTitle = new GPE_Label_Title("Social Media","PawByte social media sites to stay in the loop");
    GPE_TextURL * tHelpfulLink = NULL;

    tHelpfulLink = new GPE_TextURL("Introduction","Introduction","http://gamepencil.pawbyte.com/introduction");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new GPE_TextURL("Community","Community","http://community.pawbyte.com/");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new GPE_TextURL("Documentation","Documentation","http://docs.pawbyte.com/");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new GPE_TextURL("Tutorials","Tutorials","http://gamepencil.pawbyte.com/category/tutorials/");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new GPE_TextURL("Examples","Examples","http://gamepencil.pawbyte.com/examples");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new GPE_TextURL("Demos","Demos","http://gamepencil.pawbyte.com/demos");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new GPE_TextURL("itch.io Page","itch.io Page","https://pawbyte.itch.io/game-pencil-engine");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new GPE_TextURL("Steam Greenlight","Steam Greenlight","http://steamcommunity.com/sharedfiles/filedetails/?id=722454621");
    add_helpfullink(tHelpfulLink);


    //Social Media
    GPE_ToolIconButton * tSocialButton = NULL;

    tSocialButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/facebook.png","Facebook" );
    tSocialButton->set_website("https://facebook.com/pawbyte");
    add_socialmedia_url(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/reddit.png","Reddit" );
    tSocialButton->set_website("https://reddit.com/r/gamepencil");
    add_socialmedia_url(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/github.png","GitHub repo");
    tSocialButton->set_website("https://github.com/pawbyte/Game-Pencil-Engine");
    add_socialmedia_url(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/comments.png","Discord Chat Group" );
    tSocialButton->set_website("https://discord.gg/aNX3Fcx");
    add_socialmedia_url(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/twitter.png","Twitter" );
    tSocialButton->set_website("https://twitter.com/pawbyte");
    add_socialmedia_url(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/youtube.png","YouTube" );
    tSocialButton->set_website("https://www.youtube.com/user/PawByte");
    add_socialmedia_url(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/gamepad.png","IndieDB" );
    tSocialButton->set_website("http://www.indiedb.com/engines/game-pencil-engine");
    add_socialmedia_url(tSocialButton);
}

gamePencilHelpPageResource::~gamePencilHelpPageResource()
{
    subViewedSpace.x = GENERAL_GPE_PADDING;
    subViewedSpace.y = GENERAL_GPE_PADDING;
    subViewedSpace.w = 400;
    subViewedSpace.h = 400;
    resourceType = -2;
    subResouceId = -2;
    parentProjectName = "";

    if( helpPageList!=NULL)
    {
        delete helpPageList;
        helpPageList = NULL;
    }
    if( helpPageTabBar!=NULL)
    {
        delete helpPageTabBar;
        helpPageTabBar = NULL;
    }
    if( updateCheckerButton!=NULL)
    {
        delete updateCheckerButton;
        updateCheckerButton = NULL;
    }

    if( helpfulButtonsTitle!=NULL)
    {
        delete helpfulButtonsTitle;
        helpfulButtonsTitle = NULL;
    }

    if( helpfulLinksTitle!=NULL)
    {
        delete helpfulLinksTitle;
        helpfulLinksTitle = NULL;
    }

    if( socialMediaTitle!=NULL)
    {
        delete socialMediaTitle;
        socialMediaTitle = NULL;
    }
}

void gamePencilHelpPageResource::add_helpfulbutton(GPE_ToolPushButton * newButton)
{
    if( newButton!=NULL)
    {
        helpfulButtons.push_back(newButton);
    }
}

void gamePencilHelpPageResource::add_helpfullink(GPE_TextURL * newLink)
{
    if( newLink!=NULL)
    {
        helpfulLinks.push_back(newLink);
    }
}

void gamePencilHelpPageResource::add_socialmedia_url( GPE_ToolIconButton * newLink)
{
    if( newLink!=NULL)
    {
        socialMediaSites.push_back(newLink);
    }
}


void gamePencilHelpPageResource::prerender_self( )
{

}

void gamePencilHelpPageResource::preprocess_self(std::string alternatePath)
{

}

void gamePencilHelpPageResource::process_self(GPE_Rect * viewedSpace , GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && helpPageList!=NULL)
    {

        helpPageTabBar->set_coords(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING);
        helpPageTabBar->set_width(viewedSpace->w);
        helpPageTabBar->process_self(viewedSpace,cam);

        subViewedSpace.x = GENERAL_GPE_PADDING;
        subViewedSpace.y = helpPageTabBar->get_y2pos()+GENERAL_GPE_PADDING;
        subViewedSpace.w = viewedSpace->w-subViewedSpace.x;
        subViewedSpace.h = viewedSpace->h- subViewedSpace.y;


        helpPageList->set_height(subViewedSpace.h);
        helpPageList->barXPadding = GENERAL_GPE_PADDING*2;
        helpPageList->barXMargin = 0;
        helpPageList->clear_list();

        helpPageList->set_coords(subViewedSpace.x,subViewedSpace.y);
        helpPageList->set_width(subViewedSpace.w);
        helpPageList->set_height(subViewedSpace.h-helpPageTabBar->get_ypos());


        if( helpPageTabBar->get_selected_name()=="General Help")
        {
            int subMenuI = 0;
            GPE_TextURL * tempTextUrl = NULL;
            //helpPageList->add_gui_element(helpfulButtonsTitle,true);
            helpPageList->add_gui_element(helpfulLinksTitle,true);
            for( subMenuI= 0; subMenuI <(int)helpfulLinks.size(); subMenuI++)
            {
                tempTextUrl = helpfulLinks.at(subMenuI);
                if( tempTextUrl!=NULL)
                {
                    helpPageList->add_gui_element(tempTextUrl, true);
                }
            }
            helpPageList->add_gui_element(socialMediaTitle,true);
            GPE_ToolIconButton * tSocialLink = NULL;
            int menuLinksTotal = (int)socialMediaSites.size();
            for( subMenuI= 0; subMenuI < menuLinksTotal; subMenuI++)
            {
                tSocialLink = socialMediaSites.at(subMenuI);
                if( tSocialLink!=NULL)
                {
                    helpPageList->add_gui_element( tSocialLink, subMenuI == menuLinksTotal-1 );
                }
            }
            helpPageList->process_self(viewedSpace,cam);
        }
        else if( helpPageTabBar->get_selected_name()=="Changelog" )
        {
            pencilChangelog->set_width(subViewedSpace.w );
            pencilChangelog->set_height(subViewedSpace.h );
            pencilChangelog->set_coords(subViewedSpace.x,subViewedSpace.y);
            pencilChangelog->process_self(viewedSpace,cam);
        }
        else if( helpPageTabBar->get_selected_name()=="Version Info" )
        {
            pencilInformation->set_width(subViewedSpace.w -64);
            pencilInformation->set_height(256);
            pencilInformation->set_coords(0,0);

            helpPageList->add_gui_element(updateCheckerButton,true);
            helpPageList->add_gui_element(pencilInformation,true);

            helpPageList->process_self(viewedSpace,cam);
        }
        else if( helpPageTabBar->get_selected_name()=="Shortkeys" )
        {
            pencilShortkeys->set_width(subViewedSpace.w );
            pencilShortkeys->set_height(subViewedSpace.h );
            pencilShortkeys->set_coords(subViewedSpace.x,subViewedSpace.y);
            pencilShortkeys->process_self(viewedSpace,cam);
        }
        else
        {
            //helpPageList->add_gui_element(pencilInformation);
            helpPageList->process_self(viewedSpace,cam);
        }
    }
}

void gamePencilHelpPageResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && helpPageList!=NULL)
    {
//        gpe->render_gradient_vertical( &gpeLogoSpace,GPE_MAIN_THEME->Button_Box_Highlighted_Color);
        if( helpPageTabBar->get_selected_name()=="General Help")
        {
            //pencilCredits->render_self( viewedSpace,cam,forceRedraw);
        }
        if( helpPageTabBar->get_selected_name()=="Changelog")
        {
            pencilChangelog->render_self( viewedSpace,cam,forceRedraw);
        }
        else if( helpPageTabBar->get_selected_name()=="Shortkeys" )
        {
            pencilShortkeys->render_self( viewedSpace,cam,forceRedraw);
        }
        else
        {
            helpPageList->render_self( viewedSpace,cam,forceRedraw);
        }

        //render_new_text( gpeLogoSpace.x,gpeLogoSpace.y+gpeLogoSpace.h-GENERAL_GPE_PADDING,"PUBLIC[BETA] - Version 0.2 | Copyright (c)2016 PawByte | make.pawbyte.com",GPE_MAIN_THEME->Main_Box_Faded_Font_Color,DEFAULT_FONT,FA_LEFT,FA_BOTTOM);

        //render_new_text( subViewedSpace.x+GENERAL_GPE_PADDING,subViewedSpace.y+GENERAL_GPE_PADDING,"Recent Projects",GPE_MAIN_THEME->Main_Box_Font_Color,textboxFont,FA_LEFT,FA_TOP);
        helpPageTabBar->render_self( viewedSpace,cam,forceRedraw);
    }
}

void gamePencilHelpPageResource::save_resource(std::string alternatePath, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
}

bool gamePencilHelpPageResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    return true;
}
