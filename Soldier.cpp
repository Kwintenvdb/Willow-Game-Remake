//---------------------------
// Includes
//---------------------------
#include "Soldier.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap *Soldier::m_BmpSoldierPtr(nullptr);
Bitmap *Soldier::m_BmpExplosionPtr(nullptr);

//---------------------------
// Constructor & Destructor
//---------------------------
Soldier::Soldier(DOUBLE2 pos): 
	m_Pos(pos)
	,m_Gravity(0,1000)
	,m_CurrentState()
	,m_MoveTick()
	,m_Tickcount()
	,m_Ticker()
	,m_StartLine()
	,m_Speed(110)
	,m_Direction(1)
	,m_IsRunning(false)
	,m_Jumping(false)
	,m_HitSoldier(nullptr) // HitRegion for collision with level
	,m_JumpDistance()
	,m_Health(5)
	,m_IsDying(false) // has he started dying (== falling down)
	,m_HasDied(false) // has he died (== completed dying animation)
	,m_HitsTheLevel(false)
	,m_Exploding(false)
	,m_ExplosionTicks()
{
	if(m_BmpSoldierPtr == nullptr)
		m_BmpSoldierPtr = new Bitmap("Resources/Soldier.bmp");
	if(m_BmpSoldierPtr->Exists())
		m_BmpSoldierPtr->SetTransparencyColor(255,0,255);
	
	if(m_BmpExplosionPtr == nullptr)
		m_BmpExplosionPtr = new Bitmap("Resources/explosion.png");
	if(m_BmpExplosionPtr->Exists())
		m_BmpExplosionPtr->SetTransparencyColor(255,0,255);

	int width = m_BmpSoldierPtr->GetWidth()/6;
	int height = m_BmpSoldierPtr->GetHeight()/5;

	m_HitSoldier = new HitRegion;
	m_HitSoldier->CreateFromRect(20,50,width-20,height);
}

Soldier::~Soldier()
{
	delete m_HitSoldier;
}

void Soldier::Paint(MATRIX3X2 matView)
{
	int width = m_BmpSoldierPtr->GetWidth()/6;
	int height = m_BmpSoldierPtr->GetHeight()/5;

	MATRIX3X2 matPos, matDirection;
	if(m_Direction == 1) matPos.SetAsTranslate(m_Pos);
	else matPos.SetAsTranslate(m_Pos.x + width, m_Pos.y);
	matDirection.SetAsScale(m_Direction,1);
	GAME_ENGINE->SetTransformMatrix(matDirection * matPos * matView);

	RECT clip;
	clip.left = width * m_MoveTick;
	clip.right = clip.left + width;
	clip.top = height * m_StartLine;
	clip.bottom = clip.top + height;

	GAME_ENGINE->DrawBitmap(m_BmpSoldierPtr,0,0,clip);

	//GAME_ENGINE->SetTransformMatrix(matView);
	//GAME_ENGINE->FillHitRegion(m_HitSoldier);

	//GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
	//GAME_ENGINE->DrawString(String("JumpDistance: ") + m_JumpDistance
	//	+ "\nHealth" + m_Health,200,50);

	if(m_Exploding)
	{
		int width2 = m_BmpExplosionPtr->GetWidth()/8;
		int height2 = m_BmpExplosionPtr->GetHeight();

		RECT clip2;
		clip2.left = width2 * m_ExplosionTicks;
		clip2.right = clip2.left + width2;
		clip2.top = 0;
		clip2.bottom = height2;

		MATRIX3X2 matExplosion;
		matExplosion.SetAsTranslate(m_Pos);

		GAME_ENGINE->SetTransformMatrix(matExplosion * matView);
		GAME_ENGINE->DrawBitmap(m_BmpExplosionPtr,0,0,clip2);

		if(m_Ticker % 5 == 0) ++m_ExplosionTicks;
		if(m_ExplosionTicks == 8) m_ExplosionTicks = 0;
	}
}

