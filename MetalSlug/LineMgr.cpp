#include "framework.h"
#include "LineMgr.h"
#include "ScrollMgr.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
}

CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize(void)
{
}

void CLineMgr::Update(void)
{
}

void CLineMgr::Late_Update(void)
{
}

void CLineMgr::Render(HDC _DC)
{
	for (auto& iter : m_LineList)
		iter->Render(_DC);
}

void CLineMgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), CDeleteObj());
	m_LineList.clear();
}

void CLineMgr::Load(const TCHAR* _File) 
{
	HANDLE			hFile = CreateFile(_File, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte = 0;
	LINEINFO	tInfo{};

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(LINEINFO), &dwByte, NULL);

		if (0 == dwByte)
			break;

		m_LineList.push_back(new CLine(tInfo));
	}

	CloseHandle(hFile);
}

bool CLineMgr::Collision_Line(const float& _fX, const float& _fY, float* _fNewY)
{
	if (m_LineList.empty())
		return false;

	CLine* Min = nullptr;		
	float m_fDistance = -1.f;	

	for (auto& iter : m_LineList)
	{
		
		float x1 = iter->Get_Info().tLeftPos.fX;
		float y1 = iter->Get_Info().tLeftPos.fY;

		float x2 = iter->Get_Info().tRightPos.fX;
		float y2 = iter->Get_Info().tRightPos.fY;

		if (x1 > x2)
		{
			float temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;
		}

		if (x1 < _fX &&
			x2 > _fX)
		{
			float LineY = (((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1);
			if (LineY > _fY)
			{
				if ((m_fDistance > (LineY - _fY)) || (m_fDistance == -1.f))
				{
					m_fDistance = LineY - _fY;
					Min = iter;
				}
			}
		}
	}

	if (Min != nullptr)
	{
		float x1 = Min->Get_Info().tLeftPos.fX;
		float y1 = Min->Get_Info().tLeftPos.fY;

		float x2 = Min->Get_Info().tRightPos.fX;
		float y2 = Min->Get_Info().tRightPos.fY;

		if (x1 > x2)
		{
			float temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;
		}

		*_fNewY = ((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1;

		return true;
	}

	return false;
}


