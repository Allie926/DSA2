#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	vector3 center = vector3(0, 0, -a_fHeight / 2); // point of center of base of cone
	vector3 top = vector3(0, 0, a_fHeight / 2); // point of tip of cone

	vector3 points[360]; // points on the base of the cone

	float angle = (2.0f*(float)PI) / a_nSubdivisions;
	float currAngle = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// calculate the x and y values of the point on the circle
		float x = glm::cos(currAngle) * a_fRadius;
		float y = glm::sin(currAngle) * a_fRadius;

		// add the point vector to points
		points[i] = vector3(x, y, -a_fHeight / 2);

		// increment the current angle by the angle between each point
		currAngle += angle;
	}

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// for all subdivisions except the last, creates tris using the current point and next point
		if (i < a_nSubdivisions - 1)
		{
			// creates the tri for the two points and the top of the cone
			AddTri(points[i], points[i + 1], top);

			// creates the tri for the two points and the center of the base
			AddTri(points[i], center, points[i+1]);
		}
		// for the last subdivision, make the third point of the tri the first point in the array
		else
		{
			// creates the tri for the two points and the top of the cone
			AddTri(points[i], points[0], top);

			// creates the tri for the two points and the center of the base
			AddTri(points[i], center, points[0]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	vector3 centerBottom = vector3(0, 0, -a_fHeight / 2); // point of center of base of cylinder
	vector3 centerTop = vector3(0, 0, a_fHeight / 2); // point of the center of the top base of cylinder

	vector3 pointsBottom[360]; // array of points on the bottom face
	vector3 pointsTop[360]; // array of points on the top face

	float angle = (2.0f*(float)PI) / a_nSubdivisions;
	float currAngle = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// calculate the x and y values of the point
		float x = glm::cos(currAngle) * a_fRadius;
		float y = glm::sin(currAngle) * a_fRadius;

		// add the point vector to each point array
		pointsBottom[i] = vector3(x, y, -a_fHeight / 2);
		pointsTop[i] = vector3(x, y, a_fHeight / 2);

		// increment the current angle by the angle between each point
		currAngle += angle;
	}

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// for all subdivisions except the last, creates tris using the current point, and the next point
		if (i < a_nSubdivisions - 1)
		{
			// bottom face
			AddTri(pointsBottom[i], centerBottom, pointsBottom[i + 1]);

			// side faces
			AddQuad(pointsBottom[i], pointsBottom[i + 1], pointsTop[i], pointsTop[i + 1]);

			// top face
			AddTri(pointsTop[i], pointsTop[i + 1], centerTop);
		}
		// for the last subdivision, make the third point of the tri the first point in the array
		else
		{
			// bottom face
			AddTri(pointsBottom[i], centerBottom, pointsBottom[0]);

			// side faces
			AddQuad(pointsBottom[i], pointsBottom[0], pointsTop[i], pointsTop[0]);

			// top face
			AddTri(pointsTop[i], pointsTop[0], centerTop);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	vector3 pointsBottomInner[360]; // array of vectors on the bottom inside circle
	vector3 pointsBottomOuter[360]; // array of vectors on the bottom outside circle
	vector3 pointsTopInner[360]; // array of vectors on the top inside circle
	vector3 pointsTopOuter[360]; // array of vectors on the top outside circle

	float angle = (2.0f*(float)PI) / a_nSubdivisions;
	float currAngle = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// calculate the x and y values of the inner radius points
		float xInner = glm::cos(currAngle) * a_fInnerRadius;
		float yInner = glm::sin(currAngle) * a_fInnerRadius;

		// calculate the x and y values of the outer radius points
		float xOuter = glm::cos(currAngle) * a_fOuterRadius;
		float yOuter = glm::sin(currAngle) * a_fOuterRadius;

		// add the point vectors to inner points
		pointsBottomInner[i] = vector3(xInner, yInner, -a_fHeight / 2);
		pointsTopInner[i] = vector3(xInner, yInner, a_fHeight / 2);

		// add the point vectors to outer points
		pointsBottomOuter[i] = vector3(xOuter, yOuter, -a_fHeight / 2);
		pointsTopOuter[i] = vector3(xOuter, yOuter, a_fHeight / 2);

		// increment the current angle by the angle between each point
		currAngle += angle;
	}

	for (int i = 0; i < a_nSubdivisions; i++)
	{
	// for all subdivisions except the last, creates quads for all faces
	if (i < a_nSubdivisions - 1)
	{
		// bottom face
		AddQuad(pointsBottomOuter[i], pointsBottomInner[i], pointsBottomOuter[i + 1], pointsBottomInner[i + 1]);

		// inside face
		AddQuad(pointsBottomInner[i + 1], pointsBottomInner[i], pointsTopInner[i + 1], pointsTopInner[i]);

		// outside face
		AddQuad(pointsBottomOuter[i], pointsBottomOuter[i + 1], pointsTopOuter[i], pointsTopOuter[i + 1]);

		// top face
		AddQuad(pointsTopInner[i], pointsTopOuter[i], pointsTopInner[i + 1], pointsTopOuter[i + 1]);
	}
	// for the last subdivision, use the first point instead of the i+1 point
	else
	{
		// bottom face
		AddQuad(pointsBottomOuter[i], pointsBottomInner[i], pointsBottomOuter[0], pointsBottomInner[0]);

		// inside face
		AddQuad(pointsBottomInner[0], pointsBottomInner[i], pointsTopInner[0], pointsTopInner[i]);

		// outside face
		AddQuad(pointsBottomOuter[i], pointsBottomOuter[0], pointsTopOuter[i], pointsTopOuter[0]);

		// top face
		AddQuad(pointsTopInner[i], pointsTopOuter[i], pointsTopInner[0], pointsTopOuter[0]);
	}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	float a = (a_fOuterRadius - a_fInnerRadius) / 2;

	float c = a_fInnerRadius + a;


	float angle1 = (2.0f*(float)PI) / a_nSubdivisionsA; // angle around the tube circle
	float angle2 = (2.0f*(float)PI) / a_nSubdivisionsB; // angle around the whole torus

	float currAngle1 = 0; // current tube angle
	float currAngle2 = 0; // current torus angle

	vector3 points[360]; // array of points in each subdivision of the tube
	vector3 pointsPrev[360]; // array of previous points in each subdivision of the tube

	// loop for all subdivisions + 1
	for (int i = 0; i < a_nSubdivisionsB + 1; i++)
	{
		// populate the previous points array for every index past the first
		if (i != 0)
		{
			for (int j = 0; j < a_nSubdivisionsA; j++)
			{
				pointsPrev[j] = points[j];
			}
		}

		// loop again to calculate tube points
		for (int j = 0; j < a_nSubdivisionsA; j++)
		{
			float x = (c + a * glm::cos(currAngle2)) * glm::cos(currAngle1);
			float y = (c + a * glm::cos(currAngle2)) * glm::sin(currAngle1);
			float z = a * glm::sin(currAngle2);

			points[j] = vector3(x, y, z);

			// increment the tube angle
			currAngle1 += angle1;
		}

		// increment the torus angle
		currAngle2 += angle2;

		// for all indices except the first
		if (i != 0)
		{
			// loop to draw quads
			for (int j = 0; j < a_nSubdivisionsA; j++)
			{
				// if not at the last subdivision, create a quad using the current index previous point, the next index previous point,
				// the current point, and the next point
				if (j < a_nSubdivisionsA - 1)
				{
					AddQuad(pointsPrev[j], pointsPrev[j + 1], points[j], points[j + 1]);
				}
				// if not at the last subdivision, create a quad using the current index previous point, the first index previous point,
				// the current point, and the first point
				else
				{
					AddQuad(pointsPrev[j], pointsPrev[0], points[j], points[0]);
				}
			}
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 10)
		a_nSubdivisions = 10;

	Release();
	Init();

	float angle1 = (2.0f*(float)PI) / a_nSubdivisions; // angle around the sphere horizontally
	float angle2 = (float)PI / a_nSubdivisions; // angle around the sphere vertically

	float currAngle1 = 0; // current horizontal angle
	float currAngle2 = angle2; // current vertical angle

	vector3 points[10]; // array of points in each subdivision of the sphere
	vector3 pointsPrev[10]; // array of previous points calculated in each subdivision of the sphere
	vector3 top = vector3(0, 0, a_fRadius); // vector of the top point of the sphere
	vector3 bottom = vector3(0, 0, -a_fRadius); // vector of the bottom point of the sphere

	// loop for all subdivisions
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// loop again to calculate horizontal points
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			// don't calculate previous points for the first subdivision
			if (i != 0)
			{
				pointsPrev[j] = points[j];
			}

			// don't calculate the current point for the last subdivision (the bottom point)
			if (i != a_nSubdivisions - 1)
			{
				// calculate each point horizontally around the sphere
				float x = glm::sin(currAngle2) * glm::cos(currAngle1);
				float y = glm::sin(currAngle2) * glm::sin(currAngle1);
				float z = glm::cos(currAngle2);

				points[j] = vector3(x, y, z) * a_fRadius;
			}
			
			// increment the horizontal angle
			currAngle1 += angle1;
		}

		// increment the vertical angle
		currAngle2 += angle2;

		// loop to add tris/quads for the sphere faces
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			// for the top of the sphere, use tris
			if (i == 0)
			{
				// if not at the last subdivision, create a tri using the current point, the next point, and the top
				if (j < a_nSubdivisions - 1)
				{
					AddTri(points[j], points[j + 1], top);
				}
				// if at the last subdivision, create a tri using the current point, the first point, and the top
				else
				{
					AddTri(points[j], points[0], top);
				}
			}
			// for the middle of sphere, use quads
			else if (i > 0 && i < a_nSubdivisions - 1)
			{
				// if not at the last subdivision, create a quad using the current point, the next point,
				// the current index of previous points, and the next index of previous points
				if (j < a_nSubdivisions - 1)
				{
					AddQuad(points[j], points[j + 1], pointsPrev[j], pointsPrev[j + 1]);
				}
				// if at the last subdivision, create a quad using the current point, the first point,
				// the current index of previous points, and the first index of previous points
				else
				{
					AddQuad(points[j], points[0], pointsPrev[j], pointsPrev[0]);
				}
			}
			// for the bottom of the sphere, use tris
			else
			{
				// if not at the last subdivision, create a tri using the current index of previous points, 
				// the next index of previous points, and the bottom
				if (j < a_nSubdivisions - 1)
				{
					AddTri(pointsPrev[j], bottom, pointsPrev[j+1]);
				}
				// if at the last subdivision, create a tri using the current index of previous points, 
				// the first index of previous points, and the bottom
				else
				{
					AddTri(pointsPrev[j], bottom, pointsPrev[0]);
				}
			}
		}
		
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}