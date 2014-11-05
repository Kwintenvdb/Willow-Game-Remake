#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"
#include "Enemy.h"

//-----------------------------------------------------
// Soldier Class									
//-----------------------------------------------------
class Soldier: public Enemy
{
public:
	Soldier(DOUBLE2 pos);				// Constructor
	virtual ~Soldier();		// Destructor

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
	virtual void Explode();

	static const int STATE_WAIT = 0, STATE_RUN = 1, STATE_JUMP = 2, STATE_FALLDOWN = 3, STATE_DYING = 4;

private: 

	void Jump();

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	static Bitmap *m_BmpSoldierPtr, *m_BmpExplosionPtr;

	DOUBLE2 m_Pos, m_Velocity, m_Gravity;
	int m_CurrentState, m_MoveTick, m_Tickcount, m_Ticker, m_StartLine, m_ExplosionTicks;
	int m_Speed, m_Direction, m_Health;
	double m_JumpDistance;
	bool m_IsRunning, m_Jumping, m_IsDying, m_HasDied, m_HitsTheLevel, m_Exploding;
	HitRegion *m_HitSoldier;
	RECT2 m_HitRect;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Soldier(const Soldier& yRef);									
	Soldier& operator=(const Soldier& yRef);	
};

 
