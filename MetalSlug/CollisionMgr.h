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
	static void	Collision_Rect(vector<Obj*> _Dest, vector<Obj*> _Src);
	static void	Collision_RectPush(vector<Obj*> _Dest, vector<Obj*> _Src);

private:
	static DIR::ID Set_Dir(RECT& _coll, const RECT* _target);
};

