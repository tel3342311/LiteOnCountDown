#pragma once
#ifndef _VOLUME_METER_
#define _VOLUME_METER_

#include "BaseControl.h"
#include "AspectRatioDefine.h"

extern class CMICCapture;

class CVolumeMeter : public CControlBase
{
public:
	CVolumeMeter(CControlBase*parent=NULL);
	~CVolumeMeter();
	//override functions
	virtual void Render(ID2D1DeviceContext*d2ddc);
private:
	float m_currentPeak;
	CMICCapture* m_pMICCapture;
	ID2D1GradientStopCollection* m_pGradient_stop_collection;
	ID2D1LinearGradientBrush* m_pLinearGradientBrush;
	ID2D1LinearGradientBrush* GetGradientBrush(float ratio);
};

#endif