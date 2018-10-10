#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Allie Sangalli - axs9711@rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits
	float fRadius = 0.95f; //initial radius of each orbit

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		
		std::vector<vector3> stopsList; //declaration of list of stop positions for circle to lerp 
		float angle = (2.0f*PI) / i; //total angle between each endpoint of each orbit
		float currAngle = 0; //current angle, initially 0

		//add each endpoint to the stop positions list, then increases the current angle
		for (int j = 0; j < i; j++)
		{
			stopsList.push_back(vector3(cos(currAngle)*fRadius, sin(currAngle)*fRadius, 0));
			currAngle += angle;
		}

		m_totalStopsList.push_back(stopsList); //adds the list of stops for current orbit to the total stops list
		fSize += 0.5f; //increment the size for the next orbit
		fRadius += 0.5f; //increment the radius for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	//m4Offset = glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Z);

	static float percent = 0;
	static int index = 0;

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 1.5708f, AXIS_X));

		int size = m_totalStopsList[i].size(); //number of endpoints on the current orbit

		vector3 startPoint = m_totalStopsList[i][index % size]; //start point on the current orbit
		vector3 endPoint = m_totalStopsList[i][(index+1) % size]; //end point on the current orbit

		//calculate the current position
		vector3 v3CurrentPos = glm::lerp(startPoint, endPoint, percent);
		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

	//reset percent when at 100%, then increment the index
	if (percent >= 1.0f)
	{
		percent = 0;
		index++;
	}

	percent += 0.03f; //increment the percent every frame

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
	//release GUI
	ShutdownGUI();
}