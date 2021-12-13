#pragma once
#include "Enemy.h"
class Parachute :
	public Enemy
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	void Set_Parent(Obj* _parent) { parent = _parent; }
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);
private:
	Obj* parent = nullptr;
};

