#include "CollisionMgr.h"
#include "Include.h"
#include "Obj.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(vector<Obj*> _Dest, vector<Obj*> _Src)
{
	for (auto Dest : _Dest)
	{
		for (auto Src : _Src)
		{
			if (Dest->Get_Dead() || Src->Get_Dead())
				continue;

			RECT rc{};
			if (IntersectRect(&rc, &Dest->Get_Rect(), &Src->Get_Rect()))
			{
				Dest->Set_Collision(Src, Set_Dir(rc, &(Dest->Get_Rect())));
				Src->Set_Collision(Dest, Set_Dir(rc, &(Src->Get_Rect())));
			}
		}
	}
}

void CCollisionMgr::Collision_RectPush(vector<Obj*> _Dest, vector<Obj*> _Src)
{
	for (auto Dest : _Dest)
	{
		float fWidth = 0.f, fHeight = 0.f;
		for (auto Src : _Src)
		{
		}
	}
}

DIR::ID CCollisionMgr::Set_Dir(RECT& _coll, const RECT* _target)
{
	vector<DISDIR> distance;
	distance.reserve(4);
	
	distance.push_back(DISDIR(abs(_coll.bottom - _target->top), DIR::UP));
	distance.push_back(DISDIR(abs(_coll.top - _target->bottom), DIR::DOWN));
	distance.push_back(DISDIR(abs(_coll.left - _target->right), DIR::RIGHT));
	distance.push_back(DISDIR(abs(_coll.right - _target->left), DIR::LEFT));

	sort(distance.begin(),distance.end(),CDiscmp());

	DIR::ID dir = distance[0].dir;

	distance.erase(distance.begin(), distance.end());
	distance.clear();

	return dir;
}
