#include "DXUT.h"
#include "Ranking.h"
#include <algorithm>

using namespace std;

static int getCoinFlash();
static int getChampFlash();

CRanking::CRanking(CControlBase*parent) : CControlBase(parent)
, m_pRankBG(NULL)
, m_ppChampionBg(NULL)
, m_ppDigitBitmap(NULL)
, m_pSemiColon(NULL)
, m_ppTeamTitle(NULL)
, m_coin_bg(NULL)
, m_teamGrade(NULL)
{
	m_coin_bg = new ID2D1Bitmap*[2];
	for (int i = 0; i < 2; i++) {
		m_coin_bg[i] = NULL;
	}
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_gnd_1-1.png", &m_coin_bg[0], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_gnd_1-2.png", &m_coin_bg[1], CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);


	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_gnd_ranking.png", &m_pRankBG, CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	m_ppChampionBg = new ID2D1Bitmap*[5];
	for (int i = 0; i < 5; i++) {
		m_ppChampionBg[i] = NULL;
	}
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_no1-1.png", &m_ppChampionBg[0], CResolution::m_screenResolutionX / 2.04f, CResolution::m_screenResolutionY / 6.83f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_no1-2.png", &m_ppChampionBg[1], CResolution::m_screenResolutionX / 2.04f, CResolution::m_screenResolutionY / 6.83f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_no1-3.png", &m_ppChampionBg[2], CResolution::m_screenResolutionX / 2.04f, CResolution::m_screenResolutionY / 6.83f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_no1-4.png", &m_ppChampionBg[3], CResolution::m_screenResolutionX / 2.04f, CResolution::m_screenResolutionY / 6.83f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_no1-5.png", &m_ppChampionBg[4], CResolution::m_screenResolutionX / 2.04f, CResolution::m_screenResolutionY / 6.83f);

	m_ppDigitBitmap = new ID2D1Bitmap*[10];
	for (int i = 0; i < 10; i++) {
		m_ppDigitBitmap[i] = NULL;
	}
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number0.png", &m_ppDigitBitmap[0], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number1.png", &m_ppDigitBitmap[1], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number2.png", &m_ppDigitBitmap[2], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number3.png", &m_ppDigitBitmap[3], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number4.png", &m_ppDigitBitmap[4], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number5.png", &m_ppDigitBitmap[5], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number6.png", &m_ppDigitBitmap[6], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number7.png", &m_ppDigitBitmap[7], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number8.png", &m_ppDigitBitmap[8], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_number9.png", &m_ppDigitBitmap[9], CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);

	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_dot.png", &m_pSemiColon, CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 19.63f);

	m_ppTeamTitle = new ID2D1Bitmap*[8];
	for (int i = 0; i < 8; i++) {
		m_ppTeamTitle[i] = NULL;
	}
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_team-1.png", &m_ppTeamTitle[0], CResolution::m_screenResolutionX / 4.68f, CResolution::m_screenResolutionY / 9.f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_team-2.png", &m_ppTeamTitle[1], CResolution::m_screenResolutionX / 4.68f, CResolution::m_screenResolutionY / 9.f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_team-3.png", &m_ppTeamTitle[2], CResolution::m_screenResolutionX / 4.68f, CResolution::m_screenResolutionY / 9.f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_team-4.png", &m_ppTeamTitle[3], CResolution::m_screenResolutionX / 4.68f, CResolution::m_screenResolutionY / 9.f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_team-5.png", &m_ppTeamTitle[4], CResolution::m_screenResolutionX / 4.68f, CResolution::m_screenResolutionY / 9.f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_team-6.png", &m_ppTeamTitle[5], CResolution::m_screenResolutionX / 4.68f, CResolution::m_screenResolutionY / 9.f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_team-7.png", &m_ppTeamTitle[6], CResolution::m_screenResolutionX / 4.68f, CResolution::m_screenResolutionY / 9.f);
	CDXWrapper::LoadImageFromFileAsyncEx(L"images\\result_img_team-8.png", &m_ppTeamTitle[7], CResolution::m_screenResolutionX / 4.68f, CResolution::m_screenResolutionY / 9.f);

}


CRanking::~CRanking()
{
	SAFE_RELEASE(m_pRankBG);

	for (int i = 0; i < 5; i++) {
		SAFE_RELEASE(m_ppChampionBg[i]);
	}
	SAFE_DELETE(m_ppChampionBg);

	for (int i = 0; i < 10; i++) {
		SAFE_RELEASE(m_ppDigitBitmap[i]);
	}
	SAFE_DELETE(m_ppDigitBitmap);
	SAFE_RELEASE(m_pSemiColon);

	for (int i = 0; i < 8; i++) {
		SAFE_RELEASE(m_ppTeamTitle[i]);
	}
	SAFE_DELETE(m_ppTeamTitle);
}

