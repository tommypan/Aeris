#include "Texture.h"
#include "Macro.h"
#include "RenderSetting.h"
#include "DDSTextureLoader.h"
#include "Log.h"

Texture::Texture(const std::string& path)
{
	std::wstring stemp = std::wstring(path.begin(), path.end());
	ID3D11ShaderResourceView* mDiffuseMapSRV;
	HRESULT hr = CreateDDSTextureFromFile(RenderSetting::GetIntance()->m_pd3dDevice, stemp.c_str(), nullptr, &mDiffuseMapSRV);
	if (FAILED(hr))
	{
		//MessageBox(nullptr, L"create texture failed!", L"error", MB_OK);
		Log::LogE("create texture failed!");
		return;
	}
	// ����Դ��ͼ��ȡ2D����
	ID3D11Texture2D* tex;
	mDiffuseMapSRV->GetResource((ID3D11Resource **)&tex);

	// ��2D�����ȡ��������
	D3D11_TEXTURE2D_DESC texDesc;
	tex->GetDesc(&texDesc);
}

Texture::~Texture()
{
}