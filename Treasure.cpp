//---------------------------
// Includes
//---------------------------
#include "Treasure.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------

Bitmap *Treasure::m_BmpTreasurePtr(nullptr);

Treasure::Treasure(DOUBLE2 pos):
	m_Pos(pos)
	,m_Life(60)
	,m_Open(false)
{
	if(m_BmpTreasurePtr == nullptr)
		m_BmpTreasurePtr = new Bitmap("Resources/treasure.bmp");
	if(m_BmpTreasurePtr->Exists())
		m_BmpTreasurePtr->SetTransparencyColor(255,0,255);
}

Treasure::~Treasure()
{

}

void Treasure::Paint(MATRIX3X2 matView)
{
	MATRIX3X2 matPos;
	matPos.SetAsTranslate(m_Pos);
	GAME_ENGINE->SetTransformMatrix(matPos * matView);
	GAME_ENGINE->DrawBitmap(m_BmpTreasurePtr,0,0);
}

void Treasure::Tick()
{
	if(m_Life <= 0) m_Open = true;
}

bool Treasure::Hittest(Fireball *fireballPtr)
{
	RECT2 hitRect;
	hitRect.left = m_Pos.x + 10;
	hitRect.right = hitRect.left + m_BmpTreasurePtr->GetWidth() - 30;
	hitRect.top = m_Pos.y;
	hitRect.bottom = hitRect.top + m_BmpTreasurePtr->GetHeight();

	bool hit = fireballPtr->Hittest(hitRect);

	if(hit) --m_Life;

	return hit;
}

DOUBLE2 Treasure::GetPos()
{
	return m_Pos;
}

bool Treasure::HasOpened()
{
	return m_Open;
}

void Treasure::SetPos(DOUBLE2 newPos)
{
	m_Pos = newPos;
}