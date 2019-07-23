#pragma once
#include "Entity.h"

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
public:
	CameraProjection projection;
	float fov;
	float nearZ;
	float farZ;
	int depth;
	int cullMask;
private:
	Matrix	m_view;
	Matrix	m_proj;
	int minFov;
	int maxFov;
};