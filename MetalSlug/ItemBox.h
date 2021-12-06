#pragma once
#include "Item.h"
class ItemBox :
	public Item
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);

public:
	void	Anim_Breaking(HDC _hdc);
private:
	bool isBreaking;

};

