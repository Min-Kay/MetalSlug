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
	Init_Scroll(0,0);
}

void CScrollMgr::Check_ScrollLock()
{

	if (0 < m_fScrollX)
		m_fScrollX = 0.f;

	if (WINCX - scrollXLock > m_fScrollX)
		m_fScrollX = WINCX - scrollXLock;

	if (0 < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCY - scrollYLock > m_fScrollY)
		m_fScrollY = WINCY - scrollYLock;
}

