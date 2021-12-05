#pragma once

namespace OBJ
{
	enum ID {PLAYER,ENEMY,NPC,BULLET,PROP,END};
}

namespace BULLET
{
	enum ID {PISTOL, HEAVY, ROCKET, END};
}

namespace VEHICLE
{
	enum ID {TANK, CAMEL, END};
}

namespace NPC
{
	enum ID {SLAVE, TRAVELER, END};
}

namespace ITEM
{
	enum ID {HEAVY,ROCKET,RUBY,END};
}

namespace ENEMY
{
	enum ID {SOLIDER,TANK,END};
}

namespace SCENE
{
	enum ID {TITLE,MENU,GAME,END};
}

namespace RENDER
{
	enum ID {MAP,OBJECT,UI,END};
}

namespace DIR
{
	enum ID {UP,DOWN,LEFT,RIGHT,END};
}

namespace WEAPON
{
	enum ID {PISTOL, HEAVY, ROCKET, END};
}

namespace ACTION
{
	enum ID {IDLE, MOVE, JUMP, FIRE, STAB, GRENADE, DIE, END };
	
}

namespace ANIM
{
	enum PLAYER { PLAYER_IDLE, PLAYER_MOVE, PLAYER_FIRE,
		PLAYER_UP, PLAYER_STAB, PLAYER_GRENADE, PLAYER_DIE,
		PLAYER_JUMP, PLAYER_JUMP_UP, PLAYER_JUMP_DOWN, PLAYER_JUMP_FIRE,
		PLAYER_DOWN, PLAYER_DOWN_FIRE, PLAYER_END };
}

namespace SOLDIER
{
	enum CLASS { PRIVATE, SERGENT , END };
}