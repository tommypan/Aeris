#pragma once
#include <iostream>
#include "Macro.h"
#include <unordered_map>

class Shader
{
public:
	Shader() {};
	Shader(ID3D11Device*m_pd3dDevice,const std::string& path);
	~Shader();

	ID3DX11EffectTechnique * GetTech(const std::string& name)const;
	ID3DX11EffectVariable * GetVariable(const std::string& name);
	ID3DX11EffectMatrixVariable * GetMatrixVariable(const std::string& name);
	ID3DX11EffectVectorVariable * GetVectorVariable(const std::string& name);
	inline bool IsValide() { return _valide; };

	static Shader* GetShader(ID3D11Device*m_pd3dDevice, const std::string& path);
	static void ReleaseAllShader();
private:
	ID3DX11Effect * _effect;
	ID3DX11EffectTechnique * _effectTech;
	ID3DX11EffectVariable * _effectVar;
	ID3DX11EffectMatrixVariable * _effectMatrixVar;
	ID3DX11EffectVectorVariable * _effectVectorVar;
	bool _valide = false;
	std::string _name;
private:
	static std::unordered_map<std::string, Shader *> _shaders;
};