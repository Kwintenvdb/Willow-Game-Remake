//---------------------------
// Includes
//---------------------------
#include "Deathdog.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------
Deathdog::Deathdog(DOUBLE2 pos):
	m_Pos(pos)
	,m_MoveTick()
	,m_Ticker()
	,m_CurrentState()
	,m_StartLine()
	,m_FireballTick()
	,m_FireballLine()
	,m_Hit(false)
	,m_HasDied(false)
	,m_Health(250)
	,m_BmpDeathDogPtr(nullptr)
	,m_BmpDeathDogTransformPtr(nullptr)
	,m_BmpFireballPtr(nullptr)
	,m_ResetFireball(true)
	,m_IsFiring(false)
	,m_HasSpawned(false)
	,m_GoToSecondAttack(false)
	,m_ElapsedDistance()
{
	m_BmpDeathDogPtr = new Bitmap("Resources/deathdogattack.bmp");
	m_BmpDeathDogPtr->SetTransparencyColor(255,0,255);

	m_BmpDeathDogTransformPtr = new Bitmap("Resources/deathdogtransform.bmp");
	m_BmpDeathDogTransformPtr->SetTransparencyColor(255,0,255);

	m_BmpFireballPtr = new Bitmap("Resources/fireball_dog.bmp");
	m_BmpFireballPtr->SetTransparencyColor(255,0,255);

	for(int i = 0; i < 7; ++i)
		m_FireballPosArr[i] = DOUBLE2((m_Pos.x - 25) + i * 25, m_Pos.y);
}

Deathdog::~Deathdog()
{
	delete m_BmpDeathDogPtr; delete m_BmpDeathDogTransformPtr; delete m_BmpFireballPtr;
}

void Deathdog::Paint(MATRIX3X2 matView)
{
	if(m_CurrentState == STATE_ATTACK)
		ShootFireballs(matView);

	MATRIX3X2 matPos;
	matPos.SetAsTranslate(m_Pos);
	GAME_ENGINE->SetTransformMatrix(matPos * matView);

	int width, height;

	RECT clip;

	if(m_CurrentState == STATE_TRANSFORM)
	{
		width = m_BmpDeathDogTransformPtr->GetWidth()/11;
		height = m_BmpDeathDogTransformPtr->GetHeight();

		clip.left = width * m_MoveTick;
		clip.right = clip.left + width;
		clip.top = 0;
		clip.bottom = height;

		GAME_ENGINE->DrawBitmap(m_BmpDeathDogTransformPtr,0,0,clip);
	}
	else
	{
		width = m_BmpDeathDogPtr->GetWidth()/7;
		height = m_BmpDeathDogPtr->GetHeight()/6;

		if(!m_Hit)
		{
			clip.left = width * m_MoveTick;
			clip.right = clip.left + width;
			clip.top = height * m_StartLine;
			clip.bottom = clip.top + height;

			GAME_ENGINE->DrawBitmap(m_BmpDeathDogPtr,0,0,clip);
		}
		else
		{
			clip.left = width * m_MoveTick;
			clip.right = clip.left + width;
			clip.top = height * (m_StartLine + 3);
			clip.bottom = clip.top + height;

			GAME_ENGINE->DrawBitmap(m_BmpDeathDogPtr,0,0,clip);
		}
	}

	m_Hit = false;
}

