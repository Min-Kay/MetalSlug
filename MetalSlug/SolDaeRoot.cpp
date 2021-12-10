#include "SolDaeRoot.h"
#include "Manager.h"
#include "Player.h"

void SolDaeRoot::Initialize()
{	
    id = OBJ::BULLET;
    parentID = OBJ::ENEMY;
    speed = 0.f;
    info.cx = 70.f;
    info.cy = 260.f;

    animIndex = 0; 
    animTimer = GetTickCount();

    state = SPAWN;

    idleSpeed = 0.5f;
    totalIdle = 0.f;

    BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae_Root.bmp", L"SolDae_Root");
    BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae_Root_Sub.bmp", L"SolDae_Root_Sub");

}

int SolDaeRoot::Update()
{
    if (isDead)
        return OBJ_DEAD;

    Update_Rect();
    return OBJ_DEFAULT;
}

void SolDaeRoot::Late_Update()
{
}

void SolDaeRoot::Render(HDC _hdc)
{
    float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

   // Rectangle(_hdc,rect.left+scrollX,rect.top+scrollY,rect.right + scrollX,rect.bottom + scrollY);

    switch (state)
    {
    case SolDaeRoot::SPAWN:
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root");
        Anim_Counter(5,250.f,false);
        GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, info.cy, drawingDC, 4 + animIndex * 33, 0, 33, 130, RGB(248, 0, 248));
        if (animIndex == 5)
        {
            animIndex = 0;
            idleTimer = GetTickCount();
            state = SolDaeRoot::IDLE;
        }
        break;
    case SolDaeRoot::IDLE:
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root");
        GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, info.cy, drawingDC, 4 + 165, 0, 33, 130, RGB(248, 0, 248));
        Anim_Counter(11,100.f,false);
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root_Sub");
        GdiTransparentBlt(_hdc, rect.left - 20.f + scrollX, rect.top + 50.f + scrollY, 140, 160, drawingDC, 70 * animIndex, 0, 70, 80, RGB(248, 0, 248));
        break;
    case SolDaeRoot::ROOTING:
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root");
        GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, info.cy, drawingDC, 4 + 165, 0, 33, 130, RGB(248, 0, 248));
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root_Sub");
        GdiTransparentBlt(_hdc, rect.left - 20.f + scrollX, rect.top + 50.f + scrollY, 140, 160, drawingDC, 70 * 11, 0, 70, 80, RGB(248, 0, 248));
        break;
    case SolDaeRoot::EXPLODE:
        break;
    }
}

void SolDaeRoot::Release()
{
}

void SolDaeRoot::Set_Collision(OBJ::ID _id, Obj* _opponent, DIR::ID _dir)
{
    if (_id == OBJ::PLAYER)
    {
        static_cast<Player*>(_opponent)->Set_Dying();
    }
}

void SolDaeRoot::State_Machine()
{
    switch (state)
    {
    case SolDaeRoot::IDLE:

        totalIdle += idleSpeed;
        info.y += idleSpeed;

        if (totalIdle > 50.f)
        {
            totalIdle = 0.f;
            idleSpeed *= -1.f;
        }

        if (idleTimer + 2000.f < GetTickCount())
        {
           state = SolDaeRoot::ROOTING;
        }
        break;
    case SolDaeRoot::ROOTING:
    {
    }
        
        break;
    case SolDaeRoot::EXPLODE:
        break;
    }
}

void SolDaeRoot::Gravity()
{
}
