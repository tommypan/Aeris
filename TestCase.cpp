#include "TestCase.h"
#include "RenderSetting.h"
#include "Entity.h"
#include "Shader.h"
#include "Macro.h"
#include "Transform.h"
#include "Loader.h"
#include "Texture.h"

TestCase::TestCase() :m_theta(1.5f*XM_PI), m_phi(0.4f*XM_PI), m_radius(40.0f)
{

}

TestCase::~TestCase()
{

}

bool TestCase::LoadContent()
{
	//初始化各个物体的世界变换矩阵
	//平行光
	m_dirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_dirLight.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_dirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_dirLight.direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	//点光源
	m_pointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_pointLight.specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_pointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	m_pointLight.range = 25.0f;
	//聚光灯
	m_spotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_spotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_spotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_spotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_spotLight.spot = 96.0f;
	m_spotLight.range = 10000.0f;
	camera = new Camera();
	camera->cullMask = 1 << Layer::Default | 1 << Layer::Effect;

	Mesh* gridMesh = GeometryUtility::GetInstance()->CreateGrid(20.f, 20.f, 50, 50);
	m_grid = new Entity(gridMesh);
	m_grid->GetMaterial()->ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	m_grid->GetMaterial()->diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	m_grid->GetMaterial()->specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
	m_grid->GetTransform()->SetPosition(Vector3(0.f, 0.f, 0.f));

	Mesh* boxMesh = GeometryUtility::GetInstance()->CreateBox(2, 1.5f, 2);
	m_box = new Entity(boxMesh);
	m_box->GetMaterial()->ambient = XMFLOAT4(0.7f, 0.85f, 0.7f, 1.0f);
	m_box->GetMaterial()->diffuse = XMFLOAT4(0.7f, 0.85f, 0.7f, 1.0f);
	m_box->GetMaterial()->specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	m_box->GetTransform()->SetPosition(Vector3(0.f, .75f, 0.f));
	m_box->GetTransform()->SetScale(Vector3(2, 2, 2));
	m_box->GetTransform()->SetRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 0, 1), 90));


	Mesh* sphereMesh = GeometryUtility::GetInstance()->CreateSphere(2, 30, 30);
	m_sphere[4] = new Entity(sphereMesh);
	m_sphere[4]->GetMaterial()->ambient = XMFLOAT4(0.1f, 0.2f, 0.3f, 1.0f);
	m_sphere[4]->GetMaterial()->diffuse = XMFLOAT4(0.2f, 0.4f, 0.6f, 1.0f);
	m_sphere[4]->GetMaterial()->specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);
	m_sphere[4]->GetTransform()->SetPosition(Vector3(0.f, 3.5f, 0.f));

	//4个圆柱和4个球（位于4个对称点上）
	for (UINT i = 0; i < 2; ++i)
	{
		for (UINT j = 0; j < 2; ++j)
		{

			Mesh* sphereMesh = GeometryUtility::GetInstance()->CreateSphere(2, 30, 30);
			m_sphere[i * 2 + j] = new Entity(sphereMesh);
			m_sphere[i * 2 + j]->GetMaterial()->ambient = XMFLOAT4(0.1f, 0.2f, 0.3f, 1.0f);
			m_sphere[i * 2 + j]->GetMaterial()->diffuse = XMFLOAT4(0.2f, 0.4f, 0.6f, 1.0f);
			m_sphere[i * 2 + j]->GetMaterial()->specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);
			m_sphere[i * 2 + j]->GetTransform()->SetPosition(Vector3(-5.f + i*10.f, 4.f, -5.f + j*10.f));
			//m_sphere[i * 2 + j]->SetLayer(Layer::UI);

			Mesh* cylinderMesh = GeometryUtility::GetInstance()->CreateCylinder(0.5f, 0.5f, 2, 20, 20);
			m_cylinder[i * 2 + j] = new Entity(cylinderMesh);
			m_cylinder[i * 2 + j]->GetMaterial()->ambient = XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
			m_cylinder[i * 2 + j]->GetMaterial()->diffuse = XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
			m_cylinder[i * 2 + j]->GetMaterial()->specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
			m_cylinder[i * 2 + j]->GetTransform()->SetPosition(Vector3(-5.f + i*10.f, 1.f, -5.f + j*10.f));
			//m_cylinder[i * 2 + j]->SetLayer(Layer::UI);
		}
	}

	Mesh * testM = new Mesh();
	Loader::GetInstance()->Load("./Assets/12.obj", testM);
	Entity * customEntity = new Entity(testM);
	customEntity->GetMaterial()->ambient = XMFLOAT4(0.1f, 0.2f, 0.3f, 1.0f);
	customEntity->GetMaterial()->diffuse = XMFLOAT4(0.2f, 0.4f, 0.6f, 1.0f);
	customEntity->GetMaterial()->specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);
	customEntity->GetTransform()->SetScale(Vector3(0.05, 0.05, 0.05));
	customEntity->GetTransform()->SetPosition(Vector3(5, 0,1));

	Texture * tex = new Texture("./Assets/test2.dds");

	return true;
}

void TestCase::UnLoadContent()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(m_grid);
	SAFE_DELETE(m_box);

	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(m_sphere[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(m_cylinder[i]);
	}

	Shader::ReleaseAllShader();
}

void TestCase::Update(float dt)
{
	float x = m_radius*sinf(m_phi)*cosf(m_theta);
	float z = m_radius*sinf(m_phi)*sinf(m_theta);
	float y = m_radius*cosf(m_phi);

	Vector3 orignPos = Vector3(x, y, z);

	Matrix v = Matrix::CreateLookAt(orignPos, Vector3::Zero, Vector3::Up);
	Matrix invertM = v.Invert();
	Vector3 cameraScale;
	Quaternion cameraRot;
	Vector3 cameraPos;
	invertM.Decompose(cameraScale, cameraRot, cameraPos);
	camera->GetTransform()->SetScale(cameraScale);
	camera->GetTransform()->SetRotation(cameraRot);
	camera->GetTransform()->SetPosition(cameraPos);

	//点光源和聚光灯要设置其位置
	//点光源位置
	m_pointLight.position = XMFLOAT3(0.0f, 5.0f, 0.0f);
	//聚光灯位置设为相机位置，朝向看的位置
	m_spotLight.position = orignPos;
	XMStoreFloat3(&m_spotLight.direction, XMVector3Normalize(Vector3(0, 0, 0) - Vector3(x, y, z)));


	//set constant buffer
	Shader* shader = Shader::GetShader(nullptr, "FX\\Lighting.fx");
	shader->GetVariable("gDirLight")->SetRawValue(&m_dirLight, 0, sizeof(m_dirLight));
	shader->GetVariable("gPointLight")->SetRawValue(&m_pointLight, 0, sizeof(m_pointLight));
	shader->GetVariable("gSpotLight")->SetRawValue(&m_spotLight, 0, sizeof(m_spotLight));
	shader->GetVectorVariable("gEyePosW")->SetRawValue(&((XMFLOAT3)camera->GetTransform()->GetPosition()), 0, sizeof(XMFLOAT3));
}

void TestCase::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
	SetCapture(RenderSetting::GetIntance()->m_hWnd);
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
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - m_lastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - m_lastMousePos.y));

		m_theta -= dx;
		m_phi -= dy;

		m_phi = Clamp(m_phi, 0.1f, XM_PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		float dx = 0.2f*static_cast<float>(x - m_lastMousePos.x);
		float dy = 0.2f*static_cast<float>(y - m_lastMousePos.y);

		m_radius += dx - dy;

		m_radius = Clamp(m_radius, 3.0f, 300.0f);
	}

	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
}


