
#include "VolumeMeter.h"
#include "DXUT.h"
#include "DXWrapper.h"
#include "MICCapture.h" 
#include "CharEncode.h"
#include "AppMgr.h"

using namespace std;
static int getFlameFlash();
static int getColor(int step);
void WINAPI OnVolumePeakCallBack(UINT nIDEvent, void* pUserContext);



CVolumeMeter::CVolumeMeter(CControlBase*parent) : CControlBase(parent)
, m_pMICCapture(NULL)
, m_pGradient_stop_collection(NULL)
, m_pLinearGradientBrush(NULL)
, m_currentPeak(0.f)
, m_pFinishBmp(NULL)
, m_ppFlameBg(NULL)
, m_bIsFinishState(V_READY)
, m_nIDEvent(1001)
, m_pRectGeometry(NULL)
, m_pBlackBrush(NULL)
{
	m_pMICCapture = new CMICCapture();

	CDXWrapper::GetD2DFactory()->CreateRectangleGeometry(D2D1::RectF(0, 0, CResolution::m_screenResolutionX / 38.4f, CResolution::m_screenResolutionY / 7.94f), &m_pRectGeometry);
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

	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_img_end.png", &m_pFinishBmp, CResolution::m_screenResolutionX / 2.44f, CResolution::m_screenResolutionY / 3.375f);

	m_ppFlameBg = new ID2D1Bitmap*[2];
	for (int i = 0; i < 2; i++) {
		m_ppFlameBg[i] = NULL;
	}

	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_end-1.png", &m_ppFlameBg[0], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY / 2.769f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_end-2.png", &m_ppFlameBg[1], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY / 2.769f);
	CDXWrapper::GetD2DDC()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f),&m_pBlackBrush);
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
	SAFE_DELETE(m_ppFlameBg);
	SAFE_RELEASE(m_pRectGeometry);
	SAFE_RELEASE(m_pBlackBrush);
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

	if (m_bIsFinishState == V_START)
	{
		//if (m_pLinearGradientBrush != NULL)
		//	d2ddc->FillRectangle(D2D1::RectF(0, m_height * (1.f - m_currentPeak), m_width, m_height), m_pLinearGradientBrush);
		int level = 20.f * m_currentPeak;
		for (int i = 0; i < 20; i++)
		{
			d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 3.f + CResolution::m_screenResolutionX / 32.f * i, CResolution::m_screenResolutionY / 4.8f) * _world);
			if (i < level)
			{
				m_pBlackBrush->SetColor(D2D1::ColorF(getColor(i)));
			} 
			else 
			{
				m_pBlackBrush->SetColor(D2D1::ColorF(0xd3d3d3));
			}
			d2ddc->FillGeometry(m_pRectGeometry, m_pBlackBrush);
			m_pBlackBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			d2ddc->DrawGeometry(m_pRectGeometry, m_pBlackBrush, 5.f);

		}
		d2ddc->SetTransform(_world);
	}
	else if (m_bIsFinishState == V_FINISH)
	{
		int idx = getFlameFlash();
		if (m_ppFlameBg[idx] != NULL)
			d2ddc->DrawBitmap(m_ppFlameBg[idx]);
		
		if (m_pFinishBmp != NULL) {
			GetParentTransform(&_world);
			d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 2.45f, CResolution::m_screenResolutionY / 4.32f) * _world);
			d2ddc->DrawBitmap(m_pFinishBmp);
		}

	}
	else if (m_bIsFinishState == SHOW_GRADE) 
	{
		int idx = getFlameFlash();
		if (m_ppFlameBg[idx] != NULL)
			d2ddc->DrawBitmap(m_ppFlameBg[idx]);
	}
	
	GetParentTransform(&_world);
	d2ddc->SetTransform(_world);
}

ID2D1LinearGradientBrush* CVolumeMeter::GetGradientBrush(float fPeak)
{
	/*if (m_currentPeak == fPeak && m_pLinearGradientBrush != NULL)
		return m_pLinearGradientBrush;
	m_currentPeak = fPeak;
	SAFE_RELEASE(m_pLinearGradientBrush);
	CDXWrapper::GetD2DDC()->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
		D2D1::Point2F(m_width / 2.f, m_height),
		D2D1::Point2F(m_width / 2.f, m_height * (1.f - fPeak))),
		m_pGradient_stop_collection,
		&m_pLinearGradientBrush);
	
	return m_pLinearGradientBrush;
	*/
	m_currentPeak = fPeak;
	return NULL;
}

static float Opacity = 0.f;
static float OpacityDiff = 0.166f;

static int getFlameFlash()
{
	Opacity += OpacityDiff;
	if (Opacity >= 2.f)
		Opacity = 0.f;
	return (int)Opacity;
}

void CVolumeMeter::SetSTATE(VOLUME_STATE state)
{
	if (m_bIsFinishState == V_START && m_bIsFinishState == state)
	{
		DXUTKillTimer(m_nIDEvent);
	}
	m_bIsFinishState = state;
	if (m_bIsFinishState == V_START)
	{
		DXUTSetTimer(OnVolumePeakCallBack, 0.033f, &m_nIDEvent, this);
		CLog::Write(L"===========================NEW TEST===================================\n");
	}
	else if (m_bIsFinishState == V_FINISH)
	{
		DXUTKillTimer(m_nIDEvent);
		CLog::Write(L"===========================END TEST===================================\n");
	}
}

CMICCapture* CVolumeMeter::GetMICCapture()
{
	return m_pMICCapture;
}

VOLUME_STATE CVolumeMeter::GetSTATE()
{
	return m_bIsFinishState;
}

void WINAPI OnVolumePeakCallBack(UINT nIDEvent, void* pUserContext)
{
	CVolumeMeter* pVolumeMeter = (CVolumeMeter*) pUserContext;

	float fPeak = 0.f;
	if (pVolumeMeter->GetMICCapture() != NULL)
		pVolumeMeter->GetMICCapture()->GetPeakValue(&fPeak);
	pVolumeMeter->GetGradientBrush(sqrt(sqrt(fPeak)));
	tstringstream tss;
	tss << "Peak is " << fPeak << endl;
	CLog::Write(tss.str());
	__raise pVolumeMeter->VolumeEvent(fPeak);
}

static int getColor(int step)
{

	int _color[20] = { 0xFFFF00, 0xFFF400, 0xFFEA00, 0xFFDF00, 0xFFD500, 0xFFCA00, 0xFFC000, 0xFFB500, 0xFFAB00, 0xFFA000,0xFF9600,0xFF8B00, 0xFF8100, 0xFF7600, 0xFF6C00, 0xFF6100, 0xFF5700, 0xFF4C00, 0xFF4200, 0xFF3800 };

	if (step >= 0 && step < 20)
		return _color[19 - step];
	return 0x0;

}