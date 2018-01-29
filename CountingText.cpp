#include "DXUT.h"
#include "CountingText.h"
#include "CharEncode.h"

using namespace std;

tstring g_slogan[8] = { L"狗年兆豐光寶旺 業績長紅大成功",
						L"旺旺招財新年吉 光寶光寶最勝利",
						L"閃亮亮CBPA",        
						L"嗡嗡嗡 嗡嗡嗡 大家一起來做工",
						L"千里迢迢來爭冠 就是要來拿第一 啦~~",
						L"有情有意有能力 堅持努力不放棄",
						L"感恩光寶 囋嘆光寶 祝大家發大財",
						L"王牌！王牌！誰與爭鋒！"
};

static float Gradient = 0.f;
static float GradientDiff = 2.5f;

static float getSloganGradient();

static void resetSloganGradient() 
{
	Gradient = 0.f;
}
static float getSloganGradient(float maxWidth)
{
	Gradient += GradientDiff;
	if (Gradient >= maxWidth)
		Gradient = maxWidth;
	return Gradient;
}

CCountingText::CCountingText(CControlBase*parent) : CControlBase(parent)
, m_pSloganLayout(NULL)
, m_pTextSloganBmp(NULL)
, m_pTextBrush(NULL)
, m_pTextBlueBrush(NULL)
{
	

	CDXWrapper::GetD2DDC()->CreateSolidColorBrush(
		D2D1::ColorF(.0f, .0f, .0f, 1.f), &m_pTextBrush);

	CDXWrapper::GetD2DDC()->CreateSolidColorBrush(
		D2D1::ColorF(.0f, .49f, 1.0f, 1.f), &m_pTextBlueBrush);
}


CCountingText::~CCountingText()
{
	SAFE_RELEASE(m_pTextBrush);
	SAFE_RELEASE(m_pTextBlueBrush);
	SAFE_RELEASE(m_pSloganLayout);
	SAFE_RELEASE(m_pTextSloganBmp);
}

void CCountingText::Render(ID2D1DeviceContext*d2ddc) 
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
	if (m_pSloganLayout != NULL)
		d2ddc->DrawTextLayout(D2D1::Point2F(), m_pSloganLayout, m_pTextBrush);
	if (m_pTextSloganBmp != NULL)
	{
		float current_width = getSloganGradient(m_dtm.width);
		d2ddc->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		d2ddc->FillOpacityMask(
			m_pTextSloganBmp, m_pTextBlueBrush, D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL, NULL, &D2D1::RectF(0.f, 0.f, current_width, m_dtm.height));
		d2ddc->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

		if (current_width == m_dtm.width) 
		{
			__raise ReadyEvent();
		}
	}
}

void CCountingText::setTeamIdx(int idx)
{
	resetSloganGradient();
	m_nTeamIdx = idx;

	IDWriteFactory*_dwFac = CDXWrapper::GetDWriteFactory();
	IDWriteTextFormat*_textFormat = NULL;
	_dwFac->CreateTextFormat(
		L"Microsoft JhengHei",
		NULL,
		DWRITE_FONT_WEIGHT_SEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		40.0f,
		L"",
		&_textFormat);
	_textFormat->SetTextAlignment(
		DWRITE_TEXT_ALIGNMENT_LEADING);
	_textFormat->SetParagraphAlignment(
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	//full path text layout
	_dwFac->CreateTextLayout(
		g_slogan[m_nTeamIdx].c_str(), wcslen(g_slogan[m_nTeamIdx].c_str()), _textFormat, CResolution::m_imageResolutionX, CResolution::m_imageResolutionY, &m_pSloganLayout);
	//layout trim object
	DWRITE_TRIMMING _trim = { DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0 };
	IDWriteInlineObject*_textIO = NULL;
	_dwFac->CreateEllipsisTrimmingSign(_textFormat, &_textIO);
	m_pSloganLayout->SetTrimming(&_trim, _textIO);
	SAFE_RELEASE(_textIO);
	m_pSloganLayout->GetMetrics(&m_dtm);
	CDXWrapper::CreateTextOpacityBitmapAsyncEx(
		_textFormat, (LPWSTR)g_slogan[m_nTeamIdx].c_str(), m_dtm.layoutWidth, m_dtm.layoutHeight, &m_pTextSloganBmp);
}