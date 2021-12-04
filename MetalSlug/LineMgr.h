#pragma once

#include "Line.h"
#include "KeyMgr.h"

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();

public:
	void		Initialize(void);
	void		Update(void);
	void		Late_Update(void);
	void		Render(HDC _DC);
	void		Release(void);


public:
	static	CLineMgr* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CLineMgr;

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

public:
	void	Load(const TCHAR* _File);

public:
	bool	Collision_Line(const float& _fX, const float& _fY, float* _fNewY);

private:
	static CLineMgr*	m_pInstance;

	list<CLine*>		m_LineList; // 현재 사용중인 라인 리스트
};

