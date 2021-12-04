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
	bool		Key_Rollover(int _First, int _Second); // ���� �Է� �˻�
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

	bool			m_bKeyRollvering[VK_MAX]; // ���� �Է°� �Ϲ� �Է� ���� ���� ������ �ο� ����Ʈ

	int				m_LastKey; // ���������� �Է��� ����Ŭ�� ���� �����
	DWORD			m_Click_Time; // ���� Ŭ�� �ð� ���ſ�
	float			m_Double_Click_Time; // ���� Ŭ�� �� ������
};

