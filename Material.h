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

class Texture2D;
class Shader;
class Mesh;
__declspec(align(16)) class Material
{
	friend class Entity;
public:
	Material();
	Material(const Material& cp);
	void operator=(const Material& cp);
	Material(const std::string& path);
	~Material();

	void Render(Mesh* mesh);
	void RenderShadowMap(Mesh* mesh);
	void SetTxture(const std::string& name);
	void SetTxture(Texture2D * texture_);
	void SetShader(const std::string& name);
	void SetReciveShadow(bool value);
	void SetVariable(DirectX::XMMATRIX& test);

	const Shader* GetShader() { return _shader; };


	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
private :
	inline MaterialUniform GetUniform()
	{
		MaterialUniform result(Ambient, Diffuse, Specular, Reflect);
		return result;
	};

	void Init();
public:
	Texture2D * Texture;//这些地方其实可以封个ptr再暴露给外部，让外部没有机会瞎搞，不过这个只是自己用的renderengine，就算了
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;//w表示高光强度
	XMFLOAT4 Reflect;
	DirectX::XMFLOAT4X4 World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;
	bool CastShaow;
private:
	Shader * _shader;//同上
	int _renderQueue;
	bool _reciveShadow;
};