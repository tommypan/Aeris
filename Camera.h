#pragma once
#include "Entity.h"

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
	float fov;
	float nearZ;
	float farZ;
	int depth;
	int cullMask;
private:
	Matrix	m_view;
	Matrix	m_proj;
};