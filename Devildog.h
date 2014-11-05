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
// Devildog Class									
//-----------------------------------------------------
class Devildog: public Enemy
{
public:
	Devildog(DOUBLE2 pos, DOUBLE2 roamArea);				// Constructor
	virtual ~Devildog();		// Destructor

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

	static const int 
		 STATE_LEAVE = 0
		,STATE_ROAM = 1
		,STATE_TURN = 2
		,STATE_ATTACK = 3
		,STATE_DYING = 4
		,STATE_PUSH = 5;

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	void SetRoamArea(bool turning);

	static Bitmap *m_BmpDevildogPtr;

	DOUBLE2 m_Pos, m_Roam, m_Velocity, m_Gravity;
	int m_CurrentState, m_StartLine, m_MoveTick, m_Ticker;
	int m_Direction, m_Health;
	bool m_HasDied, m_IsDying, m_Hit;

	HitRegion *m_HitDogH;
	RECT2 m_HitRect;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Devildog(const Devildog& yRef);									
	Devildog& operator=(const Devildog& yRef);	
};

 
