/*
game_object_resource.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
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

-Game Pencil Engine <https://create.pawbyte.com>


*/

#include "game_object_resource.h"

objectResParentChainLink::objectResParentChainLink( int origObjectType)
{
    chainObjectType = origObjectType;
}

objectResParentChainLink::~objectResParentChainLink()
{
    heldChains.clear();
}

void objectResParentChainLink::grab_chain( objectResParentChainLink * chainIn )
{
    if( chainIn!=NULL)
    {
        heldChains.push_back( chainIn->chainObjectType );
        for( int i = 0; i < (int)chainIn->heldChains.size(); i++)
        {
            heldChains.push_back( chainIn->heldChains[i] );
        }
    }
}

bool objectResParentChainLink::is_inchain(int chainId)
{
    for( int i = 0; i < (int)heldChains.size(); i++)
    {
        if( heldChains[i]==chainId)
        {
            return true;
        }
    }
    return false;
}

gameObjectResource::gameObjectResource(GPE_GeneralResourceContainer * pFolder)
{
    resourcePostProcessed = true;
    projectParentFolder = pFolder;
    imageIndex = 0;
    editorMode = 0;
    int i = 0;

    animationIndex = -1;
    animInEditor = NULL;

    if(saveResourceButton!=NULL)
    {
        saveResourceButton->disable_self();
    }



    headerCodeArea = new GPE_TextAreaInputBasic(true);
    sourceCodeArea = new GPE_TextAreaInputBasic(true);

    objModeSelector = new GPE_SelectBoxBasic("Object Functions");
    objModeSelector->add_option("Components",OBJ_MODE_COMPONENTS,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/tags.png"),NULL,0,false,true);
    objModeSelector->add_option("Header Code(C++)",OBJ_MODE_HEADER,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/superscript.png"),NULL,0,false,true);
    objModeSelector->add_option("Source Code",OBJ_MODE_SOURCE,guiRCM->texture_add(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/code.png"),NULL,0,false,true);

    if( projectParentFolder!=NULL )
    {
        animationField = new GPE_DropDown_Resouce_Menu( "Default Animation",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ANIMATION]+"s") );
        loadResourceButton->set_width(animationField->get_width() );
        confirmResourceButton->set_width(animationField->get_width() );
        parentObjectField = new GPE_DropDown_Resouce_Menu( "Parent Object",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s" ) );
    }
    else
    {
        animationField = NULL;
        parentObjectField = NULL;
    }

    customComponentsGuiList = new GPE_GuiElementList();
    customComponentsGuiList->panelAlignType = GPE_PANEL_ALIGN_FULL_LEFT;
    componentsMainGuiLabel = new GPE_Label_Text("Custom Components","Custom components, useful for scene editor");
    componentsGuiLabel = new GPE_Label_Text("Custom Components","Custom components, useful for scene editor");
    inheritParentComponentButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/users.png","Inherit Components","Inherit components from parent object class");
    resetComponentsButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/trash.png","Reset Components","Clears all components form this object class");
    addNewComponentDropDown = new GPE_DropDown_Menu( "Add Component",false);
    setup_object_components ( addNewComponentDropDown );
    addNewComponentDropDown->set_width(224);

    removeComponentButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/minus.png","Remove Component");
    editCompnentButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/edit.png","Edit Component");
    componentSettingsButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/trash.png","Component Settings");

    checkBoxIsVisible = new GPE_CheckBoxBasic("Visible","The actor is rendered on screen", true);
    checkBoxNeedsCamera = new GPE_CheckBoxBasic("Needs Camera","Object will only perform logic if in view of any camera [Highly recommended for most objects]", true);
    checkBoxIsMoveable = new GPE_CheckBoxBasic("Mobile","Uses engine default physics", true);
    checkBoxIsContinuous = new GPE_CheckBoxBasic("Continuous","Object continues throughout game scenes[Keep unchecked for most objects]", false);
    resourceNameLabel = new GPE_Label_Title("Object","");
}

gameObjectResource::~gameObjectResource()
{
    if( animationField!=NULL)
    {
        delete animationField;
        animationField = NULL;
    }
    if( resourceNameLabel!=NULL)
    {
        delete resourceNameLabel;
        resourceNameLabel = NULL;
    }
    if( parentObjectField!=NULL)
    {
        delete parentObjectField;
        parentObjectField= NULL;
    }

    if( objModeSelector!=NULL)
    {
        delete objModeSelector;
        objModeSelector = NULL;
    }

    if( checkBoxNeedsCamera!=NULL)
    {
        delete checkBoxNeedsCamera;
        checkBoxNeedsCamera = NULL;
    }
    if( checkBoxIsMoveable!=NULL)
    {
        delete checkBoxIsMoveable;
        checkBoxIsMoveable = NULL;
    }
    if( checkBoxIsVisible!=NULL)
    {
        delete checkBoxIsVisible;
        checkBoxIsVisible = NULL;
    }
    if( checkBoxIsContinuous!=NULL)
    {
        delete checkBoxIsContinuous;
        checkBoxIsContinuous = NULL;
    }
    int i = 0;

    if( headerCodeArea!=NULL)
    {
        delete headerCodeArea;
        headerCodeArea = NULL;
    }

    if( sourceCodeArea!=NULL)
    {
        delete sourceCodeArea;
        sourceCodeArea = NULL;
    }


    //Deletes the components
    GPE_ObjectComponent * tempComponent = NULL;
    for( i = (int)customComponentRealList.size()-1; i>=0; i--)
    {
        tempComponent = customComponentRealList[i];
        if( tempComponent!=NULL )
        {
            delete tempComponent;
            tempComponent = NULL;
            customComponentRealList.erase(customComponentRealList.begin()+i);
        }
    }
    customComponentRealList.clear();

    //Component stuffs
    if( componentsMainGuiLabel!=NULL)
    {
        delete componentsMainGuiLabel;
        componentsMainGuiLabel = NULL;
    }
    if( componentsGuiLabel!=NULL)
    {
        delete componentsGuiLabel;
        componentsGuiLabel = NULL;
    }
    if( customComponentsGuiList!=NULL)
    {
        delete customComponentsGuiList;
        customComponentsGuiList = NULL;
    }
    if( inheritParentComponentButton!=NULL)
    {
        delete inheritParentComponentButton;
        inheritParentComponentButton = NULL;
    }
    if( resetComponentsButton!=NULL)
    {
        delete resetComponentsButton;
        resetComponentsButton = NULL;
    }
    if( addNewComponentDropDown!=NULL)
    {
        delete addNewComponentDropDown;
        addNewComponentDropDown = NULL;
    }
    if( removeComponentButton!=NULL)
    {
        delete removeComponentButton;
        removeComponentButton = NULL;
    }
    if( editCompnentButton!=NULL)
    {
        delete editCompnentButton;
        editCompnentButton = NULL;
    }
    if( componentSettingsButton!=NULL)
    {
        delete componentSettingsButton;
        componentSettingsButton = NULL;
    }
}

void gameObjectResource::add_component(std::string newComponentData)
{
    GPE_GeneralGuiElement * newComponentMeat = add_gui_component(newComponentData);
    if( newComponentMeat!=NULL)
    {
        GPE_ObjectComponent * tempComponent = NULL;
        for( int iComponents = 0; iComponents < (int)customComponentRealList.size(); iComponents++)
        {
            tempComponent = customComponentRealList.at(iComponents);
            if( tempComponent!=NULL)
            {
                if( tempComponent->get_name()==newComponentMeat->get_name() )
                {
                    //component already exists so delete it and return
                    delete newComponentMeat;
                    newComponentMeat = NULL;
                    return;
                }
            }
        }
        GPE_ObjectComponent * newComponent= new GPE_ObjectComponent();
        if( newComponent!=NULL)
        {
            newComponent->set_component( newComponentMeat);
            customComponentRealList.push_back(newComponent);
        }
        else if( newComponentMeat!=NULL)
        {
            delete newComponentMeat;
            newComponentMeat = NULL;
        }
    }
}

bool gameObjectResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    bool buildSuccessful = true;
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        int iFunc = 0;
        int paramLine = 0;
        int iErrorLine = 0;
        std::string nestedTabsStr = generate_tabs( leftTabAmount  );
        std::string nestedTabsStrObjFunc = generate_tabs( leftTabAmount +1 );

        //*fileTarget << nestedTabsStr << "var " << get_name() << " = " << exportBuildGlobalId << ";\n";
        *fileTarget << nestedTabsStr << "GPE._obj_" << get_name() << " =  (function (xPosIn, yPosIn,objectLayerId) \n";
        *fileTarget << nestedTabsStr << "{ \n";
        *fileTarget << nestedTabsStrObjFunc << "function _obj_" << get_name() <<" (xPosIn, yPosIn,objectLayerId) \n";
        *fileTarget << nestedTabsStrObjFunc << "{ \n";
        GPE_GeneralResourceContainer * parentRes = parentObjectField->get_selected_container();
        if( parentRes!=NULL)
        {
            *fileTarget << nestedTabsStrObjFunc << "    GPE._obj_" << parentRes->get_name() <<".call(this,xPosIn, yPosIn,objectLayerId); \n";
        }
        else
        {
            *fileTarget << nestedTabsStrObjFunc << "    GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); \n";
        }
        *fileTarget << nestedTabsStrObjFunc << "    this.gpeObjectType ="<< exportBuildGlobalId << "; \n";
        GPE_GeneralResourceContainer * animationRes = animationField->get_selected_container();
        if( animationRes!=NULL)
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.init_animation(" << animationRes->exportBuildGlobalId << ");\n";
        }
        else
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.init_animation(-1);\n";
        }

        if( checkBoxIsVisible!=NULL)
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.isVisible = " << checkBoxIsVisible->is_clicked() << ";\n";
        }
        else
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.isVisible = false;\n";
        }
        //
        if( checkBoxIsContinuous!=NULL)
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.gpeIsContinuousObject = " << checkBoxIsContinuous->is_clicked() << ";\n";
        }
        else
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.gpeIsContinuousObject = false;\n";
        }
        //
        if( checkBoxNeedsCamera!=NULL)
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.isViewIndependent = " << !checkBoxNeedsCamera->is_clicked() << ";\n";
        }
        else
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.isViewIndependent = false;\n";
        }

        if( !sourceCodeArea->compile_into_code( fileTarget, leftTabAmount+2,true,true) )
        {
            for( iErrorLine=0; iErrorLine < (int)sourceCodeArea->foundSyntaxErrors.size(); iErrorLine++)
            {
                GPE_Main_Logs->log_build_error(get_name()+"'s SRC: "+sourceCodeArea->foundSyntaxErrors[iErrorLine] );
                buildSuccessful = false;
            }
        }
        //returns object and gives it its prototype(parent object)
        if( parentRes!=NULL)
        {
            *fileTarget << nestedTabsStrObjFunc << "_obj_" << get_name() << ".prototype = new GPE._obj_" << parentRes->get_name() <<"(xPosIn, yPosIn); \n";
        }
        else
        {
            *fileTarget << nestedTabsStrObjFunc << "_obj_" << get_name() << ".prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); \n";
        }
        *fileTarget << nestedTabsStrObjFunc << "return _obj_" << get_name() <<"; \n";
        *fileTarget << nestedTabsStr << "}());\n\n";
    }
    return buildSuccessful;
}

