#pragma once
#include <iostream>
#include "SimpleMath.h"
#include <vector>
#include "Mesh.h"

using namespace DirectX;


class Mesh;
class Loader
{
public:
	Loader();
	~Loader();

	bool Load(const std::string& path,Mesh * mesh);
public:
	
	static Loader* GetInstance()
	{
		static Loader loader;
		return &loader;
	}
private:
	inline void SplitToVector3(char* source, char* split, XMFLOAT3& result);
	inline void SplitToVector2(char* source, char* split, XMFLOAT2& result);
	inline void AddVertexToMesh(Vector3& index,Mesh* mesh);
private:
	void* _data;

	std::vector<XMFLOAT3> posv;
	std::vector<XMFLOAT2> texcoordv;
	std::vector<XMFLOAT3> normalv;
	std::vector<XMFLOAT3> btnv;
};

#include "Loader.inl"