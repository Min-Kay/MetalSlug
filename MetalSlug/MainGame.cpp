#include "MainGame.h"
#include "Manager.h"
#include <time.h>

MainGame::MainGame()
{
	ZeroMemory(fpsText, sizeof(TCHAR) * 100);
	srand(unsigned(time(NULL)));
}

MainGame::~MainGame()
{
	Release();
}

void MainGame::Initialize()
{
	hdc = GetDC(g_hwnd);
	fps = 0; 
	hMemDC = NULL;
	hBackBuferDC = NULL;
	ObjPoolMgr::Get_Instance()->Initialize();
	CKeyMgr::Get_Instance()->Initialize();
	DataMgr::Get_Instance()->Initialize();
	SceneMgr::Get_Instance()->Initialize();
	BmpMgr::Get_Instance()->Insert_Bmp(BACKBUFFER_BMP,BACKBUFFER_KEY);
}

void MainGame::Update()
{
	SceneMgr::Get_Instance()->Update();
}

void MainGame::Late_Update()
{
	CScrollMgr::Get_Instance()->Late_Update();
	SceneMgr::Get_Instance()->Late_Update();
}

void MainGame::Render()
{
	hMemDC = BmpMgr::Get_Instance()->Find_Image(BACKBUFFER_KEY);
	hBackBuferDC = BmpMgr::Get_Instance()->Find_Image(BACKBUFFER_KEY);

	BitBlt(hBackBuferDC,0,0,WINCX,WINCY,hMemDC,0,0,SRCCOPY);

	SceneMgr::Get_Instance()->Render(hBackBuferDC);

	BitBlt(hdc,0,0,WINCX,WINCY,hBackBuferDC,0,0,SRCCOPY);

	++fps;

	if (frameTime + 1000 < GetTickCount())
	{
		swprintf_s(fpsText, L"FPS : %d / x: %.1f / y: %.1f", fps, CScrollMgr::Get_Instance()->Get_ScrollX(), CScrollMgr::Get_Instance()->Get_ScrollY());

		SetWindowText(g_hwnd, fpsText);
		frameTime = GetTickCount();
		fps = 0;
	}
}

void MainGame::Release()
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
}
