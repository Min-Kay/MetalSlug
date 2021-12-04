#pragma once

#include "Include.h"

class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();

public:
	void		Initialize();

public:
	bool		Key_Pressing(int _Key);
	bool		Key_Down(int _Key);
	bool		Key_Up(int _Key);
	bool		Key_Rollover(int _First, int _Second); // 동시 입력 검사
	bool		Key_Double_Click(int _Key);
	bool		Key_Down_Up(int _Key);

public:
	static	CKeyMgr* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CKeyMgr;

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
	static CKeyMgr* m_pInstance;
	bool			m_bKeyState[VK_MAX];

	bool			m_bKeyRollvering[VK_MAX]; // 동시 입력과 일반 입력 동시 실행 방지용 부울 리스트

	int				m_LastKey; // 마지막으로 입력한 더블클릭 변수 저장용
	DWORD			m_Click_Time; // 더블 클릭 시간 갱신용
	float			m_Double_Click_Time; // 더블 클릭 텀 설정용
};

