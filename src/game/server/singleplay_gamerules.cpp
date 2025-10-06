/***
 *
 *	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/
//
// teamplay_gamerules.cpp
//
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "gamerules.h"
#include "skill.h"
#include "items.h"

//++ BulliT
#include "aggamerules.h"
#include "multiplay_gamerules.h"
#include "singleplay_gamerules.h"
#include "speedrunstats.h"

// extern DLL_GLOBAL CGameRules	*g_pGameRules;
extern DLL_GLOBAL AgGameRules *g_pGameRules;
//-- Martin Webrant
extern DLL_GLOBAL bool g_fGameOver;
extern int gmsgDeathMsg; // client dll messages
extern int gmsgScoreInfo;
extern int gmsgMOTD;

//=========================================================
//=========================================================
CHalfLifeRules::CHalfLifeRules(void)
{
	RefreshSkillData();
}

//=========================================================
//=========================================================
void CHalfLifeRules::Think(void)
{
}

//=========================================================
//=========================================================
bool CHalfLifeRules::IsMultiplayer(void)
{
	return false;
}

//=========================================================
//=========================================================
bool CHalfLifeRules::IsDeathmatch(void)
{
	return false;
}

//=========================================================
//=========================================================
bool CHalfLifeRules::IsCoOp(void)
{
	return false;
}

//=========================================================
//=========================================================
bool CHalfLifeRules::FShouldSwitchWeapon(CBasePlayer *pPlayer, CBasePlayerItem *pWeapon)
{
	/*
	if ( !pPlayer->m_pActiveItem )
	{
		// player doesn't have an active item!
		return true;
	}

	if ( !pPlayer->m_pActiveItem->CanHolster() )
	{
		return false;
	}

	return true;
	*/
	return AgGameRules::FShouldSwitchWeapon(pPlayer, pWeapon);
}

//=========================================================
//=========================================================
bool CHalfLifeRules ::GetNextBestWeapon(CBasePlayer *pPlayer, CBasePlayerItem *pCurrentWeapon)
{
	return false;
	// return AgGameRules::GetNextBestWeapon(pPlayer, pCurrentWeapon);
}

//=========================================================
//=========================================================
bool CHalfLifeRules ::ClientConnected(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128])
{
	return true;
}

void CHalfLifeRules ::InitHUD(CBasePlayer *pl)
{
}

//=========================================================
//=========================================================
void CHalfLifeRules ::ClientDisconnected(edict_t *pClient)
{
}

//=========================================================
//=========================================================
float CHalfLifeRules::FlPlayerFallDamage(CBasePlayer *pPlayer)
{
	/*
	// subtract off the speed at which a player is allowed to fall without being hurt,
	// so damage will be based on speed beyond that, not the entire fall
	pPlayer->m_flFallVelocity -= PLAYER_MAX_SAFE_FALL_SPEED;
	return pPlayer->m_flFallVelocity * DAMAGE_FOR_FALL_SPEED;
	*/
	return PLAYER_FALL_DAMAGE;
}

