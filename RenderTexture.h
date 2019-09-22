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
	RenderTexture(ID3D11Device* d3dDevice, ID3D11DeviceContext* deviceContext,RenderTextureType type,int TextureWidth, int TexureHeight);
	~RenderTexture();

	void SetRenderTarget(int count,ID3D11RenderTargetView* renderTarget[]);
	void ClearRenderTarget(float * clearColor);
	inline ID3D11ShaderResourceView* GetShaderResourceView() {return _shaderResourceView;};
	inline ID3D11RenderTargetView* GetRenderTargetView() { return _renderTargetView; };
private:
	ID3D11ShaderResourceView* _shaderResourceView;  //Shader资源视图
	ID3D11Texture2D* _texture;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11RenderTargetView* _renderTargetView;
	D3D11_VIEWPORT _viewPort; //视口
	ID3D11DeviceContext* _deviceContext;
	RenderTextureType _renderType;
};