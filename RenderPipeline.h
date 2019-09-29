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
	void PrepareRenderTarget(bool isDefer);
	void FinishRenderTarget(bool isDefer);
	void Present();
	void ShutDown();								//release
	void GetMRTRenderTarget(ID3D11RenderTargetView* result[]);
	void SetZWrite(bool enable);
	void SetAlphaBend(bool enable);

private:
	void ClearParam();
	HRESULT InitBackBuffer();
	HRESULT InitRenderTexture();
	HRESULT InitDepthStencilState();
	HRESULT InitBlendState();
	void BuildDeferShading();
	HRESULT BuildPostEffectToBackBuffer();
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
	RenderTexture* GBufferPosTexture;
	RenderTexture* GBufferNormalTexture;
	RenderTexture* GBufferColorTexture;//x,y,z是材质本身的颜色，代表漫反射和高光反射的颜色，w表示高光强度
	RenderTexture* DepthStencilTexture;
	RenderTexture* RenderTagertTexture;
	float DefualtColor[4];
private:
	IDXGISwapChain*			_swapChain;
	ID3D11RenderTargetView* _backBufferRenderTargetView;
	ID3D11DepthStencilState*_zWriteOpenState;
	ID3D11DepthStencilState*_zWriteCloseState;
	ID3D11BlendState* _blendState;

	Mesh* _deferShadingMesh;
	Material* _deferShadingMaterial;
	MeshRender* _deferShadingTextureMeshRender;

	Mesh* _finalMesh;
	Material* _finalMaterial;
	MeshRender* _finalTextureMeshRender;
};

#endif//_DX11DEMOBASE_H_
