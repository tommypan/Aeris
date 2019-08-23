#pragma once
#include <iostream>
#include "Macro.h"

class Texture
{
public:
	Texture() :mDiffuseMapSRV(nullptr), tex(nullptr) {};
	Texture(const std::string& path);
	~Texture();
	inline ID3D11ShaderResourceView* GetShaderAttribute()
	{
		return mDiffuseMapSRV;
	}
public:
	D3D11_TEXTURE2D_DESC texDesc;
private:
	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11Texture2D* tex;

};