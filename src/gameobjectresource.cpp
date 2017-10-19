/*
gameobjectresource.cpp
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

#include "gameobjectresource.h"

collisionContainer::collisionContainer()
{
    textEditor = new GPE_TextAreaInputBasic(true);
    otherObjectType = -1;
}

collisionContainer::~collisionContainer()
{
    if(textEditor!=NULL)
    {
        delete textEditor;
        textEditor= NULL;
    }
}

customFunctionContainer::customFunctionContainer()
{
    textEditor = new GPE_TextAreaInputBasic(true);
    parametersField = new GPE_TextInputBasic("","parameter1, parameter2, parameter3");
    returnTypeField = new GPE_TextInputBasic("Return Type","void");
}

customFunctionContainer::~customFunctionContainer()
{
    if(textEditor!=NULL)
    {
        delete textEditor;
        textEditor= NULL;
    }
    if(parametersField!=NULL)
    {
        delete parametersField;
        parametersField= NULL;
    }
    if(returnTypeField!=NULL)
    {
        delete returnTypeField;
        returnTypeField = NULL;
    }
}

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

GPE_ObjectComponent::GPE_ObjectComponent()
{
    component = NULL;
    settingsGear = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/cogs.png","settings" );
}

GPE_ObjectComponent::~GPE_ObjectComponent()
{
    if( component!=NULL)
    {
        delete component;
        component = NULL;
    }

    if( settingsGear!=NULL)
    {
        delete settingsGear;
        settingsGear = NULL;
    }
}

void GPE_ObjectComponent::load_data(std::string dataString)
{
    if( component!=NULL)
    {
        component->load_data(dataString);
    }
}

void GPE_ObjectComponent::remove_data(std::string dataString)
{
    if( component!=NULL)
    {
        component->remove_data(dataString);
    }
}

void GPE_ObjectComponent::rename_component(std::string newName)
{
    if( component!=NULL)
    {
        component->set_name(newName);
    }

    if( settingsGear!=NULL)
    {
        settingsGear->set_name(newName);
    }
}

GPE_GeneralGuiElement * GPE_ObjectComponent::get_component()
{
    return component;
}

GPE_ToolIconButton * GPE_ObjectComponent::get_gear()
{
    return settingsGear;
}

std::string GPE_ObjectComponent::get_data()
{
    if( component!=NULL)
    {
        return component->get_data();
    }
    return "";
}

std::string GPE_ObjectComponent::get_name()
{
    if( component!=NULL)
    {
        return component->get_name();
    }

    if( settingsGear!=NULL)
    {
        return settingsGear->get_name();
    }
    return "";

}

std::string GPE_ObjectComponent::get_plain_string()
{
    if( component!=NULL)
    {
        return component->get_plain_string();
    }
    return "";
}

std::string GPE_ObjectComponent::get_type()
{
    if( component!=NULL)
    {
        return component->get_element_type();
    }

    return "";
}

void GPE_ObjectComponent::set_component(GPE_GeneralGuiElement * newComponent)
{
    if( newComponent!=NULL)
    {
        if( component!=NULL)
        {
            delete component;
        }
        component = newComponent;
        component->hasLineBreak = false;
        if( settingsGear!=NULL)
        {
            settingsGear->set_name( component->get_name() );
        }
    }
}

gameObjectResource::gameObjectResource(GPE_ResourceContainer * pFolder)
{
    resourcePostProcessed = true;
    projectParentFolder = pFolder;
    codeSection = 0;
    imageIndex = 0;
    editorMode = 0;
    int i = 0;
    for(i=0; i < objectFunctionTypes; i++)
    {
        objectFunctionInEdit[i] = 0;
    }
    spriteIndex = -1;
    spriteInEditor = NULL;

    editorButtonBar = new GPE_ToolIconButtonBar(0,0,32, true);
    editorButtonBar->widthAutoResizes = true;
    editorButtonBar->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/tags.png","Object Components",OBJ_EDITOR_MODE_COMPONENTS,false );
    editorButtonBar->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/star.png","Basic Functions",OBJ_EDITOR_MODE_BASIC,false );
    editorButtonBar->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/clock-o.png","Timed Functions",OBJ_EDITOR_MODE_TIMED,false );
    editorButtonBar->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/qq.png","Collision Functions",OBJ_EDITOR_MODE_COLLISIONS,false );
    editorButtonBar->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/code.png","Custom Functions",OBJ_EDITOR_MODE_CUSTOM,false );
    editorButtonBar->adkb_dton(APP_DIRECTORY_NAME+"resources/gfx/buttons/leaf.png","Special Functions",OBJ_EDITOR_MODE_SPECIAL,false );

    if(renameBox!=NULL)
    {
        renameBox->set_coords(-1,GENERAL_GPE_PADDING);
    }
    if(saveResourceButton!=NULL)
    {
        saveResourceButton->disable_self();
    }

    basicFunctionsSelector = new GPE_SelectBoxBasic(0,0,"Object Functions");
    basicFunctionsSelector->set_width(editorButtonBar->get_width() );

    classDeclarationBox = new GPE_TextAreaInputBasic(true);
    for( i = 0; i < DEFAULT_OBJECT_FUNCTIONS_COUNT; i++)
    {
        objectFunctions[i] = new GPE_TextAreaInputBasic(true);

    }
    if( CURRENT_PROJECT->myProjectLanguage == PROJECT_LANGUAGE_CPP)
    {
        basicFunctionsSelector->add_option("Class Variables",256,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/wrench.png"),NULL,0,false,true);
    }
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[0],0,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/wrench.png"),NULL,0,false,true);
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[1],1,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/bomb.png"),NULL,0,false,true);
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[2],2,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/superscript.png"),NULL,0,false,true);
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[3],3,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/superscript.png"),NULL,0,false,true);
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[4],4,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/superscript.png"),NULL,0,false,true);
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[5],5,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/superscript.png"),NULL,0,false,true);
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[6],6,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/paint-brush.png"),NULL,0,false,true);
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[7],7,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/paint-brush.png"),NULL,0,false,true);
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[8],8,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/paint-brush.png"),NULL,0,false,true);
    basicFunctionsSelector->add_option(DEFAULT_OBJECT_FUNCTIONS_NAMES[9],9,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx/buttons/paint-brush.png"),NULL,0,false,true);

    timedFunctionsSelector = new GPE_SelectBoxBasic(GENERAL_GPE_PADDING,editorButtonBar->get_width(),"Timed Functions");
    timedFunctionsSelector->set_width(editorButtonBar->get_width() );
    for( i= 0 ; i < DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT; i++)
    {
        timedFunctionsSelector->add_option("Timed Function ["+int_to_string(i)+"]",i,rsm->texture_add(APP_DIRECTORY_NAME+"resources/gfx//buttons/clock-o.png"),NULL,0,false,true);
        timedObjectFunctions[i] = new GPE_TextAreaInputBasic(true);
    }

    specialFunctionsSelector = new GPE_SelectBoxBasic(0,0,"Render Functions");
    specialFunctionsSelector->set_width(editorButtonBar->get_width() );
    for( i= 0 ; i < DEFAULT_SPECIAL_FUNCTIONS_COUNT; i++)
    {
        specialFunctionsSelector->add_option(DEFAULT_OBJECT_SPECIALS_NAMES[i] );
        specialObjectFunctions[i] = new GPE_TextAreaInputBasic(true);
    }

    colliderFunctionsSelector = new GPE_SelectBoxBasic(0,0,"Collision Functions");
    colliderFunctionsSelector->set_width(editorButtonBar->get_width() );

    customFunctionsSelector = new GPE_SelectBoxBasic(0,0,"Custom Functions");
    customFunctionsSelector->set_width(editorButtonBar->get_width() );

    confirmResourceButton->enable_self();
    cancelResourceButton->disable_self();

    if( projectParentFolder!=NULL)
    {
        spriteField = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING,48,"Object Sprite",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_SPRITE]+"s") );
        loadResourceButton->set_width(spriteField->get_width() );
        confirmResourceButton->set_width(spriteField->get_width() );
        parentObjectField = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING,96,"Parent Object",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s" ) );
        newObjectToCollideDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING,96,"Add Collision",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s" ) );
        changeObjectToCollideDropDown = new GPE_DropDown_Resouce_Menu(GENERAL_GPE_PADDING,96,"Change Collision Object",projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s" ) );
        removeCollideFunctionButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/minus.png","Remove Collision","Removes Selected Collision function");

        //Custom Fucntions Buttons
        newCustomObjectFunctionSettingsButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/cogs.png","Function Settings");
        newCustomObjectFunctionButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/plus.png","Add Custom Function");
        removeCustomObjectFunctionButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/minus.png", "Remove Custom Function" );
    }
    else
    {
        spriteField = NULL;
        parentObjectField = NULL;
        newObjectToCollideDropDown = NULL;
        changeObjectToCollideDropDown = NULL;
        removeCollideFunctionButton= NULL;

        newCustomObjectFunctionSettingsButton = NULL;
        newCustomObjectFunctionButton = NULL;
        removeCustomObjectFunctionButton = NULL;
    }

    customComponentsGuiList = new GPE_GuiElementList();
    componentsMainGuiLabel = new GPE_Label_Text("Custom Components","Custom components, useful for scene editor");
    componentsGuiLabel = new GPE_Label_Text("Custom Components","Custom components, useful for scene editor");
    inheritParentComponentButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/users.png","Inherit Components","Inherit components from parent object class");
    resetComponentsButton = new GPE_ToolPushButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/trash.png","Reset Components","Clears all components form this object class");
    addNewComponentDropDown = new GPE_DropDown_Menu(0,0,"Add Component",false);
    addNewComponentDropDown->add_menu_option("Check-Box","checkbox",1,false);
    addNewComponentDropDown->add_menu_option("Color-Picker","colorpicker",2,false);
    addNewComponentDropDown->add_menu_option("Drop-Down","dropdown",3,false);
    addNewComponentDropDown->add_menu_option("Input-Text","inputtext",4,false);
    addNewComponentDropDown->add_menu_option("Input-Number","inputnumber",5,false);
    addNewComponentDropDown->add_menu_option("Radio Button","radio",6,false);
    addNewComponentDropDown->add_menu_option("Text Label","labeltext",7,false);
    addNewComponentDropDown->add_menu_option("Text URL","texturl",8,false);
    addNewComponentDropDown->add_menu_option("Texture","Textures-resourcedropdown",50,false);
    addNewComponentDropDown->add_menu_option("Tilesheet","Tilesheets-resourcedropdown",51,false);
    addNewComponentDropDown->add_menu_option("Sprite","Sprites-resourcedropdown",52,false);
    addNewComponentDropDown->add_menu_option("Audio","Audio-resourcedropdown",53,false);
    addNewComponentDropDown->add_menu_option("Video","Videos-resourcedropdown",54,false);
    addNewComponentDropDown->add_menu_option("Function","Functions-resourcedropdown",55,false);
    addNewComponentDropDown->add_menu_option("Object","Objects-resourcedropdown",56,false);
    addNewComponentDropDown->add_menu_option("Scene","Scenes-resourcedropdown",57,false);
    addNewComponentDropDown->add_menu_option("Font","Fonts-resourcedropdown",58,false);
    addNewComponentDropDown->set_width(224);

    removeComponentButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/minus.png","Remove Component");
    editCompnentButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/edit.png","Edit Component");
    componentSettingsButton = new GPE_ToolIconButton(0,0,APP_DIRECTORY_NAME+"resources/gfx/buttons/trash.png","Component Settings");

    checkBoxIsVisible = new GPE_CheckBoxBasic("Visible","The actor is rendered on screen",GENERAL_GPE_PADDING,128+GENERAL_GPE_PADDING,true);
    checkBoxNeedsCamera = new GPE_CheckBoxBasic("Needs Camera","Object will only perform logic if in view of any camera [Highly recommended for most objects]",GENERAL_GPE_PADDING+96,128+GENERAL_GPE_PADDING,true);
    checkBoxIsMoveable = new GPE_CheckBoxBasic("Mobile","Uses engine default physics",GENERAL_GPE_PADDING,160,true);
    checkBoxIsContinuous = new GPE_CheckBoxBasic("Continuous","Object continues throughout game scenes[Keep unchecked for most objects]",GENERAL_GPE_PADDING+96,160,false);
    renameBox->set_coords(GENERAL_GPE_PADDING+32,-1);
    resourceNameLabel = new GPE_Label_Title("Object","");
}

gameObjectResource::~gameObjectResource()
{
    if( spriteField!=NULL)
    {
        delete spriteField;
        spriteField = NULL;
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
    if( newObjectToCollideDropDown!=NULL)
    {
        delete newObjectToCollideDropDown;
        newObjectToCollideDropDown= NULL;
    }
    if( editorButtonBar!=NULL)
    {
        delete editorButtonBar;
        editorButtonBar = NULL;
    }
    if( basicFunctionsSelector!=NULL)
    {
        delete basicFunctionsSelector;
        basicFunctionsSelector = NULL;
    }
    if( timedFunctionsSelector!=NULL)
    {
        delete timedFunctionsSelector;
        timedFunctionsSelector = NULL;
    }
    if( colliderFunctionsSelector!=NULL)
    {
        delete colliderFunctionsSelector;
        colliderFunctionsSelector = NULL;
    }
    if( specialFunctionsSelector!=NULL)
    {
        delete specialFunctionsSelector;
        specialFunctionsSelector = NULL;
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

    if( classDeclarationBox!=NULL)
    {
        delete classDeclarationBox;
        classDeclarationBox = NULL;
    }

    for(  i =DEFAULT_OBJECT_FUNCTIONS_COUNT-1; i >=0; i--)
    {
        if( objectFunctions[i]!=NULL)
        {
            delete objectFunctions[i];
            objectFunctions[i] = NULL;
        }
    }
    for(  i =DEFAULT_SPECIAL_FUNCTIONS_COUNT-1; i >=0; i--)
    {
        if( specialObjectFunctions[i]!=NULL)
        {
            delete specialObjectFunctions[i];
            specialObjectFunctions[i] = NULL;
        }
    }
    for(  i =DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT-1; i >=0; i--)
    {
        if( timedObjectFunctions[i]!=NULL)
        {
            delete timedObjectFunctions[i];
            timedObjectFunctions[i] = NULL;
        }
    }

    collisionContainer * tColContainer = NULL;
    for( i = (int)collisionFunctions.size()-1; i>=0; i--)
    {
        tColContainer = collisionFunctions[i];
        if( tColContainer!=NULL )
        {
            delete tColContainer;
            tColContainer = NULL;
            collisionFunctions.erase(collisionFunctions.begin()+i);
        }
    }
    collisionFunctions.clear();

    customFunctionContainer * tCustomContainer = NULL;
    for( i = (int)customFunctions.size()-1; i>=0; i--)
    {
        tCustomContainer = customFunctions[i];
        if( tCustomContainer!=NULL )
        {
            delete tCustomContainer;
            tCustomContainer = NULL;
            customFunctions.erase(customFunctions.begin()+i);
        }
    }
    customFunctions.clear();

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

    if( removeCollideFunctionButton!=NULL)
    {
        delete removeCollideFunctionButton;
        removeCollideFunctionButton = NULL;
    }
    if( removeCustomObjectFunctionButton!=NULL)
    {
        delete removeCustomObjectFunctionButton;
        removeCustomObjectFunctionButton = NULL;
    }
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

        //*fileTarget << nestedTabsStr << "var " << get_name() << " = " << html5BuildGlobalId << ";\n";
        *fileTarget << nestedTabsStr << "GPE._obj_" << get_name() << " =  (function (xPosIn, yPosIn,objectLayerId) \n";
        *fileTarget << nestedTabsStr << "{ \n";
        *fileTarget << nestedTabsStrObjFunc << "function _obj_" << get_name() <<" (xPosIn, yPosIn,objectLayerId) \n";
        *fileTarget << nestedTabsStrObjFunc << "{ \n";
        GPE_ResourceContainer * parentRes = parentObjectField->get_selected_container();
        if( parentRes!=NULL)
        {
            *fileTarget << nestedTabsStrObjFunc << "    GPE._obj_" << parentRes->get_name() <<".call(this,xPosIn, yPosIn,objectLayerId); \n";
        }
        else
        {
            *fileTarget << nestedTabsStrObjFunc << "    GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); \n";
        }
        *fileTarget << nestedTabsStrObjFunc << "    this.gpeObjectType ="<< html5BuildGlobalId << "; \n";
        GPE_ResourceContainer * spriteRes = spriteField->get_selected_container();
        if( spriteRes!=NULL)
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.init_sprite(" << spriteRes->html5BuildGlobalId << ");\n";
        }
        else
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.init_sprite(-1);\n";
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

        if( !objectFunctions[0]->compile_into_code( fileTarget, leftTabAmount+2,true,true) )
        {
            for( iErrorLine=0; iErrorLine < (int)objectFunctions[0]->foundSyntaxErrors.size(); iErrorLine++)
            {
                GPE_Main_Logs->log_build_error(get_name()+"'s Constructor: "+objectFunctions[0]->foundSyntaxErrors[0] );
                buildSuccessful = false;
            }
        }
        for( iFunc = 1; iFunc < DEFAULT_OBJECT_FUNCTIONS_COUNT; iFunc++)
        {
            if( objectFunctions[iFunc]->has_content() )
            {
                *fileTarget << nestedTabsStrObjFunc << "    this." << DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[iFunc]<< " = function()\n";
                *fileTarget << nestedTabsStrObjFunc << "    {\n";
                if( !objectFunctions[iFunc]->compile_into_code( fileTarget, leftTabAmount+3,true,true) )
                {
                    for( iErrorLine=0; iErrorLine < (int)objectFunctions[iFunc]->foundSyntaxErrors.size(); iErrorLine++)
                    {
                        GPE_Main_Logs->log_build_error(get_name()+"'s "+DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[iFunc]+" function: "+objectFunctions[iFunc]->foundSyntaxErrors[iErrorLine] );
                    }
                    buildSuccessful = false;
                    if( MAIN_EDITOR_SETTINGS->stopCompileOnError->is_clicked() )
                    {
                        break;
                    }
                }
                *fileTarget << nestedTabsStrObjFunc << "    }\n";
            }
        }
        if(objectFunctions[FUNC_ID_PRERENDER]->has_content() )
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.hasPreRenderFunction = true;\n";
        }
        if(objectFunctions[FUNC_ID_RENDER]->has_content() )
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.hasRenderFunction = true;\n";
        }
        if(objectFunctions[FUNC_ID_POSTRENDER]->has_content() )
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.hasPostRenderFunction= true;\n";
        }
        if(objectFunctions[FUNC_ID_HUD_RENDER]->has_content() )
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.hasHudRenderFunction = true;\n";
        }

        bool objectHasTimedFunction = false;
        for( iFunc = 0; iFunc < DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT; iFunc++)
        {
            if( timedObjectFunctions[iFunc]->has_content() )
            {
                *fileTarget << nestedTabsStrObjFunc << "    this.timedFunction" << iFunc << " = function()\n";
                *fileTarget << nestedTabsStrObjFunc << "    {\n";
                if( !timedObjectFunctions[iFunc]->compile_into_code( fileTarget, leftTabAmount+3,true,true) )
                {
                    for( iErrorLine=0; iErrorLine < (int)timedObjectFunctions[iFunc]->foundSyntaxErrors.size(); iErrorLine++)
                    {
                        GPE_Main_Logs->log_build_error(get_name()+"'s timedFunction["+int_to_string(iFunc)+"]: "+timedObjectFunctions[iFunc]->foundSyntaxErrors[iErrorLine] );
                    }
                    buildSuccessful = false;
                    if( MAIN_EDITOR_SETTINGS->stopCompileOnError->is_clicked() )
                    {
                        break;
                    }
                }
                *fileTarget << nestedTabsStrObjFunc << "    }\n";
                objectHasTimedFunction = true;
            }
        }
        if( objectHasTimedFunction)
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.gpeHasTimedFunctions = true;\n";
        }
        if( objectFunctions[FUNC_ID_PRELOGIC]->has_content() )
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.gpeHasStartLogicFunction = true;\n";
        }

        if( objectFunctions[FUNC_ID_LOGIC]->has_content() )
        {
            *fileTarget << nestedTabsStrObjFunc << "    this.gpeHasLogicFunction = true;\n";
        }


        for( iFunc = 0; iFunc < DEFAULT_SPECIAL_FUNCTIONS_COUNT; iFunc++)
        {
            if( specialObjectFunctions[iFunc]->has_content() )
            {
                *fileTarget << nestedTabsStrObjFunc << "    this." << DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[iFunc]<< " = function()\n";
                *fileTarget << nestedTabsStrObjFunc << "    {\n";
                if( !specialObjectFunctions[iFunc]->compile_into_code( fileTarget, leftTabAmount+3,true,true) )
                {
                    for( iErrorLine=0; iErrorLine < (int)timedObjectFunctions[iFunc]->foundSyntaxErrors.size(); iErrorLine++)
                    {
                        GPE_Main_Logs->log_build_error(get_name()+"'s "+DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[iFunc]+" function: "+timedObjectFunctions[iFunc]->foundSyntaxErrors[iErrorLine] );
                    }
                    buildSuccessful = false;
                    if( MAIN_EDITOR_SETTINGS->stopCompileOnError->is_clicked() )
                    {
                        break;
                    }
                }
                *fileTarget << nestedTabsStrObjFunc << "    }\n";
            }
        }

        GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
        GPE_ResourceContainer * otherObjResContainer = NULL;
        int objsFoundToCollide = 0;
        if( allObjsFolder!=NULL)
        {
            GPE_TextAreaInputBasic * tempColFuction = new GPE_TextAreaInputBasic();
            GPE_TextAreaInputBasic * tempColList = new GPE_TextAreaInputBasic();
            tempColFuction->add_line("switch(otherObj.gpeObjectType)");
            tempColFuction->add_line("{");

            //tempColList->add_line("this.OBJECT_COLLISION_LIST = [];");
            //add in collisions here
            collisionContainer *  cColContainer = NULL;
            for( iFunc = 0; iFunc < (int)collisionFunctions.size(); iFunc++)
            {
                cColContainer = collisionFunctions[iFunc];
                if( cColContainer!=NULL)
                {
                    otherObjResContainer = allObjsFolder->find_resource_from_id(cColContainer->otherObjectType);
                    if( otherObjResContainer!=NULL)
                    {
                        *fileTarget << nestedTabsStrObjFunc << "    this.collision_with_" << otherObjResContainer->get_name() << " = function(other)\n";
                        *fileTarget << nestedTabsStrObjFunc << "    {\n";
                        if( !cColContainer->textEditor->compile_into_code( fileTarget, leftTabAmount+3,true,true) )
                        {
                            for( iErrorLine=0; iErrorLine < (int)cColContainer->textEditor->foundSyntaxErrors.size(); iErrorLine++)
                            {
                                GPE_Main_Logs->log_build_error( get_name()+"'s Collision Function["+otherObjResContainer->get_name()+"]: "+cColContainer->textEditor->foundSyntaxErrors[iErrorLine] );
                            }
                            buildSuccessful = false;
                            if( MAIN_EDITOR_SETTINGS->stopCompileOnError->is_clicked() )
                            {
                                break;
                            }
                        }
                        *fileTarget << nestedTabsStrObjFunc << "    }\n";
                        objsFoundToCollide++;
                        tempColList->add_line("this.OBJECT_COLLISION_LIST.push("+ int_to_string( otherObjResContainer->html5BuildGlobalId )+");");
                        tempColFuction->add_line("    case "+ int_to_string( otherObjResContainer->html5BuildGlobalId )+":");
                        tempColFuction->add_line("        this.collision_with_" + otherObjResContainer->get_name()+"(otherObj);" );
                        tempColFuction->add_line("    break;");
                    }
                }
            }

            if( tempColFuction->has_content() &&  objsFoundToCollide > 0)
            {
                tempColFuction->add_line("    default:");
                tempColFuction->add_line("    break;");
                tempColFuction->add_line("}");

                *fileTarget << nestedTabsStrObjFunc << "    this.collide_with = function(otherObj)\n";
                *fileTarget << nestedTabsStrObjFunc << "    {\n";
                tempColFuction->write_short_data_into_file( fileTarget, leftTabAmount+3,true,true);
                *fileTarget << nestedTabsStrObjFunc << "    };\n\n";
            }

            delete tempColFuction;
            tempColFuction = NULL;

            tempColList->write_data_into_file( fileTarget, leftTabAmount+3,true,true);
            delete tempColList;
            tempColList = NULL;

            customFunctionContainer *  cFuncContainer = NULL;
            for( iFunc = 0; iFunc < (int)customFunctions.size(); iFunc++)
            {
                cFuncContainer = customFunctions[iFunc];
                if( cFuncContainer!=NULL)
                {
                    if( cFuncContainer->textEditor!=NULL && cFuncContainer->textEditor->has_content() && cFuncContainer->parametersField!=NULL )
                    {
                        if( GPE_MAIN_HIGHLIGHTER->process_parameters_string( cFuncContainer->parametersField->get_string() ) )
                        {
                            *fileTarget << nestedTabsStrObjFunc << "    this." << cFuncContainer->functionName << " = function("<< GPE_MAIN_HIGHLIGHTER->newParametersString << ")\n";
                            *fileTarget << nestedTabsStrObjFunc << "    {\n";
                        }
                        else
                        {
                            *fileTarget << nestedTabsStrObjFunc << "    this." << cFuncContainer->functionName << " = function( )\n";
                            *fileTarget << nestedTabsStrObjFunc << "    {\n";

                            for( iErrorLine = 0; iErrorLine < (int)GPE_MAIN_HIGHLIGHTER->functionParameterErrors.size(); iErrorLine++)
                            {
                                GPE_Main_Logs->log_build_error("[Build Error] [ "+resourceName+"] object custom ["+cFuncContainer->functionName+"] function parameters: "+GPE_MAIN_HIGHLIGHTER->functionParameterErrors[iErrorLine] );
                            }
                            buildSuccessful = false;
                            if( MAIN_EDITOR_SETTINGS->stopCompileOnError->is_clicked() )
                            {
                                break;
                            }
                        }
                        for( paramLine = 0; paramLine < (int)GPE_MAIN_HIGHLIGHTER->defaultParameterLines.size(); paramLine++)
                        {
                            *fileTarget << "     "+GPE_MAIN_HIGHLIGHTER->defaultParameterLines[paramLine] << "\n";
                        }
                        if( !cFuncContainer->textEditor->compile_into_code( fileTarget, leftTabAmount+3,true,true) )
                        {
                            for( iErrorLine=0; iErrorLine < (int)cFuncContainer->textEditor->foundSyntaxErrors.size(); iErrorLine++)
                            {
                                GPE_Main_Logs->log_build_error( get_name()+"'s Custom Function["+cFuncContainer->functionName+"]: "+cFuncContainer->textEditor->foundSyntaxErrors[iErrorLine] );
                            }
                            buildSuccessful = false;
                            if( MAIN_EDITOR_SETTINGS->stopCompileOnError->is_clicked() )
                            {
                                break;
                            }
                        }
                        *fileTarget << nestedTabsStrObjFunc << "    };\n\n";
                    }
                }
            }

            *fileTarget << nestedTabsStrObjFunc << "    this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);\n";
            *fileTarget << nestedTabsStrObjFunc << "} \n\n";
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

collisionContainer * gameObjectResource::find_collision_event( int otherObjId)
{
    collisionContainer * cContainer = NULL;
    collisionContainer * rContainer = NULL;
    if( otherObjId>0 && (int)collisionFunctions.size()>0)
    {
        for( int i = 0 ;i < (int)collisionFunctions.size(); i++ )
        {
            cContainer = collisionFunctions[i];
            if( cContainer!=NULL)
            {
                if( cContainer->otherObjectType==otherObjId)
                {
                    rContainer = cContainer;
                    break;
                }
            }
        }
    }
    return rContainer;
}

customFunctionContainer * gameObjectResource::find_custom_function( std::string functionNameIn,bool createNewOne )
{
    customFunctionContainer * tempCFuncContainer = NULL;

    for(int funcI = 0; funcI < (int)customFunctions.size(); funcI++)
    {
        tempCFuncContainer = customFunctions[funcI];
        if( tempCFuncContainer!=NULL && tempCFuncContainer->textEditor!=NULL)
        {
            if( tempCFuncContainer->functionName==functionNameIn)
            {
                return tempCFuncContainer;
            }
        }
    }
    //if nothing has been returned yet, we make a new custom function
    if( createNewOne)
    {
        tempCFuncContainer = new customFunctionContainer();
        tempCFuncContainer->functionName = functionNameIn;
        customFunctionsSelector->add_option(functionNameIn,(int)customFunctions.size()+1,NULL,NULL,-1,true);
        customFunctions.push_back(tempCFuncContainer);
        return tempCFuncContainer;
    }
    return NULL;
}

GPE_ResourceContainer * gameObjectResource::get_parent_resource()
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

GPE_TextAreaInputBasic * gameObjectResource::find_function_textarea(std::string functionNameIn,int functionTypeIn )
{
    if( functionTypeIn >= 0 && functionTypeIn <= 5 )
    {
        int funcI = 0;
        if( functionTypeIn==0)
        {
            for (funcI = 0; funcI < DEFAULT_OBJECT_FUNCTIONS_COUNT; funcI++)
            {
                if(DEFAULT_OBJECT_FUNCTIONS_NAMES[funcI]==functionNameIn)
                {
                    return objectFunctions[funcI];
                }
            }
        }
        else if(functionTypeIn==1)
        {
            for (funcI = 0; funcI < DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT; funcI++)
            {
                if("Timer"+int_to_string(funcI)==functionNameIn)
                {
                    return timedObjectFunctions[funcI];
                }
            }
        }
        else if( functionTypeIn==2)
        {
            //Query by Id
            collisionContainer * tempColContainer = NULL;
            GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
            GPE_ResourceContainer * otherObjResContainer = NULL;
            if( allObjsFolder!=NULL)
            {
                for(funcI = 0; funcI < (int)collisionFunctions.size(); funcI++)
                {
                    tempColContainer = collisionFunctions[funcI];
                    if( tempColContainer!=NULL && tempColContainer->textEditor!=NULL)
                    {
                        otherObjResContainer = allObjsFolder->find_resource_from_id(tempColContainer->otherObjectType);
                        if( otherObjResContainer!=NULL && int_to_string(otherObjResContainer->get_global_id() )==functionNameIn)
                        {
                            return tempColContainer->textEditor;
                        }
                    }
                }
                //if nothing has been returned yet, we make a new collision event
                int functionIntId = string_to_int( functionNameIn,-1);
                otherObjResContainer = allObjsFolder->find_resource_from_id(functionIntId);
                if(otherObjResContainer!=NULL )
                {
                    tempColContainer = new collisionContainer();
                    tempColContainer->otherObjectType = string_to_int( functionNameIn,-1);
                    colliderFunctionsSelector->add_option(otherObjResContainer->get_name() );
                    collisionFunctions.push_back(tempColContainer);
                    return tempColContainer->textEditor;
                }
                else
                {
                    //if nothing has been returned yet, we make a new collision event with no object to be reassigned later.
                    int functionIntId = string_to_int( functionNameIn,-1);
                    tempColContainer = new collisionContainer();
                    tempColContainer->otherObjectType = string_to_int( functionNameIn,-1);
                    colliderFunctionsSelector->add_option("Unknown ObjectId ["+int_to_string( tempColContainer->otherObjectType)+"]" );
                    collisionFunctions.push_back(tempColContainer);
                    return tempColContainer->textEditor;
                }
            }
        }
        else if( functionTypeIn==3)
        {
            //Query by name
            collisionContainer * tempColContainer = NULL;
            GPE_ResourceContainer * allObjsFolder = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s");
            GPE_ResourceContainer * otherObjResContainer = NULL;
            if( allObjsFolder!=NULL)
            {
                for(funcI = 0; funcI < (int)collisionFunctions.size(); funcI++)
                {
                    tempColContainer = collisionFunctions[funcI];
                    if( tempColContainer!=NULL && tempColContainer->textEditor!=NULL)
                    {
                        otherObjResContainer = allObjsFolder->find_resource_from_name(tempColContainer->otherObjectName );
                        if( otherObjResContainer!=NULL && otherObjResContainer->get_name() ==functionNameIn )
                        {
                            return tempColContainer->textEditor;
                        }
                    }
                }
                //if nothing has been returned yet, we make a new collision event
                int functionIntId = string_to_int( functionNameIn,-1);
                otherObjResContainer = allObjsFolder->find_resource_from_name( functionNameIn );
                if(otherObjResContainer!=NULL )
                {
                    tempColContainer = new collisionContainer();
                    tempColContainer->otherObjectType = otherObjResContainer->get_global_id();
                    tempColContainer->otherObjectName =  functionNameIn;
                    colliderFunctionsSelector->add_option( functionNameIn );
                    collisionFunctions.push_back(tempColContainer);
                    return tempColContainer->textEditor;
                }
                else
                {
                    //if nothing has been returned yet, we make a new collision event with no object to be reassigned later.
                    tempColContainer = new collisionContainer();
                    tempColContainer->otherObjectType = string_to_int( functionNameIn,-1);
                    tempColContainer->otherObjectName =  functionNameIn;
                    colliderFunctionsSelector->add_option("Unknown Object ["+functionNameIn+"]" );
                    collisionFunctions.push_back(tempColContainer);
                    return tempColContainer->textEditor;
                }
            }
        }
        else if( functionTypeIn==4)
        {
            for(funcI = 0; funcI < DEFAULT_SPECIAL_FUNCTIONS_COUNT; funcI++)
            {
                if(DEFAULT_OBJECT_SPECIALS_NAMES[funcI]==functionNameIn)
                {
                    return specialObjectFunctions[funcI];
                }
            }
        }
        else if( functionTypeIn==5)
        {
            customFunctionContainer * tempCFuncContainer = NULL;

            for(funcI = 0; funcI < (int)customFunctions.size(); funcI++)
            {
                tempCFuncContainer = customFunctions[funcI];
                if( tempCFuncContainer!=NULL && tempCFuncContainer->textEditor!=NULL)
                {
                    if( tempCFuncContainer->functionName==functionNameIn)
                    {
                        return tempCFuncContainer->textEditor;
                    }
                }
            }
            //if nothing has been returned yet, we make a new custom function
            tempCFuncContainer = new customFunctionContainer();
            tempCFuncContainer->functionName = functionNameIn;
            customFunctionsSelector->add_option(functionNameIn );
            customFunctions.push_back(tempCFuncContainer);
            return tempCFuncContainer->textEditor;
        }
     }
    return NULL;
}

void gameObjectResource::integrate_into_syntax()
{
    if( CURRENT_PROJECT!=NULL)
    {
        CURRENT_PROJECT->add_project_keyword(resourceName,"Project "+RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+" Resource",-1,"");

        if( (int)customFunctions.size() > 0)
        {
            std::string functionName = "";
            std::string fReturnType = "unknown_void";
            std::string fParameters = "";
            std::string functionDescription = "";
            std::string firstLineOfCode = "";
            customFunctionContainer * tCustomFunction = NULL;
            for( int i = 0; i  < (int)customFunctions.size(); i++ )
            {
                tCustomFunction = customFunctions[i];
                if( tCustomFunction!=NULL)
                {
                    functionName = tCustomFunction->functionName;
                    fReturnType = "void";
                    functionDescription = "User defined function...";
                    firstLineOfCode = "";
                    if( tCustomFunction->textEditor!=NULL)
                    {
                        firstLineOfCode = tCustomFunction->textEditor->get_line_string(0);
                        if( (int)firstLineOfCode.size() > 2)
                        {
                            if( firstLineOfCode[0]=='/' && (firstLineOfCode.at(1)=='/' || firstLineOfCode.at(1)=='*' ) )
                            {
                                functionDescription = get_substring(firstLineOfCode,2);
                            }
                        }
                    }
                    if( tCustomFunction->returnTypeField!=NULL)
                    {
                        fReturnType = tCustomFunction->returnTypeField->get_string();
                    }

                    if( tCustomFunction->parametersField!=NULL)
                    {
                        fParameters = tCustomFunction->parametersField->get_string();
                    }
                    CURRENT_PROJECT->add_project_function(functionName,functionDescription,fParameters,fReturnType,resourceName+" Local Function");
                }
            }
        }
    }
}

void gameObjectResource::manage_components(GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL && renameBox!=NULL)
    {
        customComponentsGuiList->set_coords(objEventManagebarBox.x,objEventManagebarBox.y);
        customComponentsGuiList->set_width(viewedSpace->w-objEventManagebarBox.x);
        customComponentsGuiList->set_height(viewedSpace->h- objEventManagebarBox.y );
        customComponentsGuiList->barXPadding = GENERAL_GPE_PADDING;
        customComponentsGuiList->barXMargin = 0;

        customComponentsGuiList->clear_list();
        //Start of New Code
        customComponentsGuiList->add_gui_element(renameBox,true);
        customComponentsGuiList->add_gui_element(spriteField,true);
        customComponentsGuiList->add_gui_element(parentObjectField,true);
        //customComponentsGuiList->add_gui_element(loadResourceButton,true);
        customComponentsGuiList->add_gui_element(checkBoxNeedsCamera,false);
        customComponentsGuiList->add_gui_element(checkBoxIsMoveable,true);
        customComponentsGuiList->add_gui_element(checkBoxIsVisible,false);
        customComponentsGuiList->add_gui_element(checkBoxIsContinuous,true);
        //End of New Code

        customComponentsGuiList->add_gui_element(componentsGuiLabel,true);
        GPE_ObjectComponent * tempCustomComponent = NULL;
        int iRealCompo = 0;
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
    }
}

void gameObjectResource::open_code(int lineNumb, int colNumb, std::string codeTitle)
{
    if( (int)codeTitle.size() > 0 )
    {
        int i = 0;
        collisionContainer * tempCollisionContainer = NULL;
        customFunctionContainer * tempCustomFunction = NULL;
        GPE_TextAreaInputBasic * foundTextArea = NULL;

        for( i = 0; i < DEFAULT_OBJECT_FUNCTIONS_COUNT && foundTextArea==NULL ; i++)
        {
            if( codeTitle == DEFAULT_OBJECT_FUNCTIONS_NAMES[i] )
            {
                foundTextArea = objectFunctions[i];
                editorButtonBar->set_selection(OBJ_EDITOR_MODE_BASIC);
                basicFunctionsSelector->set_selection(i);

            }
        }

        for( i = 0; i < DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT && foundTextArea==NULL ; i++)
        {
            if( codeTitle == "Timed Function("+int_to_string(i)+")" )
            {
                foundTextArea = timedObjectFunctions[i];
                editorButtonBar->set_selection(OBJ_EDITOR_MODE_TIMED);
                timedFunctionsSelector->set_selection(i);
            }
        }

        for( i = 0; i < DEFAULT_SPECIAL_FUNCTIONS_COUNT && foundTextArea==NULL ; i++)
        {
            if( codeTitle == DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[i] )
            {
                foundTextArea = specialObjectFunctions[i];
                editorButtonBar->set_selection(OBJ_EDITOR_MODE_SPECIAL);
                specialFunctionsSelector->set_selection(i);
            }
        }

        for( i = (int)collisionFunctions.size()-1; i >=0 && foundTextArea==NULL ; i--)
        {
            tempCollisionContainer = collisionFunctions[i];
            if( tempCollisionContainer!=NULL && tempCollisionContainer->otherObjectName==codeTitle)
            {
                foundTextArea = tempCollisionContainer->textEditor;
                editorButtonBar->set_selection(OBJ_EDITOR_MODE_COLLISIONS);
                colliderFunctionsSelector->set_selection(i);
            }
        }

        for( i = (int)customFunctions.size()-1; i >=0 && foundTextArea==NULL ; i--)
        {
            tempCustomFunction = customFunctions[i];
            if( tempCustomFunction!=NULL && tempCustomFunction->functionName==codeTitle )
            {
                foundTextArea = tempCustomFunction->textEditor;
                editorButtonBar->set_selection(OBJ_EDITOR_MODE_CUSTOM);
                customFunctionsSelector->set_selection(i);
            }
        }

        if( foundTextArea!=NULL && foundTextArea->has_content() )
        {
            foundTextArea->scroll_to_pos(lineNumb, colNumb);
        }
    }
}

void gameObjectResource::prerender_self(GPE_Renderer * cRender )
{
	standardEditableGameResource::prerender_self( cRender);
	if( removeCollideFunctionButton!=NULL)
    {
        removeCollideFunctionButton->prerender_self(cRender);
    }

    if( checkBoxNeedsCamera!=NULL)
    {
        checkBoxNeedsCamera->prerender_self(cRender);
    }
    if( checkBoxIsMoveable!=NULL)
    {
        checkBoxIsMoveable->prerender_self(cRender);
    }
    if( checkBoxIsVisible!=NULL)
    {
        checkBoxIsVisible->prerender_self(cRender);
    }
    if( checkBoxIsContinuous!=NULL)
    {
        checkBoxIsContinuous->prerender_self(cRender);
    }
}

void gameObjectResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        displayMessageTitle = "Processing GameObject";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();

        std::string otherColContainerName = "";

        std::string newameObjFilename = "";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/objects/";
        if( file_exists(alternatePath) )
        {
            newameObjFilename = alternatePath;
            soughtDir = get_path_from_file(newameObjFilename);
        }
        else
        {
            newameObjFilename = soughtDir + resourceName+".gpf";
        }
        std::ifstream gameObjFileIn( newameObjFilename.c_str() );

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
                std::string cur_layer="";
                std::string data_format="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                float foundFileVersion = 0;
                GPE_TextAreaInputBasic * textAreaToLoad = NULL;
                customFunctionContainer *  newCustomFunction = NULL;
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
                                        foundFileVersion = string_to_double(valString);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion < 2)
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
                                //Seeks for new textArea to edit
                                if( keyString=="[[[Function")
                                {
                                    valString = split_first_string(valString,']');
                                    textAreaToLoad= find_function_textarea(valString,0);
                                    if( textAreaToLoad!=NULL)
                                    {
                                        textAreaToLoad->clear_all_lines();
                                    }
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
                                    textAreaToLoad= find_function_textarea(valString,1);
                                    if( textAreaToLoad!=NULL)
                                    {
                                        textAreaToLoad->clear_all_lines();
                                    }

                                }
                                else if( keyString=="[[[CollisionFunction")
                                {
                                    valString = split_first_string(valString,']');
                                    textAreaToLoad= find_function_textarea(valString,2);
                                    if( textAreaToLoad!=NULL)
                                    {
                                        textAreaToLoad->clear_all_lines();
                                    }
                                }
                                else if( keyString=="[[[CollideFunction")
                                {
                                    valString = split_first_string(valString,']');
                                    textAreaToLoad= find_function_textarea(valString,3);
                                    if( textAreaToLoad!=NULL)
                                    {
                                        textAreaToLoad->clear_all_lines();
                                    }
                                }
                                else if( keyString=="[[[SpecialFunction")
                                {
                                    valString = split_first_string(valString,']');
                                    textAreaToLoad= find_function_textarea(valString,4);
                                    if( textAreaToLoad!=NULL)
                                    {
                                        textAreaToLoad->clear_all_lines();
                                    }
                                }
                                else if( keyString=="[[[CustomFunction")
                                {
                                    //option 5
                                    subValString = split_first_string(valString,',');
                                    newCustomFunction = find_custom_function(subValString);
                                    if( newCustomFunction!=NULL)
                                    {
                                        textAreaToLoad = newCustomFunction->textEditor;
                                        if( textAreaToLoad!=NULL)
                                        {
                                            textAreaToLoad->clear_all_lines();
                                        }
                                        subValString = split_first_string(valString,',');
                                        newCustomFunction->returnTypeField->set_string(subValString);

                                        subValString = split_first_string(valString,']');
                                        newCustomFunction->parametersField->set_string(subValString);

                                    }
                                }
                                else if( textAreaToLoad!=NULL)
                                {
                                    textAreaToLoad->add_line( currLine );
                                }
                                else
                                {
                                    //loads the rest of the file if all text areas are closed at this poin
                                    if( keyString=="ResourceName")
                                    {
                                        renameBox->set_string(valString);
                                    }
                                    else if( keyString=="SpriteId")
                                    {
                                        spriteIndex = -1;
                                        if( spriteField!=NULL)
                                        {
                                            spriteField->set_selected_target(valString);
                                            spriteIndex = spriteField->get_selected_id();
                                        }
                                    }
                                    else if( keyString=="ParentGameObjectId")
                                    {
                                        parentObjectField->set_selected_target( valString );
                                        parentObjectId = parentObjectField->get_selected_id();

                                    }
                                    else if( keyString=="IsContinuous")
                                    {
                                        checkBoxIsContinuous->set_clicked( is_bool(valString) );
                                    }
                                    else if( keyString=="IsMoveable")
                                    {
                                        checkBoxIsMoveable->set_clicked( is_bool(valString) );
                                    }
                                    else if( keyString=="IsVisible")
                                    {
                                        checkBoxIsVisible->set_clicked( is_bool(valString) );
                                    }
                                    else if( keyString=="NeedsCamera")
                                    {
                                        checkBoxNeedsCamera->set_clicked( is_bool(valString) );
                                    }
                                    else if( keyString=="CustomComponent")
                                    {
                                        add_component(valString);
                                    }
                                }
                            }
                            else if(textAreaToLoad!=NULL)
                            {
                                textAreaToLoad->add_line( trim_right_inplace(currLine) );
                            }
                        }
                        else if(textAreaToLoad!=NULL)
                        {
                            textAreaToLoad->add_line( trim_right_inplace(currLine) );
                        }
                    }
                    else
                    {
                        record_error("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }

                textAreaToLoad= NULL;

                int tFuncI = 0;
                for( tFuncI = 0; tFuncI < DEFAULT_OBJECT_FUNCTIONS_COUNT;tFuncI++)
                {
                    textAreaToLoad = objectFunctions[tFuncI];
                    if(textAreaToLoad!=NULL)
                    {
                        textAreaToLoad->init_save_history();
                    }
                }
                for( tFuncI = 0; tFuncI < DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT;tFuncI++)
                {
                    textAreaToLoad = timedObjectFunctions[tFuncI];
                    if(textAreaToLoad!=NULL)
                    {
                        textAreaToLoad->init_save_history();
                    }
                }
                for( tFuncI = 0; tFuncI < DEFAULT_SPECIAL_FUNCTIONS_COUNT;tFuncI++)
                {
                    textAreaToLoad = specialObjectFunctions[tFuncI];
                    if(textAreaToLoad!=NULL)
                    {
                        textAreaToLoad->init_save_history();
                    }
                }
                collisionContainer * tColFunction = NULL;
                for( tFuncI = 0; tFuncI < (int)collisionFunctions.size(); tFuncI++)
                {
                    tColFunction = collisionFunctions.at(tFuncI);
                    if( tColFunction!=NULL)
                    {
                        textAreaToLoad = tColFunction->textEditor;
                        if(textAreaToLoad!=NULL)
                        {
                            textAreaToLoad->init_save_history();
                        }
                    }
                }
                customFunctionContainer  * tcustFunction = NULL;
                for( tFuncI = 0; tFuncI < (int)customFunctions.size(); tFuncI++)
                {
                    tcustFunction = customFunctions.at(tFuncI);
                    if( tcustFunction!=NULL)
                    {
                        textAreaToLoad = tcustFunction->textEditor;
                        if(textAreaToLoad!=NULL)
                        {
                            textAreaToLoad->init_save_history();
                        }
                    }
                }
                gameObjFileIn.close();
            }
        }
        resourcePostProcessed = true;
    }
}

void gameObjectResource::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    preprocess_self();
    if(cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL && renameBox!=NULL)
    {
        if( editorButtonBar!=NULL)
        {
            editorButtonBar->enable_self();
            //editorButtonBar->set_width(192);
            codeSection = editorButtonBar->get_tab_id();
        }

        int functionI = 0;

        objEventManagebarBox.x = 0;
        objEventManagebarBox.y = 0;
        objEventManagebarBox.w = 0;
        objEventManagebarBox.h = 0;



        int editorPaneW = 256;
        editorPaneList->set_coords(0,0 );
        editorPaneList->set_width( editorPaneW );
        editorPaneList->set_height(viewedSpace->h);
        editorPaneList->barXPadding = GENERAL_GPE_PADDING;
        editorPaneList->barYPadding = GENERAL_GPE_PADDING;
        editorPaneList->barXMargin = 0;
        editorPaneList->barYMargin = 0;

        editorPaneList->clear_list();

        objEventManagebarBox.x = editorPaneList->get_x2pos()+GENERAL_GPE_PADDING;
        objEventManagebarBox.y = editorPaneList->get_ypos()+GENERAL_GPE_PADDING;
        GPE_TextAreaInputBasic *  fTextArea = NULL;

        if(resourceNameLabel!=NULL)
        {
            if( renameBox->get_string()!=resourceName)
            {
                resourceNameLabel->set_name(renameBox->get_string()+"*");
            }
            else
            {
                resourceNameLabel->set_name(resourceName);
            }
        }
        editorPaneList->add_gui_element(resourceNameLabel,true);
        editorPaneList->add_gui_element(editorButtonBar,true);
        //editorPaneList->set_width(viewedSpace->w);

        if( codeSection == OBJ_EDITOR_MODE_COMPONENTS )
        {
            editorPaneList->add_gui_element(componentsMainGuiLabel,true);
            editorPaneList->add_gui_element(resetComponentsButton,true);
            editorPaneList->add_gui_element(inheritParentComponentButton,true);
            editorPaneList->add_gui_element(addNewComponentDropDown,true);
            /*
            editorPaneList->add_gui_element(removeComponentButton,true);
            editorPaneList->add_gui_element(editCompnentButton,true);
            editorPaneList->add_gui_element(componentSettingsButton,true);
            */
            manage_components(viewedSpace, cam);

            GPE_ObjectComponent * tempCustomComponent = NULL;
            GPE_ObjectComponent * otherTempCustomComponent = NULL;
            int iRealCompo = 0;
            int jRealCompo = 0;

            for(  iRealCompo = (int)customComponentRealList.size()-1; iRealCompo>=0; iRealCompo--)
            {
                tempCustomComponent = customComponentRealList.at(iRealCompo);
                if( tempCustomComponent!=NULL)
                {
                    if(tempCustomComponent->get_gear()!=NULL)
                    {
                        if( tempCustomComponent->get_gear()->is_clicked() )
                        {
                            GPE_open_context_menu();
                            MAIN_CONTEXT_MENU->set_width(256);
                            MAIN_CONTEXT_MENU->add_menu_option("Rename Component",0,NULL,-1,NULL,true,true);
                            MAIN_CONTEXT_MENU->add_menu_option("Delete Component",1,NULL,-1,NULL,false,true);
                            if( tempCustomComponent->get_type()=="dropdown" || tempCustomComponent->get_type()=="selectbox")
                            {
                                MAIN_CONTEXT_MENU->add_menu_option("Add Option",2,NULL,-1,NULL,false,true);
                                MAIN_CONTEXT_MENU->add_menu_option("Remove Option",3,NULL,-1,NULL,false,true);
                            }
                            MAIN_CONTEXT_MENU->add_menu_option("Exit Menu",4,NULL,-1,NULL,true,true);
                            int menuSelection = get_popupmenu_result();
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
                                if( display_get_prompt("[WARNING]Object Component deletion?","Are you sure you will like to delete the ["+tempCustomComponent->get_name()+"] component? This action is irreversible!")==DISPLAY_QUERY_YES)
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
                            else
                            {

                            }
                            manage_components(viewedSpace, cam);
                        }
                    }
                }
            }
        }
        else if(codeSection==OBJ_EDITOR_MODE_BASIC)
        {
            editorPaneList->add_gui_element(basicFunctionsSelector,true);
        }
        else if( codeSection==OBJ_EDITOR_MODE_TIMED)
        {
            editorPaneList->add_gui_element(timedFunctionsSelector,true);
        }
        else if(codeSection==OBJ_EDITOR_MODE_COLLISIONS)
        {
            editorPaneList->add_gui_element(colliderFunctionsSelector,true);
            editorPaneList->add_gui_element(newObjectToCollideDropDown,true);
            editorPaneList->add_gui_element(changeObjectToCollideDropDown,true);
            editorPaneList->add_gui_element(removeCollideFunctionButton,true);

            if(newObjectToCollideDropDown!=NULL && removeCollideFunctionButton!=NULL)
            {
                removeCollideFunctionButton->set_width( newObjectToCollideDropDown->get_width() );
                changeObjectToCollideDropDown->set_width( newObjectToCollideDropDown->get_width() );
            }
        }
        else if( codeSection==OBJ_EDITOR_MODE_CUSTOM)
        {
            editorPaneList->add_gui_element(customFunctionsSelector,true);
            editorPaneList->add_gui_element(newCustomObjectFunctionSettingsButton,false);
            editorPaneList->add_gui_element(newCustomObjectFunctionButton,false);
            editorPaneList->add_gui_element(removeCustomObjectFunctionButton,true);
        }
        else if( codeSection==OBJ_EDITOR_MODE_SPECIAL)
        {
            editorPaneList->add_gui_element(specialFunctionsSelector,true);
        }
        else
        {
            editorPaneList->clear_list();
        }
        editorPaneList->add_gui_element(confirmResourceButton,false);
        editorPaneList->process_self(viewedSpace,cam);
        if( confirmResourceButton->is_clicked() || (userInput->check_keyboard_down(kb_ctrl) && userInput->check_keyboard_released(kb_s) ) )
        {
            save_resource();
        }


        //
        if( codeSection == OBJ_EDITOR_MODE_COMPONENTS)
        {
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
                    GPE_ResourceContainer * objectParent = parentObjectField->get_selected_container();
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
                    addNewComponentDropDown->set_selection(-1,false);
                }
            }

            if( resetComponentsButton!=NULL && resetComponentsButton->is_clicked() && (int)customComponentRealList.size() > 0)
            {
                if( display_get_prompt("Are you sure you want to delete all component elements?","There is no undo!")==DISPLAY_QUERY_YES)
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
        else if(codeSection==OBJ_EDITOR_MODE_BASIC)
        {
            if(basicFunctionsSelector!=NULL)
            {
                if( classDeclarationBox->has_content() )
                {
                    basicFunctionsSelector->alter_content_from_value(256,false);
                }
                else
                {
                    basicFunctionsSelector->alter_content_from_value(256,false);
                }
                for( functionI = 0; functionI< DEFAULT_OBJECT_FUNCTIONS_COUNT;functionI++)
                {
                    if( objectFunctions[functionI]!=NULL )
                    {
                        if( objectFunctions[functionI]->has_content() )
                        {
                            basicFunctionsSelector->alter_content_from_value(functionI,true);
                        }
                        else
                        {
                            basicFunctionsSelector->alter_content_from_value(functionI,false);
                        }
                    }
                    else
                    {
                        basicFunctionsSelector->alter_content_from_value(functionI,false);
                    }
                }
                if(  basicFunctionsSelector->get_selected_value() >= 0 && basicFunctionsSelector->get_selected_value() < DEFAULT_OBJECT_FUNCTIONS_COUNT)
                {
                    fTextArea = objectFunctions [ (int)basicFunctionsSelector->get_selected_value() ];
                }
                else
                {
                    //defaults to class variables
                    fTextArea = classDeclarationBox;
                }

                if( fTextArea!=NULL)
                {
                    fTextArea->set_coords(objEventManagebarBox.x,0);
                    fTextArea->set_width(viewedSpace->w-objEventManagebarBox.x );
                    fTextArea->set_height(viewedSpace->h);
                    /*if( basicFunctionsSelector->is_clicked() )
                    {
                        fTextArea->isInUse;
                    }*/
                    fTextArea->process_self(viewedSpace,cam);
                }
            }
        }
        else if( codeSection==OBJ_EDITOR_MODE_TIMED)
        {
            if(timedFunctionsSelector!=NULL)
            {
                for( functionI = 0; functionI< DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT;functionI++)
                {
                    if( timedObjectFunctions[functionI]!=NULL )
                    {
                        if( timedObjectFunctions[functionI]->has_content() )
                        {
                            timedFunctionsSelector->alter_content(functionI,true);
                        }
                        else
                        {
                            timedFunctionsSelector->alter_content(functionI,false);
                        }
                    }
                    else
                    {
                        timedFunctionsSelector->alter_content(functionI,false);
                    }
                }

                if( timedFunctionsSelector->get_selection() < DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT)
                {
                    fTextArea = timedObjectFunctions [timedFunctionsSelector->get_selection() ];
                    if(fTextArea!=NULL)
                    {
                        fTextArea->set_coords(objEventManagebarBox.x,0);
                        fTextArea->set_width(viewedSpace->w-objEventManagebarBox.x );
                        fTextArea->set_height(viewedSpace->h);
                        /*if( timedFunctionsSelector->is_clicked() )
                        {
                            fTextArea->isInUse;
                        }*/
                        fTextArea->process_self(viewedSpace,cam);
                    }
                }
            }
        }
        else if(codeSection==OBJ_EDITOR_MODE_COLLISIONS)
        {
            if(colliderFunctionsSelector!=NULL)
            {
                for( functionI = 0; functionI< (int)collisionFunctions.size();functionI++)
                {
                    if( collisionFunctions[functionI]!=NULL && collisionFunctions[functionI]->textEditor!=NULL)
                    {
                        if( collisionFunctions[functionI]->textEditor->has_content() )
                        {
                            colliderFunctionsSelector->alter_content(functionI,true);
                        }
                        else
                        {
                            colliderFunctionsSelector->alter_content(functionI,false);
                        }
                    }
                    else
                    {
                        colliderFunctionsSelector->alter_content(functionI,false);
                    }
                }
                if(newObjectToCollideDropDown!=NULL)
                {
                    if(newObjectToCollideDropDown->get_selected_container()!=NULL && newObjectToCollideDropDown->just_activated() )
                    {
                        //checks if the collision event already exists
                        if( find_collision_event(GPE_Action_ID)==NULL )
                        {
                            collisionContainer * newColllisionContainer = new collisionContainer();
                            newColllisionContainer->otherObjectType = GPE_Action_ID;
                            colliderFunctionsSelector->add_option(newObjectToCollideDropDown->get_selected_container()->get_name(),-1,NULL,NULL,0,true);
                            collisionFunctions.push_back(newColllisionContainer);
                        }
                        newObjectToCollideDropDown->set_name("Add Collision");
                        newObjectToCollideDropDown->set_selection(-1);
                    }


                    if(changeObjectToCollideDropDown!=NULL && changeObjectToCollideDropDown->just_activated() )
                    {
                        GPE_ResourceContainer * selectedNewContainer = changeObjectToCollideDropDown->get_selected_container();
                        if( selectedNewContainer!=NULL && colliderFunctionsSelector->get_selection() < (int)collisionFunctions.size() )
                        {
                            collisionContainer * currentCollisionContainer = collisionFunctions.at(colliderFunctionsSelector->get_selection() );
                            if( currentCollisionContainer!=NULL)
                            {
                                //checks if the collision event does not already exists
                                if( find_collision_event(GPE_Action_ID) ==NULL )
                                {
                                    GPE_SelectBoxBasic_Option * foundMenuOption= colliderFunctionsSelector->get_option( colliderFunctionsSelector->get_selection() );
                                    if( colliderFunctionsSelector!=NULL )
                                    {
                                        foundMenuOption->optionName = selectedNewContainer->get_name();
                                        foundMenuOption->optionValue = selectedNewContainer->get_global_id();
                                        currentCollisionContainer->otherObjectType = selectedNewContainer->get_global_id();
                                        currentCollisionContainer->otherObjectName = selectedNewContainer->get_name();
                                    }
                                }
                                else if( GPE_Main_Logs!=NULL)
                                {
                                    GPE_Main_Logs->log_general_line("Unable to create another collision event for object["+selectedNewContainer->get_name()+"]." );
                                }
                                changeObjectToCollideDropDown->set_name("Change Collision Object");
                                changeObjectToCollideDropDown->set_selection(-1);
                            }
                        }
                    }


                    if( removeCollideFunctionButton!=NULL)
                    {
                        removeCollideFunctionButton->set_width( newObjectToCollideDropDown->get_width() );
                        if( removeCollideFunctionButton->is_clicked() && colliderFunctionsSelector->get_size()>0 )
                        {
                            if( display_get_prompt("[WARNING]Function deletion?","Are you sure you will like to delete this collision function? This action is irreversible!")==DISPLAY_QUERY_YES)
                            {
                                int selectedCollisonFunction = colliderFunctionsSelector->get_selection();
                                colliderFunctionsSelector->remove_option( selectedCollisonFunction );
                                if( selectedCollisonFunction>=0 && selectedCollisonFunction < (int)collisionFunctions.size() )
                                {
                                    collisionContainer * colToDelete = collisionFunctions.at(selectedCollisonFunction);
                                    if( colToDelete!=NULL)
                                    {
                                        delete colToDelete;
                                        colToDelete = NULL;
                                    }
                                    collisionFunctions.erase(collisionFunctions.begin()+ selectedCollisonFunction);
                                }
                                selectedCollisonFunction--;
                                if( selectedCollisonFunction <0)
                                {
                                    selectedCollisonFunction = 0;
                                }
                                colliderFunctionsSelector->set_selection(selectedCollisonFunction);
                            }

                        }
                    }
                }

                int selectedCollision = colliderFunctionsSelector->get_selection();
                if( selectedCollision >=0 && selectedCollision < (int)collisionFunctions.size() )
                {
                    collisionContainer * currentCollisionContainer = collisionFunctions.at(selectedCollision );
                    fTextArea = currentCollisionContainer->textEditor;
                    if(fTextArea!=NULL)
                    {
                        fTextArea->set_coords(objEventManagebarBox.x,0);
                        fTextArea->set_width(viewedSpace->w-objEventManagebarBox.x );
                        fTextArea->set_height(viewedSpace->h );
                        //if( !colliderFunctionsSelector->is_inuse() )
                        fTextArea->process_self(viewedSpace,cam);
                    }
                }
            }
        }
        else if( codeSection==OBJ_EDITOR_MODE_CUSTOM)
        {
            if(customFunctionsSelector!=NULL)
            {
                for( functionI = 0; functionI < (int)customFunctions.size(); functionI++)
                {
                    if( customFunctions[functionI]!=NULL && customFunctions[functionI]->textEditor!=NULL)
                    {
                        if( customFunctions[functionI]->textEditor->has_content() )
                        {
                            customFunctionsSelector->alter_content(functionI,true);
                        }
                        else
                        {
                            customFunctionsSelector->alter_content(functionI,false);
                        }
                    }
                    else
                    {
                        customFunctionsSelector->alter_content(functionI,false);
                    }
                }
                //customFunctionsSelector->limit_height(viewedSpace->h - customFunctionsSelector->get_ypos() - 128);
                if( customFunctionsSelector->get_selection() >= 0 && customFunctionsSelector->get_selection() < (int)customFunctions.size() )
                {
                    customFunctionContainer * currentCustomFunctionContainer = customFunctions[ customFunctionsSelector->get_selection() ];
                    if( currentCustomFunctionContainer!=NULL)
                    {
                        if( currentCustomFunctionContainer->returnTypeField!=NULL)
                        {
                            currentCustomFunctionContainer->returnTypeField->set_coords(objEventManagebarBox.x,objEventManagebarBox.y);
                            currentCustomFunctionContainer->returnTypeField->set_width(viewedSpace->w/8 );
                            currentCustomFunctionContainer->returnTypeField->process_self(viewedSpace,cam);

                            currentCustomFunctionContainer->parametersField->set_coords(currentCustomFunctionContainer->returnTypeField->get_x2pos()+GENERAL_GPE_PADDING,objEventManagebarBox.y);
                            currentCustomFunctionContainer->parametersField->set_width(viewedSpace->w- currentCustomFunctionContainer->parametersField->get_xpos()-GENERAL_GPE_PADDING );
                            currentCustomFunctionContainer->parametersField->process_self(viewedSpace,cam);

                            fTextArea = currentCustomFunctionContainer->textEditor;
                            if(fTextArea!=NULL)
                            {
                                fTextArea->set_coords(currentCustomFunctionContainer->returnTypeField->get_xpos(),currentCustomFunctionContainer->returnTypeField->get_y2pos() );
                                fTextArea->set_width(viewedSpace->w-fTextArea->get_xpos() );
                                fTextArea->set_height(viewedSpace->h - currentCustomFunctionContainer->returnTypeField->get_y2pos() );
                                //if( !customFunctionsSelector->is_inuse() )
                                fTextArea->process_self(viewedSpace,cam);
                            }
                        }
                    }
                }

                if( newCustomObjectFunctionSettingsButton!=NULL && removeCustomObjectFunctionButton!=NULL && newCustomObjectFunctionButton!=NULL )
                {
                    if( newCustomObjectFunctionSettingsButton->is_clicked() )
                    {
                        GPE_open_context_menu();
                        MAIN_CONTEXT_MENU->set_width(256);
                        MAIN_CONTEXT_MENU->add_menu_option("Rename Function",0,NULL,-1,NULL,true,true);
                        MAIN_CONTEXT_MENU->add_menu_option("Exit Menu",4,NULL,-1,NULL,true,true);
                        int menuSelection = get_popupmenu_result();
                        if( menuSelection==0)
                        {

                        }
                    }
                    else if( newCustomObjectFunctionButton->is_clicked() )
                    {
                        //adds new custom function
                        std::string newFunctionName = get_string_from_popup("Please name your new function","MUST BE VALID","");
                        if( newFunctionName.size() > 0 && is_alnum(newFunctionName,false,true) )
                        {
                            if( find_custom_function(newFunctionName,false)==NULL)
                            {
                                find_custom_function(newFunctionName);
                            }
                            else
                            {
                                display_user_alert("Function Creation Error","Function Already exists!");
                            }
                        }
                        else
                        {
                            display_user_alert("Function Creation Error","Format must be alpha numeric!");
                        }
                    }
                    else if( removeCustomObjectFunctionButton->is_clicked() && customFunctionsSelector->get_size()>0 )
                    {
                        if( display_get_prompt("[WARNING]Function deletion?","Are you sure you will like to delete this custom function? This action is irreversible!")==DISPLAY_QUERY_YES)
                        {
                            int selectedCustomFunction = customFunctionsSelector->get_selection();
                            customFunctionsSelector->remove_option( selectedCustomFunction );
                            if( selectedCustomFunction>=0 && selectedCustomFunction < (int)customFunctions.size() )
                            {
                                customFunctionContainer * customToDelete = customFunctions.at(selectedCustomFunction);
                                if( customToDelete!=NULL)
                                {
                                    delete customToDelete;
                                    customToDelete = NULL;
                                }
                                customFunctions.erase(customFunctions.begin()+ selectedCustomFunction);
                            }
                            customFunctionsSelector->set_selection(selectedCustomFunction);
                        }
                    }
                }
            }
        }
        else if( codeSection==OBJ_EDITOR_MODE_SPECIAL)
        {
            //specialFunctionsSelector
            if(specialFunctionsSelector!=NULL)
            {
                for( functionI = 0; functionI< DEFAULT_SPECIAL_FUNCTIONS_COUNT;functionI++)
                {
                    if( specialObjectFunctions[functionI]!=NULL )
                    {
                        if( specialObjectFunctions[functionI]->has_content() )
                        {
                            specialFunctionsSelector->alter_content(functionI,true);
                        }
                        else
                        {
                            specialFunctionsSelector->alter_content(functionI,false);
                        }
                    }
                    else
                    {
                        specialFunctionsSelector->alter_content(functionI,false);
                    }
                }

                if( specialFunctionsSelector->get_selection() < DEFAULT_SPECIAL_FUNCTIONS_COUNT )
                {
                    fTextArea = specialObjectFunctions [ specialFunctionsSelector->get_selection()];
                    if(fTextArea!=NULL)
                    {
                        fTextArea->set_coords(objEventManagebarBox.x,0);
                        fTextArea->set_width(viewedSpace->w-objEventManagebarBox.x );
                        fTextArea->set_height(viewedSpace->h);
                        //if( !specialFunctionsSelector->is_inuse() )
                        fTextArea->process_self(viewedSpace,cam);
                    }
                }
            }
        }

        if( codeSection !=editorButtonBar->get_tab_pos() )
        {
            userInput->reset_all_input();
            process_self(viewedSpace,cam);
        }
    }
}

