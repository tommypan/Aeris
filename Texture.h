#pragma once
#include <iostream>
#include "Macro.h"

class Texture
{
public:
	Texture() :mDiffuseMapSRV(nullptr), tex(nullptr), sampleState(nullptr){};
	Texture(const std::string& path);
	~Texture();
	inline ID3D11ShaderResourceView* GetShaderAttribute()
	{
		return mDiffuseMapSRV;
	}
	inline ID3D11SamplerState* GetSampleState()
	{
		return sampleState;
	}
	bool SetSampleMode(D3D11_TEXTURE_ADDRESS_MODE mode);
public:
	D3D11_TEXTURE2D_DESC texDesc;
private:
	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11Texture2D* tex;
	ID3D11SamplerState* sampleState;
};