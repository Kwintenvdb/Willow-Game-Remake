//---------------------------
// Includes
//---------------------------
#include "Willow.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------
Willow::Willow(int x, int y): 
	 m_Pos(x,y)
	,m_RestrictedPos(0,0)
	,m_Health(50) // healthpoints of Willow
	,m_Gravity(0,1000)
	,m_Direction(1) // 1 or -1: looking right or left
	,m_MoveTick()
	,m_Tickcount()
	,m_Starcount()
	,m_CurrentState()
	,m_StartLine() // still needs to be implemented
	,m_BmpHeroPtr(nullptr)
	,m_BmpStarsPtr(nullptr)
	,m_BmpHealthPtr(nullptr)
	,m_Shooting(false) 
	,m_InAir(false) // checks if Willow is in the air
	,m_Climbing(false) // similar to m_InAir
	,m_HitWillowV(nullptr) // used for hittest with level
	,m_DisableHittest(false) // used to temporarily disable hittest so Willow can "fall through" hitregions (== jump down a ledge)
	,m_CollidesWithLevel(false) // checks if Willow is colliding with the level
	,m_LevelIntersect() // used to displace Willow up after climbing
	,m_HitDistance()
	,m_Hit(false)
	,m_Shield(false)
	,m_IncreasedPower()
{
	m_BmpHeroPtr = new Bitmap("Resources/willow_edit.bmp");
	m_BmpHeroPtr->SetTransparencyColor(255,0,255);

	m_BmpStarsPtr = new Bitmap("Resources/stars_sprites.bmp");
	m_BmpStarsPtr->SetTransparencyColor(255,0,255);

	m_BmpHealthPtr = new Bitmap("Resources/health.bmp");

	m_HitWillowV = new HitRegion();
	m_HitWillowV->CreateFromRect(15,12,25,43);
}

Willow::~Willow()
{
	delete m_BmpStarsPtr; delete m_BmpHeroPtr; delete m_HitWillowV;
}

