#include "MainViewport.h"
// Implement CMainViewport

CMainViewport::CMainViewport(CControlBase*parent) : CControlBase(parent),
	m_activePage(NULL),
	m_currentState(TOUCH_TEST_PROGRESS_INIT)
{
	SetSize(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	//get binary path 
	TCHAR buf[MAX_PATH];
	bool isExist;
	GetModuleFileName(NULL, buf, MAX_PATH);
	tstring currentDir = buf;
	currentDir = currentDir.substr(0, currentDir.find_last_of(_T("\\")));
	//read setting file
	tstring settingFile = currentDir + _T("\\Setting.txt");
	vector<tstring> lines;
	isExist = CWinFS::ReadLines(settingFile, lines);

	int value[MAX_PATH];
	for(int i = 0; i < lines.size(); i++)
	{
		int position = lines[i].find(_T("="));
		if(position > 1)
		{
			lines[i] = lines[i].substr(position + 2, 7);
			value[i] = (int) _wtof(lines[i].c_str());
		}
	}
	
	if(isExist)
	{
		_et->SetDefaultValue(value[1]);
		_vv->SetDefaultValue(value[4],value[5],value[6],value[7],value[8]);
		_st->SetDefaultValue(value[11],value[12],value[13]);
		_dlt->SetDefaultValue(value[16],value[17],value[18],value[19]);
		if(g_isManager)
		{
			CGhostPointTest*_gpt;
			_gpt = dynamic_cast<CGhostPointTest*>(m_childList[5]);
			_gpt->SetDefaultValue(value[22],value[23],value[24]);

			CDrawCircleTest*_dct;
			_dct = dynamic_cast<CDrawCircleTest*>(m_childList[6]);
			_dct->SetDefaultValue(value[27],value[28]);
		}
	}
	
	for(int i = 0; i < m_childList.GetSize(); i++)
	{
		ITouchCheckItem*_tci = dynamic_cast<ITouchCheckItem*>(m_childList[i]);
		if(_tci != NULL)
		{
			__hook(&ITouchCheckItem::EndTestEvent,_tci,&CMainViewport::OnTestItemTestingEnd,this);
			__hook(&ITouchCheckItem::EndSettingEvent,_tci,&CMainViewport::OnTestItemTestingEnd,this);
			__hook(&ITouchCheckItem::EndResultEvent,_tci,&CMainViewport::OnTestItemTestingEnd,this);
		}
	}

	//create log folder
	currentDir = currentDir + _T("\\TouchTestResult");
	if(!CWinFS::IsExist(currentDir))
		SHCreateDirectoryEx(NULL, currentDir.c_str(), NULL);
	
	//create CInitPage and CFinishPage by ITouchCheckItem list
	m_initPage = new CInitPage(&m_childList,this);
	__hook(&CInitPage::SettingToggleEvent,m_initPage,&CMainViewport::OnToggleSettings,this);
	__hook(&CInitPage::StartTesting,m_initPage,&CMainViewport::OnStartingTest,this);
	m_initPage->SetLogFolderPath(currentDir);

	m_finishPage = new CFinishPage(&m_childList,this);
	__hook(&CFinishPage::ResultToggleEvent,m_finishPage,&CMainViewport::OnToggleResult,this);
	__hook(&CFinishPage::EndResulting,m_finishPage,&CMainViewport::OnFinishPageEnd,this);

	m_activePage = m_initPage;
}

CMainViewport::~CMainViewport()
{
	SaveSettingValue();

	__unhook(&CInitPage::SettingToggleEvent,m_initPage,&CMainViewport::OnToggleSettings,this);
	__unhook(&CInitPage::StartTesting,m_initPage,&CMainViewport::OnStartingTest,this);
	SAFE_DELETE(m_initPage);

	__unhook(&CFinishPage::ResultToggleEvent,m_finishPage,&CMainViewport::OnToggleResult,this);
	__unhook(&CFinishPage::EndResulting,m_finishPage,&CMainViewport::OnFinishPageEnd,this);
	SAFE_DELETE(m_finishPage);

	for(int i = 0; i < m_childList.GetSize(); i++)
	{
		ITouchCheckItem*_tci = dynamic_cast<ITouchCheckItem*>(m_childList[i]);
		if(_tci != NULL)
		{
			__unhook(&ITouchCheckItem::EndTestEvent,_tci,&CMainViewport::OnTestItemTestingEnd,this);
			__unhook(&ITouchCheckItem::EndSettingEvent,_tci,&CMainViewport::OnTestItemTestingEnd,this);
			__unhook(&ITouchCheckItem::EndResultEvent,_tci,&CMainViewport::OnTestItemTestingEnd,this);
		}
		SAFE_DELETE(m_childList[i]);
	}
}

void CMainViewport::SaveSettingValue()
{
	//get binary path 
	TCHAR buf[MAX_PATH];
	GetModuleFileName(NULL, buf, MAX_PATH);
	tstring currentDir = buf;
	currentDir = currentDir.substr(0, currentDir.find_last_of(_T("\\")));
	//write setting file
	tstring settingFile = currentDir + _T("\\Setting.txt");
	vector<tstring> lines;
	int value[10];
	wchar_t istr[10];

	CEdgeTouch*_et;
	_et = dynamic_cast<CEdgeTouch*>(m_childList[0]);
	_et->GetDefaultValue(value);
	for(int i = 0; i < 1; i++)
	{
		_itow_s(value[i], istr, 10);
		lines.push_back(istr);
	}

	CVelocityVisualization*_vv;
	_vv = dynamic_cast<CVelocityVisualization*>(m_childList[1]);
	_vv->GetDefaultValue(value);
	for(int i = 0; i < 5; i++)
	{
		_itow_s(value[i], istr, 10);
		lines.push_back(istr);
	}

	CStationaryTest*_st;
	_st = dynamic_cast<CStationaryTest*>(m_childList[2]);
	_st->GetDefaultValue(value);
	for(int i = 0; i < 3; i++)
	{
		_itow_s(value[i], istr, 10);
		lines.push_back(istr);
	}

	CDrawLineTest*_dlt;
	_dlt = dynamic_cast<CDrawLineTest*>(m_childList[3]);
	_dlt->GetDefaultValue(value);
	for(int i = 0; i < 4; i++)
	{
		_itow_s(value[i], istr, 10);
		lines.push_back(istr);
	}
	if(g_isManager)
	{
		CGhostPointTest*_gpt;
		_gpt = dynamic_cast<CGhostPointTest*>(m_childList[5]);
		_gpt->GetDefaultValue(value);
		for(int i = 0; i < 3; i++)
		{
			_itow_s(value[i], istr, 10);
			lines.push_back(istr);
		}

		CDrawCircleTest*_dct;
		_dct = dynamic_cast<CDrawCircleTest*>(m_childList[6]);
		_dct->GetDefaultValue(value);
		for(int i = 0; i < 2; i++)
		{
			_itow_s(value[i], istr, 10);
			lines.push_back(istr);
		}
	}
	vector<tstring> content;

	content.push_back(_T("Edge Touch Sensitivity"));
	content.push_back(_T("Edge Threshold = ") + lines[0]);
	content.push_back(_T(""));
	content.push_back(_T("Touch Point Distribution"));
	content.push_back(_T("Point Interval Threshold = ") + lines[1]);
	content.push_back(_T("Line Distance threshold = ") + lines[2]);
	content.push_back(_T("Time threshold = ") + lines[3]);
	content.push_back(_T("Ignore Head Length = ") + lines[4]);
	content.push_back(_T("Ignore Tail Length = ") + lines[5]);
	content.push_back(_T(""));
	content.push_back(_T("Finger Stationary"));
	content.push_back(_T("Test Time = ") + lines[6]);
	content.push_back(_T("Test point count = ") + lines[7]);
	content.push_back(_T("Error point threshold = ") + lines[8]);
	content.push_back(_T(""));
	content.push_back(_T("Linearity"));
	content.push_back(_T("Error line threshold = ") + lines[9]);
	content.push_back(_T("Box speed(low) = ") + lines[10]);
	content.push_back(_T("Box speed(middle) = ") + lines[11]);
	content.push_back(_T("Box speed(high) = ") + lines[12]);
	if(g_isManager)
	{
		content.push_back(_T(""));
		content.push_back(_T("Ghost Test"));
		content.push_back(_T("Test time of empty frame = ") + lines[13]);
		content.push_back(_T("Test time of point test = ") + lines[14]);
		content.push_back(_T("Specific test point count = ") + lines[15]);
		content.push_back(_T(""));
		content.push_back(_T("DrawCircle Test"));
		content.push_back(_T("Radius = ") + lines[16]);
		content.push_back(_T("Error line threshold = ") + lines[17]);
	}
	CWinFS::WriteLines(settingFile, content);
}

bool CMainViewport::HandlePoint(PTOUCHINPUT ptList, int ptCount)
{
	if(m_activePage->HandlePoint(ptList,ptCount))
		return true;
	return false;
}

void CMainViewport::Render(ID2D1DeviceContext*d2ddc)
{
	m_activePage->Render(d2ddc);
}

bool CMainViewport::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_activePage != NULL)
		m_activePage->MsgProc(uMsg,wParam,lParam);
	return false;
}

