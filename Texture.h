#pragma once
#include <iostream>
#include "Macro.h"

class Texture
{
public:
	Texture() :mDiffuseMapSRV(nullptr), tex(nullptr) {};
	Texture(const std::string& path);
	~Texture();
public:
	D3D11_TEXTURE2D_DESC texDesc;
	ID3D11Texture2D* tex;
private:
	ID3D11ShaderResourceView* mDiffuseMapSRV;

};