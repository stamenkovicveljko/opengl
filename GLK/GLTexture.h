#pragma once
class CGLTexture
{

public:
	unsigned int m_ID;
	CGLTexture();
	~CGLTexture();
	void LoadFromFile(CString textFileName);
	void DLoadFromFile(CString texFileName);
	void Select();
	void Release();
	static void PreparateTexturing(bool bEnableLighting);
};

