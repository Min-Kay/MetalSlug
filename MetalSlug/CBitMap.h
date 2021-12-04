#pragma once
#include "framework.h"

class CBitMap
{
public:
	CBitMap();
	~CBitMap();
public:
	HDC		Get_MemDC() { return m_hMemDC; }
	void	Load_Bmp(const TCHAR* pFilePath);
	void	Release(void);

private:
	HDC			m_hMemDC;

	HBITMAP		m_hBitBmp;
	HBITMAP		m_hOldBmp;
};

