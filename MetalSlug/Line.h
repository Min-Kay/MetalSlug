#pragma once

#include "Include.h"

class CLine
{
public:
	CLine();
	CLine(LINEINFO& _info);
	CLine(LINEPOS& _tLeft, LINEPOS& _tRight);
	~CLine();

public:
	const	LINEINFO&		Get_Info(void) const { return m_tInfo; }

public:
	void		Render(HDC _DC);

private:
	LINEINFO		m_tInfo;
};

