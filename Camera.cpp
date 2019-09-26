#include "Camera.h"
#include "RenderPipeline.h"
#include "Scene.h"
#include <cmath>
#include "RenderTexture.h"

#define CLAMP(x, upper, lower) (min(upper, max(x, lower)))

Camera::Camera(bool isShadowCamera) :NearZ(1), FarZ(1000), Projection(CameraProjection::Perspective), _minFov(1), _maxFov(180), Fov(30),_depth(0), ZTest(true), ClearFlag(0), _renderPath(CameraRenderPath::Forward)
{
	ZeroMemory(this,sizeof(this));
	_isShadowCamera = isShadowCamera;
	memcpy(ClearColor,RenderPipeline::GetIntance()->DefualtColor,sizeof(ClearColor));
	Scene::GetInstance()->RemoveChild(this);//效率考虑，没在AddChild去动态转换判断类型
	Scene::GetInstance()->AddCamera(this);
}

Camera::~Camera()
{
}

void Camera::Render()
{
	if (RenderPipeline::GetIntance()->DeviceContext == 0)
		return;

	if (_isShadowCamera)
	{
		RenderPipeline::GetIntance()->GenShadowMap();
	}
	else
	{
		RenderPipeline::GetIntance()->PrepareRenderTarget(_renderPath != CameraRenderPath::Forward);
	}

	if (ClearFlag == CameraClearFlag::SolidColor)
	{
		RenderPipeline::GetIntance()->RenderTagertTexture->ClearRenderTarget(ClearColor);
		RenderPipeline::GetIntance()->DeferColorTexture->ClearRenderTarget(ClearColor);
		RenderPipeline::GetIntance()->DeferNormalTexture->ClearRenderTarget(ClearColor);
		RenderPipeline::GetIntance()->DeferPosTexture->ClearRenderTarget(ClearColor);
		RenderPipeline::GetIntance()->DepthStencilTexture->ClearRenderTarget(ClearColor);
	}
	else if (ClearFlag == CameraClearFlag::DepthOnly)
	{
		RenderPipeline::GetIntance()->DepthStencilTexture->ClearRenderTarget(ClearColor);
	}


	XMStoreFloat4x4(&_view, _transform->GetWorldTransform().Invert());

	if (Projection == CameraProjection::Orthongaphics)
	{
		XMMATRIX T = XMMatrixOrthographicLH(RenderPipeline::GetIntance()->Width, RenderPipeline::GetIntance()->Height, NearZ, FarZ);
		XMStoreFloat4x4(&_proj, T);
	}
	else
	{
		Fov = CLAMP(Fov, _maxFov, _minFov);
		XMMATRIX T = XMMatrixPerspectiveFovLH(Fov / _maxFov*XM_PI, RenderPipeline::GetIntance()->AspectRatio(), NearZ, FarZ);
		XMStoreFloat4x4(&_proj, T);
	}


	RenderOpaque();
	RenderTransparent();

	if (!_isShadowCamera)
	{
		RenderPipeline::GetIntance()->FinishRenderTarget(_renderPath != CameraRenderPath::Forward);
	}
}

void Camera::SetDepth(int depth)
{
	_depth = depth;
	Scene::GetInstance()->SortCameras();
}

void Camera::SetRenderPath(CameraRenderPath renderPath)
{ 
	_renderPath = renderPath;
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
			if (CullMask >> (*startIt)->GetLayer() & 1)
			{
				if (_isShadowCamera && (*startIt)->GetMaterial()->CastShaow == false)
				{
					startIt++;
					continue;
				}
				
				if (!(*startIt)->GetRenderable())
				{
					startIt++;
					continue;
				}

				if (!_isShadowCamera)
				{
					InnnerGenShadow(*startIt);
				}
				(*startIt)->Render(_view, _proj, _isShadowCamera,_renderPath != CameraRenderPath::Forward);
			}
			startIt++;
		}

		startMapIt++;
	}
}

void Camera::InnnerGenShadow(Entity* entity)
{
	XMMATRIX world = XMLoadFloat4x4(&(entity->GetTransform()->GetWorldTransform()));
	XMMATRIX view = Scene::GetInstance()->GetShadowCameraView();
	XMMATRIX proj = Scene::GetInstance()->GetShadowCameraProj();
	entity->GetMaterial()->SetCustomMatrix("gCustomMVP", world*view*proj);
}