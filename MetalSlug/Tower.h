#pragma once
#include "Enemy.h"

class ThreeHead;
class Tower :
	public Enemy
{

public:
	Tower();
	Tower(ThreeHead* _parent, THREEHEAD::HEAD _head, float _X,float _Y);
	virtual ~Tower();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	void	Set_State(THREEHEAD::STATE _state) {state = _state;}
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);


private:
	void	State_Machine(); 

private:
	ThreeHead* parent = nullptr;

	THREEHEAD::HEAD head;
	THREEHEAD::STATE state;


	bool curtainIdle;
	bool firing;

	DWORD fireTime;
};

