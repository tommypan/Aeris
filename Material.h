#pragma once
#include <iostream>
#include <unordered_map>
#include "Macro.h"

using namespace DirectX;

enum RenderQueue
{
	Geometry = 2000,
	AlphaTest = 3000,
	Transperent = 4000,
};

struct MaterialUniform
{
	MaterialUniform(const XMFLOAT4& ambient_,const XMFLOAT4& diffuse_, const XMFLOAT4& specular_,const XMFLOAT4& reflect_)
	{
		ambient = ambient_;
		diffuse = diffuse_;
		specular = specular_;
		reflect = reflect_;
	}

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;//w表示高光强度
	XMFLOAT4 reflect;

};

class Texture;
class Shader;
class Mesh;
class Material
{
	friend class Entity;
public:
	Material();
	Material(const Material& cp);
	void operator=(const Material& cp);
	Material(const std::string& path);
	~Material();

	void Render(Mesh* mesh);
	void SetTxture(const std::string& name);
	void SetShader(const std::string& name);

	void SetVariable();

	const Shader* GetShader() { return shader; };
private :
	inline MaterialUniform GetUniform()
	{
		MaterialUniform result(ambient, diffuse, specular, reflect);
		return result;
	};

	void Init();
public:
	Texture * texture;//这些地方其实可以封个ptr再暴露给外部，让外部没有机会瞎搞，不过这个只是自己用的renderengine，就算了
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;//w表示高光强度
	XMFLOAT4 reflect;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
private:
	Shader * shader;//同上
	int RenderQueue;

};