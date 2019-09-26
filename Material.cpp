#include "Material.h"
#include "Texture2D.h"
#include "Shader.h"
#include "RenderPipeline.h"
#include "Mesh.h"
#include "RenderTexture.h"

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
	//todo加载材质球，做一票事情
	Init();
}

Material::~Material()
{
	SAFE_DELETE(Texture);
}

void Material::Render(Mesh* mesh, bool isDefer)
{
	if (_shader == nullptr || mesh == nullptr)
	{
		return;
	}

	_shader = isDefer ? Shader::GetShader(RenderPipeline::GetIntance()->Device, "FX\\DeferredAttribute.fx")
		: Shader::GetShader(RenderPipeline::GetIntance()->Device, "FX\\DepthLighting.fx");

	ID3DX11EffectTechnique * technique = _shader->GetTech("LightTech");

	XMMATRIX world = XMLoadFloat4x4(&World);
	//设置世界变换矩阵
	ID3DX11EffectMatrixVariable * fxWorld = _shader->GetMatrixVariable("gWorld");
	fxWorld->SetMatrix(reinterpret_cast<float*>(&world));

	//设置投影变换矩阵
	ID3DX11EffectMatrixVariable * fxWorldViewProj = _shader->GetMatrixVariable("gWorldViewProj");
	fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&(world*View*Proj)));

	//设置材质 
	MaterialUniform uniform = GetUniform();
	ID3DX11EffectVariable * fxMaterial = _shader->GetVariable("gMaterial");
	fxMaterial->SetRawValue(&uniform, 0, sizeof(uniform));

	//设置逆矩阵的转置
	XMVECTOR det = XMMatrixDeterminant(world);
	XMMATRIX worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, world));
	ID3DX11EffectMatrixVariable * fxWorldInvTranspose = _shader->GetMatrixVariable("gWorldInvTranspose");
	fxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));

	ID3D11ShaderResourceView* textureAtrribute = Texture != nullptr ? Texture->GetShaderAttribute() : nullptr;
	if (textureAtrribute != nullptr)
	{
		_shader->GetResourceVariable("g_tex")->SetResource(textureAtrribute);
		ID3D11ShaderResourceView* test = RenderPipeline::GetIntance()->ShadowDepthTexture->GetShaderResourceView();
		_shader->GetResourceVariable("shadow_tex")->SetResource(test);
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

void Material::RenderShadowMap(Mesh* mesh)
{
	Shader* depthShader = Shader::GetShader(RenderPipeline::GetIntance()->Device, "FX\\GenDepth.fx");
	if (depthShader != nullptr)
	{
		ID3DX11EffectTechnique * technique = depthShader->GetTech("LightTech");

		XMMATRIX world = XMLoadFloat4x4(&World);
		ID3DX11EffectMatrixVariable * fxWorldViewProj = depthShader->GetMatrixVariable("gWorldViewProj");
		fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&(world*View*Proj)));

		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			technique->GetPassByIndex(i)->Apply(0, RenderPipeline::GetIntance()->DeviceContext);
			RenderPipeline::GetIntance()->DeviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
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

void Material::SetReciveShadow(bool value)
{
	if (_reciveShadow)
	{
		std::string name = "Depth" + _shader->Name;//todo check
		_shader = Shader::GetShader(RenderPipeline::GetIntance()->Device, name);
	}
	else
	{
		//todo
	}
}

void Material::SetCustomMatrix(const std::string& name,DirectX::XMMATRIX& matrix)
{
	ID3DX11EffectMatrixVariable * fxWorldViewProj = _shader->GetMatrixVariable(name);
	if (fxWorldViewProj != nullptr)
	{
		fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&(matrix)));
	}
}

void Material::Init()
{
	_renderQueue = RenderQueue::Geometry;
}