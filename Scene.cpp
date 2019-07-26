#include "Scene.h"
#include "RenderSetting.h"
#include "Entity.h"
#include "Camera.h"

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
		(*cameraIt)->Render();
		cameraIt++;
	}
}

void Scene::AddChild(Entity* child)
{
	_opacityChildren.push_back(child);//todo≈≈–Ú
}

void Scene::RemoveChild(Entity* child)
{
	_opacityChildren.remove(child);
}

void Scene::AddCamera(Camera* child)
{
	_cameras.push_back(child);
}