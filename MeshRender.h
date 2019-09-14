#pragma once
#include "Macro.h"

class Mesh;
class Material;

class MeshRender
{
public:
	MeshRender();
	MeshRender(Mesh* mesh, Material* material);
	~MeshRender();

	void Render(bool useCutstomMat = false);
	void RenderShadowMap();
private:
	Mesh * _mesh;
	Material * _material;

	ID3D11Buffer*					m_pVertexBuffer;
	ID3D11Buffer*					m_pIndexBuffer;
	ID3D11InputLayout*				m_pInputLayout;
};