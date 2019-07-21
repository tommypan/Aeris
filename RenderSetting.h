#ifndef _DX11DEMOBASE_H_
#define _DX11DEMOBASE_H_

#include <string>
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>//don't use xnamath.h
#include <directxcolors.h>

using namespace DirectX;

class RenderSetting
{
public:
	RenderSetting();
	virtual ~RenderSetting();

	std::wstring  m_mainWndCaption;					//title
	float AspectRatio() const;						//width/height

	bool InitDirect3D(HINSTANCE hInstance, HWND hWnd);

	void ShutDown();								//release

public:
	static RenderSetting* GetIntance()
	{
		static RenderSetting setting;
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
	ID3D11DepthStencilView* m_pDepthStencilView;
};

#endif//_DX11DEMOBASE_H_
