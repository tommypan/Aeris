#include "Camera.h"
#include "RenderSetting.h"
#include "Scene.h"
#include <list>
#include <cmath>

#define CLAMP(x, upper, lower) (min(upper, max(x, lower)))

Camera::Camera():nearZ(1),farZ(1000), projection(CameraProjection::Perspective), minFov(1),maxFov(180), fov(30)
{
	Scene::GetInstance()->RemoveChild(this);//效率考虑，没在AddChild去动态转换判断类型
	Scene::GetInstance()->AddCamera(this);
}

Camera::~Camera()
{
}

void Camera::Render()
{
	if (RenderSetting::GetIntance()->m_pImmediateContext == 0)
		return;
	//clear render target view
	float clearColor[4] = { 0.75f, 0.75f, 0.75f, 1.0f };
	RenderSetting::GetIntance()->m_pImmediateContext->ClearRenderTargetView(RenderSetting::GetIntance()->m_pRenderTargetView, clearColor);

	//clear depth/stencil view
	RenderSetting::GetIntance()->m_pImmediateContext->ClearDepthStencilView(RenderSetting::GetIntance()->m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	XMStoreFloat4x4(&m_view, _transform->GetWorldTransform().Invert());

	fov = CLAMP(fov,maxFov,minFov);
	XMMATRIX T = XMMatrixPerspectiveFovLH(fov/ maxFov*XM_PI, RenderSetting::GetIntance()->m_width / static_cast<float>(RenderSetting::GetIntance()->m_height),
		nearZ, farZ);
	XMStoreFloat4x4(&m_proj, T);

	RenderOpacity();
	RenderTransparent();

	RenderSetting::GetIntance()->m_pSwapChain->Present(0, 0);
}

void Camera::RenderOpacity()
{
	std::list<Entity*>& opacity = Scene::GetInstance()->GeSortedOpacityChildren();
	std::list<Entity*>::iterator startIt = opacity.begin();
	while (startIt != opacity.end())
	{
		if (cullMask >> (*startIt)->GetLayer() & 1)
		{
			(*startIt)->Render(m_view, m_proj);
		}
		startIt++;
	}
}

void Camera::RenderTransparent()
{

}