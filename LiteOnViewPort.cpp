#include "DXUT.h"
#include "LiteOnViewPort.h"


CLiteOnViewPort *CLiteOn::m_liteOnViewPort = NULL;
static int getCoinFlash();

CLiteOnViewPort::CLiteOnViewPort(CControlBase*parent) : CControlBase(parent)
, m_bPressed(false)
, m_bg(NULL)
, m_bg_frame(NULL)
, m_coin_bg(NULL)
, m_balloon_bg(NULL)
, m_FaceIcon(NULL)
, m_TeamTitle(NULL)
, m_CountingText(NULL)
{
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_1.png", &m_bg, CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_2.png", &m_bg_frame, CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	m_coin_bg = new ID2D1Bitmap*[5];
	for (int i = 0; i < 5; i++) {
		m_coin_bg[i] = NULL;
	}
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_3-1.png", &m_coin_bg[0], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_3-2.png", &m_coin_bg[1], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_3-3.png", &m_coin_bg[2], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_3-4.png", &m_coin_bg[3], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_3-5.png", &m_coin_bg[4], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_4.png", &m_balloon_bg, CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);

	m_FaceIcon = new CFaceIcon(this);
	m_FaceIcon->SetSize(CResolution::m_screenResolutionX / 3.54f, CResolution::m_screenResolutionX / 3.54f);
	m_FaceIcon->SetLocation(CResolution::m_screenResolutionX - CResolution::m_screenResolutionX / 3.54f, (CResolution::m_screenResolutionY - CResolution::m_screenResolutionX / 3.54f) / 2.f);
	m_childList.Add(m_FaceIcon);

	m_TeamTitle = new CTeamTitle(this);
	m_TeamTitle->SetSize(CResolution::m_screenResolutionX / 1.37f, CResolution::m_screenResolutionY / 4.59f);
	m_TeamTitle->SetLocation((CResolution::m_screenResolutionX - CResolution::m_screenResolutionX / 1.37f) / 2.f, 0.f);
	m_childList.Add(m_TeamTitle);

	m_CountingText = new CCountingText(this);
	m_CountingText->SetLocation(CResolution::m_screenResolutionX / 15.11f, CResolution::m_screenResolutionY / 4.06f);
	m_childList.Add(m_CountingText);
}


CLiteOnViewPort::~CLiteOnViewPort()
{
	SAFE_RELEASE(m_bg);
	SAFE_RELEASE(m_bg_frame);
	SAFE_RELEASE(m_coin_bg[0]);
	SAFE_RELEASE(m_coin_bg[1]);
	SAFE_RELEASE(m_coin_bg[2]);
	SAFE_RELEASE(m_coin_bg[3]);
	SAFE_RELEASE(m_coin_bg[4]);
	SAFE_RELEASE(m_balloon_bg);
	for (int i = 0; i < m_childList.GetSize(); i++)
		delete m_childList[i];
}

bool CLiteOnViewPort::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	return false;
}

void CLiteOnViewPort::Render(ID2D1DeviceContext*d2ddc)
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
	if (m_bg != NULL)
		d2ddc->DrawBitmap(m_bg);
	if (m_bg_frame != NULL)
		d2ddc->DrawBitmap(m_bg_frame);
	int idx = getCoinFlash();
	if (m_coin_bg[idx] != NULL) 
		d2ddc->DrawBitmap(m_coin_bg[idx]);
	if (m_balloon_bg != NULL)
		d2ddc->DrawBitmap(m_balloon_bg);

	if (m_FaceIcon != NULL)
		m_FaceIcon->Render(d2ddc);
	
	if (m_TeamTitle != NULL)
		m_TeamTitle->Render(d2ddc);

	if (m_CountingText != NULL)
		m_CountingText->Render(d2ddc);
}

bool CLiteOnViewPort::HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam)
{
	if (!m_bVisible || !m_bEnabled || m_opacity == 0.0f)
		return false;
	for (int i = m_childList.GetSize() - 1; i >= 0; i--)
	{
		if (m_childList[i]->HandleMouse(uMsg, pt, wParam, lParam))
			return true;
	}
	return false;
}

bool CLiteOnViewPort::HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bVisible || !m_bEnabled || m_opacity == 0.0f)
		return false;
	for (int i = m_childList.GetSize() - 1; i >= 0; i--)
	{
		if (m_childList[i]->HandleKeyboard(uMsg, wParam, lParam))
			return true;
	}
	switch (uMsg)
	{
	case WM_CHAR:
	{
					switch ((WCHAR)wParam)
					{
					case VK_RETURN:
					{
						break;
					}
					default:
						break;
					}
	}
		break;

	case WM_KEYDOWN:
	{
					   switch (wParam)
					   {
					   case '1':
					   {
									m_TeamTitle->setTeamIdx(0);
									return true;
					   }
					   case '2':
					   {
									m_TeamTitle->setTeamIdx(1);
									return true;
					   }
					   case '3':
					   {
									m_TeamTitle->setTeamIdx(2);
									return true;
					   }
					   case '4':
					   {
									m_TeamTitle->setTeamIdx(3);
									return true;
					   }
					   case '5':
					   {
									m_TeamTitle->setTeamIdx(4);
									return true;
					   }
					   case '6':
					   {
									m_TeamTitle->setTeamIdx(5);
									return true;
					   }
					   case '7':
					   {
									m_TeamTitle->setTeamIdx(6);
									return true;
					   }
					   case '8':
					   {
								   m_TeamTitle->setTeamIdx(7);
								   return true;
					   }
					   case VK_PRIOR:
					   {
										return true;
					   }
						   break;
					   case VK_NEXT:
					   {
									   return true;
					   }
						   break;
					   case VK_UP:
					   {
					   }
						   break;
					   case VK_DOWN:
					   {
					   }
						   break;
					   }
	}
		break;
	default:
		break;
	}
	return false;
}
bool CLiteOnViewPort::HandlePoint(PTOUCHINPUT ptList, int ptCount)
{
	if (!m_bVisible || !m_bEnabled || m_opacity == 0.0f)
		return false;

	for (int i = m_childList.GetSize() - 1; i >= 0; i--)
	{
		if (m_childList[i]->HandlePoint(ptList, ptCount))
			return true;
	}

	DWORD _tFlag = ptList[0].dwFlags;
	POINT pt = { ptList[0].x, ptList[0].y };

	static POINT _preP;
	static float _diff = 0.0f;

	if ((_tFlag & TOUCHEVENTF_DOWN) == TOUCHEVENTF_DOWN)
	{
		if (!ContainsPoint(pt))
			return false;
		m_bPressed = true;
		_preP = pt;
		_diff = 0.0f;
		return true;
	}
	else if ((_tFlag & TOUCHEVENTF_MOVE) == TOUCHEVENTF_MOVE)
	{
		if (!m_bPressed)
			return false;
		float _dx = pt.x - _preP.x;
		float _dy = pt.y - _preP.y;
		_diff += sqrt(abs(_dx * _dx + _dy * _dy));
		_preP = pt;
		return true;
	}
	else if ((_tFlag & TOUCHEVENTF_UP) == TOUCHEVENTF_UP)
	{
		if (!m_bPressed)
			return false;
		m_bPressed = false;
		if (!ContainsPoint(pt) || _diff > 10.0f)
			return false;
		return true;
	}

	return false;
}

static float Opacity = 0.f;
static float OpacityDiff = 0.1f;

static int getCoinFlash()
{
	Opacity += OpacityDiff;
	if (Opacity >= 5.f)
		Opacity = 0.f;
	return (int)Opacity;
}
