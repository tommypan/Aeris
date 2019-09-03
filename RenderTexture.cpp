#include "RenderTexture.h"

RenderTexture::RenderTexture(ID3D11Device* d3dDevice, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTarget,RenderTextureType type, int TextureWidth, int TexureHeight):
	mShaderResourceView(nullptr), mDepthStencilTexture(nullptr), mDepthStencilView(nullptr)
{
	mDeviceContext = deviceContext;
	_renderType = type;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	//第二,填充深度缓存视图形容结构体,并创建深度缓存视图
	if (_renderType == RenderTextureType::RenderDepth)
	{

		textureDesc.Width = TextureWidth;
		textureDesc.Height = TexureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; //24位是为了深度缓存，8位是为了模板缓存
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;  //注意深度缓存(纹理)的绑定标志
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		d3dDevice->CreateTexture2D(&textureDesc, NULL, &mDepthStencilTexture);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		d3dDevice->CreateDepthStencilView(mDepthStencilTexture, &depthStencilViewDesc, &mDepthStencilView);
	}else if (_renderType == RenderTextureType::RenderTarget)
	{
		//第一,填充2D纹理形容结构体,并创建2D渲染纹理
		textureDesc.Width = TextureWidth;
		textureDesc.Height = TexureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  //纹理像素为12个字节
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		d3dDevice->CreateTexture2D(&textureDesc, NULL, &mDepthStencilTexture);

		//第二，填充渲染目标视图形容体,并进行创建目标渲染视图
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		d3dDevice->CreateRenderTargetView(mDepthStencilTexture, &renderTargetViewDesc, &renderTarget);
	}
	mRenderTargetView = renderTarget;


	//第三,填充着色器资源视图形容体,并进行创建着色器资源视图,注意这是用深度缓存(纹理)来创建的，而不是渲染目标缓存(纹理)创建的
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS; //此时因为是仅仅进行深度写，而不是颜色写，所以此时Shader资源格式跟深度缓存是一样的
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	d3dDevice->CreateShaderResourceView(mDepthStencilTexture, &shaderResourceViewDesc, &mShaderResourceView);

	//第四，设置视口的属性
	mViewPort.Width = (float)TextureWidth;
	mViewPort.Height = (float)TexureHeight;
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
	mViewPort.TopLeftX = 0.0f;
	mViewPort.TopLeftY = 0.0f;
}

RenderTexture::~RenderTexture()
{

}

void RenderTexture::SetRenderTarget(ID3D11RenderTargetView* renderTarget)
{
	if (mDeviceContext != nullptr)
	{
		mDeviceContext->OMSetRenderTargets(1, &renderTarget, mDepthStencilView);

		//设置视口
		mDeviceContext->RSSetViewports(1, &mViewPort);
	}
}

void RenderTexture::ClearRenderTarget(float * clearColor)
{
	if (mDeviceContext != nullptr)
	{
		if (_renderType == RenderTextureType::RenderDepth)
		{
			mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
		else if (_renderType == RenderTextureType::RenderTarget)
		{
			mDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColor);
		}
	}

}
