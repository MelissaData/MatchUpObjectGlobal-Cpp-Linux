#include "MatchupObjectGlobal.h"

MatchUpObjectGlobal::MatchUpObjectGlobal(string license, string dataPath)
{
  dataFilePath = dataPath;

  // Set license string and set path to datafiles (.dat, etc)
  mdMatchUpObjGlobal->SetLicenseString(license.c_str());
  mdMatchUpObjGlobal->SetPathToMatchUpFiles(dataFilePath.c_str());
  mdMatchUpObjGlobal->SetKeyFile("temp.key");
  mdMatchUpObjGlobal->SetMatchcodeName("Global Address");
  mdMatchUpObjGlobal->SetMaximumCharacterSize(1);

  mdMUReadWrite::ProgramStatus pStatus = mdMatchUpObjGlobal->InitializeDataFiles();

  if (pStatus != mdMUReadWrite::ProgramStatus::ErrorNone)
  {
    cout << "Failed to Initialize Object." << endl;
    cout << pStatus << endl;
    return;
  }

  cout << "                   DataBase Date: " + string(mdMatchUpObjGlobal->GetDatabaseDate()) << endl;
  cout << "                 Expiration Date: " + string(mdMatchUpObjGlobal->GetLicenseExpirationDate()) << endl;

  /**
      * This number should match with the file properties of the Melissa Object binary file.
      * If TEST appears with the build number, there may be a license key issue.
      */
  cout << "                  Object Version: " + string(mdMatchUpObjGlobal->GetBuildNumber()) << endl;
}

// This will call the Lookup function to process the inputs as well as generate the result codes
void MatchUpObjectGlobal::ExecuteObjectAndResultCodes(string inputFilePath, string outputFilePath)
{
  ifstream inFile;
  ofstream outFile;
  string record;
  vector<string> fields;

  long total = 0, dupes = 0;

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

  try
  {
    inFile.open(inputFilePath);
    outFile.open(outputFilePath);

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

    mdMatchUpObjGlobal->Process();

    outFile << "Id|ResultCodes|DupeGroup|Key" << endl;

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

  // ResultsCodes explain any issues MatchUp Object Global has with the object.
  // List of result codes for MatchUp Object Global
  // https://wiki.melissadata.com/index.php?title=Result_Code_Details#MatchUp_Object
}

string MatchUpObjectGlobal::simplifyWhitespace(const string& input) {
  static const regex multipleWhitespacePattern("\\s+");
  return regex_replace(input, multipleWhitespacePattern, " ");
}
