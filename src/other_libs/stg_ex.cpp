/*
stg_ex.cpp
This file is part of:
stg_ex
https://pawbyte.com/stg_ex
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.

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

-stg_ex <https://pawbyte.com/stg_ex>


*/

#include "stg_ex.h"

namespace stg_ex
{
    bool char_is_alnum(char c, bool allowSpaces, bool allowUnderscores)
    {
        if( allowSpaces && c == ' ')
        {
            return true;
        }
        if( allowUnderscores && c == '_')
        {
            return true;
        }
        return isalnum(c);
    }

    bool char_is_alpha(char c, bool allowSpaces, bool allowUnderscores)
    {
        if( allowSpaces && c == ' ')
        {
            return true;
        }
        if( allowUnderscores && c == '_')
        {
            return true;
        }
        return isalpha(c);
    }

    bool is_alnum(const std::string str, bool allowSpaces, bool allowUnderscores)
    {
        int strSize = (int)str.size();
        bool validCharacterFound = true;
        if( strSize >0)
        {
            if( isalpha(str[0]) )
            {
                int i = 1;
                while( i < strSize && validCharacterFound)
                {
                    validCharacterFound = char_is_alnum(str[i], allowSpaces, allowUnderscores );
                    i++;
                }
                return validCharacterFound;
            }
        }
        return false;
    }

    std::vector <std::string> divide_string_into_lines( const std::string str,int numbLines)
    {
        std::vector <std::string> returnVector;
        /*
        int wholeLength= str.size();
        if(numbLines>0)
        {
             int maxLengthForString = wholeLength/numbLines;
             for(int ii=0;ii<numbLines;ii+=1)
             {

                  //divides the string into multiple two pieces
                  std::string line_text_xx=str.substr((i*maxLengthForString),(i+1)*maxLengthForString);
                  std::string new_text_n=str.substr((i*maxLengthForString),line_text_x.size()-(i*maxLengthForString));
                  //checks for the first character in the string of both old and new
                  std::string firstForNew=new_text_n.substr(0,1);
                  std::string firstForNew2 = string_letters(firstForNew);
                  std::string lastForOld = line_text_xx.substr(line_text_xx.size(),1);
                  std::string lastForOld2= string_letters(lastForOld);
                  //if a problem arises like the ending character not being empty, we add a dash to the string being inserted
                  if( (firstForNew.compare(" ")==0)&&(lastForOld.compare("")==0)&&(firstForNew.compare(firstForNew2) )&&(lastForOld.compare(lastForOld2) ))
                  {
                       //adds a dash to the string and adds it to our vector
                       returnVector.push_back(line_text_xx.append("-"));
                  }
                  else
                  {
                       //lovely we have a space, thus we just add it as planned.
                       returnVector.push_back(line_text_xx);
                  }
             }
        }
        else
        {
            returnVector.push_back(str);
        }
        */
        return returnVector;
    }

    std::string double_to_string(double in)
    {
        std::ostringstream converter;
        converter << in;
        return converter.str();
    }

    bool file_is_audio( const std::string str_file_name)
    {
        std::string str_found_extension = get_file_ext(str_file_name);
        if( str_found_extension=="aac" || str_found_extension=="mp3"  || str_found_extension=="ogg"  || str_found_extension=="wav" )
        {
            return true;
        }
        return false;
    }

    bool file_is_font( const std::string str_file_name)
    {
        std::string str_found_extension = get_file_ext(str_file_name);
        if( str_found_extension=="eot" || str_found_extension=="svg"  || str_found_extension=="ttf"  || str_found_extension=="woff" || str_found_extension=="woff2" )
        {
            return true;
        }
        return false;
    }

    bool file_is_image( const std::string str_file_name)
    {
        std::string str_found_extension = get_file_ext(str_file_name);
        if( str_found_extension=="bmp" || str_found_extension=="gif"  ||str_found_extension=="jpg"  || str_found_extension=="jpeg"  || str_found_extension=="png" )
        {
            return true;
        }
        return false;
    }

    bool file_is_video( const std::string str_file_name)
    {
        std::string str_found_extension = get_file_ext(str_file_name);
        if( str_found_extension=="mp4" || str_found_extension=="ogg" || str_found_extension=="webm" )
        {
            return true;
        }
        return false;
    }

    std::string file_to_dir( const std::string str_file_name)
    {
        if(!str_file_name.empty())
        {

            return str_file_name.substr(0,str_file_name.find(".") );
        }
        else
        {
            return "";
        }
        return str_file_name;
    }

    std::string float_to_string(float in)
    {
        std::ostringstream converter;
        converter << in;
        return converter.str();
    }

    //returns the inputed value with 2 digits each time
    std::string get_2digit_value_string(int numberIn)
    {
        if(numberIn<10)
        {
            std::stringstream tempText;
            tempText << "0" << int_to_string(numberIn);
            return tempText.str();
        }
        else
        {
            return int_to_string(numberIn);
        }
    }

    std::string get_file_ext( const std::string str_file_name)
    {
        if( (int)str_file_name.size() > 0)
        {
            int dotPos = str_file_name.find_first_of(".");
            if(dotPos != (int)std::string::npos)
            {
                return str_file_name.substr( dotPos+ 1);
            }
        }
        return "";
    }

