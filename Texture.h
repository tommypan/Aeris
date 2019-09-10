#pragma once
#include <iostream>
#include "Macro.h"

class Texture
{
public:
	Texture() :_shaderRes(nullptr), tex(nullptr), sampleState(nullptr){};
	Texture(const std::string& path);
	Texture(ID3D11ShaderResourceView* shaderAtt);
	~Texture();
	inline ID3D11ShaderResourceView* GetShaderAttribute()
	{
		return _shaderRes;
	}
	inline ID3D11SamplerState* GetSampleState()
	{
		return sampleState;
	}
	bool SetSampleMode(D3D11_TEXTURE_ADDRESS_MODE mode);
public:
	D3D11_TEXTURE2D_DESC texDesc;
private:
	ID3D11ShaderResourceView* _shaderRes;
	ID3D11Texture2D* tex;
	ID3D11SamplerState* sampleState;
};