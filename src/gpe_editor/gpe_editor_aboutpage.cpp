/*
gpe_editor_aboutpage.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 Game Pencil Engine contributors ( Contributors Page )

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

#include "gpe_editor_aboutpage.h"

gamePencilAboutPageResource * main_about_page = nullptr;

gamePencilAboutPageResource::gamePencilAboutPageResource()
{
    isFullScreenResource = true;
    resourceFileName = "";
    resource_name = "About Page";
    parentProjectName = "";
    parentProjectDirectory = "";

    sideAreaPanel = new pawgui::widget_selectbox("Mode");
    sideAreaPanel->set_width(256);
    sideAreaPanel->set_option_height(32);

    sideAreaPanel->add_option("General Help",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/question.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("Version Info",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/info.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("Changelog",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-text.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("Shortkeys",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/keyboard-o.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("Credits",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/heart.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("License",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-text.png"),nullptr,2, false, false);
    sideAreaPanel->add_option("3rd Party Licenses",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/group.png"),nullptr,2, false, false);

    sidePanelRect = new gpe::shape_rect();

    aboutPageList = new pawgui::widget_panel_list();

    engineLogo = new pawgui::widget_label_image( "Game Pencil Engine", gpe::app_directory_name+"resources/gfx/textures/logo_white.png","Game Pencil Engine","http://create.pawbyte.com" );
    engineLogo->resizeAspect = 0.5;
    engineLogo->set_full_width( true );

    //Information Tab
    pencilChangelog = new pawgui::widget_text_editor();
    pencilChangelog->set_read_only();
    pencilChangelog->isCodeEditor = false;
    pencilChangelog->clear_all_lines();
    pencilChangelog->import_text( gpe::app_directory_name+"changelog.txt");

    pencilInformation = new pawgui::widget_text_editor();
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

    if( gpe::version_is_lts )
    {
        versionInfoLine+=" [LTS]";
    }
    if( release_current_mode==release_type_production)
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
    //pencilInformation->add_line("SDL Version:                   "+ stg_ex::int_to_string(SDL_MAJOR_VERSION)+"."+ stg_ex::int_to_string(SDL_MINOR_VERSION)+"."+ stg_ex::int_to_string(SDL_PATCHLEVEL)+"." );
    pencilInformation->add_line("Renderer:                      "+ gpe::renderer_main->get_renderer_name() + " - " + gpe::renderer_main->get_renderer_type() + "." );
    pencilInformation->add_line("PAWGUI Version:                  "+ stg_ex::int_to_string(pawgui::version_major)+"."+ stg_ex::int_to_string(pawgui::version_minor)+"."+ stg_ex::int_to_string( pawgui::version_patch_level)+"." );
    pencilInformation->add_line("GPE(editor) Version:          "+ stg_ex::int_to_string(editor_version_number_major)+"."+ stg_ex::int_to_string(editor_version_number_minor)+"."+ stg_ex::int_to_string( editor_version_number_update)+"." );
    pencilInformation->add_line("GPE(runtime) Version:          "+ stg_ex::int_to_string(gpe::version_number_major)+"."+ stg_ex::int_to_string(gpe::version_number_minor)+"."+ stg_ex::int_to_string(gpe::version_number_update)+"." );

    pencilInformation->add_line("Release Date:                  01-02-2024");
    pencilInformation->add_line("Author:                        PawByte");
    pencilInformation->add_line("Lead Developer:                Nathan Hurde aka theweirdn8");
    pencilInformation->add_line("Email:                         help@pawbyte.com");
    pencilInformation->add_line("Website:                       gamepencil.pawbyte.com");
    pencilInformation->isCodeEditor = false;

    updateChecker_button = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/download.png","Download new updates...","Compares latest version to your copy");
    updateChecker_button->set_website("http://gamepencil.pawbyte.com/get-game-pencil-engine/");

    pencilCredits = new pawgui::widget_text_editor();
    pencilCredits->isCodeEditor = false;
    pencilCredits->set_read_only();
    pencilCredits->clear_all_lines();
    pencilCredits->load_data("credits.txt");

    pencilDescription = new pawgui::widget_text_editor();
    pencilDescription->set_read_only();
    pencilDescription->isCodeEditor = false;

    pencilDescription->clear_all_lines();
    pencilDescription->import_text( gpe::app_directory_name+"editor_description.txt");
    pencilDescription->set_height(640);

    pencilMissionStatement = new pawgui::widget_text_editor();
    pencilMissionStatement->set_read_only();
    pencilMissionStatement->isCodeEditor = false;

    pencilLicense = new pawgui::widget_text_editor();
    pencilLicense->set_read_only();
    pencilLicense->isCodeEditor = false;

    //pencilLicense->set_read_only();
    pencilLicense->add_line("Copyright (c) 2016-2018 PawByte, Nathan Hurde. All rights reserved..");
    pencilLicense->add_line(".");
    pencilLicense->add_line("Redistribution and use in source and binary forms, with or without");
    pencilLicense->add_line("modification, are permitted provided that the following conditions are");
    pencilLicense->add_line("met:.");
    pencilLicense->add_line(".");
    pencilLicense->add_line("* Redistributions of source code must retain the above copyright");
    pencilLicense->add_line("notice, this list of conditions and the following disclaimer.");
    pencilLicense->add_line("   * Redistributions in binary form must reproduce the above");
    pencilLicense->add_line("copyright notice, this list of conditions and the following disclaimer");
    pencilLicense->add_line("in the documentation and/or other materials provided with the");
    pencilLicense->add_line("distribution..");
    pencilLicense->add_line("   * Neither the name of PawByte. nor the names of its");
    pencilLicense->add_line("team nor its contributors may be used to endorse or promote products derived from");
    pencilLicense->add_line("this software without specific prior written permission..");
    pencilLicense->add_line("");
    pencilLicense->add_line("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS");
    pencilLicense->add_line("'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT");
    pencilLicense->add_line("LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR");
    pencilLicense->add_line("A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT");
    pencilLicense->add_line("OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,");
    pencilLicense->add_line("SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT");
    pencilLicense->add_line("LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,");
    pencilLicense->add_line("DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY.");
    pencilLicense->add_line("THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT");
    pencilLicense->add_line("(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE");
    pencilLicense->add_line("OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.");
    pencilLicense->add_line("");
    pencilLicense->set_height(640);

    thirdPartyLicenses = new pawgui::widget_text_editor();
    thirdPartyLicenses->set_read_only();
    thirdPartyLicenses->isCodeEditor = false;
    thirdPartyLicenses->import_text( gpe::app_directory_name+"third_party_licenses.txt");
    thirdPartyLicenses->set_height(640);

    pencilShortkeys= new pawgui::widget_text_editor();
    pencilShortkeys->set_read_only();
    pencilShortkeys->isCodeEditor = false;

    pencilShortkeys->clear_all_lines();
    pencilShortkeys->import_text( gpe::app_directory_name+"shortkeys.txt");
    pencilShortkeys->set_height(640);

    pawgui::widget_text_url * tHelpfulLink = nullptr;

    tHelpfulLink = new pawgui::widget_text_url("Introduction","Introduction","http://gamepencil.pawbyte.com/introduction");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new pawgui::widget_text_url("Community","Community","http://community.pawbyte.com/");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new pawgui::widget_text_url("Documentation","Documentation","http://docs.pawbyte.com/");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new pawgui::widget_text_url("Tutorials","Tutorials","http://gamepencil.pawbyte.com/category/tutorials/");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new pawgui::widget_text_url("Examples","Examples","http://gamepencil.pawbyte.com/examples");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new pawgui::widget_text_url("Demos","Demos","http://gamepencil.pawbyte.com/demos");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new pawgui::widget_text_url("itch.io Page","itch.io Page","https://pawbyte.itch.io/game-pencil-engine");
    add_helpfullink(tHelpfulLink);

    tHelpfulLink = new pawgui::widget_text_url("Steam Greenlight","Steam Greenlight","http://steamcommunity.com/sharedfiles/filedetails/?id=722454621");
    add_helpfullink(tHelpfulLink);

    helpTabSection = new pawgui::widget_panel_section("Helpful Links");


    for( int subMenuI= 0; subMenuI <(int)helpfulLinks.size(); subMenuI++)
    {
        tHelpfulLink = helpfulLinks.at(subMenuI);
        if( tHelpfulLink!=nullptr)
        {
            helpTabSection->add_widget( tHelpfulLink, true );
        }
    }

    //Community
    communityLinksTitle = new pawgui::widget_label_title("Community","Community links to stay connected");
    pawgui::widget_button_icon * tSocial_button = nullptr;

    tSocial_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/facebook.png","Facebook" );
    tSocial_button->set_website("https://facebook.com/pawbyte");
    add_communitylink(tSocial_button);

    tSocial_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/reddit.png","Reddit" );
    tSocial_button->set_website("https://reddit.com/r/gamepencil");
    add_communitylink(tSocial_button);

    tSocial_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/github.png","GitHub repo");
    tSocial_button->set_website("https://github.com/pawbyte/Game-Pencil-Engine");
    add_communitylink(tSocial_button);

    tSocial_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/comments.png","Discord Chat Group" );
    tSocial_button->set_website("https://discord.gg/bua3rDW");
    add_communitylink(tSocial_button);

    tSocial_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/twitter.png","Twitter" );
    tSocial_button->set_website("https://twitter.com/pawbyte");
    add_communitylink(tSocial_button);

    tSocial_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/youtube.png","YouTube" );
    tSocial_button->set_website("https://www.youtube.com/user/PawByte");
    add_communitylink(tSocial_button);

    tSocial_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/gamecontroller.png","IndieDB" );
    tSocial_button->set_website("http://www.indiedb.com/engines/game-pencil-engine");
    add_communitylink(tSocial_button);

    aboutPageList->barXPadding = pawgui::padding_default;
    aboutPageList->barXMargin = 0;
}

gamePencilAboutPageResource::~gamePencilAboutPageResource()
{
    subViewedSpace.x = pawgui::padding_default;
    subViewedSpace.y = pawgui::padding_default;
    subViewedSpace.w = 400;
    subViewedSpace.h = 400;
    resourceType = -2;
    subResouceId = -2;
    parentProjectName = "";

    if( aboutPageList!=nullptr)
    {
        delete aboutPageList;
        aboutPageList = nullptr;
    }
    if( pencilCredits!=nullptr)
    {
        delete pencilCredits;
        pencilCredits = nullptr;
    }
    if( pencilDescription!=nullptr)
    {
        delete pencilDescription;
        pencilDescription = nullptr;
    }
    if( pencilLicense!=nullptr)
    {
        delete pencilLicense;
        pencilLicense = nullptr;
    }
    if( thirdPartyLicenses!=nullptr)
    {
        delete thirdPartyLicenses;
        thirdPartyLicenses = nullptr;
    }
    if( pencilMissionStatement!=nullptr)
    {
        delete pencilMissionStatement;
        pencilMissionStatement = nullptr;
    }
       if( aboutPageList!=nullptr)
    {
        delete aboutPageList;
        aboutPageList = nullptr;
    }
    if( engineLogo!=nullptr)
    {
        delete engineLogo;
        engineLogo = nullptr;
    }
    if( updateChecker_button!=nullptr)
    {
        delete updateChecker_button;
        updateChecker_button = nullptr;
    }

    if( communityLinksTitle!=nullptr)
    {
        delete communityLinksTitle;
        communityLinksTitle = nullptr;
    }
}

void gamePencilAboutPageResource::add_helpfulbutton(pawgui::widget_button_push * new_button)
{
    if( new_button!=nullptr)
    {
        new_button->scale_object( 2 );
        helpful_buttons.push_back(new_button);
    }
}

void gamePencilAboutPageResource::add_helpfullink(pawgui::widget_text_url * newLink)
{
    if( newLink!=nullptr)
    {
        helpfulLinks.push_back(newLink);
    }
}

void gamePencilAboutPageResource::add_communitylink( pawgui::widget_button_icon * newLink)
{
    if( newLink!=nullptr)
    {
        newLink->scale_object( 2 );
        community_buttons.push_back(newLink);
    }
}

bool gamePencilAboutPageResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true; //WIPNOTFUNCTIONALYET
}

void gamePencilAboutPageResource::prerender_self( )
{

}

void gamePencilAboutPageResource::load_resource(std::string file_path)
{

}

void gamePencilAboutPageResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( view_space!=nullptr && aboutPageList!=nullptr)
    {
        int prevTab = sideAreaPanel->get_selection();
        if( panel_main_editor!=nullptr )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w;
            subViewedSpace.h = view_space->h;
            panel_main_editor->add_gui_element_fullsize( sideAreaPanel );
            panel_main_editor->process_self();

        }
        else
        {
            sideAreaPanel->set_coords(0, 0 );
            sideAreaPanel->set_height( view_space->h );
            sideAreaPanel->process_self( view_space, cam );
            subViewedSpace.x = sideAreaPanel->get_x2();
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w - sideAreaPanel->get_width();
            subViewedSpace.h = view_space->h;
        }

        if( prevTab!=sideAreaPanel->get_selection() )
        {
            aboutPageList->reset_self();
        }

        aboutPageList->alignment_h = gpe::fa_center;
        aboutPageList->set_coords(subViewedSpace.x,subViewedSpace.y );
        aboutPageList->set_width(subViewedSpace.w);
        aboutPageList->set_height(subViewedSpace.h );
        aboutPageList->barXPadding = pawgui::padding_default;
        aboutPageList->barXMargin = subViewedSpace.w/8;

        aboutPageList->clear_list();
        if( sideAreaPanel->get_selected_name()=="General Help" || sideAreaPanel->get_selected_name()=="General")
        {
            int subMenuI = 0;
            pawgui::widget_text_url * tempTextUrl = nullptr;
            aboutPageList->add_gui_element(engineLogo, true );
            aboutPageList->add_gui_section( helpTabSection );

            aboutPageList->add_gui_element(communityLinksTitle,true);
            pawgui::widget_button_icon * tCommunity_button = nullptr;
            int menuLinksTotal = (int)community_buttons.size();
            for( subMenuI= 0; subMenuI < menuLinksTotal; subMenuI++)
            {
                tCommunity_button = community_buttons.at(subMenuI);
                if( tCommunity_button!=nullptr)
                {
                    aboutPageList->add_gui_element( tCommunity_button, subMenuI == menuLinksTotal-1 );
                }
            }
        }
        else if( sideAreaPanel->get_selected_name()=="Changelog" )
        {
            aboutPageList->add_gui_element_fullsize(pencilChangelog );
        }
        else if( sideAreaPanel->get_selected_name()=="Version Info" )
        {
            aboutPageList->add_gui_element(engineLogo, true );
            aboutPageList->add_gui_element(updateChecker_button, true );
            aboutPageList->add_gui_element(pencilInformation, true );
        }
        else if( sideAreaPanel->get_selected_name()=="Shortkeys" )
        {
            aboutPageList->add_gui_element_fullsize(pencilShortkeys );
        }
        else if( sideAreaPanel->get_selected_name()=="Credits")
        {
            aboutPageList->add_gui_element_fullsize(pencilCredits);
        }
        else if( sideAreaPanel->get_selected_name()=="Description" )
        {
            aboutPageList->add_gui_element_fullsize(pencilDescription);
        }
        else if( sideAreaPanel->get_selected_name()=="License" )
        {
            aboutPageList->add_gui_element_fullsize(pencilLicense);
        }
        else if( sideAreaPanel->get_selected_name()=="3rd Party Licenses" )
        {
            aboutPageList->add_gui_element_fullsize(thirdPartyLicenses);
        }
        aboutPageList->process_self(view_space,cam);
    }
}

void gamePencilAboutPageResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( cam!=nullptr && view_space!=nullptr)
    {
        if( sideAreaPanel!=nullptr && panel_main_editor==nullptr )
        {
            sideAreaPanel->render_self( view_space,cam);
        }
        if( aboutPageList!=nullptr )
        {
            aboutPageList->render_self( view_space,cam);
        }
    }
}

void gamePencilAboutPageResource::save_resource(std::string file_path, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
}

bool gamePencilAboutPageResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    return true;
}
