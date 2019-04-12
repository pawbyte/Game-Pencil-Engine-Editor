/*
paw_gui_popups.h
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

#ifndef PAW_GUI_POPUPS
#define PAW_GUI_POPUPS

class GPE_LoaderDiplay
{
private:
    double item1Value;
    double item1Max;
    double item2Value;
    double item2Max;
public:
    std::string displayMessageTitle;
    std::string displayMessageSubtitle;
    std::string displayMessageString;
    GPE_LoaderDiplay();
    ~GPE_LoaderDiplay();
    void increment_bar1();
    void increment_bar2();
    void increment_and_update( std::string subTitle, std::string message, int barNumber = 0, bool renderUpdate = true );

    void set_bar1( double maxValue);
    void set_bar2( double maxValue);
    void render_loader();
    void reset();
    void update_messages(std::string title, std::string subTitle, std::string message, bool renderUpdate = true );
    void update_submessages(  std::string subTitle, std::string message, bool renderUpdate = true  );
};

extern GPE_LoaderDiplay * GPE_LOADER;

int GPE_GetMessageBoxResult(const std::string title, const std::string prompt, int flagType);

void display_user_alert(std::string messageTitle, std::string messageContent, int messageType = 0,GPE_Rect * camera = NULL );
int GPE_Display_Basic_Prompt(std::string messageTitle, std::string messageContent, bool showCancelButton = false );
std::string get_string_from_popup(std::string messageTitle, std::string messageContent, std::string startName = "");

#endif

