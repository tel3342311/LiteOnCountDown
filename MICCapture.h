#pragma once

#ifndef _CMICCAPTURE_
#define _CMICCAPTURE_
#include "DXUT.h"

// Timer ID and period (in milliseconds)
#define ID_TIMER  1
#define TIMER_PERIOD  125

extern class IMMDeviceEnumerator;
extern class IMMDevice;
extern class IMMDeviceCollection;
extern class IAudioMeterInformation;

class CMICCapture
{
public:
	CMICCapture();
	~CMICCapture();
	HRESULT Init();
	HRESULT UnInit();
	HRESULT GetPeakValue(float *pfPeak);
private:
	IMMDeviceEnumerator *m_pEnumerator = NULL;
	IMMDevice *m_pDevice = NULL;
	IMMDeviceCollection *m_pDeviceCollection = NULL;
	IAudioMeterInformation *m_pMeterInfo = NULL;
	IPropertyStore *m_pProperty = NULL;
	bool m_bInit;
};

#endif

