#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#pragma comment(lib, "GL\\glut32.lib")
#include <math.h>
#define PI 3.1415926535897932384626433832

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	CGLTexture::PreparateTexturing(false);
	m_tex1.DLoadFromFile(_T("res\\lena_gray.bmp"));
	m_earth_tex.DLoadFromFile(_T("res\\earth.jpg"));

	glEnable(GL_LIGHTING);

	GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 }; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light1_ambient);
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; 
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 }; 
	GLfloat light1_position[] = { -2.0, 2.0, 1.0, 1.0 }; 
	GLfloat spot_direction[] = { 1.0, 1.0, 0.0 }; 
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse); 
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular); 
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_position); 
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5); 
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5); 
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2); 
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0); 
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction); 
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0); 
	glEnable(GL_LIGHT1);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC, float cameraX, float cameraY, float cameraZ)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glLineWidth(2.0);
	glPointSize(10);

	glColor3f(0.5, 0.5, 0.5);
	//DrawRoller(3, 1, 20);


	glEnable(GL_TEXTURE_2D);

	// sphere

	//m_earth_tex.Select();
	m_tex1.Select();

	//DrawSphere(0, 0, 0, 3.0, 100, 100);


	//glPushMatrix();
	//glRotatef(-10, 0.0, 1.0, 0.0);
	//glTranslatef(3.5, 0.0, 0.0);
	//DrawDoghnut(0, 0, 0, 3, 4, 2, 100);
	//glPopMatrix();

	DrawAxes(10);

	//glPushMatrix();

	//glRotatef(-10, 0.0, 1.0, 0.0);
	//glRotatef(90, 1.0, 0.0, 0.0);
	//glPopMatrix();

	//DrawDoghnut(0, 0, 0, 3, 4, 2, 10);

	DrawTorus(0, 0, 0, 4, 0.25, 50);

	// cone

	//DrawCone(0, 0, 0, 2, PI / 4, 4, 10);

	//DrawBox(50.0);

	//glBegin(GL_QUADS);

	//glNormal3d(0.0, 0.0, 1.0);
	//glTexCoord2d(0.0, -1.0);
	//glVertex3d(-2.0, 2.0, 2.0);

	//glTexCoord2d(0.0, 0.0);
	//glVertex3d(-2.0, -2.0, 2.0);

	//glTexCoord2d(1.0, 0.0);
	//glVertex3d(2.0, -2.0, 2.0);

	//glTexCoord2d(1.0, -1.0);
	//glVertex3d(2.0, 2.0, 2.0);

	//DrawBox(2.0);
	//glEnd();
	glDisable(GL_TEXTURE_2D);


	//DrawBox(2.0);

	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / double(h), 1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ...

	m_tex1.Release();
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawRoller(double h, double r, int nSegments)
{

	int nVertices = nSegments + 1 + 1;
	float* lowerBaseVert = new float[nVertices * 3];
	float* lowerBaseNormals = new float[nVertices * 3];
	float* upperBaseVert = new float[nVertices * 3];
	float* upperBaseNormals = new float[nVertices * 3];
	float* shellVertices = new float[2 * nVertices * 3];

	//upperBaseVert[0]

	//float step = 2 * PI / nSegments;


}


void CGLRenderer::DrawAxes(double len) {
	glBegin(GL_LINES);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(len, 0.0, 0.0);

	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, len, 0.0);

	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, len);
	glEnd();

}

