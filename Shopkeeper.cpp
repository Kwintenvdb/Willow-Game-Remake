//---------------------------
// Includes
//---------------------------
#include "Shopkeeper.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

Bitmap *Shopkeeper::m_BmpKeeperSpritePtr(nullptr);
Bitmap *Shopkeeper::m_BmpShopPtr(nullptr);
Bitmap *Shopkeeper::m_BmpSelectPtr(nullptr);
Bitmap *Shopkeeper::m_BmpAcceptDeclinePtr(nullptr);
Bitmap *Shopkeeper::m_BmpInfotextPtr(nullptr);

//---------------------------
// Constructor & Destructor
//---------------------------
Shopkeeper::Shopkeeper(DOUBLE2 pos):
	 m_Pos(pos)
	,m_SelectPos(16,32)
	,m_MoveTick()
	,m_Ticker()
	,m_EnterShop(false)
	,m_ExitShop(false)
	,m_AcceptShop(false)
	,m_DeclineShop(false)
{
	if(m_BmpKeeperSpritePtr == nullptr)
		m_BmpKeeperSpritePtr = new Bitmap("Resources/shopsprite.png");
	if(m_BmpShopPtr == nullptr)
		m_BmpShopPtr = new Bitmap("Resources/shopinterface.png");
	if(m_BmpSelectPtr == nullptr)
		m_BmpSelectPtr = new Bitmap("Resources/select.png");
	if(m_BmpAcceptDeclinePtr == nullptr)
		m_BmpAcceptDeclinePtr = new Bitmap("Resources/selectText.png");
	if(m_BmpInfotextPtr == nullptr)
		m_BmpInfotextPtr = new Bitmap("Resources/infoText.png");

	m_Clip1.left = 0;
	m_Clip1.right = m_BmpAcceptDeclinePtr->GetWidth();
	m_Clip1.top = 0;

	m_Clip2.left = 0;
	m_Clip2.right = m_BmpInfotextPtr->GetWidth()/4;
	m_Clip2.top = 0;
}

Shopkeeper::~Shopkeeper()
{
	
}

void Shopkeeper::Paint(MATRIX3X2 matView)
{
	// Movetick

	++m_Ticker;
	if(m_Ticker > 50)
	{
		++m_MoveTick;
		if(m_MoveTick == 3) m_MoveTick = 0;
		m_Ticker = 0;
	}

	// Paint

	MATRIX3X2 matPos;
	matPos.SetAsTranslate(m_Pos);

	GAME_ENGINE->SetTransformMatrix(matPos * matView);

	if(!m_EnterShop && !m_ExitShop)
	{
		int widthsprite = m_BmpKeeperSpritePtr->GetWidth()/3;

		RECT clip;
		clip.left = widthsprite * m_MoveTick;
		clip.right = clip.left + widthsprite;
		clip.top = 0;
		clip.bottom = m_BmpKeeperSpritePtr->GetHeight();

		GAME_ENGINE->DrawBitmap(m_BmpKeeperSpritePtr,0,0,clip);
	}
	else if(m_EnterShop && !m_ExitShop)
	{
		int height = m_BmpShopPtr->GetHeight()/3;

		RECT clip;
		clip.left = 0;
		clip.right = m_BmpShopPtr->GetWidth();
		clip.top = height * m_MoveTick;
		clip.bottom = clip.top + height;

		GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
		GAME_ENGINE->DrawBitmap(m_BmpShopPtr,0,0,clip);

		// selection rectangle

		MATRIX3X2 matSelect;
		matSelect.SetAsTranslate(m_SelectPos);
		GAME_ENGINE->SetTransformMatrix(matSelect);
		GAME_ENGINE->DrawBitmap(m_BmpSelectPtr,0,0);

		GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
		GAME_ENGINE->SetColor(COLOR(255,255,255));
		GAME_ENGINE->DrawString(String("Selection Rect pos: ") + m_SelectPos.ToString(),15,15);
	
		PaintSelectionText();
	}	
}

bool Shopkeeper::Hittest(DOUBLE2 willowPos)
{
	if(abs(m_Pos.x - willowPos.x) < 20
		&& abs(m_Pos.y - willowPos.y) < 50
		&& !m_ExitShop)
	{
		m_EnterShop = true;
		return true;
	}
	else return false;
}

