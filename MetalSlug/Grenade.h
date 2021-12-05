#pragma once
#include "Obj.h"
class Grenade :
	public Obj
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual void Set_Collision(Obj* _opponent, DIR::ID _dir);

protected:
	virtual void Bomb(); 

	
protected:
	int damage;
	int increaseSizeX;
	int increaseSizeY;
	

private:
	int	bounceCount; 
};

