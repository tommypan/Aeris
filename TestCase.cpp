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
	//_pointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//_pointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	//_pointLight.specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	//_pointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	//_pointLight.range = 25.0f;
	////聚光灯
	//_spotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//_spotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	//_spotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//_spotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	//_spotLight.spot = 96.0f;
	//_spotLight.range = 10000.0f;

	_camera = new Camera();
	_camera->CullMask = 1 << Layer::Default; //| 1 << Layer::Effect;
	_camera->SetDepth(1);
	_camera->SetRenderPath(CameraRenderPath::DeferShading);

	_camera2 = new Camera();
	_camera2->CullMask = 1 << Layer::Effect;
	_camera2->SetDepth(2);
	_camera2->ClearFlag = CameraClearFlag::DontClear;
	_camera2->SetRenderPath(CameraRenderPath::Forward);

	Mesh* gridMesh = GeometryUtility::GetInstance()->CreateGrid(20.f, 20.f, 50, 50);
	_grid = new Entity(gridMesh);
	_grid->GetMaterial()->gloss = 16.0f;
	_grid->GetTransform()->SetPosition(Vector3(0.f, 0.f, 0.f));
	_grid->GetMaterial()->SetTxture("./Assets/grid.dds");
	ID3D11SamplerState* samplerState = Texture2D::CreateSampleState(D3D11_TEXTURE_ADDRESS_WRAP);
	_grid->GetMaterial()->Texture->SetSampleState(samplerState);
	_grid->SetLayer(Layer::Effect);
	_grid->Name = "grid";
	_grid->GetMaterial()->SetReciveShadow(true);

	Mesh* waterMesh = GeometryUtility::GetInstance()->CreateGrid(20.f, 20.f, 1, 1);
	_water = new Entity(waterMesh);
	_water->GetMaterial()->gloss = 16.0f;
	_water->GetTransform()->SetPosition(Vector3(0.f, 2.f, 0.f));
	_water->GetMaterial()->SetTxture("./Assets/water.dds");
	_water->SetLayer(Layer::Effect);
	_water->Name = "water";
	_water->SetRenderQueue(RenderQueue::Transperent);
	_water->GetMaterial()->Texture->SetSampleState(samplerState);


	Mesh* boxMesh = GeometryUtility::GetInstance()->CreateBox(2, 1.5f, 2);
	float signX = -1.0f;
	float signZ = -1.0f;
	for (int i = 0; i < 4; i++ )
	{
		_box[i] = new Entity(boxMesh);
		_box[i]->GetMaterial()->gloss = 16.0f;
		_box[i]->GetTransform()->SetPosition(Vector3(10.f*signX, 3.f, 10.f*signZ));
		_box[i]->GetTransform()->SetScale(Vector3(1, 4, 1));
		_box[i]->Name = "box";
		_box[i]->GetMaterial()->SetTxture("./Assets/grid.dds");
		_box[i]->SetLayer(Layer::Effect);
		_box[i]->GetMaterial()->CastShaow = true;
		_pointLights[i].ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		_pointLights[i].diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
		_pointLights[i].specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
		_pointLights[i].att = XMFLOAT3(0.0f, 0.1f, 0.0f);
		_pointLights[i].range = 25.0f;
		_pointLights[i].position = Vector3(10.f*signX, 5.f, 10.f*signZ);

		if (i == 0)
		{
			signX = -1.0f;
			signZ = 1.0f;
		}
		else if(i == 1)
		{
			signX = 1.0f;
			signZ = -1.0f;
		}
		else
		{
			signX = 1.0f;
			signZ = 1.0f;
		}
	}


	Entity * customEntity = new Entity("./Assets/beauty.obj");
	customEntity->GetMaterial()->gloss = 16.0f;
	customEntity->GetTransform()->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	customEntity->GetMaterial()->SetTxture("./Assets/test2.dds");
	customEntity->GetMaterial()->CastShaow = true;
	customEntity->Name = "npc";

	InitSun();

	return true;
}

void TestCase::UnLoadContent()
{
	SAFE_DELETE(_sun);
	SAFE_DELETE(_camera);
	SAFE_DELETE(_camera2);
	SAFE_DELETE(_grid);
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(_box[i]);
	}
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
	//_pointLight.position = XMFLOAT3(0.0f, 5.0f, 0.0f);
	//聚光灯位置设为相机位置，朝向看的位置
	//_spotLight.position = orignPos;
	//XMStoreFloat3(&_spotLight.direction, XMVector3Normalize(Vector3(0, 0, 0) - Vector3(x, y, z)));

	//set constant buffer
	Shader* shader = Shader::GetShader(RenderPipeline::GetIntance()->Device, "FX\\DeferredShading.fx");//todo
	DirectionalLight dl[1] = { _sun->Light };

	shader->GetVariable("gDirLight")->SetRawValue(&(dl), 0, sizeof(dl));
	shader->GetVariable("gPointLight")->SetRawValue(&_pointLights, 0, sizeof(_pointLights));
	//shader->GetVariable("gSpotLight")->SetRawValue(&_spotLight, 0, sizeof(_spotLight));
	shader->GetVectorVariable("gEyePosW")->SetRawValue(&((XMFLOAT3)_camera->GetTransform()->GetPosition()), 0, sizeof(XMFLOAT3));

	shader = Shader::GetShader(RenderPipeline::GetIntance()->Device, "FX\\DepthLighting.fx");//todo
	shader->GetVariable("gDirLight")->SetRawValue(&(dl), 0, sizeof(dl));
	shader->GetVariable("gPointLight")->SetRawValue(&_pointLights, 0, sizeof(_pointLights));
	//shader->GetVariable("gSpotLight")->SetRawValue(&_spotLight, 0, sizeof(_spotLight));
	shader->GetVectorVariable("gEyePosW")->SetRawValue(&((XMFLOAT3)_camera->GetTransform()->GetPosition()), 0, sizeof(XMFLOAT3));

	shader = Shader::GetShader(RenderPipeline::GetIntance()->Device, "FX\\NormalLighting.fx");//todo
	shader->GetVariable("gDirLight")->SetRawValue(&(dl), 0, sizeof(dl));
	shader->GetVariable("gPointLight")->SetRawValue(&_pointLights, 0, sizeof(_pointLights));
	//shader->GetVariable("gSpotLight")->SetRawValue(&_spotLight, 0, sizeof(_spotLight));
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
	Vector3 camPos = Vector3(cameraPos.x, cameraPos.y, cameraPos.z);
	_sun->GetTransform()->SetRotation(cameraRot);
	_sun->GetTransform()->SetPosition(camPos);
	camPos.Normalize();
	camPos.Negative();
	_sun->Light.direction = camPos;//入射点到光源点(所有的光表示都是从物体入射点到其它)
}