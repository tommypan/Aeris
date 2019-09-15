#ifndef _DX11DEMOBASE_H_
#define _DX11DEMOBASE_H_

#include <string>
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>//don't use xnamath.h
#include <directxcolors.h>

using namespace DirectX;

class Mesh;
class Material;
class MeshRender;
class Texture2D;
class RenderTexture;
class RenderPipeline
{
public:
	RenderPipeline();
	virtual ~RenderPipeline();

	std::wstring  m_mainWndCaption;					//title
	float AspectRatio() const;						//width/height

	bool InitDirect3D(HINSTANCE hInstance, HWND hWnd);
	void GenShadowMap();
	void PrepareRenderTarget();
	void Present();
	void ShutDown();								//release

	void SetZWrite(bool enable);
	void SetAlphaBend(bool enable);

private:
	void ClearParam();
	HRESULT InitBackBuffer();
	HRESULT InitRenderTexture();
	HRESULT InitDepthStencilState();
	HRESULT InitBlendState();
	HRESULT BuildRenderTextureToBackBuffer();
public:
	static RenderPipeline* GetIntance()
	{
		static RenderPipeline setting;
		return &setting;
	};
public:
	UINT					Width;				//window width
	UINT					Height;				//window height
	HINSTANCE				HInstance;			
	HWND					HWnd;					
	D3D_DRIVER_TYPE			DriverType;			 
	D3D_FEATURE_LEVEL		FeatureLevel;			
	ID3D11Device*			Device;			
	ID3D11DeviceContext*	DeviceContext;	
	RenderTexture* ShadowDepthTexture;
	RenderTexture* DepthStencilTexture;
	RenderTexture* RenderTagertTexture;
	float DefualtColor[4];
private:
	IDXGISwapChain*			_swapChain;
	ID3D11RenderTargetView* _backBufferRenderTargetView;
	ID3D11DepthStencilState*_zWriteOpenState;
	ID3D11DepthStencilState*_zWriteCloseState;
	ID3D11BlendState* _blendState;

	Mesh* _finalMesh;
	Material* _finalMaterial;
	MeshRender* _finalTextureMeshRender;
	Texture2D* _renderTexture;
};

#endif//_DX11DEMOBASE_H_
