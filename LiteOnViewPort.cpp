#include "DXUT.h"
#include "LiteOnViewPort.h"


CLiteOnViewPort *CLiteOn::m_liteOnViewPort = NULL;
static int getCoinFlash();
static int getCoinFlashFast();

void WINAPI OnFinishCallBack(UINT nIDEvent, void* pUserContext);
void WINAPI OnFinishGradeCallBack(UINT nIDEvent, void* pUserContext);

CLiteOnViewPort::CLiteOnViewPort(CControlBase*parent) : CControlBase(parent)
, m_bPressed(false)
, m_bg(NULL)
, m_coin_bg(NULL)
, m_balloon_bg(NULL)
, m_FaceIcon(NULL)
, m_TeamTitle(NULL)
, m_CountingText(NULL)
, m_TimeCounting(NULL)
, m_VolumeMeter(NULL)
, m_nIDEvent(1002)
, m_nIDEventGrade(1003)
, m_bFlashFast(false)
{
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_1.png", &m_bg, CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	m_coin_bg = new ID2D1Bitmap*[5];
	for (int i = 0; i < 4; i++) {
		m_coin_bg[i] = NULL;
	}
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_2-1.png", &m_coin_bg[0], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_2-2.png", &m_coin_bg[1], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_2-3.png", &m_coin_bg[2], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\main_gnd_2-4.png", &m_coin_bg[3], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);

	m_FaceIcon = new CFaceIcon(this);
	m_FaceIcon->SetSize(CResolution::m_screenResolutionX / 3.54f, CResolution::m_screenResolutionX / 3.54f);
	m_FaceIcon->SetLocation(0.f, 120.f);
	m_childList.Add(m_FaceIcon);

	m_TeamTitle = new CTeamTitle(this);
	m_TeamTitle->SetSize(CResolution::m_screenResolutionX / 1.37f, CResolution::m_screenResolutionY / 4.59f);
	m_TeamTitle->SetLocation((CResolution::m_screenResolutionX - CResolution::m_screenResolutionX / 1.37f), 0.f);
	m_childList.Add(m_TeamTitle);

	m_CountingText = new CCountingText(this);
	m_CountingText->SetLocation(CResolution::m_screenResolutionX / 15.11f, CResolution::m_screenResolutionY / 4.06f);
	m_childList.Add(m_CountingText);
	
	m_TimeCounting = new CTimeCounting(this);
	m_TimeCounting->SetLocation(CResolution::m_screenResolutionX / 2.56f, CResolution::m_screenResolutionY / 4.59f);
	m_childList.Add(m_TimeCounting);

	m_VolumeMeter = new CVolumeMeter(this);
	m_VolumeMeter->SetLocation(0.f, CResolution::m_screenResolutionY / 5.f);
	m_VolumeMeter->SetSize(CResolution::m_screenResolutionX, CResolution::m_screenResolutionY / 2.77f);
	m_childList.Add(m_VolumeMeter);

	//Event hook
	__hook(&CVolumeMeter::VolumeEvent, m_VolumeMeter, &CLiteOnViewPort::OnVolumeEvent);
	__hook(&CCountingText::ReadyEvent, m_CountingText, &CLiteOnViewPort::OnFinishTextCounting);
}


CLiteOnViewPort::~CLiteOnViewPort()
{
	__unhook(&CVolumeMeter::VolumeEvent, m_VolumeMeter, &CLiteOnViewPort::OnVolumeEvent);
	__unhook(&CCountingText::ReadyEvent, m_CountingText, &CLiteOnViewPort::OnFinishTextCounting);

	SAFE_RELEASE(m_bg);
	SAFE_RELEASE(m_bg_frame);
	SAFE_RELEASE(m_coin_bg[0]);
	SAFE_RELEASE(m_coin_bg[1]);
	SAFE_RELEASE(m_coin_bg[2]);
	SAFE_RELEASE(m_coin_bg[3]);
	SAFE_DELETE(m_coin_bg);
	for (int i = 0; i < m_childList.GetSize(); i++)
		delete m_childList[i];
}
static int countingFinish = 0;

void CLiteOnViewPort::OnVolumeEvent(float fPeak) 
{
	if (fPeak < 0.005f) 
	{
		m_FaceIcon->SetFaceState(START);
	}
	else if (fPeak < 0.1f)
	{
		m_FaceIcon->SetFaceState(PROGRESS_3);
		countingFinish++;
		if (countingFinish > 5) 
		{
			m_FaceIcon->SetFaceState(FINISH);
			OnFinishCounting();
			countingFinish = 0;
		}

	}
	else if (fPeak < 0.4f)
	{
		m_FaceIcon->SetFaceState(PROGRESS_3);
		countingFinish = 0;
	}
	else if (fPeak < 0.6f)
	{
		m_FaceIcon->SetFaceState(PROGRESS_2);
		countingFinish = 0;
	}
	else if (fPeak < 0.8f)
	{
		m_FaceIcon->SetFaceState(PROGRESS_1);
		countingFinish = 0;
	}
}

