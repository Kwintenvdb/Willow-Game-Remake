//---------------------------
// Includes
//---------------------------
#include "Necromancer.h"
//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap *Necromancer::m_BmpNecroPtr(nullptr);
Bitmap *Necromancer::m_BmpProjectilePtr(nullptr);
Bitmap *Necromancer::m_BmpStarPtr(nullptr);

//---------------------------
// Constructor & Destructor
//---------------------------
Necromancer::Necromancer(DOUBLE2 pos):
	 m_StartPos(pos) // holds the original position
	,m_Pos(pos)
	,m_CurrentState()
	,m_MoveTick()
	,m_Ticker()
	,m_StartLine()
	,m_Direction(1)
	,m_Visible(true)
	,m_IsDying(false)
	,m_HasDied(false)
	,m_HasSpawned(false)
	,m_GetWillowPos(false)
	,m_Health(150)
	,m_ProjectilePos(pos)
	,m_ProjectileTick()
	,m_FireProjectile(false)
	,m_StarTick()
	,m_ProjectileNumber()
{
	if(m_BmpNecroPtr == nullptr)
		m_BmpNecroPtr = new Bitmap("Resources/necromancer.bmp");
	if(m_BmpNecroPtr->Exists())
		m_BmpNecroPtr->SetTransparencyColor(255,0,255);

	if(m_BmpProjectilePtr == nullptr)
		m_BmpProjectilePtr = new Bitmap("Resources/projectile.png");

	if(m_BmpStarPtr == nullptr)
		m_BmpStarPtr = new Bitmap("Resources/necrostar.png");
}

Necromancer::~Necromancer()
{
	
}

void Necromancer::Tick(double deltaTime)
{
	if(m_Health <= 0)
	{
		if(!m_IsDying) m_MoveTick = 0;
		m_CurrentState = STATE_DYING;
		m_IsDying = true;
	}

	++m_Ticker;

	if(m_Ticker > 6)
	{
	switch(m_CurrentState)
	{
	case STATE_SPAWN:

		m_StartLine = 0;
		if(m_MoveTick < 4) ++m_MoveTick;
		else
		{
			m_StartLine = 1;
			m_MoveTick = 0;
			m_CurrentState = STATE_ATTACK1;
		}
		break;

	case STATE_ATTACK1:

		m_Visible = true;

		m_StartLine = 1;
		++m_MoveTick;
		if(m_MoveTick == 6)
		{
			m_StartLine = 3;
			m_MoveTick = 0;
			m_GetWillowPos = true;
			m_CurrentState = STATE_DISAPPEAR;
		}
		break;

	case STATE_DISAPPEAR:

		m_GetWillowPos = false;
		m_FireProjectile = true;
		m_StartLine = 3;
		++m_MoveTick;
		if(m_MoveTick == 6)
		{
			m_MoveTick = 0;
			m_Visible = false;
			m_CurrentState = STATE_INVISIBLE;
		}
		break;

	case STATE_INVISIBLE:

		++m_MoveTick;
		if(m_MoveTick == 10)
		{
			int random = rand()%3;
			if(random == 0)
				m_Pos = DOUBLE2(m_WillowPos.x + 75 * m_Direction, m_WillowPos.y - 25);
			else m_Pos = DOUBLE2(m_StartPos.x + (rand()%150 - 75), m_StartPos.y + (rand()%100));	// random position

			m_MoveTick = 0;

			if(m_ProjectileNumber == 0) m_ProjectileNumber = 1;
			else m_ProjectileNumber = 0;

			m_CurrentState = STATE_ATTACK1;
		}
		break;

	case STATE_DYING:

		++m_MoveTick;
		if(m_MoveTick == 3)
		{
			m_HasDied = true;
		}
		break;
	}
	++m_ProjectileTick;
	if(m_ProjectileTick == 6) m_ProjectileTick = 0;

	m_Ticker = 0;
	}

	if(m_ProjectileNumber == 0 && m_ProjectileTick == 2) m_ProjectileTick = 0;
	else if(m_ProjectileNumber == 1 && m_ProjectileTick == 6) m_ProjectileTick = 0;

	if(m_CurrentState == STATE_SPAWN) m_Pos.y += 1.25;

	// Hitrect

	int width = m_BmpNecroPtr->GetWidth()/6;
	int height = m_BmpNecroPtr->GetHeight()/4;

	m_HitRect.left = m_Pos.x + 10;
	m_HitRect.right = m_HitRect.left + width - 20;
	m_HitRect.top = m_Pos.y + 10;
	m_HitRect.bottom = m_HitRect.top + height - 20;

	m_AttackRect.left = m_ProjectilePos.x + 5;
	m_AttackRect.right = m_AttackRect.left + 25;
	m_AttackRect.top = m_ProjectilePos.y;
	m_AttackRect.bottom = m_AttackRect.top + 15;
}