void CMainViewport::UpdateTransform()
{
	CControlBase::UpdateTransform();
	if(m_initPage != NULL)
		m_initPage->UpdateTransform();
	if(m_finishPage != NULL)
		m_finishPage->UpdateTransform();
}

void CMainViewport::OnStartingTest()
{	
	//get binary path
	TCHAR buf[MAX_PATH];
	bool isExist;
	GetModuleFileName(NULL, buf, MAX_PATH);
	tstring currentDir = buf;
	currentDir = currentDir.substr(0, currentDir.find_last_of(_T("\\")));

	//create log folder
	currentDir = currentDir + _T("\\TouchTestResult");
	if(!CWinFS::IsExist(currentDir))
		SHCreateDirectoryEx(NULL, currentDir.c_str(), NULL);
	//Get time
	tstringstream tss;
	tstring fileName;
	SYSTEMTIME st;
	GetLocalTime(&st);
	tss.str(_T(""));
	tss << st.wYear; 
	tss << _T("."); 
	tss << st.wMonth;
	tss << _T("."); 
	tss << st.wDay;
	tss << _T("."); 
	tss << st.wHour;
	tss << _T("."); 
	tss << st.wMinute;
	tss << _T("."); 
	tss << st.wSecond;
	currentDir = currentDir + _T("\\") + tss.str();
	SHCreateDirectoryEx(NULL, currentDir.c_str(), NULL);
	
	CEdgeTouch*_et;
	_et = dynamic_cast<CEdgeTouch*>(m_childList[0]);
	_et->SetLogFolderPath(currentDir);

	CVelocityVisualization*_vv;
	_vv = dynamic_cast<CVelocityVisualization*>(m_childList[1]);
	_vv->SetLogFolderPath(currentDir);

	CStationaryTest*_st;
	_st = dynamic_cast<CStationaryTest*>(m_childList[2]);
	_st->SetLogFolderPath(currentDir);
	
	CDrawLineTest*_dlt;
	_dlt = dynamic_cast<CDrawLineTest*>(m_childList[3]);
	_dlt->SetLogFolderPath(currentDir);
	
	if(g_isManager)
	{
		CDrawCircleTest*_dct;
		_dct = dynamic_cast<CDrawCircleTest*>(m_childList[6]);
		_dct->SetLogFolderPath(currentDir);
	}

	if(m_currentState != TOUCH_TEST_PROGRESS_INIT)
		return;
	//reset all test item first
	for(int i = 0; i < m_childList.GetSize(); i++)
	{
		ITouchCheckItem*_tci = dynamic_cast<ITouchCheckItem*>(m_childList[i]);
		if(_tci != NULL)
			_tci->ResetTestItem();
	}
	//find first enabled touch check item
	for(int i = 0; i < m_childList.GetSize(); i++)
	{
		ITouchCheckItem*_tci = dynamic_cast<ITouchCheckItem*>(m_childList[i]);
		if(_tci != NULL)
		{
			if(_tci->IsEnableTest())
			{
				m_activePage = m_childList[i];
				_tci->StartTestItem();
				m_currentState = TOUCH_TEST_PROGRESS_TEST;
				return;
			}
		}
	}
}

