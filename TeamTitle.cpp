#include "DXUT.h"
#include "TeamTitle.h"
#include "CharEncode.h"

using namespace std;

CTeamTitle::CTeamTitle(CControlBase*parent) : CControlBase(parent)
, m_pTitleBmp(NULL)
, m_nTeamIdx(-1)
{
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_img_team-1.png", &m_pTitleBmp, CResolution::m_screenResolutionX / 1.37f, CResolution::m_screenResolutionY / 4.59f);
}


CTeamTitle::~CTeamTitle()
{
	SAFE_RELEASE(m_pTitleBmp);
}

void CTeamTitle::Render(ID2D1DeviceContext*d2ddc) {
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
	if (m_pTitleBmp != NULL)
		d2ddc->DrawBitmap(m_pTitleBmp);
}

void CTeamTitle::setTeamIdx(int idx) 
{
	SAFE_RELEASE(m_pTitleBmp);
	tstring imagePath[] = { L"images\\main_img_team-1.png", 
							L"images\\main_img_team-2.png",
							L"images\\main_img_team-3.png", 
							L"images\\main_img_team-4.png", 
							L"images\\main_img_team-5.png", 
							L"images\\main_img_team-6.png", 
							L"images\\main_img_team-7.png", 
							L"images\\main_img_team-8.png" };
	if (idx >= 0 && idx < TEAM_COUNT) 
	{
		CDXWrapper::LoadImageFromFileAsyncEx((LPWSTR)imagePath[idx].c_str(), &m_pTitleBmp, CResolution::m_screenResolutionX / 1.37f, CResolution::m_screenResolutionY / 4.59f);
	}
}