#include "Scene.h"
#include "RenderPipeline.h"
#include "Entity.h"
#include "Camera.h"
#include<algorithm>
#include "Material.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Render()
{
	std::vector<Camera*>::iterator cameraIt = _cameras.begin();
	while (cameraIt != _cameras.end())
	{
		if ((*cameraIt)->ZTest)
		{
			SortTransparentByCameraDist((*cameraIt));
		}
		if ((*cameraIt)->IsShadowCamera())
		{
			if (!(*cameraIt)->NeedGenShadow())
			{
				cameraIt++;
				continue;
			}
			(*cameraIt)->SetNeedGenShadow(false);
		}

		(*cameraIt)->Render();
		cameraIt++;
	}
	RenderPipeline::GetIntance()->Present();
}

void Scene::AddChild(Entity* child)
{
	if (child == nullptr)
	{
		return;
	}

	int childRenderQueue = child->GetRenderQueue();
	if (child->GetRenderQueue() < (int)RenderQueue::Transperent)
	{
		std::list<Entity*>&  opacityChildren = _opaqueChildrenMap[childRenderQueue];
		opacityChildren.push_back(child);
	}
	else
	{
		std::list<Entity*>&  transperantChildren = _transparentChildrenMap[childRenderQueue];
		transperantChildren.push_back(child);
	}
}

void Scene::RemoveChild(Entity* child)
{
	if (child == nullptr)
	{
		return;
	}
	int childRenderQueue = child->GetRenderQueue();
	if (child->GetRenderQueue() < (int)RenderQueue::Transperent)
	{
		std::list<Entity*>&  opacityChildren = _opaqueChildrenMap[childRenderQueue];
		opacityChildren.remove(child);
	}
	else
	{
		std::list<Entity*>&  transperantChildren = _transparentChildrenMap[childRenderQueue];
		transperantChildren.remove(child);
	}
}

void Scene::AddCamera(Camera* child)
{
	_cameras.push_back(child);
	SortCameras();
}

void Scene::SortCameras()
{
	std::sort(_cameras.begin(), _cameras.end(), &Scene::SortCamera);
}

Matrix Scene::GetShadowCameraView()
{
	std::vector<Camera*>::iterator cameraIt = _cameras.begin();
	while (cameraIt != _cameras.end())
	{
		if ((*cameraIt)->IsShadowCamera())
		{
			return (*cameraIt)->GetViewMatrix();
		}
	}
	return Matrix::Identity;
}
Matrix Scene::GetShadowCameraProj()
{
	std::vector<Camera*>::iterator cameraIt = _cameras.begin();
	while (cameraIt != _cameras.end())
	{
		if ((*cameraIt)->IsShadowCamera())
		{
			return (*cameraIt)->GetProjectMatrix();
		}
	}
	return Matrix::Identity;
}

void Scene::SortTransparentByCameraDist(Camera* targetCam)
{
	//set dist
	Vector3 cameraPos = targetCam->GetTransform()->GetPosition();
	std::map<int, std::list<Entity*>>::iterator startMapIt = _transparentChildrenMap.begin();
	while (startMapIt != _transparentChildrenMap.end())
	{
		std::list<Entity*>::iterator  startIt = (*startMapIt).second.begin();
		while (startIt != (*startMapIt).second.end())
		{
			if (targetCam->CullMask >> (*startIt)->GetLayer() & 1)
			{
				(*startIt)->SetDistToCam(((*startIt)->GetTransform()->GetPosition() - cameraPos).Length());
			}
			startIt++;
		}

		startMapIt++;
	}

	//sort
	startMapIt = _transparentChildrenMap.begin();
	while (startMapIt != _transparentChildrenMap.end())
	{
		startMapIt->second.sort();
		startMapIt++;
	}
}

bool Scene::SortCamera( Camera*& c1, Camera*& c2)
{
	return c1->Depth() < c2->Depth();
}