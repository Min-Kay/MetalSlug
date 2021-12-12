#pragma once
#include "Game.h"
class Stage2 :
	public Game
{
public:
	Stage2();
	~Stage2();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	virtual bool	Check_GameState();
	virtual void Set_CheckPoint_Objects(); 


private:
	bool spawnMidBoss;

};

