#include "DXUT.h"
#include "TimeCounting.h"


CTimeCounting::CTimeCounting(CControlBase*parent) : CControlBase(parent)
, m_ppDigitBitmap(NULL)
, m_pSemiColon(NULL)
, m_startTime(0)
, m_endTime(0)
{
	m_ppDigitBitmap = new ID2D1Bitmap*[10];
	for (int i = 0; i < 10; i++) {
		m_ppDigitBitmap[i] = NULL;
	}
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number0.png", &m_ppDigitBitmap[0], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number1.png", &m_ppDigitBitmap[1], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number2.png", &m_ppDigitBitmap[2], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number3.png", &m_ppDigitBitmap[3], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number4.png", &m_ppDigitBitmap[4], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number5.png", &m_ppDigitBitmap[5], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number6.png", &m_ppDigitBitmap[6], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number7.png", &m_ppDigitBitmap[7], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number8.png", &m_ppDigitBitmap[8], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number9.png", &m_ppDigitBitmap[9], CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);

	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_number dot.png", &m_pSemiColon, CResolution::m_screenResolutionX / 7.5, CResolution::m_screenResolutionY / 4.3);
}


CTimeCounting::~CTimeCounting()
{
	for (int i = 0; i < 10; i++) {
		SAFE_RELEASE(m_ppDigitBitmap[i]);
	}

	SAFE_RELEASE(m_pSemiColon);
}

void CTimeCounting::Render(ID2D1DeviceContext*d2ddc) {
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

	//save transform
	SetTransform(&_world);
	SetInverse(&_world);

	//get opacity
	float _o = m_opacity * GetParentOpacity();

	d2ddc->SetTransform(_world);
	UpdateDigit();
	if (m_ppDigitBitmap[m_pDigit[0]] != NULL)
		d2ddc->DrawBitmap(m_ppDigitBitmap[m_pDigit[0]]);
	d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 7.6, 0) * _world);
	if (m_ppDigitBitmap[m_pDigit[1]] != NULL)
		d2ddc->DrawBitmap(m_ppDigitBitmap[m_pDigit[1]]);
	d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 7.6 * 2, 0) * _world);
	if (m_pSemiColon != NULL)
		d2ddc->DrawBitmap(m_pSemiColon);
	d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 7.6 * 3, 0) * _world);
	if (m_ppDigitBitmap[m_pDigit[2]] != NULL)
		d2ddc->DrawBitmap(m_ppDigitBitmap[m_pDigit[2]]);
	d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 7.6f * 4, 0) * _world);
	if (m_ppDigitBitmap[m_pDigit[3]] != NULL)
		d2ddc->DrawBitmap(m_ppDigitBitmap[m_pDigit[3]]);

	SetTransform(&_world);
	SetInverse(&_world);
}

void CTimeCounting::StartCounting() 
{
	m_startTime = m_endTime = timeGetTime();
	for (int i = 0; i < 4; i++) 
	{
		m_pDigit[i] = 0;
	}
}

void CTimeCounting::UpdateDigit() 
{
	m_endTime = timeGetTime();
	float sec = (m_endTime - m_startTime) / 1000.f;
	m_pDigit[0] = sec / 10.f > 10 ? 0 : sec / 10.f;
	m_pDigit[1] = (int)sec % 10;
	m_pDigit[2] = (int)(sec * 10.f) % 10;
	m_pDigit[3] = (int)(sec * 100.f) % 10;
}