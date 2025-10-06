//++ BulliT

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "game.h"
#include "player.h"

#include "agtimer.h"
#include "agglobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DLL_GLOBAL bool g_bPaused = false;

extern int gmsgTimer;

AgTimer::AgTimer()
{
	m_fNextTimerUpdate = gpGlobals->time + 5.0; // Dont start timer directly.
	m_fLastTimeCheck = gpGlobals->time;
	m_fEffectiveTime = 0.0;
	m_pmp_timelimit = CVAR_GET_POINTER("mp_timelimit");
}

AgTimer::~AgTimer()
{
}

void AgTimer::Think()
{
	// Calculate effective time
	if (!g_bPaused)
		m_fEffectiveTime += gpGlobals->time - m_fLastTimeCheck;

	m_fLastTimeCheck = gpGlobals->time;

	if (m_fNextTimerUpdate <= m_fEffectiveTime)
	{
		// Sanity time check. Some dudes tends to put timelimit weird.
		if (timelimit.value > 2880)
			CVAR_SET_FLOAT("mp_timelimit", 2880); // Max two days.

		// Write the time. (negative turns off timer on client)
		MESSAGE_BEGIN(MSG_BROADCAST, gmsgTimer);
		WRITE_LONG((int)m_pmp_timelimit->value * 60); // Timelimit
		WRITE_LONG((int)m_fEffectiveTime); // How much time that has passed.
		MESSAGE_END();
		// Send next in 1 sec
		m_fNextTimerUpdate += 1;
	}
}

//-- Martin Webrant
