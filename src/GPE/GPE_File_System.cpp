/*
gpe_file_system.cpp
This file is part of:GAME PENCIL ENGINE
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

#include "GPE_File_System.h"
#include "GPE_Globals.h"

std::string generate_filters(const std::vector<GPE_FileFilter *> filters)
{
    std::string result ="";
    GPE_FileFilter * tempFilter = NULL;
    for(int i = 0; i < (int)filters.size(); i++)
    {
        tempFilter = filters[i];
        if( tempFilter!=NULL)
        {
            result += tempFilter->desc;
            result += '\0';
            result += tempFilter->exts;
            result += '\0';
        }
    }
    return result + '\0';
}

std::string parse_file_types(std::string allowedFileTypes,std::vector <std::string> &fileTypeVector)
{
    fileTypeVector.clear();
    std::string returnFileFilterString = "All types(*.*)\0*.*\0";
    std::vector<GPE_FileFilter *> filters;
    GPE_FileFilter * tempNewFilter = NULL;
    if( (int)allowedFileTypes.size() > 0)
    {
        if( allowedFileTypes=="All types(*.*)" ||  allowedFileTypes=="All Files" || allowedFileTypes=="None" ||  allowedFileTypes=="All Files*")
        {
            fileTypeVector.clear();
        }
        else if( allowedFileTypes=="Image Only" || allowedFileTypes=="Image" || allowedFileTypes=="Images" || allowedFileTypes=="Photos")
        {
            tempNewFilter = new GPE_FileFilter("Images (*.bmp | *.png | *.jpg)", "*.bmp;*.png;*.jpg");
            filters.push_back(tempNewFilter );
            fileTypeVector.push_back("bmp");
            fileTypeVector.push_back("BMP");
            fileTypeVector.push_back("gif");
            fileTypeVector.push_back("GIF");
            fileTypeVector.push_back("jpg");
            fileTypeVector.push_back("JPG");
            fileTypeVector.push_back("jpeg");
            fileTypeVector.push_back("JPEG");
            fileTypeVector.push_back("png");
            fileTypeVector.push_back("PNG");
        }
        else if( allowedFileTypes=="Audio Only" || allowedFileTypes=="Audio")
        {
            fileTypeVector.push_back("aac");
            fileTypeVector.push_back("AAC");
            fileTypeVector.push_back("mp3");
            fileTypeVector.push_back("MP3");
            fileTypeVector.push_back("ogg");
            fileTypeVector.push_back("OGG");
            fileTypeVector.push_back("wav");
            fileTypeVector.push_back("WAV");

            tempNewFilter = new GPE_FileFilter("Audio (*.aac | *.mp3 | *.ogg| *.wav)", "*.aac;*.mp3;*.ogg;*.wav");
            filters.push_back(tempNewFilter );
        }
        else if( allowedFileTypes=="Video Only" || allowedFileTypes=="Video" || allowedFileTypes=="Videos")
        {
            tempNewFilter = new GPE_FileFilter("Video (*.mp4 | *.ogg| *.webm)", "*.mp4;*.ogg;*.webm");
            filters.push_back(tempNewFilter );
            fileTypeVector.push_back("mp4");
            fileTypeVector.push_back("MP4");
            fileTypeVector.push_back("ogg");
            fileTypeVector.push_back("OGG");
            fileTypeVector.push_back("webm");
            fileTypeVector.push_back("WEBM");
        }
        else if( allowedFileTypes=="Fonts Only" || allowedFileTypes=="Font" || allowedFileTypes=="Fonts")
        {
            tempNewFilter = new GPE_FileFilter("Fonts (*.eot | *.svg| *.ttf)", "*.eot;*.svg;*.ttf");
            filters.push_back(tempNewFilter );
            fileTypeVector.push_back("eot");
            fileTypeVector.push_back("EOT");
            fileTypeVector.push_back("svg");
            fileTypeVector.push_back("SVG");
            fileTypeVector.push_back("ttf");
            fileTypeVector.push_back("TTF");
        }
        else if( allowedFileTypes=="Game Pencil Projects" || allowedFileTypes=="GPE Project")
        {
            tempNewFilter = new GPE_FileFilter("Game Pencil Projects (*.gppf)", "*.gppf");
            fileTypeVector.push_back("gppf");
        }
        else if( allowedFileTypes.size() > 2)
        {
            allowedFileTypes = string_replace_all(allowedFileTypes," ","");
            int asterikPosition = allowedFileTypes.find("*");
            int maxIterations = 10;
            int otherTypesIterations = 0;
            if( asterikPosition!=(int)std::string::npos)
            {
                while( asterikPosition!=(int)std::string::npos && otherTypesIterations < maxIterations )
                {
                    fileTypeVector.push_back( split_first_string(allowedFileTypes,"*") );
                    otherTypesIterations++;
                }
            }
            else
            {
                fileTypeVector.push_back(allowedFileTypes);
            }
        }
    }
    tempNewFilter = new GPE_FileFilter("All Files (*.*)", "*.*");
    filters.push_back(tempNewFilter );
    returnFileFilterString = generate_filters(filters);
    for( int iDelete = (int)filters.size()-1; iDelete>=0; iDelete--)
    {
        tempNewFilter = filters.at(iDelete);
        if( tempNewFilter!=NULL)
        {
            delete tempNewFilter;
            tempNewFilter = NULL;
        }
    }
    filters.clear();
    return returnFileFilterString;
}

bool file_passes_filter(std::string fileTypeIn, std::vector <std::string> &fileTypeVector)
{
    if( (int)fileTypeVector.size() > 0 )
    {
        if( (int)fileTypeIn.size() > 0)
        {
            std::string tempStr = "";
            for( int i = (int)fileTypeVector.size()-1; i >=0; i--)
            {
                tempStr = fileTypeVector[i];
                if(fileTypeIn==tempStr)
                {
                    return true;
                }
            }
        }
    }
    else
    {
        return true;
    }
    return false;
}

GPE_FileFilter::GPE_FileFilter(std::string fDesc, std::string fExts )
{
    desc = fDesc;
    exts = fExts;
}

GPE_FileFilter::~GPE_FileFilter()
{

}

GPE_File::GPE_File(std::string fName,bool fileIsDirectory)
{
    isDirectory = fileIsDirectory;
    if( (int)fName.size() > 0)
    {
        fileName = fName;
        if( isDirectory)
        {
            fileType = "";
        }
        else
        {
            fileType = get_file_ext(fName);
        }
    }
    else
    {
        fileName = "";
        fileType = "";
    }
    fileThumbnail = NULL;
    fileSizeInfo = 0;
    dataCreatedInfo = "";
    dataModifiedInfo = "";
}

GPE_File::~GPE_File()
{
    if( fileThumbnail!=NULL)
    {
        delete fileThumbnail;
        fileThumbnail = NULL;
    }
}

bool GPE_File::is_directory()
{
    return isDirectory;
}

std::string GPE_File::get_name()
{
    return fileName;
}

std::string GPE_File::get_type()
{
    return fileType;
}

GPE_FileDirectory::GPE_FileDirectory()
{
    fdPosition= 0;
    fileCount= 0;
    subdirectoryCount= 0;
}

GPE_FileDirectory::~GPE_FileDirectory()
{
    close_directory();
}


bool GPE_FileDirectory::open_directory(std::string directoryIn)
{
    return open_directory_sorted(directoryIn);
}

void GPE_FileDirectory::close_directory()
{
    GPE_File * tFile = NULL;
    for( int i = (int)myFiles.size()-1; i>=0; i--)
    {
        tFile = myFiles[i];
        if( tFile!=NULL)
        {
            delete tFile;
            tFile = NULL;
        }
    }
    myFiles.clear();
    fdPosition= 0;
    fileCount= 0;
    subdirectoryCount= 0;
}

void GPE_FileDirectory::filter_directory(bool onlyFolders, std::vector <std::string> &fTypesToUse)
{
    GPE_File * tempFile = NULL;
    fdPosition = 0;
    fileCount = 0;
    subdirectoryCount = 0;
    for( int i = (int)myFiles.size() - 1; i >=0; i--)
    {
        tempFile = myFiles[i];
        if( tempFile!=NULL)
        {
            if( onlyFolders)
            {
                if( tempFile->is_directory() )
                {
                    subdirectoryCount++;
                }
            }
            else if( tempFile->is_directory() )
            {
                subdirectoryCount++;
            }
            else if( file_passes_filter (tempFile->get_type(), fTypesToUse) )
            {
                fileCount++;
            }
            else
            {
                delete tempFile;
                tempFile = NULL;
                myFiles.erase(myFiles.begin()+i);
            }
        }
        else
        {
            myFiles.erase(myFiles.begin()+i);
        }
    }
}

bool GPE_FileDirectory::open_directory_sorted(std::string directoryIn)
{
    if( path_exists(directoryIn) )
    {
        close_directory();
        std::string foundErrorMessage;
        DIR *dir;
        struct dirent *ent;
        dir = opendir( directoryIn.c_str() );
        if( dir==NULL)
        {
            foundErrorMessage = strerror(errno);

            GPE_Report("Unable to open directory["+directoryIn+"]");
            GPE_Report("Error: "+foundErrorMessage);

            //tinydir_close(&dir);
            return false;
        }
        else
        {
            GPE_File * newFile = NULL;
            GPE_File * tFile = NULL;
            int i = 0;
            int j = 0;
            bool foundNewPosition = false;
            bool newFileIsDirectory = false;
            std::string newFileName = "";
            while ( (ent = readdir(dir)) != NULL )
            {
                std::string entry( ent->d_name );
                //std::string lcEntry( std::strToLower(entry) );
                /*
                if( tinydir_readfile(&dir, &file)==-1)
                {
                    foundErrorMessage = strerror(errno) ;
                    GPE_Report("Unable to open directory's file #"+int_to_string(i)+"]");
                    GPE_Report("Error: "+foundErrorMessage);
                    tinydir_close(&dir);
                    return false;
                }
                else*/
                {
                    newFileName = ent->d_name;
                    if(newFileName!=".." && newFileName!=".")
                    {
                        newFileIsDirectory = path_exists(directoryIn+"/"+ent->d_name);
                        //if( ( MAIN_EDITOR_SETTINGS!=NULL && MAIN_EDITOR_SETTINGS->showHiddenFilesInBrowser->is_clicked() ) || GPE_FOUND_OS!=GPE_IDE_LINUX || (GPE_FOUND_OS==GPE_IDE_LINUX && newFileName.find_first_of(".",0)!=0) )
                        {
                            newFile = new GPE_File(newFileName, newFileIsDirectory);
                            if( newFile!=NULL)
                            {
                                newFile->fileLocationDirectory = directoryIn;
                                foundNewPosition = false;

                                for( j = (int)myFiles.size()-1; j >=0 && foundNewPosition==false; j--)
                                {
                                    tFile = myFiles[j];
                                    if( tFile!=NULL)
                                    {
                                        if( tFile->get_name() >= newFileName )
                                        {
                                            myFiles.insert(myFiles.begin()+j,newFile);
                                            foundNewPosition = true;
                                        }
                                    }
                                }
                                if( !foundNewPosition)
                                {
                                    myFiles.push_back(newFile);
                                }
                                if( newFileIsDirectory )
                                {
                                    subdirectoryCount++;
                                }
                                else
                                {
                                    fileCount++;
                                }
                            }
                        }
                    }
                    //tinydir_next(&dir);
                }
                i++;
            }
            //tinydir_close(&dir);

        }

        return true;
    }
    return false;
}

