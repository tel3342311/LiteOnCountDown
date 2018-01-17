#pragma once

#ifndef _CCOUNTINGTEXT_
#define _CCOUNTINGTEXT_

#include "BaseControl.h"
#include "AspectRatioDefine.h"
#include "DXWrapper.h"

class CCountingText :
	public CControlBase
{
public:
	CCountingText(CControlBase*parent = NULL);
	~CCountingText();
	//override function
	virtual void Render(ID2D1DeviceContext*);
	void setTeamIdx(int idx);

private:
	ID2D1LinearGradientBrush* getBrush(float fIdx);
	IDWriteTextLayout* m_pSloganLayout;
	ID2D1GradientStopCollection* m_pGradient_stop_collection;
	ID2D1LinearGradientBrush* m_pLinear_gradient_brush;
	ID2D1Bitmap* m_pTextSloganBmp;
	ID2D1SolidColorBrush* m_pTextBrush;
	ID2D1SolidColorBrush* m_pTextBlueBrush;
	int m_nTeamIdx;
	DWRITE_TEXT_METRICS m_dtm;
};

#endif