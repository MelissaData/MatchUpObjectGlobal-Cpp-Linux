#include "MatchupObjectGlobal.h"

MatchUpObjectGlobal::MatchUpObjectGlobal(string license, string dataPath)
{
  dataFilePath = dataPath;

  // Set license string and set path to data files
  mdMatchUpObjGlobal->SetLicenseString(license.c_str());
  mdMatchUpObjGlobal->SetPathToMatchUpFiles(dataFilePath.c_str());
  mdMatchUpObjGlobal->SetKeyFile("temp.key");
  mdMatchUpObjGlobal->SetMatchcodeName("Global Address");
  mdMatchUpObjGlobal->SetMaximumCharacterSize(1);

  // Load the data files. The returned ProgramStatus reports whether initialization succeeded.
  // A non-ErrorNone status means initialization failed - commonly an invalid/expired license
  // or missing/wrong-path data files. Until it reports "No Error" the object is not ready.
  mdMUReadWrite::ProgramStatus pStatus = mdMatchUpObjGlobal->InitializeDataFiles();

  // If an issue occurred, please investigate the common causes.
  if (pStatus != mdMUReadWrite::ProgramStatus::ErrorNone)
  {
    cout << "Failed to Initialize Object." << endl;
    cout << pStatus << endl;
    return;
  }

  // Diagnostic information, handy for confirming the object loaded the data you expect:

  // Build date of the data files
  cout << "                   DataBase Date: " + string(mdMatchUpObjGlobal->GetDatabaseDate()) << endl;

  // When the license stops working
  cout << "                 Expiration Date: " + string(mdMatchUpObjGlobal->GetLicenseExpirationDate()) << endl;

  /**
      * This number should match with the file properties of the Melissa Object binary file.
      * If TEST appears with the build number, there may be a license key issue.
      */
  cout << "                  Object Version: " + string(mdMatchUpObjGlobal->GetBuildNumber()) << endl;
}

// This will call the functions to process the input files as well as generate the result codes
void MatchUpObjectGlobal::ExecuteObjectAndResultCodes(string inputFilePath, string outputFilePath)
{
  ifstream inFile;
  ofstream outFile;
  string record;
  vector<string> fields;

  long total = 0, dupes = 0;

  // Establish field mappings: when you change the matchcode, you will change these
  mdMatchUpObjGlobal->ClearMappings();

  if (mdMatchUpObjGlobal->AddMapping(mdMUReadWrite::Country) == 0 ||
    mdMatchUpObjGlobal->AddMapping(mdMUReadWrite::Address) == 0 ||
    mdMatchUpObjGlobal->AddMapping(mdMUReadWrite::Address) == 0 ||
    mdMatchUpObjGlobal->AddMapping(mdMUReadWrite::Address) == 0 ||
    mdMatchUpObjGlobal->AddMapping(mdMUReadWrite::Address) == 0)
  {
    cout << "\nError: Incorrect AddMapping() parameter" << endl;
    exit(1);
  }

  // Proccess the sample data file
  try
  {
    inFile.open(inputFilePath);
    outFile.open(outputFilePath);

    // Skip the header row, then read each record.
    getline(inFile, record);

    while (getline(inFile, record))
    {
      stringstream ss(record);
      string field;
      fields.clear();

      // Read and parse pipe delimited record
      while (getline(ss, field, '|'))
      {
        fields.push_back(field);
      }

      // Load up the fields
      mdMatchUpObjGlobal->ClearFields();

      mdMatchUpObjGlobal->AddField(fields[7].c_str());
      mdMatchUpObjGlobal->AddField(fields[3].c_str());
      mdMatchUpObjGlobal->AddField(fields[4].c_str());
      mdMatchUpObjGlobal->AddField(fields[5].c_str());
      mdMatchUpObjGlobal->AddField(fields[6].c_str());
      
      // Create a UserInfo string which uniquely identifies the records
      mdMatchUpObjGlobal->SetUserInfo(fields[0].c_str());
      
      // Build the key and submit it
      mdMatchUpObjGlobal->BuildKey();
      mdMatchUpObjGlobal->WriteRecord();
    }

    // All records are loaded; Process() runs the match/dedupe pass across them.
    mdMatchUpObjGlobal->Process();

    // Write a header row, then read each processed record back and record its result
    // codes, dupe group, and key. "MS03" in the results flags the record as a duplicate.
    outFile << "Id|ResultCodes|DupeGroup|Key" << endl;

    // ResultsCodes explain any issues MatchUp Object Global has with the object.
    // List of result codes for MatchUp Object Global
    // https://docs.melissa.com/on-premise-api/matchup-object-global/result-codes.html
    while (mdMatchUpObjGlobal->ReadRecord() != 0)
    {
      string location = mdMatchUpObjGlobal->GetResults();
      if (location.find("MS03") != string::npos)
      {
        dupes++;
      }

      mdMatchUpObjGlobal->ClearFields();

      outFile << mdMatchUpObjGlobal->GetUserInfo() << "|" << mdMatchUpObjGlobal->GetResults() << "|"
        << mdMatchUpObjGlobal->GetDupeGroup() << "|" << simplifyWhitespace(mdMatchUpObjGlobal->GetKey()) << endl;

      total++;
    }
  }
  catch (const exception ex)
  {
    cout << ex.what() << endl;
  }
}

string MatchUpObjectGlobal::simplifyWhitespace(const string& input) {
  static const regex multipleWhitespacePattern("\\s+");
  return regex_replace(input, multipleWhitespacePattern, " ");
}
