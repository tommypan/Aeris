#pragma once
#include <iostream>
#include "Transform.h"
#include "Macro.h"
#include "Material.h"

enum Layer
{
	Default = 0,
	Water = 1,
	Effect = 2,
	UI = 3,
};

class Mesh;
class Transform;
class MeshRender;
class Entity
{
public:
	Entity();
	Entity(Mesh* mesh);
	Entity(const std::string&  meshPath, const std::string& materialPath);
	Entity(const std::string&  meshPath);
	~Entity();

	void Render(DirectX::CXMMATRIX view, DirectX::CXMMATRIX proj);
	inline Transform * GetTransform() { return _transform; };
	const std::string& GetName()const { return _name; };
	Material* GetMaterial()const { return _material; };
	inline int GetRenderQueue() { return _material != nullptr ? _material->RenderQueue : 0; };
	inline void SetLayer(Layer layer) { _layer = layer; };
	inline int GetLayer() { return int(_layer); };
	inline void SetDistToCam(const float& dist) { _distToCurCamera = dist; };
	bool operator<(Entity*compareE);
protected:
	void Clear();
protected:
	std::string _name;
	Transform * _transform;
	Mesh * _mesh;
	Material * _material;
	MeshRender* _meshRender;
	DirectX::XMFLOAT4X4 _world;
	Layer _layer;
	float _distToCurCamera;
};

