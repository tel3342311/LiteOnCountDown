
#include "VolumeMeter.h"
#include "DXUT.h"
#include "DXWrapper.h"
#include "MICCapture.h" 
#include "CharEncode.h"
#include "AppMgr.h"

using namespace std;
static int getFlameFlash();

void WINAPI OnVolumePeakCallBack(UINT nIDEvent, void* pUserContext);

CVolumeMeter::CVolumeMeter(CControlBase*parent) : CControlBase(parent)
, m_pMICCapture(NULL)
, m_pGradient_stop_collection(NULL)
, m_pLinearGradientBrush(NULL)
, m_currentPeak(0.f)
, m_pFinishBmp(NULL)
, m_ppFlameBg(NULL)
, m_bIsFinishState(false)
, m_nIDEvent(1001)
{
	m_pMICCapture = new CMICCapture();

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(0.91f, 1.f, 0.f);
	gradientStops[0].position = 0.f;
	gradientStops[1].color = D2D1::ColorF(1.f, 0.22f, 0.f);
	gradientStops[1].position = 1.f;

	CDXWrapper::GetD2DDC()->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&m_pGradient_stop_collection
		);

	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_img_end.png", &m_pFinishBmp);

	m_ppFlameBg = new ID2D1Bitmap*[4];
	for (int i = 0; i < 2; i++) {
		m_ppFlameBg[i] = NULL;
	}

	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_end-1.png", &m_ppFlameBg[0]);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_end-2.png", &m_ppFlameBg[1]);
		
	CLog::Init(_T("Log.txt"));
}


CVolumeMeter::~CVolumeMeter()
{
	SAFE_DELETE(m_pMICCapture);
	SAFE_RELEASE(m_pGradient_stop_collection);
	SAFE_RELEASE(m_pLinearGradientBrush);
	SAFE_RELEASE(m_pFinishBmp);
	SAFE_RELEASE(m_ppFlameBg[0]);
	SAFE_RELEASE(m_ppFlameBg[1]);
	SAFE_RELEASE(m_ppFlameBg[2]);
	SAFE_RELEASE(m_ppFlameBg[3]);
	SAFE_DELETE(m_ppFlameBg);
}

void CVolumeMeter::Render(ID2D1DeviceContext*d2ddc)
{
	if (!m_bVisible || m_opacity == 0.0f)
		return;
	//set transform
	D2D1::Matrix3x2F _m, _world;
	GetParentTransform(&_world);
	//offset
	_m = D2D1::Matrix3x2F::Translation(m_x, m_y);
	_world = _m * _world;
	//angle
	_m = D2D1::Matrix3x2F::Rotation(m_angle);
	_world = _m * _world;
	d2ddc->SetTransform(_world);


	//get opacity
	float _o = m_opacity * GetParentOpacity();

	d2ddc->SetTransform(_world);
	if (!m_bIsFinishState)
	{
		if (m_pLinearGradientBrush != NULL)
			d2ddc->FillRectangle(D2D1::RectF(0, m_height * (1.f - m_currentPeak), m_width, m_height), m_pLinearGradientBrush);
	} 
	else
	{
		int idx = getFlameFlash();
		if (m_ppFlameBg[idx] != NULL)
			d2ddc->DrawBitmap(m_ppFlameBg[idx]);
		GetParentTransform(&_world);

		d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 3.7f, CResolution::m_screenResolutionY / 4.3f) * _world);
		if (m_pFinishBmp != NULL)
			d2ddc->DrawBitmap(m_pFinishBmp);
	}
	
	GetParentTransform(&_world);
	d2ddc->SetTransform(_world);
}

ID2D1LinearGradientBrush* CVolumeMeter::GetGradientBrush(float fPeak)
{
	if (m_currentPeak == fPeak && m_pLinearGradientBrush != NULL)
		return m_pLinearGradientBrush;
	m_currentPeak = fPeak;
	SAFE_RELEASE(m_pLinearGradientBrush);
	CDXWrapper::GetD2DDC()->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
		D2D1::Point2F(m_width / 2.f, m_height),
		D2D1::Point2F(m_width / 2.f, m_height * (1.f - fPeak))),
		m_pGradient_stop_collection,
		&m_pLinearGradientBrush);
	return m_pLinearGradientBrush;
}

static float Opacity = 0.f;
static float OpacityDiff = 0.2f;

static int getFlameFlash()
{
	Opacity += OpacityDiff;
	if (Opacity >= 4.f)
		Opacity = 0.f;
	return (int)Opacity;
}

void CVolumeMeter::SetFinish(bool isFinish)
{
	m_bIsFinishState = isFinish;
	if (!isFinish)
	{
		DXUTSetTimer(OnVolumePeakCallBack, 0.033f, &m_nIDEvent, this);
		CLog::Write(L"===========================NEW TEST===================================\n");
	}
	else
	{
		DXUTKillTimer(m_nIDEvent);
		CLog::Write(L"===========================END TEST===================================\n");
	}
}

CMICCapture* CVolumeMeter::GetMICCapture()
{
	return m_pMICCapture;
}

void WINAPI OnVolumePeakCallBack(UINT nIDEvent, void* pUserContext)
{
	CVolumeMeter* pVolumeMeter = (CVolumeMeter*) pUserContext;

	float fPeak = 0.f;
	if (pVolumeMeter->GetMICCapture() != NULL)
		pVolumeMeter->GetMICCapture()->GetPeakValue(&fPeak);
	pVolumeMeter->GetGradientBrush(fPeak);
	tstringstream tss;
	tss << "Peak is " << fPeak << endl;
	CLog::Write(tss.str());
	__raise pVolumeMeter->VolumeEvent(fPeak);
}