void CMainViewport::OnToggleSettings(ITouchCheckItem*sender)
{
	if(m_currentState != TOUCH_TEST_PROGRESS_INIT)
		return;
	if(sender == NULL)
		return;
	for(int i = 0; i < m_childList.GetSize(); i++)
	{
		ITouchCheckItem*_tci = dynamic_cast<ITouchCheckItem*>(m_childList[i]);
		if(sender == _tci)
		{
			m_activePage = m_childList[i];
			_tci->ToggleSetting();
			m_currentState = TOUCH_TEST_PROGRESS_CONFIG;
			return;
		}
	}
}

void CMainViewport::OnToggleResult(ITouchCheckItem*sender)
{
	if(m_currentState != TOUCH_TEST_PROGRESS_FINISH)
		return;
	if(sender == NULL)
		return;
	for(int i = 0; i < m_childList.GetSize(); i++)
	{
		ITouchCheckItem*_tci = dynamic_cast<ITouchCheckItem*>(m_childList[i]);
		if(sender == _tci)
		{
			m_activePage = m_childList[i];
			_tci->ToggleResult();
			m_currentState = TOUCH_TEST_PROGRESS_RESULT;
			return;
		}
	}
}

void CMainViewport::OnTestItemTestingEnd(ITouchCheckItem*sender)
{
	if(sender == NULL)
		return;
	switch(m_currentState)
	{
	case TOUCH_TEST_PROGRESS_TEST:
		{
			//find if next enabled test check item exist and make active, otherwise enter finish state
			int _nextTouchCheckItemIndex = -1;
			for(int i = 0; i < m_childList.GetSize(); i++)
			{
				ITouchCheckItem*_tci = dynamic_cast<ITouchCheckItem*>(m_childList[i]);
				if(_tci == sender)
				{
					_nextTouchCheckItemIndex = i + 1;
					break;
				}
			}
			//search first enabled test item follow by previous end one
			for(int i = _nextTouchCheckItemIndex; i < m_childList.GetSize(); i++)
			{
				ITouchCheckItem*_tci = dynamic_cast<ITouchCheckItem*>(m_childList[i]);
				if(_tci != NULL && _tci->IsEnableTest())
					break;
				else
					_nextTouchCheckItemIndex++;
			}
			//check index valid
			if(_nextTouchCheckItemIndex < 0 || _nextTouchCheckItemIndex >= m_childList.GetSize())
			{
				//processed to last, go finish state
				m_currentState = TOUCH_TEST_PROGRESS_FINISH;
				m_activePage = m_finishPage;
			}
			else
			{
				//go next ITouchCheckItem
				ITouchCheckItem*_tci = dynamic_cast<ITouchCheckItem*>(m_childList[_nextTouchCheckItemIndex]);
				if(_tci == NULL)
				{
					//cannot happen!!
					ExitProcess(-1);
				}
				_tci->StartTestItem();
				m_activePage = m_childList[_nextTouchCheckItemIndex];
			}
		}
		break;
	case TOUCH_TEST_PROGRESS_CONFIG:
		{
			m_currentState = TOUCH_TEST_PROGRESS_INIT;
			m_activePage = m_initPage;
		}
		break;
	case TOUCH_TEST_PROGRESS_RESULT:
		{
			m_currentState = TOUCH_TEST_PROGRESS_FINISH;
			m_activePage = m_finishPage;
		}
		break;
	}
}

void CMainViewport::OnFinishPageEnd()
{
	m_currentState = TOUCH_TEST_PROGRESS_INIT;
	m_activePage = m_initPage;
	
	CEdgeTouch*_et;
	_et = dynamic_cast<CEdgeTouch*>(m_childList[0]);
	_et->TakeSnapShot();

	CVelocityVisualization*_vv;
	_vv = dynamic_cast<CVelocityVisualization*>(m_childList[1]);
	_vv->TakeSnapShot();
	
	CStationaryTest*_st;
	_st = dynamic_cast<CStationaryTest*>(m_childList[2]);
	_st->TakeSnapShot();

	CDrawLineTest*_dlt;
	_dlt = dynamic_cast<CDrawLineTest*>(m_childList[3]);
	_dlt->TakeSnapshot();

	if(g_isManager)
	{
		CDrawCircleTest*_dct;
		_dct = dynamic_cast<CDrawCircleTest*>(m_childList[6]);
		_dct->TakeSnapshot();	
	}
}