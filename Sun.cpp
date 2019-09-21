#include "Sun.h"

Sun::Sun()
{
	_camera = new Camera(true);
	_camera->Projection = CameraProjection::Perspective;
	_camera->CullMask = 0xffffffff;//render all layer // 0x7fffffff render nothing;
	_camera->SetDepth(-1);// 0x7fffffff; 
	_camera->ClearFlag = CameraClearFlag::DontClear;
	_camera->GetTransform()->SetParent(_transform);
	_camera->SetNeedGenShadow(true);
	this->Name = "Sun";
	//fov Ӧ�ø��ݳ������߾���Ӧ�ó���ȥ��һ�����ʵ�
}

Sun::~Sun()
{
}

void Sun::SetLightDirection(DirectX::Vector3& direction)
{
	Light.direction = direction;
}