#pragma once
#include <iostream>
#include "SimpleMath.h"

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
	inline void SplitToVector3(char* source, char* split, Vector3& result);
private:
	void* _data;
};

#include "Loader.inl"