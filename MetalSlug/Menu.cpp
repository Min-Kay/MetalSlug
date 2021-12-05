#include "Menu.h"
#include "Manager.h"


void Menu::Initialize()
{
	m_LogoTimer = GetTickCount(); 
	BmpMgr::Get_Instance()->Insert_Bmp(LOGO_BMP,LOGO_KEY);
	isShow = false; 
}

void Menu::Update()
{

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		SceneMgr::Get_Instance()->Change_Scene(SCENE::GAME);
		return;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_ESCAPE))
	{
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
