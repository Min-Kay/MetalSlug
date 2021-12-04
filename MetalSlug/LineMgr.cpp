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

void CLineMgr::Load(const TCHAR* _File) // m_LineList �ε�
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
	// ����Ʈ�� ������� ��ȯ
	if (m_LineList.empty())
		return false;

	CLine* Min = nullptr;		// ���� �˻��ؼ� �˸´� ������ ������ �����ͺ��� (�������� �˻������� ���ǿ� �°�, m_fDistance�� �ּ��� ����)
	float m_fDistance = -1.f;	// ��� ������ ���ϸ鼭 ���ǿ� �´� ���ΰ� ������Ʈ�� �Ÿ��� ������ ����

	// ����Ʈ ���� ������ ��� �˻�
	for (auto& iter : m_LineList)
	{
		// ���� ������ leftX�� ������Ʈ�� X���� �۰�, rightX�� ������Ʈ�� X���� ũ�ٸ�
		if (iter->Get_Info().tLeftPos.fX < _fX &&
			iter->Get_Info().tRightPos.fX > _fX)
		{
			// ������ �����Ŀ� ������ ���� �ʱ�ȭ
			float x1 = iter->Get_Info().tLeftPos.fX;
			float y1 = iter->Get_Info().tLeftPos.fY;

			float x2 = iter->Get_Info().tRightPos.fX;
			float y2 = iter->Get_Info().tRightPos.fY;

			float LineY = (((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1);

			// ������Ʈ X���� ������ �����Ŀ� ���������� ������ Y���� ������Ʈ�� Y������ ũ�ٸ�
			if (LineY > _fY)
			{
				// ������Ʈ X���� ������ �����Ŀ� ���������� ������ Y���� ������Ʈ�� Y���� �Ÿ��� m_fDistance���� �۴ٸ� �Ǵ� ���� m_fDistance�� �����ٸ�(m_fDistance==-1.f)
				if ((m_fDistance > (LineY - _fY)) || (m_fDistance == -1.f))
				{
					// ���ǿ� �˸´� �ּ� �Ÿ� ���� �������� m_fDistance�� Min ����
					m_fDistance = LineY - _fY;
					Min = iter;
				}
			}
		}
	}

	// Min���� nullptr�� �ƴ϶�� (���ǿ� �´� ������ �־��ٸ�)
	if (Min != nullptr)
	{
		float x1 = Min->Get_Info().tLeftPos.fX;
		float y1 = Min->Get_Info().tLeftPos.fY;

		float x2 = Min->Get_Info().tRightPos.fX;
		float y2 = Min->Get_Info().tRightPos.fY;

		// ������Ʈ Y�� ����
		*_fNewY = ((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1;

		return true;
	}

	return false;
}


