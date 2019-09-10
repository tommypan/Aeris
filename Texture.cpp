#include "Texture.h"
#include "RenderPipeline.h"
#include "DDSTextureLoader.h"
#include "Log.h"

Texture::Texture(const std::string& path):_shaderRes(nullptr), tex(nullptr), sampleState(nullptr)
{
	std::wstring stemp = std::wstring(path.begin(), path.end());
	HRESULT hr = CreateDDSTextureFromFile(RenderPipeline::GetIntance()->m_pd3dDevice, stemp.c_str(), nullptr, &_shaderRes);
	if (FAILED(hr))
	{
		//MessageBox(nullptr, L"create texture failed!", L"error", MB_OK);
		Log::LogE("create texture failed!");
		return;
	}
	// 从资源视图获取2D纹理
	_shaderRes->GetResource((ID3D11Resource **)&tex);

	// 从2D纹理获取纹理描述
	tex->GetDesc(&texDesc);

}

Texture::Texture(ID3D11ShaderResourceView* shaderAtt)
{
	_shaderRes = shaderAtt;
}

Texture::~Texture()
{
	SAFE_RELEASE(tex);
	SAFE_RELEASE(_shaderRes);
}

bool Texture::SetSampleMode(D3D11_TEXTURE_ADDRESS_MODE mode)
{
	D3D11_SAMPLER_DESC colorMapDesc;

	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));

	colorMapDesc.AddressU = mode;

	colorMapDesc.AddressV = mode;

	colorMapDesc.AddressW = mode;

	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = RenderPipeline::GetIntance()->m_pd3dDevice->CreateSamplerState(&colorMapDesc,
		&sampleState);

	if (FAILED(hr))
	{
		Log::LogE("Failed to create color map sampler state!");
		return false;

	}

	return true;
}