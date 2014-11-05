//-----------------------------------------------------------------
// Game File
// C++ Source - WillowGame.cpp - version v2_12 jan 2013 
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "WillowGame.h"																				

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//-----------------------------------------------------------------
// WillowGame methods																				
//-----------------------------------------------------------------

WillowGame::WillowGame(): 
	 m_WillowPtr(nullptr)
	,m_BmpLevelPtr(nullptr)
	,m_HitLevelPtr(nullptr)
	,m_NrFireballs()
	,m_MoneyCount()
	,m_BmpWizardPtr(nullptr)
	,m_BmpBridgePtr(nullptr)
	,m_BmpExplosionPtr(nullptr)
	,m_BmpNumbersPtr(nullptr)
	,m_BmpGoldTimePtr(nullptr)
	,m_BmpVitalPtr(nullptr)
	,m_BmpBottombarPtr(nullptr)
	,m_MoveTick()
	,m_Ticker()
	,m_NrOfCoins()
	,m_Time(180)
	,m_TimeTicker()
	,m_BossBattle(false)
{
	for(int i = 0; i < MAX_FIREBALLS; ++i)
		m_FireballPtrArr[i] = nullptr;

	for(int i = 0; i < ENEMIES; ++i)
		m_EnemyListPtrArr[i] = nullptr;

	for(int i = 0; i < COINS; ++i)
		m_CoinPtrArr[i] = nullptr;

	for(int i  = 0; i < CHESTS; ++i)
		m_ChestPtrArr[i] = nullptr;

	for(int i  = 0; i < 2; ++i)
		m_ShopkeeperPtrArr[i] = nullptr;
}

WillowGame::~WillowGame()																						
{
	// nothing to destroy
}

void WillowGame::GameInitialize(HINSTANCE hInstance)			
{
	// Set the required values
	AbstractGame::GameInitialize(hInstance);
	GAME_ENGINE->SetTitle("WillowGame - Van den Berghe Kwinten - 1DAE4");					
	GAME_ENGINE->RunGameLoop(true);
	
	// Set the optional values
	GAME_ENGINE->SetWidth(472);
	GAME_ENGINE->SetHeight(224);
	GAME_ENGINE->SetKeyList(String("QSDZ") + (TCHAR) VK_SPACE 
		+ (TCHAR) VK_RIGHT + (TCHAR) VK_LEFT + (TCHAR) VK_UP + (TCHAR) VK_DOWN + (TCHAR) VK_RETURN);
}