bool gameObjectResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void gameObjectResource::compile_cpp()
{

}

GPE_GeneralResourceContainer * gameObjectResource::get_parent_resource()
{
    if( parentObjectField!=NULL)
    {
        if( parentObjectField->get_selected_id()< 0 || parentObjectField->get_selected_id()==get_global_rid() )
        {
            parentObjectField->set_name("Parent Object");
            parentObjectField->set_selection( -1);
            return NULL;
        }
        return parentObjectField->get_selected_container();
    }
    return NULL;
}

GPE_Animation * gameObjectResource::get_resource_animation()
{
    if( projectParentFolder!=NULL)
    {
        GPE_GeneralResourceContainer * allanimationsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_ANIMATION]+"s");
        if( allanimationsFolder!=NULL)
        {
            GPE_GeneralResourceContainer *objTypeContainer = allanimationsFolder->find_resource_from_id( animationIndex);
            if( objTypeContainer!=NULL)
            {
                return objTypeContainer->get_resource_animation();
            }
        }
    }
    return NULL;
}

bool gameObjectResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

void gameObjectResource::integrate_into_syntax()
{
    if( CURRENT_PROJECT!=NULL)
    {
        CURRENT_PROJECT->add_project_keyword(resourceName,"Project "+RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+" Resource",-1,"");

    }
}

