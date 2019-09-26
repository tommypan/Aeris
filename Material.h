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
	MaterialUniform(const float& gloss_,const XMFLOAT4& reflect_)
	{
		gloss = gloss_;
		reflect = reflect_;
	}

	float gloss;
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

	void Render(Mesh* mesh,bool isDefer);
	void RenderShadowMap(Mesh* mesh);
	void SetTxture(const std::string& name);
	void SetTxture(Texture2D * texture_);
	void SetShader(const std::string& name);
	void SetReciveShadow(bool value);
	void SetCustomMatrix(const std::string& name, DirectX::XMMATRIX& matrix);

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
		MaterialUniform result(gloss, Reflect);
		return result;
	};

	void Init();
public:
	Texture2D * Texture;//这些地方其实可以封个ptr再暴露给外部，让外部没有机会瞎搞，不过这个只是自己用的renderengine，就算了
	float gloss;//w表示高光强度
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