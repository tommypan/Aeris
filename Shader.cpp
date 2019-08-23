#include "Shader.h"
#include "Macro.h"
#include "RenderSetting.h"

std::unordered_map<std::string, Shader*> Shader::_shaders;

Shader::Shader(ID3D11Device*m_pd3dDevice, const std::string& path)
{
	if (m_pd3dDevice == nullptr)
	{
		return;
	}

	DWORD shaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined _DEBUG || defined DEBUG
	shaderFlag = D3DCOMPILE_DEBUG;
#endif
	_name = path;
	std::wstring stemp = std::wstring(path.begin(), path.end());
	ID3DBlob * blob = nullptr;
	HRESULT result = D3DX11CompileEffectFromFile(stemp.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, shaderFlag,0,m_pd3dDevice,&_effect,&blob);
	_valide = result==0;
	if (!_valide)
	{
		return;
	}

	_effectTech = _effect->GetTechniqueByName("LightTech");
	_effectMatrixVar = _effect->GetVariableByName("gWorld")->AsMatrix();
	_effectMatrixVar = _effect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	_effectMatrixVar = _effect->GetVariableByName("gWorldViewProj")->AsMatrix();
	_effectVar = _effect->GetVariableByName("gMaterial");
	_effectVar = _effect->GetVariableByName("gDirLight");
	_effectVar = _effect->GetVariableByName("gPointLight");
	_effectVar = _effect->GetVariableByName("gSpotLight");
	_effectVectorVar = _effect->GetVariableByName("gEyePosW")->AsVector();
}

Shader::~Shader()
{
}

ID3DX11EffectTechnique *  Shader::GetTech(const std::string& name)const
{
	return _effect->GetTechniqueByName(name.c_str());
}

ID3DX11EffectVariable * Shader::GetVariable(const std::string& name)
{
	return _effect->GetVariableByName(name.c_str());
}

ID3DX11EffectShaderResourceVariable * Shader::GetResourceVariable(const std::string& name)
{
	return _effect->GetVariableByName(name.c_str())->AsShaderResource();
}

ID3DX11EffectMatrixVariable * Shader::GetMatrixVariable(const std::string& name)
{
	return _effect->GetVariableByName(name.c_str())->AsMatrix();
}

ID3DX11EffectVectorVariable * Shader::GetVectorVariable(const std::string& name)
{
	return _effect->GetVariableByName(name.c_str())->AsVector();
}

Shader* Shader::GetShader(ID3D11Device*m_pd3dDevice, const std::string& path)
{
	std::unordered_map<std::string, Shader*>::const_iterator iter =_shaders.find(path);
	if (iter == Shader::_shaders.end())
	{
		Shader * shader = new Shader(m_pd3dDevice, path);
		_shaders[path] = shader;//不放在构造函数里面，外部主动实例化的shader，自己delete，不划入池子管理
		return shader;
	}
	return iter->second;
}

void Shader::ReleaseAllShader()
{
	std::unordered_map<std::string, Shader*>::const_iterator iter = _shaders.begin();
	while (iter != _shaders.end())
	{
		SAFE_DELETE((Shader*)iter->second);
		iter++;
	}

	_shaders.clear();
}