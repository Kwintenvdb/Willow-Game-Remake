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
// Brute Class									
//-----------------------------------------------------
class Brute: public Enemy
{
public:
	Brute(DOUBLE2 pos);				// Constructor
	virtual ~Brute();		// Destructor

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
	virtual void Spawn() {}
	virtual bool HasSpawned() {return true;}
	virtual void Explode(){};

	static const int STATE_WAIT = 0, STATE_ATTACK = 1, STATE_DYING = 2;

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	void ThrowAxe(MATRIX3X2 matView);

	static Bitmap* m_BmpBrutePtr, *m_BmpExplosionPtr;

	DOUBLE2 m_Pos, m_AxePos;
	RECT2 m_HitRect, m_AxeRect;
	int m_Tickcount, m_Ticker, m_AnimTick, m_StartLine, m_ExplosionTick;
	int m_CurrentState, m_Direction, m_Health;

	double m_Sin;
	bool m_AxeReturned, m_IsAttacking, m_HasDied;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Brute(const Brute& yRef);									
	Brute& operator=(const Brute& yRef);	
};

 
