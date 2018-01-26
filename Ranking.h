#pragma once

#ifndef _CRANKING_
#define _CRANKING_

#include "BaseControl.h"
#include "AspectRatioDefine.h"
#include "DXWrapper.h"
#include <vector>
#include <utility>

using namespace std;

class CRanking :
	public CControlBase
{
public:
	CRanking(CControlBase*parent = NULL);
	~CRanking();
	//override function
	virtual void Render(ID2D1DeviceContext*);
	void UpdateRank(vector<pair<int, float>>* list);
private:
	ID2D1Bitmap* m_pRankBG;
	ID2D1Bitmap** m_ppChampionBg;
	ID2D1Bitmap** m_ppDigitBitmap;
	ID2D1Bitmap* m_pSemiColon;
	ID2D1Bitmap** m_ppTeamTitle;
	ID2D1Bitmap** m_coin_bg;
	int m_pDigit[4];
	void UpdateDigit(float fGrade = 0);
	vector<pair<int, float>>* m_teamGrade;
};

#endif

