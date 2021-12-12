#pragma once
#include "framework.h"
class Obj;

class BlockMgr
{
private:
	BlockMgr();
	~BlockMgr();
private:
	static BlockMgr* instance;

public:
	static BlockMgr* Get_Instance() 
	{
		if (!instance)
			instance = new BlockMgr;

		return instance;
	}

	static void Destroy_Instance()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

public:
	void Release();
	void Add_Block(Obj* obj);
	void Delete_Block(Obj* obj);
	bool Collision_Block(Obj* target, float* fy);

private:
	list<Obj*> blocks;
};

