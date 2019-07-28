#pragma once
#include "Entity.h"
#include <list>
#include <map>

enum CameraProjection
{
	Perspective = 0,
	Orthongaphics = 1,
};
class Camera : public Entity
{
public:
	Camera();
	~Camera();

	void Render();

private:
	void RenderOpacity();
	void RenderTransparent();
	void InnerRenderEntitys(std::map<int, std::list<Entity*>>& opacityMap);
public:
	CameraProjection projection;
	float fov;
	float nearZ;
	float farZ;
	int depth;
	int cullMask;
	bool zTest;
private:
	Matrix	m_view;
	Matrix	m_proj;
	int minFov;
	int maxFov;
};