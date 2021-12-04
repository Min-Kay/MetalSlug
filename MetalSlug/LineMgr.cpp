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

void CLineMgr::Load(const TCHAR* _File) // m_LineList 로드
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
	// 리스트가 비었으면 반환
	if (m_LineList.empty())
		return false;

	CLine* Min = nullptr;		// 조건 검사해서 알맞는 라인을 저장할 포인터변수 (모든라인을 검사했을때 조건에 맞고, m_fDistance가 최소인 라인)
	float m_fDistance = -1.f;	// 모든 라인을 비교하면서 조건에 맞는 라인과 오브젝트의 거리를 저장할 변수

	// 리스트 안의 라인을 모두 검사
	for (auto& iter : m_LineList)
	{
		// 현재 라인의 leftX가 오브젝트의 X보다 작고, rightX가 오브젝트의 X보다 크다면
		if (iter->Get_Info().tLeftPos.fX < _fX &&
			iter->Get_Info().tRightPos.fX > _fX)
		{
			// 직선의 방정식에 대입할 변수 초기화
			float x1 = iter->Get_Info().tLeftPos.fX;
			float y1 = iter->Get_Info().tLeftPos.fY;

			float x2 = iter->Get_Info().tRightPos.fX;
			float y2 = iter->Get_Info().tRightPos.fY;

			float LineY = (((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1);

			// 오브젝트 X값을 직선의 방정식에 대입했을때 나오는 Y값이 오브젝트의 Y값보다 크다면
			if (LineY > _fY)
			{
				// 오브젝트 X값을 직선의 방정식에 대입했을때 나오는 Y값과 오브젝트의 Y값의 거리가 m_fDistance보다 작다면 또는 기존 m_fDistance가 없었다면(m_fDistance==-1.f)
				if ((m_fDistance > (LineY - _fY)) || (m_fDistance == -1.f))
				{
					// 조건에 알맞는 최소 거리 라인 기준으로 m_fDistance와 Min 설정
					m_fDistance = LineY - _fY;
					Min = iter;
				}
			}
		}
	}

	// Min값이 nullptr이 아니라면 (조건에 맞는 라인이 있었다면)
	if (Min != nullptr)
	{
		float x1 = Min->Get_Info().tLeftPos.fX;
		float y1 = Min->Get_Info().tLeftPos.fY;

		float x2 = Min->Get_Info().tRightPos.fX;
		float y2 = Min->Get_Info().tRightPos.fY;

		// 오브젝트 Y값 변경
		*_fNewY = ((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1;

		return true;
	}

	return false;
}