void CGLRenderer::DrawBox(float a)
{

	float vert[72];
	float norm[72];
	float texCoords[48];

	float sideNorm[6][3];

	int ind = 0;
	int texInd = 0;

	// left side
	vert[ind++] = -a / 2; vert[ind++] = -a / 2; vert[ind++] = a / 2; // vert 0
	vert[ind++] = a / 2; vert[ind++] = -a / 2; vert[ind++] = a / 2; // vert 1
	vert[ind++] = a / 2; vert[ind++] = a / 2; vert[ind++] = a / 2; // vert 2
	vert[ind++] = -a / 2; vert[ind++] = a / 2; vert[ind++] = a / 2; // vert 3

	sideNorm[0][0] = 0.0; sideNorm[0][1] = 0.0; sideNorm[0][2] = 1.0;


	// front side
	vert[ind++] = a / 2; vert[ind++] = -a / 2; vert[ind++] = a / 2;
	vert[ind++] = a / 2; vert[ind++] = -a / 2; vert[ind++] = -a / 2;
	vert[ind++] = a / 2; vert[ind++] = a / 2; vert[ind++] = -a / 2;
	vert[ind++] = a / 2; vert[ind++] = a / 2; vert[ind++] = a / 2;
	sideNorm[1][0] = 1.0; sideNorm[1][1] = 0.0; sideNorm[1][2] = 0.0;

	// right side
	vert[ind++] = -a / 2; vert[ind++] = a / 2; vert[ind++] = -a / 2;
	vert[ind++] = a / 2; vert[ind++] = a / 2; vert[ind++] = -a / 2;
	vert[ind++] = a / 2; vert[ind++] = -a / 2; vert[ind++] = -a / 2;
	vert[ind++] = -a / 2; vert[ind++] = -a / 2; vert[ind++] = -a / 2;
	sideNorm[2][0] = 0.0; sideNorm[2][1] = 0.0; sideNorm[2][2] = -1.0;

	// back side
	vert[ind++] = -a / 2; vert[ind++] = -a / 2; vert[ind++] = a / 2;
	vert[ind++] = -a / 2; vert[ind++] = a / 2; vert[ind++] = a / 2;
	vert[ind++] = -a / 2; vert[ind++] = a / 2; vert[ind++] = -a / 2;
	vert[ind++] = -a / 2; vert[ind++] = -a / 2; vert[ind++] = -a / 2;
	sideNorm[3][0] = -1.0; sideNorm[3][1] = 0.0; sideNorm[3][2] = 0.0;

	// top side
	vert[ind++] = -a / 2; vert[ind++] = a / 2; vert[ind++] = -a / 2;
	vert[ind++] = -a / 2; vert[ind++] = a / 2; vert[ind++] = a / 2;
	vert[ind++] = a / 2; vert[ind++] = a / 2; vert[ind++] = a / 2;
	vert[ind++] = a / 2; vert[ind++] = a / 2; vert[ind++] = -a / 2;
	sideNorm[4][0] = 0.0; sideNorm[4][1] = 1.0; sideNorm[4][2] = 0.0;

	// bottom side
	vert[ind++] = -a / 2; vert[ind++] = -a / 2; vert[ind++] = a / 2;
	vert[ind++] = -a / 2; vert[ind++] = -a / 2; vert[ind++] = -a / 2;
	vert[ind++] = a / 2; vert[ind++] = -a / 2; vert[ind++] = -a / 2;
	vert[ind++] = a / 2; vert[ind++] = -a / 2; vert[ind++] = a / 2;
	sideNorm[5][0] = 0.0; sideNorm[5][1] = -1.0; sideNorm[5][2] = 0.0;


	for (int i = 0; i < 6; i++)
	{
		texCoords[i * 8 + 0] = 0.0;
		texCoords[i * 8 + 1] = 0.0;

		texCoords[i * 8 + 2] = 1.0;
		texCoords[i * 8 + 3] = 0.0;

		texCoords[i * 8 + 4] = 1.0;
		texCoords[i * 8 + 5] = -1.0;

		texCoords[i * 8 + 6] = 0.0;
		texCoords[i * 8 + 7] = -1.0;

		for (int j = 0; j < 4; j++)
			// normals
			for (int p = 0; p < 3; p++)
				norm[i * 4 * 3 + j * 3 + p] = sideNorm[i][p];
	}

	glVertexPointer(3, GL_FLOAT, 0, vert);
	glNormalPointer(GL_FLOAT, 0, norm);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


}

