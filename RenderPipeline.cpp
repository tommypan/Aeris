#include "RenderPipeline.h"
#include "Log.h"
#include "RenderTexture.h"
#include "GeometryUtility.h"
#include "Material.h"
#include "MeshRender.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Mesh.h"

RenderPipeline::RenderPipeline():DefualtColor{ 0.75f, 0.75f, 0.75f, 1.0f }
{
	ClearParam();
}

RenderPipeline::~RenderPipeline()
{
	ShutDown();
}

float RenderPipeline::AspectRatio() const
{
	return static_cast<float>(Width / Height);
}

bool RenderPipeline::InitDirect3D(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;
	HInstance = hInstance;
	HWnd = hWnd;
	RECT rc;
	GetClientRect(HWnd, &rc);
	Width = rc.right - rc.left;
	Height = rc.bottom - rc.top;

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
	sd.BufferDesc.Width = Width;
	sd.BufferDesc.Height = Height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = HWnd;
	sd.SampleDesc.Count = 1;	
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	//create device and swapchain
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		hr = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[driverTypeIndex], nullptr,
			createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &_swapChain,
			&Device, &FeatureLevel, &DeviceContext);
		if (SUCCEEDED(hr))
		{
			DriverType = driverTypes[driverTypeIndex];
			break;
		}
	}
	if (FAILED(hr))
		return false;

	_renderTextureMesh = GeometryUtility::GetInstance()->CreateScreenRect();
	_renderTextureMaterial = new Material();
	_renderTextureMaterial->SetShader("FX\\Lighting.fx");
	_renderTextureMeshRender = new MeshRender(_renderTextureMesh, _renderTextureMaterial);

	hr = InitBackBuffer();
	if (FAILED(hr))
		return false;

	hr = InitRenderTexture();
	if (FAILED(hr))
		return false;

	hr = InitDepthStencilState();
	if (FAILED(hr))
		return false;

	hr = InitBlendState();
	if (FAILED(hr))
		return false;

	return true;
}

void RenderPipeline::BuildShadowMap()
{

}

void RenderPipeline::Prepare()
{
	DepthStencilTexture->SetRenderTarget(RenderTagertTexture->GetRenderTargetView());
}

void RenderPipeline::Present()
{
	BuildRenderTextureToBackBuffer();

	_swapChain->Present(0, 0);
}

void RenderPipeline::ShutDown()
{
	/*if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
	if (m_pDepthStencilView) m_pDepthStencilView->Release();
	if (m_pZWriteOpenState)m_pZWriteOpenState->Release();
	if (m_pZWriteCloseState)m_pZWriteCloseState->Release();*/
	//if (m_pImmediateContext) m_pImmediateContext->Release();
	SAFE_DELETE(_renderTextureMesh)
	SAFE_DELETE(_renderTextureMaterial)
	SAFE_DELETE(_renderTextureMeshRender)
	if (Device) Device->Release();
}

void RenderPipeline::SetZWrite(bool enable)
{
	if (enable)
	{
		DeviceContext->OMSetDepthStencilState(_zWriteOpenState, 0);
	}
	else
	{
		float blendFactor[] = { 0.f,0.f,0.f,0.f };
		DeviceContext->OMSetDepthStencilState(_zWriteCloseState, 1);
	}
}

void RenderPipeline::SetAlphaBend(bool enable)
{
	if (enable)
	{
		float blendFactor[] = { 0.f,0.f,0.f,0.f };
		DeviceContext->OMSetBlendState(_blendState, blendFactor, 0xffffffff);
	}
	else
	{
		DeviceContext->OMSetBlendState(0, 0, 0xffffffff);
	}
}

void RenderPipeline::ClearParam()
{
	m_mainWndCaption = L"Directx11 Application";
	DriverType = D3D_DRIVER_TYPE_HARDWARE;
	FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	Device = nullptr;
	DeviceContext = nullptr;
	_backBufferRenderTargetView = nullptr;
	_swapChain = nullptr;
	HWnd = nullptr;
	Width = 800;
	Height = 600;
	_zWriteOpenState = nullptr;
	_zWriteCloseState = nullptr;
	_blendState = nullptr;
	_renderTextureMesh = nullptr;
	_renderTextureMaterial = nullptr;
	_renderTextureMeshRender = nullptr;
	_renderTexture = nullptr;
}

HRESULT RenderPipeline::InitBackBuffer()
{

	HRESULT hr = S_OK;
	ID3D11Texture2D *pBackBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
	{
		Log::LogE("Init Back Buffer 1 step failed!");
		return hr;
	}


	hr = Device->CreateRenderTargetView(pBackBuffer, nullptr, &_backBufferRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		Log::LogE("Init Back Buffer 2 step failed!");
		return hr;
	}

	return hr;
}


HRESULT RenderPipeline::InitRenderTexture()
{
	RenderTagertTexture = new RenderTexture(Device, DeviceContext, RenderTextureType::RenderTarget, Width, Height);

	DepthStencilTexture = new RenderTexture(Device, DeviceContext, RenderTextureType::RenderDepth, Width, Height);

	return true;
}

HRESULT RenderPipeline::InitDepthStencilState()
{
	HRESULT hr = S_OK;
	// 开启深度写入,深度检测，/关闭模板状态
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//dsDesc.StencilEnable = true; 要一起设置
	hr = Device->CreateDepthStencilState(&dsDesc, &_zWriteOpenState);
	if (FAILED(hr))
	{
		Log::LogE("Init Depth Stencil State 1 step failed!");
		return hr;
	}

	// 关闭深度写入，开启深度检测/关闭模板状态
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//dsDesc.StencilEnable = true; 要一起设置
	hr = Device->CreateDepthStencilState(&dsDesc, &_zWriteCloseState);
	if (FAILED(hr))
	{
		Log::LogE("Init Depth Stencil State 2 step failed!");
		return hr;
	}

	return hr;
}

HRESULT RenderPipeline::InitBlendState()
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

	if (FAILED(Device->CreateBlendState(&blendStateDescription, &_blendState)))
	{
		Log::LogE("Create 'Transparent' blend state failed!");
		return false;
	}

	return true;
}

HRESULT RenderPipeline::BuildRenderTextureToBackBuffer()
{

	DeviceContext->OMSetRenderTargets(1, &_backBufferRenderTargetView, nullptr);
	DeviceContext->ClearRenderTargetView(_backBufferRenderTargetView, DefualtColor);
	_renderTextureMeshRender->Render(true);
	Shader* shader = Shader::GetShader(RenderPipeline::GetIntance()->Device, "FX\\texture.fx");
	ID3DX11EffectTechnique * m_pTechnique = shader->GetTech("TextureTech");
	shader->GetResourceVariable("g_tex")->SetResource(RenderTagertTexture->GetShaderResourceView());
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		m_pTechnique->GetPassByIndex(i)->Apply(0, RenderPipeline::GetIntance()->DeviceContext);
		RenderPipeline::GetIntance()->DeviceContext->DrawIndexed(_renderTextureMesh->GetIndexCount(), 0, 0);
	}

	return true;
}