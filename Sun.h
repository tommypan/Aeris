#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Light.h"

class Sun : Entity//ƽ�й�ֻ�з��򣬵���Ҫ������Ӱ���������λ�þ������ȥ��Ⱦshadow map
{
public:
	Sun();
	~Sun();

	void SetLightDirection(DirectX::Vector3& direction);
public:
	DirectionalLight Light;//ֻ�з���
private:
	Camera _camera;
};