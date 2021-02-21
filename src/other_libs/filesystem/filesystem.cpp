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

#include <filesystem>
#include <sstream>
#include <set>

#include <cstddef>

#include "../filesystem.h"

#if defined(_WIN32)
#include <cwchar>

#include <windows.h>
#endif

namespace fs = std::filesystem;

using std::string;
using std::vector;
using std::stringstream;
using std::size_t;

#if defined(_WIN32)
using std::wstring;
#endif

namespace misc {

  // this disallows folder from endlessly recursive copying inside itself...
  bool stdfilesystem::directory_copy_retained(string dname, string newname) {
    std::error_code ec;
    bool result = false;
    const fs::path path1 = fs::u8path(dname);
    const fs::path path2 = fs::u8path(newname);
    const fs::path path3 = fs::u8path(path2.u8string().substr(0, path1.u8string().length()));
    if (stdfilesystem::retained_string.empty() && stdfilesystem::retained_length == 0) {
      stdfilesystem::retained_length = path1.u8string().length();
      stdfilesystem::retained_string = path2.u8string().substr(stdfilesystem::retained_length);
    }
    if (directory_exists(dname)) {
      if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
        filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
        path1.u8string() != path3.u8string()) {
        fs::copy(path1, path2, fs::copy_options::recursive, ec);
        result = (ec.value() == 0);
      } else if (path1.u8string() == path3.u8string()) {
        vector<string> itemVec = split_string(directory_contents_helper(dname, "*.*", true), '\n');
        if (!directory_exists(newname)) {
          directory_create(newname);
          for (const string &item : itemVec) {
            if (directory_exists(filename_remove_slash(item)) &&
              filename_remove_slash(item).substr(stdfilesystem::retained_length) != stdfilesystem::retained_string) {
              directory_copy_retained(filename_remove_slash(item), filename_add_slash(path2.u8string()) +
              filename_name(filename_remove_slash(item)));
            } else if (file_exists(item)) {
              fs::copy(item, filename_add_slash(path2.u8string()) + filename_name(item), ec);
              // ignore and skip errored copies and copy what is left.
              // uncomment the line below to break if one copy failed.
              // if (ec.value() == 0) { result = true; } else { return false; }
            }
          }
          // check size to determine success instead of error code.
          // comment the line below out if you want break on error.
          result = (directory_exists(newname) && szSrc == directory_size(newname));
        }
      }
    }
    return result;
  }

  // replace all occurances of substring within string with string...
  string stdfilesystem::string_replace_all(string str, string sstr, string nstr) {
    size_t pos = 0;
    while ((pos = str.find(sstr, pos)) != string::npos) {
      str.replace(pos, sstr.length(), nstr);
      pos += nstr.length();
    }
    return str;
  }

  // splits string by delimiter character and convert to vector...
  vector<string> stdfilesystem::split_string(const string &str, char delimiter) {
    vector<string> vec;
    stringstream sstr(str);
    string tmp;
    while (std::getline(sstr, tmp, delimiter))
      vec.push_back(tmp);
    return vec;
  }

  // narrow 2 wide convert...
  wstring stdfilesystem::widen(string str) {
    #if defined(_WIN32)
    size_t wchar_count = str.size() + 1;
    vector<wchar_t> buf(wchar_count);
    return wstring{ buf.data(), (size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count) };
    #else
    fs::path path = fs::u8path(str);
    return path.wstring();
    #endif
  }

  // wide to narrow str conv...
  string stdfilesystem::narrow(wstring wstr) {
    #if defined(_WIN32)
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    vector<char> buf(nbytes);
    return string{ buf.data(), (size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, NULL, NULL) };
    #else
    fs::path path = wstr;
    return path.u8string();
    #endif
  }

  // gets the working directory...
  string stdfilesystem::get_working_directory() {
    std::error_code ec;
    string result = filename_add_slash(fs::current_path(ec).u8string());
    return (ec.value() == 0) ? result : "";
  }

  // set new working directory using path...
  bool stdfilesystem::set_working_directory(string dname) {
    std::error_code ec;
    const fs::path path = fs::u8path(dname);
    fs::current_path(path, ec);
    if (ec.value() == 0) {
      working_directory = get_working_directory();
    }
    return (ec.value() == 0);
  }

  // get executable abs fname...
  string stdfilesystem::get_executable_path() {
    string path;
    #if defined(_WIN32)
    wchar_t buffer[MAX_PATH];
    if (GetModuleFileNameW(NULL, buffer, MAX_PATH) != 0) {
      path = narrow(buffer);
    }
    #elif defined(__APPLE__) && defined(__MACH__)
    char buffer[PROC_PIDPATHINFO_MAXSIZE];
    if (proc_pidpath(getpid(), buffer, sizeof(buffer)) > 0) {
      path = string(buffer) + "\0";
    }
    #elif defined(__linux__) && !defined(__ANDROID__)
    char *buffer = realpath("/proc/self/exe", NULL);
    path = buffer ? : "";
    free(buffer);
    #elif defined(__FreeBSD__)
    size_t length;
    // CTL_KERN::KERN_PROC::KERN_PROC_PATHNAME(-1)
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    if (sysctl(mib, 4, NULL, &length, NULL, 0) == 0) {
      path.resize(length, '\0');
      char *buffer = path.data();
      if (sysctl(mib, 4, buffer, &length, NULL, 0) == 0) {
        path = string(buffer) + "\0";
      }
    }
    #endif
    return path;
  }

  // get temporary directory...
  string stdfilesystem::get_temp_directory() {
    std::error_code ec;
    string result = filename_add_slash(fs::temp_directory_path(ec).u8string());
    return (ec.value() == 0) ? result : "";
  }

  // get environment variable value from name...
  string stdfilesystem::environment_get_variable(string name) {
    #if defined(_WIN32)
    string value;
    wchar_t buffer[32767];
    wstring u8name = widen(name);
    if (GetEnvironmentVariableW(u8name.c_str(), buffer, 32767) != 0) {
      value = narrow(buffer);
    }
    return value;
    #elif defined(__unix__)
    char *value = getenv(name.c_str());
    return value ? value : "";
    #endif
    return "";
  }

  // sets an environment variable with the name and value...
  bool stdfilesystem::environment_set_variable(string name, string value) {
    #if defined(_WIN32)
    wstring u8name = widen(name);
    wstring u8value = widen(value);
    if (value == "") return (SetEnvironmentVariableW(u8name.c_str(), NULL) != 0);
    return (SetEnvironmentVariableW(u8name.c_str(), u8value.c_str()) != 0);
    #elif defined(__unix__)
    if (value == "") return (unsetenv(name.c_str()) == 0);
    return (setenv(name.c_str(), value.c_str(), 1) == 0);
    #endif
    return false;
  }

  // make filename canonical -- absolute...
  string stdfilesystem::filename_canonical(string fname) {
    string result = "";
    if (directory_exists(fname)) {
      result = filename_add_slash(fname, true);
    } else if (file_exists(fname)) {
      result = filename_weakly_canonical(fname);
    }
    return result;
  }

  // make filename weakly canonical -- absolute...
  string stdfilesystem::filename_weakly_canonical(string fname) {
    std::error_code ec;
    const fs::path path = fs::u8path(fname);
    string result = fs::weakly_canonical(path, ec).u8string();
    #ifdef _WIN32
    result = string_replace_all(result, "/", "\\");
    #endif
    if (ec.value() == 0 && directory_exists(result)) {
      return filename_add_slash(result);
    }
    return (ec.value() == 0) ? result : "";
  }

  // remove trailing slash and make weakly canonical when specified...
  string stdfilesystem::filename_remove_slash(string dname, bool canonical = false) {
    if (canonical) dname = enigma_user::filename_weakly_canonical(dname);
    #ifdef _WIN32
    while (dname.back() == '\\' || dname.back() == '/') dname.pop_back();
    #else
    while (dname.back() == '/') dname.pop_back();
    #endif
    return dname;
  }

  // add trailing slash and make weakly canonical when specified...
  string stdfilesystem::filename_add_slash(string dname, bool canonical = false) {
    #ifdef _WIN32
    filename_remove_slash(dname, canonical);
    dname += "\\";
    #else
    if (canonical) dname = enigma_user::filename_weakly_canonical(dname);
    if (dname.back() != '/') dname += "/";
    #endif
    return dname;
  }

  string stdfilesystem::filename_path(string fname) {
    size_t fpos = fname.find_last_of("/\\");
    return fname.substr(0,fpos + 1);
  }

  //  remove path -- keep file spec...
  string stdfilesystem::filename_name(string fname) {
    size_t fpos = fname.find_last_of("/\\");
    return fname.substr(fpos + 1);
  }

  // remove path -- keep extension...
  string stdfilesystem::filename_ext(string fname) {
    fname = filename_name(fname);
    size_t fpos = fname.find_last_of(".");
    if (fpos == string::npos)
      return "";
    return fname.substr(fpos);
  }

  // get filesize in bytes for filename...
  std::uintmax_t stdfilesystem::file_size(string fname) {
    std::error_code ec;
    if (!file_exists(fname)) return 0;
    const fs::path path = fs::u8path(fname);
    std::uintmax_t result = fs::file_size(path, ec);
    return (ec.value() == 0) ? result : 0;
  }

  // returns filename existence...
  bool stdfilesystem::file_exists(string fname) {
    std::error_code ec;
    const fs::path path = fs::u8path(fname);
    return (fs::exists(path, ec) && ec.value() == 0 &&
      (!fs::is_directory(path, ec)) && ec.value() == 0);
  }

  // deletes a certain filename...
  bool stdfilesystem::file_delete(string fname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    const fs::path path = fs::u8path(fname);
    return (fs::remove(path, ec) && ec.value() == 0);
  }

  // rename file, makes folders for dst if needed...
  bool stdfilesystem::file_rename(string oldname, string newname) {
    std::error_code ec;
    if (!file_exists(oldname)) return false;
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const fs::path path1 = fs::u8path(oldname);
    const fs::path path2 = fs::u8path(newname);
    fs::rename(path1, path2, ec);
    return (ec.value() == 0);
  }

  // copy file, new folders for dst if needed...
  bool stdfilesystem::file_copy(string fname, string newname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const fs::path path1 = fs::u8path(fname);
    const fs::path path2 = fs::u8path(newname);
    fs::copy(path1, path2, ec);
    return (ec.value() == 0);
  }

  // gets the entire directory size in bytes...
  std::uintmax_t stdfilesystem::directory_size(string dname) {
    std::uintmax_t result = 0;
    if (!directory_exists(dname)) return 0;
    const fs::path path = fs::u8path(filename_remove_slash(dname, true));
    if (fs::exists(path)) {
      fs::directory_iterator end_itr;
      for (fs::directory_iterator dir_ite(path); dir_ite != end_itr; dir_ite++) {
        fs::path file_path = fs::u8path(filename_canonical(dir_ite->path().u8string()));
        if (!fs::is_directory(dir_ite->status())) {
          result += file_size(file_path.u8string());
        } else {
          result += directory_size(file_path.u8string());
        }
      }
    }
    return result;
  }

  // checks whether directory exists...
  bool stdfilesystem::directory_exists(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, false);
    const fs::path path = fs::u8path(dname);
    return (fs::exists(path, ec) && ec.value() == 0 &&
      fs::is_directory(path, ec) && ec.value() == 0);
  }

  // creates directories recursively...
  bool stdfilesystem::directory_create(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, true);
    const fs::path path = fs::u8path(dname);
    return (fs::create_directories(path, ec) && ec.value() == 0);
  }

  // destroys directories recursively...
  bool stdfilesystem::directory_destroy(string dname) {
    std::error_code ec;
    if (!directory_exists(dname)) return false;
    dname = filename_remove_slash(dname, true);
    const fs::path path = fs::u8path(dname);
    return (fs::remove(path, ec) && ec.value() == 0);
  }

  // rename directory, new folders when needed for dst,,,
  bool stdfilesystem::directory_rename(string oldname, string newname) {
    std::error_code ec;
    if (!directory_exists(oldname)) return false;
    if (!directory_exists(newname)) directory_create(newname);
    oldname = filename_remove_slash(oldname, true);
    newname = filename_remove_slash(newname, true);
    bool result = false;
    const fs::path path1 = fs::u8path(oldname);
    const fs::path path2 = fs::u8path(newname);
    const fs::path path3 = fs::u8path(path2.u8string().substr(0, path1.u8string().length()));
    if (directory_exists(oldname)) {
      if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
        filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
        path1.u8string() != path3.u8string()) {
        fs::rename(path1, path2, ec);
        result = (ec.value() == 0);
      }
    }
    return result;
  }

  // copy directory, new folders if needed for dst...
  bool stdfilesystem::directory_copy(string dname, string newname) {
    if (!directory_exists(dname)) return false;
    dname = filename_remove_slash(dname, true);
    newname = filename_remove_slash(newname, true);
    stdfilesystem::retained_string = "";
    stdfilesystem::retained_length = 0;
    // check size to determine success instead of error code.
    // comment the line below out if you want break on error.
    stdfilesystem::szsource = directory_size(dname);
    return directory_copy_retained(dname, newname);
  }

  // get directory contents from the file filter pattern...
  vector stdfilesystem::directory_contents(string dname, string pattern) {
    std::error_code ec;
    string result = "";
    if (!directory_exists(dname)) return "";
    dname = filename_remove_slash(dname, true);
    const fs::path path = fs::u8path(dname);
    if (directory_exists(dname)) {
      fs::directory_iterator end_itr;
      for (fs::directory_iterator dir_ite(path, ec); dir_ite != end_itr; dir_ite++) {
        if (ec.value() != 0) { break; }
        fs::path file_path = fs::u8path(filename_canonical(dir_ite->path().u8string()));
        if (!fs::is_directory(dir_ite->status(ec)) && ec.value() == 0) {
          result += file_path.u8string() + "\n";
        } else if (ec.value() == 0) {
          result += filename_add_slash(file_path.u8string()) + "\n";
        }
      }
    }
    if (pattern.empty()) pattern = "*.*";
    if (result.back() == '\n') result.pop_back();
    pattern = string_replace_all(pattern, " ", "");
    pattern = string_replace_all(pattern, "*", "");
    vector<string> itemVec = split_string(result, '\n');
    vector<string> extVec = split_string(pattern, ';');
    std::set<string> filteredItems;
    for (const string &item : itemVec) {
      for (const string &ext : extVec) {
        if (ext == "." || ext == filename_ext(item) || directory_exists(item)) {
          filteredItems.insert(item);
          break;
        }
      }
    }
    vector result;
    if (filteredItems.empty()) return result;
    for (const string &filteredName : filteredItems) {
      result.push_back(filteredName);
    }
    result.pop_back();
    return result;
  }

} // namespace misc
