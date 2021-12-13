#pragma once
#include "Obj.h"
class Item :
	public Obj
{
public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hdc) PURE;
	virtual void Release() PURE;

public:
	const ITEM::ID& Get_ItemID() const { return item_id; }

	void Valid();

protected:
	ITEM::ID item_id = ITEM::WEAPON; 

	bool	isValid = true;
	DWORD	validTime = GetTickCount();
};

