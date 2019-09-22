#include "Entity.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderPipeline.h"
#include "Shader.h"
#include "MeshRender.h"
#include "Scene.h"

Entity::Entity():_layer(Layer::Default)
{ 
	_transform = new Transform(this); 
	_mesh = new Mesh();
	_material = new Material();
	Scene::GetInstance()->AddChild(this);
}

Entity::Entity(Mesh* mesh) :_layer(Layer::Default)
{
	_transform = new Transform(this);
	_mesh = mesh;
	_material = new Material();
	_material->SetShader("FX\\DeferredAttribute.fx");
	_meshRender = new MeshRender(_mesh,_material);
	Scene::GetInstance()->AddChild(this);
}

Entity::Entity(const std::string&  meshPath, const std::string& materialPathh) :_mesh(nullptr),_material(nullptr), _layer(Layer::Default)
{
	//各种判空
	_transform = new Transform(this);
	_mesh = new Mesh(meshPath);
	_material = new Material(materialPathh);
	_material->SetShader("FX\\DeferredAttribute.fx");
	_meshRender = new MeshRender(_mesh, _material);
	Scene::GetInstance()->AddChild(this);
}

Entity::Entity(const std::string&  meshPath) :_mesh(nullptr), _material(nullptr), _layer(Layer::Default)
{
	//各种判空
	_transform = new Transform(this);
	_mesh = new Mesh(meshPath);
	_material = new Material();
	_material->SetShader("FX\\DeferredAttribute.fx");
	_meshRender = new MeshRender(_mesh, _material);
	Scene::GetInstance()->AddChild(this);
}

Entity::~Entity()
{
	Clear();
}

void Entity::Render(CXMMATRIX view, CXMMATRIX proj, bool genShadowMap)
{
	if (_meshRender != nullptr)
	{
		if (_material != nullptr)
		{
			_material->World = _transform->GetWorldTransform();
			_material->View = view;
			_material->Proj = proj;
		}
		if (genShadowMap)
		{
			_meshRender->RenderShadowMap();
		}
		else
		{
			_meshRender->Render();
		}
	}
	
}

void Entity::test(CXMMATRIX view, CXMMATRIX proj)
{
	XMMATRIX world = XMLoadFloat4x4(&_transform->GetWorldTransform());
	XMMATRIX mvp = (world*view*proj);
	_material->SetCustomMatrix("customMVP",mvp);
}

void Entity::SetRenderQueue(int queue)
{
	bool needResort = false;
	if (queue != _material->_renderQueue)//todo 有更好的实现
	{
		needResort = true;
		Scene::GetInstance()->RemoveChild(this);
	}
	_material->_renderQueue = queue;
	if (needResort)
	{
		Scene::GetInstance()->AddChild(this);
	}
}

bool Entity::operator<(Entity* compareE)
{
	if (GetRenderQueue() == compareE->GetRenderQueue())
	{
		return this->_distToCurCamera > compareE->_distToCurCamera;
	}

	return GetRenderQueue() < compareE->GetRenderQueue();
}

void Entity::Clear()
{
	_mesh = nullptr;
	_material = nullptr;
	SAFE_DELETE(_meshRender);
	SAFE_DELETE(_transform);

}