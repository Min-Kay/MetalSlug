#pragma once
#include "Scene.h"
class Game :
	public Scene
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	void KeyInput();

public:
	virtual void	Check_GameState();

public:
	void	Init_Information(); 
	void	Render_Information(HDC _hdc);
	int		Check_Number(TCHAR name);
	int		Check_NumSize(int i); 
protected:
	bool	isClear;
	bool	isFail;

	TCHAR	scoreCount[10] = L"";
	TCHAR	bulletCount[10] = L"";
	TCHAR	lifeCount[10] = L"";
	TCHAR	grenadeCount[10] = L"";

	RECT	uiRect;
};