void gameObjectResource::render_self(GPE_Renderer * cRender,GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL)
    {
        if( forceRedraw)
        {
            render_rectangle(cRender,0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_TEMPLATE->Program_Color,false);
        }
        if( codeSection==OBJ_EDITOR_MODE_COMPONENTS )
        {
            if( customComponentsGuiList!=NULL)
            {
                customComponentsGuiList->render_self(cRender,viewedSpace,cam,forceRedraw);
            }
            if( forceRedraw)
            {
                render_rectangle(cRender,objEventManagebarBox.x-1,0,objEventManagebarBox.x+1,viewedSpace->h,GPE_MAIN_TEMPLATE->Main_Border_Color,false);
            }
        }
        else
        {
            GPE_TextAreaInputBasic *  fTextArea = NULL;
            if(codeSection==OBJ_EDITOR_MODE_BASIC)
            {
                if(basicFunctionsSelector!=NULL)
                {
                    //render_new_text(cRender,basicFunctionsSelector->get_xpos()+basicFunctionsSelector->get_width()/2,basicFunctionsSelector->get_ypos(),"Object Functions",GPE_MAIN_TEMPLATE->Main_Box_Font_Color,FONT_CATEGORY_BAR,FA_CENTER,FA_BOTTOM);
                    if( objectFunctionInEdit[0] < DEFAULT_OBJECT_FUNCTIONS_COUNT)
                    {
                        if(  basicFunctionsSelector->get_selected_value() >= 0 && basicFunctionsSelector->get_selected_value() < DEFAULT_OBJECT_FUNCTIONS_COUNT)
                        {
                            fTextArea = objectFunctions [ (int)basicFunctionsSelector->get_selected_value() ];
                        }
                        else
                        {
                            //defaults to class variables
                            fTextArea = classDeclarationBox;
                        }
                    }
                }
            }
            else if(codeSection==OBJ_EDITOR_MODE_TIMED)
            {
                if(timedFunctionsSelector!=NULL)
                {
                    if( objectFunctionInEdit[1] < DEFAULT_OBJECT_FUNCTIONS_COUNT)
                    {
                        fTextArea = timedObjectFunctions [timedFunctionsSelector->get_selection() ];
                    }
                }
            }
            else if(codeSection==OBJ_EDITOR_MODE_COLLISIONS)
            {
                if(colliderFunctionsSelector!=NULL)
                {
                    if( colliderFunctionsSelector->get_selection()  >= 0 && colliderFunctionsSelector->get_selection() < (int)collisionFunctions.size() )
                    {
                        collisionContainer * currentCollisionContainer = collisionFunctions.at(colliderFunctionsSelector->get_selection() );
                        fTextArea = currentCollisionContainer->textEditor;
                    }
                }
            }
            else if(codeSection==OBJ_EDITOR_MODE_CUSTOM)
            {
                if(customFunctionsSelector!=NULL)
                {
                    if( customFunctionsSelector->get_selection() >= 0 && customFunctionsSelector->get_selection() < (int)customFunctions.size() )
                    {
                        customFunctionContainer * currentCustomFunctionContainer = customFunctions[ customFunctionsSelector->get_selection() ];
                        if( currentCustomFunctionContainer!=NULL)
                        {
                            if( currentCustomFunctionContainer->returnTypeField!=NULL)
                            {
                                currentCustomFunctionContainer->returnTypeField->render_self(cRender,viewedSpace,cam,forceRedraw);
                            }
                            if( currentCustomFunctionContainer->parametersField!=NULL)
                            {
                                currentCustomFunctionContainer->parametersField->render_self(cRender,viewedSpace,cam,forceRedraw);
                            }
                            fTextArea = currentCustomFunctionContainer->textEditor;
                        }
                    }
                }
            }
            else if(codeSection==OBJ_EDITOR_MODE_SPECIAL)
            {
                if(specialFunctionsSelector!=NULL)
                {
                    if( objectFunctionInEdit[3] < DEFAULT_SPECIAL_FUNCTIONS_COUNT)
                    {
                        fTextArea = specialObjectFunctions [specialFunctionsSelector->get_selection() ];
                    }
                }
            }

            if(fTextArea!=NULL)
            {
                fTextArea->render_self(cRender,viewedSpace,cam,forceRedraw);
            }
        }
        if( editorPaneList!=NULL)
        {

            editorPaneList->render_self(cRender,viewedSpace,cam,forceRedraw);
        }
    }
}


