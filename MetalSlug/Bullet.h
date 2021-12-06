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
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir) { if(_opponent->Get_ID() != parentID) isDead = true; }

protected:
	float speed;
	OBJ::ID parentID;
};

