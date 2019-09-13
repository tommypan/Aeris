#include "Sun.h"

Sun::Sun()
{
	_camera.Projection = CameraProjection::Orthongaphics;
	_camera.CullMask = 0x7fffffff;
	_camera.SetDepth(-1);// 0x7fffffff; �㼶����ȷ��������Ⱦ
	_camera.ClearFlag = CameraClearFlag::DontClear;
	_camera.GetTransform()->SetParent(_transform);
	//fov Ӧ�ø��ݳ������߾���Ӧ�ó���ȥ��һ�����ʵ�
}

Sun::~Sun()
{
}

void Sun::SetLightDirection(DirectX::Vector3& direction)
{
	Light.direction = direction;
}