GPE_File * GPE_FileDirectory::get_file(int position)
{
    if( position >=0 && position < (int)myFiles.size() )
    {
        return myFiles.at(position);
    }
    return NULL;
}

int GPE_FileDirectory::get_count()
{
    return (int)myFiles.size();
}

int GPE_FileDirectory::get_subdirectory_count()
{
    return subdirectoryCount;
}

int GPE_FileDirectory::get_file_count()
{
    return fileCount;
}

GPE_File * GPE_FileDirectory::get_next_file()
{
    if( has_next_file() )
    {
        fdPosition++;
        return get_file(fdPosition);
    }
    return NULL;
}

bool GPE_FileDirectory::has_next_file()
{
    if( fdPosition>=0 && fdPosition < (int)myFiles.size()-1 )
    {
        return true;
    }
    return false;
}

bool GPE_FileDirectory::set_position(int position)
{
    if( position >=0 && position < (int)myFiles.size() )
    {
        fdPosition = position;
    }
    return false;
}



void GPE_OpenProgram(std::string programLocation, std::string programInfo, bool openProgrmaInfoOnFail )
{
    GPE_Report("Attempting to open ["+programLocation+"] with ["+programInfo+"] data...");
    if( file_exists(programLocation) || path_exists(programLocation) )
    {
#ifdef _WIN32
        // win implementation
        //programLocation = "start "+urlIn;
        programInfo = string_replace_all(programInfo,"/","\\");
        ShellExecute(NULL,NULL,programLocation.c_str(),programInfo.c_str(),NULL,SW_SHOW );
#elif __APPLE__
        // apple implementation
        programLocation = "open "+programLocation;

        system(programLocation.c_str() );
#else
        // Linux and others implementation
        programLocation = "xdg-open \""+programLocation+"\"";
        system(programLocation.c_str() );
#endif
    }
    else if( openProgrmaInfoOnFail )
    {
        GPE_OpenURL(programInfo);
    }
}

