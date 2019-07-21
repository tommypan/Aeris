#include "RenderSetting.h"

RenderSetting::RenderSetting():
m_mainWndCaption(L"Directx11 Application"),
m_driverType(D3D_DRIVER_TYPE_HARDWARE),
m_featureLevel(D3D_FEATURE_LEVEL_11_0),
m_pd3dDevice(nullptr),
m_pImmediateContext(nullptr),
m_pRenderTargetView(nullptr),
m_pDepthStencilBuffer(nullptr),
m_pDepthStencilView(nullptr),
m_pSwapChain(nullptr),
m_hWnd(nullptr),
m_width(800),
m_height(600)
{}

RenderSetting::~RenderSetting()
{
	ShutDown();
}

float RenderSetting::AspectRatio() const
{
	return static_cast<float>(m_width / m_height);
}

bool RenderSetting::InitDirect3D(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	m_hInstance = hInstance;
	m_hWnd = hWnd;
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_width = rc.right - rc.left;
	m_height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Width = m_width;
	sd.BufferDesc.Height = m_height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;	
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	//create device and swapchain
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		hr = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[driverTypeIndex], nullptr,
			createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &m_pSwapChain,
			&m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
		if (SUCCEEDED(hr))
		{
			m_driverType = driverTypes[driverTypeIndex];
			break;
		}
	}
	if (FAILED(hr))
		return hr;

	//create render target view
	ID3D11Texture2D *pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
	

	//create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = m_width;
	descDepth.Height = m_height;
	descDepth.ArraySize = 1;
	descDepth.MipLevels = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencilBuffer);
	if (FAILED(hr))
		return hr;

	//create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &descDSV, &m_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	
	//setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(m_width);
	vp.Height = static_cast<float>(m_height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_pImmediateContext->RSSetViewports(1, &vp);


	//test start
	// 关闭深度测试的深度/模板状态
	// 若绘制非透明物体，务必严格按照绘制顺序
	// 绘制透明物体则不需要担心绘制顺序？、/
	//D3D11_DEPTH_STENCIL_DESC dsDesc;
	//ZeroMemory(&dsDesc, sizeof(dsDesc));//--------------
	//ID3D11DepthStencilState *ppDepthStencilState = nullptr;
	////m_pImmediateContext->OMGetDepthStencilState(&ppDepthStencilState,0);
	////ppDepthStencilState->GetDesc(&dsDesc);
	//dsDesc.DepthEnable = false;
	//dsDesc.StencilEnable = false;
	//hr = m_pd3dDevice->CreateDepthStencilState(&dsDesc, &ppDepthStencilState);
	//if (FAILED(hr))
	//	return hr;
	//m_pImmediateContext->OMSetDepthStencilState(ppDepthStencilState, 0);
	//test end
	return true;
}


void RenderSetting::ShutDown()
{
	if (m_pImmediateContext) m_pImmediateContext->ClearState();

	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
	if (m_pDepthStencilView) m_pDepthStencilView->Release();
}