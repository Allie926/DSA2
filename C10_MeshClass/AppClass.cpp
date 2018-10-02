#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(2.0f, C_BROWN);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	static DWORD dStartingTime = GetTickCount(); // time the program starts
	DWORD dCurrentTime = GetTickCount(); // milliseconds at current frame
	DWORD dDelta = dCurrentTime - dStartingTime; // delta of current time and start time
	float fTimer = static_cast<float>(dDelta / 1000.0f); // delta time in seconds
	static float time = 1.0f; // time to lerp

	float fPercent = MapValue(fTimer, 0.0f, time, 0.0f, 1.0f);

	static vector3 v3InitialPoint(0, 0, 0); // initial point
	static vector3 v3EndPoint(5, 0, 0); // end point

	float fStart = 0.0f;
	float fEnd = 180.0f;
	float fCurrent = glm::lerp(fStart, fEnd, fPercent);

	// position vector, lerps by percent
	// is fully at the target point (100%) when the timer reaches time
	// does the same thing as lerping by fTimer/time
	vector3 v3Position = glm::lerp(v3InitialPoint, v3EndPoint, fPercent); 

	//matrix4 m4Rotation = glm::rotate(v3Position, fCurrent);
	matrix4 m4Position = glm::translate(IDENTITY_M4, v3Position); // translation matrix

	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Position);
		
	// if percent is 100 (reached final point), swap start and end points and reset start time
	// same thing as fTimer == time
	if (fPercent >= 1.0f)
	{
		std::swap(v3InitialPoint, v3EndPoint);
		dStartingTime = GetTickCount();
	}

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();

	//dStartingTime = GetTickCount();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}