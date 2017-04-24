// Main.cpp
#include "../DXFramework/System.h"
#include "AppBoomtown.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//App1* app = new App1();
	AppBoomtown* appBoomtown = new AppBoomtown();
	System* m_System;

	// Create the system object.
	m_System = new System(appBoomtown);

	// Initialize and run the system object.
	m_System->Run();

	// Shutdown and release the system object.
	delete m_System;
	m_System = 0;

	return 0;
}