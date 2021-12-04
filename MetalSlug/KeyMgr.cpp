#include "framework.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;
CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
	ZeroMemory(m_bKeyRollvering, sizeof(m_bKeyRollvering));
}


CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::Initialize()
{
	m_Click_Time = 0.f;
	m_LastKey = VK_MAX;
	m_Double_Click_Time = 400.f;
}

bool CKeyMgr::Key_Pressing(int _Key)
{
	if (GetAsyncKeyState(_Key) & 0x8000)
		return true;

	return false;
}

bool CKeyMgr::Key_Down(int _Key)
{
	// 이전에 눌림이 없고 현재 눌렸을 경우

	if (m_bKeyRollvering[_Key])
		return false;

	if (!m_bKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		return true;
	}

	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
		m_bKeyState[_Key] = !m_bKeyState[_Key];

	return false;
}

bool CKeyMgr::Key_Up(int _Key)
{
	// 이전에 눌림이 있고 현재 눌리지 않았을 경우

	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		return true;
	}

	if (!m_bKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
		m_bKeyState[_Key] = !m_bKeyState[_Key];


	return false;
}

bool CKeyMgr::Key_Rollover(int _First, int _Second) // 키 동시 입력
{
	if (!m_bKeyState[_First] && (GetAsyncKeyState(_First) & 0x8000) && !m_bKeyState[_Second] && (GetAsyncKeyState(_Second) & 0x8000))
	{
		m_bKeyState[_First] = !m_bKeyState[_First];
		m_bKeyState[_Second] = !m_bKeyState[_Second];
		m_bKeyRollvering[_First] = true;
		m_bKeyRollvering[_Second] = true;
		return true;
	}

	if (m_bKeyState[_First] && !(GetAsyncKeyState(_First) & 0x8000))
	{
		m_bKeyState[_First] = !m_bKeyState[_First];
		m_bKeyRollvering[_First] = false;
	}

	if (m_bKeyState[_Second] && !(GetAsyncKeyState(_Second) & 0x8000))
	{
		m_bKeyState[_Second] = !m_bKeyState[_Second];
		m_bKeyRollvering[_Second] = false;
	}

	return false;
}

bool CKeyMgr::Key_Double_Click(int _Key) // 키 더블 클릭 
{
	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000) && _Key != m_LastKey)
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		m_LastKey = _Key;
		m_Click_Time = GetTickCount();
		return false;
	}

	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000) && _Key == m_LastKey)
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		m_LastKey = VK_MAX;
		return true;
	}

	if (!m_bKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
		m_bKeyState[_Key] = !m_bKeyState[_Key];

	if (_Key == m_LastKey && m_Click_Time + m_Double_Click_Time < GetTickCount())
	{
		m_LastKey = VK_MAX;
		return false;
	}

	return false;
}

bool CKeyMgr::Key_Down_Up(int _Key)
{
	return !Key_Down(_Key) && Key_Up(_Key);
}

