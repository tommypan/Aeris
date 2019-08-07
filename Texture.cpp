#include "Texture.h"
#include "RenderSetting.h"
#include "DDSTextureLoader.h"
#include "Log.h"

Texture::Texture(const std::string& path):mDiffuseMapSRV(nullptr), tex(nullptr)
{
	std::wstring stemp = std::wstring(path.begin(), path.end());
	HRESULT hr = CreateDDSTextureFromFile(RenderSetting::GetIntance()->m_pd3dDevice, stemp.c_str(), nullptr, &mDiffuseMapSRV);
	if (FAILED(hr))
	{
		//MessageBox(nullptr, L"create texture failed!", L"error", MB_OK);
		Log::LogE("create texture failed!");
		return;
	}
	// 从资源视图获取2D纹理
	mDiffuseMapSRV->GetResource((ID3D11Resource **)&tex);

	// 从2D纹理获取纹理描述
	tex->GetDesc(&texDesc);
}

Texture::~Texture()
{
	SAFE_RELEASE(tex);
	SAFE_RELEASE(mDiffuseMapSRV);
}