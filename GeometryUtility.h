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
	//单例模式
	static GeometryUtility* GetInstance()
	{
		static GeometryUtility instance;
		return &instance;
	}

	Mesh* CreateScreenRect();

	//创建一个立方体：指定宽(X方向)、高(Y方向)、深(Z方向)
	Mesh* CreateBox(float width, float height, float depth);

	//创建一个网络格子：指定总宽度(X方向)、总高度(Z方向); m、n为宽、高方向上的格子数量
	Mesh* CreateGrid(float width, float height, UINT m, UINT n);

	//创建一个圆柱(不含上、下两个口)：指定上端半径(topRadius)、下端半径(bottomRadius)、高度(height)、
	//sllice、stack
	Mesh* CreateCylinder(float topRadius, float bottomRadius, float height, int slice, int stack);
	
	//给现成的圆柱添加上口
	void AddCylinderTopCap(float topRadius, float bottomRadius, float height, int slice, int stack, Mesh &mesh);
	
	//给现成的圆柱添加下口
	void AddCylinderBottomCap(float topRadius, float bottomRadius, float height, int slice, int stack, Mesh &mesh);
	
	//创建一个球体：指定半径(radius)、slice和stack
	Mesh* CreateSphere(float radius, int slice, int stack);
};

#endif//_GEOMETRY_GENERATOR_H
