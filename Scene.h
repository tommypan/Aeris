#ifndef _LIGHT_DEMO_H_
#define _LIGHT_DEMO_H_

#include "RenderSetting.h"
#include "d3dx11effect.h"
#include "GeometryUtility.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include <list>
#include "Camera.h"


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"dxguid.lib")

class Entity;
class Scene
{
public:
	Scene();
	~Scene();

	bool LoadContent();
	void UnLoadContent();

	void Update(float dt);
	void Render();
	void AddChild(Entity* child);

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	 std::list<Entity*>& GeSortedOpacityChildren() { return _opacityChildren; };
	const std::list<Entity*>& GeSortedTransparentChildren() { return _transparentChildren; };
private:
	Camera * camera;
	//光源和材质
	DirectionalLight				m_dirLight;
	PointLight						m_pointLight;
	SpotLight						m_spotLight;

	//绘制的几种几何图形
	Entity*		m_grid;
	Entity*		m_box;
	Entity*		m_sphere[5];
	Entity*		m_cylinder[4];


	float							m_theta;
	float							m_phi;
	float							m_radius;
	POINT							m_lastMousePos;

public:
	static Scene* GetInstance()
	{
		static Scene instance;
		return &instance;
	}
private:
	//std::list<Entity*> roots;//有序链表,有序用于渲染
	std::list<Entity*> _opacityChildren;
	std::list<Entity*> _transparentChildren;
};

#endif//_LIGHT_DEMO_H_
