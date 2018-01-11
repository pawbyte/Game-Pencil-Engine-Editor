/*
gpe_editor_aboutpage.cpp
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

#include "gpe_editor_aboutpage.h"

gamePencilAboutPageResource::gamePencilAboutPageResource(GPE_ResourceContainer * pFolder)
{
    resourceFileName = "";
    resourceName = "About Page";
    parentProjectName = "";
    parentProjectDirectory = "";
    //GPE_Animation * mainMenuSprite = rsm->sprite_add(APP_DIRECTORY_NAME+"resources/gfx/sprites/gpe_main_icons_big.png",70,true,0,0,false);
    projectParentFolder = pFolder;
    renameBox->disable_self();
    confirmResourceButton->disable_self();
    cancelResourceButton->disable_self();
    loadResourceButton->disable_self();
    saveResourceButton->disable_self();


    aboutPageList = new GPE_GuiElementList();
    aboutPageTabBar = new GPE_TabBar();
    aboutPageTabBar->add_new_tab("Description");
    aboutPageTabBar->add_new_tab("Information");
    aboutPageTabBar->add_new_tab("Credits");
    aboutPageTabBar->add_new_tab("License");
    aboutPageTabBar->open_tab(0);
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


    pencilCredits->add_line("");
    pencilCredits->add_line("Patreon Backers (Ordered Alphabetically):");
    pencilCredits->add_line("-----------------------------------");
    pencilCredits->add_line("     Feral Monk");
    pencilCredits->add_line("     James Thomas");
    pencilCredits->add_line("     Jen Anderson");
    pencilCredits->add_line("     Mark Medrano");
    pencilCredits->add_line("     Keith Thomas");
    pencilCredits->add_line("     Noel Hurde");
    pencilCredits->add_line("     Thomas Ingham");
    pencilCredits->add_line("     Roshin Varghese");




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
    pencilDescription->import_text(APP_DIRECTORY_NAME+"editor_description.txt");
    pencilDescription->set_height(640);

        //Information Tab
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
    pencilMissionStatement = new GPE_TextAreaInputBasic();
    pencilMissionStatement->set_read_only();
    pencilMissionStatement->isCodeEditor = false;
    pencilLicense= new GPE_TextAreaInputBasic();
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
    aboutPageList->barXPadding = GENERAL_GPE_PADDING;
    aboutPageList->barXMargin = 0;
}

gamePencilAboutPageResource::~gamePencilAboutPageResource()
{
    subViewedSpace.x = GENERAL_GPE_PADDING;
    subViewedSpace.y = GENERAL_GPE_PADDING;
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
    if( aboutPageTabBar!=NULL)
    {
        delete aboutPageTabBar;
        aboutPageTabBar = NULL;
    }
}

void gamePencilAboutPageResource::prerender_self( )
{

}

void gamePencilAboutPageResource::preprocess_self(std::string alternatePath)
{

}

void gamePencilAboutPageResource::process_self(GPE_Rect * viewedSpace , GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && aboutPageList!=NULL)
    {
        aboutPageTabBar->set_coords(GENERAL_GPE_PADDING,GENERAL_GPE_PADDING );
        aboutPageTabBar->process_self(viewedSpace,cam);

        subViewedSpace.x = GENERAL_GPE_PADDING;
        subViewedSpace.y = aboutPageTabBar->get_y2pos()+GENERAL_GPE_PADDING;
        subViewedSpace.w = viewedSpace->w-subViewedSpace.x;
        subViewedSpace.h = viewedSpace->h- subViewedSpace.y;



        aboutPageList->set_coords(subViewedSpace.x,subViewedSpace.y );
        aboutPageList->set_width(subViewedSpace.w);
        aboutPageList->set_height(subViewedSpace.h-aboutPageList->get_ypos());
        aboutPageList->barXPadding = GENERAL_GPE_PADDING;
        aboutPageList->barXMargin = subViewedSpace.w/8;

        aboutPageList->clear_list();
        if( aboutPageTabBar->get_selected_name()=="Credits")
        {
            pencilCredits->set_width(subViewedSpace.w );
            pencilCredits->set_height(subViewedSpace.h );
            pencilCredits->set_coords(subViewedSpace.x,subViewedSpace.y);
            pencilCredits->process_self(viewedSpace,cam);
        }
        else if( aboutPageTabBar->get_selected_name()=="Description" )
        {
            pencilDescription->set_width(subViewedSpace.w );
            pencilDescription->set_height(subViewedSpace.h);
            pencilDescription->set_coords(subViewedSpace.x,subViewedSpace.y);
            pencilDescription->process_self(viewedSpace,cam);
        }
        else if( aboutPageTabBar->get_selected_name()=="Information" )
        {
            pencilInformation->set_width(subViewedSpace.w );
            pencilInformation->set_height(subViewedSpace.h);
            pencilInformation->set_coords(subViewedSpace.x,subViewedSpace.y);
            pencilInformation->process_self(viewedSpace,cam);
        }
        else if( aboutPageTabBar->get_selected_name()=="License" )
        {
            pencilLicense->set_width(subViewedSpace.w );
            pencilLicense->set_height(subViewedSpace.h );
            pencilLicense->set_coords(subViewedSpace.x,subViewedSpace.y);
            pencilLicense->process_self(viewedSpace,cam);
        }
        else
        {
            //aboutPageList->add_gui_element(aboutPageTabBar,true);
            aboutPageList->process_self(viewedSpace,cam);
        }
    }
}

void gamePencilAboutPageResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && aboutPageList!=NULL)
    {
//        gpe->render_gradient_vertical( &gpeLogoSpace,GPE_MAIN_THEME->Button_Box_Highlighted_Color);
        if( aboutPageTabBar->get_selected_name()=="Credits")
        {
            pencilCredits->render_self( viewedSpace,cam,forceRedraw);
        }
        if( aboutPageTabBar->get_selected_name()=="Description")
        {
            pencilDescription->render_self( viewedSpace,cam,forceRedraw);
        }
        else if( aboutPageTabBar->get_selected_name()=="Information" )
        {
            pencilInformation->render_self( viewedSpace,cam,forceRedraw);
        }
        else if( aboutPageTabBar->get_selected_name()=="License" )
        {
            pencilLicense->render_self( viewedSpace,cam,forceRedraw);
        }
        else
        {
            aboutPageList->render_self( viewedSpace,cam,forceRedraw);
        }

        //render_new_text( gpeLogoSpace.x,gpeLogoSpace.y+gpeLogoSpace.h-GENERAL_GPE_PADDING,"PUBLIC[BETA] - Version 0.2 | Copyright (c)2016 PawByte | make.pawbyte.com",GPE_MAIN_THEME->Main_Box_Faded_Font_Color,DEFAULT_FONT,FA_LEFT,FA_BOTTOM);

        //render_new_text( subViewedSpace.x+GENERAL_GPE_PADDING,subViewedSpace.y+GENERAL_GPE_PADDING,"Recent Projects",GPE_MAIN_THEME->Main_Box_Font_Color,textboxFont,FA_LEFT,FA_TOP);
        aboutPageTabBar->render_self( viewedSpace,cam,forceRedraw);
    }
}

void gamePencilAboutPageResource::save_resource(std::string alternatePath, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
}

bool gamePencilAboutPageResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    return true;
}
