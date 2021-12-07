#include "CollisionMgr.h"
#include "Include.h"
#include "Obj.h"
#include "Enemy.h"
#include "Player.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(list<Obj*> _Dest, list<Obj*> _Src)
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
				Dest->Set_Collision(Src->Get_ID(), Src, Set_Dir(rc, &(Dest->Get_Rect())));
				Src->Set_Collision(Dest->Get_ID(), Dest, Set_Dir(rc, &(Src->Get_Rect())));
			}
		}
	}
}

void CCollisionMgr::Collision_RectPush(list<Obj*> _Dest, list<Obj*> _Src)
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
				if (static_cast<Enemy*>(Src)->Get_CollMode())
				{
					float x = rc.right - rc.left;
					float y = rc.bottom - rc.top; 

					if (rc.top < Dest->Get_Rect().bottom && rc.top > Dest->Get_Rect().top)
					{
						if (static_cast<Player*>(Dest))
						{
							static_cast<Player*>(Dest)->Set_BoxCollide(true);
							static_cast<Player*>(Dest)->Set_CollisionY(Dest->Get_Info().y - y);
						}
						else
						{
							Dest->Add_Y(-y);
						}
						continue;
					}
					else if (rc.left < Dest->Get_Rect().right && rc.left > Dest->Get_Rect().left)
					{
						Dest->Add_X(-x);
					}
					else if (rc.right > Dest->Get_Rect().left && rc.right < Dest->Get_Rect().right)
					{
						Dest->Add_X(x);
					}
				}
			}
			else if(static_cast<Player*>(Dest))
			{
				static_cast<Player*>(Dest)->Set_BoxCollide(false);
			}
			
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