//=========================================================
//=========================================================
void CHalfLifeRules ::PlayerSpawn(CBasePlayer *pPlayer)
{
	bool addDefault;
	CBaseEntity *pWeaponEntity = NULL;

	pPlayer->pev->weapons |= (1 << WEAPON_SUIT);

	addDefault = true;

	while (pWeaponEntity = UTIL_FindEntityByClassname(pWeaponEntity, "game_player_equip"))
	{
		pWeaponEntity->Touch(pPlayer);
		addDefault = false;
	}

	if (addDefault)
	{
		pPlayer->pev->health = ag_start_health.value;
		pPlayer->pev->armorvalue = ag_start_armour.value;

		if (0 < ag_start_longjump.value)
		{
			pPlayer->m_fLongJump = true;
			g_engfuncs.pfnSetPhysicsKeyValue(pPlayer->edict(), "slj", "1");
			pPlayer->OnPickupLongjump();
		}
		if (0 < ag_start_glock.value)
			pPlayer->GiveNamedItem("weapon_9mmhandgun");
		if (0 < ag_start_crowbar.value)
			pPlayer->GiveNamedItem("weapon_crowbar");
		if (0 < ag_start_shotgun.value)
			pPlayer->GiveNamedItem("weapon_shotgun");
		if (0 < ag_start_mp5.value)
			pPlayer->GiveNamedItem("weapon_9mmAR");
		if (0 < ag_start_gauss.value)
			pPlayer->GiveNamedItem("weapon_gauss");
		if (0 < ag_start_hgrenade.value)
			pPlayer->GiveNamedItem("weapon_handgrenade");
		if (0 < ag_start_tripmine.value)
			pPlayer->GiveNamedItem("weapon_tripmine");
		if (0 < ag_start_egon.value)
			pPlayer->GiveNamedItem("weapon_egon");
		if (0 < ag_start_crossbow.value)
			pPlayer->GiveNamedItem("weapon_crossbow");
		if (0 < ag_start_357.value)
			pPlayer->GiveNamedItem("weapon_357");
		if (0 < ag_start_rpg.value)
			pPlayer->GiveNamedItem("weapon_rpg");
		if (0 < ag_start_satchel.value)
			pPlayer->GiveNamedItem("weapon_satchel");
		if (0 < ag_start_snark.value)
			pPlayer->GiveNamedItem("weapon_snark");
		if (0 < ag_start_hornet.value)
			pPlayer->GiveNamedItem("weapon_hornetgun");

		if (0 < ag_start_hgrenade.value)
			pPlayer->GiveAmmo(ag_start_hgrenade.value, "Hand Grenade", HANDGRENADE_MAX_CARRY);
		if (0 < ag_start_satchel.value)
			pPlayer->GiveAmmo(ag_start_satchel.value, "Satchel Charge", SATCHEL_MAX_CARRY);
		if (0 < ag_start_tripmine.value)
			pPlayer->GiveAmmo(ag_start_tripmine.value, "Trip Mine", TRIPMINE_MAX_CARRY);
		if (0 < ag_start_snark.value)
			pPlayer->GiveAmmo(ag_start_snark.value, "Snarks", SNARK_MAX_CARRY);
		if (0 < ag_start_hornet.value)
			pPlayer->GiveAmmo(ag_start_hornet.value, "Hornets", HORNET_MAX_CARRY);
		if (0 < ag_start_m203.value)
			pPlayer->GiveAmmo(ag_start_m203.value, "ARgrenades", M203_GRENADE_MAX_CARRY);
		if (0 < ag_start_uranium.value)
			pPlayer->GiveAmmo(ag_start_uranium.value, "uranium", URANIUM_MAX_CARRY);
		if (0 < ag_start_9mmar.value)
			pPlayer->GiveAmmo(ag_start_9mmar.value, "9mm", _9MM_MAX_CARRY);
		if (0 < ag_start_357ammo.value)
			pPlayer->GiveAmmo(ag_start_357ammo.value, "357", _357_MAX_CARRY);
		if (0 < ag_start_bockshot.value)
			pPlayer->GiveAmmo(ag_start_bockshot.value, "buckshot", BUCKSHOT_MAX_CARRY);
		if (0 < ag_start_bolts.value)
			pPlayer->GiveAmmo(ag_start_bolts.value, "bolts", BOLT_MAX_CARRY);
		if (0 < ag_start_rockets.value)
			pPlayer->GiveAmmo(ag_start_rockets.value, "rockets", ROCKET_MAX_CARRY);
	}
}

//=========================================================
//=========================================================
bool CHalfLifeRules ::AllowAutoTargetCrosshair(void)
{
	return (g_iSkillLevel == SKILL_EASY);
}

//=========================================================
//=========================================================
void CHalfLifeRules ::PlayerThink(CBasePlayer *pPlayer)
{
}

//=========================================================
//=========================================================
bool CHalfLifeRules ::FPlayerCanRespawn(CBasePlayer *pPlayer)
{
	return true;
}

//=========================================================
//=========================================================
float CHalfLifeRules ::FlPlayerSpawnTime(CBasePlayer *pPlayer)
{
	return gpGlobals->time; // now!
}

//=========================================================
// IPointsForKill - how many points awarded to anyone
// that kills this player?
//=========================================================
int CHalfLifeRules ::IPointsForKill(CBasePlayer *pAttacker, CBasePlayer *pKilled)
{
	return 1;
}

//=========================================================
// PlayerKilled - someone/something killed this player
//=========================================================
void CHalfLifeRules ::PlayerKilled(CBasePlayer *pVictim, entvars_t *pKiller, entvars_t *pInflictor)
{
}

//=========================================================
// Deathnotice
//=========================================================
void CHalfLifeRules::DeathNotice(CBasePlayer *pVictim, entvars_t *pKiller, entvars_t *pInflictor)
{
}

//=========================================================
// PlayerGotWeapon - player has grabbed a weapon that was
// sitting in the world
//=========================================================
void CHalfLifeRules ::PlayerGotWeapon(CBasePlayer *pPlayer, CBasePlayerItem *pWeapon)
{
}

//=========================================================
// FlWeaponRespawnTime - what is the time in the future
// at which this weapon may spawn?
//=========================================================
float CHalfLifeRules ::FlWeaponRespawnTime(CBasePlayerItem *pWeapon)
{
	// return -1;
	return gpGlobals->time + WEAPON_RESPAWN_TIME;
}

