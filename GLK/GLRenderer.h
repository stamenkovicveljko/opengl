#pragma once
#include "GLTexture.h"

class CGLRenderer
{
public:
	CGLTexture m_tex1;
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawRoller(double h, double r, int nSegments);
	void DrawAxes(double len);
	void DrawBox(float a);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
