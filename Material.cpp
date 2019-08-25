#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "RenderSetting.h"
#include "Mesh.h"


Material::Material()
{
	ZeroMemory(this,sizeof(this));
	Init();
}

Material::Material(const Material& cp)
{

}

void Material::operator=(const Material& cp)
{

}

Material::Material(const std::string& path)
{
	//todo���ز�������һƱ����
	Init();
}

Material::~Material()
{
	SAFE_DELETE(texture);
}

void Material::Render(Mesh* mesh)
{
	if (shader == nullptr || mesh == nullptr)
	{
		return;
	}

	ID3DX11EffectTechnique * m_pTechnique = shader->GetTech("LightTech");

	XMMATRIX world_ = XMLoadFloat4x4(&world);
	//��������任����
	ID3DX11EffectMatrixVariable * m_pFxWorld = shader->GetMatrixVariable("gWorld");
	m_pFxWorld->SetMatrix(reinterpret_cast<float*>(&world_));

	//����ͶӰ�任����
	ID3DX11EffectMatrixVariable * m_pFxWorldViewProj = shader->GetMatrixVariable("gWorldViewProj");
	m_pFxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&(world_*view*proj)));

	//���ò��� 
	MaterialUniform uniform = GetUniform();
	ID3DX11EffectVariable * m_pFxMaterial = shader->GetVariable("gMaterial");
	m_pFxMaterial->SetRawValue(&uniform, 0, sizeof(uniform));

	//����������ת��
	XMVECTOR det = XMMatrixDeterminant(world_);
	XMMATRIX worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, world_));
	ID3DX11EffectMatrixVariable * m_pFxWorldInvTranspose = shader->GetMatrixVariable("gWorldInvTranspose");
	m_pFxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));

	ID3D11ShaderResourceView* textureAtrribute = texture != nullptr ? texture->GetShaderAttribute() : nullptr;
	if (textureAtrribute != nullptr)
	{
		shader->GetResourceVariable("g_tex")->SetResource(textureAtrribute);
		shader->GetSamplerVariable("samTex")->SetSampler(0, texture->GetSampleState());
	}

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		m_pTechnique->GetPassByIndex(i)->Apply(0, RenderSetting::GetIntance()->m_pImmediateContext);
		RenderSetting::GetIntance()->m_pImmediateContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}
}

void Material::SetTxture(const std::string& name)
{
	texture = new Texture(name);
}

void Material::SetShader(const std::string& name)
{
	shader = Shader::GetShader(RenderSetting::GetIntance()->m_pd3dDevice, name);
}

void Material::SetVariable()
{

}

void Material::Init()
{
	RenderQueue = RenderQueue::Geometry;
}