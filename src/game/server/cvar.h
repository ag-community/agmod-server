#include <map>
#include <string>
#include <vector>

// CVar is potentially votable, if the corresponding CVars are enabled (sv_ag_allow_vote,
// sv_ag_vote_setting, etc.)
constexpr int CCVAR_VOTABLE = 1 << 0;

// CVar only requires sv_ag_vote_setting 1 to be votable, otherwise it would need 2
constexpr int CCVAR_BASIC_PRIVILEGES = 1 << 1;

// Players will always be notified if such a CVar gets changed to a value different than
// the default value
constexpr int CCVAR_GAMEMODE = 1 << 2;

namespace CVar
{
struct ChangedCVar
{
	std::string name;
	std::string defaultValue;
	std::string changedValue;
};

cvar_t Create(char *name, char *value, int flags = 0, int customFlags = 0);
void Load();

bool IsTracked(std::string cvarName);
bool IsVotable(std::string cvarName);
bool IsBasicSetting(std::string cvarName);

std::vector<ChangedCVar> GetChangesOverGamemode();
std::map<std::string, std::string> GetGamemodeCVars();

void StartRecordingChanges();
void StopRecordingStartupChanges();
void StopRecordingServerChanges();
void StopRecordingGamemodeChanges();

void RestoreAllToGamemodeValue();

void IgnoreLogging();
void RestoreLogging();
};
