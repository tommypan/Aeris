#pragma once
#include <iostream>
#include "Macro.h"

class Texture2D
{
public:
	Texture2D() :_shaderRes(nullptr), _tex(nullptr), _sampleState(nullptr){};
	Texture2D(const std::string& path);
	Texture2D(ID3D11ShaderResourceView* shaderAtt);
	~Texture2D();
	inline ID3D11ShaderResourceView* GetShaderAttribute()
	{
		return _shaderRes;
	}
	inline ID3D11SamplerState* GetSampleState()
	{
		return _sampleState;
	}
	
	inline void SetSampleState(ID3D11SamplerState* sampleState)
	{
		_sampleState = sampleState;
	};
public:
	static ID3D11SamplerState* CreateSampleState(D3D11_TEXTURE_ADDRESS_MODE mode);//需要有个池子存储

	static void DestorySamplState(ID3D11SamplerState* state);
public:
	D3D11_TEXTURE2D_DESC TexDesc;
private:
	ID3D11ShaderResourceView* _shaderRes;
	ID3D11Texture2D* _tex;
	ID3D11SamplerState* _sampleState;
};