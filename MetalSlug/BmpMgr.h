#pragma once
#include "framework.h"

class CBitMap;

class BmpMgr
{
private:
	BmpMgr();
	~BmpMgr();

public:
	void		Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC			Find_Image(const TCHAR* pImgKey);
	void		Release(void);

public:
	static	BmpMgr* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new BmpMgr;

		return m_pInstance;
	}

	static	void		Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static BmpMgr* m_pInstance;
	map<const TCHAR*, CBitMap*>		m_mapBit;
};

