#include "Sun.h"

Sun::Sun()
{
	_camera.projection = CameraProjection::Orthongaphics;
	_camera.cullMask = 0xffffffff;
	_camera.depth = 0;// 0x7fffffff;
	_camera.clearFlag = CameraClearFlag::DontClear;
	_camera.GetTransform()->SetParent(_transform);
	//fov Ӧ�ø��ݳ������߾���Ӧ�ó���ȥ��һ�����ʵ�
}

Sun::~Sun()
{
}

void Sun::SetLightDirection(DirectX::Vector3& direction)
{
	light.direction = direction;
}