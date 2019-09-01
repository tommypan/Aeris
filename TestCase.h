#pragma once
#include "GeometryUtility.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"


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
	Camera * camera;
	Camera * camera2;
	//光源和材质
	DirectionalLight				m_dirLight;
	PointLight						m_pointLight;
	SpotLight						m_spotLight;

	//绘制的几种几何图形
	Entity*		m_grid;
	Entity*		m_box;
	Entity*		m_box2;
	Entity*		m_sphere[5];
	Entity*		m_cylinder[4];


	float							m_theta;
	float							m_phi;
	float							m_radius;
	POINT							m_lastMousePos;

};