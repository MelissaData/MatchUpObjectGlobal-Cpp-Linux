# Melissa MatchUp Object Global Linux C++

R = .
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -g -pthread -m64
LDLIBS = -L/home/jessicasusilo/test/gitlab/linux/matchup/melissamatchupobjectgloballinuxcpp/MelissaMatchupObjectGlobalLinuxCpp/Build -Wl,--no-as-needed,-ldl,-rpath,$(DIR) -lmdMatchup -lmdGlobalParse
RM = rm -f
OUT_DIR = Build

all: MelissaMatchupObjectGlobalLinuxCpp

MelissaMatchupObjectGlobalLinuxCpp: Main.cpp MatchupObjectGlobal.cpp mdMatchup.h mdMatchupEnums.h | $(OUT_DIR)
				$(CXX) $(CXXFLAGS) Main.cpp MatchupObjectGlobal.cpp -o $(OUT_DIR)/$@ $(LDLIBS)

clean:
				$(RM) *.o $(OUT_DIR)/MelissaMatchupObjectGlobalLinuxCpp
