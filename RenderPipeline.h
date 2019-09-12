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
class Texture;
class RenderTexture;
class RenderPipeline
{
public:
	RenderPipeline();
	virtual ~RenderPipeline();

	std::wstring  m_mainWndCaption;					//title
	float AspectRatio() const;						//width/height

	bool InitDirect3D(HINSTANCE hInstance, HWND hWnd);
	void BuildShadowMap();
	void Prepare();
	void Present();
	void ShutDown();								//release

	void SetZWrite(bool enable);
	void SetAlphaBend(bool enable);

private:
	bool InitRenderTexture();
	bool InitDepthStencilState();
	bool InitBlendState();
	bool BuildRenderTextureToBackBuffer();
public:
	static RenderPipeline* GetIntance()
	{
		static RenderPipeline setting;
		return &setting;
	};
public:
	UINT					m_width;				//window width
	UINT					m_height;				//window height
	HINSTANCE				m_hInstance;			
	HWND					m_hWnd;					
	D3D_DRIVER_TYPE			m_driverType;			 
	D3D_FEATURE_LEVEL		m_featureLevel;			
	ID3D11Device*			m_pd3dDevice;			
	ID3D11DeviceContext*	m_pImmediateContext;	
	IDXGISwapChain*			m_pSwapChain;			
	ID3D11RenderTargetView* m_pRenderTargetView;	
	ID3D11Texture2D*        m_pDepthStencilBuffer;	
	//ID3D11DepthStencilView* m_pDepthStencilView;
	RenderTexture* m_DepthStencilTexture;
	RenderTexture* m_RenderTagertTexture;
	ID3D11DepthStencilState*m_pZWriteOpenState;
	ID3D11DepthStencilState*m_pZWriteCloseState;
	ID3D11BlendState* m_pBlendState;
private:
	Mesh* _renderTextureMesh;
	Material* _renderTextureMaterial;
	MeshRender* _renderTextureMeshRender;
	Texture* _renderTexture;
};

#endif//_DX11DEMOBASE_H_
