/***************************
//LightHelper.fx
//光照结构定义及计算函数  
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
	float4 specular;//w表示高光强度
	float4 reflect;
};

//计算平行光
void ComputeDirectionalLight(Material mat,        //材质
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
	float3 lightVec = -L.direction;

	// 环境光直接计算
	ambient = mat.ambient * L.ambient;

	// 计算漫反射系数
	//光线、法线方向归一化

	float diffuseFactor = dot(lightVec, normal);

	// 顶点背向光源不再计算
	[flatten]
	if (diffuseFactor > 0.0f)
	{

		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		//计算漫反射光
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		//计算高光
		spec = specFactor * mat.specular * L.specular;
	}
}

//计算点光源
void ComputePointLight(Material mat,        //材质
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
	float3 lightVec = L.position - pos;

	//顶点到光源距离
	float d = length(lightVec);

	//超过范围不再计算
	if (d > L.range)
		return;

	//归一化光照方向
	lightVec /= d;

	//计算环境光
	ambient = mat.ambient * L.ambient;

	//漫反射系数
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		//计算漫反射光
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		//计算高光
		spec = specFactor * mat.specular * L.specular;
	}

	// 计算衰减
	float att = 1.0f / dot(L.att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec *= att;
}
//计算聚光灯
void ComputeSpotLight(Material mat,            //材质
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
	float3 lightVec = L.position - pos;

	//顶点到光源距离
	float d = length(lightVec);

	//距离大于光照方向不再计算
	if (d > L.range)
		return;

	//归一化光照方向
	lightVec /= d;

	//计算环境光
	ambient = mat.ambient * L.ambient;
	//计算漫反射系数
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		//漫反射光
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		//高光
		spec = specFactor * mat.specular * L.specular;
	}

	//聚光衰减系数
	float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.spot);
	//衰减系数
	float att = spot / dot(L.att, float3(1.0f, d, d*d));
	ambient *= spot;
	diffuse *= att;
	spec *= att;
}