#include "Item.h"
#include "Manager.h"

void Item::Valid()
{
	if (isValid && validTime + 500.f < GetTickCount())
	{
		isValid = false;
	}
}
