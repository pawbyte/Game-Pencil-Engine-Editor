/*

 MIT License

 Copyright © 2020 Samuel Venable

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#include "../../dlgmodule.h"
#include "buttons.h"
#include "config.h"
#include <sys/stat.h>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <string>

using std::string;

// const char *cocoa_widget_get_button_name(int type) {
//   return dialog_module::widget_get_button_name(type);
// }

  namespace {

    string caption;
    string current_icon;

    int const btn_array_len = 7;
    string btn_array[btn_array_len] = { "Abort", "Ignore", "OK", "Cancel", "Yes", "No", "Retry" };

    extern "C" const char *cocoa_get_bundle_id();
    extern "C" int cocoa_show_message(const char *str, bool has_cancel, const char *icon, const char *title);
    extern "C" int cocoa_show_question(const char *str, bool has_cancel, const char *icon, const char *title);
    extern "C" int cocoa_show_attempt(const char *str, const char *icon, const char *title);
    extern "C" int cocoa_show_error(const char *str, bool _abort, const char *icon, const char *title);
    extern "C" const char *cocoa_input_box(const char *str, const char *def, const char *icon, const char *title, bool numbers);
    extern "C" const char *cocoa_password_box(const char *str, const char *def, const char *icon, const char *title, bool numbers);
    extern "C" const char *cocoa_get_open_filename(const char *filter, const char *fname, const char *dir, const char *title, bool mselect);
    extern "C" const char *cocoa_get_save_filename(const char *filter, const char *fname, const char *dir, const char *title);
    extern "C" const char *cocoa_get_directory(const char *capt, const char *root);
    extern "C" int cocoa_get_color(int defcol, const char *title);
    extern "C" char *cocoa_widget_get_owner();
    extern "C" void cocoa_widget_set_owner(char *hwnd);

    string remove_trailing_zeros(double numb) {
      string strnumb = std::to_string(numb);
      while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
      strnumb.pop_back();
      return strnumb;
    }

    bool file_exists(string fname) {
      struct stat sb;
      return (stat(fname.c_str(), &sb) == 0 &&
        S_ISREG(sb.st_mode) != 0);
    }

    string filename_absolute(string fname) {
      char rpath[PATH_MAX];
      char *result = realpath(fname.c_str(), rpath);
      if (result != NULL) {
        if (file_exists(result)) return result;
      }
      return "";
    }

  } // anonymous namespace

  int show_message(char *str) {
    string str_str = str;
    return cocoa_show_message(str_str.c_str(), false, current_icon.c_str(), (caption == "") ? "Information" : caption.c_str());
  }

  int show_message_cancelable(char *str) {
    string str_str = str;
    return cocoa_show_message(str_str.c_str(), true, current_icon.c_str(), (caption == "") ? "Question" : caption.c_str());
  }

  int show_question(char *str) {
    string str_str = str;
    return cocoa_show_question(str_str.c_str(), false, current_icon.c_str(), (caption == "") ? "Question" : caption.c_str());
  }

  int show_question_cancelable(char *str) {
    string str_str = str;
    return cocoa_show_question(str_str.c_str(), true, current_icon.c_str(), (caption == "") ? "Question" : caption.c_str());
  }

  int show_attempt(char *str) {
    string str_str = str;
    return cocoa_show_attempt(str_str.c_str(), current_icon.c_str(), (caption == "") ? "Error" : caption.c_str());
  }

  int show_error(char *str, bool abort) {
    string str_str = str;
    int result = cocoa_show_error(str_str.c_str(), abort, current_icon.c_str(), (caption == "") ? "Error" : caption.c_str());
    if (result == 1) exit(0);
    return result;
  }

  char *get_string(char *str, char *def) {
    string str_str = str;
    string str_def = def;
    return (char *)cocoa_input_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption == "") ? "Input Query" : caption.c_str(), false);
  }

  char *get_password(char *str, char *def) {
    string str_str = str;
    string str_def = def;
    return (char *)cocoa_password_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption == "") ? "Input Query" : caption.c_str(), false);
  }

  double get_integer(char *str, double def) {
    double DIGITS_MIN = -999999999999999;
    double DIGITS_MAX = 999999999999999;

    if (def < DIGITS_MIN) def = DIGITS_MIN;
    if (def > DIGITS_MAX) def = DIGITS_MAX;

    string str_str = str;
    string str_def = remove_trailing_zeros(def);
    double result = strtod(cocoa_input_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption == "") ? "Input Query" : caption.c_str(), true), NULL);

    if (result < DIGITS_MIN) result = DIGITS_MIN;
    if (result > DIGITS_MAX) result = DIGITS_MAX;

    return result;
  }

  double get_passcode(char *str, double def) {
    double DIGITS_MIN = -999999999999999;
    double DIGITS_MAX = 999999999999999;

    if (def < DIGITS_MIN) def = DIGITS_MIN;
    if (def > DIGITS_MAX) def = DIGITS_MAX;

    string str_str = str;
    string str_def = remove_trailing_zeros(def);
    double result = strtod(cocoa_password_box(str_str.c_str(), str_def.c_str(), current_icon.c_str(), (caption == "") ? "Input Query" : caption.c_str(), true), NULL);

    if (result < DIGITS_MIN) result = DIGITS_MIN;
    if (result > DIGITS_MAX) result = DIGITS_MAX;

    return result;
  }

  char *get_open_filename(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), "", "", false);
    return (char *)result.c_str();
  }

  char *get_open_filename_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), str_dir.c_str(), str_title.c_str(), false);
    return (char *)result.c_str();
  }

  char *get_open_filenames(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), "", "", true);
    return (char *)result.c_str();
  }

  char *get_open_filenames_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    result = cocoa_get_open_filename(str_filter.c_str(), str_fname.c_str(), str_dir.c_str(), str_title.c_str(), true);
    return (char *)result.c_str();
  }

  char *get_save_filename(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    result = cocoa_get_save_filename(str_filter.c_str(), str_fname.c_str(), "", "");
    return (char *)result.c_str();
  }

  char *get_save_filename_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    result = cocoa_get_save_filename(str_filter.c_str(), str_fname.c_str(), str_dir.c_str(), str_title.c_str());
    return (char *)result.c_str();
  }

  char *get_directory(char *dname) {
    string str_dname = dname;  static string result;
    result = cocoa_get_directory("", str_dname.c_str());
    return (char *)result.c_str();
  }

  char *get_directory_alt(char *capt, char *root) {
    string str_dname = root; string str_title = capt; static string result;
    result = cocoa_get_directory(str_title.c_str(), str_dname.c_str());
    return (char *)result.c_str();
  }

  int get_color(int defcol) {
    return cocoa_get_color(defcol, "");
  }

  int get_color_ext(int defcol, char *title) {
    string str_title = title;
    return cocoa_get_color(defcol, str_title.c_str());
  }

  char *widget_get_caption() {
    return (char *)caption.c_str();
  }

  void widget_set_caption(char *str) {
    caption = str;
  }

  char *widget_get_owner() {
    return cocoa_widget_get_owner();
  }

  void widget_set_owner(char *hwnd) {
    cocoa_widget_set_owner(hwnd);
  }

  char *widget_get_icon() {
    return (char *)current_icon.c_str();
  }

  void widget_set_icon(char *icon) {
    current_icon = filename_absolute(icon);
  }

  char *widget_get_system() {
    return (char *)"Cocoa";
  }

  void widget_set_system(char *sys) {

  }

  void widget_set_button_name(int type, char *name) {
    string path = (getenv("TMPDIR") ? : "/tmp");
    path += "/" + string(cocoa_get_bundle_id()) + "/";
    string fname1 = path + "libdlgmod.ini";
    string fname2 = path + "libdlgmod.tmp";
    mkdir(path.c_str(), 0777);
    std::ofstream ostrm;
    ostrm.open(fname2);
    if (ostrm) {
      ostrm << "[buttons]\n" <<
        "btn_abort = " << ((type == 0) ? name : widget_get_button_name(BUTTON_ABORT)) << "\n" <<
        "btn_ignore = " << ((type == 1) ? name : widget_get_button_name(BUTTON_IGNORE)) << "\n" <<
        "btn_ok = " << ((type == 2) ? name : widget_get_button_name(BUTTON_OK)) << "\n" <<
        "btn_cancel = " << ((type == 3) ? name : widget_get_button_name(BUTTON_CANCEL)) << "\n" <<
        "btn_yes = " << ((type == 4) ? name : widget_get_button_name(BUTTON_YES)) << "\n" <<
        "btn_no = " << ((type == 5) ? name : widget_get_button_name(BUTTON_NO)) << "\n" <<
        "btn_retry = " << ((type == 6) ? name : widget_get_button_name(BUTTON_RETRY)) << "\n";
    }
    if (file_exists(fname1))
      std::remove(fname1.c_str());
    std::rename(fname2.c_str(), fname1.c_str());
  }

  char *widget_get_button_name(int type) {
    string path = (getenv("TMPDIR") ? : "/tmp");
    path += "/" + string(cocoa_get_bundle_id()) + "/";
    string fname = path + "libdlgmod.ini";
    if (file_exists(fname)) {
      config cfg(fname);
      btn_array[BUTTON_ABORT] = cfg.get_value("buttons", "btn_abort");
      btn_array[BUTTON_IGNORE] = cfg.get_value("buttons", "btn_ignore");
      btn_array[BUTTON_OK] = cfg.get_value("buttons", "btn_ok");
      btn_array[BUTTON_CANCEL] = cfg.get_value("buttons", "btn_cancel");
      btn_array[BUTTON_YES] = cfg.get_value("buttons", "btn_yes");
      btn_array[BUTTON_NO] = cfg.get_value("buttons", "btn_no");
      btn_array[BUTTON_RETRY] = cfg.get_value("buttons", "btn_retry");
    }
    return (char *)btn_array[(int)type].c_str();
  }

