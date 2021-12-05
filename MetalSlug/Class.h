#pragma once
#include "framework.h"
#include "Include.h"

template <typename T>
void Safe_Delete(T& temp)
{
	if (temp)
	{
		delete temp;
		temp = nullptr;
	}
};

class KillObj
{
public:
	KillObj() {}
	~KillObj() {}

public:
	template<typename T>
	void operator()(T& Temp)
	{
		Temp->Set_Dead(true);
	}
};


class CDeleteObj
{
public:
	CDeleteObj() {}
	~CDeleteObj() {}

public:
	template<typename T>
	void operator()(T& Temp)
	{
		if (nullptr != Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}
};

class CTag_Finder
{
public:
	CTag_Finder(const TCHAR* pString)
		: m_pString(pString) {}

public:
	template<typename T>
	bool	operator()(T& Temp)
	{
		return !lstrcmp(m_pString, Temp.first);
	}

private:
	const TCHAR* m_pString;
};

class CDiscmp
{
public:
	CDiscmp() {}

public:
	template<typename T>
	bool	operator()(T& Temp1, T& Temp2)
	{
		return Temp1.dis < Temp2.dis;
	}

};


class CDeleteMap
{
public:
	CDeleteMap() {}
	~CDeleteMap() {}

public:
	template<typename T>
	void operator()(T& Temp)
	{
		if (nullptr != Temp.second)
		{
			delete Temp.second;
			Temp.second = nullptr;
		}
	}
};

template<typename T>
bool CompareY(T Dest, T Sour)
{
	return Dest->Get_Info().y < Sour->Get_Info().y;
}

template<typename T>
bool CompareDead(T temp1, T temp2)
{
	return temp1->Get_Dead() > temp2->Get_Dead();
}
