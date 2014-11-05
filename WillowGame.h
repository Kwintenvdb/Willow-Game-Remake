//-----------------------------------------------------------------
// Game File
// C++ Source - WillowGame.h - version v2_12 jan 2013 
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Student data
// Name: Kwinten Van den Berghe
// Group: 1DAE4
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "Resource.h"	
#include "GameEngine.h"
#include "AbstractGame.h"
#include "Willow.h"
#include "Fireball.h"
#include "Soldier.h"
#include "Brute.h"
#include "Enemy.h"
#include "Devildog.h"
#include "Deathdog.h"
#include "Treasure.h"
#include "Coin.h"
#include "Necromancer.h"
#include "Shopkeeper.h"

//-----------------------------------------------------------------
// WillowGame Class																
//-----------------------------------------------------------------
class WillowGame : public AbstractGame, public Callable
{
public:				
	//---------------------------
	// Constructor(s)
	//---------------------------
	WillowGame();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~WillowGame();

	//---------------------------
	// General Methods
	//---------------------------

	void GameInitialize(HINSTANCE hInstance);
	void GameStart();				
	void GameEnd();
	void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam);
	//void MouseMove(int x, int y, WPARAM wParam);
	//void CheckKeyboard();
	void KeyPressed(TCHAR cKey);
	void GameTick(double deltaTime);
	void GamePaint(RECT rect);

	//void CallAction(Caller* callerPtr);

	// -------------------------
	// Member functions
	// -------------------------

	static const int MAX_FIREBALLS = 200;
	static const int ENEMIES = 21;
	static const int COINS = 50;
	static const int CHESTS = 3;

private:
	// -------------------------
	// Member functions
	// -------------------------

	void CameraHUD();
	void DrawWillow();
	void DrawFireballs();
	void DrawEnemies();
	void DrawChests();
	void DrawCoins();
	void DrawWizard();
	void DrawShopkeepers();
	void DrawBridge();
	void DrawHUD();

	void TickCamera();
	void TickWillow(double deltaTime);
	void TickFireballs(double deltaTime);
	void TickEnemies(double deltaTime);
	void TickChestsAndCoins(double deltaTime);

	void BossBattle();
	void HandleShopSelection(int selection);
	bool CheckMoney(int itemCost);

	// -------------------------
	// Datamembers
	// -------------------------

	Bitmap *m_BmpLevelPtr, *m_BmpWizardPtr, *m_BmpBridgePtr, 
		*m_BmpExplosionPtr, *m_BmpNumbersPtr, *m_BmpGoldTimePtr,
		*m_BmpVitalPtr, *m_BmpBottombarPtr;
	Willow *m_WillowPtr;
	Fireball *m_FireballPtrArr[MAX_FIREBALLS];
	Enemy *m_EnemyListPtrArr[ENEMIES];
	HitRegion *m_HitLevelPtr;

	DOUBLE2 m_PosWillow, m_PosFireballsArr[MAX_FIREBALLS], m_PosEnemiesArr[ENEMIES], m_PosCamera;
	MATRIX3X2 m_MatView;

	Treasure *m_ChestPtrArr[CHESTS];
	Coin *m_CoinPtrArr[COINS];

	Shopkeeper *m_ShopkeeperPtrArr[2];

	int m_NrFireballs, m_MoneyCount, m_MoveTick, m_Ticker, m_NrOfCoins;
	double m_Time, m_TimeTicker;
	bool m_BossBattle;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	WillowGame(const WillowGame& tRef);
	WillowGame& operator=(const WillowGame& tRef);
};