    std::string get_file_ext_last_dot( const std::string str_file_name)
    {
        if( (int)str_file_name.size() > 0)
        {
            int dotPos = str_file_name.find_last_of(".");
            if(dotPos != (int)std::string::npos)
            {
                return str_file_name.substr( dotPos+ 1);
            }
        }
        return "";
    }

    std::string get_file_noext( const std::string str_file_name)
    {
        if( str_file_name.size() > 0)
        {
            int dotPos = str_file_name.find_first_of(".");
            if(dotPos != (int)std::string::npos)
            {
                return str_file_name.substr( 0,dotPos);
            }
            else
            {
                return str_file_name;
            }
        }
        return "";
    }

    std::string get_local_from_global_file( const std::string str)
    {
        if( str.size()>0)
        {
            size_t found;
            found=str.find_last_of("/\\");
            if( found != std::string::npos)
            {
                std::string dString = str.substr(0,found);
                std::string rString = str.substr(found+1);
                return rString;
            }
        }
        return str;
    }

    std::string get_path_from_file( const std::string str)
    {
        if( str.size()>0)
        {
            size_t found;
            found=str.find_last_of("/\\");
            if( found != std::string::npos)
            {
                std::string rString = str.substr(0,found);
                return rString+"\\";
            }
        }
        return str;
    }

    std::string get_short_filename(  const std::string str_file_name,bool show_extension)
    {
        if(!str_file_name.empty())
        {
            if(show_extension)
            {
                return str_file_name.substr(str_file_name.find_last_of("\\/") +1);
            }
            else
            {
                return str_file_name.substr(str_file_name.find_last_of("\\/")+1,str_file_name.find(".") );
            }
        }
        else
        {
            return "";
        }
        return str_file_name;
    }

    std::string get_substring( const std::string str, int cStart, int cLength)
    {
        int strSize = (int)str.size();
        if(  strSize > 0 && strSize > cStart && cStart >= 0)
        {
            if( strSize > cStart+cLength && cLength > 0)
            {
                return str.substr(cStart,cLength);
            }
            else if( cLength!=0 )
            {
                return str.substr(cStart);
            }
        }
        return "";
    }

    int get_leading_space_count(std::string str)
    {
        if( (int)str.size() > 0)
        {
            int spacesCounted = 0;
            for( int i = 0; i < (int)str.size(); i++)
            {
                if( str[i]==' ')
                {
                    spacesCounted++;
                }
                else
                {
                    break;
                }
            }
            return spacesCounted;
        }
        return 0;
    }

    int get_trailing_space_count( std::string str)
    {
        if( (int)str.size() > 0)
        {
            int spacesCounted = 0;
            for( int i = (int)str.size()-1; i>=0; i--)
            {
                if( str[i]==' ')
                {
                    spacesCounted++;
                }
                else
                {
                    break;
                }
            }
            return spacesCounted;
        }
        return 0;
    }

    std::string int_to_string(int in)
    {
        std::ostringstream converter;
        converter << in;
        return converter.str();
    }

    int split_first_int(std::string &s, char separator)
    {
        size_t seppos = s.find_first_of(separator);
        if (seppos == std::string::npos)
        {
            s = "";
            return 0; // not found
        }
        int num = string_to_int(s.substr(0, seppos));
        s = s.substr(seppos+1);
        return num;
    }

    std::string split_first_string(std::string &s, char separator)
    {
        size_t seppos = s.find_first_of(separator);
        if (seppos == std::string::npos)
            return ""; // not found
        std::string outs = s.substr(0, seppos);
        s = s.substr(seppos+1);
        return outs;
    }

    std::string split_first_string(std::string& s, std::string separator)
    {
        //if(separator!=NULL)
        {
            if((int)separator.length()>=1)
            {
                size_t seppos = s.find(separator);
                if (seppos == std::string::npos)
                    return ""; // not found
                std::string outs = s.substr(0, seppos);
                s = s.substr(seppos+separator.length() );
                return outs;
            }
        }
        return "";
    }

    bool string_contains(const std::string& hay, const std::string& needle)
    {
        int haySize = (int)needle.size();
        int needleSize = (int)needle.size();
        if( (int)hay.size() > 0 && needleSize > 0 && haySize >= needleSize )
        {
            return ( hay.find(needle ) != std::string::npos );
        }
        return false;
    }

    bool string_ends(const std::string& hay, const std::string& needle)
    {
        if (hay.size() < needle.size() )
        {
            return false;
        }
        return std::equal(hay.begin() + hay.size() - needle.size(), hay.end(), needle.begin());
    }



    int string_count(std::string str, std::string needle )
    {
        int position = 0;
        int returnCount=0;
        //
        int strSize = (int)str.size();
        int needleSize = (int)needle.size();
        if( strSize > 0 && needleSize > 0 )
        {
            position = str.find( needle, position+1 );
            while ( position != (int)std::string::npos && position < strSize )
            {
                position = str.find( needle, position+needleSize );
                returnCount++;
            }
        }
        return returnCount;
    }