void gameObjectResource::manage_components(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && customComponentsGuiList!=NULL && renameBox!=NULL)
    {
        customComponentsGuiList->set_coords(0, 0);
        customComponentsGuiList->set_width(viewedSpace->w );
        customComponentsGuiList->set_height(viewedSpace->h  );
        customComponentsGuiList->barXPadding = GENERAL_GPE_GUI_PADDING;
        customComponentsGuiList->barXMargin = 0;

        customComponentsGuiList->clear_list();
        //Start of New Code
        customComponentsGuiList->add_gui_element(renameBox,true);
        customComponentsGuiList->add_gui_element(animationField,true);
        customComponentsGuiList->add_gui_element(parentObjectField,true);
        //customComponentsGuiList->add_gui_element(loadResourceButton,true);
        customComponentsGuiList->add_gui_element(checkBoxNeedsCamera,true);
        customComponentsGuiList->add_gui_element(checkBoxIsMoveable,true);
        customComponentsGuiList->add_gui_element(checkBoxIsVisible,true);
        customComponentsGuiList->add_gui_element(checkBoxIsContinuous,true);
        //End of New Code

        customComponentsGuiList->add_gui_element(componentsMainGuiLabel,true);
        customComponentsGuiList->add_gui_element(resetComponentsButton,true);
        customComponentsGuiList->add_gui_element(inheritParentComponentButton,true);
        customComponentsGuiList->add_gui_element(addNewComponentDropDown,true);

        customComponentsGuiList->add_gui_element(componentsGuiLabel,true);

        GPE_ObjectComponent * tempCustomComponent = NULL;
        GPE_ObjectComponent * otherTempCustomComponent = NULL;
        int iRealCompo = 0;
        int jRealCompo = 0;

        for(  iRealCompo = 0; iRealCompo < (int)customComponentRealList.size(); iRealCompo++)
        {
            tempCustomComponent = customComponentRealList.at(iRealCompo);
            if( tempCustomComponent!=NULL)
            {
                customComponentsGuiList->add_gui_element(tempCustomComponent->get_component(),false);
                customComponentsGuiList->add_gui_element(tempCustomComponent->get_gear(),true);
            }
        }
        customComponentsGuiList->process_self(viewedSpace,cam);

        for(  iRealCompo = (int)customComponentRealList.size()-1; iRealCompo>=0; iRealCompo--)
        {
            tempCustomComponent = customComponentRealList.at(iRealCompo);
            if( tempCustomComponent!=NULL)
            {
                if(tempCustomComponent->get_gear()!=NULL)
                {
                    if( tempCustomComponent->get_gear()->is_clicked() )
                    {
                        customComponentsGuiList->clear_list();
                        GPE_open_context_menu(-1,-1,256);
                        MAIN_CONTEXT_MENU->add_menu_option("Rename Component",0,NULL,-1,NULL,true,true);
                        MAIN_CONTEXT_MENU->add_menu_option("Delete Component",1,NULL,-1,NULL,false,true);
                        if( tempCustomComponent->get_type()=="dropdown" || tempCustomComponent->get_type()=="selectbox")
                        {
                            MAIN_CONTEXT_MENU->add_menu_option("Add Option",2,NULL,-1,NULL,false,true);
                            MAIN_CONTEXT_MENU->add_menu_option("Remove Option",3,NULL,-1,NULL,false,true);
                        }
                        MAIN_CONTEXT_MENU->add_menu_option("Exit Menu",4,NULL,-1,NULL,true,true);
                        int menuSelection = GPE_Get_Context_Result();
                        if( menuSelection==0)
                        {
                            //rename
                            std::string newComponentName = get_string_from_popup("Name your new component","Must be unique!","");
                            bool componentNameInUseAlready = false;
                            if( newComponentName.size() > 0 )
                            {
                                if( tempCustomComponent->get_type()=="labeltext" || is_alnum(newComponentName,false,true) )
                                {
                                    for(  jRealCompo = 0; jRealCompo < (int)customComponentRealList.size(); jRealCompo++)
                                    {
                                        if( jRealCompo!=iRealCompo)
                                        {
                                            otherTempCustomComponent = customComponentRealList.at(jRealCompo);
                                            if( otherTempCustomComponent!=NULL)
                                            {
                                                if( otherTempCustomComponent->get_name() ==newComponentName )
                                                {
                                                    componentNameInUseAlready = true;
                                                }
                                            }
                                        }
                                    }
                                    if( !componentNameInUseAlready)
                                    {
                                        tempCustomComponent->rename_component( newComponentName );
                                    }
                                }
                            }
                        }
                        else if( menuSelection==1)
                        {
                            if( GPE_Display_Basic_Prompt("[WARNING]Object Component deletion?","Are you sure you will like to delete the ["+tempCustomComponent->get_name()+"] component? This action is irreversible!")==DISPLAY_QUERY_YES)
                            {
                                //delete
                                delete tempCustomComponent;
                                tempCustomComponent = NULL;
                                customComponentRealList.erase( customComponentRealList.begin()+iRealCompo);
                            }
                        }
                        else if( menuSelection==2)
                        {
                            if( tempCustomComponent->get_type()=="dropdown" || tempCustomComponent->get_type()=="selectbox")
                            {
                                std::string extraDataString = "[menu]";
                                std::string newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                while( newMenuOptionName.size() > 0 && (  is_alnum(newMenuOptionName,true,true) ) )
                                {
                                    extraDataString+="[option]"+newMenuOptionName+":"+newMenuOptionName+":-1[/option]";
                                    newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                }
                                extraDataString+="[/menu]0,0";
                                tempCustomComponent->load_data(extraDataString);
                            }
                        }
                        else if( menuSelection==3)
                        {
                            if( tempCustomComponent->get_type()=="dropdown" || tempCustomComponent->get_type()=="selectbox")
                            {
                                std::string extraDataString = "";
                                std::string newMenuOptionName = get_string_from_popup("Enter option to remove.","option name:","");
                                if( newMenuOptionName.size() > 0 && (  is_alnum(newMenuOptionName,true,true) ) )
                                {
                                    extraDataString+="[option]"+newMenuOptionName+"[/option]";
                                    tempCustomComponent->remove_data(extraDataString);
                                }
                            }
                        }
                        //manage_components(viewedSpace, cam);
                        break;
                    }
                }
            }
        }
    }
}

