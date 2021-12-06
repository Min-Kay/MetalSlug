#include "SceneMgr.h"
#include "Menu.h"
#include "Title.h"
#include "Game.h"
#include "Stage2.h"

SceneMgr* SceneMgr::pInstance = nullptr;

SceneMgr::SceneMgr()
{
}

SceneMgr::~SceneMgr()
{
	Release();
}

void SceneMgr::Initialize()
{
	currScene = SCENE::MENU;
	scene = new Menu;
	scene->Initialize();
}

void SceneMgr::Update()
{
	if (scene)
		scene->Update();
}

void SceneMgr::Late_Update()
{
	if(scene)
		scene->Late_Update();
}

void SceneMgr::Render(HDC _hdc)
{
	if (scene)
		scene->Render(_hdc);
}

void SceneMgr::Release()
{
	if (scene)
		scene->Release();
	SAFE_DELETE(scene);
}

void SceneMgr::Change_Scene(SCENE::ID _id)
{
	if (scene)
	{
		scene->Release();
		SAFE_DELETE(scene);
	}
	
	switch (_id)
	{
	case SCENE::TITLE:
		scene = new Title;
		break;
	case SCENE::MENU:
		scene = new Menu;
		break;
	case SCENE::GAME:
		scene = new Game;
		break;
	case SCENE::GAME1:
		scene = new Stage2;
		break;
	default:
		scene = new Menu;
		break;
	}
	scene->Initialize(); 
	currScene = _id; 
}