void Willow::Paint(MATRIX3X2 matView)
{
	//------------------------------
	// Willow
	//------------------------------

	int height = m_BmpHeroPtr->GetHeight()/ROWS;
	int width = m_BmpHeroPtr->GetWidth()/COLUMNS;

	MATRIX3X2 matPos, matDirection;

	if(m_Direction == 1) matPos.SetAsTranslate(m_Pos);
	else matPos.SetAsTranslate(m_Pos.x + width,m_Pos.y);

	matDirection.SetAsScale(m_Direction,1);

	GAME_ENGINE->SetTransformMatrix(matDirection * matPos * matView);

	RECT clip_will;

   	switch(m_CurrentState)
	{
	case STATE_WAITING:

		clip_will.left = 0;
		clip_will.right = clip_will.left + width;
		clip_will.top = 0;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_RUN:

		clip_will.left = width + width * m_MoveTick;
		clip_will.right = clip_will.left + width;
		clip_will.top = 0;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_SHOOT:

		clip_will.left = width * m_MoveTick;
		clip_will.right = clip_will.left + width;
		clip_will.top = height;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_JUMP:

		clip_will.left = 0;
		clip_will.right = width;
		clip_will.top = height * 2;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_CLIMB:

		clip_will.left = width * (m_MoveTick + 1);
		clip_will.right = clip_will.left + width;
		clip_will.top = height * 2;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_CROUCH:

		clip_will.left = 0;
		clip_will.right = width;
		clip_will.top = height * 3;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_CROUCHJUMP:

		clip_will.left = width * m_MoveTick;
		clip_will.right = clip_will.left + width;
		clip_will.top = height * 3;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_JUMPDOWN:

		clip_will.left = width * m_MoveTick;
		clip_will.right = clip_will.left + width;
		clip_will.top = height * 4;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_ELECTROCUTED:

		clip_will.left = width * (m_MoveTick % 2);
		clip_will.right = clip_will.left + width;
		clip_will.top = height * 5;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_DYING:

		clip_will.left = width * m_MoveTick;
		clip_will.right = clip_will.left + width;
		clip_will.top = height * 6;
		clip_will.bottom = clip_will.top + height;

		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_CROUCHSHOOT:

		clip_will.left = width * m_MoveTick;
		clip_will.right = clip_will.left + width;
		clip_will.top = height * 7;
		clip_will.bottom = clip_will.top + height;
		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	case STATE_JUMPSHOOT:

		clip_will.left = width * m_MoveTick;
		clip_will.right = clip_will.left + width;
		clip_will.top = height * 8;
		clip_will.bottom = clip_will.top + height;
		GAME_ENGINE->DrawBitmap(m_BmpHeroPtr,0,0,clip_will);
		break;

	default:
		int d=4;
	}

	//------------------------------
	// Star powerup
	//------------------------------

	int width_stars = 36;
	int height_stars = 48;

	int column = m_Starcount % 10;
	int row = m_Starcount / 12;

	RECT clip_stars;

		clip_stars.left = width_stars * column;
		clip_stars.right = clip_stars.left + width_stars;
		clip_stars.top = height_stars * row;
		clip_stars.bottom = clip_stars.top + height_stars;

	if(GAME_ENGINE->IsKeyDown(VK_SPACE) && m_Starcount >= 5) 
		GAME_ENGINE->DrawBitmap(m_BmpStarsPtr,2,0,clip_stars);

	//------------------------------
	// Test HUD
	//------------------------------

//	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());

//	GAME_ENGINE->SetColor(COLOR(255,255,255));
//	GAME_ENGINE->DrawString(String("Starcount: ") + m_Starcount
//		+ "\nMovetick: " + m_MoveTick
//		+ "\nHealth: " + m_Health
//		+ "\nCurrentState: " + m_CurrentState,20,10);

	GAME_ENGINE->SetColor(COLOR(255,0,0,160));

	int healthbarwidth = m_BmpHealthPtr->GetWidth()/6;

	RECT cliphealth;

	if(m_Health >= 50) cliphealth.left = 0;
	else if(m_Health >= 40) cliphealth.left = healthbarwidth;
	else if(m_Health >= 30) cliphealth.left = healthbarwidth * 2;
	else if(m_Health >= 20) cliphealth.left = healthbarwidth * 3;
	else if(m_Health >= 10) cliphealth.left = healthbarwidth * 4;
	else cliphealth.left = healthbarwidth * 5;
	cliphealth.right = cliphealth.left + healthbarwidth;
	cliphealth.top = 0;
	cliphealth.bottom = m_BmpHealthPtr->GetHeight();

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(52,10));
	GAME_ENGINE->DrawBitmap(m_BmpHealthPtr,0,0,cliphealth);

	//GAME_ENGINE->SetTransformMatrix(matView);
	//GAME_ENGINE->FillRect(m_HitRect);
}

