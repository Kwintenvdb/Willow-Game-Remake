#pragma once

#include "GameEngine.h"
#include "Fireball.h"

class Enemy
{
public:
	virtual ~Enemy() {};

	virtual void Tick(double deltaTime) = 0;
	virtual void Paint(MATRIX3X2 matView) = 0;
	virtual DOUBLE2 GetPos() = 0;
	virtual bool HasDied() = 0;
	virtual void Hittest(HitRegion *levelPtr) = 0;
	virtual void GetHitRect(RECT2* hitRect, double* power) = 0;
	virtual bool HitByFireballs(Fireball *fireballPtr) = 0;
	virtual void MoveToWillow(DOUBLE2 willowPos) = 0;
	virtual void Spawn() = 0;
	virtual bool HasSpawned() = 0;
	virtual void Explode() = 0;
};

 
