/***************************
//LightHelper.fx
//���սṹ���弰���㺯��  
***************************/

struct DirectionalLight
{
	float4 ambient;	
	float4 diffuse;	
	float4 specular;	
	float3 direction;	
	float pad;			
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;      
					 
	float3 att;
	float pad;
};

struct SpotLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;      
					  
	float3 direction;
	float spot;          

	float3 att;
	float pad;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;//w��ʾ�߹�ǿ��
	float4 reflect;
};

//����ƽ�й�
void ComputeDirectionalLight(Material mat,        //����
	DirectionalLight L,    //ƽ�й�
	float3 normal,        //���㷨��
	float3 toEye,        //���㵽�۾�������
	out float4 ambient,    //������:������
	out float4 diffuse,    //������:�������
	out float4 spec)    //������:�߹�
{
	// �����ʼ��Ϊ0
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ���߷���
	float3 lightVec = -L.direction;

	// ������ֱ�Ӽ���
	ambient = mat.ambient * L.ambient;

	// ����������ϵ��
	//���ߡ����߷����һ��

	float diffuseFactor = dot(lightVec, normal);

	// ���㱳���Դ���ټ���
	[flatten]
	if (diffuseFactor > 0.0f)
	{

		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		//�����������
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		//����߹�
		spec = specFactor * mat.specular * L.specular;
	}
}

//������Դ
void ComputePointLight(Material mat,        //����
	PointLight L,        //���Դ
	float3 pos,            //����λ��
	float3 normal,        //���㷨��
	float3 toEye,        //���㵽�۾�������
	out float4 ambient, //������:������
	out float4 diffuse, //������:�������
	out float4 spec)    //������:�߹�
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//���շ��򣺶��㵽��Դ
	float3 lightVec = L.position - pos;

	//���㵽��Դ����
	float d = length(lightVec);

	//������Χ���ټ���
	if (d > L.range)
		return;

	//��һ�����շ���
	lightVec /= d;

	//���㻷����
	ambient = mat.ambient * L.ambient;

	//������ϵ��
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		//�����������
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		//����߹�
		spec = specFactor * mat.specular * L.specular;
	}

	// ����˥��
	float att = 1.0f / dot(L.att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec *= att;
}
//����۹��
void ComputeSpotLight(Material mat,            //����
	SpotLight L,        //�۹��
	float3 pos,            //����λ��
	float3 normal,        //���㷨��
	float3 toEye,        //���㵽�۾�����
	out float4 ambient, //������:������
	out float4 diffuse, //������:�������
	out float4 spec)    //������:�߹�
{
	//��ʼ�����
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//���շ��򣺶��㵽��Դ
	float3 lightVec = L.position - pos;

	//���㵽��Դ����
	float d = length(lightVec);

	//������ڹ��շ����ټ���
	if (d > L.range)
		return;

	//��һ�����շ���
	lightVec /= d;

	//���㻷����
	ambient = mat.ambient * L.ambient;
	//����������ϵ��
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		//�������
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		//�߹�
		spec = specFactor * mat.specular * L.specular;
	}

	//�۹�˥��ϵ��
	float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.spot);
	//˥��ϵ��
	float att = spot / dot(L.att, float3(1.0f, d, d*d));
	ambient *= spot;
	diffuse *= att;
	spec *= att;
}