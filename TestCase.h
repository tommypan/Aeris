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
	//光源和材质
	Sun*				_sun;
	PointLight						_pointLight;
	SpotLight						_spotLight;

	//绘制的几种几何图形
	Entity*		_grid;
	Entity*		_box;
	Entity*		_box2;
	Entity*		_sphere[5];
	Entity*		_cylinder[4];

	float							_theta;
	float							_phi;
	float							_radius;
	POINT							_lastMousePos;
};