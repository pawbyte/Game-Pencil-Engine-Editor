/*
gpe_editor_aboutpage.cpp
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

#include "gpe_editor_aboutpage.h"

gamePencilAboutPageResource * main_ABOUT_PAGE = NULL;

gamePencilAboutPageResource::gamePencilAboutPageResource()
{
    isFullScreenResource = true;
    resourceFileName = "";
    resource_name = "About Page";
    parentProjectName = "";
    parentProjectDirectory = "";

    sideAreaPanel = new GPE_SelectBoxBasic("Mode");
    sideAreaPanel->set_width(256);
    sideAreaPanel->set_option_height(64);
    sideAreaPanel->add_option("Credits",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/heart.png"),NULL,2, false, false);
    sideAreaPanel->add_option("License",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-text.png"),NULL,2, false, false);
    sideAreaPanel->add_option("3rd Party Licenses",-1,paw_gui_rsm->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/group.png"),NULL,2, false, false);

    sidePanelRect = new gpe::shape_rect();

    aboutPageList = new GPE_GuiElementList();

    pencilCredits = new GPE_TextAreaInputBasic();
    pencilCredits->isCodeEditor = false;
    pencilCredits->set_read_only();
    pencilCredits->clear_all_lines();
    pencilCredits->add_line("PawByte Development Team");
    pencilCredits->add_line("-----------------------------------");
    pencilCredits->add_line("Nathan Hurde:               Lead IDE & Engine Developer");
    pencilCredits->add_line("Chase Lee:                  Lead Web Developer & Color Theme Author");
    pencilCredits->add_line("Karl Cons:                  Alpha testing of Version 0.2");

    pencilCredits->add_line("");
    pencilCredits->add_line("");
    pencilCredits->add_line("Community Contributors (Ordered Alphabetically):");
    pencilCredits->add_line("-----------------------------------");
    pencilCredits->add_line("Anthony Manetti:            Private Alpha testing of Version 0.2");
    pencilCredits->add_line("Clint R Bellenger:          Technical advising and help with input systems");
    pencilCredits->add_line("Daniel Gump:                Technical advising and help with HTML5 engine optimizations");
    pencilCredits->add_line("@deadda7a:                  Helped with creation of new color theming for Version 1.0.1 and beyond.");
    pencilCredits->add_line("@GTMalachiasV:              Critical feedback and testing for improving Project Browser");
    pencilCredits->add_line("@JeZxLee:                   Located Launcher error for files with spaces/symbols in their file name.");
    pencilCredits->add_line("Mac Carter:                 Usage of Mac to create & test OSX build.");
    pencilCredits->add_line("Russell Johnston:           Technical advising and help with syntax highlighting system");
    pencilCredits->add_line("Tim C( timchoh585):         Updated README.md to a professional level");
    pencilCredits->add_line("Toyam Cox( Vaelatern):      Created initial makefile");
    pencilCredits->add_line("YellowAfterLife:            Technical advising and help with collision systems");

    pencilCredits->add_line("");
    pencilCredits->add_line("");
    pencilCredits->add_line("Artwork (Ordered Alphabetically):");
    pencilCredits->add_line("-----------------------------------");
    pencilCredits->add_line("David Gandy:               Creator of Font Aesome [ https://twitter.com/davegandy ]");
    pencilCredits->add_line("Damian Kaczmarek           Font-Awesome-SVG-PNG Author [ rush@rushbase.net @Rush ]");
    pencilCredits->add_line("Dominykas Blyžė            Font-Awesome-SVG-PNG Author [ hello@dominykas.com @dominykas]");
    pencilCredits->add_line("@deadda7a:                 Created 'Game Pencil' logo");
    pencilCredits->add_line("Font Awesome:              Icons and Buttons [ http://fontawesome.io/ ]");
    pencilCredits->add_line("The Font Awesome font is licensed under the SIL OFL 1.1:");
    pencilCredits->add_line("http://scripts.sil.org/OFL  ");
    pencilCredits->add_line("Font-Awesome-SVG-PNG is licensed under the MIT license  ");


    pencilCredits->add_line("");
    pencilCredits->add_line("");
    pencilCredits->add_line("Technologies (Ordered Alphabetically):");
    pencilCredits->add_line("-----------------------------------");
    pencilCredits->add_line("Electron[Used for Desktop Exports]:            http://electron.atom.io/");
    pencilCredits->add_line("IsImageOk:                                     http://blog.sajithmr.me/javascript-check-an-image-is-loaded-or-not/");
    pencilCredits->add_line("JavaScript FileReader:                         http://stackoverflow.com/questions/23331546/how-to-use-javascript-to-read-local-text-file-and-read-line-by-line");
    pencilCredits->add_line("RequestFrameAnimation:                         http://www.paulirish.com/2011/requestanimationframe-for-smart-animating/");
    pencilCredits->add_line("Simple DirectMedia Layer(SDL):                 http://www.libsdl.org");
    pencilCredits->add_line("SDL_GFX:                                       http://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/");
    pencilCredits->add_line("fps_cap & FPS_Calculator functions:            https://davidgow.net/handmadepenguin/ch18.html/");


    pencilCredits->add_line("");
    pencilCredits->add_line("Patreon Backers (Ordered Alphabetically):");
    pencilCredits->add_line("-----------------------------------");
    pencilCredits->add_line("     Feral Monk");
    pencilCredits->add_line("     James Thomas");
    pencilCredits->add_line("     James Thomas");
    pencilCredits->add_line("     Joseph Yarrow");
    pencilCredits->add_line("     Mark Medrano");
    pencilCredits->add_line("     Keith Thomas");
    pencilCredits->add_line("     Noel Hurde");
    pencilCredits->add_line("     Thomas Ingham");
    pencilCredits->add_line("     Roshin Varghese");

    pencilCredits->add_line("Itch.io Supporters (Ordered Alphabetically):");
    pencilCredits->add_line("-----------------------------------");
    pencilCredits->add_line("     Aisha Halim");
    pencilCredits->add_line("     Alessandro Montoli ( funkygallo)");
    pencilCredits->add_line("     _Auron_");
    pencilCredits->add_line("     bojjenclon");
    pencilCredits->add_line("     CammyGamesYT");
    pencilCredits->add_line("     Chad Davis");
    pencilCredits->add_line("     DarkLightGames");
    pencilCredits->add_line("     Davey13");
    pencilCredits->add_line("     evilemprzurg");
    pencilCredits->add_line("     F. Keitz");
    pencilCredits->add_line("     Florian Zwerina");
    pencilCredits->add_line("     Goldenkitty");
    pencilCredits->add_line("     Goldenxp");
    pencilCredits->add_line("     jarrett9999");
    pencilCredits->add_line("     Mark Henning");
    pencilCredits->add_line("     microrutter");
    pencilCredits->add_line("     M. Moy");
    pencilCredits->add_line("     Kat Leopardess");
    pencilCredits->add_line("     popcade");
    pencilCredits->add_line("     raymond13557");
    pencilCredits->add_line("     RookTKO");
    pencilCredits->add_line("     Samson194");
    pencilCredits->add_line("     TH");
    pencilCredits->add_line("     Veron Hurde Jr.");
    pencilCredits->add_line("     Woody Stanfield");
    pencilCredits->add_line("     Zonemaster");



    pencilCredits->add_line("");
    pencilCredits->add_line("KickStarter Backers (Ordered Alphabetically):");
    pencilCredits->add_line("-----------------------------------");

    pencilCredits->add_line("     Aisha Halim");
    pencilCredits->add_line("     Aleksander Kwitakowski");
    pencilCredits->add_line("     Alex King");
    pencilCredits->add_line("     Alyssia Ashkevron");
    pencilCredits->add_line("     Arturas Norkus");
    pencilCredits->add_line("     Blair Johnson");
    pencilCredits->add_line("     Christopher Murt");
    pencilCredits->add_line("     Christopher Pope");
    pencilCredits->add_line("     Climb Fitness");
    pencilCredits->add_line("     Curtis Hurde");
    pencilCredits->add_line("     Daniel Stempniewicz");
    pencilCredits->add_line("     David Goldsmith");
    pencilCredits->add_line("     Derek Lawson");
    pencilCredits->add_line("     James Donachie");
    pencilCredits->add_line("     Giacomo Russo");
    pencilCredits->add_line("     Trevor Hayes");
    pencilCredits->add_line("     Godewijn Perizonius");
    pencilCredits->add_line("     Greg Polander");
    pencilCredits->add_line("     HBComics");
    pencilCredits->add_line("     Henri Niva");
    pencilCredits->add_line("     Jake Miller");
    pencilCredits->add_line("     James Bowling");
    pencilCredits->add_line("     Jason Mason Pierce");
    pencilCredits->add_line("     Jason Hornbuckle");
    pencilCredits->add_line("     Jeff Hanson");
    pencilCredits->add_line("     Joel A. Luellwitz");
    pencilCredits->add_line("     Johan Brodd");
    pencilCredits->add_line("     John Isakisson");
    pencilCredits->add_line("     John Lerchen");
    pencilCredits->add_line("     John Robertson");
    pencilCredits->add_line("     Johnathon Stevens");
    pencilCredits->add_line("     Jonathan Chang");
    pencilCredits->add_line("     Jose P. Zagal");
    pencilCredits->add_line("     Joshua Boren");
    pencilCredits->add_line("     jwilloug");
    pencilCredits->add_line("     Kelly Samuels");
    pencilCredits->add_line("     kuroiXiru");
    pencilCredits->add_line("     Pawel Sowizral");
    pencilCredits->add_line("     Lukasz Stempniewicz");
    pencilCredits->add_line("     Eden Caldas");
    pencilCredits->add_line("     Markus Løtveit");
    pencilCredits->add_line("     Matthew Schie");
    pencilCredits->add_line("     Matthew Sutherland");
    pencilCredits->add_line("     Max Juchheim");
    pencilCredits->add_line("     Meg Stivison");
    pencilCredits->add_line("     Michael Grenetz");
    pencilCredits->add_line("     Michael Parkin-White");
    pencilCredits->add_line("     Paul Beerkens");
    pencilCredits->add_line("     Paul Broadhurst");
    pencilCredits->add_line("     Roy Mwale");
    pencilCredits->add_line("     Sagi Koren");
    pencilCredits->add_line("     Seumas Cordell Froemke");
    pencilCredits->add_line("     Taran Buford");
    pencilCredits->add_line("     Teresa Ziebarth");
    pencilCredits->add_line("     Toby Hutton");
    pencilCredits->add_line("     Andreas Heldt");
    pencilCredits->add_line("     Zach Underwood");

    pencilCredits->add_line("");
    pencilCredits->add_line("");
    pencilCredits->add_line("Thank you for backing Agriduel aka Fields of Fresh which lead to this IDE and engine being developed.");
    pencilCredits->add_line("");
    pencilCredits->add_line("-----------------------------------");
    pencilCredits->add_line("");
    pencilCredits->add_line("Thank you for using the Game Pencil Engine.");
    pencilCredits->add_line("");
    pencilCredits->add_line("");

    pencilDescription = new GPE_TextAreaInputBasic();
    pencilDescription->set_read_only();
    pencilDescription->isCodeEditor = false;

    pencilDescription->clear_all_lines();
    pencilDescription->import_text( gpe::app_directory_name+"editor_description.txt");
    pencilDescription->set_height(640);

    pencilMissionStatement = new GPE_TextAreaInputBasic();
    pencilMissionStatement->set_read_only();
    pencilMissionStatement->isCodeEditor = false;

    pencilLicense = new GPE_TextAreaInputBasic();
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

    thirdPartyLicenses = new GPE_TextAreaInputBasic();
    thirdPartyLicenses->set_read_only();
    thirdPartyLicenses->isCodeEditor = false;
    thirdPartyLicenses->import_text( gpe::app_directory_name+"third_party_licenses.txt");
    thirdPartyLicenses->set_height(640);

    aboutPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
    aboutPageList->barXMargin = 0;
}

gamePencilAboutPageResource::~gamePencilAboutPageResource()
{
    subViewedSpace.x = GENERAL_GPE_GUI_PADDING;
    subViewedSpace.y = GENERAL_GPE_GUI_PADDING;
    subViewedSpace.w = 400;
    subViewedSpace.h = 400;
    resourceType = -2;
    subResouceId = -2;
    parentProjectName = "";

    if( aboutPageList!=NULL)
    {
        delete aboutPageList;
        aboutPageList = NULL;
    }
    if( pencilCredits!=NULL)
    {
        delete pencilCredits;
        pencilCredits = NULL;
    }
    if( pencilDescription!=NULL)
    {
        delete pencilDescription;
        pencilDescription = NULL;
    }
    if( pencilLicense!=NULL)
    {
        delete pencilLicense;
        pencilLicense = NULL;
    }
    if( thirdPartyLicenses!=NULL)
    {
        delete thirdPartyLicenses;
        thirdPartyLicenses = NULL;
    }
    if( pencilMissionStatement!=NULL)
    {
        delete pencilMissionStatement;
        pencilMissionStatement = NULL;
    }
}

bool gamePencilAboutPageResource::include_local_files( std::string pBuildDir , int buildType )
{

}

void gamePencilAboutPageResource::prerender_self( )
{

}

void gamePencilAboutPageResource::load_resource(std::string file_path)
{

}

void gamePencilAboutPageResource::process_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( viewedSpace!=NULL && aboutPageList!=NULL)
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
            aboutPageList->reset_self();
        }



        aboutPageList->set_coords(subViewedSpace.x,subViewedSpace.y );
        aboutPageList->set_width(subViewedSpace.w);
        aboutPageList->set_height(subViewedSpace.h );
        aboutPageList->barXPadding = GENERAL_GPE_GUI_PADDING;
        aboutPageList->barXMargin = subViewedSpace.w/8;

        aboutPageList->clear_list();
        if( sideAreaPanel->get_selected_name()=="Credits")
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
        aboutPageList->process_self(viewedSpace,cam);
    }
}

void gamePencilAboutPageResource::render_self( gpe::shape_rect * viewedSpace, gpe::shape_rect * cam )
{
    viewedSpace = gpe::camera_find(viewedSpace);
    cam = gpe::camera_find(cam);
    if( cam!=NULL && viewedSpace!=NULL)
    {
        if( sideAreaPanel!=NULL && PANEL_GENERAL_EDITOR==NULL )
        {
            sideAreaPanel->render_self( viewedSpace,cam);
        }
        if( aboutPageList!=NULL )
        {
            aboutPageList->render_self( viewedSpace,cam);
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
