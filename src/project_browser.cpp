/*
project_browser.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2017 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2017 PawByte.
Copyright (c) 2014-2017 Game Pencil Engine contributors ( Contributors Page )

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

#include "project_browser.h"

quickProjectReader::quickProjectReader()
{
    detectedName = "";
    detectedIcon = "";
    detectedLocation = "";
    detectedSaveTime = "";
    detectedVersion = "";
    detectedLanguage = "JS";
}

quickProjectReader::~quickProjectReader()
{

}

bool quickProjectReader::review_project(std::string projectFileName)
{
    detectedName = "";
    detectedIcon = "";
    detectedLocation = "";
    detectedSaveTime = "";
    detectedVersion = "Unknown";
    if( file_exists(projectFileName) )
    {
        std::ifstream newprofileFile( projectFileName.c_str() );

        //If the level file could be loaded
        if( !newprofileFile.fail() )
        {
            //makes sure the file is open
            if (newprofileFile.is_open())
            {
                detectedName = projectFileName;
                detectedLocation = projectFileName;

                std::string firstChar="";
                std::string section="";
                std::string cur_layer="";
                std::string data_format="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string newResName= "";
                std::string tempNewResName = "";
                std::string tempFoundIcon;
                int equalPos=-1;
                int endBracketPos = -1;
                int foundResGlobalId = -1;
                int tempStrToInt = -1;
                int iItr = 0;
                int iLang = 0;
                //makes sure the file is in good condition and the version is still unloaded
                while ( newprofileFile.good() )
                {
                    getline (newprofileFile,currLine); //gets the next line of the file
                    currLine = trim_left_inplace(currLine);
                    currLine = trim_right_inplace(currLine);

                    if(!currLine.empty() )
                    {
                        // skips comment lines
                        if( currLine[0]!= '#' && currLine[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLine.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLine.substr(0,equalPos);
                                valString = currLine.substr(equalPos+1,currLine.length());
                                //finds the version of the game level file
                                if (keyString== "Version")
                                {
                                    detectedVersion = valString;
                                }
                                else if( keyString=="Count" || keyString=="ResourcesCount")
                                {
                                    resourcesInProject = string_to_int(valString);
                                }
                                else if(keyString=="ProjectName" || keyString=="Name")
                                {
                                    detectedName = valString;
                                }
                                else if(keyString=="ProjectLanguage" || keyString=="ProgrammingLanguage" || keyString=="CodingLanguage")
                                {
                                    for( iLang = 0; iLang < PROJECT_LANGUAGE_MAX; iLang++)
                                    {
                                        if( valString==PROJECT_LANGUAGE_NAMES[iLang] )
                                        {
                                            detectedLanguage = PROJECT_LANGUAGE_NAMES[iLang];
                                        }
                                    }
                                }
                                else if( keyString=="ProjectIcon")
                                {
                                    if( path_exists( get_file_noext(projectFileName) ) )
                                    {
                                        tempFoundIcon = get_file_noext(projectFileName)+"/"+valString;
                                        if( file_exists( tempFoundIcon )  && file_is_image(tempFoundIcon) )
                                        {
                                            detectedIcon = tempFoundIcon;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                newprofileFile.close();
                if( ( (int)tempFoundIcon.size()==0) && path_exists( get_file_noext(projectFileName) ) )
                {
                    if( file_exists( get_file_noext(projectFileName)+"/icon.png" ) )
                    {
                        detectedIcon = get_file_noext(projectFileName)+"/icon.png";
                    }
                }
                return true;
            }
        }
    }
    return false;
}

gamePencilProjectBrowserResource::gamePencilProjectBrowserResource(GPE_ResourceContainer * pFolder)
{
    //Added as of Version 1.1.3 [ BEGIN ]
    projectsCategoryTabs = new GPE_TabBar();
    projectsCategoryTabs->set_coords(16,16);
    projectsCategoryTabs->add_new_tab("Recent Projects",true);
    projectsCategoryTabs->add_new_tab("Example Projects",false);
    projectsCategoryTabs->add_new_tab("Favorites Folder",false);

    headerSpaceHeight = 96;
    headerViewedSpace.x = 0;
    headerViewedSpace.y = 0;
    headerViewedSpace.w = SCREEN_WIDTH;
    headerViewedSpace.h = headerSpaceHeight;

    subViewedSpace.x = 0;
    subViewedSpace.y = 0;
    subViewedSpace.w = SCREEN_WIDTH;
    subViewedSpace.h = SCREEN_HEIGHT;

    headerPageList = new GPE_GuiElementList();
    headerPageList->set_horizontal_align(FA_LEFT);
    headerPageList->barXMargin = GENERAL_GPE_PADDING;
    headerPageList->barYMargin = 0;
    headerPageList->barYPadding = GENERAL_GPE_PADDING/2;
    headerPageList->hideXScroll = true;
    headerPageList->hideYScroll = true;
    //Added as of Version 1.1.3 [ END ]
    processedFirst = false;
    resourceFileName = "";
    resourceName = "Project Browser";
    parentProjectName = "";
    parentProjectDirectory = "";
    projectParentFolder = NULL;
    resourceType = -5;
    subResouceId = -5;
    renameBox->disable_self();
    confirmResourceButton->disable_self();
    cancelResourceButton->disable_self();
    loadResourceButton->disable_self();
    saveResourceButton->disable_self();



    projectBrowserList = new GPE_GuiElementList();
    projectBrowserList->set_horizontal_align(FA_LEFT);
    projectBrowserList->barXMargin = GENERAL_GPE_PADDING;
    projectBrowserList->barYMargin = 0;
    projectBrowserList->barYPadding = GENERAL_GPE_PADDING/2;

    projectsBrowserTitle = new GPE_Label_Title("Project Browser","Project Browser");
    projectsTitle = new GPE_Label_Text("Favorite Projects Folder","Favorite Projects Folder");
    recentProjectsTitle = new GPE_Label_Text("Recent Projects","Recent Projects");
    exampleProjectsTitle = new GPE_Label_Text("Example Projects","Example Projects");
    emptyProjectsListLabel = new GPE_Label_Text("No Recent projects","No Recent projects");

    scanProjectFolderButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/search.png","Scan Folder","Scan Folder");
    scanProjectFolderButton->set_width(128);

    newProjectButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/file.png","New Project","New Project");
    newProjectButton->set_width(128);

    loadProjectButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/folder-open.png","Load Project","Load Project");
    loadProjectButton->set_width(128);

    refreshButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/refresh.png","Refresh","Refresh Project Lists");
    refreshButton->set_width(96);

    quickProjectReader * qpr = new quickProjectReader();
    GPE_ToolPushButtonMultiLine * tRecentListButton = NULL;
    std::string tempProjectName = "";
    std::string newMultiLine = "";
    int  rProjectListSize = (int)recentProjectsList.size();
    int i = 0;
    record_error("Loading Recent Projects...");
    for( i = 0; i < GPE_MAX_RECENT_PROJECT_LIST_SIZE; i++)
    {
        tempProjectName = GPE_MAIN_GUI->get_recent_project_name(i);
        if( (int)tempProjectName.size() > 0 && file_exists(tempProjectName) )
        {
            if( qpr->review_project(tempProjectName) )
            {
                if( file_exists(qpr->detectedIcon) ==false)
                {
                    qpr->detectedIcon =APP_DIRECTORY_NAME+"resources/gamepencil_icon_72dpi.png";
                }
                newMultiLine = qpr->detectedName;
                if((int)qpr->detectedVersion.size() > 0 )
                {
                    newMultiLine+="\nVersion: "+qpr->detectedVersion+"  |  "+qpr->detectedLanguage+" Project";
                }
                newMultiLine+="\nLocation: "+tempProjectName;
                tRecentListButton = new GPE_ToolPushButtonMultiLine(0,0,qpr->detectedIcon,newMultiLine,qpr->detectedLocation);
                recentProjectsList.push_back(tRecentListButton);
            }
        }
    }
    delete qpr;
    qpr = NULL;

    record_error("Loading Favorite Projects Folder...");
    std::string favFolder = "";
    if( MAIN_EDITOR_SETTINGS!=NULL)
    {
        favFolder = MAIN_EDITOR_SETTINGS->projectFolderListLocation;
    }
    projectFolderLocationLabel = new GPE_Label_Text(favFolder,favFolder);
    load_projecs_folder(favFolder);

    processedFirst = true;
    load_example_projects_folder();
    record_error("Loading Completed...");

}

gamePencilProjectBrowserResource::~gamePencilProjectBrowserResource()
{
    if( projectBrowserList!=NULL)
    {
        projectBrowserList->clear_list();
        delete projectBrowserList;
        projectBrowserList = NULL;
    }
    if( exampleProjectsTitle!=NULL)
    {
        delete exampleProjectsTitle;
        exampleProjectsTitle = NULL;
    }
    if( projectsCategoryTabs!=NULL)
    {
        delete projectsCategoryTabs;
        projectsCategoryTabs = NULL;
    }
    if( projectFolderLocationLabel!=NULL)
    {
        delete projectFolderLocationLabel;
        projectFolderLocationLabel = NULL;
    }
    if( recentProjectsTitle!=NULL)
    {
        delete recentProjectsTitle;
        recentProjectsTitle = NULL;
    }
    if( projectsTitle!=NULL)
    {
        delete projectsTitle;
        projectsTitle = NULL;
    }
    if( scanProjectFolderButton!=NULL)
    {
        delete scanProjectFolderButton;
        scanProjectFolderButton = NULL;
    }
    if( newProjectButton!=NULL)
    {
        delete newProjectButton;
        newProjectButton = NULL;
    }

    if( loadProjectButton!=NULL)
    {
        delete loadProjectButton;
        loadProjectButton = NULL;
    }
}

void gamePencilProjectBrowserResource::prerender_self(GPE_Renderer * cRender)
{

}

void gamePencilProjectBrowserResource::preprocess_self(std::string alternatePath)
{

}

void gamePencilProjectBrowserResource::process_self(GPE_Rect * viewedSpace,GPE_Rect *cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( !processedFirst )
    {
        if( MAIN_EDITOR_SETTINGS!=NULL)
        {
            load_projecs_folder(MAIN_EDITOR_SETTINGS->projectFolderListLocation);
            processedFirst = true;
        }
    }

    if( cam!=NULL && projectBrowserList!=NULL && headerPageList!=NULL && projectsCategoryTabs!=NULL && viewedSpace!=NULL)
    {
        headerViewedSpace.x = viewedSpace->x;
        headerViewedSpace.y = viewedSpace->y;
        headerViewedSpace.w = viewedSpace->w;
        headerViewedSpace.h = headerSpaceHeight;

        subViewedSpace.x = viewedSpace->x;
        subViewedSpace.y = headerViewedSpace.y+headerViewedSpace.h;
        subViewedSpace.w = viewedSpace->w;
        subViewedSpace.h = viewedSpace->h - headerViewedSpace.h;

        int prevTab = projectsCategoryTabs->tabInUse;
        headerPageList->clear_list();
        headerPageList->set_coords(0,0);
        headerPageList->set_width(headerViewedSpace.w);
        headerPageList->set_height(headerViewedSpace.h );
        headerPageList->barXPadding = GENERAL_GPE_PADDING;
        headerPageList->barXMargin = GENERAL_GPE_PADDING;
        headerPageList->add_gui_element(projectsBrowserTitle,true);
        headerPageList->add_gui_element(loadProjectButton,false);
        headerPageList->add_gui_element(newProjectButton,false);
        headerPageList->add_gui_element(refreshButton,true);
        headerPageList->add_gui_element(projectsCategoryTabs,true);
        headerPageList->process_self(&headerViewedSpace,cam);

        if( prevTab!=projectsCategoryTabs->tabInUse)
        {
            projectBrowserList->reset_self();
        }

        projectBrowserList->clear_list();
        projectBrowserList->set_coords(0,0);
        projectBrowserList->set_width(subViewedSpace.w);
        projectBrowserList->set_height(subViewedSpace.h );
        projectBrowserList->barXPadding = GENERAL_GPE_PADDING;
        projectBrowserList->barXMargin = GENERAL_GPE_PADDING;

        projectBrowserList->yScroll->documentActions = true;
        //Recent Projects
        GPE_TextURL * tTUrl = NULL;
        int i = 0;
        int  rProjectListSize = 0;
        int favProjectListSize = 0;
        int exampleProjectListSize = 0;
        if(projectsCategoryTabs->get_selected_name()=="Recent Projects")
        {
            //int  rProjectListSize = GPE_MAIN_GUI->get_recent_project_list_size();
            rProjectListSize = (int)recentProjectsList.size();
            projectBrowserList->add_gui_element(recentProjectsTitle,true);
            if( rProjectListSize > 0)
            {
                for( i = 0; i <  rProjectListSize; i++)
                {
                    if( recentProjectsList[i]!=NULL)
                    {
                        recentProjectsList[i]->set_width(viewedSpace->w-(projectBrowserList->barXMargin+projectBrowserList->barXPadding+GENERAL_GPE_PADDING)*2 );
                        //recentProjectsList[i]->set_height(64);
                        projectBrowserList->add_gui_element(recentProjectsList[i],true);
                    }
                }
            }
            else
            {
                projectBrowserList->add_gui_element(emptyProjectsListLabel,true);
            }
        }
        else if(projectsCategoryTabs->get_selected_name()=="Example Projects")
        {
             //int  rProjectListSize = GPE_MAIN_GUI->get_recent_project_list_size();
            int  exProjectListSize = (int)examplesProjectsList.size();
            projectBrowserList->add_gui_element(exampleProjectsTitle,true);
            if( exProjectListSize > 0)
            {
                for( i = 0; i <  exProjectListSize; i++)
                {
                    if( examplesProjectsList[i]!=NULL)
                    {
                        examplesProjectsList[i]->set_width(viewedSpace->w-(projectBrowserList->barXMargin+projectBrowserList->barXPadding+GENERAL_GPE_PADDING)*2 );
                        //examplesProjectsList[i]->set_height(64);
                        projectBrowserList->add_gui_element(examplesProjectsList[i],true);
                    }
                }
            }
            else
            {
                projectBrowserList->add_gui_element(emptyProjectsListLabel,true);
            }
        }
        else
        {
            //Favorites Folder
            favProjectListSize = (int)foundProjectsList.size();
            projectBrowserList->add_gui_element(projectsTitle,false);
            projectBrowserList->add_gui_element(scanProjectFolderButton,true);
            if( favProjectListSize > 0)
            {
                projectBrowserList->add_gui_element(projectFolderLocationLabel,true);
                for( i = 0; i <  favProjectListSize; i++)
                {
                    if( foundProjectsList[i]!=NULL)
                    {
                        foundProjectsList[i]->set_width(viewedSpace->w-(projectBrowserList->barXMargin+projectBrowserList->barXPadding+GENERAL_GPE_PADDING)*2 );
                        //foundProjectsList[i]->set_height(64);
                        projectBrowserList->add_gui_element(foundProjectsList[i],true);
                    }
                }
            }
        }
        projectBrowserList->process_self(&subViewedSpace,cam);

        if( refreshButton!=NULL && refreshButton->is_clicked() )
        {
            rProjectListSize = (int)recentProjectsList.size();
            i = 0;
            GPE_ToolPushButtonMultiLine * tRecentListButton = NULL;
            if( rProjectListSize > 0)
            {
                for( i = rProjectListSize-1; i >=0; i--)
                {
                    tRecentListButton = recentProjectsList[i];
                    if( tRecentListButton!=NULL)
                    {
                        delete tRecentListButton;
                        tRecentListButton = NULL;
                    }
                }
                recentProjectsList.clear();
            }

            quickProjectReader * qpr = new quickProjectReader();
            tRecentListButton = NULL;
            std::string tempProjectName = "";
            std::string newMultiLine = "";
            for( i = 0; i < GPE_MAX_RECENT_PROJECT_LIST_SIZE; i++)
            {
                tempProjectName = GPE_MAIN_GUI->get_recent_project_name(i);
                if( (int)tempProjectName.size() > 0 && file_exists(tempProjectName) )
                {
                    if( qpr->review_project(tempProjectName) )
                    {
                        if( file_exists(qpr->detectedIcon) ==false)
                        {
                            qpr->detectedIcon =APP_DIRECTORY_NAME+"resources/gamepencil_icon_72dpi.png";
                        }
                        newMultiLine = qpr->detectedName;
                        if((int)qpr->detectedVersion.size() > 0 )
                        {
                            newMultiLine+="\nVersion: "+qpr->detectedVersion+"  |  "+qpr->detectedLanguage+" Project";
                        }
                        newMultiLine+="\nLocation: "+tempProjectName;
                        tRecentListButton = new GPE_ToolPushButtonMultiLine(0,0,qpr->detectedIcon, newMultiLine, qpr->detectedLocation);
                        recentProjectsList.push_back(tRecentListButton);
                    }
                }
            }
            delete qpr;
            qpr = NULL;
            if( MAIN_EDITOR_SETTINGS!=NULL)
            {
                load_projecs_folder(MAIN_EDITOR_SETTINGS->projectFolderListLocation );
                processedFirst = true;
            }
            else
            {
                std::string favFolder = APP_DIRECTORY_NAME+"examples";
                projectFolderLocationLabel = new GPE_Label_Text(favFolder,favFolder);
                load_projecs_folder(favFolder);
            }
            userInput->reset_all_input();
            load_example_projects_folder();
            process_self(viewedSpace,cam);
        }
        else if( newProjectButton!=NULL && newProjectButton->is_clicked() )
        {
            GPE_MAIN_GUI->launch_new_project();
        }
        else if( loadProjectButton!=NULL && loadProjectButton->is_clicked() )
        {
            GPE_MAIN_GUI->open_new_project();
        }
        else if( scanProjectFolderButton!=NULL && scanProjectFolderButton->is_clicked() )
        {
            std::string lastDirectory = "";
            std::string newPathToScan = GPE_GetDirectoryName("Scan Folder for Projects", lastDirectory,false);
            if( path_exists(newPathToScan) )
            {
                load_projecs_folder(newPathToScan);
                userInput->reset_all_input();
                process_self(viewedSpace,cam);
            }
        }
        else
        {
            favProjectListSize = (int)foundProjectsList.size();
            exampleProjectListSize = (int)examplesProjectsList.size();
            rProjectListSize = (int)recentProjectsList.size();
            bool projectOpened = false;
            if(projectsCategoryTabs->get_selected_name()=="Recent Projects")
            {
                for( i = 0; i <  rProjectListSize && !projectOpened; i++)
                {
                    if( recentProjectsList[i]!=NULL)
                    {
                        if( recentProjectsList[i]->is_clicked() )
                        {
                            GPE_MAIN_GUI->open_project(recentProjectsList[i]->descriptionText );
                            projectOpened= true;
                        }
                    }
                }
            }
            else if( projectOpened==false && projectsCategoryTabs->get_selected_name()=="Example Projects" )
            {
                for( i = 0; i <  exampleProjectListSize && !projectOpened; i++)
                {
                    if( examplesProjectsList[i]!=NULL)
                    {
                        if( examplesProjectsList[i]->is_clicked() )
                        {
                            GPE_MAIN_GUI->open_project(examplesProjectsList[i]->descriptionText );
                            projectOpened= true;
                        }
                    }
                }
            }
            else if( projectOpened==false)
            {
                for( i = 0; i <  favProjectListSize && !projectOpened; i++)
                {
                    if( foundProjectsList[i]!=NULL)
                    {
                        if( foundProjectsList[i]->is_clicked() )
                        {
                            GPE_MAIN_GUI->open_project(foundProjectsList[i]->descriptionText );
                            projectOpened = true;
                        }
                    }
                }
            }
        }
    }
    processedFirst = true;
}

void gamePencilProjectBrowserResource::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace,GPE_Rect *cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    GPE_Rect wholeRect;
    wholeRect.x = 0;
    wholeRect.y = 0;
    wholeRect.w = viewedSpace->w;
    wholeRect.h = viewedSpace->h;
    cam = GPE_find_camera(cam);
    viewedSpace = GPE_find_camera(viewedSpace);
    if( cam!=NULL && projectBrowserList!=NULL && projectsCategoryTabs!=NULL && viewedSpace!=NULL)
    {
        if( forceRedraw)
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
        }
        headerPageList->render_self(cRender,&headerViewedSpace,cam,forceRedraw);

        if( forceRedraw)
        {
            render_rectangle(cRender,projectsCategoryTabs->get_xpos(),projectsCategoryTabs->get_y2pos(),viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Header_Color,false);
        }
        cRender->reset_viewpoint();
        cRender->set_viewpoint( &subViewedSpace );
        projectBrowserList->render_self(cRender,&subViewedSpace,cam, forceRedraw);
        /*
        if(editorPageTabBar->get_selected_name()=="Platforms")
        {
            exportSettingsBar->render_self(cRender,&subViewedSpace,cam,forceRedraw);
        }*/
        cRender->reset_viewpoint();
        cRender->set_viewpoint( viewedSpace);
    }
}

