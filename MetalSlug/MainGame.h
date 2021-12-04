#pragma once
#include "framework.h"
#include "Include.h"

class MainGame
{
public:
	MainGame();
	virtual ~MainGame();

public:
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render(); 
	void	Release(); 

protected:
	HDC			hdc;
	int			fps;
	DWORD		frameTime;
	TCHAR		fpsText[100] = L"";
	HDC	hMemDC;
	HDC	hBackBuferDC;
};

