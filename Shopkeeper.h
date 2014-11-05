#pragma once
//-----------------------------------------------------
// Name:
// First name:
// Group: 1DAE.
//-----------------------------------------------------

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------
// Shopkeeper Class									
//-----------------------------------------------------
class Shopkeeper
{
public:
	Shopkeeper(DOUBLE2 pos);				// Constructor
	virtual ~Shopkeeper();		// Destructor

	//-------------------------------------------------
	// Methods							
	//-------------------------------------------------

	void Paint(MATRIX3X2 matView);
	bool Hittest(DOUBLE2 willowPos);
	bool InShop();
	void MoveSelection(int right1left2up3down4);
	void ExitShop();
	int GetSelection();

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	void PaintSelectionText();

	static Bitmap *m_BmpKeeperSpritePtr, *m_BmpShopPtr, *m_BmpSelectPtr,
		*m_BmpAcceptDeclinePtr, *m_BmpInfotextPtr;

	DOUBLE2 m_Pos, m_SelectPos;
	int m_MoveTick, m_Ticker;
	bool m_EnterShop, m_ExitShop, m_AcceptShop, m_DeclineShop;

	RECT m_Clip1, m_Clip2;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Shopkeeper(const Shopkeeper& yRef);									
	Shopkeeper& operator=(const Shopkeeper& yRef);	
};

 
