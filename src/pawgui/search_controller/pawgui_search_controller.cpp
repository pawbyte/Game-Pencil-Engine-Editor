/*
pawgui_search_controller.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2024 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2024 PawByte LLC.
Copyright (c) 2014-2024 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )
Copyright (c) 2024 P.Industries

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

Contributors:
- PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>
- P Industries <https://p.industries>
*/

#include "pawgui_search_controller.h"

namespace pawgui
{
    search_controller * main_search_controller = nullptr;


    search_controller::search_controller()
    {
        previousSearchMode = pawgui::search_mode::find_text;
        textSearchMode = pawgui::search_mode::find_text;
        showFindAllResults = false;
        findTextstringBox = new widget_input_text("");
        findTextstringBox->resubmissionAllowed = true;
        findTextstringBox->set_width(128);
        replaceTextstringBox = new widget_input_text("");
        replaceTextstringBox->set_width(128);
        goToLinestringBox = new widget_input_number("");
        find_button = new widget_button_label( "Find","Find string");
        goTo_button = new widget_button_label( "Go To","Go To This Line in Text Area");

        findAll_button = new widget_button_label( "Find All","Find All Copies of string in Text Area");
        replace_button = new widget_button_label( "Replace","Replace Next string in Text Area");
        replaceAll_button = new widget_button_label( "Replace All","Replace All Copries of strings in Text Area");

        findMatchCase = new widget_checkbox("Match case","Search is case-sensitive", true);
        scopeLabel = new widget_label_text("Scope:     ","Scope of your search");
        findScope = new widget_dropdown_menu( "Scope");
        findScope->add_menu_option("Open Tabs","Open Tabs",0,false);
        findScope->add_menu_option("Project Resources","Project Resources",1,true);
        findScope->add_menu_option("Workspace Resources","Workspace Resources",2);
    }

    search_controller::~search_controller()
    {
        if( findTextstringBox!=nullptr)
        {
            delete findTextstringBox;
            findTextstringBox = nullptr;
        }
        if( goToLinestringBox!=nullptr)
        {
            delete findTextstringBox;
            goToLinestringBox = nullptr;
        }
        if( replaceTextstringBox!=nullptr)
        {
            delete replaceTextstringBox;
            replaceTextstringBox = nullptr;
        }
        if( find_button!=nullptr)
        {
            delete find_button;
            find_button = nullptr;
        }
        if( scopeLabel!=nullptr)
        {
            delete scopeLabel;
            scopeLabel = nullptr;
        }
        if( findScope!=nullptr)
        {
            delete findScope;
            findScope = nullptr;
        }

        if( findAll_button!=nullptr)
        {
            delete findAll_button;
            findAll_button = nullptr;
        }
        if( goTo_button!=nullptr)
        {
            delete goTo_button;
            goTo_button = nullptr;
        }
        if( replace_button!=nullptr)
        {
            delete replace_button;
            replace_button = nullptr;
        }
        if( replaceAll_button!=nullptr)
        {
            delete replaceAll_button;
            replaceAll_button = nullptr;
        }
        if( findMatchCase!=nullptr)
        {
            delete findMatchCase;
            findMatchCase = nullptr;
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
            if( findTextstringBox->is_inuse() )
            {
                return true;
            }
            break;

        case search_mode::goto_line:
            if( goToLinestringBox->is_inuse() )
            {
                return true;
            }
            break;
        case search_mode::find_text_all:
            if( findTextstringBox->is_inuse() )
            {
                return true;
            }
            if( replaceTextstringBox->is_inuse() )
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
