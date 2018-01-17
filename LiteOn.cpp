//--------------------------------------------------------------------------------------
// File: LiteOn.cpp
//
// Empty starting point for new Direct3D 9 and/or Direct3D 11 applications
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "DXWrapper.h"
#include "CManipulationEventSink.h"
#include "AppMgr.h"
#include "AspectRatioDefine.h"
#include "DelayDeleteHelper.h"
#include "LiteOnViewPort.h"


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D11 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	pDeviceSettings->d3d11.CreateFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	pDeviceSettings->d3d11.AutoDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	pDeviceSettings->d3d11.sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	pDeviceSettings->d3d11.DriverType = D3D_DRIVER_TYPE_HARDWARE;
	pDeviceSettings->d3d11.DeviceFeatureLevel = D3D_FEATURE_LEVEL_9_3;
	pDeviceSettings->d3d11.sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	pDeviceSettings->d3d11.SyncInterval = 1;
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext )
{
	IDXGIDevice1 * pDXGIDevice;
	HRESULT hr = pd3dDevice->QueryInterface(__uuidof(IDXGIDevice1), (void **)&pDXGIDevice);
	if (SUCCEEDED(hr))
	{
		hr = CDXWrapper::OnD3DDeviceCreated(pDXGIDevice);
		UINT _maxLatency;
		hr = pDXGIDevice->GetMaximumFrameLatency(&_maxLatency);
		if (_maxLatency != 1)
			pDXGIDevice->SetMaximumFrameLatency(1);
		/*
		if(pd3dDevice->GetFeatureLevel() <= D3D_FEATURE_LEVEL_9_3)
		{
		//try gaining more GPU priority if hardware does not support feature level above 10.0
		pDXGIDevice->SetGPUThreadPriority(2);
		}*/
		SAFE_RELEASE(pDXGIDevice);
	}
	return hr;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain1* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr = S_OK;

	hr = CDXWrapper::OnD3DSwapchainResized(pd3dDevice,pSwapChain,pBackBufferSurfaceDesc);
	CDXWrapper::UnLockDeviceUsage(false);
	
    return hr;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                  double fTime, float fElapsedTime, void* pUserContext )
{
	/*
	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    pd3dImmediateContext->OMSetRenderTargets( 1, &pRTV, pDSV );*/

	ID2D1DeviceContext*d2ddc = CDXWrapper::GetD2DDC();
	if(d2ddc == NULL)
		return;
	d2ddc->BeginDraw();
	d2ddc->Clear(D2D1::ColorF(.0f,.7f,.7f));

	//draw UI here
	if (CLiteOn::m_liteOnViewPort != NULL)
		CLiteOn::m_liteOnViewPort->Render(d2ddc);

	d2ddc->EndDraw();

}


//--------------------------------------------------------------------------------------
// Release D3D11resources created in OnD3D10ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
    CDXWrapper::LockDeviceUsage();
	CDXWrapper::OnD3DSwapchainReleasing();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
	CDXWrapper::OnD3DDeviceDestroyed();
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	UINT cInputs = 0;
	PTOUCHINPUT pInputs = NULL;
	bool bIncomingPoints = bIncomingPoints = CManipulationEventSink::TranslateWMMessageToPointData(pInputs, cInputs, uMsg, wParam, lParam);

	if(bIncomingPoints)
	{
		//send point to UI controls
		delete[] pInputs;
	}
	else 
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
		case WM_CHAR:
		{
			if (CLiteOn::m_liteOnViewPort != NULL && CLiteOn::m_liteOnViewPort->HandleKeyboard(uMsg, wParam, lParam))
				return true;
		}
			break;
		default:
			break;
		}
	}

    return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Call if device was removed.  Return true to find a new device, false to quit
//--------------------------------------------------------------------------------------
bool CALLBACK OnDeviceRemoved( void* pUserContext )
{
    return false;
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device (either D3D9 or D3D11) 
    // that is available on the system depending on which D3D callbacks are set below

    // Set general DXUT callbacks
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackDeviceRemoved( OnDeviceRemoved );

    // Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
    DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
    DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );

    // Perform any application-level initialization here

    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"LiteOn" );
	//CAppMgr::SetFramelessWindow(DXUTGetHWND());
	//CAppMgr::SetNonActivateWindow(DXUTGetHWND());
	//CAppMgr::SetLayerWindow(DXUTGetHWND(), true);
	//SetLayeredWindowAttributes(DXUTGetHWND(), 0, BYTE(0), LWA_ALPHA);
	
	//manipulation lib
	CManipulationEventSink::m_majorInstance = new CManipulationEventSink;
	CManipulationEventSink::m_majorInstance->Init();

	DX::ThrowIfFailed(CDXWrapper::InitDeviceIndependentResources());
	//initial lock
	CDXWrapper::LockDeviceUsage();
	CResolution::Init();
    //Create D3D11 device
    DXUTCreateDevice( D3D_FEATURE_LEVEL_11_1, true, CResolution::m_screenResolutionX, CResolution::m_screenResolutionY );

#pragma region Create UI object

	//Create UI control
	CDelayDeleteHelperAsync::InitHelper(5);
	CDXWrapper::InitExFunction(4);
	CLiteOn::m_liteOnViewPort = new CLiteOnViewPort;
	CLiteOn::m_liteOnViewPort->SetLocation(0, 0);
	CLiteOn::m_liteOnViewPort->SetSize(CResolution::m_screenResolutionX, CResolution::m_screenResolutionY);
	//if(CResolution::m_screenResolutionX < CResolution::m_screenResolutionY)
	//initial idle monitor
	//CIdleMonitor::EnterIdleCount();
#pragma endregion

    DXUTMainLoop(); // Enter into the DXUT render loop
    // Perform any application-level cleanup here
	delete CLiteOn::m_liteOnViewPort;
	delete CManipulationEventSink::m_majorInstance;
	CDXWrapper::UnInitExFunction();

	CDelayDeleteHelperAsync::CleanPool();

	CDXWrapper::UnLockDeviceUsage(true);
	CDXWrapper::UnInitDeviceIndependentResources();

    return DXUTGetExitCode();
}