void Soldier::Tick(double deltaTime)
{
	if(m_Health <= 0)
	{
		if(!m_IsDying)
		{
			m_MoveTick = 0;
			m_Velocity.y -= 250;
			//m_Exploding = true;
		}
		m_CurrentState = STATE_DYING;
		m_IsDying = true;
	}

	++m_Ticker;

	if(m_Ticker > 5)
	{
	switch(m_CurrentState)
	{
	case STATE_WAIT:

		m_IsRunning = false;
		m_Jumping = false;
		m_Velocity.x = 0;

		m_StartLine = 0;

		++m_Tickcount;
		if(m_Tickcount % 15 == 0)
			m_MoveTick++;

		if(m_MoveTick == 5)
			m_MoveTick = 0;
		break;

	case STATE_RUN:

		m_IsRunning = true;
		//m_Jumping = false;

		m_StartLine = 1;
		m_MoveTick++;

		if(m_MoveTick == 6) m_MoveTick = 0;
		break;

	case STATE_JUMP:

		m_IsRunning = false;

		m_StartLine = 4;

		if(m_MoveTick < 2) ++m_MoveTick;
		if(m_MoveTick == 2)
		{
			Jump();
			m_Jumping = true;
			//m_MoveTick = 0;
		}
		break;

	case STATE_FALLDOWN:

		m_IsRunning = false;

		m_StartLine = 2;

		if(m_MoveTick < 3) ++m_MoveTick;
		break;

	case STATE_DYING:

		m_StartLine = 3;
		m_IsRunning = false;
		m_Jumping = false;

		m_Velocity.x = -m_Direction * 100;

		if(m_MoveTick < 3) ++m_MoveTick;
		if(m_MoveTick == 2) m_Exploding = true;
		if(m_MoveTick == 3 && m_HitsTheLevel) m_HasDied = true;
		break;
	}

	m_Ticker = 0;
	}

	// Velocity and gravity

	m_Velocity += m_Gravity * deltaTime;
	if(m_IsRunning || m_Jumping 
		&& m_CurrentState != STATE_DYING) m_Velocity.x = m_Direction * m_Speed;
	if(m_Jumping) m_Velocity.x = m_Direction * (m_Speed - 30);
	m_Pos += m_Velocity * deltaTime;

	// Hitregions

	if(m_CurrentState == STATE_DYING) m_HitSoldier->SetPos(m_Pos.x, m_Pos.y - 10);
	else m_HitSoldier->SetPos(m_Pos);

	m_HitRect.left = m_Pos.x + 20;
	m_HitRect.right = m_HitRect.left + 30;
	m_HitRect.top = m_Pos.y + 20;
	m_HitRect.bottom = m_HitRect.top + 45;
}

DOUBLE2 Soldier::GetPos()
{
	return m_Pos;
}

bool Soldier::HasDied()
{
	return m_HasDied;
}

void Soldier::Hittest(HitRegion *levelPtr)
{
	bool enable; // enables / disables hittest to make soldier jump through the level
	if(m_Jumping && m_Velocity.y <= 0 || (m_CurrentState == STATE_FALLDOWN && m_MoveTick == 3)) enable = false;
	else enable = true;

	RECT2 hitRect = m_HitSoldier->CollisionTest(levelPtr);

	if(enable)
	{
		if(abs(hitRect.bottom - m_HitSoldier->GetBounds().bottom) < 0.001)
		{
			m_Velocity.y = 0;
			m_Pos.y -= (hitRect.bottom - hitRect.top);

			m_Jumping = false;
			m_HitsTheLevel = true;
		}
		else m_HitsTheLevel = false;
	}
}

void Soldier::GetHitRect(RECT2* hitRect, double* power)
{
	*hitRect = m_HitRect;
	*power = 2;
}

bool Soldier::HitByFireballs(Fireball *fireballPtr)
{
	bool hit = fireballPtr->Hittest(m_HitRect);
	if(hit) m_Health -= fireballPtr->GetFirePower();
	return hit;
}

void Soldier::Jump()
{
	double jumpVelocity(0);
	if(m_JumpDistance < 100) jumpVelocity = 250;
	if(m_JumpDistance < 40) jumpVelocity = 150;

	if(!m_Jumping)
	{
		m_Velocity.y -= jumpVelocity + 200;
		m_Pos.y -= 1;
	}
}

void Soldier::MoveToWillow(DOUBLE2 willowPos)
{
	// this function handles the Soldier's interaction with Willow

	m_JumpDistance = abs(m_Pos.y - willowPos.y);

	bool inRange = abs(m_Pos.x - willowPos.x) < 150;
	bool inRangeY = abs(m_Pos.x - willowPos.x) < 50;  // this is used for when Willow is higher / lower than the soldier
	bool visible(false);
	bool changeDir(false);	// this bool makes the soldier run "around" Willow
	bool above = willowPos.y < m_Pos.y;
	bool below = willowPos.y - m_Pos.y > 30;

	int yVisibility = 100;

	if(abs((m_Pos.y + 5) - willowPos.y) < 20) visible = true; // m_Pos.y + 5 is to make up for height difference between Willow and soldier

	if(inRange && visible && !m_Jumping && m_CurrentState != STATE_DYING)	// if Willow is at the same level as the soldier
	{
		if(abs(willowPos.x - m_Pos.x) <= 60)
		{
			changeDir = false;
		}
		else changeDir = true;

		m_CurrentState = STATE_RUN;

		if(changeDir)
		{
			if(m_Pos.x > willowPos.x) m_Direction = -1;
			else m_Direction = 1;
		}
	}
	else if(!visible && inRange && m_CurrentState != STATE_DYING)
	{
		if(m_CurrentState == STATE_RUN)
		{
			m_MoveTick = 0;
			m_CurrentState = STATE_WAIT;
		}

		if(m_CurrentState == STATE_WAIT && inRangeY)
		{
			if(above && m_JumpDistance <= yVisibility && m_MoveTick == 3)
			{
				m_MoveTick = 0;
				m_CurrentState = STATE_JUMP;
			}

			if(below && m_JumpDistance <= yVisibility && m_MoveTick == 4)
			{
				m_MoveTick = 0;
				m_CurrentState = STATE_FALLDOWN;
			}
		}
	}
	else if(!m_Jumping) m_CurrentState = STATE_WAIT;
}

void Soldier::Explode()
{
	m_Exploding = true;
}