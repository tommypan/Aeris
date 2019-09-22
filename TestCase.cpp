#include "TestCase.h"
#include "RenderPipeline.h"
#include "Entity.h"
#include "Shader.h"
#include "Macro.h"
#include "Transform.h"
#include "Texture2D.h"

TestCase::TestCase() :_theta(9.00588036), _phi(0.575953603f), _radius(40.0f)
{

}

TestCase::~TestCase()
{

}

bool TestCase::LoadContent()
{
	//初始化各个物体的世界变换矩阵
	//点光源
	//m_pointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_pointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	//m_pointLight.specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	//m_pointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	//m_pointLight.range = 25.0f;
	////聚光灯
	//m_spotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_spotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	//m_spotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//m_spotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	//m_spotLight.spot = 96.0f;
	//m_spotLight.range = 10000.0f;


	//camera = new Camera();
	//camera->cullMask = 1 << Layer::Default; //| 1 << Layer::Effect;
	//camera->SetDepth(1);
	//

	//camera2 = new Camera();
	//camera2->cullMask = 1 << Layer::Effect;
	//camera2->SetDepth(2);
	//camera2->clearFlag = CameraClearFlag::DontClear;

	_camera = new Camera();
	_camera->CullMask = 1 << Layer::Effect; //| 1 << Layer::Effect;
	_camera->SetDepth(1);

	_camera2 = new Camera();
	_camera2->CullMask = 1 << Layer::Default;
	_camera2->SetDepth(2);
	_camera2->ClearFlag = CameraClearFlag::DontClear;

	Mesh* gridMesh = GeometryUtility::GetInstance()->CreateGrid(20.f, 20.f, 50, 50);
	_grid = new Entity(gridMesh);
	_grid->GetMaterial()->Ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	_grid->GetMaterial()->gloss = 16.0f;
	_grid->GetTransform()->SetPosition(Vector3(0.f, 0.f, 0.f));
	_grid->GetMaterial()->SetTxture("./Assets/grid.dds");
	ID3D11SamplerState* samplerState = Texture2D::CreateSampleState(D3D11_TEXTURE_ADDRESS_WRAP);
	_grid->GetMaterial()->Texture->SetSampleState(samplerState);
	_grid->SetLayer(Layer::Effect);
	_grid->Name = "grid";

	Mesh* boxMesh = GeometryUtility::GetInstance()->CreateBox(2, 1.5f, 2);
	_box = new Entity(boxMesh);
	_box->GetMaterial()->Ambient = XMFLOAT4(0.7f, 0.85f, 0.7f, 1.0f);
	_box->GetMaterial()->gloss = 16.0f;
	_box->SetRenderQueue(RenderQueue::Transperent);
	_box->GetTransform()->SetPosition(Vector3(0.f, 2.f, 3.f));
	_box->GetTransform()->SetScale(Vector3(2, 2, 2));
	_box->GetTransform()->SetRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 0, 1), 90));
	_box->Name = "box1";
	//_box->GetMaterial()->CastShaow = true;
	//Mesh* boxMesh2 = GeometryUtility::GetInstance()->CreateBox(2, 1.5f, 2);
	_box2 = new Entity(boxMesh);
	_box2->GetMaterial()->Ambient = XMFLOAT4(0.7f, 0.85f, 0.7f, 1.0f);
	_box2->GetMaterial()->gloss = 16.0f;
	_box2->SetRenderQueue(RenderQueue::Transperent);
	_box2->GetTransform()->SetPosition(Vector3(-1.f, 3.f, 8.f));
	_box2->GetTransform()->SetScale(Vector3(2, 2, 2));
	_box2->GetTransform()->SetRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 0, 1), 90));
	_box2->Name = "box2";
	//_box2->GetMaterial()->CastShaow = true;

	//Mesh* sphereMesh = GeometryUtility::GetInstance()->CreateSphere(2, 30, 30);
	//m_sphere[4] = new Entity(sphereMesh);
	//m_sphere[4]->GetMaterial()->ambient = XMFLOAT4(0.1f, 0.2f, 0.3f, 1.0f);
	//m_sphere[4]->GetMaterial()->diffuse = XMFLOAT4(0.2f, 0.4f, 0.6f, 1.0f);
	//m_sphere[4]->GetMaterial()->specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);
	//m_sphere[4]->GetTransform()->SetPosition(Vector3(0.f, 3.5f, 0.f));

	////4个圆柱和4个球（位于4个对称点上）
	//for (UINT i = 0; i < 2; ++i)
	//{
	//	for (UINT j = 0; j < 2; ++j)
	//	{

	//		Mesh* sphereMesh = GeometryUtility::GetInstance()->CreateSphere(2, 30, 30);
	//		m_sphere[i * 2 + j] = new Entity(sphereMesh);
	//		m_sphere[i * 2 + j]->GetMaterial()->ambient = XMFLOAT4(0.1f, 0.2f, 0.3f, 1.0f);
	//		m_sphere[i * 2 + j]->GetMaterial()->diffuse = XMFLOAT4(0.2f, 0.4f, 0.6f, 1.0f);
	//		m_sphere[i * 2 + j]->GetMaterial()->specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);
	//		m_sphere[i * 2 + j]->GetTransform()->SetPosition(Vector3(-5.f + i*10.f, 4.f, -5.f + j*10.f));
	//		//m_sphere[i * 2 + j]->SetLayer(Layer::UI);

	//		Mesh* cylinderMesh = GeometryUtility::GetInstance()->CreateCylinder(0.5f, 0.5f, 2, 20, 20);
	//		m_cylinder[i * 2 + j] = new Entity(cylinderMesh);
	//		m_cylinder[i * 2 + j]->GetMaterial()->ambient = XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
	//		m_cylinder[i * 2 + j]->GetMaterial()->diffuse = XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
	//		m_cylinder[i * 2 + j]->GetMaterial()->specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
	//		m_cylinder[i * 2 + j]->GetTransform()->SetPosition(Vector3(-5.f + i*10.f, 1.f, -5.f + j*10.f));
	//		//m_cylinder[i * 2 + j]->SetLayer(Layer::UI);
	//	}
	//}

	Entity * customEntity = new Entity("./Assets/12.obj");
	customEntity->GetMaterial()->Ambient = XMFLOAT4(0.1f, 0.2f, 0.3f, 1.0f);
	customEntity->GetMaterial()->gloss = 16.0f;
	customEntity->GetTransform()->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	customEntity->GetMaterial()->SetTxture("./Assets/test2.dds");
	customEntity->GetMaterial()->CastShaow = true;
	customEntity->Name = "npc";

	Shader* deshader = Shader::GetShader(RenderPipeline::GetIntance()->Device, "FX\\DeferredAttribute.fx");


	InitSun();

	return true;
}

