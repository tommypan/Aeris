#include "Sun.h"

Sun::Sun()
{
	_camera.Projection = CameraProjection::Orthongaphics;
	_camera.CullMask = 0x7fffffff;
	_camera.SetDepth(-1);// 0x7fffffff; 层级设置确保优先渲染
	_camera.ClearFlag = CameraClearFlag::DontClear;
	_camera.GetTransform()->SetParent(_transform);
	//fov 应该根据场景或者具体应用场景去算一个合适的
}

Sun::~Sun()
{
}

void Sun::SetLightDirection(DirectX::Vector3& direction)
{
	Light.direction = direction;
}