void Deathdog::Tick(double deltaTime)
{
	if(GAME_ENGINE->IsKeyDown(VK_RSHIFT)) m_CurrentState = STATE_DYING;
	//else m_CurrentState = STATE_ATTACK;

	if(m_Health <= 0) m_CurrentState = STATE_DYING;

	++m_Ticker;
	if(m_Ticker > 10)
	{
	switch(m_CurrentState)
	{
	case STATE_TRANSFORM:

		++m_MoveTick;
		if(m_MoveTick == 10)
		{
			m_MoveTick = 0;
			m_CurrentState = STATE_ATTACK;
		}
		break;

	case STATE_ATTACK:

		++m_MoveTick;
		if(m_StartLine < 2)
		{
			if(m_MoveTick == 7)
			{
				m_MoveTick = 0;
				++m_StartLine;
				if(!m_GoToSecondAttack && m_StartLine == 2) m_StartLine = 0;
			}

			if(m_StartLine == 1 && m_MoveTick > 3 && m_ResetFireball && !m_IsFiring)
			{
				m_MoveTick = 3;
				m_ResetFireball = false;

				for(int i = 0; i < 7; ++i)
					m_FireballPosArr[i].y = m_Pos.y + 60;
			}
		}
		else if(m_StartLine == 2)
		{
			if(m_MoveTick == 4)
			{
				m_MoveTick = 0;
				m_StartLine = 0;
			}

			if(m_MoveTick > 2 && m_ResetFireball)
			{
				m_MoveTick = 2;
				m_ResetFireball = false;
				m_GoToSecondAttack = false;

				for(int i = 0; i < 7; ++i)
					m_FireballPosArr[i].y = m_Pos.y + 15;
			}
		}

		//m_ResetFireball = false;
		m_FireballLine += 0.5;
		if(m_FireballLine >= 5) m_FireballLine = 0;
		break;

	case STATE_DYING:

		m_StartLine = 2;
		m_MoveTick = 2;
		m_HasDied = true;
		break;
	}
	m_Ticker = 0;
	}

	// Hitregion

	m_HitRect.left = m_Pos.x + 45;
	m_HitRect.right = m_HitRect.left + 20;
	m_HitRect.top = m_Pos.y;
	m_HitRect.bottom = m_HitRect.top + 150;

	m_AttackRect.left = m_FireballPosArr[0].x;
	m_AttackRect.right = m_FireballPosArr[6].x + 30;
	m_AttackRect.top = m_FireballPosArr[0].y + 30;
	m_AttackRect.bottom = m_AttackRect.top + 25;
}

DOUBLE2 Deathdog::GetPos()
{
	return m_Pos;
}

bool Deathdog::HasDied()
{
	return m_HasDied;
}

void Deathdog::Hittest(HitRegion *levelPtr)
{

}

void Deathdog::GetHitRect(RECT2* hitRect, double* power)
{
	if(m_IsFiring) *hitRect = m_AttackRect;
	*power = 7.5;
}

bool Deathdog::HitByFireballs(Fireball *fireballPtr)
{
	bool hit = fireballPtr->Hittest(m_HitRect);
	m_Hit = hit;
	if(hit) m_Health -= fireballPtr->GetFirePower();
	return hit;
}

void Deathdog::ShootFireballs(MATRIX3X2 matView)
{
	int width = m_BmpFireballPtr->GetWidth()/7;
	int height = m_BmpFireballPtr->GetHeight()/4;

	int distance = 150, speed = 3;

	int clipStart(0);

	if(!m_ResetFireball)
	{
		for(int i = 0; i < 7; ++i)
		{
			clipStart = i;

			RECT clip;
			clip.left = width * clipStart;
			clip.right = clip.left + width;
			clip.top = height * (int) m_FireballLine;
			clip.bottom = clip.top + height;

			MATRIX3X2 matPos; matPos.SetAsTranslate(m_FireballPosArr[i]);

			GAME_ENGINE->SetTransformMatrix(matPos * matView);
			GAME_ENGINE->DrawBitmap(m_BmpFireballPtr,0,0,clip);

			double displacement = i;
			displacement -= 3.5;
			displacement /= 4.5;
			
			m_FireballPosArr[i].x -= speed;
			m_FireballPosArr[i].y -= displacement*displacement;
		}

		m_ElapsedDistance += speed;
		if(m_ElapsedDistance < distance)
			m_IsFiring = true;
	}

	if(m_ElapsedDistance >= distance)
	{
		m_ResetFireball = true;
		m_IsFiring = false;
		m_ElapsedDistance = 0;
	}

	if(m_ResetFireball)
	{
		for(int i = 0; i < 7; ++i)
			m_FireballPosArr[i] = DOUBLE2((m_Pos.x-25) + i * 25, m_Pos.y);
		m_FireballLine = 0;
	}
}

void Deathdog::MoveToWillow(DOUBLE2 willowPos)
{
	bool inRange = m_Pos.x - willowPos.x < 150;

	if(inRange && willowPos.y < 620)	// checks if Willow has jumped up
		m_GoToSecondAttack = true;
}

void Deathdog::Spawn()
{
	m_HasSpawned = true;
}

bool Deathdog::HasSpawned()
{
	return m_HasSpawned;
}
