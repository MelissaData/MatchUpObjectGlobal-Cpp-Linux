#!/bin/bash

# Name:    MelissaMatchUpObjectGlobalLinuxCpp
# Purpose: Use the MelissaUpdater to make the MelissaMatchUpObjectGlobalLinuxCpp code usable

######################### Constants ##########################

RED='\033[0;31m' #RED
NC='\033[0m' # No Color

######################### Parameters ##########################

global=""
us=""
license=""
quiet="false"

while [ $# -gt 0 ] ; do
    case $1 in
    --global) 
        global="$2"

        if [ "$global" == "--us" ] [ "$global" == "--license" ] || [ "$global" == "--quiet" ] || [ -z "$global" ];
        then
            printf "${RED}Error: Missing an argument for parameter \'global\'.${NC}\n"  
            exit 1
        fi  
        ;;
    --us) 
        us="$2"

        if [ "$us" == "--global" ] [ "$us" == "--license" ] || [ "$us" == "--quiet" ] || [ -z "$us" ];
        then
            printf "${RED}Error: Missing an argument for parameter \'us\'.${NC}\n"  
            exit 1
        fi  
        ;;
    --license) 
        license="$2"

        if [ "$license" == "--global" ] || [ "$license" == "--us" ] [ "$license" == "--quiet" ] || [ -z "$license" ];
        then
            printf "${RED}Error: Missing an argument for parameter \'license\'.${NC}\n"  
            exit 1
        fi    
        ;;
    --quiet) 
        quiet="true" 
        
        if [ "$quiet" == "--global" ] || [ "$quiet" == "--us" ] || [ "$quiet" == "--license" ] || [ -z "$quiet" ];
        then
            printf "${RED}Error: problem setting \'quiet\'.${NC}\n"  
            exit 1
        fi    
        ;;
    esac
    shift
done

######################### Config ###########################

RELEASE_VERSION='2024.Q1'
ProductName="GLOBAL_MU_DATA"

# Uses the location of the .sh file 
# Modify this if you want to use 
CurrentPath=$(pwd)
ProjectPath="$CurrentPath/MelissaMatchupObjectGlobalLinuxCpp"
BuildPath="$ProjectPath/Build"
DataPath="$ProjectPath/Data"

if [ ! -d $DataPath ];
then
    mkdir $DataPath
fi

if [ ! -d $BuildPath ];
then
    mkdir $BuildPath
fi

# Config variables for download file(s)
Config1_FileName="libmdMatchup.so"
Config1_ReleaseVersion=$RELEASE_VERSION
Config1_OS="LINUX"
Config1_Compiler="GCC48"
Config1_Architecture="64BIT"
Config1_Type="BINARY"

Config2_FileName="libmdGlobalParse.so"
Config2_ReleaseVersion=$RELEASE_VERSION
Config2_OS="LINUX"
Config2_Compiler="GCC48"
Config2_Architecture="64BIT"
Config2_Type="BINARY"

Config3_FileName="mdMatchup.h"
Config3_ReleaseVersion=$RELEASE_VERSION
Config3_OS="ANY"
Config3_Compiler="C"
Config3_Architecture="ANY"
Config3_Type="INTERFACE"

Config4_FileName="mdMatchupEnums.h"
Config4_ReleaseVersion=$RELEASE_VERSION
Config4_OS="ANY"
Config4_Compiler="C"
Config4_Architecture="ANY"
Config4_Type="INTERFACE"

# ######################## Functions #########################

DownloadDataFiles()
{
    printf "============================== MELISSA UPDATER ============================\n"
    printf "MELISSA UPDATER IS DOWNLOADING DATA FILE(S)...\n"

    ./MelissaUpdater/MelissaUpdater manifest -p $ProductName -r $RELEASE_VERSION -l $1 -t $DataPath 
    if [ $? -ne 0 ];
    then
        printf "\nCannot run Melissa Updater. Please check your license string!\n"
        exit 1
    fi     

    printf "Melissa Updater finished downloading data file(s)!\n"
}

