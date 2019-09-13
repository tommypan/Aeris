#include "Material.h"
#include "Texture2D.h"
#include "Shader.h"
#include "RenderPipeline.h"
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
	SAFE_DELETE(Texture);
}

void Material::Render(Mesh* mesh)
{
	if (_shader == nullptr || mesh == nullptr)
	{
		return;
	}

	ID3DX11EffectTechnique * technique = _shader->GetTech("LightTech");

	XMMATRIX world = XMLoadFloat4x4(&World);
	//��������任����
	ID3DX11EffectMatrixVariable * fxWorld = _shader->GetMatrixVariable("gWorld");
	fxWorld->SetMatrix(reinterpret_cast<float*>(&world));

	//����ͶӰ�任����
	ID3DX11EffectMatrixVariable * fxWorldViewProj = _shader->GetMatrixVariable("gWorldViewProj");
	fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&(world*View*Proj)));

	//���ò��� 
	MaterialUniform uniform = GetUniform();
	ID3DX11EffectVariable * fxMaterial = _shader->GetVariable("gMaterial");
	fxMaterial->SetRawValue(&uniform, 0, sizeof(uniform));

	//����������ת��
	XMVECTOR det = XMMatrixDeterminant(world);
	XMMATRIX worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, world));
	ID3DX11EffectMatrixVariable * fxWorldInvTranspose = _shader->GetMatrixVariable("gWorldInvTranspose");
	fxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));

	ID3D11ShaderResourceView* textureAtrribute = Texture != nullptr ? Texture->GetShaderAttribute() : nullptr;
	if (textureAtrribute != nullptr)
	{
		_shader->GetResourceVariable("g_tex")->SetResource(textureAtrribute);
		_shader->GetSamplerVariable("samTex")->SetSampler(0, Texture->GetSampleState());
	}

	D3DX11_TECHNIQUE_DESC techDesc;
	technique->GetDesc(&techDesc);
	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		technique->GetPassByIndex(i)->Apply(0, RenderPipeline::GetIntance()->DeviceContext);
		RenderPipeline::GetIntance()->DeviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}
}

void Material::SetTxture(const std::string& name)
{
	Texture = new Texture2D(name);
}
void Material::SetTxture(Texture2D * texture_)
{
	Texture = texture_;
}

void Material::SetShader(const std::string& name)
{
	_shader = Shader::GetShader(RenderPipeline::GetIntance()->Device, name);
}

void Material::SetVariable()
{

}

void Material::Init()
{
	_renderQueue = RenderQueue::Geometry;
}