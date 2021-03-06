/*
 * ReadTextFile.cpp
 *
 *  Created on: Feb 18, 2011
 *      Author: bkloppenborg
 */
 
 /* 
 * Copyright (c) 2012 Brian Kloppenborg
 *
 * The authors request, but do not require, that you acknowledge the
 * use of this software in any publications.  See 
 * https://github.com/bkloppenborg/liboi/wiki
 * for example citations
 *
 * This file is part of the OpenCL Interferometry Library (LIBOI).
 * 
 * LIBOI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 
 * as published by the Free Software Foundation, either version 3 
 * of the License, or (at your option) any later version.
 * 
 * LIBOI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with LIBOI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <streambuf>

#include "textio.hpp"

using namespace std;

/// Split string `s` when the specified delimeter is found. Results are stored
/// in elems
void SplitString(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

/// Split string `s` when the specified delimeter is found.
/// Results are returned as a vector<string>
vector<string> SplitString(const string &s, char delim)
{
    vector<string> elems;
    SplitString(s, delim, elems);
    return elems;
}

/// Removes whitespace from the specified string.
string StripWhitespace(string str)
{
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}

/// Removes whitespace from a vector of strings
void StripWhitespace(vector<string> & strings)
{
	unsigned int end = strings.size();
	for(size_t i = 0; i < end; i++)
	{
		// Strip all whitespace from the beginning and end of the string.
		strings[i] = StripWhitespace(strings[i]);

		// Remove the element from the vector if it is empty.
		if(strings[i].size() == 0)
		{
			strings.erase(strings.begin() + i);
			end--;
		}
	}
}

/// Reads in a file, returns entire contents as a standard, null-terminated, string.
string ReadFile(const string &filename, const string &error_message)
{
	string str;

    ifstream infile;
    infile.open(filename.c_str());

    if(infile.is_open())
    {
		// Determine the size of the file, allocate that much space on str
		infile.seekg(0, std::ios::end);
		str.reserve(infile.tellg());
		// Seek to the start of the file
		infile.seekg(0, std::ios::beg);

		// Now read in the entire file.
		str.assign((std::istreambuf_iterator<char>(infile)),
					std::istreambuf_iterator<char>());
    }
    else
    {
        /// \exception runtime_error Error opening array file
        /// The file could not be located.  It is likely that the user just specified an invalid path.
        throw std::runtime_error(error_message);
    }

	return str;
}

/// Reads in a file, returns non-comment lines as a vector of strings
/// If the file cannot be opened, an exception is thrown with the message contained in error_message
vector<string> ReadFile(const string &filename, const string &comment_chars, const string &error_message)
{
    ifstream infile;
    infile.open(filename.c_str());
    vector < string > lines;

    if (infile.is_open())
    {
        string line;

        while (!infile.eof())
        {
            getline(infile, line);
            while ((line.size() == 0 || comment_chars.find(line[0]) != string::npos)
                   && !infile.eof())
            {
                getline(infile, line);
            }
            if (!infile.eof())
                lines.push_back(line);
        }
        infile.close();
    }
    else
    {
        /// \exception runtime_error Error opening array file
        /// The file could not be located.  It is likely that the user just specified an invalid path.
        throw std::runtime_error(error_message);
    }

    return lines;
}

// Tokenizes a line.
vector<string> Tokenize(const string &line)
{
    istringstream lineStream(line);

    vector < string > tokens;
    while (lineStream)
    {
        string item;

        lineStream >> item;
        tokens.push_back(item);
    }

    return tokens;
}

vector<string> Tokenize(const string &line, const vector< vector<int> > & split_info)
{
	vector <string> tokens;
	string tmp;
	int start, n_chars;

	vector< vector<int> >::iterator it;

	for (unsigned int i = 0; i < split_info.size(); i++)
	{
		start = split_info[i][0];
		n_chars = split_info[i][1];

		tmp = line.substr(start, n_chars);
		tokens.push_back(tmp);
	}


	return tokens;
}

// See if a file exists.  Returns true if it does.
bool FileExists(const string &filename)
{
  ifstream ifile(filename.c_str());
  return bool(ifile);
}
