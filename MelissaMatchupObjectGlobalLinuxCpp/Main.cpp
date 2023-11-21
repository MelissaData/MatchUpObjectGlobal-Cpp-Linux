#include <iostream>
#include <string>
#include <cstdlib>
#include <list>

#include "mdMatchup.h"
#include "MatchupObjectGlobal.h"
#include "DataContainer.h"

using namespace std;

// function declarations
void ParseArguments(string& license, string& testGlobalFile, string& testUsFile, string& dataPath, int argc, char* argv[]);
void RunAsConsole(string license, string testGlobalFile, string testUsFile, string dataPath);
list<string> SplitResultCodes(string s, string delimiter);

int main(int argc, char* argv[])
{
	// variables
	string testGlobalFile = "";
	string testUsFile = "";
	string license = "";
	string dataPath = "";

	ParseArguments(license, testGlobalFile, testUsFile, dataPath, argc, argv);
	RunAsConsole(license, testGlobalFile, testUsFile, dataPath);

	return 0;
}

void ParseArguments(string& license, string& testGlobalFile, string& testUsFile, string& dataPath, int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "--global" || string(argv[i]) == "-g")
		{
			if (argv[i + 1] != NULL)
			{
				testGlobalFile = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--us" || string(argv[i]) == "-u")
		{
			if (argv[i + 1] != NULL)
			{
				testUsFile = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--license" || string(argv[i]) == "-l")
		{
			if (argv[i + 1] != NULL)
			{
				license = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--dataPath" || string(argv[i]) == "-d")
		{
			if (argv[i + 1] != NULL)
			{
				dataPath = argv[i + 1];
			}
		}
	}
}

void RunAsConsole(string license, string testGlobalFile, string testUsFile, string dataPath)
{
	cout << "\n\n=============== WELCOME TO MELISSA MATCHUP OBJECT GLOBAL LINUX C++ ===============\n" << endl;
	
	MatchUpObjectGlobal* matchUpObjectGlobal = new MatchUpObjectGlobal(license, dataPath);

	bool shouldContinueRunning = true;

	while (shouldContinueRunning)
	{
		DataContainer dataContainer = DataContainer();

		if (testGlobalFile.empty() && testUsFile.empty())
		{
			cout << "\nFill in each value to see the MatchUp Object Global results" << endl;

			cout << "Global Input File: ";
			getline(cin, dataContainer.InputFilePath1);

			cout << "US Input File: ";
			getline(cin, dataContainer.InputFilePath2);
		}
		else
		{
			dataContainer.InputFilePath1 = testGlobalFile;
			dataContainer.InputFilePath2 = testUsFile;
		}

		dataContainer.InputFilePath1 = dataContainer.AdjustInputFilePath(dataContainer.InputFilePath1);
		dataContainer.InputFilePath2 = dataContainer.AdjustInputFilePath(dataContainer.InputFilePath2);

		dataContainer.OutputFilePath1 = dataContainer.FormatOutputFile(dataContainer.InputFilePath1);
		dataContainer.OutputFilePath2 = dataContainer.FormatOutputFile(dataContainer.InputFilePath2);

		// Print user input
		cout << "\n===================================== INPUTS =====================================\n" << endl;
		
		vector<string> sections = dataContainer.GetWrapped(dataContainer.InputFilePath1, 50);

		cout << "                Global Input File: " + sections[0] << endl;

		for (int i = 1; i < sections.size(); i++)
		{
			if (i == sections.size() - 1)
			{
				sections[i] = sections[i].substr(0, sections[i].length() - 1);
			}

			cout << "\t                           " + sections[i] << endl;
		}
		
		sections = dataContainer.GetWrapped(dataContainer.InputFilePath2, 50);

		cout << "                    US Input File: " + sections[0] << endl;

		for (int i = 1; i < sections.size(); i++)
		{
			if (i == sections.size() - 1)
			{
				sections[i] = sections[i].substr(0, sections[i].length() - 1);
			}

			cout << "\t                           " + sections[i] << endl;
		}

		// Execute Address Object
		matchUpObjectGlobal->ExecuteObjectAndResultCodes(dataContainer.InputFilePath1, dataContainer.OutputFilePath1);
		matchUpObjectGlobal->ExecuteObjectAndResultCodes(dataContainer.InputFilePath2, dataContainer.OutputFilePath2);

		// Print output
		cout << "\n===================================== OUTPUT =====================================\n" << endl;
		
		sections = dataContainer.GetWrapped(dataContainer.OutputFilePath1, 50);

		cout << "\n  MatchUp Object Global Information:" << endl;
		cout << "               Global Output File: " + sections[0] << endl;

		for (int i = 1; i < sections.size(); i++)
		{
			if (i == sections.size() - 1)
			{
				sections[i] = sections[i].substr(0, sections[i].length() - 1);
			}

			cout << "\t                           " + sections[i] << endl;
		}

		sections = dataContainer.GetWrapped(dataContainer.OutputFilePath2, 50);

		cout << "                   US Output File: " + sections[0] << endl;

		for (int i = 1; i < sections.size(); i++)
		{
			if (i == sections.size() - 1)
			{
				sections[i] = sections[i].substr(0, sections[i].length() - 1);
			}

			cout << "\t                           " + sections[i] << endl;
		}


		bool isValid = false;
		if (!(testGlobalFile + testUsFile).empty())
		{
			isValid = true;
			shouldContinueRunning = false;
		}
		while (!isValid)
		{
			string testAnotherResponse;

			cout << "\nTest another file? (Y/N)" << endl;
			getline(cin, testAnotherResponse);

			if (!testAnotherResponse.empty())
			{
				if (testAnotherResponse == "y" || testAnotherResponse == "Y")
				{
					isValid = true;
				}
				else if (testAnotherResponse == "n" || testAnotherResponse == "N")
				{
					isValid = true;
					shouldContinueRunning = false;
				}
				else
				{
					cout << "Invalid Response, please respond 'Y' or 'N'" << endl;
				}
			}
		}
	}
	cout << "\n===================== THANK YOU FOR USING MELISSA C++ OBJECT =====================\n" << endl;
}