void gamePencilProjectBrowserResource::save_resource(std::string alternatePath , int backupId)
{

}

bool gamePencilProjectBrowserResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{

}

void gamePencilProjectBrowserResource::load_example_projects_folder( )
{
    GPE_ToolPushButtonMultiLine * tempProjectButton = NULL;

    int i = 0;
    int j = 0;


    for( i = (int)examplesProjectsList.size()-1; i>=0; i--)
    {
        tempProjectButton = examplesProjectsList[i];
        if( tempProjectButton!=NULL)
        {
            delete tempProjectButton;
            tempProjectButton = NULL;
        }
    }
    examplesProjectsList.clear();
    std::string tempProjectName= "";
    std::string newMultiLine= "";
    std::string exampleProjectsLocation = APP_DIRECTORY_NAME+"examples";
    record_error("Browsing for example projects in ["+exampleProjectsLocation+"] folder");
    if( path_exists( exampleProjectsLocation ) )
    {
        quickProjectReader * qpr = new quickProjectReader();
        GPE_FileDirectory * exampleFolderDirectory = new GPE_FileDirectory();

        exampleFolderDirectory->open_directory( exampleProjectsLocation );

        GPE_File * tempFile = NULL;
        std::vector<std::string > fileFilterTypes;
        fileFilterTypes.push_back("gppf");

        exampleFolderDirectory->filter_directory(false,fileFilterTypes);

        bool projectLoadedAlready = false;
        if( exampleFolderDirectory->get_count() > 0 )
        {
            projectFolderLocationLabel->set_name(exampleProjectsLocation);
            projectFolderLocationLabel->descriptionText = exampleProjectsLocation;

            for( i = 0; i < exampleFolderDirectory->get_count(); i++)
            {
                tempFile = exampleFolderDirectory->get_file(i);
                if( tempFile!=NULL)
                {
                    tempProjectName = exampleProjectsLocation+"/"+tempFile->get_name();

                    if( (int)tempProjectName.size() > 0 && file_exists(tempProjectName) )
                    {
                        projectLoadedAlready = false;
                        for( j = (int)recentProjectsList.size()-1; j>=0; j--)
                        {
                            tempProjectButton = recentProjectsList[j];
                            if( tempProjectButton!=NULL && tempProjectButton->descriptionText==tempProjectName )
                            {
                                projectLoadedAlready = true;
                                break;
                            }
                        }

                        if( !projectLoadedAlready && qpr->review_project(tempProjectName) )
                        {
                            if( file_exists(qpr->detectedIcon) ==false)
                            {
                                qpr->detectedIcon =APP_DIRECTORY_NAME+"resources/gamepencil_icon_72dpi.png";
                            }
                            newMultiLine = qpr->detectedName;
                            if((int)qpr->detectedVersion.size() > 0 )
                            {
                                newMultiLine+="\nVersion: "+qpr->detectedVersion+"  |  "+qpr->detectedLanguage+" Project";
                            }
                            newMultiLine+="\nLocation: "+tempProjectName;
                            tempProjectButton = new GPE_ToolPushButtonMultiLine(0,0,qpr->detectedIcon,newMultiLine, qpr->detectedLocation);
                            examplesProjectsList.push_back(tempProjectButton);
                        }
                    }
                }
            }
        }
        delete exampleFolderDirectory;
        exampleFolderDirectory = NULL;

        delete qpr;
        qpr = NULL;
    }
}

