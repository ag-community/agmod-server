#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "game.h"

#include "speedrunstats.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

extern bool g_isUsingChangelevelTrigger;

namespace SpeedrunStats
{
namespace
{
std::vector<std::string> startingMaps;

long attempts;
long runs;

long sessionJumps;
long sessionDucks;
double sessionDistance;
double sessionDistance2D;
long sessionLoads;

long runJumps;
long runDucks;
double runDistance;
double runDistance2D;
long runLoads;

double runStartTime;

// Resets before you got to finish a run
// Initialized to -1 because the first map you load is not an actual reset
long runResets = -1;


void LoadStartingMaps()
{
	startingMaps.clear();

	startingMaps.push_back("c1a0"); // HL1 campaign
	startingMaps.push_back("t0a0"); // Hazard Course

	std::ifstream is(std::string(AgGetDirectory()) + "/startingmaps.txt");
	if (!is)
		return;

	std::string line;
	while (std::getline(is, line))
	{
		AgTrim(line);

		if (line.size() >= 2 && line.substr(0, 2) == "//")
			continue; // comment, ignore

		std::string mapName;
		if (UTIL_EndsWith(line, ";"))
			mapName = line.substr(0, line.size() - 1);
		else
			mapName = line;

		if (mapName.size() == 0)
			continue;

		if (std::count(startingMaps.begin(), startingMaps.end(), mapName) > 0)
			return; // don't insert if it already exists

		startingMaps.push_back(mapName);
	}

	AgConsole(UTIL_VarArgs("Loaded %d starting maps", startingMaps.size()));
}

void Clear()
{
	runJumps = 0L;
	runDucks = 0L;
	runDistance = 0.0;
	runDistance2D = 0.0;
	runStartTime = 0.0;
	runLoads = 0L;
}

void ResetRun()
{
	// This is mainly to avoid having 2 new attempts when it's really only 1 when starting
	// the run from the main menu, which loads c1a0 twice: the first one to start the game
	// dll and all its cvars, and set sv_singleplayer to 1, then the second one is so that
	// these cvars actually take effect
	if ((runStartTime + ATTEMPT_MIN_TIME) < AgTime())
	{
		attempts++;
		runResets++;
	}
	Clear();

	// runStartTime = gpGlobals->time;
	//  TODO: try to copy BXT way, get the realtime = sys_timescale.value * host_framerate.value,
	//  or try to do it with AgTime() but subtracting the load times
	runStartTime = AgTime();
}

bool IsStartingMap(std::string mapName)
{
	return std::count(startingMaps.begin(), startingMaps.end(), mapName) > 0;
}

bool IsRunning()
{
	return runStartTime != 0.0;
}
}

void Init()
{
	if (startingMaps.empty())
		LoadStartingMaps();

	// if (!IsStartingMap(STRING(gpGlobals->mapname)))
	//	return;

	if (g_isUsingChangelevelTrigger)
		return;

	ResetRun();

	attempts++;
}

void AddJump()
{
	sessionJumps++;
	runJumps++;
}

void AddDuck()
{
	sessionDucks++;
	runDucks++;
}

// I have to fiddle with includes to make Vector work, so I'll leave it as vec3_t for now
void AddDistance(float* dist)
{
	Vector vec = dist;

	const auto length = vec.Length();
	const auto length2D = vec.Length2D();

	sessionDistance += length;
	runDistance += length;

	sessionDistance2D += length2D;
	runDistance2D += length2D;
}

void AddLoad()
{
	sessionLoads++;
	runLoads++;
}

void PrintSession()
{
	std::string msg;
	msg.append("-------------------\n");
	msg.append("   Session Stats\n");
	msg.append("-------------------\n");
	msg.append(UTIL_VarArgs("Jumps: %ld\n", sessionJumps));
	msg.append(UTIL_VarArgs("Ducks: %ld\n", sessionDucks));
	msg.append(UTIL_VarArgs("Distance: %.2f\n", sessionDistance));
	msg.append(UTIL_VarArgs("Distance 2D: %.2f\n", sessionDistance2D));
	msg.append(UTIL_VarArgs("Loads: %ld\n", sessionLoads));
	msg.append(UTIL_VarArgs("Runs: %ld\n", runs));
	msg.append(UTIL_VarArgs("Attempts: %ld\n", attempts));
	msg.append("-------------------\n");

	AgConsole(msg, nullptr);
}

void PrintRun()
{
	double avgSpeed = 0.0;
	double avgSpeed2D = 0.0;

	const auto runTime = AgTime() - runStartTime;

	if (runTime > 0.0)
	{
		avgSpeed = runDistance / runTime;
		avgSpeed2D = runDistance2D / runTime;
	}

	const auto formattedTime = UTIL_GetFormattedTime(runTime);

	std::string msg;
	msg.append("-------------------\n");
	msg.append("     Run Stats\n");
	msg.append("-------------------\n");
	msg.append(UTIL_VarArgs("Jumps: %ld\n", runJumps));
	msg.append(UTIL_VarArgs("Ducks: %ld\n", runDucks));
	msg.append(UTIL_VarArgs("Distance: %.2f\n", runDistance));
	msg.append(UTIL_VarArgs("Distance 2D: %.2f\n", runDistance2D));
	msg.append(UTIL_VarArgs("Avg. Speed: %.2f\n", avgSpeed));
	msg.append(UTIL_VarArgs("Avg. Speed 2D: %.2f\n", avgSpeed2D));
	msg.append(UTIL_VarArgs("Loads: %ld\n", runLoads));
	msg.append(UTIL_VarArgs("Resets: %ld\n", runResets));
	msg.append(UTIL_VarArgs("Time w/ loads: %s\n", formattedTime.c_str()));
	msg.append("-------------------\n");

	AgConsole(msg, nullptr);
}

void ShowRunTime()
{
	// TODO: refactor similar code, in AgGameRules::GoToIntermission()
	const auto runTime = AgTime() - runStartTime;

	const auto formattedTime = UTIL_GetFormattedTime(runTime);

	AgString msg = UTIL_VarArgs("\nRun duration: %s\n", formattedTime.c_str());

	AgConsole(msg);
}

void EndRun()
{
	if (!IsRunning())
		return;

	runs++;

	ShowRunTime();
	PrintRun();

	Clear();
	runResets = -1;
}
}
