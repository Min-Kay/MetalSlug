#include "framework.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"

CScrollMgr*	CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
{
}


CScrollMgr::~CScrollMgr()
{
	Release();
}

void CScrollMgr::Update(void)
{
}

void CScrollMgr::Late_Update(void)
{
	Check_ScrollLock();
}

void CScrollMgr::Release(void)
{
	Init_Scroll(0);
}

void CScrollMgr::Check_ScrollLock()
{
	if (m_fScrollX > 0) m_fScrollX = 0.f;
	if (m_fScrollX < -scrollXLock) m_fScrollX = -scrollXLock;

	if (m_fScrollY > 0) m_fScrollY = 0.f;
	if (m_fScrollY < -scrollYLock) m_fScrollY = -scrollYLock;
}

