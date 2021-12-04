#pragma once

#include "Include.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	void		Initialize(void) {}
	void		Update(void);
	void		Late_Update(void);
	void		Render(HDC _DC) {}
	void		Release(void);

public:
	float			Get_ScrollX() { return m_fScrollX; }
	float			Get_ScrollY() { return m_fScrollY; }
	void			Set_ScrollX(float _fX) {m_fScrollX += _fX;  }
	void			Set_ScrollY(float _fy) { m_fScrollY += _fy; }
	void			Init_Scroll(float _fX, float _fY = 0.f) {m_fScrollX = _fX; m_fScrollY = 0.f;}

	void			Set_ScrollLockX(float _fx) { scrollXLock = _fx - WINCX; }
	void			Set_ScrollLockY(float _fy) { scrollYLock = _fy - WINCY; }

	void			Check_ScrollLock();
public:
	static	CScrollMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CScrollMgr;

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
	static CScrollMgr*	m_pInstance;
	float				m_fScrollX = 0.f;
	float				m_fScrollY = 0.f;

	float				scrollXLock = 0.f;
	float				scrollYLock = 0.f;

};

