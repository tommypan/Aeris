#include "RenderSetting.h"
#include "Log.h"

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
m_height(600),
m_pZWriteOpenState(nullptr),
m_pZWriteCloseState(nullptr)
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


	// 开启深度写入,深度检测，/关闭模板状态
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//dsDesc.StencilEnable = true; 要一起设置
	hr = m_pd3dDevice->CreateDepthStencilState(&dsDesc, &m_pZWriteOpenState);
	if (FAILED(hr))
		return hr;

	// 关闭深度写入，开启深度检测/关闭模板状态
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//dsDesc.StencilEnable = true; 要一起设置
	hr = m_pd3dDevice->CreateDepthStencilState(&dsDesc, &m_pZWriteCloseState);
	if (FAILED(hr))
		return hr;

	m_pImmediateContext->OMSetDepthStencilState(m_pZWriteOpenState, 0);

	InitBlendState();
	return true;
}

bool RenderSetting::InitBlendState()
{
	D3D11_BLEND_DESC blendStateDescription;
	// 初始化blend描述符 
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// 创建一个alpha blend状态. 
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pd3dDevice->CreateBlendState(&blendStateDescription, &m_pBlendState)))
	{
		Log::LogE("Create 'Transparent' blend state failed!");
		return false;
	}

	return true;
}
void RenderSetting::ShutDown()
{
	/*if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
	if (m_pDepthStencilView) m_pDepthStencilView->Release();
	if (m_pZWriteOpenState)m_pZWriteOpenState->Release();
	if (m_pZWriteCloseState)m_pZWriteCloseState->Release();*/
	//if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
}

void RenderSetting::SetZWrite(bool enable)
{
	if (enable)
	{
		m_pImmediateContext->OMSetDepthStencilState(m_pZWriteOpenState, 0);
	}
	else
	{
		float blendFactor[] = { 0.f,0.f,0.f,0.f };
		m_pImmediateContext->OMSetDepthStencilState(m_pZWriteCloseState, 1);
	}
}

void RenderSetting::SetAlphaBend(bool enable)
{
	if (enable)
	{
		float blendFactor[] = { 0.f,0.f,0.f,0.f };
		m_pImmediateContext->OMSetBlendState(m_pBlendState, blendFactor, 0xffffffff);
	}
	else
	{
		m_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);
	}
}