#pragma once
#ifndef _TIMECOUNTING_
#define _TIMECOUNTING_

#include "BaseControl.h"
#include "AspectRatioDefine.h"
#include "DXWrapper.h"

class CTimeCounting :
	public CControlBase
{
public:
	CTimeCounting(CControlBase*parent = NULL);
	~CTimeCounting();
	//override function
	virtual void Render(ID2D1DeviceContext*);
	void StartCounting();
	float EndCounting();
private:
	void UpdateDigit();
	ID2D1Bitmap** m_ppDigitBitmap;
	ID2D1Bitmap* m_pSemiColon;
	DWORD m_startTime;
	DWORD m_endTime;
	int m_pDigit[4];
	float m_fGrade;
	bool m_bIsFinish;
};

#endif