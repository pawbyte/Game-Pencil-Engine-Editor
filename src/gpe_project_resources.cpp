/*
gpe_project_resources.cpp
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

#include "gpe_project_resources.h"

gamePencilEditorSettingsResource * MAIN_EDITOR_SETTINGS = NULL;
gameJSCompilerSettingsResource * GPE_JS_COMPILER_SETTINGS = NULL;
gameCPPCompilerSettingsResource * GPE_CPP_COMPILER_SETTINGS = NULL;
gamePencilAboutPageResource * MAIN_ABOUT_PAGE = NULL;
gamePencilHelpPageResource * MAIN_HELP_PAGE = NULL;
gamePencilStartPageResource * MAIN_START_PAGE = NULL;
gamePencilProjectBrowserResource * PROJECT_BROWSER_PAGE = NULL;

std::string spriteDataLabels[SPRITE_DATA_FIELD_COUNT];
int spriteDataLabelWidth;

std::string tsDataLabels[TILESHEET_DATA_FIELD_COUNT];
int tsDataLabelWidth;

std::string SUPPORTED_AUDIO_EXT[SUPPORTED_AUDIO_FORMAT_COUNT];
std::string SUPPORTED_VIDEO_EXT[SUPPORTED_VIDEO_FORMAT_COUNT];
std::string SUPPORTED_FONT_EXT[FONT_FILE_TYPES];

std::string DEFAULT_OBJECT_FUNCTIONS_NAMES[DEFAULT_OBJECT_FUNCTIONS_COUNT];

std::string DEFAULT_OBJECT_SPECIALS_NAMES[DEFAULT_SPECIAL_FUNCTIONS_COUNT];

std::string DEFAULT_OBJECT_FUNCTIONS_HTML5_NAMES[DEFAULT_OBJECT_FUNCTIONS_COUNT];
std::string DEFAULT_OBJECT_SPECIALS_HTML5_NAMES[DEFAULT_SPECIAL_FUNCTIONS_COUNT];

std::string DEFAULT_SCENE_SUBEDITOR_NAMES[5];

standardEditableGameResource::standardEditableGameResource(GPE_ResourceContainer * ppFolder)
{
    resourceName = "";
    globalResouceIdNumber = -1;
    commonButtonAlignment = 0;
    justOpenedThisFrame = false;
    parentProjectName = "";
    projectParentFolder = ppFolder;
    if( projectParentFolder!=NULL)
    {
        parentProjectName = projectParentFolder->get_name();
    }
    renameBox = new GPE_TextInputBasic("","Name");
    renameBox->set_label("Name");

    loadResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/folder.png","Load Resource",-1);
    exportResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/plane.png","Export Resource TO File",-1);
    saveResourceButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/save.png","Save Resource to File",-1);

    confirmResourceButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/check.png","Confirm Changes","Confirm and Save Changes");
    cancelResourceButton =  new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/times.png","Cancel Changes","Cancel Changes and Revert to previous settings");

    int maxSize = std::max(confirmResourceButton->get_width(),cancelResourceButton->get_width() );
    //confirmResourceButton->set_width(maxSize);
    //cancelResourceButton->set_width(maxSize);
    //makes it so the main button options are the same width along with the rename string box too for design purposes

    editorPaneList = new GPE_GuiElementList();
}


standardEditableGameResource::~standardEditableGameResource()
{
    if( confirmResourceButton!=NULL)
    {
        delete confirmResourceButton;
        confirmResourceButton = NULL;
    }
    if( cancelResourceButton!=NULL)
    {
        delete cancelResourceButton;
        cancelResourceButton = NULL;
    }
    if( loadResourceButton!=NULL)
    {
        delete loadResourceButton;
        loadResourceButton = NULL;
    }
    if( exportResourceButton!=NULL)
    {
        delete exportResourceButton;
        exportResourceButton = NULL;
    }
    if( saveResourceButton!=NULL)
    {
        delete saveResourceButton;
        saveResourceButton = NULL;
    }
    if( renameBox!=NULL)
    {
        delete renameBox;
        renameBox = NULL;
    }
    if( editorPaneList!=NULL)
    {
        delete editorPaneList;
        editorPaneList = NULL;
    }
}

std::string standardEditableGameResource::get_current_name()
{
    if( renameBox!=NULL)
    {
        if( is_alnum(renameBox->get_string(),false,true)==true)
        {
            return renameBox->get_string();
        }
        else if( GPE_Main_Logs!=NULL && resourceType!=RESOURCE_TYPE_PROJECT_SETTINGS && globalResouceIdNumber >=1000 )
        {
            GPE_Main_Logs->log_general_comment("Non-alphabetical resource ["+renameBox->get_string()+"] being renamed to "+resourceName+"." );
        }
    }
    return get_name();
}

GPE_GeneralGuiElement * standardEditableGameResource::add_gui_component(std::string componentData)
{
    if( (int)componentData.size() > 0)
    {
        int colonPos = componentData.find(":");
        int equalPos = componentData.find("==|||==");

        if( colonPos!=(int)std::string::npos && equalPos!=(int)std::string::npos && (equalPos>colonPos) )
        {
            std::string newComponentType = split_first_string(componentData,":");
            std::string newComponentName = split_first_string(componentData,"==|||==");

            GPE_GeneralGuiElement * newComponentField = NULL;
            if( newComponentType=="checkbox")
            {
                newComponentField = new GPE_CheckBoxBasic(newComponentName);
            }
            else if( newComponentType=="colorpicker")
            {
                GPE_Input_Field_Color * newColorField = new GPE_Input_Field_Color( newComponentName);
                newColorField->set_label(newComponentName);
                newComponentField = newColorField;
            }
            else if( newComponentType=="dropdown")
            {
                newComponentField = new GPE_DropDown_Menu( newComponentName,false);
            }
            else if(newComponentType=="inputtext")
            {
                GPE_TextInputBasic * tempNewField = new GPE_TextInputBasic("");
                tempNewField->set_name(newComponentName);
                tempNewField->set_label(newComponentName);
                tempNewField->set_width(256);
                newComponentField = tempNewField;
            }
            else if(newComponentType=="inputnumber")
            {
                GPE_TextInputBasic * tempNewField = new GPE_TextInputNumber("");
                tempNewField->set_name(newComponentName);
                tempNewField->set_label(newComponentName);
                tempNewField->set_width(256);
                newComponentField = tempNewField;
            }
            else if(newComponentType=="radio")
            {
                GPE_RadioButtonControllerBasic * tempRadioButton = new GPE_RadioButtonControllerBasic();
                tempRadioButton->set_name(newComponentName);
                newComponentField = tempRadioButton;
            }
            else if(newComponentType=="labeltext")
            {
                newComponentField = new GPE_Label_Text(newComponentName,newComponentName);
            }
            else if(newComponentType=="texturl")
            {
                newComponentField = new GPE_TextURL(newComponentName,newComponentName);
            }
            else if( newComponentType=="resourcedropdown")
            {
                std::string dropdownType = split_first_string(componentData,"=");
                newComponentField = new GPE_DropDown_Resouce_Menu(0,0,newComponentName,NULL );
            }
            if( newComponentField!=NULL)
            {
                newComponentField->load_data(componentData);
            }
            //record_error("Returning ["+newComponentType+"] gui component named ["+newComponentName+"] using ["+componentData+"] data.");
            return newComponentField;
        }
    }
    record_error("Returning NULL gui component using ["+componentData+"] data.");
    return NULL;
}

void standardEditableGameResource::prerender_self( )
{
    if( loadResourceButton!=NULL)
    {
        loadResourceButton->prerender_self( );
    }
    if( exportResourceButton!=NULL)
    {
        exportResourceButton->prerender_self( );
    }
    if( saveResourceButton!=NULL)
    {
        saveResourceButton->prerender_self( );
    }
    if( confirmResourceButton!=NULL)
    {
        confirmResourceButton->prerender_self( );
    }
    if( cancelResourceButton!=NULL)
    {
        cancelResourceButton->prerender_self( );
    }
}

void standardEditableGameResource::open_code( int lineNumb, int colNumb, std::string codeTitle )
{

}

void standardEditableGameResource::process_resource(GPE_Rect * viewedSpace ,GPE_Rect * cam )
{
    isModified = false;
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    process_self(viewedSpace,cam);
    process_export();
    /*
    if( confirmResourceButton!=NULL)
    {
        if( commonButtonAlignment==1)
        {
            cancelResourceButton->set_coords(viewedSpace->w-cancelResourceButton->get_width()-GENERAL_GPE_PADDING,viewedSpace->h-cancelResourceButton->get_height()-GPE_AVERAGE_LINE_HEIGHT);
            cancelResourceButton->process_self(viewedSpace,cam);
            if( confirmResourceButton!=NULL)
            {
                if( confirmResourceButton->is_enabled() )
                {
                    confirmResourceButton->set_coords(cancelResourceButton->get_xpos()-confirmResourceButton->get_width()-GENERAL_GPE_PADDING,cancelResourceButton->get_ypos());
                }
                else
                {
                    confirmResourceButton->set_coords(cancelResourceButton->get_xpos()-confirmResourceButton->get_width()-GENERAL_GPE_PADDING,cancelResourceButton->get_ypos());
                }
                confirmResourceButton->process_self(viewedSpace,cam);
                if( confirmResourceButton->is_clicked() && editorMode==0 )
                {
                    if( renameBox!=NULL)
                    {
                        set_name( renameBox->get_string() );
                    }
                }
            }
        }
        else
        {
            cancelResourceButton->set_coords(-1,viewedSpace->h-cancelResourceButton->get_height()-GPE_AVERAGE_LINE_HEIGHT);
            cancelResourceButton->process_self(viewedSpace,cam);
            if( confirmResourceButton!=NULL)
            {
                if( cancelResourceButton->is_enabled() )
                {
                    confirmResourceButton->set_coords(-1,cancelResourceButton->get_ypos()-confirmResourceButton->get_height()-GPE_AVERAGE_LINE_HEIGHT);
                }
                else
                {
                    confirmResourceButton->set_coords(-1,viewedSpace->h-confirmResourceButton->get_height()-GPE_AVERAGE_LINE_HEIGHT);
                }
                confirmResourceButton->process_self(viewedSpace,cam);
                if( confirmResourceButton->is_clicked() && editorMode==0 )
                {
                    if( renameBox!=NULL)
                    {
                        set_name( renameBox->get_string() );
                    }
                }
            }
        }
    }
    if( renameBox!=NULL)
    {
        renameBox->process_self(viewedSpace,cam);
        //set_name( renameBox->get_string() );
        if(resourceName!=renameBox->get_string() )
        {
            isModified = true;
        }
    }
    */
}

