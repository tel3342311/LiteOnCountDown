#include "DXUT.h"
#include "FaceIcon.h"

CFaceIcon::CFaceIcon(CControlBase*parent) : CControlBase(parent)
, m_pFaceIcon(NULL)
{
	m_pFaceIcon = new ID2D1Bitmap*[7];
	for (int i = 0; i < 7; i++) {
		m_pFaceIcon[i] = NULL;
	}
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_stage0.png", &m_pFaceIcon[0]);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_stage1.png", &m_pFaceIcon[1], CResolution::m_screenResolutionX / 3.5, CResolution::m_screenResolutionX / 3.5);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_stage2.png", &m_pFaceIcon[2], CResolution::m_screenResolutionX / 3.5, CResolution::m_screenResolutionX / 3.5);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_stage3.png", &m_pFaceIcon[3], CResolution::m_screenResolutionX / 3.5, CResolution::m_screenResolutionX / 3.5);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_stage4.png", &m_pFaceIcon[4], CResolution::m_screenResolutionX / 3.5, CResolution::m_screenResolutionX / 3.5);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_stage5.png", &m_pFaceIcon[5], CResolution::m_screenResolutionX / 3.5, CResolution::m_screenResolutionX / 3.5);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_ani_stage6.png", &m_pFaceIcon[6], CResolution::m_screenResolutionX / 3.5, CResolution::m_screenResolutionX / 3.5);

}


CFaceIcon::~CFaceIcon()
{
	for (int i = 0; i < 7; i++) {
		SAFE_RELEASE(m_pFaceIcon[i]);
	}
	SAFE_DELETE(m_pFaceIcon);
}

void CFaceIcon::Render(ID2D1DeviceContext*d2ddc)
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
	if (m_pFaceIcon[g_dragMode] != NULL)
		d2ddc->DrawBitmap(m_pFaceIcon[g_dragMode]);
}

void CFaceIcon::SetFaceState(FACEMODE mode) 
{
	g_dragMode = mode;
}