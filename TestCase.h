#pragma once
#include "GeometryUtility.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "Sun.h"

class TestCase
{
public:
	TestCase();
	~TestCase();
	bool LoadContent();
	void UnLoadContent();

	void Update(float dt);

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
private:
	void InitSun();
private:
	Camera * _camera;
	Camera * _camera2;
	//��Դ�Ͳ���
	Sun*				_sun;
	PointLight						_pointLights[4];
	SpotLight						_spotLight;

	//���Ƶļ��ּ���ͼ��
	Entity*		_grid;
	Entity*		_water;
	Entity*		_box[4];
	Entity*		_wall[4];

	float							_theta;
	float							_phi;
	float							_radius;
	POINT							_lastMousePos;
};