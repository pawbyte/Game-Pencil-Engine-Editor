/*
paw_gui_stree.cpp
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

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

#include "paw_gui_stree.h"
#include "paw_gui_popups.h"


GPE_SpecialMenu_Branch::GPE_SpecialMenu_Branch()
{
    branchLevel = 1;
    rightClickedId = -1;
    isLocked = false;
    needsNewLine = true;
    iconTexture = NULL;
    selectedBranch = NULL;
    parentBranch = NULL;
    treeParent = NULL;

    selectedSubOption = -1;
    lastWidth = 0;
    foundX2Pos = 0;
    previouslySoughtId = -1;
    mouseInRange = false;
    isVisible = true;

    opName = name = "unnamed_branch";
    branchType = -1;
    globalId = -1;

    barXPadding = 0;
    barYPadding = 0;
    subMenuIsOpen = false;
    elementBox.h = RESOURCE_AREA_HEIGHT;
}

GPE_SpecialMenu_Branch::~GPE_SpecialMenu_Branch()
{
    remove_all_branches();
}

void GPE_SpecialMenu_Branch::add_to_list( GPE_GuiElementList * tList )
{
    if( tList!=NULL )
    {
        tList->add_indented_element( indentationLevel, this );
        if( subMenuIsOpen )
        {
            int subSize = (int)subElements.size();
            GPE_SpecialMenu_Branch * cBranch = NULL;
            for( int i = 0; i < subSize; i++ )
            {
                cBranch= subElements.at( i );
                if( cBranch!=NULL )
                {
                    cBranch->indentationLevel = indentationLevel+1;
                    cBranch->add_to_list( tList );
                }
            }
        }
    }
}

void GPE_SpecialMenu_Branch::add_to_searched_list( GPE_GuiElementList * tList, std::string needleString  )
{
    if( tList!=NULL )
    {
        bool addedSelf = false;
        if( string_contains( string_lower( name ) , needleString) )
        {
            tList->add_indented_element( indentationLevel, this );
            addedSelf = true;
        }
        int subSize = (int)subElements.size();

        if( subSize > 0  )
        {
            GPE_SpecialMenu_Branch * cBranch = NULL;
            for( int i = 0; i < subSize; i++ )
            {
                cBranch= subElements.at( i );
                if( cBranch!=NULL )
                {
                    if( addedSelf )
                    {
                        cBranch->indentationLevel = indentationLevel+1;
                    }
                    else
                    {
                        cBranch->indentationLevel;
                    }
                    cBranch->add_to_searched_list( tList, needleString );
                }
            }
        }
    }
}

void GPE_SpecialMenu_Branch::add_branch( GPE_SpecialMenu_Branch * branch, bool changeId)
{
    if( branch!=NULL )
    {
        if( treeParent == NULL )
        {
            //should only be done by the tree...
            treeParent = this;
        }
        branch->parentBranch = this;
        branch->treeParent = treeParent;
        branch->branchLevel = branchLevel+1;
        if( changeId )
        {
            branch->set_global_id( increment_count() );
        }
        subElements.push_back( branch );
    }
}


bool GPE_SpecialMenu_Branch::GPE_SpecialMenu_Branch::can_add_new_branch()
{
    //This is only changable via child classes
    return true;
}

int GPE_SpecialMenu_Branch::get_global_id()
{
    return globalId;
}

GPE_SpecialMenu_Branch * GPE_SpecialMenu_Branch::get_first_branch()
{
    if( (int)subElements.size() > 0 )
    {
        return subElements[0];
    }
    return NULL;
}

GPE_SpecialMenu_Branch * GPE_SpecialMenu_Branch::get_last_branch()
{
    if( (int)subElements.size() > 0 )
    {
        return subElements[ (int)subElements.size() - 1];
    }
    return NULL;
}
std::string GPE_SpecialMenu_Branch::get_name()
{
    return name;
}

int GPE_SpecialMenu_Branch::get_selected_id()
{
    return selectedSubOption;
}

int GPE_SpecialMenu_Branch::get_type()
{
    return branchType;
}

bool GPE_SpecialMenu_Branch::hovering_openclose( GPE_Rect * viewedSpace, GPE_Rect * cam )
{
    if( !isHovered)
    {
        return false;
    }
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );

    if( input->mouse_x <= viewedSpace->x - cam->x + elementBox.x + branchLevel *RESOURCE_AREA_HEIGHT+GENERAL_PLUSMINUX_ICON_SIZE )
    {
        return true;
    }
    return false;
}

int GPE_SpecialMenu_Branch::increment_count()
{
    if( treeParent!=NULL )
    {
        return treeParent->increment_count();
    }
    else
    {
        GPE_Report("Tree parent is NULL..." );
    }
    return -1;
}

void GPE_SpecialMenu_Branch::insert_branch( GPE_SpecialMenu_Branch * branch, int pos)
{
    if( branch!=NULL && can_add_new_branch() )
    {
        if( pos > 0 && pos < (int)subElements.size() )
        {
            subElements.insert( subElements.begin()+ pos, branch );
        }
        else
        {
            subElements.push_back( branch );
        }
    }
}

bool GPE_SpecialMenu_Branch::is_visible()
{
    return isVisible;
}

int GPE_SpecialMenu_Branch::matches(GPE_SpecialMenu_Branch * otherBranch )
{
    if( otherBranch!=NULL)
    {
        //id and type check..
        if( get_global_id()==otherBranch->get_global_id() && otherBranch->get_type() == branchType)
        {
            if( name.compare( otherBranch->get_name() )==0 )
            {
                return 1; //return true
            }
        }
        return 0; //return false
    }
    return -1; //defaults if other branch doesnt exist...
}
void GPE_SpecialMenu_Branch::open_and_view()
{
    subMenuIsOpen = true;
    if( parentBranch!=NULL && parentBranch->matches( this)==false )
    {
        parentBranch->subMenuIsOpen = true;
        parentBranch->open_and_view();
    }
}

void GPE_SpecialMenu_Branch::process_self(GPE_Rect * viewedSpace, GPE_Rect *cam  )
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );
    GPE_GeneralGuiElement::process_self( viewedSpace, cam );
    elementBox.w = cam->w;
    elementBox.h = RESOURCE_AREA_HEIGHT;
    if( isHovered )
    {
        if( isPressed )
        {

        }
        else if( isClicked )
        {
            if( hovering_openclose( viewedSpace, cam) )
            {
                subMenuIsOpen = !subMenuIsOpen;
            }
            if( treeParent!=NULL )
            {
                treeParent->selectedSubOption = globalId;
                treeParent->selectedBranch = this;
            }
        }
        else if(  isRightClicked )
        {
            if( hovering_openclose( viewedSpace, cam) )
            {
                subMenuIsOpen = !subMenuIsOpen;
            }
            if( treeParent!=NULL )
            {
                treeParent->rightClickedId = globalId;
                treeParent->selectedSubOption = globalId;
                treeParent->selectedBranch = this;
            }
        }
    }
    else
    {
        isClicked = isPressed = isRightClicked = false;
    }
}

bool GPE_SpecialMenu_Branch::remove_branch( int branchId )
{
    return true;
}

void GPE_SpecialMenu_Branch::remove_branch( std::string branchName )
{
    if( (int)branchName.size() > 0 )
    {

    }
}

void GPE_SpecialMenu_Branch::remove_branch( GPE_SpecialMenu_Branch * branch )
{
    if( branch!=NULL )
    {
        if( treeParent!=NULL && treeParent->selectedBranch!=NULL )
        {
            if( branch->matches( treeParent->selectedBranch ) )
            {
                treeParent->selectedBranch = NULL;
                treeParent->selectedSubOption = -1;

            }
        }
        GPE_SpecialMenu_Branch * foundBranch = NULL;
        for( int i = (int)subElements.size()-1; i >=0; i-- )
        {
            foundBranch = subElements.at(  i );
            if( foundBranch!=NULL &&  branch->matches( branch ) )
            {
                //GPE_Report( "Deleting :" + foundBranch->get_name() );
                //foundBranch->remove_all_branches();
                delete foundBranch;
                foundBranch = NULL;
                break;
            }
        }
    }
}

void GPE_SpecialMenu_Branch::remove_all_branches()
{
    GPE_SpecialMenu_Branch * foundBranch = NULL;
    for( int i = (int)subElements.size()-1; i >=0; i-- )
    {
        foundBranch = subElements.at(  i );
        if( foundBranch!=NULL )
        {
            //GPE_Report( "Deleting :" + foundBranch->get_name() );
            //foundBranch->remove_all_branches();
            if( treeParent!=NULL && treeParent->selectedBranch!=NULL )
            {
                if( foundBranch->matches( treeParent->selectedBranch ) )
                {
                    treeParent->selectedBranch = NULL;
                    treeParent->selectedSubOption = -1;

                }
            }
            delete foundBranch;
            foundBranch = NULL;
        }
    }
    subElements.clear();
}

void GPE_SpecialMenu_Branch::render_branch( )
{
    int subElCount  = (int)subElements.size();
    GPE_SpecialMenu_Branch * cBranch = NULL;
    for(  int myItr = 0; myItr < subElCount; myItr++ )
    {
        cBranch = subElements[myItr];
        if( cBranch!=NULL)
        {
            cBranch->render_branch();
        }
    }
}

void GPE_SpecialMenu_Branch::render_self( GPE_Rect * viewedSpace, GPE_Rect *cam, bool forceRedraw   )
{
    bool selfIsInView = false;
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if( cam!=NULL && viewedSpace!=NULL && forceRedraw )
    {
        int rendY = elementBox.x;
        if( rendY>=cam->y && rendY <=cam->y+cam->h )
        {
            selfIsInView = true;
        }
        else if( rendY+RESOURCE_AREA_HEIGHT >=cam->y && rendY+RESOURCE_AREA_HEIGHT <=cam->y+cam->h  )
        {
            selfIsInView = true;
        }

        if( (selfIsInView || forceRedraw) )
        {
            if(treeParent!=NULL && treeParent->get_selected_id()==globalId && globalId > 0  )
            {
                gcanvas->render_rectangle( 0,elementBox.y-cam->y,
                                       cam->w-cam->x,elementBox.y+16-cam->y,GPE_MAIN_THEME->Button_Box_Highlighted_Color,false,255 );
            }
        }
        if( (int)subElements.size() > 0 )
        {
            if( subMenuIsOpen )
            {
                if( (selfIsInView || forceRedraw) )
                {
                    gfs->render_text( elementBox.x-cam->x, elementBox.y-cam->y,"-",GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STREE_BRANCH,FA_LEFT,FA_TOP);
                }
                GPE_SpecialMenu_Branch * foundBranch = NULL;

                /*
                for( int i=0; i< (int)subElements.size();i++ )
                {
                    foundBranch = subElements[i];
                    if(foundBranch!=NULL)
                    {
                        foundBranch->previouslySoughtId = selectedSubOption;
                        foundBranch->render_self( viewedSpace,cam, forceRedraw);

                    }
                }*/
                gfs->render_text( elementBox.x-cam->x,
                                 elementBox.y-cam->y,
                                 "-",GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STREE_BRANCH,FA_LEFT,FA_TOP);

            }
            else if( (selfIsInView || forceRedraw ) )
            {
                gfs->render_text( elementBox.x-cam->x,
                                 elementBox.y-cam->y,
                                 "+",GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STREE_BRANCH,FA_LEFT,FA_TOP);

            }
        }

        if( selfIsInView || forceRedraw)
        {
            if( iconTexture!=NULL )
            {
                iconTexture->render_tex_resized(elementBox.x+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,elementBox.y-cam->y, RESOURCE_AREA_HEIGHT,RESOURCE_AREA_HEIGHT,NULL,  GPE_MAIN_THEME->Main_Box_Font_Color );
            }
            gfs->render_text( elementBox.x+RESOURCE_AREA_HEIGHT+GENERAL_PLUSMINUX_ICON_SIZE-cam->x,elementBox.y-cam->y+RESOURCE_AREA_HEIGHT/2, name,GPE_MAIN_THEME->Main_Box_Font_Color,FONT_STREE_BRANCH,FA_LEFT,FA_CENTER);
        }
    }
}

