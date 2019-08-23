#include "Entity.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderSetting.h"
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
	_material->SetShader("FX\\Lighting.fx");
	_meshRender = new MeshRender(_mesh,_material);
	Scene::GetInstance()->AddChild(this);
}

Entity::Entity(const std::string&  meshPath, const std::string& materialPathh) :_mesh(nullptr),_material(nullptr), _layer(Layer::Default)
{
	//各种判空
	_transform = new Transform(this);
	_mesh = new Mesh(meshPath);
	_material = new Material(materialPathh);
	_material->SetShader("FX\\Lighting.fx");
	_meshRender = new MeshRender(_mesh, _material);
	Scene::GetInstance()->AddChild(this);
}

Entity::Entity(const std::string&  meshPath) :_mesh(nullptr), _material(nullptr), _layer(Layer::Default)
{
	//各种判空
	_transform = new Transform(this);
	_mesh = new Mesh(meshPath);
	_material = new Material();
	_material->SetShader("FX\\Lighting.fx");
	_meshRender = new MeshRender(_mesh, _material);
	Scene::GetInstance()->AddChild(this);
}

Entity::~Entity()
{
	Clear();
}

void Entity::Render(CXMMATRIX view, CXMMATRIX proj)
{
	if (_meshRender != nullptr)
	{
		if (_material != nullptr)//todo
		{
			_material->world = _transform->GetWorldTransform();
			_material->view = view;
			_material->proj = proj;
		}
		_meshRender->Render();
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