void CGLRenderer::DrawSphere(float cx, float cy, float cz, float r, int sectors, int stacks)
{
	glPushMatrix();

	glTranslatef(cx, cy, cz);

	int size = (stacks + 1) * (sectors + 1) * 3;
	float* vert = new float[size];
	float* norm = new float[size];
	float* texCoords = new float[(stacks + 1) * (sectors + 1) * 2];
	int nIndices = (sectors - 1) * stacks * 6;
	unsigned int* indices = new unsigned int[nIndices];

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / stacks;
	float theta, phi, s, t;

	int ind = 0, texInd = 0;
	for (int i = 0; i <= stacks; ++i) {
		theta = PI / 2 - i * stackStep;

		float y = r * sinf(theta);

		for (int j = 0; j <= sectors; ++j) {

			phi = j * sectorStep;

			float x = r * cosf(theta) * sin(phi);
			float z = r * cosf(theta) * cos(phi);

			vert[ind + 0] = x;
			vert[ind + 1] = y;
			vert[ind + 2] = z;

			norm[ind + 0] = x / r;
			norm[ind + 1] = y / r;
			norm[ind + 2] = z / r;

			ind += 3;

			s = (float)j / sectors;
			t = (float)i / stacks;
			texCoords[texInd + 0] = s;
			texCoords[texInd + 1] = t;
			texInd += 2;
		}
	}

	ind = 0;
	int k1, k2;
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (sectors + 1);     // beginning of current stack
		k2 = k1 + sectors + 1;      // beginning of next stack

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices[ind++] = k1;
				indices[ind++] = k2;
				indices[ind++] = k1 + 1;
			}

			// k1+1 => k2 => k2+1
			if (i != (stacks - 1))
			{
				indices[ind++] = k1 + 1;
				indices[ind++] = k2;
				indices[ind++] = k2 + 1;
			}
		}
	}


	glVertexPointer(3, GL_FLOAT, 0, vert);
	glNormalPointer(GL_FLOAT, 0, norm);
	//glIndexPointer(GL_FLOAT, 0, indices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);


	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_INDEX_ARRAY);


	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, (void*)indices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_INDEX_ARRAY);

	glPopMatrix();
}

void CGLRenderer::DrawCone(float cx, float cy, float cz, float r, float coneAngle, float h, int segments)
{
	glPushMatrix();

	glTranslatef(cx, cy, cz);

	float step = 2 * PI / (float)segments;

	float upperPoint[3] = { 0.0, 0.0, 0.0 };
	float lowerPoint[3] = { 0.0, h, 0.0 };

	// base
	int nVertices = segments + 1 + 1;
	float* vert = new float[nVertices * 3];
	float* norm = new float[nVertices * 3];
	float* shellNormals = new float[nVertices * 3];
	int nIndices = segments * 3;
	unsigned int* indices = new unsigned int[nIndices];



	float c = sqrt(r * r + h * h);

	vert[0] = 0.0;

	norm[0] = 0.0;
	vert[1] = 0.0;
	vert[2] = 0.0;
	norm[1] = -1.0;
	norm[2] = 0.0;

	shellNormals[0] = 0.0;
	shellNormals[1] = 1.0;
	shellNormals[2] = 0.0;


	int ind = 3;

	float alpha = atan(r / h);
	for (int i = 0; i <= segments; ++i) {
		float theta = i * step;
		

		float x = cosf(theta) * r;
		float y = 0;
		float z = sin(theta) * r;

		vert[ind] = x;
		vert[ind + 1] = y;
		vert[ind + 2] = z;
		
		norm[ind + 0] = 0.0f;
		norm[ind + 1] = -1.0;
		norm[ind + 2] = 0.0;

		shellNormals[ind + 0] = x * h / c;
		shellNormals[ind + 1] = tan(alpha);
		shellNormals[ind + 2] = z * r / c;

		glBegin(GL_LINES);
		glVertex3f(x, y, z);
		glVertex3f(x + shellNormals[ind + 0], y + shellNormals[ind + 1], z + shellNormals[ind + 2]);
		glEnd();


		ind += 3;
	}

	ind = 0;
	for (int i = 1; i <= segments; ++i) {
		indices[ind++] = 0;
		indices[ind++] = i;
		indices[ind++] = i + 1;
	}

	// base
	glVertexPointer(3, GL_FLOAT, 0, vert);
	glNormalPointer(GL_FLOAT, 0, norm);
	//glTexCoordPointer(2, GL_FLOAT, 0, texCoords);


	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);


	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, indices);

	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);


	// shell

	vert[1] = h;

	glVertexPointer(3, GL_FLOAT, 0, vert);
	glNormalPointer(GL_FLOAT, 0, shellNormals);
	//glTexCoordPointer(2, GL_FLOAT, 0, texCoords);


	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);


	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, indices);

	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
}

