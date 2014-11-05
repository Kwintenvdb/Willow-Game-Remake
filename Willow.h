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
// Willow Class									
//-----------------------------------------------------
class Willow
{
public:
	Willow(int x, int y);				// Constructor
	virtual ~Willow();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------

	void Paint(MATRIX3X2 matView);
	void Tick(double deltaTime);
	void HitTest(HitRegion *levelPtr);
	void HitByEnemies(Enemy *enemyPtr);
	DOUBLE2 GetPos();
	DOUBLE2 GetDimensions();
	HitRegion* GetHitRegion();
	int GetHealth();
	int GetDirection();
	int GetFirePower();
	void SetRestrictedPos(DOUBLE2 area);
	void AddLifePoint(int amount);
	void AddShield();
	void IncreasePower(int amount);
	void Kill();

	static const int 
		 STATE_WAITING = 0
		,STATE_RUN = 1
		,STATE_SHOOT = 2
		,STATE_JUMP = 3
		,STATE_CLIMB = 4
		,STATE_CROUCH = 5
		,STATE_CROUCHJUMP = 6
		,STATE_JUMPDOWN = 7
		,STATE_ELECTROCUTED = 8
		,STATE_DYING = 9
		,STATE_CROUCHSHOOT = 10
		,STATE_JUMPSHOOT = 11;
	
	static const int COLUMNS = 7, ROWS = 9; 

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	void Jump();
	void Climb();
	void Respawn();

	DOUBLE2 m_Pos, m_RestrictedPos, m_Gravity, m_Velocity; 

	int m_MoveTick, m_Starcount, m_Tickcount, m_Direction;
	int m_CurrentState, m_StartLine;
	double m_Health;

	Bitmap *m_BmpHeroPtr, *m_BmpStarsPtr, *m_BmpHealthPtr;

	bool m_Shooting, m_InAir, m_Climbing, m_DisableHittest, m_CollidesWithLevel;

	double m_Acceleration, m_LevelIntersect, m_HitDistance;

	HitRegion *m_HitWillowV;
	RECT2 m_HitRect;
	bool m_Hit, m_Shield;
	int m_IncreasedPower;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Willow(const Willow& yRef);									
	Willow& operator=(const Willow& yRef);	
};

 
