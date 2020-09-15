#pragma once
#include "GLTexture.h"

class CGLRenderer
{
public:
	CGLTexture m_tex1, m_earth_tex;
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC, float cameraX, float cameraY, float cameraZ);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawRoller(double h, double r, int nSegments);
	void DrawAxes(double len);
	void DrawBox(float a);
	void DrawSphere(float cx, float cy, float cz, float r, int sectors, int stacks);
	void DrawCone(float cx, float cy, float cz, float r, float coneAngle, float h, int segments);
	void DrawDoghnut(float cx, float cy, float cz, float r1, float r2, float h, int segments);
	void DrawTorus(float cx, float cy, float cz, float r, float h, int segments);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
