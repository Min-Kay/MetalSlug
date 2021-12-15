#pragma once
#include "Enemy.h"
class Burst :
	public Enemy
{
public:
	Burst();
	virtual ~Burst();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

public:
	void Set_Active();
	const bool& Get_Active() const { return active; }
	const bool& Get_Spawning() const { return spawning; }
	void Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir);

	virtual void Update_Rect(); 
private:
	bool active;
	bool spawning;
	bool maintain;

	DWORD grownTime;

	DWORD maintainTime;

	float totalCY;
	float init_CY; 
};

