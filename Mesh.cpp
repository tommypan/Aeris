#include "Mesh.h"
#include "Macro.h"
#include "Loader.h"

std::unordered_map<std::string, Mesh*> Mesh::_meshDatas;

Mesh::Mesh()
{

}

Mesh::Mesh(const Mesh& cp)
{
	*this = cp;
}

void Mesh::operator=(const Mesh& cp)
{
	Vertices.assign(cp.Vertices.begin(), cp.Vertices.end());
	Indices.assign(cp.Indices.begin(), cp.Indices.end());
}

Mesh::Mesh(const std::string& path)
{
	Loader::GetInstance()->Load(path, this);
	_valide = true;
}


Mesh::~Mesh()
{

}

UINT Mesh::GetIndexCount()
{
	return Indices.size();
}

Mesh* Mesh::GetMesh(const std::string& path)
{
	std::unordered_map<std::string, Mesh*>::const_iterator iter = _meshDatas.find(path);
	if (iter == _meshDatas.end())
	{
		Mesh * meshData = new Mesh(path);
		//todo check valid
		_meshDatas[path] = meshData;//不放在构造函数里面，外部主动实例化的shader，自己delete，不划入池子管理
		return meshData;
	}
	return iter->second;
}

void Mesh::ReleaseAllMeshData()
{
	std::unordered_map<std::string, Mesh*>::const_iterator iter = _meshDatas.begin();
	while (iter != _meshDatas.end())
	{
		SAFE_DELETE((Mesh*)iter->second);
		iter++;
	}

	_meshDatas.clear();
}