void GPE_OpenURL(std::string urlIn)
{
    //urlIn+="&";
    /**/
#ifdef _WIN32
    // win implementation
    ShellExecute(NULL, "open", urlIn.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif __APPLE__
    // apple implementation
    urlIn = "open "+urlIn;
    system(urlIn.c_str() );
#else
    // Linux and others implementation
    urlIn = "xdg-open \""+urlIn+"\"";
    system(urlIn.c_str() );
#endif
}

int getFileSize(const std::string &fileName)
{
    std::ifstream file(fileName.c_str(), std::ifstream::in | std::ifstream::binary);

    if(!file.is_open())
    {
        return -1;
    }

    file.seekg(0, std::ios::end);
    int fileSize = file.tellg();
    file.close();

    return fileSize;
}

std::string getFileSizeString(const std::string &fileName)
{
    float fSizeInBytes = getFileSize( fileName );
    if( fSizeInBytes <  0 )
    {
        return "Error: File Size not found...";
    }
    if( fSizeInBytes >=BSIZE_TB )
    {
        return float_to_string( fSizeInBytes/BSIZE_TB )+"GB";
    }
    else if( fSizeInBytes >=BSIZE_GB )
    {
        return float_to_string( fSizeInBytes/BSIZE_GB )+"GB";
    }
    else if( fSizeInBytes >=BSIZE_MB )
    {
        return float_to_string( fSizeInBytes/BSIZE_MB )+"MB";
    }
    else if( fSizeInBytes >=BSIZE_KB )
    {
        return float_to_string( fSizeInBytes/BSIZE_KB )+"KB";
    }
    return float_to_string( fSizeInBytes )+" bytes";
}

int clean_folder(std::string folderName)
{
    GPE_FileDirectory * dir = new GPE_FileDirectory();
    GPE_File * file = NULL;
    int iFile = 0;
    int iDirectory = 0;

    std::string fileToClick = "";
    std::vector< std::string > foldersToDelete;
    foldersToDelete.push_back(folderName);
    std::string currentFolderToClear = folderName;
    int filesDeletedCount = 0;
    if( dir!=NULL)
    {
        while( (int)foldersToDelete.size() > 0 )
        {
            currentFolderToClear = foldersToDelete[0];
            dir->open_directory(currentFolderToClear);
            for (iFile = (int)dir->get_count()-1; iFile>=0; iFile--)
            {
                file = dir->get_file(iFile);
                if( file!=NULL)
                {
                    fileToClick = file->get_name();
                    if( fileToClick!="." && fileToClick!="..")
                    {
                        fileToClick = currentFolderToClear+"/"+fileToClick;
                        if( file->is_directory() )
                        {
                            foldersToDelete.push_back(fileToClick );
                        }
                        else
                        {
                            remove(fileToClick.c_str() );
                            filesDeletedCount++;
                        }
                    }
                }
            }
            for( iDirectory = (int)foldersToDelete.size()-1; iDirectory >=0; iDirectory--)
            {
                if( currentFolderToClear==foldersToDelete.at(iDirectory) )
                {
                    foldersToDelete.erase( foldersToDelete.begin()+iDirectory);
                }
            }
        }
        delete dir;
        dir = NULL;
        return filesDeletedCount;
    }
    return 0;
}

int copy_folder(std::string folderName, std::string targetFolder, bool copySubFolders, bool overwriteExistingFiles )
{
    if( path_exists( folderName ) == false )
    {
        return -1;
    }
    if( path_exists(targetFolder) == false )
    {
        return -2;
    }

    int filesCopiedCount = 0;
    GPE_FileDirectory * dir = new GPE_FileDirectory();
    GPE_File * file = NULL;
    std::string currentFileName = "";
    int iFile = 0;
    int iDirectory = 0;
    int addedFolderFileCount = 0;
    if( dir!=NULL )
    {
        dir->open_directory(folderName);
        for (iFile = 0; iFile < (int)dir->get_count(); iFile++)
        {
            file = dir->get_file(iFile);
            if( file!=NULL)
            {
                currentFileName = file->get_name();
                if( currentFileName!="." && currentFileName!="..")
                {
                    //displayMessageString = "Copying "+currentFileName;
                    currentFileName = folderName+"/"+currentFileName;
                    if( file->is_directory() )
                    {
                        if( copySubFolders)
                        {
                            create_directory(targetFolder+"/"+file->get_name() );
                            addedFolderFileCount= copy_folder( currentFileName, targetFolder+"/"+ file->get_name(), true, overwriteExistingFiles );

                            if( addedFolderFileCount > 0 )
                            {
                                filesCopiedCount += addedFolderFileCount;
                            }
                        }
                    }
                    else
                    {
                        if( copy_file(currentFileName,targetFolder+"/"+ file->get_name(), overwriteExistingFiles ) )
                        {
                            filesCopiedCount++;
                        }
                    }
                }
            }
        }
        delete dir;
        dir = NULL;
        return filesCopiedCount;
    }
    return 0;
}


bool GPE_Seek_Settings_Folder()
{
    //Sets the folder used in all get_user_settings_folder() calls based on the 2 parameters above
    GPE_APP_SETTINGS_FOLDER = APP_DIRECTORY_NAME;
    if( (int)GPE_ENGINE_SETTINGS->programPublisher.size() ==0 )
    {
        return false;
    }
    //if a publisher is said to be to this program
    //Here is where he fun happens...
    bool useProgramFolder = false;
    bool homeDirFound  = true;

    //Going thru all of the possible getenv paramters that i know of...
    char* homeDir = getenv("%UserProfile%");
    std::string foundPath = "";
    //Attempt 1...
    if( homeDir!=NULL)
    {
        //Attempt 1...
        foundPath = homeDir;
    }
    else
    {
        //Attempt 2...
        homeDir = getenv("home");
        if( homeDir!=NULL )
        {
            foundPath = homeDir;
        }
        else
        {
            //Attempt 3...
            homeDir = getenv("HOME");
            if( homeDir!=NULL)
            {
                foundPath = homeDir;
            }
            else
            {
                //final shot...
                homeDir = getenv("homepath");
                if( homeDir!=NULL )
                {
                    foundPath = homeDir;
                }
                else
                {
                    //All attempts failed defaults to program folder...
                    return false;
                }
            }
        }
    }

    if( homeDirFound && path_exists(foundPath) )
    {
        std::string appDataPath = foundPath;
        if( GPE_FOUND_OS==GPE_IDE_WINDOWS)
        {
            //First Layer of Windows unlocked :-)
            if( create_directory(appDataPath+"/AppData")!=-1)
            {
                foundPath = appDataPath = appDataPath+"/AppData";

                //See if we can get into the Roaming folder now
                if( create_directory(appDataPath+"/Roaming")!=-1)
                {
                    foundPath = appDataPath = appDataPath+"/Roaming";
                }
                else if( create_directory(appDataPath+"/Local")!=-1 )
                {
                    //If failed, lets see if we can get into the Local folder now
                    foundPath = appDataPath = appDataPath+"/Local";
                }
                else if( create_directory(appDataPath+"/LocalLow")!=-1 )
                {
                    foundPath = appDataPath = appDataPath+"/LocalLow";
                }
                else
                {
                    //Windows failed to get into settings...
                    return false;
                }
            }
            else
            {
                useProgramFolder = true;
                appDataPath = foundPath = "";
            }
        }
        else if( GPE_FOUND_OS==GPE_IDE_MAC)
        {
            appDataPath = foundPath+"/~/Library/Preferences";
            if( create_directory(appDataPath)==-1)
            {
                useProgramFolder = true;
                appDataPath = foundPath = "";
            }
            else
            {
                return false;
            }
        }

        if( useProgramFolder )
        {
            //Something failed above so now we just use the program folder...
            return false;
        }

        //If we have some sort of file path...
        if( GPE_FOUND_OS==GPE_IDE_LINUX)
        {
            appDataPath = appDataPath+"/."+GPE_ENGINE_SETTINGS->programPublisher;
        }
        else
        {
            //Windows, Mac, etc afaik...
            appDataPath = appDataPath+"/"+GPE_ENGINE_SETTINGS->programPublisher;
        }

        //If we are able to get into the publisher folder
        if( create_directory(appDataPath)!=-1)
        {
            foundPath = appDataPath;
            GPE_APP_SETTINGS_FOLDER = foundPath;
            if( (int)GPE_ENGINE_SETTINGS->programTitle.size() > 0 )
            {
                //If we are able to get into the titled program's folder
                if( create_directory( foundPath+"/"+GPE_ENGINE_SETTINGS->programTitle )!=-1 )
                {
                    foundPath = foundPath+"/"+GPE_ENGINE_SETTINGS->programTitle;
                    GPE_APP_SETTINGS_FOLDER = foundPath;
                }
                GPE_APP_SETTINGS_FOLDER = GPE_APP_SETTINGS_FOLDER+"/";
                //If this fails we will just default to the publisher and call it a day...
            }
            return true;
        }
        else
        {
            return false;
        }
    }


    //save the settings and local directory to the application directory
    if( useProgramFolder )
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::string get_user_settings_folder()
{
    return GPE_APP_SETTINGS_FOLDER;
}

std::string get_user_temp_folder()
{
    std::string foundPath = get_user_settings_folder();
    std::string tempFolderString = foundPath;
    foundPath = foundPath+"/temp_files";
    if( create_directory(foundPath)!=-1)
    {
        tempFolderString = foundPath+"/";
    }
    return tempFolderString;
}

std::string get_user_screenshot_folder()
{
    std::string foundPath = get_user_settings_folder();
    std::string tempScreenshotString = foundPath;
    foundPath = foundPath+"/screenshots";
    if( create_directory(foundPath)!=-1)
    {
        tempScreenshotString = foundPath+"/";
    }
    return tempScreenshotString;
}

bool delete_file( std::string fName)
{
    //Function not yet created
    return false;
}