void gameObjectResource::save_resource(std::string alternatePath, int backupId)
{
    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = get_path_from_file(alternatePath);
    if( path_exists(soughtDir) )
    {
        newFileOut = alternatePath;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/objects/";
        newFileOut = soughtDir + resourceName+".gpf";
    }

    if( !usingAltSaveSource )
    {
        preprocess_self();
    }
    displayMessageTitle = "Saving GameObject";
    displayMessageSubtitle = resourceName;
    displayMessageString = "...";
    display_user_messaage();
    int i = 0;
    collisionContainer * cColContainer;
    customFunctionContainer * customFuncContainer;
    GPE_ResourceContainer * objsParentContainer = projectParentFolder->find_resource_from_name(RESOURCE_TYPE_NAMES[RESOURCE_TYPE_OBJECT]+"s" );
    GPE_ResourceContainer * tOtherContainer = NULL;
    std::string otherColContainerName = "";
    spriteIndex = -1;
    if( spriteField!=NULL)
    {
        spriteIndex = spriteField->get_selected_id();
    }

    std::ofstream newGameObjFile( newFileOut.c_str() );
    if( !newGameObjFile.fail() )
    {
        resourceNameLabel->set_name(resourceName );
        //makes sure the file is open
        if (newGameObjFile.is_open())
        {
            newGameObjFile << "#    --------------------------------------------------  # \n";
            newGameObjFile << "#     \n";
            newGameObjFile << "#     \n";
            newGameObjFile << "#    Game Pencil Engine Project GameObject File \n";
            newGameObjFile << "#    Created automatically via the Game Pencil Engine Editor \n";
            newGameObjFile << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
            newGameObjFile << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
            newGameObjFile << "#     \n";
            newGameObjFile << "#     \n";
            newGameObjFile << "#    --------------------------------------------------  # \n";
            newGameObjFile << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
            newGameObjFile << "ResourceName=" << resourceName << "\n";
            if( spriteField!=NULL)
            {
                 newGameObjFile << "SpriteId=" << spriteField->get_selected_name() << "\n";
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

            //Saves the main functions
            for(  i = 0; i < DEFAULT_OBJECT_FUNCTIONS_COUNT; i++)
            {
                if( objectFunctions[i]!=NULL)
                {
                    if( objectFunctions[i]->has_content() )
                    {
                        newGameObjFile << "\n[[[Function="+DEFAULT_OBJECT_FUNCTIONS_NAMES[i]+"]]] \n";
                        objectFunctions[i]->write_data_into_file(&newGameObjFile);
                    }
                }
            }

            //Saves the special functions
            for(  i = 0; i < DEFAULT_SPECIAL_FUNCTIONS_COUNT; i++)
            {
                if( specialObjectFunctions[i]!=NULL)
                {
                    if( specialObjectFunctions[i]->has_content() )
                    {
                        newGameObjFile << "\n[[[SpecialFunction="+DEFAULT_OBJECT_SPECIALS_NAMES[i]+"]]] \n";
                        specialObjectFunctions[i]->write_data_into_file(&newGameObjFile);
                    }
                }
            }

            //Saves the timed functions
            for(  i = 0; i < DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT; i++)
            {
                if( timedObjectFunctions[i]!=NULL)
                {
                    if( timedObjectFunctions[i]->has_content() )
                    {
                        newGameObjFile << "\n[[[TimedFunction=Timer"+int_to_string(i)+"]]] \n";
                        timedObjectFunctions[i]->write_data_into_file(&newGameObjFile);
                    }
                }
            }
            //Saves the Collision Functions
            for(  i = 0; i < (int)collisionFunctions.size(); i++)
            {
                cColContainer = collisionFunctions[i];
                if( cColContainer!=NULL)
                {
                    otherColContainerName = int_to_string(cColContainer->otherObjectType);
                    tOtherContainer = objsParentContainer->find_resource_from_id(cColContainer->otherObjectType);
                    if( tOtherContainer!=NULL)
                    {
                        otherColContainerName = tOtherContainer->get_name();
                    }
                    if( cColContainer->textEditor!=NULL)
                    {
                        if( cColContainer->textEditor->has_content() )
                        {
                            newGameObjFile << "\n[[[CollideFunction="+cColContainer->otherObjectName+"]]] \n";
                            cColContainer->textEditor->write_data_into_file(&newGameObjFile);
                        }
                    }
                }
            }
            //Saves the Custom Functions
            for(  i = 0; i < (int)customFunctions.size(); i++)
            {
                customFuncContainer = customFunctions[i];
                if( customFuncContainer!=NULL)
                {
                    newGameObjFile << "\n[[[CustomFunction="+customFuncContainer->functionName+",";

                    if( customFuncContainer->returnTypeField!=NULL)
                    {
                        newGameObjFile << customFuncContainer->returnTypeField->get_string()+",";
                    }
                    else
                    {
                        newGameObjFile << "void,";
                    }
                    if( customFuncContainer->parametersField!=NULL)
                    {
                        newGameObjFile << customFuncContainer->parametersField->get_string()+"]]]\n";
                    }
                    else
                    {
                        newGameObjFile << "]]]\n";
                    }

                    if( customFuncContainer->textEditor!=NULL && customFuncContainer->textEditor->has_content() )
                    {
                        customFuncContainer->textEditor->write_data_into_file(&newGameObjFile);
                    }
                    else
                    {
                        newGameObjFile << "\n";
                    }
                }
            }

            newGameObjFile.close();
        }
    }
    if( !usingAltSaveSource)
    {
        isModified = false;
    }
}

int gameObjectResource::search_for_string(std::string needle)
{
    int foundStrings = 0;
    GPE_TextAreaInputBasic * foundTextArea = NULL;
    GPE_Main_Logs->log_general_comment("Searching ["+resourceName+"] object..");


    if( GPE_MAIN_GUI!=NULL  )
    {
        int i = 0;
        collisionContainer * tempCollisionContainer = NULL;
        customFunctionContainer * tempCustomFunction = NULL;
        GPE_MAIN_GUI->searchResultProjectName = parentProjectName;
        GPE_MAIN_GUI->searchResultResourceId = globalResouceIdNumber;
        GPE_MAIN_GUI->searchResultResourceName = resourceName;
        for( i = 0; i < DEFAULT_OBJECT_FUNCTIONS_COUNT; i++)
        {
            foundTextArea = objectFunctions[i];
            if( foundTextArea!=NULL && foundTextArea->has_content() )
            {
                foundStrings=foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,true, DEFAULT_OBJECT_FUNCTIONS_NAMES[i] );
            }
        }

        for( i = 0; i < DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT; i++)
        {
            foundTextArea = timedObjectFunctions[i];
            if( foundTextArea!=NULL && foundTextArea->has_content() )
            {
                foundStrings=foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,true, "Timed Function("+int_to_string(i)+")");
            }
        }

        for( i = 0; i < DEFAULT_SPECIAL_FUNCTIONS_COUNT; i++)
        {
            foundTextArea = specialObjectFunctions[i];
            if( foundTextArea!=NULL && foundTextArea->has_content() )
            {
                foundStrings=foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,true, DEFAULT_OBJECT_SPECIALS_NAMES[i] );
            }
        }

        for( i = (int)customFunctions.size()-1; i >= 0; i--)
        {
            tempCustomFunction = customFunctions[i];
            if( tempCustomFunction!=NULL)
            {
                foundTextArea = tempCustomFunction->textEditor;
                if( foundTextArea!=NULL && foundTextArea->has_content() )
                {
                    foundStrings=foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,true, tempCustomFunction->functionName );
                }
            }
        }

        for( i = (int)collisionFunctions.size()-1; i >= 0; i--)
        {
            tempCollisionContainer = collisionFunctions[i];
            if( tempCollisionContainer!=NULL)
            {
                foundTextArea = tempCollisionContainer->textEditor;
                if( foundTextArea!=NULL && foundTextArea->has_content() )
                {
                    foundStrings=foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,true,tempCollisionContainer->otherObjectName );
                }
            }
        }
    }
    return foundStrings;
}

