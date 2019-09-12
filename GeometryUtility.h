#ifndef _GEOMETRY_GENERATOR_H
#define _GEOMETRY_GENERATOR_H

#include<vector>
#include "RenderPipeline.h"

class Mesh;
class GeometryUtility
{
private:
	GeometryUtility() {}
public:
	//����ģʽ
	static GeometryUtility* GetInstance()
	{
		static GeometryUtility instance;
		return &instance;
	}

	Mesh* CreateScreenRect();

	//����һ�������壺ָ����(X����)����(Y����)����(Z����)
	Mesh* CreateBox(float width, float height, float depth);

	//����һ��������ӣ�ָ���ܿ��(X����)���ܸ߶�(Z����); m��nΪ���߷����ϵĸ�������
	Mesh* CreateGrid(float width, float height, UINT m, UINT n);

	//����һ��Բ��(�����ϡ���������)��ָ���϶˰뾶(topRadius)���¶˰뾶(bottomRadius)���߶�(height)��
	//sllice��stack
	Mesh* CreateCylinder(float topRadius, float bottomRadius, float height, int slice, int stack);
	
	//���ֳɵ�Բ������Ͽ�
	void AddCylinderTopCap(float topRadius, float bottomRadius, float height, int slice, int stack, Mesh &mesh);
	
	//���ֳɵ�Բ������¿�
	void AddCylinderBottomCap(float topRadius, float bottomRadius, float height, int slice, int stack, Mesh &mesh);
	
	//����һ�����壺ָ���뾶(radius)��slice��stack
	Mesh* CreateSphere(float radius, int slice, int stack);
};

#endif//_GEOMETRY_GENERATOR_H
