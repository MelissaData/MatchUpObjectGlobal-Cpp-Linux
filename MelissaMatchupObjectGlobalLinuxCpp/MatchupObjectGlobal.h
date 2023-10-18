#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<stdexcept>
#include<regex>

#include "mdMatchUp.h"
#include "DataContainer.h"

using namespace std;

#ifndef MATCHUPOBJECTGLOBAL_H
#define MATCHUPOBJECTGLOBAL_H

class MatchUpObjectGlobal
{
public:
	// Path to MatchUp Object Global data files (.dat, etc)
	string dataFilePath;

	// Create instance of Melissa MatchUp Object Global
	mdMUReadWrite* mdMatchUpObjGlobal = new mdMUReadWrite;

	MatchUpObjectGlobal(string license, string dataPath);

	// This will call the Lookup function to process the inputs as well as generate the result codes
	void ExecuteObjectAndResultCodes(string inputFilePath, string outputFilePath);

	string simplifyWhitespace(const string& input);
};

#endif
