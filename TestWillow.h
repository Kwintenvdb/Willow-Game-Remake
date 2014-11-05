//-----------------------------------------------------------------
// Game File
// C++ Source - TestWillow.h - version v2_12 jan 2013 
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Student data
// Name:
// Group:
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "Resource.h"	
#include "GameEngine.h"
#include "AbstractGame.h"
#include "Willow.h"

//-----------------------------------------------------------------
// TestWillow Class																
//-----------------------------------------------------------------
class TestWillow : public AbstractGame, public Callable
{
public:				
	//---------------------------
	// Constructor(s)
	//---------------------------
	TestWillow();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~TestWillow();

	//---------------------------
	// General Methods
	//---------------------------

	void GameInitialize(HINSTANCE hInstance);
	void GameStart();				
	void GameEnd();
	//void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam);
	//void MouseMove(int x, int y, WPARAM wParam);
	//void CheckKeyboard();
	void KeyPressed(TCHAR cKey);
	void GameTick(double deltaTime);
	void GamePaint(RECT rect);

	//void CallAction(Caller* callerPtr);

	// -------------------------
	// Member functions
	// -------------------------

private:
	// -------------------------
	// Member functions
	// -------------------------

	// -------------------------
	// Datamembers
	// -------------------------

	int m_Tickcount, m_Tickdivide;

	Willow *m_WillowPtr;

	HitRegion *m_HitLevelPtr;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	TestWillow(const TestWillow& tRef);
	TestWillow& operator=(const TestWillow& tRef);
};
