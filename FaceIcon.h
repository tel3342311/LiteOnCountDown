#pragma once

#ifndef _FACEICON_
#define _FACEICON_

#include "BaseControl.h"
#include "AspectRatioDefine.h"
#include "DXWrapper.h"

enum FACEMODE{
	PREPARE,
	COUNTDOWN,
	START,
	PROGRESS_1,
	PROGRESS_2,
	PROGRESS_3,
	FINISH
};

static FACEMODE g_dragMode = PREPARE;

class CFaceIcon :
	public CControlBase
{
public:
	CFaceIcon(CControlBase*parent = NULL);
	~CFaceIcon();
	//override function
	virtual void Render(ID2D1DeviceContext*);
protected:
	ID2D1Bitmap** m_pFaceIcon;
};

#endif