void gameObjectResource::open_code(int lineNumb, int colNumb, std::string codeTitle)
{
    if( (int)codeTitle.size() > 0 )
    {
        GPE_TextAreaInputBasic * foundTextArea = NULL;

        if( codeTitle == "header" )
        {
            foundTextArea = headerCodeArea;
            objModeSelector->set_selection( OBJ_MODE_HEADER);
        }
        else
        {
            foundTextArea = sourceCodeArea;
            objModeSelector->set_selection( OBJ_MODE_SOURCE);
        }

        if( foundTextArea!=NULL && foundTextArea->has_content() )
        {
            foundTextArea->scroll_to_pos(lineNumb, colNumb);
        }
    }
}

void gameObjectResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();

    if( checkBoxNeedsCamera!=NULL)
    {
        checkBoxNeedsCamera->prerender_self( );
    }
    if( checkBoxIsMoveable!=NULL)
    {
        checkBoxIsMoveable->prerender_self( );
    }
    if( checkBoxIsVisible!=NULL)
    {
        checkBoxIsVisible->prerender_self( );
    }
    if( checkBoxIsContinuous!=NULL)
    {
        checkBoxIsContinuous->prerender_self( );
    }
}

void gameObjectResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Game Object", resourceName );
        }

        std::string otherColContainerName = "";

        std::string newameObjFilename = "";
        std::string headerFileName ="";
        std::string sourceFileName ="";

        std::string soughtDir = file_to_dir(parentProjectName)+"/gpe_project/resources/objects/";
        if( file_exists(alternatePath) )
        {
            newameObjFilename = alternatePath;
            soughtDir = get_path_from_file(newameObjFilename);
        }
        else
        {
            newameObjFilename = soughtDir + resourceName+".gpf";
        }

        headerFileName = soughtDir + resourceName+".h";
        sourceFileName = soughtDir + resourceName+".src";

        if( CURRENT_PROJECT !=NULL)
        {
            if( CURRENT_PROJECT->get_project_language_id() == PROGRAM_LANGUAGE_CPP )
            {
                sourceFileName = soughtDir+resourceName+".cpp";
            }
        }

        std::ifstream gameObjFileIn( newameObjFilename.c_str() );

        //Used for importing older object class embedded in same file...
        bool inCodeSection = false; //Default to false so nothing happens til a function is entered
        std::string nestedFunctionStr = generate_tabs( 1  );
        std::string nestedContentStr = generate_tabs( 2  );
        //End of variables used to pre-format older JS format embedded in same file...

        //If the level file could be loaded
        if( !gameObjFileIn.fail() )
        {
            //makes sure the file is open
            if (gameObjFileIn.is_open())
            {
                int equalPos = 0;
                int colonSeperaterPos = 0;
                std::string firstChar="";
                std::string section="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string foundReturnTypeString="";
                std::string foundParametersString ="";
                std::string currLine="";
                std::string currLineToBeProcessed;


                float foundFileVersion = 0;
                GPE_TextAreaInputBasic * textAreaToLoad = sourceCodeArea;
                if( CURRENT_PROJECT!=NULL )
                {
                    if( CURRENT_PROJECT->get_project_language_id() == PROGRAM_LANGUAGE_JS )
                    {

                    }
                }

                while ( gameObjFileIn.good() )
                {
                    getline (gameObjFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = trim_left_inplace(currLine);
                    currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

                    if( foundFileVersion <=0)
                    {
                        //Empty Line skipping is only allowed at the top of the file
                        if(!currLineToBeProcessed.empty() )
                        {
                            //Comment skipping is only allowed at the top of the file
                            if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                            {
                                //searches for an equal character and parses through the variable
                                equalPos=currLineToBeProcessed.find_first_of("=");
                                if(equalPos!=(int)std::string::npos)
                                {
                                    //if the equalPos is present, then parse on through and carryon
                                    keyString = currLineToBeProcessed.substr(0,equalPos);
                                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( keyString=="Version")
                                    {
                                        foundFileVersion = string_to_float(valString);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion <= 2)
                    {
                        //Begin processing the file.
                        if(!currLineToBeProcessed.empty() )
                        {
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                                //Seeks for new textArea to edit based on older format
                                if( string_starts( keyString, "[[[") )
                                {
                                    if( inCodeSection )
                                    {
                                        textAreaToLoad->add_line(nestedFunctionStr + "}");
                                        textAreaToLoad->add_line("");
                                    }

                                    if( keyString=="[[[Function")
                                    {
                                        valString = split_first_string(valString,']');
                                        if( textAreaToLoad!=NULL)
                                        {
                                            textAreaToLoad->add_line(nestedFunctionStr + "this."+valString + " = function()" );
                                            textAreaToLoad->add_line(nestedFunctionStr + "{");
                                        }
                                        inCodeSection = true;
                                    }
                                    else if( keyString=="[[[TimedFunction")
                                    {
                                        colonSeperaterPos=valString.find_first_of(":");
                                        if(colonSeperaterPos!=(int)std::string::npos)
                                        {
                                            subValString = valString.substr(colonSeperaterPos+1);
                                            subValString = split_first_string(subValString,']');
                                            valString = valString.substr(0,colonSeperaterPos);
                                        }
                                        else
                                        {
                                            valString = split_first_string(valString,']');
                                        }
                                        if( textAreaToLoad!=NULL)
                                        {
                                            textAreaToLoad->add_line(nestedFunctionStr + "this."+valString  + " = function()" );
                                            textAreaToLoad->add_line(nestedFunctionStr + "{");
                                        }
                                        inCodeSection = true;
                                    }
                                    else if( keyString=="[[[CollisionFunction" || keyString=="[[[CollideFunction")
                                    {
                                        valString = split_first_string(valString,']');
                                        if( textAreaToLoad!=NULL)
                                        {
                                            textAreaToLoad->add_line(nestedFunctionStr + "this.collide_with_"+valString  + " = function()" );
                                            textAreaToLoad->add_line(nestedFunctionStr + "{");
                                        }
                                        inCodeSection = true;
                                    }
                                    else if( keyString=="[[[SpecialFunction")
                                    {
                                        valString = split_first_string(valString,']');
                                        if( textAreaToLoad!=NULL)
                                        {
                                            textAreaToLoad->add_line(nestedFunctionStr + "this."+valString  + " = function()" );
                                            textAreaToLoad->add_line(nestedFunctionStr + "{");
                                        }
                                        inCodeSection = true;
                                    }
                                    else if( keyString=="[[[CustomFunction")
                                    {
                                        //option 5
                                        subValString = split_first_string(valString,',');

                                        foundReturnTypeString = split_first_string(valString,',');

                                        foundParametersString = split_first_string(valString,']');

                                        textAreaToLoad->add_line(nestedFunctionStr + "this."+subValString  + " = function( "+ foundParametersString +")" );

                                        textAreaToLoad->add_line(nestedFunctionStr + "{");
                                        inCodeSection = true;
                                    }
                                }
                                //loads the rest of the file if all text areas are closed at this poin
                                else if( keyString=="ResourceName")
                                {
                                        renameBox->set_string(valString);
                                }
                                else if( keyString=="animId" || keyString=="Animation" || keyString=="animationId" )
                                {
                                    animationIndex = -1;
                                    if( animationField!=NULL)
                                    {
                                        animationField->set_selected_target(valString);
                                        animationIndex = animationField->get_selected_id();
                                    }
                                }
                                else if( keyString=="ParentGameObjectId")
                                {
                                    parentObjectField->set_selected_target( valString );
                                    parentObjectId = parentObjectField->get_selected_id();

                                }
                                else if( keyString=="IsContinuous")
                                {
                                    checkBoxIsContinuous->set_clicked( string_to_bool(valString) );
                                }
                                else if( keyString=="IsMoveable")
                                {
                                    checkBoxIsMoveable->set_clicked( string_to_bool(valString) );
                                }
                                else if( keyString=="IsVisible")
                                {
                                    checkBoxIsVisible->set_clicked( string_to_bool(valString) );
                                }
                                else if( keyString=="NeedsCamera")
                                {
                                    checkBoxNeedsCamera->set_clicked( string_to_bool(valString) );
                                }
                                else if( keyString=="CustomComponent")
                                {
                                    add_component(valString);
                                }
                                else if( inCodeSection && textAreaToLoad!=NULL )
                                {
                                    textAreaToLoad->add_line( nestedContentStr + currLineToBeProcessed );
                                }
                            }
                            else if( inCodeSection && textAreaToLoad!=NULL )
                            {
                                textAreaToLoad->add_line( nestedContentStr + currLineToBeProcessed );
                            }
                        }
                    }
                    else
                    {
                        GPE_Report("Invalid FoundFileVersion ="+float_to_string(foundFileVersion)+".");
                    }
                }

                gameObjFileIn.close();

                //Used to pre-format older GPE end-of-file content
                if( inCodeSection )
                {
                    textAreaToLoad->add_line(nestedFunctionStr+ "}");
                    textAreaToLoad->add_line("");
                }
            }
        }

        headerCodeArea->import_text( headerFileName );
        headerCodeArea->init_save_history();

        sourceCodeArea->import_text( sourceFileName );
        sourceCodeArea->init_save_history();

        resourcePostProcessed = true;
    }
}

void gameObjectResource::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    //Processes the object's rename box 1st
    if(resourceNameLabel!=NULL)
    {
        if( renameBox!=NULL && renameBox->get_string()!=resourceName)
        {
            resourceNameLabel->set_name(renameBox->get_string()+"*");
        }
        else
        {
            resourceNameLabel->set_name(resourceName);
        }
    }

    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    preprocess_self();

    //Exits if we have a a bad variable
    if( cam == NULL || viewedSpace == NULL || objModeSelector == NULL )
    {
        return;
    }
    selectedMode = objModeSelector->get_selected_value();

    int functionI = 0;
    GPE_TextAreaInputBasic *  fTextArea = NULL;

    if( PANEL_GENERAL_EDITOR!=NULL )
    {
        PANEL_GENERAL_EDITOR->add_gui_element(resourceNameLabel,true);
        PANEL_GENERAL_EDITOR->add_gui_element(objModeSelector,true);
        PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,false);
        //GPE_Report("Attempting to process with code section found "+int_to_string(codeSection) );
        PANEL_GENERAL_EDITOR->process_self(NULL, NULL);
    }
    else
    {
        confirmResourceButton->set_clicked( false );
    }

    if( confirmResourceButton->is_clicked() || (input->check_keyboard_down(kb_ctrl) && input->check_keyboard_released(kb_s) ) )
    {
        save_resource();
    }

        //
    if( selectedMode == OBJ_MODE_COMPONENTS )
    {
        manage_components(viewedSpace, cam);
        if( loadResourceButton!=NULL)
        {
            if( loadResourceButton->is_clicked() )
            {

            }
        }
        if(renameBox!=NULL)
        {
            if( renameBox->get_string()!=resourceName)
            {
                isModified = true;
            }
        }

        if( parentObjectField!=NULL)
        {
            if(parentObjectField->get_name()!="Parent Object" && parentObjectField->just_activated() )
            {
                //parentObjectField->set_name("Parent Object");
                //parentObjectField->set_selection( -1);
            }
            if( parentObjectField->get_selected_id()< 0 || parentObjectField->get_selected_id()==get_global_rid() )
            {
                parentObjectField->set_name("Parent Object");
                parentObjectField->set_selection( -1);
            }
        }


        if( inheritParentComponentButton!=NULL && parentObjectField!=NULL)
        {
            if( inheritParentComponentButton->is_clicked() )
            {
                GPE_GeneralResourceContainer * objectParent = parentObjectField->get_selected_container();
                std::string inheritedComponentData = "";
                GPE_GeneralGuiElement * inheritedComponent = NULL;
                bool componentAlreadyExists = false;

                if( objectParent!=NULL)
                {
                    generalGameResource * parentGenRes = objectParent->get_held_resource();
                    if( parentGenRes!=NULL)
                    {
                        gameObjectResource* parentObjRes = (gameObjectResource*)parentGenRes;
                        if( parentObjRes!=NULL)
                        {
                            int jCompo = 0;
                            GPE_ObjectComponent * myTempCustomComponent = NULL;
                            GPE_ObjectComponent * parentTempCustomComponent = NULL;
                            for( int iCompo = 0; iCompo < (int)parentObjRes->customComponentRealList.size(); iCompo++)
                            {
                                parentTempCustomComponent = parentObjRes->customComponentRealList.at(iCompo);
                                componentAlreadyExists = false;
                                if( parentTempCustomComponent!=NULL)
                                {
                                    for( jCompo = 0; jCompo < (int)customComponentRealList.size(); jCompo++)
                                    {
                                        myTempCustomComponent = customComponentRealList.at(jCompo);
                                        if( myTempCustomComponent!=NULL)
                                        {
                                            if(myTempCustomComponent->get_name()==parentTempCustomComponent->get_name() )
                                            {
                                                componentAlreadyExists = true;
                                            }
                                        }
                                    }

                                    if( !componentAlreadyExists && parentTempCustomComponent->get_component()!=NULL)
                                    {
                                        //Copy Parent Component Property to Child...

                                        inheritedComponentData = parentTempCustomComponent->get_component()->get_data();
                                        inheritedComponent = add_gui_component(inheritedComponentData);
                                        if( inheritedComponent!=NULL )
                                        {
                                            GPE_ObjectComponent * newComponent = new GPE_ObjectComponent();
                                            if( newComponent!=NULL)
                                            {
                                                newComponent->set_component( inheritedComponent );
                                                customComponentRealList.push_back(newComponent);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if( addNewComponentDropDown!=NULL)
        {
            std::string newComponentType = addNewComponentDropDown->get_selected_tag();
            if( (int)newComponentType.size() > 0)
            {
                std::string newComponentName = get_string_from_popup("Name your new component","Must be unique!","");
                bool componentNameInUseAlready = false;
                if( newComponentName.size() > 0 )
                {
                    if( newComponentType=="labeltext" || is_alnum(newComponentName,false,true) )
                    {
                        GPE_ObjectComponent * myTempCustomComponent = NULL;
                        for( int iCompo = 0; iCompo < (int)customComponentRealList.size(); iCompo++)
                        {
                            myTempCustomComponent = customComponentRealList.at(iCompo);
                            if( myTempCustomComponent!=NULL)
                            {
                                if( myTempCustomComponent->get_name()==newComponentName)
                                {
                                    componentNameInUseAlready = true;
                                    break;
                                }
                            }
                        }
                        if( !componentNameInUseAlready)
                        {
                            std::string newComponentData = newComponentType+":"+newComponentName+"==|||==";

                            std::string extraDataString = "_blank";
                            if( newComponentType=="texturl")
                            {
                                std::string newurlLink = get_string_from_popup("Please enter a URL for this new component","Example: http://www.pawbyte.com","");
                                std::string newUrlDescription = get_string_from_popup("Please enter a description of your URL","Please describe your URL","");
                                if( (int)newurlLink.size() > 0)
                                {
                                    int fileColonPos = newurlLink.find("file://");
                                    int httpPos = newurlLink.find("http://");
                                    int httpsPos = newurlLink.find("https://");
                                    if( fileColonPos==(int)std::string::npos && httpPos==(int)std::string::npos && httpsPos==(int)std::string::npos )
                                    {
                                        newurlLink = "http://"+newurlLink;
                                    }
                                }
                                extraDataString = newUrlDescription+",,,"+newurlLink+",,,";
                            }
                            else if( newComponentType=="dropdown" || newComponentType=="radio")
                            {
                                extraDataString = "[menu]";
                                std::string newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                while( (int)newMenuOptionName.size() > 0 && (  is_alnum(newMenuOptionName,true,true) ) )
                                {
                                    extraDataString+="[option]"+newMenuOptionName+":"+newMenuOptionName+":-1[/option]";
                                    newMenuOptionName = get_string_from_popup("Enter an option name.","option name:","");
                                }
                                extraDataString+="[/menu]0,0";
                            }
                            else
                            {
                                int dropdownResPos = newComponentType.find("-resourcedropdown");
                                if( dropdownResPos!=(int)std::string::npos)
                                {
                                    std::string newDropDownType = get_substring(newComponentType,0,dropdownResPos);
                                    newComponentData = "resourcedropdown:"+newComponentName+"==|||==";
                                    extraDataString = newDropDownType+",,,-1,";
                                }
                            }
                            newComponentData = newComponentData+extraDataString;
                            GPE_GeneralGuiElement * newComponentField = add_gui_component(newComponentData);

                            if( newComponentField!=NULL)
                            {
                                GPE_ObjectComponent * newComponent = new GPE_ObjectComponent();
                                if( newComponent!=NULL)
                                {
                                    newComponent->set_component( newComponentField );
                                    customComponentRealList.push_back(newComponent);
                                }
                            }
                        }
                        else
                        {
                            display_user_alert("Invalid Component Name Entered","Please enter a unique component name with alpha-numeric values and no spaces!");
                        }
                    }
                    else
                    {
                        display_user_alert("Invalid Component Name Entered","Please enter a unique component name with alpha-numeric values and no spaces!");
                    }
                }
                addNewComponentDropDown->set_option_value(-1);
            }
        }

        if( resetComponentsButton!=NULL && resetComponentsButton->is_clicked() && (int)customComponentRealList.size() > 0)
        {
            if( GPE_Display_Basic_Prompt("Are you sure you want to delete all component elements?","There is no undo!")==DISPLAY_QUERY_YES)
            {
                GPE_ObjectComponent * myTempCustomComponent = NULL;
                for( int iCompo = 0; iCompo < (int)customComponentRealList.size(); iCompo++)
                {
                    myTempCustomComponent = customComponentRealList.at(iCompo);
                    if( myTempCustomComponent!=NULL)
                    {
                        delete myTempCustomComponent;
                        myTempCustomComponent = NULL;
                    }
                }
                customComponentRealList.clear();
                customComponentsGuiList->clear_list();
            }
        }
    }
    else
    {
        if(selectedMode == OBJ_MODE_HEADER)
        {
            fTextArea = headerCodeArea;
        }
        else
        {
            fTextArea = sourceCodeArea;
        }

        if( fTextArea!=NULL)
        {
            fTextArea->set_coords(0,0);
            fTextArea->set_width(viewedSpace->w );
            fTextArea->set_height(viewedSpace->h);
            /*if( basicFunctionsSelector->is_clicked() )
            {
                fTextArea->isInUse;
            }*/
            fTextArea->process_self(viewedSpace,cam);
        }
    }
}

void gameObjectResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL )
    {
        if( selectedMode == OBJ_MODE_COMPONENTS )
        {
            if( customComponentsGuiList!=NULL)
            {
                customComponentsGuiList->render_self( viewedSpace,cam);
            }
        }
        else
        {
            GPE_TextAreaInputBasic *  fTextArea = NULL;

            if( selectedMode == OBJ_MODE_HEADER )
            {
                fTextArea = headerCodeArea;
            }
            else
            {
                fTextArea = sourceCodeArea;
            }

            if(fTextArea!=NULL)
            {
                fTextArea->render_self( viewedSpace,cam);
            }
        }
    }
}


void gameObjectResource::save_resource(std::string alternatePath, int backupId)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Game Object", resourceName );
    }
    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string headerFileName ="";
    std::string sourceFileName ="";
    std::string soughtDir = get_path_from_file(alternatePath);
    if( path_exists(soughtDir) )
    {
        newFileOut = alternatePath;
        headerFileName = soughtDir+resourceName+".h";
        sourceFileName = soughtDir+resourceName+".src";

        if( CURRENT_PROJECT !=NULL)
        {
            if( CURRENT_PROJECT->get_project_language_id() == PROGRAM_LANGUAGE_CPP )
            {
                sourceFileName = soughtDir+resourceName+".cpp";
            }
        }
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = file_to_dir(parentProjectName)+"/gpe_project/resources/objects/";
        newFileOut = soughtDir + resourceName+".gpf";

        headerFileName = soughtDir + resourceName+".h";
        sourceFileName = soughtDir + resourceName+".src";
        if( CURRENT_PROJECT !=NULL)
        {
            if( CURRENT_PROJECT->get_project_language_id() == PROGRAM_LANGUAGE_CPP )
            {
                sourceFileName = soughtDir+resourceName+".cpp";
            }
        }
    }

    if( !usingAltSaveSource )
    {
        preprocess_self();
    }
    int i = 0;
    GPE_GeneralResourceContainer * objsParentContainer = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s" );
    GPE_GeneralResourceContainer * tOtherContainer = NULL;
    std::string otherColContainerName = "";
    animationIndex = -1;
    if( animationField!=NULL)
    {
        animationIndex = animationField->get_selected_id();
    }

    std::ofstream newGameObjFile( newFileOut.c_str() );
    if( !newGameObjFile.fail() )
    {
        resourceNameLabel->set_name(resourceName );
        //makes sure the file is open
        if (newGameObjFile.is_open())
        {
            write_header_on_file(&newGameObjFile);
            if( animationField!=NULL)
            {
                newGameObjFile << "animId=" << animationField->get_selected_name() << "\n";
            }
            if( parentObjectField!=NULL)
            {
                newGameObjFile << "ParentGameObjectId=" << parentObjectField->get_selected_name() << "\n";
            }
            if( checkBoxNeedsCamera!=NULL)
            {
                newGameObjFile << "NeedsCamera=" << checkBoxNeedsCamera->is_clicked() << "\n";
            }
            if( checkBoxIsMoveable!=NULL)
            {
                newGameObjFile << "IsMoveable=" << checkBoxIsMoveable->is_clicked() << "\n";
            }
            if( checkBoxIsVisible!=NULL)
            {
                newGameObjFile << "IsVisible=" << checkBoxIsVisible->is_clicked() << "\n";
            }
            if( checkBoxIsContinuous!=NULL)
            {
                newGameObjFile << "IsContinuous=" << checkBoxIsContinuous->is_clicked() << "\n";
            }

            //Saves the custom components
            newGameObjFile << "\n";
            GPE_ObjectComponent * tempComponentVariable = NULL;
            for( i = 0; i < (int)customComponentRealList.size(); i++)
            {
                tempComponentVariable = customComponentRealList[i];
                if( tempComponentVariable!=NULL)
                {
                    if( tempComponentVariable->get_component()!=NULL)
                    {
                        newGameObjFile << "CustomComponent="+tempComponentVariable->get_component()->get_data()+"\n";
                    }
                }
            }
            newGameObjFile.close();
        }
    }

    //Saves the object's code
    if( headerCodeArea->has_content() )
    {
        headerCodeArea->export_text( headerFileName);
    }

    if( sourceCodeArea->has_content() )
    {
        sourceCodeArea->export_text( sourceFileName );
    }

    if( !usingAltSaveSource)
    {
        isModified = false;
    }
}

int gameObjectResource::search_for_string(std::string needle)
{
    int foundStrings = 0;
    GPE_Main_Logs->log_general_comment("Searching ["+resourceName+"] object..");


    if( GPE_ANCHOR_GC!=NULL  )
    {
        int i = 0;
        GPE_ANCHOR_GC->searchResultProjectName = parentProjectName;
        GPE_ANCHOR_GC->searchResultResourceId = globalResouceIdNumber;
        GPE_ANCHOR_GC->searchResultResourceName = resourceName;
        if( headerCodeArea!=NULL && headerCodeArea->has_content() )
        {
            foundStrings+=headerCodeArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked(),true, "header" );
        }

        if( sourceCodeArea!=NULL && sourceCodeArea->has_content() )
        {
            foundStrings += sourceCodeArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked(),true, "source" );
        }
    }
    return foundStrings;
}

int gameObjectResource::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundStrings = 0;
    int tempFoundCount = 0;
    GPE_Main_Logs->log_general_comment("Searching ["+resourceName+"] object..");
    int i = 0;

    if( GPE_ANCHOR_GC!=NULL )
    {
        GPE_ANCHOR_GC->searchResultProjectName = parentProjectName;
        GPE_ANCHOR_GC->searchResultResourceId = globalResouceIdNumber;
        GPE_ANCHOR_GC->searchResultResourceName = resourceName;

        if( headerCodeArea!=NULL && headerCodeArea->has_content() )
        {
            tempFoundCount =headerCodeArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked(),false,"header" );
            if( tempFoundCount > 0)
            {
                foundStrings+=tempFoundCount;
                //displayMessageString = "Replaced "+int_to_string( headerCodeArea->replace_all_found(needle, newStr ) )+" copies.";                        }
            }
        }

        if( sourceCodeArea!=NULL && sourceCodeArea->has_content() )
        {
            tempFoundCount = sourceCodeArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked(),false, "source" );
            if( tempFoundCount > 0)
            {
                foundStrings+=tempFoundCount;
                //displayMessageString = "Replaced "+int_to_string( headerCodeArea->replace_all_found(needle, newStr ) )+" copies.";                        }
            }
        }
    }
    return foundStrings;
}


void gameObjectResource::update_box(int newX, int newY, int newW, int newH)
{

}

bool gameObjectResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    resourceNameLabel->set_name(renameBox->get_string() );
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            preprocess_self();
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "GameObject=" << resourceName << "," << get_global_rid() << ",\n";
            return true;
        }
    }
    return false;
}

