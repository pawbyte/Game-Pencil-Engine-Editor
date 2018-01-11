/*
gpe_editor_start_page.cpp
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

#include "gpe_editor_start_page.h"

gamePencilStartPageResource::gamePencilStartPageResource(GPE_ResourceContainer * pFolder)
{
    resourceFileName = "";
    resourceName = "Start Page";
    parentProjectName = "";
    parentProjectDirectory = "";
    projectParentFolder = NULL;
    subViewedSpace.x = GENERAL_GPE_PADDING;
    subViewedSpace.x = GENERAL_GPE_PADDING;
    subViewedSpace.w = 400;
    subViewedSpace.h = 400;
    resourceType = -2;
    subResouceId = -2;
    renameBox->disable_self();
    confirmResourceButton->disable_self();
    cancelResourceButton->disable_self();
    loadResourceButton->disable_self();
    saveResourceButton->disable_self();

    startPageList = new GPE_GuiElementList();
    startPageList->set_horizontal_align(FA_CENTER);
    startPageList->barXPadding = GENERAL_GPE_PADDING;
    startPageList->barXMargin = 128;
    labelEditorShortcuts = new GPE_Label_Title("Editor Shortcuts","Editor Shortcuts");
    /*/Patreon Stuff */
    labelPatreon = new GPE_Label_Title("Support Our Patreon Campaign","");
    labelPatreonLogo = new GPE_Label_Image(GPE_LOGO);
    labelPatreonLogo->load_label_image(APP_DIRECTORY_NAME+"resources/gfx/textures/become_a_patron_button.png");
    labelPatreonLogo->hasLineBreak = true;
    labelPatreonLogo->webUrl = "https://www.patreon.com/pawbyte?ty=h";

    labelPatreonDescription = new GPE_Label_Text("By donating a small amount you can keep this engine going!","");
    urlPatreonPage = new GPE_TextURL("Fund future exports and features","A few dollars can go a long way","https://www.patreon.com/pawbyte?ty=h");
    urlPatreonPage->hasLineBreak = true;

    labelHelpfulLinks = new GPE_Label_Title("Helpful Links","Helpful Links");

    GPE_TextURL * tTempUrl = NULL;
    tTempUrl = new GPE_TextURL("Game Pencil Home Page","","http://gamepencil.pawbyte.com");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);
    tTempUrl = new GPE_TextURL("Documentation","","http://docs.pawbyte.com");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("Official Forums","","http://forums.pawbyte.com");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("itch.io Marketplace","","https://itch.io/game-assets/tag-gamepencil");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("Discord Chat","","https://discord.gg/aNX3Fcx");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("Official News","","http://gamepencil.pawbyte.com/category/news/");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("GitHub","","https://github.com/pawbyte/Game-Pencil-Engine");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    tTempUrl = new GPE_TextURL("PawByte","","http://www.pawbyte.com");
    tTempUrl->hasLineBreak = true;
    helpfulWebLinks.push_back(tTempUrl);

    projectBrowserButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/binoculars.png","Project Browser","",-1,64);
    projectBrowserButton->enable_background(false);
    projectBrowserButton->enable_border(false);

    forumPageButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/comments.png","Forums","",-1,64);
    forumPageButton->enable_background(false);
    forumPageButton->set_website("http://community.pawbyte.com");
    forumPageButton->enable_border(false);

    helpPageButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/question.png","Help","",-1,64);
    helpPageButton->enable_background(false);
    helpPageButton->enable_border(false);

    tipOfTheDayButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/info.png","Tip of the Day","",-1,64);
    tipOfTheDayButton->enable_background(false);
    tipOfTheDayButton->enable_border(false);
    std::string copyRightInfoString = "";
    if( GPE_VERSION_UPDATE_NUMBER==0)
    {
        copyRightInfoString = float_to_string(GPE_VERSION_DOUBLE_NUMBER)+".0.0";
    }
    else
    {
        copyRightInfoString = float_to_string(GPE_VERSION_DOUBLE_NUMBER);
    }
    if( GPE_MAIN_GUI!=NULL)
    {
        if( GPE_MAIN_GUI->editorReleaseType==RELEASE_TYPE_RC)
        {
            copyRightInfoString+="   - Release Candidate";
        }
        else if( GPE_MAIN_GUI->editorReleaseType==RELEASE_TYPE_BETA)
        {
            copyRightInfoString+="   - Beta Version";
        }
    }

    labelCopyrightInfo = new GPE_Label_Text("Game Pencil Engine - Version "+copyRightInfoString+".","");
    labelVersionInfo = new GPE_Label_Text("Copyright (c) 2018 PawByte | make.pawbyte.com","");
    labelImgGPELogo = new GPE_Label_Image(GPE_LOGO);
    labelImgGPELogo->hasLineBreak = true;
    labelImgGPELogo->resizeAspect = 0.667;
 }

