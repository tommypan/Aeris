#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Light.h"

class Sun : Entity//平行光只有方向，但是要产生阴影，必须得有位置决定相机去渲染shadow map
{
public:
	Sun();
	~Sun();

	void SetLightDirection(DirectX::Vector3& direction);
public:
	DirectionalLight Light;//只有方向
private:
	Camera _camera;
};