void CGLRenderer::DrawDoghnut(float cx, float cy, float cz, float r1, float r2, float h, int segments)
{
	glPushMatrix();

	glTranslatef(cx, cy, cz);

	float step = 2 * PI / segments;
	int nVertices = segments + 1;

	float* vert = new float[2 * nVertices * 3];
	float* vertOuter = new float[2 * nVertices * 3];
	float* texOuterCoords = new float[2 * nVertices * 2];
	float* vertUpper = new float[2 * nVertices * 3];
	float* vertLower = new float[2 * nVertices * 3];

	float* norm = new float[2 * nVertices * 3];
	float* normOuter = new float[2 * nVertices * 3];
	float* normUpper = new float[2 * nVertices * 3];
	float* normLower = new float[2 * nVertices * 3];

	int ind = 0, texInd = 0;
	for (int i = 0; i < nVertices; ++i) {

		// manji omotac
		float x1 = r1 * cos(i * step);
		float y1 = h/2;
		float z1 = r1 * sin(i * step);

		float x2 = r1 * cos(i * step);
		float y2 = - h / 2;
		float z2 = r1 * sin(i * step);
		
		vert[ind] = x1;
		vert[ind + 1] = y1;
		vert[ind + 2] = z1;

		vert[ind + 3] = x2;
		vert[ind + 4] = y2;
		vert[ind + 5] = z2;

		norm[ind] = -x1 / r1;
		norm[ind + 1] = 0;
		norm[ind + 2] = -z1 / r1;

		norm[ind + 3] = -x2 / r1;
		norm[ind + 4] = 0;
		norm[ind + 5] = -z2 / r1;


		// veci omotac

		double bigX1 = r2 * cos(i * step);
		double bigY1 = h / 2;
		double bigZ1 = r2 * sin(i * step);

		double bigX2 = r2 * cos(i * step);
		double bigY2 = -h / 2;
		double bigZ2 = r2 * sin(i * step);

		vertOuter[ind] = bigX1;
		vertOuter[ind + 1] = bigY1;
		vertOuter[ind + 2] = bigZ1;

		vertOuter[ind + 3] = bigX2;
		vertOuter[ind + 4] = bigY2;
		vertOuter[ind + 5] = bigZ2;

		normOuter[ind] = bigX1 / r2;
		normOuter[ind + 1] = 0;
		normOuter[ind + 2] = bigZ1 / r2;

		normOuter[ind + 3] = bigX2 / r2;
		normOuter[ind + 4] = 0;
		normOuter[ind + 5] = bigZ2 / r2;

		texOuterCoords[texInd] = 0.5 + 0.125;
		texOuterCoords[texInd + 1] = 1 - (float) i / segments;

		texOuterCoords[texInd + 2] = 0.5 - 0.125;
		texOuterCoords[texInd + 3] = 1 - (float)(i + 1) / segments;

		// donji sloj

		vertLower[ind] = x2;
		vertLower[ind + 1] = y2;
		vertLower[ind + 2] = z2;

		vertLower[ind + 3] = bigX2;
		vertLower[ind + 4] = bigY2;
		vertLower[ind + 5] = bigZ2;

		normLower[ind] = 0.0;
		normLower[ind + 1] = -1.0;
		normLower[ind + 2] = 0.0;

		normLower[ind + 3] = 0.0;
		normLower[ind + 4] = -1.0;
		normLower[ind + 5] = 0.0;


		// gornji sloj

		vertUpper[ind] = x1;
		vertUpper[ind + 1] = y1;
		vertUpper[ind + 2] = z1;

		vertUpper[ind + 3] = bigX1;
		vertUpper[ind + 4] = bigY1;
		vertUpper[ind + 5] = bigZ1;

		normUpper[ind] = 0.0;
		normUpper[ind + 1] = 1.0;
		normUpper[ind + 2] = 0.0;

		normUpper[ind + 3] = 0.0;
		normUpper[ind + 4] = 1.0;
		normUpper[ind + 5] = 0.0;

		ind += 6;
		texInd += 4;
	}



	glVertexPointer(3, GL_FLOAT, 0, vert);
	glNormalPointer(GL_FLOAT, 0, norm);


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);


	glDrawArrays(GL_QUAD_STRIP, 0, 2 * nVertices);

	glVertexPointer(3, GL_FLOAT, 0, vertLower);
	glNormalPointer(GL_FLOAT, 0, normLower);

	glDrawArrays(GL_QUAD_STRIP, 0, 2 * nVertices);

	glVertexPointer(3, GL_FLOAT, 0, vertUpper);
	glNormalPointer(GL_FLOAT, 0, normUpper);

	glDrawArrays(GL_QUAD_STRIP, 0, 2 * nVertices);



	glTexCoordPointer(2, GL_FLOAT, 0, texOuterCoords);


	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertOuter);
	glNormalPointer(GL_FLOAT, 0, normOuter);

	glDrawArrays(GL_QUAD_STRIP, 0, 2 * nVertices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);


	glPopMatrix();

}

