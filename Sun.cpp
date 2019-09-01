#include "Sun.h"

Sun::Sun()
{
	_camera.projection = CameraProjection::Orthongaphics;
	_camera.cullMask = 0xffffffff;
	_camera.depth = 0;// 0x7fffffff;
	_camera.clearFlag = CameraClearFlag::DontClear;
	_camera.GetTransform()->SetParent(_transform);
	//fov 应该根据场景或者具体应用场景去算一个合适的
}

Sun::~Sun()
{
}

void Sun::SetLightDirection(DirectX::Vector3& direction)
{
	light.direction = direction;
}