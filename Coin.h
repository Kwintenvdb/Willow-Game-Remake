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

//-----------------------------------------------------
// Coin Class									
//-----------------------------------------------------
class Coin
{
public:
	Coin();				// Constructor
	virtual ~Coin();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------

	void Paint(MATRIX3X2 matView);
	void Tick(double deltaTime);
	DOUBLE2 GetPos();
	bool Hittest(DOUBLE2 willowPos);
	void HitLevel(HitRegion *levelPtr);
	void Spawn(DOUBLE2 pos);
	bool HasSpawned();

private: 

	void Bounce();

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static Bitmap *m_BmpCoinPtr;

	DOUBLE2 m_Pos, m_Gravity, m_Velocity;
	HitRegion *m_HitCoinPtr;
	int m_MoveTick, m_Ticker, m_NrOfBounces;
	bool m_HasSpawned;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Coin(const Coin& yRef);									
	Coin& operator=(const Coin& yRef);	
};

 
