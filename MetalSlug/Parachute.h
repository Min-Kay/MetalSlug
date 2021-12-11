#pragma once
#include "Obj.h"
class Parachute :
	public Obj
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	void Set_Parent(Obj* _parent) { parent = _parent; }
private:
	Obj* parent;
};

