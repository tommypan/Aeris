#include "Mesh.h"
#include "Macro.h"

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
	vertices.assign(cp.vertices.begin(), cp.vertices.end());
	indices.assign(cp.indices.begin(), cp.indices.end());
}

Mesh::Mesh(const std::string& path)
{
	_valide = true;
}


Mesh::~Mesh()
{

}

UINT Mesh::GetIndexCount()
{
	return indices.size();
}

Mesh* Mesh::GetMesh(const std::string& path)
{
	std::unordered_map<std::string, Mesh*>::const_iterator iter = _meshDatas.find(path);
	if (iter == _meshDatas.end())
	{
		Mesh * meshData = new Mesh(path);
		//todo check valid
		_meshDatas[path] = meshData;//�����ڹ��캯�����棬�ⲿ����ʵ������shader���Լ�delete����������ӹ���
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