void WillowGame::GameStart()
{
	m_WillowPtr = new Willow(200,650);

	// Bitmaps

	m_BmpLevelPtr = new Bitmap("Resources/level2.bmp");

	m_BmpWizardPtr = new Bitmap("Resources/Wizard.bmp");
	m_BmpWizardPtr->SetTransparencyColor(255,0,255);

	m_BmpBridgePtr = new Bitmap("Resources/bridge.png");
	m_BmpBridgePtr->SetTransparencyColor(255,0,255);

	m_BmpExplosionPtr = new Bitmap("Resources/explosion.png");
	m_BmpExplosionPtr->SetTransparencyColor(255,0,255);

	m_BmpNumbersPtr = new Bitmap("Resources/numbers.png");
	m_BmpNumbersPtr->SetTransparencyColor(255,0,255);

	m_BmpGoldTimePtr = new Bitmap("Resources/gold_time.png");
	m_BmpGoldTimePtr->SetTransparencyColor(255,0,255);

	m_BmpVitalPtr = new Bitmap("Resources/vital.png");
	m_BmpVitalPtr->SetTransparencyColor(255,0,255);

	m_BmpBottombarPtr = new Bitmap("Resources/bottombar.png");
	
	m_HitLevelPtr = new HitRegion;
	m_HitLevelPtr->CreateFromSVG("Resources/HitLevel.svg");

	// Other classes

	for(int i = 0; i < MAX_FIREBALLS; ++i)
		m_FireballPtrArr[i] = new Fireball;

	//m_EnemyListPtrArr[0] = new Soldier(DOUBLE2(420,500));
	//m_EnemyListPtrArr[1] = new Brute(DOUBLE2(1000,620));
	//m_EnemyListPtrArr[2] = new Devildog(DOUBLE2(350,500), DOUBLE2(200,550));
	//m_EnemyListPtrArr[3] = new Deathdog(DOUBLE2(3400,575));
	//m_EnemyListPtrArr[4] = new Necromancer(DOUBLE2(2045,300));

	// Soldiers

	m_EnemyListPtrArr[0]  = new Soldier(DOUBLE2(450,500));
	m_EnemyListPtrArr[1]  = new Soldier(DOUBLE2(680,500));
	m_EnemyListPtrArr[2]  = new Soldier(DOUBLE2(700,600));
	m_EnemyListPtrArr[3]  = new Soldier(DOUBLE2(850,600));
	m_EnemyListPtrArr[4]  = new Soldier(DOUBLE2(1200,600));
	m_EnemyListPtrArr[5]  = new Soldier(DOUBLE2(1500,500));
	m_EnemyListPtrArr[6]  = new Soldier(DOUBLE2(1450,250));
	m_EnemyListPtrArr[7]  = new Soldier(DOUBLE2(1450,350));
	m_EnemyListPtrArr[8]  = new Soldier(DOUBLE2(1800,250));
	m_EnemyListPtrArr[9]  = new Soldier(DOUBLE2(2260,250));
	m_EnemyListPtrArr[10] = new Soldier(DOUBLE2(2390,300));
	m_EnemyListPtrArr[11] = new Soldier(DOUBLE2(2420,400));
	m_EnemyListPtrArr[12] = new Soldier(DOUBLE2(2280,550));
	m_EnemyListPtrArr[13] = new Soldier(DOUBLE2(2450,550));

	// Devildogs

	m_EnemyListPtrArr[14] = new Devildog(DOUBLE2(1800,400),DOUBLE2(1700,1800));
	m_EnemyListPtrArr[15] = new Devildog(DOUBLE2(1480,250),DOUBLE2(1400,1650));
	m_EnemyListPtrArr[16] = new Devildog(DOUBLE2(2380,400),DOUBLE2(2380,2455));

	// Brutes

	m_EnemyListPtrArr[17] = new Brute(DOUBLE2(2635,605));
	m_EnemyListPtrArr[18] = new Brute(DOUBLE2(2900,625));

	// Necromancer

	m_EnemyListPtrArr[19] = new Necromancer(DOUBLE2(2045,300));

	// Deathdog

	m_EnemyListPtrArr[20] = new Deathdog(DOUBLE2(3400,575));

	// Coins and treasures

	for (int i = 0; i < COINS; ++i)
		m_CoinPtrArr[i] = new Coin();

	m_ChestPtrArr[0] = new Treasure(DOUBLE2(1250,402));
	m_ChestPtrArr[1] = new Treasure(DOUBLE2(1845,605));
	m_ChestPtrArr[2] = new Treasure(DOUBLE2(2530,385));

	m_ShopkeeperPtrArr[0] = new Shopkeeper(DOUBLE2(1845,430));
	m_ShopkeeperPtrArr[1] = new Shopkeeper(DOUBLE2(2600,540));
}

void WillowGame::GameEnd()
{
	delete m_WillowPtr; delete m_BmpLevelPtr; delete m_HitLevelPtr;
	delete m_BmpWizardPtr; delete m_BmpBridgePtr; delete m_BmpNumbersPtr;
	delete m_BmpGoldTimePtr; delete m_BmpVitalPtr; delete m_BmpBottombarPtr;

	for(int i = 0; i < MAX_FIREBALLS; ++i)
		delete m_FireballPtrArr[i];

	for(int i = 0; i < ENEMIES; ++i)
		delete m_EnemyListPtrArr[i];

	for (int i = 0; i < COINS; ++i)
		delete m_CoinPtrArr[i];

	for(int i  = 0; i < CHESTS; ++i)
		delete m_ChestPtrArr[i];

	for(int i  = 0; i < 2; ++i)
		delete m_ShopkeeperPtrArr[i];
}

