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

protected:
	bool	isClear;
	bool	isFail;

	TCHAR	bulletCount[5] = L"";
	TCHAR	lifeCount[5] = L"";
	TCHAR	scoreCount[5] = L"";

	RECT	bulletRect;
	RECT	lifeRect;
	RECT	scoreRect;


};

