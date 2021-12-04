#include "Obj.h"

void Obj::Update_Rect()
{
	rect.bottom = info.y + info.cy * 0.5f;
	rect.top = info.y - info.cy * 0.5f;
	rect.left = info.x - info.cx * 0.5f;
	rect.right = info.x + info.cx * 0.5f;
}