void WillowGame::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{	
	//if (isLeft == true && isDown == true)
	//{	
	//	m_ChestPtr->SetPos(DOUBLE2(x,y));
	//	GAME_ENGINE->MessageBox(String("") + x + " " + y);
	//}
}

//void WillowGame::MouseMove(int x, int y, WPARAM wParam)
//{	
//	// Insert the code that needs to be executed when the mouse pointer moves across the game window
//
//	/* Example:
//	if ( x > 261 && x < 261 + 117 ) // check if mouse position is within x coordinates of choice
//	{
//		if ( y > 182 && y < 182 + 33 ) // check if mouse position also is within y coordinates of choice
//		{
//			GAME_ENGINE->MessageBox("Da mouse wuz here.");
//		}
//	}
//	*/
//
//}
//
//void WillowGame::CheckKeyboard()
//{	
//	// Here you can check if a key of choice is held down
//	// Is executed once per frame 
//
//	/* Example:
//	if (GAME_ENGINE->IsKeyDown('K')) xIcon -= xSpeed;
//	if (GAME_ENGINE->IsKeyDown('L')) yIcon += xSpeed;
//	if (GAME_ENGINE->IsKeyDown('M')) xIcon += xSpeed;
//	if (GAME_ENGINE->IsKeyDown('O')) yIcon -= ySpeed;
//	*/
//}
//

void WillowGame::KeyPressed(TCHAR cKey)
{	
	if(cKey == VK_SPACE)
	{
		if(m_FireballPtrArr[m_NrFireballs] != nullptr)
			m_FireballPtrArr[m_NrFireballs]->Fire(DOUBLE2(m_PosWillow.x,m_PosWillow.y+6), m_WillowPtr->GetDirection(), m_WillowPtr->GetFirePower());

		m_NrFireballs++;
	}

	for(int i = 0; i < 2; ++i)
	{
		if(m_ShopkeeperPtrArr[i]->InShop())
		{
			if(cKey == VK_RIGHT)
				m_ShopkeeperPtrArr[i]->MoveSelection(1);
			if(cKey == VK_LEFT)
				m_ShopkeeperPtrArr[i]->MoveSelection(2);
			if(cKey == VK_UP)
				m_ShopkeeperPtrArr[i]->MoveSelection(3);
			if(cKey == VK_DOWN)
				m_ShopkeeperPtrArr[i]->MoveSelection(4);

			if(cKey == VK_RETURN) 
				HandleShopSelection(m_ShopkeeperPtrArr[i]->GetSelection());

			if(cKey == VK_RETURN)
				m_ShopkeeperPtrArr[i]->ExitShop();
		}
	}
}

void WillowGame::GameTick(double deltaTime)
{
	if(deltaTime > 0.1) deltaTime = 0.1;

	if(!m_ShopkeeperPtrArr[1]->InShop() && !m_ShopkeeperPtrArr[0]->InShop())
	{
		TickCamera();

		TickWillow(deltaTime);

		TickFireballs(deltaTime);

		TickEnemies(deltaTime);

		TickChestsAndCoins(deltaTime);

		BossBattle();
	}

	m_TimeTicker += deltaTime;
	if(m_TimeTicker >= 1)
	{
		m_Time--;
		m_TimeTicker = 0;
	}
}

void WillowGame::GamePaint(RECT rect)
{
	CameraHUD();

	DrawEnemies();

	DrawChests();

	DrawCoins();

	DrawBridge();

	DrawWillow();
	
	DrawFireballs();

	DrawWizard();

	DrawShopkeepers();

	DrawHUD();
}

