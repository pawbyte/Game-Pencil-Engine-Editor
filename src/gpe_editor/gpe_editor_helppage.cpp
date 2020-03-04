/*
gpe_editor_helppage.cpp
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

#include "gpe_editor_helppage.h"

gamePencilHelpPageResource * main_HELP_PAGE = NULL;

gamePencilHelpPageResource::gamePencilHelpPageResource()
{
    isFullScreenResource = true;

    resourceFileName = "";
    resource_name = "Help Page";
    parentProjectName = "";
    parentProjectDirectory = "";
    engineLogo = new GPE_Label_Image( "gpe_logo", gpe::app_directory_name+"resources/gfx/textures/game_penciil_logo.png","Game Pencil Engine","http://create.pawbyte.com" );
    engineLogo->resizeAspect = 0.5;
    engineLogo->set_full_width( true );
    sideAreaPanel = new GPE_SelectBoxBasic("Mode");
    sideAreaPanel->set_width(160);
    sideAreaPanel->set_option_height(64);
    sideAreaPanel->add_option("General Help",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/question.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Version Info",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/info.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Changelog",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-text.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Shortkeys",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/keyboard-o.png"),NULL,2, false, false);

    sidePanelRect = new gpe::shape_rect();

    helpPageList = new GPE_GuiElementList();

    //Information Tab
    pencilChangelog = new GPE_TextAreaInputBasic();
    pencilChangelog->set_read_only();
    pencilChangelog->isCodeEditor = false;
    pencilChangelog->clear_all_lines();
    pencilChangelog->import_text( gpe::app_directory_name+"changelog.txt");

    pencilInformation = new GPE_TextAreaInputBasic();
    pencilInformation->set_read_only();
    pencilInformation->isCodeEditor = false;
    pencilInformation->clear_all_lines();
    pencilInformation->add_line("Name:                          Game Pencil Engine");
    std::string versionInfoLine = "";
    if( gpe::version_number_update==0)
    {
        versionInfoLine = "Version:                       "+ stg_ex::float_to_string( gpe::version_number_total )+"0";
    }
    else
    {
        versionInfoLine = "Version:                       "+ stg_ex::float_to_string( gpe::version_number_total );
    }

    if( release_current_mode==relesae_type_production)
    {
        versionInfoLine+=" [PRODUCTION]";
    }
    if( release_current_mode==release_type_rc)
    {
        versionInfoLine+=" [Release Candidate]";
    }
    else if( release_current_mode==release_type_beta)
    {
        versionInfoLine+=" [Beta Version]";
    }
    else if( release_current_mode==release_type_alpha)
    {
        versionInfoLine+=" [Alpha Version]";
    }

    pencilInformation->add_line( versionInfoLine+"." );
    pencilInformation->add_line("SDL Version:                   "+ stg_ex::int_to_string(SDL_MAJOR_VERSION)+"."+ stg_ex::int_to_string(SDL_MINOR_VERSION)+"."+ stg_ex::int_to_string(SDL_PATCHLEVEL)+"." );
    pencilInformation->add_line("Renderer:                      "+ gpe::renderer_main->get_renderer_name() + " - " + gpe::renderer_main->get_renderer_type() + "." );
    pencilInformation->add_line("PGUI Version:                  "+ stg_ex::int_to_string(PGUI_MAJOR_VERSION)+"."+ stg_ex::int_to_string(PGUI_MINOR_VERSION)+"."+ stg_ex::int_to_string(PGUI_PATCHLEVEL)+"." );
    pencilInformation->add_line("GPE(runtime) Version:          "+ stg_ex::int_to_string(gpe::version_nuber_key)+"."+ stg_ex::int_to_string(gpe::version_number_sub)+"."+ stg_ex::int_to_string(gpe::version_number_update)+"." );

    pencilInformation->add_line("Release Date:                  03-18-2020");
    pencilInformation->add_line("Author:                        PawByte");
    pencilInformation->add_line("Lead Developer:                Nathan Hurde aka theweirdn8");
    pencilInformation->add_line("Email:                         help@pawbyte.com");
    pencilInformation->add_line("Website:                       gamepencil.pawbyte.com");
    pencilInformation->isCodeEditor = false;

    updateCheckerButton = new GPE_ToolPushButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/download.png","Download new updates...","Compares latest version to your copy");
    updateCheckerButton->set_website("http://gamepencil.pawbyte.com/get-game-pencil-engine/");

    pencilShortkeys= new GPE_TextAreaInputBasic();
    pencilShortkeys->set_read_only();
    pencilShortkeys->isCodeEditor = false;

    pencilShortkeys->clear_all_lines();
    pencilShortkeys->import_text( gpe::app_directory_name+"shortkeys.txt");
    pencilShortkeys->set_height(640);


    helpPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
    helpPageList->barXMargin = 0;

    helpfulLinksTitle = new GPE_Label_Title("Helpful Links","Some links you might find useful while using GPE.");

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


    //Community
    communityLinksTitle = new GPE_Label_Title("Community","Community links to stay connected");
    GPE_ToolIconButton * tSocialButton = NULL;

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/facebook.png","Facebook" );
    tSocialButton->set_website("https://facebook.com/pawbyte");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/reddit.png","Reddit" );
    tSocialButton->set_website("https://reddit.com/r/gamepencil");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/github.png","GitHub repo");
    tSocialButton->set_website("https://github.com/pawbyte/Game-Pencil-Engine");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/comments.png","Discord Chat Group" );
    tSocialButton->set_website("https://discord.gg/bua3rDW");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/twitter.png","Twitter" );
    tSocialButton->set_website("https://twitter.com/pawbyte");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/youtube.png","YouTube" );
    tSocialButton->set_website("https://www.youtube.com/user/PawByte");
    add_communitylink(tSocialButton);

    tSocialButton = new GPE_ToolIconButton( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/gamecontroller.png","IndieDB" );
    tSocialButton->set_website("http://www.indiedb.com/engines/game-pencil-engine");
    add_communitylink(tSocialButton);
}

gamePencilHelpPageResource::~gamePencilHelpPageResource()
{
    subViewedSpace.x = GENERAL_GPE_GUI_PADDING;
    subViewedSpace.y = GENERAL_GPE_GUI_PADDING;
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
    if( engineLogo!=NULL)
    {
        delete engineLogo;
        engineLogo = NULL;
    }
    if( updateCheckerButton!=NULL)
    {
        delete updateCheckerButton;
        updateCheckerButton = NULL;
    }

    if( helpfulLinksTitle!=NULL)
    {
        delete helpfulLinksTitle;
        helpfulLinksTitle = NULL;
    }

    if( communityLinksTitle!=NULL)
    {
        delete communityLinksTitle;
        communityLinksTitle = NULL;
    }
}

void gamePencilHelpPageResource::add_helpfulbutton(GPE_ToolPushButton * newButton)
{
    if( newButton!=NULL)
    {
        newButton->scale_object( 2 );
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

void gamePencilHelpPageResource::add_communitylink( GPE_ToolIconButton * newLink)
{
    if( newLink!=NULL)
    {
        newLink->scale_object( 2 );
        communityButtons.push_back(newLink);
    }
}

bool gamePencilHelpPageResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

void gamePencilHelpPageResource::prerender_self( )
{

}

void gamePencilHelpPageResource::load_resource(std::string file_path)
{

}

void gamePencilHelpPageResource::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( viewedSpace!=NULL && helpPageList!=NULL)
    {
        int prevTab = sideAreaPanel->get_selection();
        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = viewedSpace->w;
            subViewedSpace.h = viewedSpace->h;
            PANEL_GENERAL_EDITOR->add_gui_element_fullsize( sideAreaPanel );
            PANEL_GENERAL_EDITOR->process_self();
        }
        else
        {
            sideAreaPanel->set_coords(0, 0 );
            sideAreaPanel->set_height( viewedSpace->h );
            sideAreaPanel->process_self( viewedSpace, cam );
            subViewedSpace.x = sideAreaPanel->get_x2pos();
            subViewedSpace.y = 0;
            subViewedSpace.w = viewedSpace->w - sideAreaPanel->get_width();
            subViewedSpace.h = viewedSpace->h;
        }

        if( prevTab!=sideAreaPanel->get_selection() )
        {
            helpPageList->reset_self();
        }

        helpPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
        helpPageList->barYPadding = GENERAL_GPE_GUI_PADDING;
        helpPageList->hAlign = gpe::fa_center;
        helpPageList->clear_list();

        helpPageList->set_coords(subViewedSpace.x,subViewedSpace.y);
        helpPageList->set_width(subViewedSpace.w);
        helpPageList->set_height(subViewedSpace.h );


        if( sideAreaPanel->get_selected_name()=="General Help" || sideAreaPanel->get_selected_name()=="General")
        {
            int subMenuI = 0;
            GPE_TextURL * tempTextUrl = NULL;
            helpPageList->add_gui_element(engineLogo, true );
            helpPageList->add_gui_element(helpfulLinksTitle,true);
            for( subMenuI= 0; subMenuI <(int)helpfulLinks.size(); subMenuI++)
            {
                tempTextUrl = helpfulLinks.at(subMenuI);
                if( tempTextUrl!=NULL)
                {
                    helpPageList->add_gui_element(tempTextUrl, true);
                }
            }
            helpPageList->add_gui_element(communityLinksTitle,true);
            GPE_ToolIconButton * tCommunityButton = NULL;
            int menuLinksTotal = (int)communityButtons.size();
            for( subMenuI= 0; subMenuI < menuLinksTotal; subMenuI++)
            {
                tCommunityButton = communityButtons.at(subMenuI);
                if( tCommunityButton!=NULL)
                {
                    helpPageList->add_gui_element( tCommunityButton, subMenuI == menuLinksTotal-1 );
                }
            }
        }
        else if( sideAreaPanel->get_selected_name()=="Changelog" )
        {
            helpPageList->add_gui_element_fullsize(pencilChangelog );
        }
        else if( sideAreaPanel->get_selected_name()=="Version Info" )
        {
            helpPageList->add_gui_element(engineLogo, true );
            helpPageList->add_gui_element(updateCheckerButton, true );
            helpPageList->add_gui_element(pencilInformation, true );
        }
        else if( sideAreaPanel->get_selected_name()=="Shortkeys" )
        {
            helpPageList->add_gui_element_fullsize(pencilShortkeys );
        }
        helpPageList->process_self(viewedSpace,cam);
    }
}

void gamePencilHelpPageResource::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        if(  sideAreaPanel!=NULL && PANEL_GENERAL_EDITOR==NULL )
        {
            sideAreaPanel->render_self( viewedSpace,cam);
        }
        if( helpPageList!=NULL )
        {
            helpPageList->render_self( viewedSpace,cam);
        }
    }
}

void gamePencilHelpPageResource::save_resource(std::string file_path, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
}

bool gamePencilHelpPageResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    return true;
}
