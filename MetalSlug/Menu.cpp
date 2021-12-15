#include "Menu.h"
#include "Manager.h"


void Menu::Initialize()
{
	m_LogoTimer = GetTickCount(); 
	BmpMgr::Get_Instance()->Insert_Bmp(LOGO_BMP,LOGO_KEY);
	isShow = false; 
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"Menu.mp3",0.7f);
}

void Menu::Update()
{

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Ok.mp3", CSoundMgr::UI, 1.0f);
		SceneMgr::Get_Instance()->Change_Scene(SCENE::GAME);
		return;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_ESCAPE))
	{
		CSoundMgr::Get_Instance()->Destroy_Instance();
		BlockMgr::Get_Instance()->Destroy_Instance();
		BmpMgr::Get_Instance()->Destroy_Instance();
		ObjPoolMgr::Get_Instance()->Destroy_Instance();
		CLineMgr::Get_Instance()->Destroy_Instance();
		CScrollMgr::Get_Instance()->Destroy_Instance();
		CKeyMgr::Get_Instance()->Destroy_Instance();
		DataMgr::Get_Instance()->Destroy_Instance();
		SceneMgr::Get_Instance()->Destroy_Instance();
		DestroyWindow(g_hwnd);
		return;
	}
}

void Menu::Late_Update()
{
	if (m_LogoTimer + 500.f < GetTickCount())
	{
		isShow = !isShow;
		m_LogoTimer = GetTickCount();
	}
}

void Menu::Render(HDC _hdc)
{
	drawingDC = BmpMgr::Get_Instance()->Find_Image(LOGO_KEY);
	BitBlt(_hdc,0,0,WINCX,WINCY,drawingDC, isShow * 800,0, SRCCOPY);

}

void Menu::Release()
{
	ReleaseDC(g_hwnd,drawingDC);
}
