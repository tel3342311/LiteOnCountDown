#pragma once
#ifndef _VOLUME_METER_
#define _VOLUME_METER_

#include "BaseControl.h"
#include "AspectRatioDefine.h"

extern class CMICCapture;
enum VOLUME_STATE{
	V_READY,
	V_START,
	V_FINISH,
	SHOW_GRADE,
};

class CVolumeMeter : public CControlBase
{
public:
	CVolumeMeter(CControlBase*parent=NULL);
	~CVolumeMeter();
	//override functions
	virtual void Render(ID2D1DeviceContext*d2ddc);
	void SetSTATE(VOLUME_STATE state);
	VOLUME_STATE GetSTATE();
	ID2D1LinearGradientBrush* GetGradientBrush(float ratio);
	CMICCapture* GetMICCapture();
	//send volume notification
	__event void VolumeEvent(float peak);
private:
	float m_currentPeak;
	CMICCapture* m_pMICCapture;
	ID2D1GradientStopCollection* m_pGradient_stop_collection;
	ID2D1LinearGradientBrush* m_pLinearGradientBrush;
	ID2D1SolidColorBrush *m_pBlackBrush;
	ID2D1Bitmap* m_pFinishBmp;
	ID2D1Bitmap** m_ppFlameBg;
	ID2D1RectangleGeometry* m_pRectGeometry;
	VOLUME_STATE m_bIsFinishState;
	UINT m_nIDEvent;
};

#endif