//---------------------------
// Includes
//---------------------------
#include "Coin.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap *Coin::m_BmpCoinPtr(nullptr);

//---------------------------
// Constructor & Destructor
//---------------------------
Coin::Coin():
	m_MoveTick()
	,m_Ticker()
	,m_HasSpawned(false)
	,m_HitCoinPtr(nullptr)
	,m_Gravity(0,1000)
	,m_NrOfBounces()
{
	if(m_BmpCoinPtr == nullptr)
		m_BmpCoinPtr = new Bitmap("Resources/coin.bmp");
	if(m_BmpCoinPtr->Exists())
		m_BmpCoinPtr->SetTransparencyColor(255,0,255);

	m_HitCoinPtr = new HitRegion();
	m_HitCoinPtr->CreateFromRect(0,0,m_BmpCoinPtr->GetWidth()/4,m_BmpCoinPtr->GetHeight());
}

Coin::~Coin()
{
	delete m_HitCoinPtr;
}

void Coin::Paint(MATRIX3X2 matView)
{
	MATRIX3X2 matPos;
	matPos.SetAsTranslate(m_Pos);

	int width = m_BmpCoinPtr->GetWidth()/4;
	int height = m_BmpCoinPtr->GetHeight();

	RECT clip;

		clip.left = width * m_MoveTick;
		clip.right = clip.left + width;
		clip.top = 0;
		clip.bottom = height;

	GAME_ENGINE->SetTransformMatrix(matPos * matView);
	GAME_ENGINE->DrawBitmap(m_BmpCoinPtr,0,0,clip);
}

void Coin::Tick(double deltaTime)
{
	++m_Ticker;

	if(m_Ticker > 5)
	{
		++m_MoveTick;
		if(m_MoveTick == 4)
			m_MoveTick = 0;

		m_Ticker = 0;
	}

	m_HitCoinPtr->SetPos(m_Pos);

	m_Velocity += m_Gravity * deltaTime;
	m_Pos += m_Velocity * deltaTime;
}

void Coin::Spawn(DOUBLE2 pos)
{
	m_Pos = pos;
	m_HasSpawned = true;
}

bool Coin::HasSpawned()
{
	return m_HasSpawned;
}

DOUBLE2 Coin::GetPos()
{
	return m_Pos;
}

bool Coin::Hittest(DOUBLE2 willowPos)
{
	if(abs(m_Pos.x - willowPos.x) < 20
		&& abs(m_Pos.y - willowPos.y) < 50)
	{
		return true;
	}
	else return false;
}

void Coin::HitLevel(HitRegion *levelPtr)
{
	RECT2 hitRect;

	hitRect = m_HitCoinPtr->CollisionTest(levelPtr);

	if(abs(hitRect.bottom - m_HitCoinPtr->GetBounds().bottom) < 0.1)
	{
		m_Velocity.y = 0;
		m_Pos.y -= (hitRect.bottom - hitRect.top);

		if(m_NrOfBounces < 25)
			Bounce();
	}
}

void Coin::Bounce()
{
	double bounceHeight = 250 - m_NrOfBounces * 10;

	m_Velocity.y = -bounceHeight;

	++m_NrOfBounces;
}