DownloadSO() 
{
    printf "\nMELISSA UPDATER IS DOWNLOADING SO(S)...\n"
    
    # Check for quiet mode
    if [ $quiet == "true" ];
    then
        ./MelissaUpdater/MelissaUpdater file --filename $Config1_FileName --release_version $Config1_ReleaseVersion --license $1 --os $Config1_OS --compiler $Config1_Compiler --architecture $Config1_Architecture --type $Config1_Type --target_directory $BuildPath &> /dev/null     
        if [ $? -ne 0 ];
        then
            printf "\nCannot run Melissa Updater. Please check your license string!\n"
            exit 1
        fi

        printf "Melissa Updater finished downloading $Config1_FileName!\n"

        ./MelissaUpdater/MelissaUpdater file --filename $Config2_FileName --release_version $Config2_ReleaseVersion --license $1 --os $Config2_OS --compiler $Config2_Compiler --architecture $Config2_Architecture --type $Config2_Type --target_directory $BuildPath &> /dev/null
        if [ $? -ne 0 ];
        then
            printf "\nCannot run Melissa Updater. Please check your license string!\n"
            exit 1
        fi

        printf "Melissa Updater finished downloading $Config2_FileName!\n"

        ./MelissaUpdater/MelissaUpdater file --filename $Config3_FileName --release_version $Config3_ReleaseVersion --license $1 --os $Config3_OS --compiler $Config3_Compiler --architecture $Config3_Architecture --type $Config3_Type --target_directory $ProjectPath &> /dev/null    
        if [ $? -ne 0 ];
        then
            printf "\nCannot run Melissa Updater. Please check your license string!\n"
            exit 1
        fi

        printf "Melissa Updater finished downloading $Config3_FileName!\n"

        ./MelissaUpdater/MelissaUpdater file --filename $Config4_FileName --release_version $Config4_ReleaseVersion --license $1 --os $Config4_OS --compiler $Config4_Compiler --architecture $Config4_Architecture --type $Config4_Type --target_directory $ProjectPath &> /dev/null
        if [ $? -ne 0 ];
        then
            printf "\nCannot run Melissa Updater. Please check your license string!\n"
            exit 1
        fi

        printf "Melissa Updater finished downloading $Config4_FileName!\n"
    else
        ./MelissaUpdater/MelissaUpdater file --filename $Config1_FileName --release_version $Config1_ReleaseVersion --license $1 --os $Config1_OS --compiler $Config1_Compiler --architecture $Config1_Architecture --type $Config1_Type --target_directory $BuildPath    
        if [ $? -ne 0 ];
        then
            printf "\nCannot run Melissa Updater. Please check your license string!\n"
            exit 1
        fi

        printf "Melissa Updater finished downloading $Config1_FileName!\n"
   
        ./MelissaUpdater/MelissaUpdater file --filename $Config2_FileName --release_version $Config2_ReleaseVersion --license $1 --os $Config2_OS --compiler $Config2_Compiler --architecture $Config2_Architecture --type $Config2_Type --target_directory $BuildPath 
        if [ $? -ne 0 ];
        then
            printf "\nCannot run Melissa Updater. Please check your license string!\n"
            exit 1
        fi

        printf "Melissa Updater finished downloading $Config2_FileName!\n"

        ./MelissaUpdater/MelissaUpdater file --filename $Config3_FileName --release_version $Config3_ReleaseVersion --license $1 --os $Config3_OS --compiler $Config3_Compiler --architecture $Config3_Architecture --type $Config3_Type --target_directory $ProjectPath 
        if [ $? -ne 0 ];
        then
            printf "\nCannot run Melissa Updater. Please check your license string!\n"
            exit 1
        fi

        printf "Melissa Updater finished downloading $Config3_FileName!\n"

        ./MelissaUpdater/MelissaUpdater file --filename $Config4_FileName --release_version $Config4_ReleaseVersion --license $1 --os $Config4_OS --compiler $Config4_Compiler --architecture $Config4_Architecture --type $Config4_Type --target_directory $ProjectPath 
        if [ $? -ne 0 ];
        then
            printf "\nCannot run Melissa Updater. Please check your license string!\n"
            exit 1
        fi

        printf "Melissa Updater finished downloading $Config4_FileName!\n"
    fi
}

CheckSOs() 
{
    printf "\nDouble checking SO file(s) were downloaded...\n"
    FileMissing=0
    if [ ! -f $BuildPath/$Config1_FileName ];
    then
        printf "\n$Config1_FileName not found\n"
        FileMissing=1
    fi
    if [ ! -f $BuildPath/$Config2_FileName ];
    then
        printf "\n$Config2_FileName not found\n"
        FileMissing=1
    fi

    if [ $FileMissing -eq 1 ];
    then
        printf "\nMissing the above data file(s).  Please check that your license string and directory are correct."
        echo 0
    else
        echo 1
    fi
}

########################## Main ############################
printf "\n====================== Melissa MatchUp Object Global ======================\n                          [ C++ | Linux | 64BIT ]\n"

# Get license (either from parameters or user input)
if [ -z "$license" ];
then
    printf "Please enter your license string: "
    read license
fi

# Check for License from Environment Variables 
if [ -z "$license" ];
then
    license=`echo $MD_LICENSE` 
fi

if [ -z "$license" ];
then
    printf "\nLicense String is invalid!\n"
    exit 1
fi

# Use Melissa Updater to download data file(s) 
# Download data file(s) 
DownloadDataFiles $license      # comment out this line if using DQS Release

# Set data file(s) path
#DataPath=""      # uncomment this line and change to your DQS Release data file(s) directory 

#if [ ! -d $DataPath ]; # uncomment this section of code if you are using your own DQS Release data file(s) directory
#then
    #printf "\nData path is invalid!\n"
    #exit 1
#fi

# Download SO(s)
DownloadSO $license 

#Check if all SO(s) have been downloaded. Exit script if missing
SOsAreDownloaded=$(CheckSOs)

if echo "$SOsAreDownloaded" | grep -q "0";
then
   echo "$SOsAreDownloaded" | sed 's/0$//'
   printf "\nAborting program, see above.\n"
   exit 1
fi

printf "\nAll file(s) have been downloaded/updated!\n"

# Start program
# Build project
printf "\n=============================== BUILD PROJECT =============================\n"

# Setting the path to the lib in the makefile
LibPath=$(grep '^LDLIBS = -L[^ ]*' $ProjectPath/makefile | sed 's/ -Wl,.*//')
NewLibPath="LDLIBS = -L$ProjectPath/Build"

sed -i -e "s|$LibPath|$NewLibPath|g" $ProjectPath/makefile

# Generate the executable file
cd MelissaMatchupObjectGlobalLinuxCpp
make &> /dev/null
cd ..

# Export the path to the lib so that the executable knows where to look
export LD_LIBRARY_PATH=$BuildPath

# Run Project
if [ -z "$global" ] && [ -z "$us" ];
then
    cd MelissaMatchupObjectGlobalLinuxCpp
    $BuildPath/MelissaMatchupObjectGlobalLinuxCpp --license $license --dataPath $DataPath
    cd ..
else
    cd MelissaMatchupObjectGlobalLinuxCpp
    $BuildPath/MelissaMatchupObjectGlobalLinuxCpp --license $license --dataPath $DataPath --global "$global" --us "$us"
    cd ..
fi
