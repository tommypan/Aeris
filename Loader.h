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
	void SplitToVector3(char* source, char* split,Vector3& result);
	static Loader* GetInstance()
	{
		static Loader loader;
		return &loader;
	}

private:
	void* _data;
};