//----------------------
// Paint methods
//----------------------

void WillowGame::CameraHUD()
{
	// Camera and level

	GAME_ENGINE->SetTransformMatrix(m_MatView);

	GAME_ENGINE->DrawBitmap(m_BmpLevelPtr,0,0);
	GAME_ENGINE->SetColor(COLOR(255,0,0,160));
	
	//GAME_ENGINE->FillHitRegion(m_HitLevelPtr);
	//GAME_ENGINE->FillHitRegion(m_WillowPtr->GetHitRegion());

	//for(int i = 0; i < ENEMIES; ++i)
	//{
	//	if(m_EnemyListPtrArr[i] != nullptr)
	//	{
	//		RECT2 r; double k;
	//		m_EnemyListPtrArr[i]->GetHitRect(&r,&k);
	//		GAME_ENGINE->FillRect(r);
	//	}
	//}

	//for(int i = 0; i < MAX_FIREBALLS; ++i)
	//{
	//	if(m_FireballPtrArr[i] != nullptr)
	//	{
	//		GAME_ENGINE->FillRect(m_FireballPtrArr[i]->GetHitRect());
	//	}
	//}
}

void WillowGame::DrawWillow()
{
	// Willow

	m_WillowPtr->Paint(m_MatView);

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
	GAME_ENGINE->SetColor(COLOR(255,255,255));
//	GAME_ENGINE->DrawString(String("Nr of fireballs: ") + m_NrFireballs
//		+ "\nMoney: " + m_MoneyCount
//		+ "\nNecromancer pos: " + m_PosEnemiesArr[4].ToString(), 15,65);
}

void WillowGame::DrawFireballs()
{
	// Fireballs

	for(int i = 0; i < MAX_FIREBALLS; ++i)
	{
		if(m_FireballPtrArr[i] != nullptr && m_FireballPtrArr[i]->WasFired())
		{
			m_FireballPtrArr[i]->Paint(m_MatView);
		}
 	}
}

void WillowGame::DrawEnemies()
{
	// Enemies

	for(int i = 0; i < ENEMIES; ++i)
	{
		if(m_EnemyListPtrArr[i] != nullptr
			&& m_EnemyListPtrArr[i]->HasSpawned())
		{
			m_EnemyListPtrArr[i]->Paint(m_MatView);
		}
	}
}

void WillowGame::DrawChests()
{
	// Treasure chests

	for(int i  = 0; i < CHESTS; ++i)
	{
		if(m_ChestPtrArr[i] != nullptr)
		{
			m_ChestPtrArr[i]->Paint(m_MatView);
		}
	}

}

void WillowGame::DrawCoins()
{
	// Coins

	for (int i = 0; i < COINS; ++i)
	{
		if(m_CoinPtrArr[i] != nullptr && m_CoinPtrArr[i]->HasSpawned())
		{
			m_CoinPtrArr[i]->Paint(m_MatView);
			if(m_CoinPtrArr[i]->Hittest(m_PosWillow))
			{
				int random = rand()%2;
				if(random == 0)
					m_MoneyCount += 100;
				else
					m_MoneyCount += 1;

				delete m_CoinPtrArr[i];
				m_CoinPtrArr[i] = nullptr;
			}
		}
	}
}

void WillowGame::DrawWizard()
{
	++m_Ticker;
	if(m_Ticker > 20)
	{
		++m_MoveTick;
		if(m_MoveTick > 3) m_MoveTick = 0;
		m_Ticker = 0;
	}

	int width = m_BmpWizardPtr->GetWidth()/4;
	int height = m_BmpWizardPtr->GetHeight();

	DOUBLE2 wizardPos(120,650);
	MATRIX3X2 matWizard;
	matWizard.SetAsTranslate(wizardPos);

	RECT clip;

		clip.left = width * m_MoveTick;
		clip.right = clip.left + width;
		clip.top = 0;
		clip.bottom = height;

	GAME_ENGINE->SetTransformMatrix(matWizard * m_MatView);
	GAME_ENGINE->DrawBitmap(m_BmpWizardPtr,0,0,clip);
}

