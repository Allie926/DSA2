#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BLACK);
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

	// static displacement float
	static float fDisp = -10.0f;
	fDisp += 0.05f;

	// global translation matrix
	matrix4 m4Space = glm::translate(vector3(fDisp, 0, 0));

	// creates translation matrices for every cube to make the invader image
	matrix4 m4Translation = m4Space * glm::translate(vector3(-2, -2, 0));
	matrix4 m4Translation2 = m4Space * glm::translate(vector3(-1, -2, 0));
	matrix4 m4Translation3 = m4Space * glm::translate(vector3(1, -2, 0));
	matrix4 m4Translation4 = m4Space * glm::translate(vector3(2, -2, 0));
	matrix4 m4Translation5 = m4Space * glm::translate(vector3(-5, -1, 0));
	matrix4 m4Translation6 = m4Space * glm::translate(vector3(-3, -1, 0));
	matrix4 m4Translation7 = m4Space * glm::translate(vector3(3, -1, 0));
	matrix4 m4Translation8 = m4Space * glm::translate(vector3(5, -1, 0));
	matrix4 m4Translation9 = m4Space * glm::translate(vector3(-5, 0, 0));
	matrix4 m4Translation10 = m4Space * glm::translate(vector3(-3, 0, 0));
	matrix4 m4Translation11 = m4Space * glm::translate(vector3(-2, 0, 0));
	matrix4 m4Translation12 = m4Space * glm::translate(vector3(-1, 0, 0));
	matrix4 m4Translation13 = m4Space * glm::translate(vector3(0, 0, 0));
	matrix4 m4Translation14 = m4Space * glm::translate(vector3(1, 0, 0));
	matrix4 m4Translation15 = m4Space * glm::translate(vector3(2, 0, 0));
	matrix4 m4Translation16 = m4Space * glm::translate(vector3(3, 0, 0));
	matrix4 m4Translation17 = m4Space * glm::translate(vector3(5, 0, 0));
	matrix4 m4Translation18 = m4Space * glm::translate(vector3(-5, 1, 0));
	matrix4 m4Translation19 = m4Space * glm::translate(vector3(-4, 1, 0));
	matrix4 m4Translation20 = m4Space * glm::translate(vector3(-3, 1, 0));
	matrix4 m4Translation21 = m4Space * glm::translate(vector3(-2, 1, 0));
	matrix4 m4Translation22 = m4Space * glm::translate(vector3(-1, 1, 0));
	matrix4 m4Translation23 = m4Space * glm::translate(vector3(0, 1, 0));
	matrix4 m4Translation24 = m4Space * glm::translate(vector3(1, 1, 0));
	matrix4 m4Translation25 = m4Space * glm::translate(vector3(2, 1, 0));
	matrix4 m4Translation26 = m4Space * glm::translate(vector3(3, 1, 0));
	matrix4 m4Translation27 = m4Space * glm::translate(vector3(4, 1, 0));
	matrix4 m4Translation28 = m4Space * glm::translate(vector3(5, 1, 0));
	matrix4 m4Translation29 = m4Space * glm::translate(vector3(-4, 2, 0));
	matrix4 m4Translation30 = m4Space * glm::translate(vector3(-3, 2, 0));
	matrix4 m4Translation31 = m4Space * glm::translate(vector3(-1, 2, 0));
	matrix4 m4Translation32 = m4Space * glm::translate(vector3(0, 2, 0));
	matrix4 m4Translation33 = m4Space * glm::translate(vector3(1, 2, 0));
	matrix4 m4Translation34 = m4Space * glm::translate(vector3(3, 2, 0));
	matrix4 m4Translation35 = m4Space * glm::translate(vector3(4, 2, 0));
	matrix4 m4Translation36 = m4Space * glm::translate(vector3(-3, 3, 0));
	matrix4 m4Translation37 = m4Space * glm::translate(vector3(-2, 3, 0));
	matrix4 m4Translation38 = m4Space * glm::translate(vector3(-1, 3, 0));
	matrix4 m4Translation39 = m4Space * glm::translate(vector3(0, 3, 0));
	matrix4 m4Translation40 = m4Space * glm::translate(vector3(1, 3, 0));
	matrix4 m4Translation41 = m4Space * glm::translate(vector3(2, 3, 0));
	matrix4 m4Translation42 = m4Space * glm::translate(vector3(3, 3, 0));
	matrix4 m4Translation43 = m4Space * glm::translate(vector3(-2, 4, 0));
	matrix4 m4Translation44 = m4Space * glm::translate(vector3(2, 4, 0));
	matrix4 m4Translation45 = m4Space * glm::translate(vector3(-3, 5, 0));
	matrix4 m4Translation46 = m4Space * glm::translate(vector3(3, 5, 0));

	//draws every cube
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation2);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation3);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation4);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation5);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation6);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation7);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation8);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation9);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation10);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation11);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation12);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation13);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation14);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation15);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation16);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation17);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation18);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation19);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation20);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation21);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation22);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation23);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation24);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation25);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation26);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation27);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation28);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation29);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation30);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation31);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation32);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation33);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation34);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation35);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation36);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation37);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation38);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation39);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation40);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation41);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation42);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation43);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation44);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation45);
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m4Translation46);

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
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	//release GUI
	ShutdownGUI();
}