void Willow::Tick(double deltaTime)
{
	if(m_Health <= 0) m_CurrentState = STATE_DYING;

	//------------------------------
	// Behavior of animation states
	//------------------------------

		// Is he shooting?
	if(!GAME_ENGINE->IsKeyDown(VK_SPACE)) m_Shooting = false;

	if(m_Shooting && m_CurrentState != STATE_CROUCH && m_CurrentState != STATE_CROUCHSHOOT)
	{
		m_CurrentState = STATE_SHOOT;
	}
	if(m_Shooting && m_CurrentState == STATE_CROUCH) m_CurrentState = STATE_CROUCHSHOOT;
	if(m_Shooting && m_CurrentState == STATE_JUMP) m_CurrentState = STATE_JUMPSHOOT;

	double actorSpeed = 0;

	++m_Tickcount;

	if(m_Tickcount > 5)
	{
	switch(m_CurrentState)
	{
	case STATE_WAITING:

		m_Shooting = false;
		m_StartLine = 0;
		m_MoveTick = 0;
		break;

	case STATE_RUN:

		m_StartLine = 0;
		m_DisableHittest = false;
		m_MoveTick++;
		m_MoveTick %= 6;
		break;

	case STATE_SHOOT:

		m_StartLine = 1;
		if(m_MoveTick < 2) ++m_MoveTick;
		if(m_MoveTick == 2 && !m_Shooting)
		{
			m_Shooting = false;
			m_CurrentState = STATE_WAITING;
		}
		break;

	case STATE_JUMP:

		m_StartLine = 2;
		m_DisableHittest = false;
		m_MoveTick = 0;
		if(m_Shooting) m_CurrentState = STATE_JUMPSHOOT;
		break;

	case STATE_CLIMB:

		m_StartLine = 2;
		m_MoveTick++;
		if(m_MoveTick > 4)
		{
			m_MoveTick = 0;
			m_Pos.y -= m_LevelIntersect;	// puts Willow a little higher so he collides with the level
			m_Climbing = false;
			m_CurrentState = STATE_WAITING;
		}
		break;

	case STATE_CROUCH:

		m_StartLine = 3;
		m_MoveTick = 0;
		break;

	case STATE_CROUCHJUMP:
		
		m_StartLine = 3;
		++m_MoveTick;
		if(m_MoveTick > 3)
		{
			m_MoveTick = 0;
			m_CurrentState = STATE_CROUCH;
		}
		break;

	case STATE_JUMPDOWN:

		m_StartLine = 4;
		m_DisableHittest = true;
		++m_MoveTick;
		if(m_MoveTick > 3)
		{
			m_MoveTick = 0;
			m_DisableHittest = false;
			m_CurrentState = STATE_WAITING;
		}
		break;

	case STATE_ELECTROCUTED:

		m_StartLine = 5;
		++m_MoveTick;
		if(m_MoveTick > 3)
		{
			m_MoveTick = 0;
			m_CurrentState = STATE_WAITING;
		}
		break;

	case STATE_DYING:

		m_StartLine = 6;
		++m_MoveTick;
		if(m_MoveTick > 6)
		{
			m_MoveTick = 0;
			Respawn();
		}
		break;

	case STATE_CROUCHSHOOT:

		m_StartLine = 7;
		++m_MoveTick;
		if(m_MoveTick > 1)
		{
			m_Shooting = false;
			m_CurrentState = STATE_CROUCH;
		}
		break;

	case STATE_JUMPSHOOT:

		m_StartLine = 8;
		++m_MoveTick;
		if(m_MoveTick > 1)
		{
			//m_Shooting = false;
			m_CurrentState = STATE_JUMP;
		}
		break;
	}

	//------------------------------
	// Star powerup
	//------------------------------

	if(GAME_ENGINE->IsKeyDown(VK_SPACE))
	{
		m_Starcount += 3;

		if(m_Starcount >= 120)
			m_Starcount = 100;
	}

	if(!GAME_ENGINE->IsKeyDown(VK_SPACE)) m_Starcount = m_IncreasedPower;

	m_Tickcount = 0;
	}

	//------------------------------
	// Checks
	//------------------------------

	if(GAME_ENGINE->IsKeyDown('A') && m_CurrentState != STATE_CROUCH && m_CurrentState != STATE_CROUCHJUMP && m_CurrentState != STATE_CLIMB
		&& !m_Shooting)
	{
		m_CurrentState = STATE_RUN;
		actorSpeed = -140;
		m_Direction = -1;
	}
	else if(GAME_ENGINE->IsKeyDown('D') && m_CurrentState != STATE_CROUCH && m_CurrentState != STATE_CROUCHJUMP && m_CurrentState != STATE_CLIMB
		&& !m_Shooting) 
	{
		m_CurrentState = STATE_RUN;
		actorSpeed = 140;
		m_Direction = 1;
	}
	else if(m_CurrentState != STATE_SHOOT && m_CurrentState != STATE_CLIMB 
		&& m_CurrentState != STATE_CROUCHJUMP && m_CurrentState != STATE_JUMPDOWN
		&& m_CurrentState != STATE_DYING && m_CurrentState != STATE_CROUCHSHOOT
		&& m_CurrentState != STATE_ELECTROCUTED) 
		m_CurrentState = STATE_WAITING;

	if(GAME_ENGINE->IsKeyDown(VK_SPACE) && m_CurrentState != STATE_JUMPDOWN && m_CurrentState != STATE_RUN)
	{
		m_MoveTick = 0;
		m_Shooting = true;
	}

	// Jumping

	if(GAME_ENGINE->IsKeyDown('W') && !m_InAir) Jump();
	if(m_InAir && !m_Shooting)
	{
		m_CurrentState = STATE_JUMP;
	}

	// Climbing

	if(GAME_ENGINE->IsKeyDown('C') 
		&& !m_Climbing 
		&& m_InAir
		&& m_CollidesWithLevel 
		&& m_HitDistance < 30) Climb();

	if(m_Climbing)
		m_CurrentState = STATE_CLIMB;

	// Crouching

	if(GAME_ENGINE->IsKeyDown('S') && !m_InAir && m_CurrentState != STATE_JUMPDOWN)
	{
		if(m_CurrentState != STATE_CROUCHJUMP && m_CurrentState != STATE_CROUCHSHOOT) m_CurrentState = STATE_CROUCH;
		
		if(m_CurrentState == STATE_CROUCHSHOOT && GAME_ENGINE->IsKeyDown(VK_SPACE)) m_Shooting = true;
		
		if(GAME_ENGINE->IsKeyDown('D'))
		{
			m_CurrentState = STATE_CROUCHJUMP;
			actorSpeed = 500;
			m_Direction = 1;
		}

		if(GAME_ENGINE->IsKeyDown('A'))
		{
			m_CurrentState = STATE_CROUCHJUMP;
			actorSpeed = -500;
			m_Direction = -1;
		}
	}

	// Jumping down

	if(GAME_ENGINE->IsKeyDown(VK_DOWN) && m_Pos.y < 640 && !m_InAir)
	{
		m_CurrentState = STATE_JUMPDOWN;
	}

	// Handles gravity and movement
	
	if(m_Climbing) m_Gravity = DOUBLE2(0,0);
	else m_Gravity = DOUBLE2(0,1000);

	m_Velocity += m_Gravity * deltaTime;	
	m_Velocity.x = actorSpeed;
	m_Pos += m_Velocity * deltaTime;

	if(m_RestrictedPos != DOUBLE2(0,0) && m_Pos.x < m_RestrictedPos.x)
		m_Pos.x = m_RestrictedPos.x;
	if(m_RestrictedPos != DOUBLE2(0,0) && m_Pos.x > m_RestrictedPos.y)
		m_Pos.x = m_RestrictedPos.y;

	//------------------------------
	// HitRegions / collisions
	//------------------------------

	m_HitWillowV->SetPos(m_Pos);

	if(m_CurrentState == STATE_CROUCH || m_CurrentState == STATE_CROUCHSHOOT || m_CurrentState == STATE_CROUCHJUMP)
	{
		m_HitRect.left = m_Pos.x + 10;
		m_HitRect.right = m_HitRect.left + 20;
		m_HitRect.top = m_Pos.y + 25;
		m_HitRect.bottom = m_HitRect.top + 20;
	}
	else
	{
		m_HitRect.left = m_Pos.x + 10;
		m_HitRect.right = m_HitRect.left + 20;
		m_HitRect.top = m_Pos.y + 5;
		m_HitRect.bottom = m_HitRect.top + 40;
	}
}

