#pragma once
#include "framework.h"
#include "Include.h"

class Obj;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void	Collision_Rect(list<Obj*> _Dest, list<Obj*> _Src);
private:
	static DIR::ID Set_Dir(RECT& _coll, const RECT* _target);
};

