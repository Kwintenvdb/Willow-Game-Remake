#pragma once
//-----------------------------------------------------
// Name:
// First name:
// Group: 1DAE.
//-----------------------------------------------------

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Fireball.h"

//-----------------------------------------------------
// Treasure Class									
//-----------------------------------------------------
class Treasure
{
public:
	Treasure(DOUBLE2 pos);				// Constructor
	virtual ~Treasure();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------

	void Paint(MATRIX3X2 matView);
	void Tick();
	bool Hittest(Fireball *fireballPtr);
	DOUBLE2 GetPos();
	bool HasOpened();
	void SetPos(DOUBLE2 newPos);

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	static Bitmap *m_BmpTreasurePtr;

	DOUBLE2 m_Pos;
	int m_Life;
	bool m_Open;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Treasure(const Treasure& yRef);									
	Treasure& operator=(const Treasure& yRef);	
};

 
