
#include "VolumeMeter.h"
#include "DXUT.h"
#include "DXWrapper.h"

CVolumeMeter::CVolumeMeter(CControlBase*parent) : CControlBase(parent)
, m_pMICCapture(NULL)
, m_pGradient_stop_collection(NULL)
, m_pLinearGradientBrush(NULL)
{
	//m_pMICCapture = new CMICCapture();

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow);
	gradientStops[0].position = 0.f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Red);
	gradientStops[1].position = 1.f;

	CDXWrapper::GetD2DDC()->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&m_pGradient_stop_collection
		);

	CDXWrapper::GetD2DDC()->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
		D2D1::Point2F(m_width / 2.f, m_height),
		D2D1::Point2F(m_width / 2.f, 0.f)),
		m_pGradient_stop_collection,
		&m_pLinearGradientBrush);
}


CVolumeMeter::~CVolumeMeter()
{
	SAFE_DELETE(m_pMICCapture);
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

	//save transform
	SetTransform(&_world);
	SetInverse(&_world);

	//get opacity
	float _o = m_opacity * GetParentOpacity();

	d2ddc->SetTransform(_world);
	
	d2ddc->DrawRectangle(NULL, m_pLinearGradientBrush);
}
