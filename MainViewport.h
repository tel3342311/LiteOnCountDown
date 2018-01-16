#pragma once

#ifndef _MAINIEWPORT_
#define _MAINVIEWPORT_

#include "BaseControl.h"
#include "DXWrapper.h"

enum TOUCH_TEST_PROGRESS
{
	TOUCH_TEST_PROGRESS_INIT = 0,
	TOUCH_TEST_PROGRESS_TEST,
	TOUCH_TEST_PROGRESS_CONFIG,
	TOUCH_TEST_PROGRESS_RESULT,
	TOUCH_TEST_PROGRESS_FINISH,
};

extern bool g_isManager;

class CMainViewport;

class CMainViewport : public CControlBase
{
public:
	CMainViewport(CControlBase* = NULL);
	~CMainViewport();

	virtual bool HandlePoint(PTOUCHINPUT ptList, int ptCount);
	virtual void Render(ID2D1DeviceContext*);
	virtual void UpdateTransform();
	virtual bool MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	//object instance

	CControlBase*m_activePage;

	TOUCH_TEST_PROGRESS m_currentState;
};

#endif