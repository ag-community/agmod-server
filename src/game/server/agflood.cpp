#include <algorithm>
#include <string>
#include <vector>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "agflood.h"

namespace AgFlood
{
namespace
{
	struct Flooder
	{
		std::string steamID;
		double timeSinceLastNameChange;
		double timeSinceLastModelChange;

		std::string enforcedModel;
		double timeSinceLastModelEnforcement;
		double timeSinceLastSpecEnforcement;

		Flooder(const char *steamID, double timeSinceLastNameChange, double timeSinceLastModelChange,
		    const char *enforcedModel, double timeSinceLastModelEnforcement, double timeSinceLastSpecEnforcement)
		    : steamID(steamID)
		    , timeSinceLastNameChange(timeSinceLastNameChange)
		    , timeSinceLastModelChange(timeSinceLastModelChange)
		    , enforcedModel(enforcedModel)
		    , timeSinceLastModelEnforcement(timeSinceLastModelEnforcement)
		    , timeSinceLastSpecEnforcement(timeSinceLastSpecEnforcement)
		{
		}
	};

	std::vector<Flooder> flooders;
}

void UpdateFlooding(CBasePlayer *player)
{
	const char *playerSteamID = player->GetAuthID();

	const auto timePassedNameChange = AgTime() - player->m_flLastNameChange;
	const auto timePassedModelChange = AgTime() - player->m_flLastModelChange;

	const auto timePassedModelEnforcement = AgTime() - player->m_flLastModelEnforcement;
	const auto timePassedSpecEnforcement = AgTime() - player->m_flLastSpecEnforcement;

	const char *enforcedModel = player->m_enforcedModel.c_str();

	const auto it = std::find_if(
	    flooders.begin(),
	    flooders.end(),
	    [=](const Flooder &entry)
	    {
		    return !strcmp(entry.steamID.c_str(), playerSteamID);
	    });

	if (it != flooders.end())
	{
		it->timeSinceLastNameChange = timePassedNameChange;
		it->timeSinceLastModelChange = timePassedModelChange;

		it->enforcedModel = enforcedModel;
		it->timeSinceLastModelEnforcement = timePassedModelEnforcement;
		it->timeSinceLastSpecEnforcement = timePassedSpecEnforcement;
	}
	else
		flooders.emplace_back(
		    playerSteamID, timePassedNameChange, timePassedModelChange,
		    enforcedModel, timePassedModelEnforcement, timePassedSpecEnforcement);
}

void RestoreFlooding(CBasePlayer *player)
{
	const auto it = std::find_if(
	    flooders.begin(),
	    flooders.end(),
	    [=](const Flooder &entry)
	    {
		    return !strcmp(entry.steamID.c_str(), player->GetAuthID());
	    });

	if (it != flooders.end())
	{
		player->m_flLastNameChange = AgTime() - it->timeSinceLastNameChange;
		player->m_flLastModelChange = AgTime() - it->timeSinceLastModelChange;

		player->m_enforcedModel = it->enforcedModel;
		player->m_flLastModelEnforcement = AgTime() - it->timeSinceLastModelEnforcement;
		player->m_flLastSpecEnforcement = AgTime() - it->timeSinceLastSpecEnforcement;
	}
}
}
