/*
dictionaryresource.cpp
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

#include "dictionaryresource.h"

definitionResource::definitionResource(std::string key, std::string value)
{
    myKey = key;
    myValue = value;
    keyField = new GPE_TextInputBasic(myKey);
    valueField = new GPE_TextInputBasic(myValue);
    removeSelfButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/remove.png","Remove Definition",-1,32 );
}

definitionResource::~definitionResource()
{
    if( keyField!=NULL)
    {
        delete keyField;
        keyField = NULL;
    }

    if( valueField!=NULL)
    {
        delete valueField;
        valueField = NULL;
    }

    if( removeSelfButton!=NULL)
    {
        delete removeSelfButton;
        removeSelfButton = NULL;
    }
}

dictionaryResource::dictionaryResource(GPE_GeneralResourceContainer * pFolder )
{
    projectParentFolder = pFolder;
    addDefinitionButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/plus-circle.png","Add Definition","Adds a new definition to dictionary");
    clearDictionaryButton = new GPE_ToolPushButton( APP_DIRECTORY_NAME+"resources/gfx/buttons/eraser.png","Clear Dictionary","Clears the entire dictionary");
}

dictionaryResource::~dictionaryResource()
{
    if( addDefinitionButton!=NULL )
    {
        delete addDefinitionButton;
        addDefinitionButton = NULL;
    }
    clear_dictionary();
}

definitionResource * dictionaryResource::add_definition( std::string key, std::string value)
{
    if( (int)key.size() > 0 )
    {
        definitionResource * newDefinition = new definitionResource( key, value);
        if( newDefinition!=NULL)
        myDefinitions.push_back( newDefinition );
        return newDefinition;
    }
    return NULL;
}

void dictionaryResource::clear_dictionary()
{
    definitionResource * tempObj = NULL;
    for( int i = (int)myDefinitions.size()-1; i >=0; i--)
    {
        tempObj = myDefinitions[i];
        if( tempObj!=NULL)
        {
            delete tempObj;
            tempObj = NULL;
        }
    }
    myDefinitions.clear();
}

bool dictionaryResource::export_and_play_native( bool launchProgram )
{
    return false;
}


void dictionaryResource::integrate_into_syntax()
{

}

void dictionaryResource::open_code(int lineNumb, int colNumb, std::string codeTitle )
{

}

void dictionaryResource::prerender_self( )
{

}

void dictionaryResource::preprocess_self(std::string alternatePath )
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        displayMessageTitle = "Processing Dictionary";
        displayMessageSubtitle = resourceName;
        displayMessageString = "...";
        display_user_messaage();
        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/dictionaries/";
        if( file_exists(alternatePath) )
        {
            newFileIn = alternatePath;
            soughtDir = get_path_from_file(newFileIn);
        }
        else
        {
            newFileIn = soughtDir + resourceName+".gpf";
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        GPE_Report("Loading dictionary - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
        {
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
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
                std::string foundWord = "";
                std::string foundValue = "";
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
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
                                equalPos = currLineToBeProcessed.find_first_of("=");
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

                                if( keyString=="ResourceName")
                                {
                                    renameBox->set_string(valString);
                                }
                                else if( keyString=="Definition" || keyString=="definition" || keyString=="word" )
                                {
                                    foundWord = split_first_string(valString,",,,");
                                    foundValue = split_first_string(valString,",,,");
                                    add_definition( foundWord, foundValue);
                                }
                            }
                        }
                    }
                    else
                    {
                        GPE_Report("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
    }
}

void dictionaryResource::process_self(GPE_Rect * viewedSpace ,GPE_Rect * cam )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && viewedSpace!=NULL && editorPaneList!=NULL)
    {
        editorPaneList->set_coords(0,0);
        editorPaneList->set_width(viewedSpace->w);
        editorPaneList->set_height(viewedSpace->h);
        editorPaneList->barXPadding = GENERAL_GPE_PADDING;
        editorPaneList->barYPadding = GENERAL_GPE_PADDING;
        editorPaneList->barXMargin  = GENERAL_GPE_PADDING;
        editorPaneList->barYMargin  = 0;
        editorPaneList->clear_list();

        editorPaneList->add_gui_element(renameBox,true);
        editorPaneList->add_gui_element( addDefinitionButton,false );
        editorPaneList->add_gui_element( clearDictionaryButton,true );

        definitionResource * tempObj = NULL;
        int i = 0;
        for( i = 0; i < (int)myDefinitions.size(); i++ )
        {
            tempObj = myDefinitions[i];
            if( tempObj!=NULL)
            {
                editorPaneList->add_gui_element(tempObj->keyField,false);
                editorPaneList->add_gui_element(tempObj->valueField,false);
                editorPaneList->add_gui_element(tempObj->removeSelfButton,true);
            }
        }


        editorPaneList->add_gui_element(exportResourceButton,false);
        editorPaneList->add_gui_element(loadResourceButton,true);
        editorPaneList->add_gui_element(confirmResourceButton,false);
        editorPaneList->add_gui_element(cancelResourceButton,true);
        editorPaneList->process_self( viewedSpace, cam);

        for(  i = (int)myDefinitions.size()-1; i >=0; i--)
        {
            tempObj = myDefinitions[i];
            if( tempObj!=NULL && tempObj->removeSelfButton!=NULL && tempObj->removeSelfButton->is_clicked() )
            {
                if( display_get_prompt("Definition Deletion","Are you sure you will like to delete ["+tempObj->myKey+"]?You can not restore dictioanry from editor!",true)== DISPLAY_QUERY_YES )
                {
                    delete tempObj;
                    tempObj = NULL;
                    myDefinitions.erase( myDefinitions.begin()+i );
                }
            }
        }

        if( addDefinitionButton!=NULL && addDefinitionButton->is_clicked() )
        {
            std::string newWord = get_string_from_popup("Enter a new word","Enter a unique word","");
            newWord = trim_left_inplace(newWord);
            GPE_Report("Found "+newWord);

            if( (int)newWord.size() > 0 )
            {
                if( word_exists( newWord) ==false )
                {
                    GPE_Report("Adding definition["+newWord+"]...");
                    add_definition(newWord,"");
                    GPE_Report("Adding definition successful");
                }
                else if( GPE_Main_Logs!=NULL )
                {
                    GPE_Main_Logs->log_other_line("Unable to add duplicate key to dictionary...");
                }
            }

        }
        else if( clearDictionaryButton!=NULL && clearDictionaryButton->is_clicked() )
        {
            int dictionarySize = (int)myDefinitions.size();
            if( dictionarySize > 0 )
            {
                if( display_get_prompt("Dictionary Clear?","Are you sure you will like to clear this dictionaryYou can not restore dictionary from editor!",true)== DISPLAY_QUERY_YES )
                {
                    clear_dictionary();
                }
            }
        }
    }
}

void dictionaryResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam , bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && viewedSpace!=NULL )
    {
        if( editorPaneList!=NULL )
        {
            editorPaneList->render_self(  viewedSpace, cam, forceRedraw );
        }
    }
}

void dictionaryResource::save_resource(std::string alternatePath, int backupId )
{
    displayMessageTitle = "Saving Game Dictionary";
    displayMessageSubtitle = resourceName;
    displayMessageString = "...";
    display_user_messaage();

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
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/dictionaries/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the font file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);
            newSaveDataFile << "#     \n";

            std::string resFileLocation = "";
            std::string resFileCopySrc;
            std::string resFileCopyDest;
            int i = 0;
            definitionResource * tempObj = NULL;
            for( int i = 0; i < (int)myDefinitions.size(); i++ )
            {
                tempObj = myDefinitions[i];
                if( tempObj!=NULL)
                {
                    tempObj->myKey = string_replace_all(tempObj->keyField->get_string(),",","" );
                    newSaveDataFile << "Definition="+tempObj->myKey+",,,"+tempObj->myValue+",\n";
                }
            }

            newSaveDataFile.close();
            if( !usingAltSaveSource)
            {
                isModified = false;
            }
            displayMessageTitle = "Dictionary Successfully Saved!";
            displayMessageSubtitle = resourceName;
            displayMessageString = "...";
            display_user_messaage();
        }
        else
        {
            GPE_Main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}


int dictionaryResource::search_for_string(std::string needle)
{
    return 0;
}

int dictionaryResource::search_and_replace_string(std::string needle, std::string newStr )
{
    return 0;
}

void dictionaryResource::update_project_layers()
{

}

bool dictionaryResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Dictionary=" << resourceName << "," << get_global_rid() << ",";

            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}
bool dictionaryResource::word_exists( std::string needleWord, int allowedPos)
{
    int i = 0;
    definitionResource * tempObj = NULL;
    if( allowedPos <= 0)
    {
        for(  i = (int)myDefinitions.size()-1; i >=0; i--)
        {
            tempObj = myDefinitions[i];
            if( tempObj!=NULL  )
            {
                if( tempObj->myKey == needleWord )
                {
                    return true;
                }
                else if( tempObj->keyField!=NULL && tempObj->keyField->get_string()==needleWord )
                {
                    return true;
                }
            }
        }
    }
    else
    {
        for(  i = (int)myDefinitions.size()-1; i >=0; i--)
        {
            if( i!=allowedPos)
            {
                tempObj = myDefinitions[i];
                if( tempObj!=NULL  )
                {
                    if( tempObj->myKey == needleWord )
                    {
                        return true;
                    }
                    else if( tempObj->keyField!=NULL && tempObj->keyField->get_string()==needleWord )
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
