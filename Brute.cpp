//---------------------------
// Includes
//---------------------------
#include "Brute.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------

Bitmap* Brute::m_BmpBrutePtr(nullptr);
Bitmap* Brute::m_BmpExplosionPtr(nullptr);

Brute::Brute(DOUBLE2 pos): 
	m_Pos(pos)
	,m_AxePos(pos.x + 50, pos.y + 15)
	,m_Tickcount()
	,m_Ticker()
	,m_AnimTick()
	,m_StartLine()
	,m_CurrentState()
	,m_ExplosionTick()
	,m_Sin()
	,m_AxeReturned(false)
	,m_IsAttacking(false)
	,m_HasDied(false)
	,m_Direction(1)
	,m_Health(60)
{
	if(m_BmpBrutePtr == nullptr)
		m_BmpBrutePtr = new Bitmap("Resources/Brute.bmp");
	if(m_BmpBrutePtr->Exists())
		m_BmpBrutePtr->SetTransparencyColor(255,0,255);

	if(m_BmpExplosionPtr == nullptr)
		m_BmpExplosionPtr = new Bitmap("Resources/explosion.png");
	if(m_BmpBrutePtr->Exists())
		m_BmpExplosionPtr->SetTransparencyColor(255,0,255);
}

Brute::~Brute()
{
	
}

void Brute::Paint(MATRIX3X2 matView)
{
	//GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
	//GAME_ENGINE->DrawString(String("Axe pos: ") + m_AxePos.ToString(),150,15);

	int width = m_BmpBrutePtr->GetWidth()/4;
	int height = m_BmpBrutePtr->GetHeight()/2;

	MATRIX3X2 matPos, matDirection;
	matDirection.SetAsScale(m_Direction,1);
	if(m_Direction == 1)
		matPos.SetAsTranslate(m_Pos);
	else
		matPos.SetAsTranslate(m_Pos.x + width, m_Pos.y);

	GAME_ENGINE->SetTransformMatrix(matDirection * matPos * matView);

	RECT clip;
		clip.left = width * m_AnimTick;
		clip.right = clip.left + width;
		clip.top = height * m_StartLine;
		clip.bottom = clip.top + height;

	GAME_ENGINE->DrawBitmap(m_BmpBrutePtr,0,0,clip);

	if(m_CurrentState == STATE_ATTACK)
	{
		ThrowAxe(matView);
	}

	int width2 = m_BmpExplosionPtr->GetWidth()/8;

	RECT clip2;
	clip2.left = width2 * m_ExplosionTick;
	clip2.right = clip2.left + width2;
	clip2.top = 0;
	clip2.bottom = m_BmpExplosionPtr->GetHeight();

	if(m_CurrentState == STATE_DYING)
	{
		GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(m_Pos.x + 5,m_Pos.y) * matView);
		GAME_ENGINE->DrawBitmap(m_BmpExplosionPtr,0,0,clip2);
	}
}

void Brute::Tick(double deltaTime)
{
	if(m_IsAttacking) m_CurrentState = STATE_ATTACK;
	if(m_Health <= 0) m_CurrentState = STATE_DYING;

	++m_Ticker;
	if(m_Ticker > 6)
	{
	switch(m_CurrentState)
	{
	case STATE_WAIT:

		m_StartLine = 0;

		++m_Tickcount;
		if(m_Tickcount % 10 == 0)
		{
			++m_AnimTick %= 3;
			m_Tickcount = 0;
		}

		if(m_AxeReturned)
		{
			m_AxeReturned = false;	// resets the axereturned boolean
			m_AxePos.x = m_Pos.x + 50;	// resets axepos to default value
		}
		break;

	case STATE_ATTACK:

		m_StartLine = 1;

		//m_IsAttacking = true;

		if(m_AnimTick < 2)
			++m_AnimTick;

		if(m_AxeReturned)
		{
			m_AnimTick = 0;
			m_IsAttacking = false;
			m_CurrentState = STATE_WAIT;
		}
		break;

	case STATE_DYING:

		m_AnimTick = 0;
		m_StartLine = 0;
		
		++m_ExplosionTick;
		if(m_ExplosionTick == 8)
			m_HasDied = true;
		break;
	}

	m_Ticker = 0;
	}

	if(m_CurrentState == STATE_ATTACK) m_Sin += 0.07;
	if(m_Sin >= (M_PI*2 - 0.6) && !m_AxeReturned)	// checks if axe has returned to brute
	{
		m_AxeReturned = true;
		m_Sin = 0;
	}

	// Hitrect

	int width = m_BmpBrutePtr->GetWidth()/4;
	int height = m_BmpBrutePtr->GetHeight()/2;

	m_HitRect.left = m_Pos.x + 30;
	m_HitRect.right = m_HitRect.left + width - 60;
	m_HitRect.top = m_Pos.y;
	m_HitRect.bottom = m_HitRect.top + height;

	m_AxeRect.left = m_AxePos.x;
	m_AxeRect.right = m_AxeRect.left + 28;
	m_AxeRect.top = m_AxePos.y;
	m_AxeRect.bottom = m_AxeRect.top + 28;
}

DOUBLE2 Brute::GetPos()
{
	return m_Pos;
}

bool Brute::HasDied()
{
	return m_HasDied;
}

void Brute::Hittest(HitRegion *levelPtr)
{

}

void Brute::GetHitRect(RECT2* hitRect, double* power)
{
	*hitRect = m_AxeRect;
	*power = 10;
}

bool Brute::HitByFireballs(Fireball *fireballPtr)
{
	bool hit = fireballPtr->Hittest(m_HitRect);
	if(hit) m_Health -= fireballPtr->GetFirePower();
	return hit;
}

void Brute::ThrowAxe(MATRIX3X2 matView)
{
	int width = m_BmpBrutePtr->GetWidth()/4;
	int height = m_BmpBrutePtr->GetHeight()/2;

	if(!m_AxeReturned && m_Direction == 1)
		m_AxePos.x += (10 * sin(m_Sin));	// axe moves along a sine wave
	if(!m_AxeReturned && m_Direction == -1)
		m_AxePos.x -= (10 * sin(m_Sin));	// axe moves along a sine wave

	RECT clip;

		clip.left = width * 3;
		clip.right = clip.left + 50;
		clip.top = height;
		clip.bottom = clip.top + 50;

	MATRIX3X2 matAxe, matCenter, matRotate;
	matAxe.SetAsTranslate(m_AxePos);
	matCenter.SetAsTranslate(-10,-15);		// centers the axe so it rotates around the centerpoint
	matRotate.SetAsRotate(m_Sin * 3.5);		// rotates the axe while in the air

	GAME_ENGINE->SetTransformMatrix(matCenter * matRotate * matCenter.Inverse() * matAxe * matView);

	GAME_ENGINE->DrawBitmap(m_BmpBrutePtr,0,0,clip);
}

void Brute::MoveToWillow(DOUBLE2 willowPos)
{
	bool inRange = abs(m_Pos.x - willowPos.x) < 150 && abs(m_Pos.y - willowPos.y) < 35;

	if(inRange && !m_IsAttacking && !m_AxeReturned) m_IsAttacking = true;

	if(willowPos.x < m_Pos.x) m_Direction = -1;
	else m_Direction = 1;
}