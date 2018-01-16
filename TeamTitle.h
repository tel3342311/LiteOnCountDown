#pragma once
#include "BaseControl.h"
#include "AspectRatioDefine.h"
#include "DXWrapper.h"

#ifndef _TEAMTITLE_
#define _TEAMTITLE_

#define TEAM_COUNT 8
class CTeamTitle :
	public CControlBase
{
public:
	CTeamTitle(CControlBase*parent = NULL);
	~CTeamTitle();
	//override function
	virtual void Render(ID2D1DeviceContext*);
	void setTeamIdx(int idx);
private:
	ID2D1Bitmap* m_pTitleBmp;
	int m_nTeamIdx;
};

#endif