void standardEditableGameResource::process_export()
{
    bool exportCalled = false;
    if( exportResourceButton!=NULL && exportResourceButton->is_clicked() )
    {
        exportCalled = true;
    }
    else if( input->check_keyboard_down(kb_ctrl) && input->check_keyboard_pressed(kb_e) )
    {
        exportCalled = true;
        input->reset_all_input();
    }

    if( exportCalled )
    {
        if( resourceType >=0 && resourceType < res_type_count )
        {
            std::string fileToExportName = GPE_GetSaveFileName( "Export "+RESOURCE_TYPE_NAMES[resourceType]+" to file",".gpf", MAIN_GUI_SETTINGS->fileSaveExportFileDir );
            if( (int)fileToExportName.size() > 0)
            {
                if(  get_file_ext(fileToExportName) !="gpf" )
                {
                    fileToExportName = get_file_noext(fileToExportName)+".gpf";
                }
                GPE_Main_Logs->log_general_line("Exporting "+RESOURCE_TYPE_NAMES[resourceType]+" resource to "+fileToExportName+".");
                save_resource(fileToExportName);
            }
            else
            {
                GPE_Main_Logs->log_general_comment("Export of ["+resourceName+"] canceled.");
            }
        }
        exportResourceButton->set_clicked( false );
    }
}

