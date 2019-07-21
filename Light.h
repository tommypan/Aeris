#pragma once
#include <windows.h>
#include <DirectXMath.h>

using namespace DirectX;

//ƽ�й�
struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient;	//������
	XMFLOAT4 diffuse;	//�������
	XMFLOAT4 specular;	//�߹�
	XMFLOAT3 direction;	//���շ���
	float pad;			//������HLSL�С�4D�������������ƥ��
};
//���Դ
struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;

	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 position;//��Դλ��
	float range;      //���շ�Χ

					  // Packed into 4D vector: (A0, A1, A2, Pad)
	XMFLOAT3 att;     //˥��ϵ��
	float pad; // Pad the last float so we can set an array of lights if we wanted.
};
//�۹��
struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;

	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 position;//����λ��
	float range;      //���շ�Χ

					  // Packed into 4D vector: (Direction, Spot)
	XMFLOAT3 direction;//���շ���
	float spot;        //����ǿ��ϵ��   

					   // Packed into 4D vector: (Att, Pad)
	XMFLOAT3 att;      //˥��ϵ��
	float pad; // Pad the last float so we can set an array of lights if we wanted.
};
