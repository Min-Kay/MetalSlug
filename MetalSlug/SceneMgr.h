#pragma once
#include "framework.h"
#include "Include.h"


class Scene; 

class SceneMgr
{

private:
	SceneMgr();
	virtual ~SceneMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hdc);
	void Release(); 

public:
	const SCENE::ID& Get_SceneId() const { return currScene; }
	void	Change_Scene(SCENE::ID _id);

private:

private:
	SCENE::ID	currScene;
	Scene* scene;

public:
	static SceneMgr* Get_Instance()
	{
		if (!pInstance)
			pInstance = new SceneMgr;
		return pInstance;
	}

	static void	Destroy_Instance()
	{
		if (pInstance)
		{
			delete	pInstance;
			pInstance = nullptr;
		}
	}

private:
	static SceneMgr* pInstance;
};

