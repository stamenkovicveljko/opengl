#include "StdAfx.h"
#include "GLTexture.h"
#include "GL\glaux.h"
#pragma comment (lib, "glaux.lib")

CGLTexture::CGLTexture()
{
}

CGLTexture::~CGLTexture()
{
	Release();
}

void CGLTexture::LoadFromFile(CString texFileName)
{
	if (m_ID != 0) Release();

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	AUX_RGBImageRec* TextureImage;
	TextureImage = auxDIBImageLoad(texFileName);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);

	if (TextureImage)
	{
		if (TextureImage->data)
		{
			free(TextureImage->data);
		}
		free(TextureImage);
	}
}

void CGLTexture::Select()
{
	if (m_ID)
		glBindTexture(GL_TEXTURE_2D, m_ID);
}

void CGLTexture::Release()
{
	if (m_ID) {
		glDeleteTextures(1, &m_ID);
		m_ID = 0;
	}
}

void CGLTexture::PreparateTexturing(bool bEnableLighting)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	if (bEnableLighting)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	else
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
