#include "Camera.h"
#include "RenderSetting.h"
#include "Scene.h"
#include <list>
#include <cmath>

#define CLAMP(x, upper, lower) (min(upper, max(x, lower)))

Camera::Camera():nearZ(1),farZ(1000), projection(CameraProjection::Perspective), minFov(1),maxFov(180), fov(30)
{

}

Camera::~Camera()
{
}

void Camera::Render()
{
	Vector3 target(0.0f, 0.0f, 0.0f);
	Vector3  up(0.0f, 1.0f, 0.0f);
	Matrix v= Matrix::CreateLookAt(_transform->GetPosition(), Vector3::Zero, Vector3::Up);
	//XMStoreFloat4x4(&m_view, v);
	_transform->SetRotation(Quaternion::CreateFromRotationMatrix(v));
	XMStoreFloat4x4(&m_view, _transform->GetWorldTransform().Invert());

	//XMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//XMMATRIX V = XMMatrixLookAtLH((XMVECTOR)_transform->GetPosition(), target, up);
	//XMStoreFloat4x4(&m_view, V);

	fov = CLAMP(fov,maxFov,minFov);
	XMMATRIX T = XMMatrixPerspectiveFovLH(fov/ maxFov*XM_PI, RenderSetting::GetIntance()->m_width / static_cast<float>(RenderSetting::GetIntance()->m_height),
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