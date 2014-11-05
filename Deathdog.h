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
#include "Enemy.h"
#include "Fireball.h"

//-----------------------------------------------------
// Deathdog Class									
//-----------------------------------------------------
class Deathdog: public Enemy
{
public:
	Deathdog(DOUBLE2 pos);				// Constructor
	virtual ~Deathdog();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------

	virtual void Tick(double deltaTime);
	virtual void Paint(MATRIX3X2 matView);
	virtual DOUBLE2 GetPos();
	virtual bool HasDied();
	virtual void Hittest(HitRegion *levelPtr);
	virtual void GetHitRect(RECT2* hitRect, double* power);
	virtual bool HitByFireballs(Fireball *fireballPtr);
	virtual void MoveToWillow(DOUBLE2 willowPos);
	virtual void Spawn();
	virtual bool HasSpawned();
	virtual void Explode() {}

	static const int STATE_TRANSFORM = 0, STATE_ATTACK = 1, STATE_DYING = 2;

private: 

	void ShootFireballs(MATRIX3X2 matView);

	Bitmap *m_BmpDeathDogPtr, *m_BmpDeathDogTransformPtr, *m_BmpFireballPtr;
	DOUBLE2 m_Pos, m_FireballPosArr[7];
	RECT2 m_HitRect, m_AttackRect;

	int m_MoveTick, m_Ticker, m_CurrentState, m_StartLine, m_FireballTick;
	bool m_Hit, m_HasDied, m_ResetFireball, m_IsFiring, m_GoToSecondAttack, m_HasSpawned;
	int m_Health, m_ElapsedDistance;
	double m_FireballLine;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Deathdog(const Deathdog& yRef);									
	Deathdog& operator=(const Deathdog& yRef);	
};

 
