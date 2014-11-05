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

//-----------------------------------------------------
// Necromancer Class									
//-----------------------------------------------------
class Necromancer: public Enemy
{
public:
	Necromancer(DOUBLE2 pos);				// Constructor
	virtual ~Necromancer();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------

	virtual void Tick(double deltaTime);
	virtual void Paint(MATRIX3X2 matView);
	virtual DOUBLE2 GetPos();
	virtual bool HasDied();
	virtual void Hittest(HitRegion *levelPtr){};
	virtual void GetHitRect(RECT2* hitRect, double* power);
	virtual bool HitByFireballs(Fireball *fireballPtr);
	virtual void MoveToWillow(DOUBLE2 willowPos);
	virtual void Spawn();
	virtual bool HasSpawned();
	virtual void Explode(){};

	static const int 
		 STATE_SPAWN = 0
		,STATE_ATTACK1 = 1
		,STATE_ATTACK2 = 2
		,STATE_DISAPPEAR = 3
		,STATE_INVISIBLE = 4
		,STATE_DYING = 5;

	static const int STARS = 6;

private: 
	
	void SpawnStar(MATRIX3X2 matView);
	void FireProjectile(MATRIX3X2 matView);

	static Bitmap *m_BmpNecroPtr, *m_BmpProjectilePtr, *m_BmpStarPtr;

	DOUBLE2 m_StartPos, m_Pos, m_WillowPos, m_ProjectilePos;
	RECT2 m_HitRect, m_AttackRect;

	int m_CurrentState, m_MoveTick, m_Ticker, m_StartLine, m_Direction, m_Health, m_ProjectileTick;
	int m_StarTick, m_ProjectileNumber;
	bool m_Visible, m_IsDying, m_HasDied, m_HasSpawned, m_GetWillowPos, m_FireProjectile;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Necromancer(const Necromancer& yRef);									
	Necromancer& operator=(const Necromancer& yRef);	
};

 