bool Shopkeeper::InShop()
{
	return m_EnterShop;
}

void Shopkeeper::MoveSelection(int right1left2up3down4) // odd variable name to make it easier to understand what number will move the selection in what direction
{
	int moveX = 48;
	int moveY = 40;

	int width = m_BmpInfotextPtr->GetWidth()/4;
	int height = m_BmpInfotextPtr->GetHeight()/3;

	if(m_AcceptShop)
	{
		if(right1left2up3down4 == 1 && m_SelectPos.x < 336)
		{
			m_SelectPos.x += moveX;

			// these lines of code make the selection rect jump over the shopkeeper's picture
			// to the next item to buy
			if(m_SelectPos.x > 64 && m_SelectPos.x < 288)
				m_SelectPos.x = 288;

			m_Clip2.left += width;
		}

		if(right1left2up3down4 == 2 && m_SelectPos.x > 16)
		{
			m_SelectPos.x -= moveX;

			if(m_SelectPos.x < 288 && m_SelectPos.x > 64)
				m_SelectPos.x = 64;

			m_Clip2.left -= width;
		}

		if(right1left2up3down4 == 3 && m_SelectPos.y > 32)
		{
			m_SelectPos.y -= moveY;

			m_Clip2.top -= height;
		}

		if(right1left2up3down4 == 4 && m_SelectPos.y < 112)
		{
			m_SelectPos.y += moveY;

			m_Clip2.top += height;
		}
	}
}

void Shopkeeper::ExitShop()
{
	if(m_SelectPos == DOUBLE2(336,112))
	{
		m_ExitShop = true;
		m_EnterShop = false;
	}
}

void Shopkeeper::PaintSelectionText()
{
	// do you want to shop?

	int height = m_BmpAcceptDeclinePtr->GetHeight()/2;

	if(GAME_ENGINE->IsKeyDown(VK_RIGHT) && m_Clip1.top < height) m_Clip1.top = height;
	if(GAME_ENGINE->IsKeyDown(VK_LEFT) && m_Clip1.top > 0) m_Clip1.top = 0;

	if(m_Clip1.top > height) m_Clip1.top = height;
	if(m_Clip1.top < 0) m_Clip1.top = 0;

	m_Clip1.bottom = m_Clip1.top + height;

	if(!m_AcceptShop || m_DeclineShop)
		GAME_ENGINE->DrawBitmap(m_BmpAcceptDeclinePtr,13,148,m_Clip1);

	if(m_Clip1.top == 0 && GAME_ENGINE->IsKeyDown(VK_RETURN))
		m_AcceptShop = true;
	if(m_Clip1.top == height && GAME_ENGINE->IsKeyDown(VK_RETURN))
	{
		m_DeclineShop = true;
		m_ExitShop = true;
		m_EnterShop = false;
	}

	// what to buy?

	int width = m_BmpInfotextPtr->GetWidth()/4;
	int height2 = m_BmpInfotextPtr->GetHeight()/3;

	m_Clip2.right = m_Clip2.left + width;
	m_Clip2.bottom = m_Clip2.top + height2;

	if(m_AcceptShop)
		GAME_ENGINE->DrawBitmap(m_BmpInfotextPtr,13,148,m_Clip2);
}

int Shopkeeper::GetSelection()
{
	int selection(0);

	if(m_SelectPos == DOUBLE2(16,32)) selection = 1;
	if(m_SelectPos == DOUBLE2(16,72)) selection = 2;
	if(m_SelectPos == DOUBLE2(16,112)) selection = 3;

	if(m_SelectPos == DOUBLE2(64,32)) selection = 4;
	if(m_SelectPos == DOUBLE2(64,72)) selection = 5;
	if(m_SelectPos == DOUBLE2(64,112)) selection = 6;

	if(m_SelectPos == DOUBLE2(288,32)) selection = 7;
	if(m_SelectPos == DOUBLE2(288,72)) selection = 8;
	if(m_SelectPos == DOUBLE2(288,112)) selection = 9;

	if(m_SelectPos == DOUBLE2(336,32)) selection = 10;
	if(m_SelectPos == DOUBLE2(336,72)) selection = 11;
	if(m_SelectPos == DOUBLE2(336,112)) selection = 12;

	return selection;
}