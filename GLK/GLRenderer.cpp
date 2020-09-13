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
	m_tex1.LoadFromFile(_T("res\\lena_gray.bmp"));

	//GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 }; 
	//GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; 
	//GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 }; 
	//GLfloat light1_position[] = { -2.0, 2.0, 1.0, 1.0 }; 
	//GLfloat spot_direction[] = { -1.0, -1.0, 0.0 }; 
	//glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient); 
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse); 
	//glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular); 
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_position); 
	//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5); 
	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5); 
	//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2); 
	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0); 
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction); 
	//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0); 
	//glEnable(GL_LIGHT1);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glLineWidth(2.0);
	glPointSize(10);

	glColor3f(0.5, 0.5, 0.5);

	glEnable(GL_TEXTURE_2D);
	m_tex1.Select();
	DrawBox(3.0);
	glDisable(GL_TEXTURE_2D);

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

	float* buf = new float[3 * nSegments];

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, 0);

	double step = 2 * PI / nSegments;
	for (int i = 0; i < nSegments; i++) {
		double x = cos(i * step) * r;
		double y = 0;
		double z = sin(i * step) * r;

		glVertex3d(x, y, z);
		buf[3 * i + 0] = x;
		buf[3 * i + 1] = y;
		buf[3 * i + 2] = z;
	}
	glVertex3d(buf[0], buf[1], buf[2]);

	glEnd();



	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, 0);

	for (int i = 0; i < nSegments; i++) {
		glVertex3d(buf[3 * i + 0], buf[3 * i + 1] + h, buf[3 * i + 2]);
	}

	glVertex3d(buf[0], buf[1] + h, buf[2]);
	glEnd();



	glBegin(GL_QUAD_STRIP);

	for (int i = 0; i < nSegments; i++) {
		glVertex3d(buf[3 * i + 0], buf[3 * i + 1], buf[3 * i + 2]);
		glVertex3d(buf[3 * i + 0], buf[3 * i + 1] + h, buf[3 * i + 2]);
	}


	glVertex3d(buf[0], buf[1], buf[2]);
	glVertex3d(buf[0], buf[1] + h, buf[2]);
	glEnd();
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
		texCoords[i * 8 + 5] = 1.0;

		texCoords[i * 8 + 6] = 0.0;
		texCoords[i * 8 + 7] = 1.0;

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