void standardEditableGameResource::integrate_into_syntax()
{
    if( CURRENT_PROJECT!=NULL)
    {
        if( resourceType >=0 && resourceType < res_type_count)
        {
            CURRENT_PROJECT->add_project_keyword(resourceName,"Project "+RESOURCE_TYPE_NAMES[resourceType]+" Resource",-1,"");
        }
    }
}

void standardEditableGameResource::preprocess_self(std::string alternatePath)
{

}

void standardEditableGameResource::process_self(GPE_Rect * viewedSpace ,GPE_Rect * cam )
{

}

void standardEditableGameResource::render_resource(GPE_Rect * viewedSpace ,GPE_Rect *cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL)
    {
        if( forceRedraw || justOpenedThisFrame )
        {
            //gpe->render_rectangle( 0,0,viewedSpace->w,viewedSpace->h,GPE_MAIN_THEME->Program_Color,false);
            forceRedraw = true;
        }
        render_self( viewedSpace,cam,forceRedraw);
    }
    justOpenedThisFrame = false;
}


void standardEditableGameResource::render_self(GPE_Rect * viewedSpace ,GPE_Rect *cam,bool forceRedraw  )
{

}

void standardEditableGameResource::seek_parent_name()
{
    if( projectParentFolder!=NULL)
    {
        parentProjectName = projectParentFolder->get_name();
    }
}

void standardEditableGameResource::set_name(std::string newName)
{
    resourceName = newName;
    if(renameBox!=NULL)
    {
        renameBox->set_string(newName);
    }
}

void standardEditableGameResource::set_parent_name(std::string newName)
{
    parentProjectName = newName;
}

int standardEditableGameResource::search_for_string(std::string needle)
{
    return 0;
}

int standardEditableGameResource::search_and_replace_string(std::string needle, std::string newStr )
{
    return 0;
}

bool standardEditableGameResource::write_header_on_file(std::ofstream * fileTarget)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        *fileTarget << "#    --------------------------------------------------  # \n";
        *fileTarget << "#     \n";
        *fileTarget << "#     \n";
        if( resourceType >=0 && resourceType < res_type_count)
        {
            *fileTarget << "#    Game Pencil Engine Project Game " << RESOURCE_TYPE_NAMES[resourceType] <<" DataFile \n";
        }
        *fileTarget << "#    Created automatically via the Game Pencil Engine Editor \n";
        *fileTarget << "#    Warning: Manually editing this file may cause unexpected bugs and errors. \n";
        *fileTarget << "#    If you have any problems reading this file please report it to help@pawbyte.com . \n";
        *fileTarget << "#     \n";
        *fileTarget << "#     \n";
        *fileTarget << "#    --------------------------------------------------  # \n";
        *fileTarget << "Version=" << GPE_VERSION_DOUBLE_NUMBER << "\n";
        *fileTarget << "ResourceName=" << resourceName << "\n";
        *fileTarget << "#     \n";
        return true;
    }
    return false;
}
