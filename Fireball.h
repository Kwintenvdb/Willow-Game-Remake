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
// Fireball Class									
//-----------------------------------------------------
class Fireball
{
public:
	Fireball();				// Constructor
	virtual ~Fireball();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------

	void Paint(MATRIX3X2 matView);
	void Tick(double deltaTime);
	bool Hittest(RECT2 r1);
	void Fire(DOUBLE2 startPos, int direction, int firePower);
	bool WasFired();
	void SetHit(bool hit);
	bool HasExploded();
	DOUBLE2 GetPos();
	RECT2 GetHitRect();
	int GetFirePower();
	void SetFirePower(int power);

	static const int START_FIRE = 3;
	static const int FIRE_TICKS = 17;
	static const int ALL_TICKS = 26;

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	static Bitmap *m_BmpFireBallPtr;

	DOUBLE2 m_Pos;
	int m_Tickcount, m_Speed, m_ElapsedDistance, m_Direction, m_FirePower, m_Ticker;
	RECT2 m_HitRect;
	bool m_WasFired, m_Hit;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Fireball(const Fireball& yRef);									
	Fireball& operator=(const Fireball& yRef);	
};

 
