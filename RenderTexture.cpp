#include "RenderTexture.h"

RenderTexture::RenderTexture(ID3D11Device* d3dDevice, ID3D11DeviceContext* deviceContext, RenderTextureType type, int TextureWidth, int TexureHeight):
	_shaderResourceView(nullptr), _texture(nullptr), _depthStencilView(nullptr), _renderTargetView(nullptr)
{
	_deviceContext = deviceContext;
	_renderType = type;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Format = _renderType == RenderTextureType::RenderDepth ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.BindFlags = _renderType == RenderTextureType::RenderDepth ? (D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE) : D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Width = TextureWidth;
	textureDesc.Height = TexureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	d3dDevice->CreateTexture2D(&textureDesc, NULL, &_texture);

	if (_renderType == RenderTextureType::RenderDepth)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		d3dDevice->CreateDepthStencilView(_texture, &depthStencilViewDesc, &_depthStencilView);
	}
	else if (_renderType == RenderTextureType::RenderTarget)
	{
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		d3dDevice->CreateRenderTargetView(_texture, &renderTargetViewDesc, &_renderTargetView);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = _renderType == RenderTextureType::RenderDepth ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	d3dDevice->CreateShaderResourceView(_texture, &shaderResourceViewDesc, &_shaderResourceView);

	_viewPort.Width = (float)TextureWidth;
	_viewPort.Height = (float)TexureHeight;
	_viewPort.MinDepth = 0.0f;
	_viewPort.MaxDepth = 1.0f;
	_viewPort.TopLeftX = 0.0f;
	_viewPort.TopLeftY = 0.0f;
}

RenderTexture::~RenderTexture()
{

}

void RenderTexture::SetRenderTarget(ID3D11RenderTargetView* renderTargetView)
{
	_deviceContext->OMSetRenderTargets(1, &renderTargetView, _depthStencilView);
	//ÉèÖÃÊÓ¿Ú
	_deviceContext->RSSetViewports(1, &_viewPort);
}

void RenderTexture::ClearRenderTarget(float * clearColor)
{
	if (_deviceContext != nullptr)
	{
		if (_renderType == RenderTextureType::RenderDepth)
		{
			_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
		else if (_renderType == RenderTextureType::RenderTarget)
		{
			_deviceContext->ClearRenderTargetView(_renderTargetView, clearColor);
		}
	}
}
