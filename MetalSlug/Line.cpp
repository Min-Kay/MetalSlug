#include "framework.h"
#include "Line.h"
#include "Manager.h"

CLine::CLine()
{
}

CLine::CLine(LINEINFO& _info)
	: m_tInfo(_info)
{
}


CLine::CLine(LINEPOS& _tLeft, LINEPOS& _tRight)
	: m_tInfo(_tLeft, _tRight)
{

}

CLine::~CLine()
{
}

void CLine::Render(HDC _DC)
{
	int scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	MoveToEx(_DC, (int)m_tInfo.tLeftPos.fX + scrollX, (int)m_tInfo.tLeftPos.fY + scrollY, nullptr);
	LineTo(_DC, (int)m_tInfo.tRightPos.fX + scrollX, (int)m_tInfo.tRightPos.fY + scrollY);
}
