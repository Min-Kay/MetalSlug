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
	enum ID {IDLE, MOVE, RUN, JUMP, ATTACK, DIE, END };
}