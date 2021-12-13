#include "DataMgr.h"
#include "Weapons.h"

DataMgr* DataMgr::instance = nullptr;

void DataMgr::Initialize()
{
	score = 0; 
	life = 3;
	grenade = 5;
	kill = 0;
	weapon = nullptr; 
}