void Necromancer::Paint(MATRIX3X2 matView)
{
	int width = m_BmpNecroPtr->GetWidth()/6;
	int height = m_BmpNecroPtr->GetHeight()/4;

	MATRIX3X2 matPos, matDirection;

	if(m_Direction == 1)
		matPos.SetAsTranslate(m_Pos);
	else matPos.SetAsTranslate(m_Pos.x + width, m_Pos.y);
	matDirection.SetAsScale(m_Direction,1);

	GAME_ENGINE->SetTransformMatrix(matDirection * matPos * matView);

	RECT clip;

		clip.left = width * m_MoveTick;
		clip.right = clip.left + width;
		clip.top = height * m_StartLine;
		clip.bottom = clip.top + height;

	if(m_Visible)
		GAME_ENGINE->DrawBitmap(m_BmpNecroPtr,0,0,clip);

	FireProjectile(matView);
	if(m_CurrentState == STATE_SPAWN)
		 SpawnStar(matView);

//	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
//	GAME_ENGINE->DrawString(String("projectile number: ") + m_ProjectileNumber
//		+ "\nprojectile tick: " + m_ProjectileTick,200,15);
}

DOUBLE2 Necromancer::GetPos()
{
	return m_Pos;
}

bool Necromancer::HasDied()
{
	return m_HasDied;
}

void Necromancer::GetHitRect(RECT2* hitRect, double* power)
{
	if(m_CurrentState != STATE_INVISIBLE) *hitRect = m_AttackRect;
	*power = 50;
}

bool Necromancer::HitByFireballs(Fireball *fireballPtr)
{
	if(m_Visible)
	{
		bool hit = fireballPtr->Hittest(m_HitRect);
		if(hit) m_Health -= fireballPtr->GetFirePower();
		return hit;
	}
	else return false;
}

void Necromancer::MoveToWillow(DOUBLE2 willowPos)
{
	if(m_GetWillowPos) m_WillowPos = willowPos;

	if(willowPos.x > m_Pos.x) m_Direction = -1;
	else m_Direction = 1;
}

void Necromancer::Spawn()
{
	m_HasSpawned = true;
}

bool Necromancer::HasSpawned()
{
	return m_HasSpawned;
}

void Necromancer::SpawnStar(MATRIX3X2 matView)
{
	// Purely cosmetic, this function paints some star sprites when the Necromancer spawns

	int width = m_BmpStarPtr->GetWidth()/4;
	int height = m_BmpStarPtr->GetHeight();

	DOUBLE2 starPosArr[STARS];

	starPosArr[0] = DOUBLE2(m_Pos.x + 25, m_Pos.y + 25);
	starPosArr[1] = DOUBLE2(m_Pos.x - 25, m_Pos.y + 35);
	starPosArr[2] = DOUBLE2(m_Pos.x + 55, m_Pos.y + 40);
	starPosArr[3] = DOUBLE2(m_Pos.x + 50, m_Pos.y - 10);
	starPosArr[4] = DOUBLE2(m_Pos.x + 30, m_Pos.y + 50);
	starPosArr[5] = DOUBLE2(m_Pos.x - 5, m_Pos.y - 5);

	if(m_Ticker == 0)
	{
		++m_StarTick;
		if(m_StarTick == 5) m_StarTick = 0;
	}

	for(int i = 0; i < STARS; ++i)
	{
		RECT clip;
		clip.left = width * m_StarTick;
		clip.right = clip.left + width;
		clip.top = 0;
		clip.bottom = height;

		MATRIX3X2 matPos;
		matPos.SetAsTranslate(starPosArr[i]);

		GAME_ENGINE->SetTransformMatrix(matPos * matView);
		GAME_ENGINE->DrawBitmap(m_BmpStarPtr,0,0,clip);
	}
}

void Necromancer::FireProjectile(MATRIX3X2 matView)
{
	int clipwidth = m_BmpProjectilePtr->GetWidth()/6;
	int clipheight = m_BmpProjectilePtr->GetHeight()/2;

	RECT clip;
	clip.left = clipwidth * m_ProjectileTick;
	clip.right = clip.left + clipwidth;
	clip.top = clipheight * m_ProjectileNumber;
	clip.bottom = clip.top + clipheight;

	double projectileSpeed(5);
	int maxDistance(275);

	// direction vector for the projectile
	DOUBLE2 projectileVector = m_WillowPos - m_Pos;
	projectileVector = projectileVector.Normalized();

	// the projectile moves along the vector
	if(m_FireProjectile) m_ProjectilePos += projectileVector * projectileSpeed;
	else m_ProjectilePos = m_Pos;

	// if the projectile has traveled 275 pixels, get ready to fire new one
	DOUBLE2 distance = m_ProjectilePos - m_Pos;
	if(distance.Length() >= maxDistance) m_FireProjectile = false;

	double angle = m_Pos.AngleWith(projectileVector);

	MATRIX3X2 matRotate, matPos, matCenter;
	matCenter.SetAsTranslate(-clipwidth/2,-clipheight/2);
	matPos.SetAsTranslate(m_ProjectilePos);
	matRotate.SetAsRotate(angle);

	GAME_ENGINE->SetTransformMatrix(matCenter * matRotate * matCenter.Inverse() * matPos * matView);
	if(m_FireProjectile) GAME_ENGINE->DrawBitmap(m_BmpProjectilePtr,0,0,clip);
}