#ifndef _LIGHT_DEMO_H_
#define _LIGHT_DEMO_H_

#include <list>
#include <vector>
#include <map>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"dxguid.lib")

class Entity;
class Camera;
class Scene
{
public:
	Scene();
	~Scene();


	void Render();
	void AddChild(Entity* child);
	void RemoveChild(Entity* child);
	void AddCamera(Camera* child);

	std::map<int, std::list<Entity*>>& GeSortedOpaqueChildren() { return _opaqueChildrenMap; };
	std::map<int, std::list<Entity*>>& GeSortedTransparentChildren() { return _transparentChildrenMap; };
private:
	void SortTransparentByCameraDist(Camera* targetCam);
public:
	static Scene* GetInstance()
	{
		static Scene instance;
		return &instance;
	}
private:
	static bool SortCamera( Camera*& c1,  Camera*&  c2);
private:
	std::map<int, std::list<Entity*>> _opaqueChildrenMap;
	std::map<int, std::list<Entity*>> _transparentChildrenMap;
	std::vector<Camera*> _cameras;
};

#endif//_LIGHT_DEMO_H_
