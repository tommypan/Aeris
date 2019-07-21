#pragma once
#include <iostream>
#include <unordered_map>
#include "Macro.h"

using namespace DirectX;

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
	XMFLOAT4 specular;//w��ʾ�߹�ǿ��
	XMFLOAT4 reflect;

};

class Texture;
class Shader;
class Mesh;
class Material
{
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

public:
	Texture * texture;//��Щ�ط���ʵ���Է��ptr�ٱ�¶���ⲿ�����ⲿû�л���Ϲ�㣬�������ֻ���Լ��õ�renderengine��������
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;//w��ʾ�߹�ǿ��
	XMFLOAT4 reflect;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
private:
	Shader * shader;//ͬ��

	//��Դ�Ͳ���
	//DirectionalLight				m_dirLight;
	//PointLight						m_pointLight;
	//SpotLight						m_spotLight;
	//XMFLOAT3						m_eyePosW;

//private:
//	static std::unordered_map<std::string, Material*> _materials; sharedmaterial��������

};