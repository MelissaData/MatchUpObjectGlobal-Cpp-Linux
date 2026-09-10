#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<stdexcept>
#include<regex>

#include "mdMatchup.h"
#include "DataContainer.h"

using namespace std;

#ifndef MATCHUPOBJECTGLOBAL_H
#define MATCHUPOBJECTGLOBAL_H

/**
 * Wrapper that owns a single Melissa MatchUp Object Global instance (an mdMUReadWrite) and
 * encapsulates one-time setup (license + data files + matchcode) and the per-file dedupe
 * pipeline. Reuse one instance across many files; do NOT re-initialize per file.
 */
class MatchUpObjectGlobal
{
public:
	// Path to the MatchUp Object Global data files.
	string dataFilePath;

	// The underlying Melissa MatchUp Object Global instance.
	mdMUReadWrite* mdMatchUpObjGlobal = new mdMUReadWrite;

	/**
	 * Performs the mandatory one-time setup, in this required order:
	 *   1. SetLicenseString        - authorize the object.
	 *   2. SetPathToMatchUpFiles   - tell it where the data files live.
	 *   3. SetKeyFile / SetMatchcodeName / SetMaximumCharacterSize - configure the run.
	 *   4. InitializeDataFiles     - load the data into memory.
	 *
	 * @param license  The Melissa license string used to authorize the object.
	 * @param dataPath Path to the folder containing the MatchUp Object data files.
	 */
	MatchUpObjectGlobal(string license, string dataPath);

	/**
	 * Runs the MatchUp dedupe pipeline over one input file and writes results to an
	 * output file. Per file: map the matchcode fields, then for each input record
	 * ClearFields -> AddField -> SetUserInfo -> BuildKey -> WriteRecord; call Process()
	 * to dedupe; then ReadRecord() each result and write its codes / dupe group / key.
	 *
	 * @param inputFilePath  Pipe-delimited input file to read.
	 * @param outputFilePath File to write the per-record results to.
	 */
	void ExecuteObjectAndResultCodes(string inputFilePath, string outputFilePath);

	/**
	 * Collapses any run of whitespace in the input into a single space. Used to tidy
	 * the generated key before writing it to the output file.
	 *
	 * @param input The string to normalize.
	 * @return The input with each whitespace run reduced to one space.
	 */
	string simplifyWhitespace(const string& input);
};

#endif