void Willow::Jump()
{
 	m_Velocity.y = -345;
	m_Pos.y -= 1;	// Prevents getting "stuck" because of collisiontest
	m_InAir = true;
	//m_CurrentState = STATE_JUMP;
}

void Willow::Climb()
{
	m_Climbing = true;
	m_Velocity.y = 0;
}

void Willow::HitByEnemies(Enemy *enemyPtr)
{
	// checks if an enemy hits Willow (== overlaps with him) 
	// and reduces Willow's healthpoints when this is the case

	RECT2 r1;
	double power;	// the power variable allows enemies to do different amounts of damage

	enemyPtr->GetHitRect(&r1, &power);	// stores variables 

	bool noIntersect =
	r1.right < m_HitRect.left || r1.left > m_HitRect.right || 
	r1.top > m_HitRect.bottom || r1.bottom < m_HitRect.top;

	m_Hit = !noIntersect;

	if(m_Hit && power == 50) m_CurrentState = STATE_ELECTROCUTED;

	if(m_Hit && !m_Shield) m_Health -= (0.06 + power/100);
	if(m_Hit && m_Shield) m_Shield = false;
}

void Willow::HitTest(HitRegion *levelPtr)
{
	// collision between willow and the level

	if(!m_DisableHittest)
	{
	RECT2 hitRectV = m_HitWillowV->CollisionTest(levelPtr);

	m_HitDistance = hitRectV.bottom - hitRectV.top;
	m_LevelIntersect =  abs(m_HitWillowV->GetBounds().bottom - hitRectV.bottom) + m_HitDistance;

	if(hitRectV.bottom - hitRectV.top == 0) m_CollidesWithLevel = false;
	else m_CollidesWithLevel = true;

	if(m_Velocity.y >= 10 &&	// only if Willow is moving down
		!abs((m_HitWillowV->GetBounds().bottom - m_HitWillowV->GetBounds().top) -
		(hitRectV.bottom - hitRectV.top)) < 1)
	{
		if(abs(hitRectV.bottom - m_HitWillowV->GetBounds().bottom) < 0.01)
		{
			m_Velocity.y = 0;
			m_Pos.y -= (hitRectV.bottom - hitRectV.top);
			m_InAir = false;
		}
		else m_InAir = true;
		}
	}

	// horizontal raycast

	DOUBLE2 vector(38,0);
	HIT hitArr[1];

	if(m_CurrentState != STATE_JUMP && m_CurrentState != STATE_CLIMB)
	{
		if(m_Direction == 1)
		{
			if(levelPtr->Raycast(DOUBLE2(m_Pos.x,m_Pos.y+40),vector,hitArr,1,-1) > 0)
			{
				double depth = hitArr[0].lambda;
				DOUBLE2 displacement = (1 - depth) * vector;
				m_Velocity.x = 0;
				m_Pos -= displacement;
			}
		}
		else
		{
			DOUBLE2 vector2(-38,0);

			if(levelPtr->Raycast(DOUBLE2(m_Pos.x+45,m_Pos.y+40),vector2,hitArr,1,-1) > 0)
			{
				double depth = hitArr[0].lambda;
				DOUBLE2 displacement = (1 - depth) * vector2;
				m_Velocity.x = 0;
				m_Pos -= displacement;
			}
		}
	}
}

