#include "framework.h"
#include "BlockMgr.h"
#include "Enemy.h"

BlockMgr* BlockMgr::instance = nullptr;

BlockMgr::BlockMgr()
{
}

BlockMgr::~BlockMgr()
{
	Release();
}

void BlockMgr::Release()
{
	blocks.erase(blocks.begin(),blocks.end());
	blocks.clear();
}

void BlockMgr::Add_Block(Obj* obj)
{
	blocks.push_back(obj);
}

void BlockMgr::Delete_Block(Obj* obj)
{
	auto iter = find(blocks.begin(), blocks.end(), obj);
	if (iter != blocks.end())
	{
		blocks.erase(iter);
	}
}

bool BlockMgr::Collision_Block(Obj* target, float* fy)
{
	bool isCollide = false;

	for (auto i : blocks)
	{
		if (i->Get_Dead())
			continue;

		RECT rc{};
		if (IntersectRect(&rc, &target->Get_Rect(), &i->Get_Rect()))
		{
			float x = float(rc.right - rc.left);
			float y = float(rc.bottom - rc.top);

			if (dynamic_cast<Enemy*>(i) && !static_cast<Enemy*>(i)->Get_CollMode())
				continue;

			if (dynamic_cast<Enemy*>(i) && target->Get_ID() == OBJ::ENEMY)
				continue;

			if (x > y)
			{
				if (rc.top < target->Get_Rect().bottom && rc.top > target->Get_Rect().top)
				{
					isCollide = true;
					*fy = target->Get_Info().y - y;
					continue;
				}
				else if (rc.left < target->Get_Rect().right && rc.left > target->Get_Rect().left)
				{
					target->Add_X(-x);
				}
				else if (rc.right > target->Get_Rect().left&& rc.right < target->Get_Rect().right)
				{
					target->Add_X(x);
				}
			}
			else if (x < y)
			{
				if (rc.left < target->Get_Rect().right && rc.left > target->Get_Rect().left)
				{
					target->Add_X(-x);
				}
				else if (rc.right > target->Get_Rect().left&& rc.right < target->Get_Rect().right)
				{
					target->Add_X(x);
				}
				else if (rc.top < target->Get_Rect().bottom && rc.top > target->Get_Rect().top)
				{
					isCollide = true;
					*fy = target->Get_Info().y - y;
				}
			}
		}
	}
	return isCollide;
}
