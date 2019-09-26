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
	Texture2D * Texture;//��Щ�ط���ʵ���Է��ptr�ٱ�¶���ⲿ�����ⲿû�л���Ϲ�㣬�������ֻ���Լ��õ�renderengine��������
	float gloss;//w��ʾ�߹�ǿ��
	XMFLOAT4 Reflect;
	DirectX::XMFLOAT4X4 World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;
	bool CastShaow;
private:
	Shader * _shader;//ͬ��
	int _renderQueue;
	bool _reciveShadow;
};