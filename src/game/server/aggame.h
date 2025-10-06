//++ BulliT

#if !defined(AFX_AGGAMETYPES_H__B6D8EF5B_9423_4422_B935_1D71B6146DCA__INCLUDED_)
#define AFX_AGGAMETYPES_H__B6D8EF5B_9423_4422_B935_1D71B6146DCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "agglobal.h"

class AgGame
{
public:
	AgGame();
	virtual ~AgGame();

	AgString m_sCommandName;
	AgString m_sName;
	AgString m_sCfg;
	AgString m_sDescription;

	// This asterisk number is here to keep track of gamemode dirty state
	// If some gamemode cvar is changed, the name will have at least 1 asterisk,
	// and then another asterisk every 5 changed cvars, to know approximately
	// how "dirty" the gamemode is at this moment
	int m_iAsterisks;

	bool IsValid();
};

#endif // !defined(AFX_AGGAMETYPES_H__B6D8EF5B_9423_4422_B935_1D71B6146DCA__INCLUDED_)
//-- Martin Webrant