DOUBLE2 Willow::GetPos()
{
	return m_Pos;
}

DOUBLE2 Willow::GetDimensions()
{
	DOUBLE2 dimensions;
	dimensions.x = m_BmpHeroPtr->GetWidth()/COLUMNS;
	dimensions.y = m_BmpHeroPtr->GetHeight()/ROWS;

	return dimensions;
}

HitRegion* Willow::GetHitRegion()
{
	return m_HitWillowV;
}

int Willow::GetDirection()
{
	return m_Direction;
}

int Willow::GetFirePower()
{
	return m_Starcount;
}

void Willow::SetRestrictedPos(DOUBLE2 area)
{
	m_RestrictedPos = area;
}

void Willow::Respawn()
{
	// this function is called when Willow has died

	if(m_Pos.x < 1200)
		m_Pos = DOUBLE2(250,650);
	else if(m_Pos.x < 2500)
		m_Pos = DOUBLE2(1500,300);

	m_Health = 50;
	m_CurrentState = STATE_WAITING;
}

void Willow::AddLifePoint(int amount)
{
	m_Health += amount * 10;
	if(m_Health > 50) m_Health = 50;
}

void Willow::AddShield()
{
	m_Shield = true;
}

void Willow::IncreasePower(int amount)
{
	if(amount == 1)
	{
		m_IncreasedPower = 60;
	}
	else if(amount == 2)
	{
		m_IncreasedPower = 80;
	}
}

void Willow::Kill()
{
	m_CurrentState = STATE_DYING;
}