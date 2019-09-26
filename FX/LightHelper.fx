/***************************
//LightHelper.fx
//光照结构定义及计算函数
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
	float SpecGloss;//表示高光强度
	float4 Reflect;
};

//计算平行光
void ComputeDirectionalLight(float4 inAmbient,
	float4 inSpecular,
	DirectionalLight L,    //平行光
	float3 normal,        //顶点法线
	float3 toEye,        //顶点到眼睛的向量
	out float4 ambient,    //计算结果:环境光
	out float4 diffuse,    //计算结果:漫反射光
	out float4 spec)    //计算结果:高光
{
	// 结果初始化为0
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 光线方向
	float3 lightVec = -L.Direction;

	// 环境光直接计算
	ambient = inAmbient * L.Ambient;

	// 计算漫反射系数
	//光线、法线方向归一化

	float diffuseFactor = dot(lightVec, normal);

	// 顶点背向光源不再计算
	[flatten]
	if (diffuseFactor > 0.0f)
	{

		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), inSpecular.w);
		//计算漫反射光
		float4 mDiffuse = float4(inSpecular.xyz, 0.0f);
		diffuse = diffuseFactor * mDiffuse * L.Diffuse;
		//计算高光
		spec = specFactor * mDiffuse * L.Specular;
	}
}

//计算点光源
void ComputePointLight(float4 inAmbient,
	float4 inSpecular,
	PointLight L,        //点光源
	float3 pos,            //顶点位置
	float3 normal,        //顶点法线
	float3 toEye,        //顶点到眼睛的向量
	out float4 ambient, //计算结果:环境光
	out float4 diffuse, //计算结果:漫反射光
	out float4 spec)    //计算结果:高光
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//光照方向：顶点到光源
	float3 lightVec = L.Position - pos;

	//顶点到光源距离
	float d = length(lightVec);

	//超过范围不再计算
	if (d > L.Range)
		return;

	//归一化光照方向
	lightVec /= d;

	//计算环境光
	ambient = inAmbient * L.Ambient;

	//漫反射系数
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), inSpecular.w);
		//计算漫反射光
		float4 mDiffuse = float4(inSpecular.xyz, 0.0f);
		diffuse = diffuseFactor * mDiffuse * L.Diffuse;
		//计算高光
		spec = specFactor * inSpecular * L.Specular;
	}

	// 计算衰减
	float Att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	diffuse *= Att;
	spec *= Att;
}
//计算聚光灯
void ComputeSpotLight(float4 inAmbient,
	float4 inSpecular,
	SpotLight L,        //聚光灯
	float3 pos,            //顶点位置
	float3 normal,        //顶点法线
	float3 toEye,        //顶点到眼睛向量
	out float4 ambient, //计算结果:环境光
	out float4 diffuse, //计算结果:漫反射光
	out float4 spec)    //计算结果:高光
{
	//初始化结果
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//光照方向：顶点到光源
	float3 lightVec = L.Position - pos;

	//顶点到光源距离
	float d = length(lightVec);

	//距离大于光照方向不再计算
	if (d > L.Range)
		return;

	//归一化光照方向
	lightVec /= d;

	//计算环境光
	ambient = inAmbient * L.Ambient;
	//计算漫反射系数
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), inSpecular.w);
		//漫反射光
		float4 mDiffuse = float4(inSpecular.xyz, 0.0f);
		diffuse = diffuseFactor * mDiffuse * L.Diffuse;
		//高光
		spec = specFactor * inSpecular * L.Specular;
	}

	//聚光衰减系数
	float Spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);
	//衰减系数
	float Att = Spot / dot(L.Att, float3(1.0f, d, d*d));
	ambient *= Spot;
	diffuse *= Att;
	spec *= Att;
}