void gamePencilProjectBrowserResource::load_projecs_folder( std::string projectsDirectory)
{
    GPE_ToolPushButtonMultiLine * tempProjectButton = NULL;

    int i = 0;
    int j = 0;
    std::string tempProjectName = "";
    std::string newMultiLine = "";

    for( i = (int)foundProjectsList.size()-1; i>=0; i--)
    {
        tempProjectButton = foundProjectsList[i];
        if( tempProjectButton!=NULL)
        {
            delete tempProjectButton;
            tempProjectButton = NULL;
        }
    }
    foundProjectsList.clear();

    record_error("Browsing for  projects in ["+projectsDirectory+"] folder");
    if( path_exists(projectsDirectory) )
    {
        quickProjectReader * qpr = new quickProjectReader();
        GPE_FileDirectory * favFolderDirectory = new GPE_FileDirectory();
        foundProjectsDirectory = projectsDirectory;

        favFolderDirectory->open_directory(projectsDirectory);

        GPE_File * tempFile = NULL;
        std::vector<std::string > fileFilterTypes;
        fileFilterTypes.push_back("gppf");

        favFolderDirectory->filter_directory(false,fileFilterTypes);

        bool projectLoadedAlready = false;
        if( favFolderDirectory->get_count() > 0 )
        {
            projectFolderLocationLabel->set_name(projectsDirectory);
            projectFolderLocationLabel->descriptionText = projectsDirectory;
            if( MAIN_EDITOR_SETTINGS!=NULL )
            {
                MAIN_EDITOR_SETTINGS->projectFolderListLocation = projectsDirectory;
            }
            for( i = 0; i < favFolderDirectory->get_count(); i++)
            {
                tempFile = favFolderDirectory->get_file(i);
                if( tempFile!=NULL)
                {
                    tempProjectName = projectsDirectory+"/"+tempFile->get_name();

                    if( (int)tempProjectName.size() > 0 && file_exists(tempProjectName) )
                    {
                        projectLoadedAlready = false;
                        for( j = (int)recentProjectsList.size()-1; j>=0; j--)
                        {
                            tempProjectButton = recentProjectsList[j];
                            if( tempProjectButton!=NULL && tempProjectButton->descriptionText==tempProjectName )
                            {
                                projectLoadedAlready = true;
                                break;
                            }
                        }

                        if( !projectLoadedAlready && qpr->review_project(tempProjectName) )
                        {
                            if( file_exists(qpr->detectedIcon) ==false)
                            {
                                qpr->detectedIcon =APP_DIRECTORY_NAME+"resources/gamepencil_icon_72dpi.png";
                            }
                            newMultiLine = qpr->detectedName;
                            if((int)qpr->detectedVersion.size() > 0 )
                            {
                                newMultiLine+="\nVersion: "+qpr->detectedVersion+"  |  "+qpr->detectedLanguage+" Project";
                            }
                            newMultiLine+="\nLocation: "+tempProjectName;
                            tempProjectButton = new GPE_ToolPushButtonMultiLine(0,0,qpr->detectedIcon,newMultiLine, qpr->detectedLocation);
                            foundProjectsList.push_back(tempProjectButton);
                        }
                    }
                }
            }
        }
        delete favFolderDirectory;
        favFolderDirectory = NULL;

        delete qpr;
        qpr = NULL;
    }
}

