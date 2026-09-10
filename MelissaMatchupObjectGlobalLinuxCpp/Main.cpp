/**
 * @file Main.cpp
 * @brief MatchUp Object Global is an extremely fast and powerful programmer's tool that can
 * be integrated into custom applications to eliminate duplicate records.
 *
 * High-level flow of this sample:
 *   1. SETUP     - create an mdMUReadWrite instance, license it, point it at the data
 *                  files, choose a matchcode, then InitializeDataFiles() (one time).
 *   2. INPUT     - read a pipe-delimited input file record by record.
 *   3. PROCESS   - for each record map its fields, BuildKey() and WriteRecord(); then
 *                  Process() runs the dedupe pass across all records.
 *   4. READ      - ReadRecord() each processed record and read its result codes, dupe
 *                  group, and key; write them to an output file.
 *   5. INTERPRET - a result code of "MS03" marks a record as a duplicate.
 *
 * The pieces of this sample map onto that flow:
 *   - main / ParseArguments / RunAsConsole : console harness (argument parsing + the interactive loop).
 *   - MatchUpObjectGlobal : wrapper around mdMUReadWrite (setup + the dedupe pipeline).
 *   - DataContainer       : holds the input/output file paths and small path helpers.
 *
 * Where mdMUReadWrite comes from:
 *   There is no generated wrapper source for C++. mdMatchup.h and
 *   mdMatchupEnums.h declare the API, and libmdMatchup.so carries the implementation. libmdGlobalParse.so is
 *   downloaded alongside it. The
 *   accompanying MelissaMatchupObjectGlobalLinuxCpp.sh script downloads all four on every run.
 *
 * Reference:
 *   Quickstart    : https://docs.melissa.com/on-premise-api/matchup-object/matchup-object-quickstart.html
 *   Release notes : https://releasenotes.melissa.com/on-premise-api/matchup-object/
 *   Result codes  : https://docs.melissa.com/on-premise-api/matchup-object-global/result-codes.html
 */

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

/**
 * Entry point. Reads the optional command-line arguments, then hands control to
 * RunAsConsole, which performs the actual MatchUp Object Global setup and processing.
 *
 * @param argc The count of command-line arguments.
 * @param argv The raw command-line arguments.
 */
int main(int argc, char* argv[])
{
	// Populated by ParseArguments below.
	string testGlobalFile = "";
	string testUsFile = "";
	string license = "";
	string dataPath = "";

	ParseArguments(license, testGlobalFile, testUsFile, dataPath, argc, argv);
	RunAsConsole(license, testGlobalFile, testUsFile, dataPath);

	return 0;
}

/**
 * Reads the supported command-line options into the reference parameters.
 *
 * Recognized flags (each followed by its value):
 *   --license / -l   : the Melissa license string
 *   --global / -g    : path to the global input file to test in one-shot mode
 *   --us / -u        : path to the US input file to test in one-shot mode
 *   --dataPath / -d  : path to the MatchUp Object data files
 *
 * @param license        Receives the Melissa license string.
 * @param testGlobalFile Receives the global input file to test in one-shot mode.
 * @param testUsFile     Receives the US input file to test in one-shot mode.
 * @param dataPath       Receives the path to the MatchUp Object data files.
 * @param argc           The count of command-line arguments to parse.
 * @param argv           The raw command-line arguments to parse.
 */
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

/**
 * Sets up the MatchUp Object once, then drives the input -> process -> output cycle.
 *
 * In interactive mode (no file args) it loops, asking for the two input files each
 * pass until the user answers "N". In one-shot mode (file args supplied) it runs a
 * single pass and exits. Each input file is deduped to a matching "_output.txt" file.
 *
 * @param license        The Melissa license string used to initialize the object.
 * @param testGlobalFile A global input file to process in one-shot mode; if empty, the program prompts interactively.
 * @param testUsFile     A US input file to process in one-shot mode; if empty, the program prompts interactively.
 * @param dataPath       Path to the MatchUp Object data files.
 */
void RunAsConsole(string license, string testGlobalFile, string testUsFile, string dataPath)
{
	cout << "\n\n=============== WELCOME TO MELISSA MATCHUP OBJECT GLOBAL LINUX C++ ===============\n" << endl;
	
	// Construct the wrapper. This is where the object is licensed, pointed at the
	// data files, configured, and initialized (see the MatchUpObjectGlobal constructor below).
	MatchUpObjectGlobal* matchUpObjectGlobal = new MatchUpObjectGlobal(license, dataPath);

	bool shouldContinueRunning = true;

	while (shouldContinueRunning)
	{
		// Holder for this pass's input/output file paths.
		DataContainer dataContainer = DataContainer();

		if (testGlobalFile.empty() && testUsFile.empty())
		{
			// Interactive mode: prompt the user for the two input file paths.
			cout << "\nFill in each value to see the MatchUp Object Global results" << endl;

			cout << "Global Input File: ";
			getline(cin, dataContainer.InputFilePath1);

			cout << "US Input File: ";
			getline(cin, dataContainer.InputFilePath2);
		}
		else
		{
			// One-shot mode: use the file paths passed on the command line.
			dataContainer.InputFilePath1 = testGlobalFile;
			dataContainer.InputFilePath2 = testUsFile;
		}

		// Resolve each input path to an absolute path.
		dataContainer.InputFilePath1 = dataContainer.AdjustInputFilePath(dataContainer.InputFilePath1);
		dataContainer.InputFilePath2 = dataContainer.AdjustInputFilePath(dataContainer.InputFilePath2);

		// Derive each output path from its input path (e.g. foo.txt -> foo_output.txt).
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

		// Execute MatchUp Object Global
		// Dedupe each input file independently, writing results to its output file.
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

		// In one-shot mode there is nothing more to do after a single pass: mark the
		// input handled and stop the outer loop.
		if (!(testGlobalFile + testUsFile).empty())
		{
			isValid = true;
			shouldContinueRunning = false;
		}

		// Interactive mode: ask whether to process another pair of files. Keep prompting
		// until we get a valid Y/N. "N" ends the program; "Y" falls through to another pass.
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
