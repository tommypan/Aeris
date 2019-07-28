#include "Scene.h"
#include "RenderSetting.h"
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
		if ((*cameraIt)->zTest)
		{
			SortTransparentByCameraDist((*cameraIt));
		}
		(*cameraIt)->Render();
		cameraIt++;
	}
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
		std::list<Entity*>&  opacityChildren = _opacityChildrenMap[childRenderQueue];
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
		std::list<Entity*>&  opacityChildren = _opacityChildrenMap[childRenderQueue];
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
	std::sort(_cameras.begin(), _cameras.end(), &Scene::SortCamera);
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
			if (targetCam->cullMask >> (*startIt)->GetLayer() & 1)
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
	return c1->depth < c2->depth;
}