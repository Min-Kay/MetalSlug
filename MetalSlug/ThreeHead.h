#pragma once
#include "Enemy.h"
#include "Include.h"

class Tower;

class ThreeHead :
	public Enemy
{
public:
	ThreeHead();
	virtual ~ThreeHead();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

private:
	void	State_Machine();

public:
	void	Send_State(THREEHEAD::STATE _state); 
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) {}

	void	Tower_On();
	bool	AllDestroied();
private:
	THREEHEAD::STATE state;
	
	float totalY;
	bool towerOn;

	bool doorOpening;
	
	DWORD dyingTimer;
	DWORD effectTimer;
	DWORD effectStable;
	int effectIndex;

	float	randomX;
	float	randomY;
	float	randomCX;
	float	randomCY;

	Tower* left = nullptr;
	Tower* mid = nullptr;
	Tower* right = nullptr;
};