int gameObjectResource::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundStrings = 0;
    int tempFoundCount = 0;
    GPE_TextAreaInputBasic * foundTextArea = NULL;
    GPE_Main_Logs->log_general_comment("Searching ["+resourceName+"] object..");
    int i = 0;
    collisionContainer * tempCollisionContainer = NULL;
    customFunctionContainer * tempCustomFunction = NULL;

    if( GPE_MAIN_GUI!=NULL )
    {
        GPE_MAIN_GUI->searchResultProjectName = parentProjectName;
        GPE_MAIN_GUI->searchResultResourceId = globalResouceIdNumber;
        GPE_MAIN_GUI->searchResultResourceName = resourceName;
        for( i = 0; i < DEFAULT_OBJECT_FUNCTIONS_COUNT; i++)
        {
            foundTextArea = objectFunctions[i];
            if( foundTextArea!=NULL && foundTextArea->has_content() )
            {
                tempFoundCount =foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,false, DEFAULT_OBJECT_FUNCTIONS_NAMES[i] );
                if( tempFoundCount > 0)
                {
                    foundStrings+=tempFoundCount;
                    displayMessageString = "Replaced "+int_to_string( foundTextArea->replace_all_found(needle, newStr ) )+" copies.";;
                }
            }
        }

        for( i = 0; i < DEFAULT_OBJECT_TIMED_FUNCTIONS_COUNT; i++)
        {
            foundTextArea = timedObjectFunctions[i];
            if( foundTextArea!=NULL && foundTextArea->has_content() )
            {
                tempFoundCount =foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,false, "Timed Function("+int_to_string(i)+")");
                if( tempFoundCount > 0)
                {
                    foundStrings+=tempFoundCount;
                    displayMessageString = "Replaced "+int_to_string( foundTextArea->replace_all_found(needle, newStr ) )+" copies.";;
                }
            }
        }

        for( i = 0; i < DEFAULT_SPECIAL_FUNCTIONS_COUNT; i++)
        {
            foundTextArea = specialObjectFunctions[i];
            if( foundTextArea!=NULL && foundTextArea->has_content() )
            {
                tempFoundCount =foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,false, DEFAULT_OBJECT_SPECIALS_NAMES[i] );
                if( tempFoundCount > 0)
                {
                    foundStrings+=tempFoundCount;
                    displayMessageString = "Replaced "+int_to_string( foundTextArea->replace_all_found(needle, newStr ) )+" copies.";;
                }
            }
        }

        for( i = (int)customFunctions.size()-1; i >= 0; i--)
        {
            tempCustomFunction = customFunctions[i];
            if( tempCustomFunction!=NULL)
            {
                foundTextArea = tempCustomFunction->textEditor;
                if( foundTextArea!=NULL && foundTextArea->has_content() )
                {
                    tempFoundCount =foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,false, tempCustomFunction->functionName );
                    if( tempFoundCount > 0)
                    {
                        foundStrings+=tempFoundCount;
                        displayMessageString = "Replaced "+int_to_string( foundTextArea->replace_all_found(needle, newStr ) )+" copies.";;
                    }
                }
            }
        }

        for( i = (int)collisionFunctions.size()-1; i >= 0; i--)
        {
            tempCollisionContainer = collisionFunctions[i];
            if( tempCollisionContainer!=NULL)
            {
                foundTextArea = tempCollisionContainer->textEditor;
                if( foundTextArea!=NULL && foundTextArea->has_content() )
                {
                    tempFoundCount =foundTextArea->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() ,false,tempCollisionContainer->otherObjectName );
                    if( tempFoundCount > 0)
                    {
                        foundStrings+=tempFoundCount;
                        displayMessageString = "Replaced "+int_to_string( foundTextArea->replace_all_found(needle, newStr ) )+" copies.";;                        }
                }
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