void CRanking::Render(ID2D1DeviceContext*d2ddc)
{
	if (!m_bVisible || m_opacity == 0.0f || m_teamGrade == NULL)
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

	int idx = getCoinFlash();
	if (m_coin_bg[idx] != NULL)
		d2ddc->DrawBitmap(m_coin_bg[idx]);

	if (m_pRankBG != NULL)
		d2ddc->DrawBitmap(m_pRankBG);
	
	int idxChamp = getChampFlash();
	if (m_ppChampionBg[idxChamp] != NULL)
	{
		d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 192.f, 0) * _world);
		d2ddc->DrawBitmap(m_ppChampionBg[idxChamp]);
	}
	for (int i = 0; i < 8; i++)
	{
		int _nIdx = m_teamGrade->at(i).first;
		float _grade = m_teamGrade->at(i).second;
		if (m_ppTeamTitle[_nIdx] != NULL)
		{
			float headOffset = 0;
			if (i % 4 == 0)
				headOffset = CResolution::m_screenResolutionY / 54.f;
			if ( i < 4 )
				d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 10.66f, CResolution::m_screenResolutionY / 6.35f * i + headOffset) * _world);
			else
				d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 1.67f, CResolution::m_screenResolutionY / 6.35f * (i - 4) + headOffset) * _world);
			d2ddc->DrawBitmap(m_ppTeamTitle[_nIdx]);
		}

		D2D1::Matrix3x2F _mTeamTitle; 
		d2ddc->GetTransform(&_mTeamTitle);
		UpdateDigit(_grade);
		d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 4.17f, CResolution::m_screenResolutionY / 36.f) * _mTeamTitle);
		if (m_ppDigitBitmap[m_pDigit[0]] != NULL)
			d2ddc->DrawBitmap(m_ppDigitBitmap[m_pDigit[0]]);
		d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 4.17f + CResolution::m_screenResolutionX / 34.9f, CResolution::m_screenResolutionY / 36.f) * _mTeamTitle);
		if (m_ppDigitBitmap[m_pDigit[1]] != NULL)
			d2ddc->DrawBitmap(m_ppDigitBitmap[m_pDigit[1]]);
		d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 4.17f + CResolution::m_screenResolutionX / 34.9f * 2, CResolution::m_screenResolutionY / 36.f) * _mTeamTitle);
		if (m_pSemiColon != NULL)
			d2ddc->DrawBitmap(m_pSemiColon);
		d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 4.17f + CResolution::m_screenResolutionX / 34.9f * 3, CResolution::m_screenResolutionY / 36.f) * _mTeamTitle);
		if (m_ppDigitBitmap[m_pDigit[2]] != NULL)
			d2ddc->DrawBitmap(m_ppDigitBitmap[m_pDigit[2]]);
		d2ddc->SetTransform(D2D1::Matrix3x2F::Translation(CResolution::m_screenResolutionX / 4.17f + CResolution::m_screenResolutionX / 34.9f * 4, CResolution::m_screenResolutionY / 36.f) * _mTeamTitle);
		if (m_ppDigitBitmap[m_pDigit[3]] != NULL)
			d2ddc->DrawBitmap(m_ppDigitBitmap[m_pDigit[3]]);
	}
}

void CRanking::UpdateDigit(float fGrade)
{
	float sec = fGrade;
	m_pDigit[0] = sec / 10.f > 10 ? 0 : sec / 10.f;
	m_pDigit[1] = (int)sec % 10;
	m_pDigit[2] = (int)(sec * 10.f) % 10;
	m_pDigit[3] = (int)(sec * 100.f) % 10;
}

bool pairCompare(const std::pair<int, float>& firstElem, const std::pair<int, float>& secondElem) 
{
	return firstElem.second > secondElem.second;

}

void CRanking::UpdateRank(vector<pair<int, float>>* teamGrade)
{
	m_teamGrade = teamGrade;
	std::sort(m_teamGrade->begin(), m_teamGrade->end(), pairCompare);
}

static float Opacity = 0.f;
static float OpacityDiff = 0.083f;

static int getCoinFlash()
{
	Opacity += OpacityDiff;
	if (Opacity >= 2.f)
		Opacity = 0.f;
	return (int)Opacity;
}

static float champ = 0.f;
static float champDiff = 0.166f;

static int getChampFlash()
{
	champ += champDiff;
	if (champ >= 5.f)
		champ = 0.f;
	return (int)champ;
}