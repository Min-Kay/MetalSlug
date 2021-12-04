#include "BmpMgr.h"
#include "Include.h"
#include "CBitMap.h"

BmpMgr* BmpMgr::m_pInstance = nullptr;
BmpMgr::BmpMgr()
{
}


BmpMgr::~BmpMgr()
{
	Release();
}

void BmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));

	if (iter != m_mapBit.end())
		return;

	CBitMap* pBmp = new CBitMap;
	pBmp->Load_Bmp(pFilePath);

	m_mapBit.emplace(pImgKey, pBmp);
}

HDC BmpMgr::Find_Image(const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

void BmpMgr::Release(void)
{
	for_each(m_mapBit.begin(), m_mapBit.end(), CDeleteMap());
	m_mapBit.clear();
}