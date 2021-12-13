#pragma once
#include "Item.h"
class WepItem :
	public Item
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;


public:
	void	Set_WepID(WEAPON::ID _id) { wep_id = _id; }

public:
	const WEAPON::ID& Get_WepID() const { return wep_id; }

public:
	virtual void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir); 

private:
	WEAPON::ID wep_id = WEAPON::HEAVY;
};

