#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<filesystem>

using namespace std;

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

class DataContainer
{
public:
  string InputFilePath1;
  string InputFilePath2;
  string OutputFilePath1;
  string OutputFilePath2;

  DataContainer() {};

  string AdjustInputFilePath(string inputFilePath)
  {
    filesystem::path givenPath = inputFilePath;

    // If givenPath is already absolute, return it as is
    if (givenPath.is_absolute())
    {
      return inputFilePath;
    }
    else
    {
      // Otherwise, combine it with the current path to make it absolute
      filesystem::path currentPath = filesystem::current_path();
      return (currentPath / givenPath).string();
    }
  }

  string FormatOutputFile(string inputFilePath)
  {
    int location = inputFilePath.find(".txt");
    string outputFilePath = "";

    if (location != string::npos)
    {
      outputFilePath = inputFilePath.substr(0, location) + "_output.txt";
    }

    return outputFilePath;
  }

  vector<string> GetWrapped(string& path, int maxLineLength)
  {
    string filePath = path;

    vector<string> lines;
    stringstream ss(filePath);
    string token;

    while (getline(ss, token, '/'))
    {
      lines.push_back(token);
    }

    string currentLine = "";
    vector<string> wrappedString;

    for (const auto& section : lines)
    {
      if (currentLine.length() + section.length() > maxLineLength)
      {
        wrappedString.push_back(currentLine);
        currentLine = "";
      }

      if (section == path)
      {
        currentLine += section;
      }
      else
      {
        currentLine += section + "/";
      }
    }

    if (!currentLine.empty())
    {
      wrappedString.push_back(currentLine);
    }

    return wrappedString;
  }
};

#endif