#include "MeshRender.h"
#include "Macro.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderPipeline.h"
#include "Shader.h"

struct VertexStruct
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 texcoord;
};


MeshRender::MeshRender()
{
}

MeshRender::MeshRender(Mesh* mesh, Material* material)
{
	_mesh = mesh;
	_material = material;

	if (_mesh == nullptr || material == nullptr || _mesh->Vertices.size() == 0)
	{
		return;
	}

	std::vector<VertexStruct> vertices(_mesh->Vertices.size());

	for (UINT i = 0; i < _mesh->Vertices.size(); ++i)
	{
		vertices[i].pos = _mesh->Vertices[i].pos;
		vertices[i].normal = _mesh->Vertices[i].pos;
		vertices[i].texcoord = _mesh->Vertices[i].coord;
	}

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexStruct)* _mesh->Vertices.size();
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = &vertices[0];
	RenderPipeline::GetIntance()->Device->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);

	std::vector<UINT> indices(_mesh->Indices.size());
	for (UINT i = 0; i < _mesh->Indices.size(); ++i)
	{
		indices[i] = _mesh->Indices[i];
	}
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof(UINT)* _mesh->Indices.size();

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = &indices[0];
	RenderPipeline::GetIntance()->Device->CreateBuffer(&indexDesc, &indexData, &m_pIndexBuffer);

	//define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3DX11EffectTechnique * m_pTechnique = _material->GetShader()->GetTech("LightTech");
	UINT numLayoutElements = ARRAYSIZE(layout);
	D3DX11_PASS_DESC passDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	//create the input layout
	HRESULT hr = RenderPipeline::GetIntance()->Device->CreateInputLayout(layout, numLayoutElements, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout);
}

MeshRender::~MeshRender()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pInputLayout);
	_mesh = nullptr;
	_material = nullptr;
}

void MeshRender::Render(bool useCutstomMat)
{
	BindBuffer();

	if (!useCutstomMat)
	{
		_material->Render(_mesh);
	}
}

void MeshRender::RenderShadowMap()
{
	BindBuffer();
	_material->RenderShadowMap(_mesh);
}

void MeshRender::BindBuffer()
{
	//set vertex buffer
	UINT stride = sizeof(VertexStruct);
	UINT offset = 0;
	RenderPipeline::GetIntance()->DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//set primitive topology
	RenderPipeline::GetIntance()->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//set index buffer
	RenderPipeline::GetIntance()->DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//set input layout
	RenderPipeline::GetIntance()->DeviceContext->IASetInputLayout(m_pInputLayout);
}