#pragma once
#include <iostream>
#include "Transform.h"
#include "Macro.h"

enum Layer
{
	Default = 0,
	Water = 1,
	Effect = 2,
	UI = 3,
};

class Mesh;
class Material;
class Transform;
class MeshRender;
class Entity
{
public:
	Entity();
	Entity(Mesh* mesh);
	Entity(const std::string&  meshPath, const std::string& materialPath);
	~Entity();

	void Render(DirectX::CXMMATRIX view, DirectX::CXMMATRIX proj);
	inline Transform * GetTransform() { return _transform; };
	const std::string& GetName()const { return _name; };
	Material* GetMaterial() { return _material; };
	void SetLayer(Layer layer);
	int GetLayer() { return int(_layer); };
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
};

