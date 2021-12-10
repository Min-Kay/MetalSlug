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

    totalSub = 0.f;
    subSpeed = 0.5f;

    fallingSpeed = 7.f;

    collY = 0.f;

    BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae_Root.bmp", L"SolDae_Root");
    BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae_Root_Sub.bmp", L"SolDae_Root_Sub");
    BmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SolDae_Root_Explode.bmp", L"SolDae_Root_Explode");

}

int SolDaeRoot::Update()
{
    if (isDead)
        return OBJ_DEAD;

    State_Machine();
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
        GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, info.cy, drawingDC, animIndex * 33 - 2, 0, 33, 130, RGB(248, 0, 248));
        if (animIndex == 5)
        {
            animIndex = 0;
            idleTimer = GetTickCount();
            state = SolDaeRoot::IDLE;
        }
        break;
    case SolDaeRoot::IDLE:
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root");
        GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, info.cy, drawingDC,165, 0, 33, 130, RGB(248, 0, 248));
        Anim_Counter(5,70.f,false);
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root_Sub");
        GdiTransparentBlt(_hdc, rect.left - 25.f + scrollX, rect.bottom - 100.f + scrollY + subSpeed, 110, 110, drawingDC, 70 * animIndex + 27 , 0, 70, 70, RGB(248, 0, 248));
        break;
    case SolDaeRoot::ROOTING:
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root");
        GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, info.cy, drawingDC,165, 0, 33, 130, RGB(248, 0, 248));
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root_Sub");
        GdiTransparentBlt(_hdc, rect.left - 25.f + scrollX, rect.bottom - 100.f + scrollY + subSpeed, 110, 110, drawingDC, 380, 0, 70, 70, RGB(248, 0, 248));
        break;
    case SolDaeRoot::EXPLODE:
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root");
        GdiTransparentBlt(_hdc, rect.left + scrollX, rect.top + scrollY, info.cx, info.cy, drawingDC, 165, 0, 33, 130, RGB(248, 0, 248));
        Anim_Counter(7,100.f,false);
        drawingDC = BmpMgr::Get_Instance()->Find_Image(L"SolDae_Root_Explode");
        GdiTransparentBlt(_hdc, info.x - 90.f + scrollX, collY - 190.f + scrollY, 180, 190, drawingDC, 90 * animIndex + 15, 0, 90, 95, RGB(248, 0, 248));
        if (animIndex == 7)
            isDead = true; 
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
        totalSub += subSpeed;

        if (totalIdle > 50.f)
        {
            totalIdle = 0.f;
            idleSpeed *= -1.f;
        }

        if (totalSub > 10.f)
        {
            subSpeed *= -1.f;
        }

        if (idleTimer + 2000.f < GetTickCount())
        {
           state = SolDaeRoot::ROOTING;
        }
        break;
    case SolDaeRoot::ROOTING:
    {
        bool lineCol = CLineMgr::Get_Instance()->Collision_Line(info.x, info.y, &collY);

        if (lineCol &&  info.y >= collY - info.cy * 0.5f)
        {
            state = SolDaeRoot::EXPLODE; 
            break;
        }
        else
        {
            info.y += fallingSpeed;
        }
    }
        break;
    case SolDaeRoot::EXPLODE:
        if (idleSpeed < 0) idleSpeed *= -1.f;
        info.y += idleSpeed; 
        break;
    }
}
