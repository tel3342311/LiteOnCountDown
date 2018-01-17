#include "DXUT.h"
#include "CountingText.h"
#include "CharEncode.h"

using namespace std;

tstring g_slogan[8] = { L"��z�U���M�ȱ���A��ۥ��_�@�_�o�o�o",
						L"�����}�i�F������ �g�N�O�g�N�O�g�g�g",
						L"�������|���, ���F�ܤ��@��",        
						L"........................",
						L"CBPA",
						L"�ڭ̪��믫�N�O �������N����O ����V�O�����",
						L"���� ���� �j�a�@�_�Ӱ��u",
						L"�d���|�|�Ӫ��a�A���A�X���S�p��A�N�OMTD��"
};

static float getSloganGradient();

CCountingText::CCountingText(CControlBase*parent) : CControlBase(parent)
, m_pSloganLayout(NULL)
, m_pGradient_stop_collection(NULL)
, m_pLinear_gradient_brush(NULL)
, m_pTextSloganBmp(NULL)
, m_pTextBrush(NULL)
, m_pTextBlueBrush(NULL)
{
	IDWriteFactory*_dwFac = CDXWrapper::GetDWriteFactory();
	IDWriteTextFormat*_textFormat = NULL;
	_dwFac->CreateTextFormat(
		L"Segoe UI",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		72.0f,
		L"",
		&_textFormat);
	_textFormat->SetTextAlignment(
		DWRITE_TEXT_ALIGNMENT_LEADING);
	_textFormat->SetParagraphAlignment(
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	//full path text layout
	_dwFac->CreateTextLayout(
		g_slogan[0].c_str(), wcslen(g_slogan[0].c_str()), _textFormat, 600, 66, &m_pSloganLayout);
	//layout trim object
	DWRITE_TRIMMING _trim = { DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0 };
	IDWriteInlineObject*_textIO = NULL;
	_dwFac->CreateEllipsisTrimmingSign(_textFormat, &_textIO);
	m_pSloganLayout->SetTrimming(&_trim, _textIO);
	SAFE_RELEASE(_textIO);
	CDXWrapper::CreateTextOpacityBitmapAsyncEx(
		_textFormat, (LPWSTR)g_slogan[0].c_str(), 600, 66, &m_pTextSloganBmp);

	CDXWrapper::GetD2DDC()->CreateSolidColorBrush(
		D2D1::ColorF(.0f, .0f, .0f, 1.f), &m_pTextBrush);

	CDXWrapper::GetD2DDC()->CreateSolidColorBrush(
		D2D1::ColorF(.0f, .5f, 1.0f, 1.f), &m_pTextBlueBrush);
}


CCountingText::~CCountingText()
{
}

void CCountingText::Render(ID2D1DeviceContext*d2ddc) {
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
	if (m_pTextSloganBmp != NULL)
	{
		d2ddc->DrawTextLayout(D2D1::Point2F(), m_pSloganLayout, m_pTextBrush);
		d2ddc->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		d2ddc->FillOpacityMask(
			m_pTextSloganBmp, m_pTextBlueBrush, D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL, NULL, &D2D1::RectF(0.f, 0.f, getSloganGradient(), 100.f));
		d2ddc->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
}

void CCountingText::setTeamIdx(int idx)
{
	m_nTeamIdx = idx;
}

ID2D1LinearGradientBrush* CCountingText::getBrush(float fIdx)
{
	SAFE_RELEASE(m_pGradient_stop_collection);
	SAFE_RELEASE(m_pLinear_gradient_brush);
	D2D1_GRADIENT_STOP gradient_stops[3];
	gradient_stops[0].color = D2D1::ColorF(0.f, 0.5, 1.f);
	gradient_stops[0].position = 0.0f;
	gradient_stops[1].color = D2D1::ColorF(D2D1::ColorF::Black);
	gradient_stops[1].position = fIdx;
	gradient_stops[2].color = D2D1::ColorF(D2D1::ColorF::Black);
	gradient_stops[2].position = 1.f;
	CDXWrapper::GetD2DDC()->CreateGradientStopCollection(gradient_stops, 3, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &m_pGradient_stop_collection);
	CDXWrapper::GetD2DDC()->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 33), D2D1::Point2F(1024.f,33)),
		m_pGradient_stop_collection, &m_pLinear_gradient_brush);
	return m_pLinear_gradient_brush;
}

static float Gradient = 0.f;
static float GradientDiff = 2.f;

static float getSloganGradient()
{
	Gradient += GradientDiff;
	if (Gradient >= 600.f)
		Gradient = 0.f;
	return Gradient;
}