//This function is also shared by scene editor to make sure code is uniform and reduce potential mismatchings
void setup_object_components( GPE_DropDown_Menu * componentMenu)
{
    if( componentMenu == NULL)
    {
        return;
    }
    componentMenu->add_menu_option("Check-Box","checkbox",1,false);
    componentMenu->add_menu_option("Color-Picker","colorpicker",2,false);
    componentMenu->add_menu_option("Drop-Down","dropdown",3,false);
    componentMenu->add_menu_option("Input-Text","inputtext",4,false);
    componentMenu->add_menu_option("Input-Number","inputnumber",5,false);
    componentMenu->add_menu_option("Radio Button","radio",6,false);
    componentMenu->add_menu_option("Text Label","labeltext",7,false);
    componentMenu->add_menu_option("Text URL","texturl",8,false);

    GPE_KeyPair *  contentSelectorKey = componentMenu->add_menu_option("Content Selector","contentselector",9,false);
    contentSelectorKey->add_keypair("Content Selector-Id","contentselector-int",10);
    contentSelectorKey->add_keypair("Content Selector-float","contentselector-float",11);
    contentSelectorKey->add_keypair("Content Selector-String","contentselector-string",12);
    contentSelectorKey->add_keypair("Content Selector-Color","contentselector-color",13);

    componentMenu->add_menu_option("Texture","Textures-resourcedropdown",50,false);
    componentMenu->add_menu_option("Tilesheet","Tilesheets-resourcedropdown",51,false);
    componentMenu->add_menu_option("Animation","Animations-resourcedropdown",52,false);
    componentMenu->add_menu_option("Audio","Audio-resourcedropdown",53,false);
    componentMenu->add_menu_option("Video","Videos-resourcedropdown",54,false);
    componentMenu->add_menu_option("Function","Functions-resourcedropdown",55,false);
    componentMenu->add_menu_option("Object","Objects-resourcedropdown",56,false);
    componentMenu->add_menu_option("Scene","Scenes-resourcedropdown",57,false);
    componentMenu->add_menu_option("Font","Fonts-resourcedropdown",58,false);
}
