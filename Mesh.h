#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

class Vertex
{
public:
	Vertex() { ZeroMemory(this, sizeof(this)); };
	Vertex(XMFLOAT3 pos_, XMFLOAT3 normal_, XMFLOAT3 tangent_, XMFLOAT2 coord_) {};
	~Vertex() {};
	Vertex(const Vertex& cp)
	{
		pos = cp.pos;
		normal = cp.normal;
		tangent = cp.tangent;
		coord = cp.coord;
	};

public:
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT2 coord;
};


class Mesh
{
public:
	Mesh();
	Mesh(const Mesh& cp);
	void operator=(const Mesh& cp);
	Mesh(const std::string& path);
	~Mesh();

	UINT GetIndexCount();
	inline bool IsValide() { return _valide; };
public:
	static Mesh* GetMesh(const std::string& path);
	static void ReleaseAllMeshData();
public:
	std::vector<Vertex> Vertices;
	std::vector<UINT> Indices;
private:
	bool _valide = false;
private:
	static std::unordered_map<std::string, Mesh*> _meshDatas;
};