void WillowGame::DrawShopkeepers()
{
	for(int i = 0; i < 2; ++i)
	{
		m_ShopkeeperPtrArr[i]->Paint(m_MatView);

		m_ShopkeeperPtrArr[i]->Hittest(m_PosWillow);
	}
}

void WillowGame::DrawBridge()
{
	int logWidth = m_BmpBridgePtr->GetWidth();
	int nrLogs = 20;
	int logXPos(1915);

	for(int i = 0; i <  nrLogs; ++i)
	{
		logXPos += (logWidth * i);

		MATRIX3X2 matBridgePart;
		matBridgePart.SetAsTranslate(logXPos,464);
		GAME_ENGINE->SetTransformMatrix(matBridgePart * m_MatView);
		GAME_ENGINE->DrawBitmap(m_BmpBridgePtr,0,0);

		logXPos = 1915;
	}
}

void WillowGame::DrawHUD()
{
	// Time

	int minutes = (int) m_Time / 60;
	int seconds = (int) m_Time % 60;

	int digit1(0), digit2(0);

	String stringSeconds;
	stringSeconds = String("") + seconds;
	if(stringSeconds.GetLength() > 1)
	{
		digit1 = stringSeconds.SubString(0,1).ToInteger();
		digit2 = stringSeconds.SubString(1).ToInteger();
	}
	else
	{
		digit2 = stringSeconds.ToInteger();
	}

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
	//GAME_ENGINE->DrawString(String("") + minutes + " " + seconds,250,100);

	RECT clipGoldTime;
	clipGoldTime.left = m_BmpGoldTimePtr->GetWidth()/2;
	clipGoldTime.right = m_BmpGoldTimePtr->GetWidth();
	clipGoldTime.top = 0;
	clipGoldTime.bottom = m_BmpGoldTimePtr->GetHeight();

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(GAME_ENGINE->GetWidth()-100,12));
	GAME_ENGINE->DrawBitmap(m_BmpGoldTimePtr,0,0,clipGoldTime);

	clipGoldTime.left = 0;
	clipGoldTime.right = m_BmpGoldTimePtr->GetWidth()/2;

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(GAME_ENGINE->GetWidth()-240,12));
	GAME_ENGINE->DrawBitmap(m_BmpGoldTimePtr,0,0,clipGoldTime);

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(10,12));
	GAME_ENGINE->DrawBitmap(m_BmpVitalPtr,0,0);

	int startclip(0);
	int width = m_BmpNumbersPtr->GetWidth()/11;
	int height = m_BmpNumbersPtr->GetHeight()/2;

	switch(minutes)
	{
	case 3:
		startclip = 4;
		break;
	case 2:
		startclip = 3;
		break;
	case 1:
		startclip = 2;
		break;
	case 0:
		startclip = 1;
		break;
	}

	RECT clipminutes;
	clipminutes.left = width * startclip;
	clipminutes.right = clipminutes.left + width;
	clipminutes.top = 0;
	clipminutes.bottom = height;

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(GAME_ENGINE->GetWidth()-60,12));
	GAME_ENGINE->DrawBitmap(m_BmpNumbersPtr,0,0,clipminutes);

	RECT colon;
	colon.left = 0;
	colon.right = width;
	colon.top = 0;
	colon.bottom = height;

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(GAME_ENGINE->GetWidth()-60 + width,12));
	GAME_ENGINE->DrawBitmap(m_BmpNumbersPtr,0,0,colon);

	RECT clipdigit1;
	clipdigit1.left = (digit1 + 1) * width;
	clipdigit1.right = clipdigit1.left + width;
	clipdigit1.top = 0;
	clipdigit1.bottom = height;

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(GAME_ENGINE->GetWidth()-60 + width*2,12));
	GAME_ENGINE->DrawBitmap(m_BmpNumbersPtr,0,0,clipdigit1);

	RECT clipdigit2;
	clipdigit2.left = (digit2 + 1) * width;
	clipdigit2.right = clipdigit2.left + width;
	clipdigit2.top = 0;
	clipdigit2.bottom = height;

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(GAME_ENGINE->GetWidth()-60 + width*3,12));
	GAME_ENGINE->DrawBitmap(m_BmpNumbersPtr,0,0,clipdigit2);

	// Gold

	String money = String("") + m_MoneyCount;

	for(int i = 0; i < money.GetLength(); ++i)
	{
		int digitgold = money.SubString(i,1).ToInteger();

		RECT clipgold;
		clipgold.left = (digitgold + 1) * width;
		clipgold.right = clipgold.left + width;
		clipgold.top = height;
		clipgold.bottom = clipgold.top + height;

		GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(GAME_ENGINE->GetWidth()-165 + width*i,12));
		GAME_ENGINE->DrawBitmap(m_BmpNumbersPtr,0,0,clipgold);
	}

	// bottom HUD

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateTranslationMatrix(0,GAME_ENGINE->GetHeight()-16));
	GAME_ENGINE->DrawBitmap(m_BmpBottombarPtr,0,0);

	GAME_ENGINE->SetTransformMatrix(MATRIX3X2::CreateIdentityMatrix());
	int middlebar = GAME_ENGINE->GetHeight()-m_BmpBottombarPtr->GetHeight()/2 - 1;
	int firepower = m_WillowPtr->GetFirePower();
	if(firepower >= 100) firepower = 100;
	GAME_ENGINE->FillRect(12,middlebar,10 + firepower * 1.5,middlebar + 2);
}

