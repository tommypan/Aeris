#include "Camera.h"
#include "RenderPipeline.h"
#include "Scene.h"
#include <cmath>
#include "RenderTexture.h"

#define CLAMP(x, upper, lower) (min(upper, max(x, lower)))

Camera::Camera() :nearZ(1), farZ(1000), projection(CameraProjection::Perspective), minFov(1), maxFov(180), fov(30),_depth(0), zTest(true), clearFlag(0), clearColor{ 0.75f, 0.75f, 0.75f, 1.0f }
{
	Scene::GetInstance()->RemoveChild(this);//效率考虑，没在AddChild去动态转换判断类型
	Scene::GetInstance()->AddCamera(this);
}

Camera::~Camera()
{
}

void Camera::Render()
{
	if (RenderPipeline::GetIntance()->m_pImmediateContext == 0)
		return;

	if (clearFlag == CameraClearFlag::SolidColor)
	{
		RenderPipeline::GetIntance()->m_RenderTagertTexture->ClearRenderTarget(clearColor);
		RenderPipeline::GetIntance()->m_DepthStencilTexture->ClearRenderTarget(clearColor);
	}
	else if (clearFlag == CameraClearFlag::DepthOnly)
	{
		RenderPipeline::GetIntance()->m_DepthStencilTexture->ClearRenderTarget(clearColor);
	}


	XMStoreFloat4x4(&m_view, _transform->GetWorldTransform().Invert());

	fov = CLAMP(fov,maxFov,minFov);
	XMMATRIX T = XMMatrixPerspectiveFovLH(fov/ maxFov*XM_PI, RenderPipeline::GetIntance()->AspectRatio(),nearZ, farZ);
	XMStoreFloat4x4(&m_proj, T);

	RenderOpaque();
	RenderTransparent();
}

void Camera::SetDepth(int depth)
{
	_depth = depth;
	Scene::GetInstance()->SortCameras();
}

void Camera::RenderOpaque()
{
	std::map<int, std::list<Entity*>>& renderList = Scene::GetInstance()->GeSortedOpaqueChildren();
	if (renderList.size() > 0)
	{
		RenderPipeline::GetIntance()->SetZWrite(true);
		RenderPipeline::GetIntance()->SetAlphaBend(false);
		InnerRenderEntitys(renderList);
	}
}

void Camera::RenderTransparent()
{
	std::map<int, std::list<Entity*>>& renderList = Scene::GetInstance()->GeSortedTransparentChildren();
	if (renderList.size() > 0)
	{
		RenderPipeline::GetIntance()->SetZWrite(false);
		RenderPipeline::GetIntance()->SetAlphaBend(true);
		InnerRenderEntitys(renderList);
	}

}

void Camera::InnerRenderEntitys(std::map<int, std::list<Entity*>>& entitysMap)
{
	std::map<int, std::list<Entity*>>::iterator startMapIt = entitysMap.begin();
	while (startMapIt != entitysMap.end())
	{
		std::list<Entity*>::iterator  startIt = (*startMapIt).second.begin();
		while (startIt != (*startMapIt).second.end())
		{
			if (cullMask >> (*startIt)->GetLayer() & 1)
			{
				(*startIt)->Render(m_view, m_proj);
			}
			startIt++;
		}

		startMapIt++;
	}
}