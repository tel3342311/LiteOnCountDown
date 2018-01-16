#include "DXUT.h"
#include "MICCapture.h"


CMICCapture::CMICCapture() : m_pEnumerator(NULL)
, m_pDevice(NULL)
, m_pDeviceCollection(NULL)
, m_pMeterInfo(NULL)
, m_pProperty(NULL)
, m_bInit(false)
{
	Init();
}


CMICCapture::~CMICCapture()
{
	UnInit();
}

HRESULT CMICCapture::Init() 
{
	if (m_bInit)
		return S_OK;
	HRESULT hr;
	// Get enumerator for audio endpoint devices.
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&m_pEnumerator);
	if (FAILED(hr))
		return hr;

	// Get audio-capturing device.
	hr = m_pEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &m_pDeviceCollection);

	UINT count = 0;
	LPUWSTR pwszID = NULL;
	hr = m_pDeviceCollection->GetCount(&count);
	for (ULONG i = 0; i < count; i++) {
		hr = m_pDeviceCollection->Item(i, &m_pDevice);

		hr = m_pDevice->GetId(&pwszID);

		hr = m_pDevice->OpenPropertyStore(STGM_READ, &m_pProperty);

		PROPVARIANT varName;
		PropVariantInit(&varName);

		hr = m_pProperty->GetValue(PKEY_Device_FriendlyName, &varName);

		wprintf(varName.pwszVal);
		SAFE_RELEASE(m_pProperty);
	}
	hr = m_pDevice->Activate(__uuidof(IAudioMeterInformation),
		CLSCTX_ALL, NULL, (void**)&m_pMeterInfo);
}

HRESULT CMICCapture::UnInit() 
{
	if (!m_bInit)
		return S_OK;

	SAFE_RELEASE(m_pEnumerator);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pDeviceCollection);
	SAFE_RELEASE(m_pMeterInfo);
	SAFE_RELEASE(m_pProperty);

	m_bInit = FALSE;
	return S_OK;
}

HRESULT CMICCapture::GetPeakValue(float *pfPeak) 
{
	if (m_pMeterInfo == NULL)
		return E_FAIL;
	m_pMeterInfo->GetPeakValue(pfPeak);
	return S_OK;
}