//----------------------
// Tick methods
//----------------------

void WillowGame::TickCamera()
{
	MATRIX3X2 matCamera;
	matCamera.SetAsTranslate(m_PosCamera);
	m_MatView = matCamera.Inverse();
}

void WillowGame::TickWillow(double deltaTime)
{
	// Willow

	m_PosWillow = m_WillowPtr->GetPos();

	m_WillowPtr->Tick(deltaTime);
	m_WillowPtr->HitTest(m_HitLevelPtr);

	if(m_PosWillow.y > 750) m_WillowPtr->Kill();
	if(m_Time == 0)
	{
		m_WillowPtr->Kill();
		m_Time = 180;
	}

	int centerx = GAME_ENGINE->GetWidth()/2 - (m_WillowPtr->GetDimensions().x/2);
	int centery = GAME_ENGINE->GetHeight()/2 - (m_WillowPtr->GetDimensions().y/2);

	if(m_PosWillow.y - m_PosCamera.y >= centery || m_PosWillow.y - m_PosCamera.y <= centery) 
		m_PosCamera.y = m_PosWillow.y - centery;
	if(m_PosWillow.x - m_PosCamera.x >= centerx || m_PosWillow.x - m_PosCamera.x <= centerx) 
		m_PosCamera.x = m_PosWillow.x - centerx;

	if(m_PosCamera.y >= m_BmpLevelPtr->GetHeight() - GAME_ENGINE->GetHeight())
		m_PosCamera.y = m_BmpLevelPtr->GetHeight() - GAME_ENGINE->GetHeight();
	if(m_PosCamera.x <= 0) m_PosCamera.x = 0;
	if(m_PosCamera.x >= m_BmpLevelPtr->GetWidth() - GAME_ENGINE->GetWidth())
		m_PosCamera.x = m_BmpLevelPtr->GetWidth() - GAME_ENGINE->GetWidth();
}