    std::string string_digits(std::string str)
    {
        return str;
    }

    std::string string_letters(std::string str)
    {
        return str;
    }

    std::string string_lettersdigits(std::string str)
    {
        return str;
    }

    std::string string_lower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower );
        return str;
    }

    std::string string_repeat(std::string str,int repeatCount )
    {
        if( repeatCount > 0)
        {
            std::string strReturn = "";
            for( int i = 0; i < repeatCount ; i++)
            {
                strReturn+=str;
            }
            return strReturn;
        }
        return "";
    }

    std::string string_replace_all(std::string str, std::string substring, std::string newstr)
    {
        int newStrLen = (int)newstr.size();
        int substrLen = (int)substring.size();
        int position = str.find( substring ); // find first substring
        //
        while ( position != (int)std::string::npos )
        {
            str.replace( position, substrLen, newstr );
            if( newStrLen ==0 )
            {
                position = str.find( substring, position  );
            }
            else
            {
                position = str.find( substring, position + newStrLen );
            }
        }
        return str;
    }

    bool string_starts(const std::string& hay, const std::string& needle)
    {
        int haySize = (int)hay.size();
        int needleSize = (int)needle.size();
        if(  needleSize > 0 && haySize >= needleSize )
        {
            for( int i = 0; i < needleSize; i++)
            {
                if( hay[i]!=needle[i] )
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    bool string_to_bool(std::string value)
    {
        trim_string(value,' ');

        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        if (value == "true")
            return true;
        if (value == "yes")
            return true;
        if (value == "1")
            return true;
        if (value == "false")
            return false;
        if (value == "no")
            return false;
        if (value == "0")
            return false;

        return false;
    }

    int string_to_int(const std::string& s, int default_value)
    {
        int result = default_value;
        if( (int)s.size() > 0)
        {
            char *endp;

            long value = std::strtol(s.c_str(), &endp, 10);

            if (endp == s.c_str() ||  (*endp != 0) )
            {
            }
            else
            {
                result = value;
            }
        }
        return result;
    }

    float string_to_float( const std::string& s, float default_value )
    {
        //Credit: Alessandro Jacopson ( http://stackoverflow.com/users/15485/alessandro-jacopson )
        //Source: http://stackoverflow.com/a/393027
        std::istringstream i(s);
        if( (int)s.size() > 0)
        {
            float x;
            if (!(i >> x))
            {
                return default_value;
            }
            return x;
        }
        return default_value;
    }

    std::string string_upper(std::string str)
    {
        return str;
    }

    std::string trim_string(const std::string s, char c)
    {
        if (s.length() == 0)
            return "";

        unsigned int first = 0;
        unsigned int last = s.length()-1;

        while (s.at(first) == c && first < s.length()-1)
        {
            first++;
        }
        while (s.at(last) == c && last >= first && last > 0)
        {
            last--;
        }
        if (first <= last)
            return s.substr(first,last-first+1);
        return "";
    }

    std::string trim_left_inplace(std::string       s,const std::string& delimiters)
    {
        return s.erase( 0, s.find_first_not_of( delimiters ) );
    }

    std::string trim_right_inplace(std::string s, const std::string& delimiters)
    {
        return s.erase(s.find_last_not_of(delimiters) + 1);
    }

    std::string trim_string(std::string s, const std::string& delimiters)
    {
        return trim_left_inplace(trim_right_inplace(s, delimiters), delimiters);
    }

    bool wrap_string( const std::string str,std::vector < std::string > &strVector, int lineWidth, int maxLines)
    {
        if( lineWidth <= 0)
        {
            lineWidth = 256;
        }

        strVector.clear();
        if( (int) str.size() >= lineWidth )
        {
            int prevSpacePos = 0;
            int prevSavedPos = 0;
            int spacePos = 0;
            int countedStrings = 0;
            while( (int)str.size() > spacePos && ( maxLines <=0 || ( maxLines > 0 && countedStrings < maxLines ) ) )
            {
                spacePos=str.find(" ",prevSpacePos);
                if( spacePos!=(int)std::string::npos )
                {
                    if( spacePos-prevSavedPos >= lineWidth )
                    {
                        if( prevSpacePos > prevSavedPos)
                        {
                            strVector.push_back( str.substr(prevSavedPos,prevSpacePos - prevSavedPos )  );
                            prevSavedPos = prevSpacePos;
                            prevSpacePos++;
                        }
                        else
                        {
                            strVector.push_back( str.substr(prevSavedPos,1 )  );
                            prevSpacePos = prevSavedPos+1;
                        }
                        countedStrings++;
                    }
                    else
                    {
                        prevSpacePos = spacePos+1;
                    }
                }
                else
                {
                    strVector.push_back( str.substr(prevSavedPos,lineWidth)  );
                    countedStrings++;
                    prevSpacePos = prevSavedPos+lineWidth+1;
                    prevSavedPos = prevSpacePos;
                    spacePos = prevSpacePos+1;
                }
            }
            return true;
        }
        if( (int)strVector.size() ==0)
        {
            strVector.push_back( str );
            return false;
        }
        return false;
    }
}

