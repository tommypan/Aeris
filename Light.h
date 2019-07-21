#pragma once
#include <windows.h>
#include <DirectXMath.h>

using namespace DirectX;

//平行光
struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient;	//环境光
	XMFLOAT4 diffuse;	//漫反射光
	XMFLOAT4 specular;	//高光
	XMFLOAT3 direction;	//光照方向
	float pad;			//用于与HLSL中“4D向量”对齐规则匹配
};
//点光源
struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;

	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 position;//光源位置
	float range;      //光照范围

					  // Packed into 4D vector: (A0, A1, A2, Pad)
	XMFLOAT3 att;     //衰减系数
	float pad; // Pad the last float so we can set an array of lights if we wanted.
};
//聚光灯
struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;

	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 position;//光照位置
	float range;      //光照范围

					  // Packed into 4D vector: (Direction, Spot)
	XMFLOAT3 direction;//光照方向
	float spot;        //光照强度系数   

					   // Packed into 4D vector: (Att, Pad)
	XMFLOAT3 att;      //衰减系数
	float pad; // Pad the last float so we can set an array of lights if we wanted.
};
