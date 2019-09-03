#pragma once
#include "Macro.h"

enum RenderTextureType
{
	RenderTarget = 0,
	RenderDepth = 1,
};

class RenderTexture
{
public:
	RenderTexture(ID3D11Device* d3dDevice, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTarget, RenderTextureType type,int TextureWidth, int TexureHeight);
	~RenderTexture();

	void SetRenderTarget(ID3D11RenderTargetView* renderTarget);
	void ClearRenderTarget(float * clearColor);
	inline ID3D11ShaderResourceView* GetShaderResourceView() {return mShaderResourceView;};

private:
	ID3D11ShaderResourceView* mShaderResourceView;  //Shader��Դ��ͼ
	ID3D11Texture2D* mDepthStencilTexture;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RenderTargetView* mRenderTargetView;
	D3D11_VIEWPORT mViewPort; //�ӿ�
	ID3D11DeviceContext* mDeviceContext;
	RenderTextureType _renderType;
};