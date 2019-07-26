#ifndef _LIGHT_DEMO_H_
#define _LIGHT_DEMO_H_

#include <list>
#include <vector>


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

	 std::list<Entity*>& GeSortedOpacityChildren() { return _opacityChildren; };
	const std::list<Entity*>& GeSortedTransparentChildren() { return _transparentChildren; };

public:
	static Scene* GetInstance()
	{
		static Scene instance;
		return &instance;
	}
private:
	std::list<Entity*> _opacityChildren;
	std::list<Entity*> _transparentChildren;
	std::vector<Camera*> _cameras;
};

#endif//_LIGHT_DEMO_H_
