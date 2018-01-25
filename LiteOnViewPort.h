#pragma once

#ifndef _LITEONVIEWPORT_
#define _LITEONVIEWPORT_

#include "BaseControl.h"
#include "AspectRatioDefine.h"
#include "DXWrapper.h"
#include "FaceIcon.h"
#include "TeamTitle.h"
#include "CountingText.h"
#include "TimeCounting.h"
#include "VolumeMeter.h"
#include <vector>
#include <utility>

using namespace std;

class CLiteOnViewPort;

static class CLiteOn
{
public:
	static CLiteOnViewPort *m_liteOnViewPort;
};

class CLiteOnViewPort :
	public CControlBase
{
public:
	CLiteOnViewPort(CControlBase*parent = NULL);
	~CLiteOnViewPort(void);
	//public function
	bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//override functions
	virtual void Render(ID2D1DeviceContext*d2ddc);
	virtual bool HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	virtual bool HandlePoint(PTOUCHINPUT ptList, int ptCount);
	virtual bool HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnChangeTeam(int idx);
	void OnStartCounting();
	void OnFinishCounting();
	void OnVolumeEvent(float fPeak);
	void OnFinishTextCounting();
	void ShowGrade();
	void FinishShowGrade();
private:
	bool m_bPressed;
	//Rainbow bg
	ID2D1Bitmap* m_bg;
	//Coin animation bg
	ID2D1Bitmap** m_coin_bg;
	//Ballon Bg
	ID2D1Bitmap* m_balloon_bg;
	CFaceIcon* m_FaceIcon;
	CTeamTitle* m_TeamTitle;
	CCountingText* m_CountingText;
	CTimeCounting* m_TimeCounting;
	CVolumeMeter* m_VolumeMeter;
	UINT m_nIDEvent;
	UINT m_nIDEventGrade;
	bool m_bFlashFast;
	int m_nCurrentTeamIdx;
	vector<pair<int, float>> m_teamGrade;
};

#endif

