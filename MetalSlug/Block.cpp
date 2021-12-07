#include "Block.h"

void Block::Initialize()
{
	isDead = false;
	id = OBJ::BLOCK;
}

int Block::Update()
{
	if (haveGravity)
		Gravity();

	Update_Rect();
	return OBJ_DEFAULT;
}

void Block::Late_Update()
{
}

void Block::Render(HDC _hdc)
{
	Rectangle(_hdc,rect.left,rect.top,rect.right,rect.bottom);
}

void Block::Release()
{
}