bool GPE_SpecialMenu_Branch::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn  )
{

}

void GPE_SpecialMenu_Branch::set_global_id( int newId )
{
    globalId = newId;
}

void GPE_SpecialMenu_Branch::set_name( std::string newName )
{
    opName = name = newName;
}


GPE_SpecialMenu_Tree::GPE_SpecialMenu_Tree( std::string menuName,  int optionSuperType, int optionId  )
{
    useMetaTop = true;
    addButton = new GPE_ToolIconButton(APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/plus.png","Add Branch" );
    searchField = new GPE_TextInputBasic("","Search..." );
    needsNewLine = true;
    treeList = new GPE_GuiElementList();
    opName = name = menuName;
    branchType = optionSuperType;
    globalId = optionId;

    hasScrollControl = false;
    hasArrowkeyControl = false;

    opTexture = NULL;
    maxSuperFolderCount = 32;
    seekedX2Pos = 0;
    barTitleWidth = 0;
    barTitleHeight = 32;
    globalBranchCounter = 0;

    justResized = false;
    showYScroll = false;
    beingResized = false;
    treeResized = false;
    branchMoved = false;

    elementBox.x = 0;
    elementBox.y = 48;
    elementBox.w = 192;
    elementBox.h = 128;

    barXPadding = 8;
    barYPadding = 8;

    treeParent = this;
}

GPE_SpecialMenu_Tree::~GPE_SpecialMenu_Tree( )
{
    if( treeList!=NULL)
    {
        delete treeList;
        treeList = NULL;
    }

    remove_all_branches();
}
bool GPE_SpecialMenu_Tree::add_button_clicked()
{
    if( useMetaTop )
    {
        return addButton->is_clicked();
    }
    return false;
}

bool GPE_SpecialMenu_Tree::can_add_new_branch()
{
    if( (int)subElements.size() < maxSuperFolderCount || maxSuperFolderCount < 0 )
    {
        return true;
    }
    return false;
}

int GPE_SpecialMenu_Tree::increment_count()
{
    globalBranchCounter+=1;
    return globalBranchCounter;
}

void GPE_SpecialMenu_Tree::process_self( GPE_Rect * viewedSpace, GPE_Rect * cam  )
{
    rightClickedId = -1;
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera(cam);
    GPE_GeneralGuiElement::process_self( viewedSpace, cam );
    indentationLevel = 0;
    //if( isVisible )
    {
        int sOpNumber = -1;
        justResized = false;

        bool mouseInRange = false;

        if( elementBox.w > SCREEN_WIDTH/2)
        {
            elementBox.w = SCREEN_WIDTH/2;
            treeResized = justResized = true;
        }

        if( elementBox.w < 32 && isVisible)
        {
            elementBox.w = 32;
            treeResized = justResized = true;
        }
        if( isHovered )
        {
            mouseInRange = true;
        }
        if( input->check_mouse_pressed(0) || input->check_mouse_pressed(1) || input->check_mouse_pressed(2) )
        {
            if( mouseInRange)
            {
                hasScrollControl = true;
                hasArrowkeyControl = true;
            }
            else
            {
                hasScrollControl = false;
                hasArrowkeyControl = false;
            }
        }

        //if( mouseInRange || menuResized || resourcebarMoved)
        {

            GPE_SpecialMenu_Branch * cBranch = NULL;

            if( useMetaTop )
            {
                addButton->set_coords( elementBox.x, elementBox.y );
                searchField->set_coords( addButton->get_x2pos()+GENERAL_GPE_PADDING, elementBox.y );
                searchField->set_width( elementBox.w - searchField->get_xpos()-GENERAL_GPE_PADDING );
                addButton->process_self( viewedSpace, cam  );
                searchField->process_self( viewedSpace, cam  );
            }
            else
            {
                addButton->set_clicked( false );
                searchField->set_string( "");
            }

            treeList->clear_list();
            treeList->set_coords( elementBox.x, elementBox.y+32 );
            treeList->set_width( elementBox.w);
            treeList->set_height( elementBox.h-32 );
            treeList->barXPadding = 0;
            treeList->barYPadding = 0;
            treeList->barXMargin = 0;
            treeList->barYMargin = 0;

            std::string searchedString = searchField->get_string();

            int i = 0;
            int subElementsSize = (int)subElements.size();
            if( (int)searchedString.size() > 0 && useMetaTop)
            {
                searchedString = string_lower( searchedString );
                for( i=0; i< subElementsSize; i++)
                {
                    cBranch = subElements[i];
                    if(cBranch!=NULL)
                    {
                        cBranch->indentationLevel = indentationLevel;
                        cBranch->add_to_searched_list( treeList, searchedString );
                    }
                }
            }
            else
            {
                for( i=0; i< subElementsSize; i++)
                {
                    cBranch = subElements[i];
                    if(cBranch!=NULL)
                    {
                        cBranch->indentationLevel = indentationLevel;
                        cBranch->add_to_list( treeList );
                    }
                }
            }
            treeList->process_self( viewedSpace, cam );
        }
    }
}

void GPE_SpecialMenu_Tree::render_branch()
{
    GPE_SpecialMenu_Branch::render_branch( );
}

void GPE_SpecialMenu_Tree::render_self( GPE_Rect * viewedSpace,GPE_Rect * cam, bool forceRedraw )
{
    viewedSpace = GPE_find_camera( viewedSpace );
    cam = GPE_find_camera( cam );
    /*if( resourcebarMoved render_self)
    {
        forceRedraw = true;
    }*/
    if( isVisible && forceRedraw)
    {

        if( GPE_MAIN_THEME->themeBgTexture == NULL)
        {
            //gcanvas->render_rectangle( 0,0,elementBox.w,elementBox.h,GPE_MAIN_THEME->Program_Color,false);
        }


        if( useMetaTop )
        {
            addButton->render_self( viewedSpace, cam  );
            searchField->render_self( viewedSpace, cam  );
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x - cam->x, elementBox.y - cam->y,
                                   elementBox.x + elementBox.w, elementBox.y + barTitleHeight - cam->x,GPE_MAIN_THEME->PopUp_Box_Color,false);
            gfs->render_text( elementBox.x +GENERAL_GPE_PADDING - cam->x, elementBox.y - cam->y, name, GPE_MAIN_THEME->PopUp_Box_Font_Color,FONT_STREE_BRANCH,FA_LEFT,FA_TOP);
        }
        if( hasScrollControl)
        {
            gcanvas->render_rectangle( elementBox.x -cam->x,elementBox.y -cam->y,
                                   elementBox.x+elementBox.w -cam->x, elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Button_Box_Highlighted_Color,true);
        }
        else
        {
            gcanvas->render_rectangle( elementBox.x -cam->x,elementBox.y -cam->y,
                                   elementBox.x+elementBox.w -cam->x, elementBox.y+elementBox.h-cam->y,GPE_MAIN_THEME->Text_Box_Outline_Color,true);
        }

        if( treeList!=NULL )
        {
            ///cBranch->selectedSubOption = selectedSubOption;
            treeList->render_self(viewedSpace, cam, forceRedraw);
        }
    }
}

void GPE_SpecialMenu_Tree::toggle_self()
{

}

bool GPE_SpecialMenu_Tree::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn  )
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        GPE_SpecialMenu_Branch * cBranch = NULL;
        for( int i = 0; i < (int)subElements.size(); i++)
        {
            cBranch = subElements[i];
            if( cBranch!=NULL )
            {
                cBranch->save_branch_data( fileTarget, 0 );
            }
        }
    }
    return false;
}

void GPE_SpecialMenu_Tree::select_branch(GPE_SpecialMenu_Branch * branch )
{
    if( branch!=NULL)
    {
        selectedBranch = branch;
        selectedSubOption = branch->get_global_id();
    }
    else
    {
        selectedBranch = NULL;
        selectedSubOption = -1;
    }
}
