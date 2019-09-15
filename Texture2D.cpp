#include "Texture2D.h"
#include "RenderPipeline.h"
#include "DDSTextureLoader.h"
#include "Log.h"

Texture2D::Texture2D(const std::string& path):_shaderRes(nullptr), _tex(nullptr), _sampleState(nullptr)
{
	std::wstring stemp = std::wstring(path.begin(), path.end());
	HRESULT hr = CreateDDSTextureFromFile(RenderPipeline::GetIntance()->Device, stemp.c_str(), nullptr, &_shaderRes);
	if (FAILED(hr))
	{
		//MessageBox(nullptr, L"create texture failed!", L"error", MB_OK);
		Log::LogE("create texture failed!");
		return;
	}
	// 从资源视图获取2D纹理
	_shaderRes->GetResource((ID3D11Resource **)&_tex);

	// 从2D纹理获取纹理描述
	_tex->GetDesc(&TexDesc);

}

Texture2D::Texture2D(ID3D11ShaderResourceView* shaderAtt)
{
	_shaderRes = shaderAtt;
}

Texture2D::~Texture2D()
{
	SAFE_RELEASE(_tex);
	SAFE_RELEASE(_shaderRes);
	SAFE_RELEASE(_sampleState);
}

ID3D11SamplerState* Texture2D::CreateSampleState(D3D11_TEXTURE_ADDRESS_MODE mode)
{
	ID3D11SamplerState* result = nullptr;

	D3D11_SAMPLER_DESC colorMapDesc;

	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));

	colorMapDesc.AddressU = mode;

	colorMapDesc.AddressV = mode;

	colorMapDesc.AddressW = mode;

	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = RenderPipeline::GetIntance()->Device->CreateSamplerState(&colorMapDesc,
		&result);

	if (FAILED(hr))
	{
		Log::LogE("Failed to create color map sampler state!");
		return nullptr;

	}

	return result;
}

void Texture2D::DestorySamplState(ID3D11SamplerState* state)
{
	SAFE_RELEASE(state)
}