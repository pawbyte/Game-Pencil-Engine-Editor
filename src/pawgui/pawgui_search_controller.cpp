/*
pawgui_search_controller.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_search_controller.h"

namespace pawgui
{
    search_controller * main_search_controller = NULL;


    search_controller::search_controller()
    {
        previousSearchMode = pawgui::search_mode::find_text;
        textSearchMode = pawgui::search_mode::find_text;
        showFindAllResults = false;
        findTextStringBox = new widget_input_text("");
        findTextStringBox->resubmissionAllowed = true;
        findTextStringBox->set_width(128);
        replaceTextStringBox = new widget_input_text("");
        replaceTextStringBox->set_width(128);
        goToLineStringBox = new widget_input_number("");
        findButton = new widget_button_label( "Find","Find String");
        goToButton = new widget_button_label( "Go To","Go To This Line in Text Area");

        findAllButton = new widget_button_label( "Find All","Find All Copies of String in Text Area");
        replaceButton = new widget_button_label( "Replace","Replace Next String in Text Area");
        replaceAllButton = new widget_button_label( "Replace All","Replace All Copries of Strings in Text Area");

        findMatchCase = new widget_checkbox("Match case","Search is case-sensitive", true);
        scopeLabel = new widget_label_text("Scope:     ","Scope of your search");
        findScope = new widget_dropdown_menu( "Scope");
        findScope->add_menu_option("Open Tabs","Open Tabs",0,false);
        findScope->add_menu_option("Project Resources","Project Resources",1,true);
        findScope->add_menu_option("Workspace Resources","Workspace Resources",2);
    }

    search_controller::~search_controller()
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

    void search_controller::close_finder()
    {
        if( textSearchMode >= pawgui::search_mode::mode_none)
        {
            previousSearchMode = textSearchMode;
        }
        textSearchMode = pawgui::search_mode::mode_none;
        showFindAllResults = false;
    }
    bool search_controller::using_search()
    {
        switch( textSearchMode)
        {
        case search_mode::find_text:
            if( findTextStringBox->is_inuse() )
            {
                return true;
            }
            break;

        case search_mode::goto_line:
            if( goToLineStringBox->is_inuse() )
            {
                return true;
            }
            break;
        case search_mode::find_text_all:
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
}