gamePencilStartPageResource::~gamePencilStartPageResource()
{
    record_error("Starting to delete start page...");
    subViewedSpace.x = GENERAL_GPE_PADDING;
    subViewedSpace.x = GENERAL_GPE_PADDING;
    subViewedSpace.w = 400;
    subViewedSpace.h = 400;
    resourceType = -2;
    subResouceId = -2;
    parentProjectName = "";
    record_error("Deleting recent project lists...");
    GPE_TextURL * tUrlHolder = NULL;
    int i = 0;

    record_error("Deleting helpful website links..");
    for( i = 0; i <(int)helpfulWebLinks.size(); i++)
    {
       tUrlHolder = helpfulWebLinks[i];
       if( tUrlHolder!=NULL)
       {
           delete tUrlHolder;
           tUrlHolder = NULL;
           helpfulWebLinks[i] = NULL;
       }
    }
    helpfulWebLinks.clear();
    record_error("Deleting Project Browser....");
    if( projectBrowserButton!=NULL)
    {
        delete projectBrowserButton;
        projectBrowserButton = NULL;
    }
    if( startPageList!=NULL)
    {
        delete startPageList;
        startPageList = NULL;
    }
    record_error("Deleting editor shortcuts....");
    if( labelEditorShortcuts!=NULL)
    {
        delete labelEditorShortcuts;
        labelEditorShortcuts = NULL;
    }
    record_error("Deleting helpful links....");
    if( labelHelpfulLinks!=NULL)
    {
        delete labelHelpfulLinks;
        labelHelpfulLinks = NULL;
    }
    record_error("Deleting copyright info....");
    if( labelCopyrightInfo!=NULL)
    {
        delete labelCopyrightInfo;
        labelCopyrightInfo = NULL;
    }
    record_error("Deleting version info....");
    if( labelVersionInfo!=NULL)
    {
        delete labelVersionInfo;
        labelVersionInfo = NULL;
    }
    record_error("Deleting gpe img logo....");
    if( labelImgGPELogo!=NULL)
    {
        delete labelImgGPELogo;
        labelImgGPELogo = NULL;
    }
    record_error("Startpage successfully deleted....");
}

void gamePencilStartPageResource::prerender_self( )
{

}

void gamePencilStartPageResource::preprocess_self(std::string alternatePath)
{

}

void gamePencilStartPageResource::process_self(GPE_Rect * viewedSpace , GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && cam!=NULL && startPageList!=NULL)
    {
        subViewedSpace.x = 0;
        subViewedSpace.y = 0;
        subViewedSpace.w = viewedSpace->w;
        subViewedSpace.h = viewedSpace->h;

        startPageList->set_coords(subViewedSpace.x,subViewedSpace.y);
        startPageList->set_width(subViewedSpace.w);
        startPageList->set_height(subViewedSpace.h);
        startPageList->barXPadding = 0;
        startPageList->barXMargin = 0;

        startPageList->clear_list();
        //labelImgGPELogo->set_image(GPE_LOGO);
        startPageList->add_gui_element(labelImgGPELogo,true);
        startPageList->add_gui_element(labelCopyrightInfo,true);
        startPageList->add_gui_element(labelVersionInfo,true);
        startPageList->add_gui_element(labelEditorShortcuts,true);
        startPageList->add_gui_element(projectBrowserButton,false);
        startPageList->add_gui_element(forumPageButton,true);
        startPageList->add_gui_element(helpPageButton,false);
        startPageList->add_gui_element(tipOfTheDayButton,true);
        startPageList->yScroll->documentActions = true;

        startPageList->add_gui_element(labelHelpfulLinks,true);
        for( int i = 0 ; i < (int)helpfulWebLinks.size(); i++)
        {
            startPageList->add_gui_element( helpfulWebLinks[i] ,true);
        }
        /*Patreon Area */
        startPageList->add_gui_element(labelPatreon,true);
        startPageList->add_gui_element(labelPatreonLogo,true);
        startPageList->add_gui_element(labelPatreonDescription,true);
        startPageList->add_gui_element(urlPatreonPage,true);


        startPageList->process_self(viewedSpace,cam);

        //Get individual processes here
        if( projectBrowserButton->is_clicked() )
        {
            GPE_Main_TabManager->add_new_tab(PROJECT_BROWSER_PAGE);
        }
        else if( tipOfTheDayButton->is_clicked() )
        {
            GPE_Show_Tip_Of_Day();
        }
        else if( helpPageButton->is_clicked() )
        {
            GPE_Main_TabManager->add_new_tab(MAIN_HELP_PAGE);
        }
    }
}

void gamePencilStartPageResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( viewedSpace!=NULL && startPageList!=NULL)
    {
        //gpe->render_rect( &subViewedSpace,GPE_MAIN_THEME->Program_Header_Color,false);
        //render_new_text( subViewedSpace.x+GENERAL_GPE_PADDING,subViewedSpace.y+GENERAL_GPE_PADDING,"Recent Projects",GPE_MAIN_THEME->Main_Box_Font_Color,textboxFont,FA_LEFT,FA_TOP);
        if( forceRedraw)
        {
            startPageList->render_self( viewedSpace,cam,forceRedraw);
        }
    }
}

void gamePencilStartPageResource::save_resource(std::string alternatePath, int backupId)
{
    bool usingAltSaveSource = false;
    isModified = false;
}

bool gamePencilStartPageResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    return true;
}


