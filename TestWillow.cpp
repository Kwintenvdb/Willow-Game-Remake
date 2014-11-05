//-----------------------------------------------------------------
// Game File
// C++ Source - TestWillow.cpp - version v2_12 jan 2013 
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "TestWillow.h"																				

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//-----------------------------------------------------------------
// TestWillow methods																				
//-----------------------------------------------------------------

TestWillow::TestWillow(): 
	m_Tickcount()
	,m_Tickdivide(4)
	,m_WillowPtr(nullptr)
	,m_HitLevelPtr(nullptr)
{
	// nothing to create
}

TestWillow::~TestWillow()																						
{
	// nothing to destroy
}

void TestWillow::GameInitialize(HINSTANCE hInstance)			
{
	// Set the required values
	AbstractGame::GameInitialize(hInstance);
	GAME_ENGINE->SetTitle("TestWillow - Name First name - group");					
	GAME_ENGINE->RunGameLoop(true);
	
	// Set the optional values
	GAME_ENGINE->SetWidth(640);
	GAME_ENGINE->SetHeight(480);
	//GAME_ENGINE->SetKeyList(String("QSDZ") + (TCHAR) VK_SPACE);
}

void TestWillow::GameStart()
{
	m_WillowPtr = new Willow(15,100);

	m_HitLevelPtr = new HitRegion;
	m_HitLevelPtr->CreateFromRect(0,200,100,225);
}

void TestWillow::GameEnd()
{
	delete m_WillowPtr;
}

//void TestWillow::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
//{	
//	// Insert the code that needs to be executed when the game registers a mouse button action
//
//	/* Example:
//	if (isLeft == true && isDown == true) // is it a left mouse click?
//	{	
//		if ( x > 261 && x < 261 + 117 ) // check if click lies within x coordinates of choice
//		{
//			if ( y > 182 && y < 182 + 33 ) // check if click also lies within y coordinates of choice
//			{
//				GAME_ENGINE->MessageBox("Clicked.");
//			}
//		}
//	}
//	*/
//}
//
//void TestWillow::MouseMove(int x, int y, WPARAM wParam)
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
//void TestWillow::CheckKeyboard()
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
void TestWillow::KeyPressed(TCHAR cKey)
{	
	
}

void TestWillow::GameTick(double deltaTime)
{
	m_WillowPtr->Tick(deltaTime);
	m_WillowPtr->HitTest(m_HitLevelPtr);
}

void TestWillow::GamePaint(RECT rect)
{
	//m_WillowPtr->Paint();

	GAME_ENGINE->SetColor(COLOR(255,0,0,160));
	GAME_ENGINE->FillHitRegion(m_HitLevelPtr);
}

//void TestWillow::CallAction(Caller* callerPtr)
//{
//	// Insert the code that needs to be executed when a Caller has to perform an action
//}





