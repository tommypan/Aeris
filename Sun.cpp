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

	Light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	Light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Light.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	Light.direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	//fov 应该根据场景或者具体应用场景去算一个合适的
}

Sun::~Sun()
{
}

void Sun::SetLightDirection(DirectX::Vector3& direction)
{
	Light.direction = direction;
}