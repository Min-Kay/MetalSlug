
#include "Block.h"
class Car :
	public Block
{
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

	virtual void Add_HP(int _hp) { if (isDead) return; onHit = true; hp += _hp; }

protected:
	bool onHit;
};