void WillowGame::TickFireballs(double deltaTime)
{
	// Fireballs

	for(int i = 0; i < MAX_FIREBALLS; ++i)
	{
		if(m_FireballPtrArr[i] != nullptr && m_FireballPtrArr[i]->WasFired())
		{
			// For every enemy, check if it has been hit by a fireball

			for(int j = 0; j < ENEMIES; ++j)
			{
				if(m_EnemyListPtrArr[j] != nullptr
					&& m_EnemyListPtrArr[j]->HasSpawned())
				{
					m_EnemyListPtrArr[j]->HitByFireballs(m_FireballPtrArr[i]);
				}
			}

			m_FireballPtrArr[i]->Tick(deltaTime);	// tick fireballs

			if(m_FireballPtrArr[i]->HasExploded())	// check if fireball has exploded
			{
				delete m_FireballPtrArr[i];
				m_FireballPtrArr[i] = nullptr;
			}		
		}
	}

	//if(m_NrFireballs == MAX_FIREBALLS) 
	//{
	//	m_NrFireballs = 0;

	//	for(int i = 0; i < MAX_FIREBALLS; ++i)
	//	{
	//		delete m_FireballPtrArr[i];
	//		m_FireballPtrArr[i] = nullptr;
	//	}
	//}
}

void WillowGame::TickEnemies(double deltaTime)
{
	// Enemies

	for(int i = 0; i < ENEMIES; ++i)
	{
		if(m_EnemyListPtrArr[i] != nullptr
			&& m_EnemyListPtrArr[i]->HasSpawned())
		{
			m_PosEnemiesArr[i] = m_EnemyListPtrArr[i]->GetPos();	// stores positions of enemies
			m_EnemyListPtrArr[i]->Tick(deltaTime);
			m_EnemyListPtrArr[i]->Hittest(m_HitLevelPtr);

			m_EnemyListPtrArr[i]->MoveToWillow(m_PosWillow);	// if possible, the enemy will interact with Willow

			m_WillowPtr->HitByEnemies(m_EnemyListPtrArr[i]);

			if(m_EnemyListPtrArr[i]->HasDied())
			{
				// Spawn a coin if an enemy has died

				if(!m_CoinPtrArr[m_NrOfCoins]->HasSpawned())	// extra check
					m_CoinPtrArr[m_NrOfCoins]->Spawn(m_EnemyListPtrArr[i]->GetPos());

				++m_NrOfCoins;	// go to next step in array of coins

				delete m_EnemyListPtrArr[i];
				m_EnemyListPtrArr[i] = nullptr;
			}
		}
	}
}

void WillowGame::TickChestsAndCoins(double deltaTime)
{
	// Treasure chests

	for(int i  = 0; i < CHESTS; ++i)
	{
		if(m_ChestPtrArr[i] != nullptr)
		{
			m_ChestPtrArr[i]->Tick();

			for(int j = 0; j < MAX_FIREBALLS; ++j)
				if(m_FireballPtrArr[j] != nullptr && m_FireballPtrArr[j]->WasFired())
					if(m_ChestPtrArr[i]->Hittest(m_FireballPtrArr[j]))
					{
						m_FireballPtrArr[j]->SetHit(true);
					}

			if(m_ChestPtrArr[i]->HasOpened())
			{
				// Spawn a coin if the chest has been opened

				if(!m_CoinPtrArr[m_NrOfCoins]->HasSpawned())	// extra check
					m_CoinPtrArr[m_NrOfCoins]->Spawn(m_ChestPtrArr[i]->GetPos());

				++m_NrOfCoins;	// go to next step in array of coins

				delete m_ChestPtrArr[i];
				m_ChestPtrArr[i] = nullptr;
			}
		}
	}

	// Coins

	for(int k = 0; k < COINS; ++k)
	{
		if(m_CoinPtrArr[k] != nullptr && m_CoinPtrArr[k]->HasSpawned())
		{
			m_CoinPtrArr[k]->Tick(deltaTime);
			m_CoinPtrArr[k]->HitLevel(m_HitLevelPtr);
		}
	}
}

