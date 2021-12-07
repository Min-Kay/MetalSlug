#pragma once
#include "Obj.h"
class Bullet :
	public Obj
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	const OBJ::ID& Get_ParentID() const { return parentID; }

public:
	void Set_ParentID(OBJ::ID _parent) { parentID = _parent; }

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);

public:
	void Anim_Counter(int count, float _timer, bool _roop = true, int start = 0);

public:
	const int& Get_Damage() const { return damage; }
	void Set_Damage(int _dam) { damage = _dam; }

protected:
	float speed;
	int damage;
	OBJ::ID parentID;
	int animIndex = 0;
	DWORD animTimer;
};