void TestCase::UnLoadContent()
{
	SAFE_DELETE(_sun);
	SAFE_DELETE(_camera);
	SAFE_DELETE(_camera2);
	SAFE_DELETE(_grid);
	SAFE_DELETE(_box);
	SAFE_DELETE(_box2);

	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(_sphere[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(_cylinder[i]);
	}

	Shader::ReleaseAllShader();
}

void TestCase::Update(float dt)
{

	float x = _radius*sinf(_phi)*cosf(_theta);
	float z = _radius*sinf(_phi)*sinf(_theta);
	float y = _radius*cosf(_phi);

	Vector3 orignPos = Vector3(x, y, z);

	Matrix v = Matrix::CreateLookAt(orignPos, Vector3::Zero, Vector3::Up);
	Matrix invertM = v.Invert();
	Vector3 cameraScale;
	Quaternion cameraRot;
	Vector3 cameraPos;
	invertM.Decompose(cameraScale, cameraRot, cameraPos);
	_camera->GetTransform()->SetScale(cameraScale);
	_camera->GetTransform()->SetRotation(cameraRot);
	_camera->GetTransform()->SetPosition(cameraPos);
	_camera2->GetTransform()->SetScale(cameraScale);
	_camera2->GetTransform()->SetRotation(cameraRot);
	_camera2->GetTransform()->SetPosition(cameraPos);

	
	//点光源和聚光灯要设置其位置
	//点光源位置
	//m_pointLight.position = XMFLOAT3(0.0f, 5.0f, 0.0f);
	//聚光灯位置设为相机位置，朝向看的位置
	//m_spotLight.position = orignPos;
	//XMStoreFloat3(&m_spotLight.direction, XMVector3Normalize(Vector3(0, 0, 0) - Vector3(x, y, z)));

	//set constant buffer
	Shader* shader = Shader::GetShader(RenderPipeline::GetIntance()->Device, "FX\\DeferredShading.fx");//todo
	DirectionalLight dl[1] = { _sun->Light };
	shader->GetVariable("gDirLight")->SetRawValue(&(dl), 0, sizeof(dl));
	//shader->GetVariable("gPointLight")->SetRawValue(&m_pointLight, 0, sizeof(m_pointLight));
	//shader->GetVariable("gSpotLight")->SetRawValue(&m_spotLight, 0, sizeof(m_spotLight));
	shader->GetVectorVariable("gEyePosW")->SetRawValue(&((XMFLOAT3)_camera->GetTransform()->GetPosition()), 0, sizeof(XMFLOAT3));
}

void TestCase::OnMouseDown(WPARAM btnState, int x, int y)
{
	_lastMousePos.x = x;
	_lastMousePos.y = y;
	SetCapture(RenderPipeline::GetIntance()->HWnd);
}

void TestCase::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

//限制number在low和high之间 
template<typename T>
static T Clamp(const T& x, const T& low, const T& high)
{
	return x < low ? low : (x > high ? high : x);
}

void TestCase::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		//角度转弧度
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - _lastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - _lastMousePos.y));

		_theta -= dx;
		_phi -= dy;

		_phi = Clamp(_phi, 0.1f, XM_PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		float dx = 0.2f*static_cast<float>(x - _lastMousePos.x);
		float dy = 0.2f*static_cast<float>(y - _lastMousePos.y);

		_radius += dx - dy;

		_radius = Clamp(_radius, 3.0f, 300.0f);
	}

	_lastMousePos.x = x;
	_lastMousePos.y = y;
}


void TestCase::InitSun()
{
	_sun = new Sun();

	//平行光
	_sun->Light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	_sun->Light.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	_sun->Light.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	_sun->Light.direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	float x = _radius*sinf(_phi)*cosf(_theta);
	float z = _radius*sinf(_phi)*sinf(_theta);
	float y = _radius*cosf(_phi);

	Vector3 orignPos = Vector3(x, y, z);

	Matrix v = Matrix::CreateLookAt(orignPos, Vector3::Zero, Vector3::Up);
	Matrix invertM = v.Invert();
	Vector3 cameraScale;
	Quaternion cameraRot;
	Vector3 cameraPos;
	invertM.Decompose(cameraScale, cameraRot, cameraPos);
	_sun->GetTransform()->SetRotation(cameraRot);
	_sun->GetTransform()->SetPosition(Vector3(cameraPos.x, cameraPos.y, cameraPos.z));
}