void CLiteOnViewPort::OnFinishTextCounting()
{
	m_FaceIcon->SetFaceState(COUNTDOWN);
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
	if (m_VolumeMeter != NULL && m_VolumeMeter->GetSTATE() == V_START)
	{
		if (m_coin_bg[0] != NULL)
			d2ddc->DrawBitmap(m_coin_bg[0]);
	}
	else
	{
		int idx = m_bFlashFast ? getCoinFlashFast() : getCoinFlash();
		if (m_coin_bg[idx] != NULL)
			d2ddc->DrawBitmap(m_coin_bg[idx]);
	}
	
	
	if (m_VolumeMeter != NULL)
		m_VolumeMeter->Render(d2ddc);

	if (m_TeamTitle != NULL)
		m_TeamTitle->Render(d2ddc);

	if (m_CountingText != NULL)
		m_CountingText->Render(d2ddc);

	if (m_FaceIcon != NULL)
		m_FaceIcon->Render(d2ddc);

	if (m_TimeCounting != NULL)
		m_TimeCounting->Render(d2ddc);

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
				OnStartCounting();
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
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		{
					OnChangeTeam(wParam - 0x31);
					return true;
		}
		case VK_PRIOR:
		{
						return true;
		}
			break;
		case VK_NEXT:
		{
						m_FaceIcon->m_bVisible = !m_FaceIcon->m_bVisible;
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

void CLiteOnViewPort::OnChangeTeam(int idx)
{
	m_VolumeMeter->m_bVisible = false;
	m_TimeCounting->m_bVisible = false;
	m_TeamTitle->setTeamIdx(idx);
	m_CountingText->m_bVisible = true;
	m_CountingText->setTeamIdx(idx);
	m_FaceIcon->SetFaceState(PREPARE);
}

void CLiteOnViewPort::OnStartCounting()
{
	m_CountingText->m_bVisible = false;
	
	m_VolumeMeter->m_bVisible = true;
	m_VolumeMeter->SetSTATE(V_START);
	
	m_TimeCounting->m_bVisible = true;
	m_TimeCounting->StartCounting();
	m_TimeCounting->SetLocation(CResolution::m_screenResolutionX / 2.56f, CResolution::m_screenResolutionY / 4.59f);
}

void CLiteOnViewPort::OnFinishCounting()
{
	m_TimeCounting->EndCounting();
	m_TimeCounting->m_bVisible = false;
	m_VolumeMeter->SetSTATE(V_FINISH);

	DXUTSetTimer(OnFinishCallBack, 2.f, &m_nIDEvent, this);
	
}

void CLiteOnViewPort::ShowGrade()
{
	m_TimeCounting->m_bVisible = true;
	m_TimeCounting->SetLocation(CResolution::m_screenResolutionX / 2.74f, CResolution::m_screenResolutionY / 3.32f);
	m_VolumeMeter->SetSTATE(SHOW_GRADE);
	m_bFlashFast = true;
	DXUTKillTimer(m_nIDEvent);
	DXUTSetTimer(OnFinishGradeCallBack, 2.f, &m_nIDEventGrade, this);
}

void CLiteOnViewPort::FinishShowGrade()
{
	m_VolumeMeter->m_bVisible = false;
	m_bFlashFast = false;
	DXUTKillTimer(m_nIDEventGrade);
}

static float Opacity = 0.f;
static float OpacityDiff = 0.083f;
static float OpacityDiffFast = 0.166f;

static int getCoinFlash()
{
	Opacity += OpacityDiff;
	if (Opacity >= 4.f)
		Opacity = 0.f;
	return (int)Opacity;
}

static int getCoinFlashFast()
{
	Opacity += OpacityDiffFast;
	if (Opacity >= 4.f)
		Opacity = 0.f;
	return (int)Opacity;
}

void WINAPI OnFinishCallBack(UINT nIDEvent, void* pUserContext)
{
	CLiteOnViewPort* pLiteOnViewPort = (CLiteOnViewPort*)pUserContext;
	pLiteOnViewPort->ShowGrade();
}

void WINAPI OnFinishGradeCallBack(UINT nIDEvent, void* pUserContext)
{
	CLiteOnViewPort* pLiteOnViewPort = (CLiteOnViewPort*)pUserContext;
	pLiteOnViewPort->FinishShowGrade();
}