//=========================================================
// FlWeaponRespawnTime - Returns 0 if the weapon can respawn
// now,  otherwise it returns the time at which it can try
// to spawn again.
//=========================================================
float CHalfLifeRules ::FlWeaponTryRespawn(CBasePlayerItem *pWeapon)
{
	return 0;
}

//=========================================================
// VecWeaponRespawnSpot - where should this weapon spawn?
// Some game variations may choose to randomize spawn locations
//=========================================================
Vector CHalfLifeRules ::VecWeaponRespawnSpot(CBasePlayerItem *pWeapon)
{
	return pWeapon->pev->origin;
}

//=========================================================
// WeaponShouldRespawn - any conditions inhibiting the
// respawning of this weapon?
//=========================================================
int CHalfLifeRules ::WeaponShouldRespawn(CBasePlayerItem *pWeapon)
{
	// return GR_WEAPON_RESPAWN_NO;
	if (pWeapon->pev->spawnflags & SF_NORESPAWN)
	{
		return GR_WEAPON_RESPAWN_NO;
	}

	return GR_WEAPON_RESPAWN_YES;
}

//=========================================================
//=========================================================
bool CHalfLifeRules::CanHaveItem(CBasePlayer *pPlayer, CItem *pItem)
{
	return true;
}

//=========================================================
//=========================================================
void CHalfLifeRules::PlayerGotItem(CBasePlayer *pPlayer, CItem *pItem)
{
}

//=========================================================
//=========================================================
int CHalfLifeRules::ItemShouldRespawn(CItem *pItem)
{
	// return GR_ITEM_RESPAWN_NO;
	if (pItem->pev->spawnflags & SF_NORESPAWN)
	{
		return GR_ITEM_RESPAWN_NO;
	}

	return GR_ITEM_RESPAWN_YES;
}

//=========================================================
// At what time in the future may this Item respawn?
//=========================================================
float CHalfLifeRules::FlItemRespawnTime(CItem *pItem)
{
	// return -1;
	return gpGlobals->time + ITEM_RESPAWN_TIME;
}

//=========================================================
// Where should this item respawn?
// Some game variations may choose to randomize spawn locations
//=========================================================
Vector CHalfLifeRules::VecItemRespawnSpot(CItem *pItem)
{
	return pItem->pev->origin;
}

//=========================================================
//=========================================================
bool CHalfLifeRules::IsAllowedToSpawn(CBaseEntity *pEntity)
{
	return true;
	// return AgGameRules::IsAllowedToSpawn(pEntity);
}

//=========================================================
//=========================================================
void CHalfLifeRules::PlayerGotAmmo(CBasePlayer *pPlayer, char *szName, int iCount)
{
}

//=========================================================
//=========================================================
int CHalfLifeRules::AmmoShouldRespawn(CBasePlayerAmmo *pAmmo)
{
	// return GR_AMMO_RESPAWN_NO;
	if (pAmmo->pev->spawnflags & SF_NORESPAWN)
	{
		return GR_AMMO_RESPAWN_NO;
	}

	return GR_AMMO_RESPAWN_YES;
}

//=========================================================
//=========================================================
float CHalfLifeRules::FlAmmoRespawnTime(CBasePlayerAmmo *pAmmo)
{
	// return -1;
	return gpGlobals->time + AMMO_RESPAWN_TIME;
}

//=========================================================
//=========================================================
Vector CHalfLifeRules::VecAmmoRespawnSpot(CBasePlayerAmmo *pAmmo)
{
	return pAmmo->pev->origin;
}

//=========================================================
//=========================================================
float CHalfLifeRules::FlHealthChargerRechargeTime(void)
{
	// return 0;// don't recharge
	return HEALTHCHARGER_RECHARGE_TIME;
}

float CHalfLifeRules::FlHEVChargerRechargeTime(void)
{
	return HEVCHARGER_RECHARGE_TIME;
}

//=========================================================
//=========================================================
int CHalfLifeRules::DeadPlayerWeapons(CBasePlayer *pPlayer)
{
	return GR_PLR_DROP_GUN_NO;
}

//=========================================================
//=========================================================
int CHalfLifeRules::DeadPlayerAmmo(CBasePlayer *pPlayer)
{
	return GR_PLR_DROP_AMMO_NO;
}

//=========================================================
//=========================================================
int CHalfLifeRules::PlayerRelationship(CBaseEntity *pPlayer, CBaseEntity *pTarget)
{
	// why would a single player in half life need this?
	return GR_NOTTEAMMATE;
}

//=========================================================
//=========================================================
bool CHalfLifeRules ::FAllowMonsters(void)
{
	return true;
}

void CHalfLifeRules ::EndMultiplayerGame()
{
	SpeedrunStats::EndRun();
}
