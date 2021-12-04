#include "CBitMap.h"
#include "Include.h"

CBitMap::CBitMap()
{
}


CBitMap::~CBitMap()
{
}

void CBitMap::Load_Bmp(const TCHAR* pFilePath)
{
	HDC			hDC = GetDC(g_hwnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hwnd, hDC);

	m_hBitBmp = (HBITMAP)LoadImage(NULL,pFilePath,IMAGE_BITMAP,0,0,	LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (nullptr == m_hBitBmp)
	{
		MessageBox(g_hwnd, L"비트맵 로드 실패", L"실패", MB_OK);
		return;
	}

	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitBmp);
}

void CBitMap::Release(void)
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitBmp);
	DeleteDC(m_hMemDC);
}