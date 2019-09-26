/***************************
//LightHelper.fx
//���սṹ���弰���㺯��
***************************/

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float Pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float Pad;
};

struct Material
{
	//float4 Ambient;
	float SpecGloss;//��ʾ�߹�ǿ��
	float4 Reflect;
};

//����ƽ�й�
void ComputeDirectionalLight(float4 inAmbient,
	float4 inSpecular,
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
	float3 lightVec = -L.Direction;

	// ������ֱ�Ӽ���
	ambient = inAmbient * L.Ambient;

	// ����������ϵ��
	//���ߡ����߷����һ��

	float diffuseFactor = dot(lightVec, normal);

	// ���㱳���Դ���ټ���
	[flatten]
	if (diffuseFactor > 0.0f)
	{

		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), inSpecular.w);
		//�����������
		float4 mDiffuse = float4(inSpecular.xyz, 0.0f);
		diffuse = diffuseFactor * mDiffuse * L.Diffuse;
		//����߹�
		spec = specFactor * mDiffuse * L.Specular;
	}
}

//������Դ
void ComputePointLight(float4 inAmbient,
	float4 inSpecular,
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
	float3 lightVec = L.Position - pos;

	//���㵽��Դ����
	float d = length(lightVec);

	//������Χ���ټ���
	if (d > L.Range)
		return;

	//��һ�����շ���
	lightVec /= d;

	//���㻷����
	ambient = inAmbient * L.Ambient;

	//������ϵ��
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), inSpecular.w);
		//�����������
		float4 mDiffuse = float4(inSpecular.xyz, 0.0f);
		diffuse = diffuseFactor * mDiffuse * L.Diffuse;
		//����߹�
		spec = specFactor * inSpecular * L.Specular;
	}

	// ����˥��
	float Att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	diffuse *= Att;
	spec *= Att;
}
//����۹��
void ComputeSpotLight(float4 inAmbient,
	float4 inSpecular,
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
	float3 lightVec = L.Position - pos;

	//���㵽��Դ����
	float d = length(lightVec);

	//������ڹ��շ����ټ���
	if (d > L.Range)
		return;

	//��һ�����շ���
	lightVec /= d;

	//���㻷����
	ambient = inAmbient * L.Ambient;
	//����������ϵ��
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), inSpecular.w);
		//�������
		float4 mDiffuse = float4(inSpecular.xyz, 0.0f);
		diffuse = diffuseFactor * mDiffuse * L.Diffuse;
		//�߹�
		spec = specFactor * inSpecular * L.Specular;
	}

	//�۹�˥��ϵ��
	float Spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);
	//˥��ϵ��
	float Att = Spot / dot(L.Att, float3(1.0f, d, d*d));
	ambient *= Spot;
	diffuse *= Att;
	spec *= Att;
}