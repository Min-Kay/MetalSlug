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

protected:
	bool	isClear;
	bool	isFail;

};