void CGLRenderer::DrawTorus(float cx, float cy, float cz, float r, float h, int segments)
{

	glPushMatrix();

	glTranslatef(cx, cy, cz);

	float step = 2 * PI / segments;
	float thetaStep = 2 * PI / segments;

	glBegin(GL_QUADS);

	for (int i = 0; i < segments; i++) {
		for (int j = 0; j < segments; j++) {
				float phi = i * step;
				float theta = j  * thetaStep;

				float promenljiva = cos(theta) * h + r;

				float x = promenljiva * sin(phi);
				float y = sin(theta) * h;
				float z = promenljiva * cos(phi);

				glVertex3f(x, y, z);
				glNormal3f(sin(phi) * cos(theta), y / h, cos(phi) * cos(theta));


				phi = (i + 0) * step;
				theta = (j + 1) * thetaStep;

				promenljiva = cos(theta) * h + r;

				x = promenljiva * sin(phi);
				y = sin(theta) * h;
				z = promenljiva * cos(phi);

				glVertex3f(x, y, z);
				glNormal3f(sin(phi) * cos(theta), y / h, cos(phi) * cos(theta));

				phi = (i + 1) * step;
				theta = (j + 1) * thetaStep;

				promenljiva = cos(theta) * h + r;

				x = promenljiva * sin(phi);
				y = sin(theta) * h;
				z = promenljiva * cos(phi);

				glVertex3f(x, y, z);
				glNormal3f(sin(phi) * cos(theta), y / h, cos(phi) * cos(theta));

				phi = (i + 1) * step;
				theta = (j + 0) * thetaStep;

				promenljiva = cos(theta) * h + r;

				x = promenljiva * sin(phi);
				y = sin(theta) * h;
				z = promenljiva * cos(phi);

				glVertex3f(x, y, z);
				glNormal3f(sin(phi) * cos(theta), y / h, cos(phi) * cos(theta));

		}
	}
	glEnd();
	glPopMatrix();
}

