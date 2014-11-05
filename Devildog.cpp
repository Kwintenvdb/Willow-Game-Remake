//---------------------------
// Includes
//---------------------------
#include "Devildog.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------

Bitmap *Devildog::m_BmpDevildogPtr(nullptr);

Devildog::Devildog(DOUBLE2 pos, DOUBLE2 roamArea):
	m_Pos(pos)
	,m_Roam(roamArea) // holds 2 x coordinates that determine the boundary of the dog's "territory"
	,m_CurrentState()
	,m_StartLine()
	,m_MoveTick()
	,m_Ticker()
	,m_Direction(1)
	,m_HasDied(false)
	,m_IsDying(false)
	,m_Hit(false)
	,m_HitDogH(nullptr)
	,m_Gravity(0,1000)
	,m_Health(70)
	
{
	if(m_BmpDevildogPtr == nullptr)
		m_BmpDevildogPtr = new Bitmap("Resources/devildog.bmp");
	if(m_BmpDevildogPtr->Exists())
		m_BmpDevildogPtr->SetTransparencyColor(255,0,255);

	m_HitDogH = new HitRegion;
	m_HitDogH->CreateFromRect(0,40,75,75);
}

Devildog::~Devildog()
{
	delete m_HitDogH;
}

void Devildog::Paint(MATRIX3X2 matView)
{
	int width = m_BmpDevildogPtr->GetWidth()/6;
	int height = m_BmpDevildogPtr->GetHeight()/4;

	RECT clip;

		clip.left = width * m_MoveTick;
		clip.right = clip.left + width;
		clip.top = height * m_StartLine;
		clip.bottom = clip.top + height;
	
	MATRIX3X2 matDirection, matPos;

	if(m_Direction == 1) matPos.SetAsTranslate(m_Pos);
	else matPos.SetAsTranslate(m_Pos.x + width, m_Pos.y);

	matDirection.SetAsScale(m_Direction,1);
	GAME_ENGINE->SetTransformMatrix(matDirection * matPos * matView);

	GAME_ENGINE->DrawBitmap(m_BmpDevildogPtr,0,0,clip);

	GAME_ENGINE->SetColor(COLOR(255,0,0,125));
}

void Devildog::Tick(double deltaTime)
{
	if(m_Health <= 0)
	{
		if(!m_IsDying) m_MoveTick = 0;
		m_CurrentState = STATE_DYING;
		m_IsDying = true;
	}

	if(m_Hit)
	{
		m_MoveTick = 0;
		m_CurrentState = STATE_PUSH;
	}

	bool isTurning(false);
	if(m_MoveTick >= 4 && m_CurrentState == STATE_TURN) isTurning = true;

	SetRoamArea(isTurning);

	if(GAME_ENGINE->IsKeyDown('E'))
	{
		m_MoveTick = 0;
		m_CurrentState = STATE_ATTACK;
	}
	else if(GAME_ENGINE->IsKeyDown('Q'))
	{	
		m_MoveTick = 0;
		m_CurrentState = STATE_DYING;
	}

	double dogSpeed = 0;

	++m_Ticker;
	if(m_Ticker > 6)
	{
	switch(m_CurrentState)
	{
	case STATE_LEAVE:

		m_StartLine = 0;
		++m_MoveTick;
		if(m_MoveTick == 2)
		{
			m_CurrentState = STATE_ROAM;
		}
		break;

	case STATE_ROAM:

		m_StartLine = 1;
		
		++m_MoveTick;
		if(m_MoveTick == 4)
		{
			m_MoveTick = 0;
		}
		break;

	case STATE_TURN:

		m_StartLine = 1;

		if(!isTurning)
		{
			m_MoveTick = 3;
		}

		++m_MoveTick;
		if(m_MoveTick == 6)
		{
			m_Direction *= -1;
			m_Pos.x += m_Direction * 2; // prevents getting stuck
			m_MoveTick = 0;
			m_CurrentState = STATE_ROAM;
		}
		break;

	case STATE_ATTACK:

		m_StartLine = 2;

		++m_MoveTick;
		if(m_MoveTick == 6)
		{
			m_MoveTick = 0;
			m_CurrentState = STATE_ROAM;
		}
		break;

	case STATE_DYING:

		m_StartLine = 3;

		++m_MoveTick;
		if(m_MoveTick == 3)
		{
			m_HasDied = true;
		}
		break;

	case STATE_PUSH:

		m_StartLine = 3;

		++m_MoveTick;
		if(m_MoveTick == 1)
		{
			m_MoveTick = 0;
			m_CurrentState = STATE_ROAM;
		}
		break;
	}
	m_Ticker = 0;
	}

	if(m_CurrentState == STATE_ROAM) dogSpeed = 100;	// this is not in the switch case, otherwise the speed would be slowed down by the ticker as well
	if(m_CurrentState == STATE_ATTACK) dogSpeed = 150;
	if(m_CurrentState == STATE_PUSH) dogSpeed = -200;

	dogSpeed *= m_Direction;

	m_Velocity += m_Gravity * deltaTime;
	m_Velocity.x = dogSpeed;

	m_Pos += m_Velocity * deltaTime;

	m_Hit = false;

	// Hitregion

	m_HitDogH->SetPos(m_Pos);

	m_HitRect.left = m_Pos.x + 15;
	m_HitRect.right = m_HitRect.left + 55;
	m_HitRect.top = m_Pos.y + 45;
	m_HitRect.bottom = m_HitRect.top + 35;
	
}

DOUBLE2 Devildog::GetPos()
{
	return m_Pos;
}

void Devildog::SetRoamArea(bool turning)
{
	// If the dog tries to walk out of the roam area, he turns around

	if(!turning)
		if(m_Pos.x > m_Roam.y
			|| m_Pos.x < m_Roam.x) m_CurrentState = STATE_TURN;
}

bool Devildog::HasDied()
{
	return m_HasDied;
}

void Devildog::Hittest(HitRegion *levelPtr)
{
	RECT2 hitRectH;

	hitRectH = m_HitDogH->CollisionTest(levelPtr);

	if(abs(hitRectH.bottom - m_HitDogH->GetBounds().bottom) < 0.1)
	{
		m_Velocity.y = 0;
		m_Pos.y -= (hitRectH.bottom - hitRectH.top);
	}
}

void Devildog::GetHitRect(RECT2* hitRect, double* power)
{
	*hitRect = m_HitRect;
	*power = 3;
}

bool Devildog::HitByFireballs(Fireball *fireballPtr)
{
	bool hit = fireballPtr->Hittest(m_HitRect);
	m_Hit = hit;
	if(hit) m_Health -= fireballPtr->GetFirePower();
	return hit;
}

void Devildog::MoveToWillow(DOUBLE2 willowPos)
{
	int width = m_BmpDevildogPtr->GetWidth()/6;

	bool inRange = abs(m_Pos.x - willowPos.x) < 150;
	bool onPlatform = abs((m_Pos.y + 20) - willowPos.y) < 20;
	bool inRoamArea = willowPos.x >= m_Roam.x && willowPos.x <= m_Roam.y + width;
	bool visible(false);
	if(m_Direction == 1 && willowPos.x > m_Pos.x
		|| m_Direction == -1 && willowPos.x < m_Pos.x) visible = true;

	if(inRange && inRoamArea && visible && onPlatform
		&& m_CurrentState != STATE_PUSH) m_CurrentState = STATE_ATTACK;
}