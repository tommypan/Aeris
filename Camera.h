#pragma once
#include "Entity.h"
#include <list>
#include <map>

enum CameraProjection
{
	Perspective = 0,
	Orthongaphics = 1,
};

enum CameraClearFlag
{
	SolidColor = 0,
	DepthOnly = 1,
	DontClear = 2,
};

class Camera : public Entity
{
public:
	Camera(bool isShadowCamera = false);
	~Camera();

	void Render();
	void SetDepth(int depth);
	inline int Depth() { return _depth; };
	inline Matrix& GetViewMatrix(){ return _view; };
	inline Matrix& GetProjectMatrix() { return _proj; };
	inline bool IsShadowCamera() { return _isShadowCamera; };
private:
	void RenderOpaque();
	void RenderTransparent();
	void InnerRenderEntitys(std::map<int, std::list<Entity*>>& opacityMap);
	void InnnerGenShadow(Entity* entity);
public:
	CameraProjection Projection;
	float Fov;
	float NearZ;
	float FarZ;
	int CullMask;
	bool ZTest;
	int ClearFlag;
	float ClearColor[4];
private:
	bool _isShadowCamera;
	Matrix	_view;
	Matrix	_proj;
	int _minFov;
	int _maxFov;
	int _depth;
};