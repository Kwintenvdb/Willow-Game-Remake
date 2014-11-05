//---------------------------
// Includes
//---------------------------
#include "Fireball.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap *Fireball::m_BmpFireBallPtr(nullptr);

//---------------------------
// Constructor & Destructor
//---------------------------
Fireball::Fireball(): 
	m_Tickcount()
	,m_Speed(225)
	,m_ElapsedDistance()
	,m_WasFired(false)
	,m_Hit(false)
	,m_Direction(1)
	,m_FirePower()
	,m_Ticker()
{
	if(m_BmpFireBallPtr == nullptr)
		m_BmpFireBallPtr = new Bitmap("Resources/FireBall.bmp");
	if(m_BmpFireBallPtr->Exists())
		m_BmpFireBallPtr->SetTransparencyColor(255,0,255);
}

Fireball::~Fireball()
{

}

void Fireball::Paint(MATRIX3X2 matView)
{
	int clipheight = m_BmpFireBallPtr->GetHeight();
	int clipwidth = m_BmpFireBallPtr->GetWidth()/26;

	RECT clip;

		clip.left = clipwidth * m_Tickcount;
		clip.right = clip.left + clipwidth;
		clip.top = 0;
		clip.bottom = clip.top + clipheight;

	double scale;
	if(m_FirePower >= 0) scale = 1;
	if(m_FirePower >= 25) scale = 1.1;
	if(m_FirePower >= 60) scale = 1.25;
	if(m_FirePower >= 80) scale = 1.5;
	if(m_FirePower >= 100) scale = 1.6;

	MATRIX3X2 matPos, matDirection, matScale, matCenter;

	if(m_Direction == 1) matPos.SetAsTranslate(m_Pos);
	else matPos.SetAsTranslate(m_Pos.x + clipwidth, m_Pos.y);
	matDirection.SetAsScale(m_Direction, 1);

	matCenter.SetAsTranslate(-clipwidth,-clipheight);
	matScale.SetAsScale(scale);

	GAME_ENGINE->SetTransformMatrix(matCenter * matScale * matCenter.Inverse() * matDirection * matPos * matView);

	GAME_ENGINE->DrawBitmap(m_BmpFireBallPtr,0,0,clip);
}

void Fireball::Tick(double deltaTime)
{
	//m_HitregionPtr->SetPos(m_Pos);

	int clipheight = m_BmpFireBallPtr->GetHeight();
	int clipwidth = m_BmpFireBallPtr->GetWidth()/26;

	m_HitRect.left = m_Pos.x;
	m_HitRect.right = m_HitRect.left + clipwidth;
	m_HitRect.top = m_Pos.y;
	m_HitRect.bottom = m_HitRect.top + clipheight;

	int maxDistance = 175;

	m_ElapsedDistance += m_Speed * deltaTime;
	if(m_ElapsedDistance >= maxDistance) m_Hit = true;

	++m_Ticker;
	if(m_Ticker > 4) ++m_Tickcount;

	if(!m_Hit)
	{
		//++m_Tickcount;
		if(m_Tickcount == FIRE_TICKS) m_Tickcount = START_FIRE;

		if(m_Direction == 1)
			m_Pos.x += m_Speed * deltaTime;
		else m_Pos.x -= m_Speed * deltaTime;

	}
	else if(m_Hit)
	{
		if(m_Tickcount < FIRE_TICKS) m_Tickcount = FIRE_TICKS;
		//++m_Tickcount;
	}
}

void Fireball::Fire(DOUBLE2 startPos, int direction, int firePower)
{
	m_Pos = startPos;
	m_Direction = direction;
	m_WasFired = true;

	m_FirePower = firePower;
}

bool Fireball::WasFired()
{
	return m_WasFired;
}

void Fireball::SetHit(bool hit)
{
	m_Hit = hit;
}

bool Fireball::Hittest(RECT2 r1)
{
	bool noIntersect =
	r1.right < m_HitRect.left || r1.left > m_HitRect.right || 
	r1.top > m_HitRect.bottom || r1.bottom < m_HitRect.top;

	if(!noIntersect) m_Hit = true;
	return !noIntersect;
}

bool Fireball::HasExploded()
{
	if(m_Tickcount > ALL_TICKS) return true;
	else return false;
}

DOUBLE2 Fireball::GetPos()
{
	return m_Pos;
}

RECT2 Fireball::GetHitRect()
{
	return m_HitRect;
}

int Fireball::GetFirePower()
{
	if(m_FirePower >= 100) return 10;
	else if(m_FirePower >= 80) return 8;
	else if(m_FirePower >= 60) return 6;
	else if(m_FirePower >= 25) return 2;
	else return 1;
}

void Fireball::SetFirePower(int power)
{
	m_FirePower = power;
}