void WillowGame::BossBattle()
{
	DOUBLE2 firstBoss(1880,280), secondBoss(3250,500);
	bool battle1(false);

	// necromancer

	if(m_EnemyListPtrArr[19] != nullptr)
	{
		if(m_PosWillow.x >= firstBoss.x && !m_EnemyListPtrArr[19]->HasDied())	// if Willow enters the boss area and the boss hasn't died yet
		{
			m_PosCamera.x = firstBoss.x;
			m_PosCamera.y = firstBoss.y;

			m_BossBattle = true;
			battle1 = true;
		}

		if(m_BossBattle)
		{
			m_EnemyListPtrArr[19]->Spawn();
			m_WillowPtr->SetRestrictedPos(DOUBLE2(firstBoss.x,firstBoss.x+310));
		}
	}
	else
	{
		m_WillowPtr->SetRestrictedPos(DOUBLE2(0,0));
		m_BossBattle = false;
	}

	// deathdog

	if(m_EnemyListPtrArr[20] != nullptr)
	{
		if(m_PosWillow.x >= secondBoss.x && !m_EnemyListPtrArr[20]->HasDied())	// if Willow enters the boss area and the boss hasn't died yet
		{
			m_PosCamera.x = secondBoss.x;
			m_PosCamera.y = secondBoss.y;

			m_BossBattle = true;
		}

		if(m_BossBattle && !battle1)
		{
			m_EnemyListPtrArr[20]->Spawn();
			m_WillowPtr->SetRestrictedPos(DOUBLE2(secondBoss.x,secondBoss.x+310));
		}
	}
	else
	{
		m_WillowPtr->SetRestrictedPos(DOUBLE2(0,0));
		m_BossBattle = false;
	}
}

void WillowGame::HandleShopSelection(int selection)
{
	// Checks what you want to buy in the shop, if you have enough gold, and gives you what you bought 

	int itemCost;

	switch(selection)
	{
	case 1:
		itemCost = 850;
		if(CheckMoney(itemCost))
		{
			m_WillowPtr->IncreasePower(1);
		}
		break;
	case 2:
		itemCost = 400;
		if(CheckMoney(itemCost))
		{
			m_WillowPtr->AddLifePoint(1);
		}
		break;
	case 3:
		itemCost = 500;
		if(CheckMoney(itemCost))
		{
			m_WillowPtr->AddShield();
		}
		break;
	case 4:
		itemCost = 2000;
		if(CheckMoney(itemCost))
		{
			m_WillowPtr->IncreasePower(2);
		}
		break;
	case 5:
		itemCost = 800;
		if(CheckMoney(itemCost))
		{
			m_WillowPtr->AddLifePoint(2);
		}
		break;
	case 6:
		itemCost = 1000;
		if(CheckMoney(itemCost))
		{
			m_WillowPtr->AddShield();
		}
		break;
	case 7:
		itemCost = 250;
		if(CheckMoney(itemCost))
		{
			m_WillowPtr->AddShield();
		}
		break;
	case 8:
		itemCost = 2000;
		if(CheckMoney(itemCost))
		{
			
		}
		break;
	case 9:
		itemCost = 5000;
		if(CheckMoney(itemCost))
		{
			
		}
		break;
	case 10:
		itemCost = 500;
		if(CheckMoney(itemCost))
		{
			
		}
		break;
	case 11:
		itemCost = 50;
		if(CheckMoney(itemCost))
		{
			
		}
		break;
	}
}

bool WillowGame::CheckMoney(int itemCost)
{
	// this function checks if the player has enough money to buy something from the store
	// if he does, the amount of money will be subtracted

	if(m_MoneyCount >= itemCost)
	{
		m_MoneyCount -= itemCost;
		return true;
	}
	return false;
}

//void WillowGame::CallAction(Caller* callerPtr)
//{
//	// Insert the code that needs to be executed when a Caller has to perform an action
//}