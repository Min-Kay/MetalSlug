#pragma once
#include "Scene.h"
class Game :
	public Scene
{
public:
	Game();
	virtual ~Game();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	void KeyInput();

public:
	virtual bool	Check_GameState();

public:
	void	Init_Information(); 
	void	Render_Information(HDC _hdc);
	int		Check_Number(TCHAR name);
	int		Check_NumSize(int i); 
	void	Check_Scrolling();

	virtual void	Set_CheckPoint_Objects();


	void	Check_Checkpoint_Clear();

protected:
	bool	isClear;
	bool	isFail;

	TCHAR	scoreCount[10] = L"";
	TCHAR	bulletCount[10] = L"";
	TCHAR	lifeCount[10] = L"";
	TCHAR	grenadeCount[10] = L"";

	RECT	uiRect;

	list<POINT> scrollLock;
	bool	checkPoint;
	bool	scrollUpdating;

	float  formalX;
	float  formalY;
	float  currPlayerPos;
	float totalX = 0.f;
	float totalY = 0.f;


	int	  maxCheckPoint;
	int	  currCheckPoint = 0;
};

