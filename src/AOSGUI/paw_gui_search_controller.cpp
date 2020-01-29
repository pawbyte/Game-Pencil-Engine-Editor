/*
paw_gui_search_controller.cpp
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

#include "paw_gui_search_controller.h"

PAW_GUI_SEARCH_CONTROLLER * MAIN_SEARCH_CONTROLLER = NULL;


PAW_GUI_SEARCH_CONTROLLER::PAW_GUI_SEARCH_CONTROLLER()
{
    previousSearchMode = SEARCH_MODE_FINDALL;
    textSearchMode = 0;
    showFindAllResults = false;
    findTextStringBox = new GPE_TextInputBasic("");
    findTextStringBox->resubmissionAllowed = true;
    findTextStringBox->set_width(128);
    replaceTextStringBox = new GPE_TextInputBasic("");
    replaceTextStringBox->set_width(128);
    goToLineStringBox = new GPE_TextInputNumber("");
    findButton = new GPE_ToolLabelButton( "Find","Find String");
    goToButton = new GPE_ToolLabelButton( "Go To","Go To This Line in Text Area");

    findAllButton = new GPE_ToolLabelButton( "Find All","Find All Copies of String in Text Area");
    replaceButton = new GPE_ToolLabelButton( "Replace","Replace Next String in Text Area");
    replaceAllButton = new GPE_ToolLabelButton( "Replace All","Replace All Copries of Strings in Text Area");

    findMatchCase = new GPE_CheckBoxBasic("Match case","Search is case-sensitive", true);
    scopeLabel = new GPE_Label_Text("Scope:     ","Scope of your search");
    findScope = new GPE_DropDown_Menu( "Scope");
    findScope->add_menu_option("Open Tabs","Open Tabs",0,false);
    findScope->add_menu_option("Project Resources","Project Resources",1,true);
    findScope->add_menu_option("Workspace Resources","Workspace Resources",2);
}

PAW_GUI_SEARCH_CONTROLLER::~PAW_GUI_SEARCH_CONTROLLER()
{
    if( findTextStringBox!=NULL)
    {
        delete findTextStringBox;
        findTextStringBox = NULL;
    }
    if( goToLineStringBox!=NULL)
    {
        delete findTextStringBox;
        goToLineStringBox = NULL;
    }
    if( replaceTextStringBox!=NULL)
    {
        delete replaceTextStringBox;
        replaceTextStringBox = NULL;
    }
    if( findButton!=NULL)
    {
        delete findButton;
        findButton = NULL;
    }
    if( scopeLabel!=NULL)
    {
        delete scopeLabel;
        scopeLabel = NULL;
    }
    if( findScope!=NULL)
    {
        delete findScope;
        findScope = NULL;
    }

    if( findAllButton!=NULL)
    {
        delete findAllButton;
        findAllButton = NULL;
    }
    if( goToButton!=NULL)
    {
        delete goToButton;
        goToButton = NULL;
    }
    if( replaceButton!=NULL)
    {
        delete replaceButton;
        replaceButton = NULL;
    }
    if( replaceAllButton!=NULL)
    {
        delete replaceAllButton;
        replaceAllButton = NULL;
    }
    if( findMatchCase!=NULL)
    {
        delete findMatchCase;
        findMatchCase = NULL;
    }
}

void PAW_GUI_SEARCH_CONTROLLER::close_finder()
{
    if( textSearchMode >= SEARCH_MODE_FIND)
    {
        previousSearchMode = textSearchMode;
    }
    textSearchMode = -1;
    showFindAllResults = false;
}
bool PAW_GUI_SEARCH_CONTROLLER::using_search()
{
    switch( textSearchMode)
    {
    case SEARCH_MODE_FIND:
        if( findTextStringBox->is_inuse() )
        {
            return true;
        }
        break;

    case SEARCH_MODE_GOTO:
        if( goToLineStringBox->is_inuse() )
        {
            return true;
        }
        break;
    case SEARCH_MODE_FINDALL:
        if( findTextStringBox->is_inuse() )
        {
            return true;
        }
        if( replaceTextStringBox->is_inuse() )
        {
            return true;
        }
        break;
    default:
        return false;
        break;
    }
    return false;
}

