#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<filesystem>

using namespace std;

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

/**
 * Holds the input/output file paths for one pass, plus small path helpers
 * (AdjustInputFilePath resolves a relative path to absolute; FormatOutputFile derives
 * the output name; GetWrapped wraps a long path for display).
 */
class DataContainer
{
public:
  // Input: the global input file path.
  string InputFilePath1;
  // Input: the US input file path.
  string InputFilePath2;
  // Output: the global output file path.
  string OutputFilePath1;
  // Output: the US output file path.
  string OutputFilePath2;

  DataContainer() {};

  /**
   * Resolves the given input path to an absolute path. If it is already absolute it is
   * returned unchanged; otherwise it is combined with the current working directory.
   *
   * @param inputFilePath The input file path to resolve.
   * @return The absolute form of inputFilePath.
   */
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

  /**
   * Derives the output file path from the input path by inserting "_output" before the
   * ".txt" extension.
   *
   * @param inputFilePath The input file path to derive the output path from.
   * @return The derived output file path.
   */
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

  /**
   * Splits a long file path into chunks no longer than maxLineLength so it prints
   * neatly across several lines in the console output. Display-only helper.
   *
   * @param path          The file path to wrap.
   * @param maxLineLength The maximum length of each wrapped line.
   * @return A list of path chunks, each no longer than maxLineLength.
   */
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