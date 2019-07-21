#include "Camera.h"
#include "RenderSetting.h"
#include "Scene.h"
#include <list>

Camera::Camera():nearZ(1),farZ(1000)
{

}

Camera::~Camera()
{
}

void Camera::Render()
{
	XMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH((XMVECTOR)_transform->GetPosition(), target, up);
	XMStoreFloat4x4(&m_view, V);
	XMMATRIX T = XMMatrixPerspectiveFovLH(XM_PIDIV4, RenderSetting::GetIntance()->m_width / static_cast<float>(RenderSetting::GetIntance()->m_height),
		nearZ, farZ);
	XMStoreFloat4x4(&m_proj, T);

	RenderOpacity();
	RenderTransparent();
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