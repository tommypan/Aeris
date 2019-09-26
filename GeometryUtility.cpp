#include "GeometryUtility.h"
#include "Mesh.h"

Mesh* GeometryUtility::CreateScreenRect()
{
	Mesh *	meshData = new Mesh();
	meshData->Vertices.clear();
	meshData->Indices.clear();
	meshData->Vertices.resize(4);
	meshData->Indices.resize(6);

	float halfW = 1.0f;
	float halfH = 1.0f;

	meshData->Vertices[0].pos = XMFLOAT3(-halfW, -halfH,0);
	meshData->Vertices[0].normal = XMFLOAT3(0.f, 0.f, -1.f);
	meshData->Vertices[0].coord = XMFLOAT2(0.f, 1.f);
	meshData->Vertices[1].pos = XMFLOAT3(-halfW, halfH, 0);
	meshData->Vertices[1].normal = XMFLOAT3(0.f, 0.f, -1.f);
	meshData->Vertices[1].coord = XMFLOAT2(0.f, 0.f);
	meshData->Vertices[2].pos = XMFLOAT3(halfW, halfH, 0);
	meshData->Vertices[2].normal = XMFLOAT3(0.f, 0.f, -1.f);
	meshData->Vertices[2].coord = XMFLOAT2(1.f, 0.f);
	meshData->Vertices[3].pos = XMFLOAT3(halfW, -halfH, 0);
	meshData->Vertices[3].normal = XMFLOAT3(0.f, 0.f, -1.f);
	meshData->Vertices[3].coord = XMFLOAT2(1.f, 1.f);

	meshData->Indices[0] = 0;
	meshData->Indices[1] = 1;
	meshData->Indices[2] = 2;
	meshData->Indices[3] = 0;
	meshData->Indices[4] = 2;
	meshData->Indices[5] = 3;

	return meshData;
}

Mesh* GeometryUtility::CreateBox(float width, float height, float depth)
{
	Mesh* meshData = new Mesh();
	meshData->Vertices.clear();
	meshData->Indices.clear();

	//一共24个顶点(每面4个)
	meshData->Vertices.resize(24);
	//一共36个索引(每面6个)
	meshData->Indices.resize(36);

	float halfW = width * 0.5f;
	float halfH = height * 0.5f;
	float halfD = depth * 0.5f;

	//眼睛面向z轴正方向
	//构建顶点
	//前面
	meshData->Vertices[0].pos = XMFLOAT3(-halfW, -halfH, -halfD);
	meshData->Vertices[0].normal = XMFLOAT3(0.f, 0.f, -1.f);
	meshData->Vertices[0].coord = XMFLOAT2(0.f, 1.f);
	meshData->Vertices[1].pos = XMFLOAT3(-halfW, halfH, -halfD);
	meshData->Vertices[1].normal = XMFLOAT3(0.f, 0.f, -1.f);
	meshData->Vertices[1].coord = XMFLOAT2(0.f, 0.f);
	meshData->Vertices[2].pos = XMFLOAT3(halfW, halfH, -halfD);
	meshData->Vertices[2].normal = XMFLOAT3(0.f, 0.f, -1.f);
	meshData->Vertices[2].coord = XMFLOAT2(1.f, 0.f);
	meshData->Vertices[3].pos = XMFLOAT3(halfW, -halfH, -halfD);
	meshData->Vertices[3].normal = XMFLOAT3(0.f, 0.f, -1.f);
	meshData->Vertices[3].coord = XMFLOAT2(1.f, 1.f);
	//左侧面
	meshData->Vertices[4].pos = XMFLOAT3(-halfW, -halfH, halfD);
	meshData->Vertices[4].normal = XMFLOAT3(-1.f, 0.f, 0.f);
	meshData->Vertices[4].coord = XMFLOAT2(0.f, 1.f);
	meshData->Vertices[5].pos = XMFLOAT3(-halfW, halfH, halfD);
	meshData->Vertices[5].normal = XMFLOAT3(-1.f, 0.f, 0.f);
	meshData->Vertices[5].coord = XMFLOAT2(0.f, 0.f);
	meshData->Vertices[6].pos = XMFLOAT3(-halfW, halfH, -halfD);
	meshData->Vertices[6].normal = XMFLOAT3(-1.f, 0.f, 0.f);
	meshData->Vertices[6].coord = XMFLOAT2(1.f, 0.f);
	meshData->Vertices[7].pos = XMFLOAT3(-halfW, -halfH, -halfD);
	meshData->Vertices[7].normal = XMFLOAT3(-1.f, 0.f, 0.f);
	meshData->Vertices[7].coord = XMFLOAT2(1.f, 1.f);
	//背面
	meshData->Vertices[8].pos = XMFLOAT3(halfW, -halfH, halfD);
	meshData->Vertices[8].normal = XMFLOAT3(0.f, 0.f, 1.f);
	meshData->Vertices[8].coord = XMFLOAT2(0.f, 1.f);
	meshData->Vertices[9].pos = XMFLOAT3(halfW, halfH, halfD);
	meshData->Vertices[9].normal = XMFLOAT3(0.f, 0.f, 1.f);
	meshData->Vertices[9].coord = XMFLOAT2(0.f, 0.f);
	meshData->Vertices[10].pos = XMFLOAT3(-halfW, halfH, halfD);
	meshData->Vertices[10].normal = XMFLOAT3(0.f, 0.f, 1.f);
	meshData->Vertices[10].coord = XMFLOAT2(1.f, 0.f);
	meshData->Vertices[11].pos = XMFLOAT3(-halfW, -halfH, halfD);
	meshData->Vertices[11].normal = XMFLOAT3(0.f, 0.f, 1.f);
	meshData->Vertices[11].coord = XMFLOAT2(1.f, 1.f);
	//右侧面
	meshData->Vertices[12].pos = XMFLOAT3(halfW, -halfH, -halfD);
	meshData->Vertices[12].normal = XMFLOAT3(1.f, 0.f, 0.f);
	meshData->Vertices[12].coord = XMFLOAT2(0.f, 1.f);
	meshData->Vertices[13].pos = XMFLOAT3(halfW, halfH, -halfD);
	meshData->Vertices[13].normal = XMFLOAT3(1.f, 0.f, 0.f);
	meshData->Vertices[13].coord = XMFLOAT2(0.f, 0.f);
	meshData->Vertices[14].pos = XMFLOAT3(halfW, halfH, halfD);
	meshData->Vertices[14].normal = XMFLOAT3(1.f, 0.f, 0.f);
	meshData->Vertices[14].coord = XMFLOAT2(1.f, 0.f);
	meshData->Vertices[15].pos = XMFLOAT3(halfW, -halfH, halfD);
	meshData->Vertices[15].normal = XMFLOAT3(1.f, 0.f, 0.f);
	meshData->Vertices[15].coord = XMFLOAT2(1.f, 1.f);
	//上面
	meshData->Vertices[16].pos = XMFLOAT3(-halfW, halfH, -halfD);
	meshData->Vertices[16].normal = XMFLOAT3(0.f, 1.f, 0.f);
	meshData->Vertices[16].coord = XMFLOAT2(0.f, 1.f);
	meshData->Vertices[17].pos = XMFLOAT3(-halfW, halfH, halfD);
	meshData->Vertices[17].normal = XMFLOAT3(0.f, 1.f, 0.f);
	meshData->Vertices[17].coord = XMFLOAT2(0.f, 0.f);
	meshData->Vertices[18].pos = XMFLOAT3(halfW, halfH, halfD);
	meshData->Vertices[18].normal = XMFLOAT3(0.f, 1.f, 0.f);
	meshData->Vertices[18].coord = XMFLOAT2(1.f, 0.f);
	meshData->Vertices[19].pos = XMFLOAT3(halfW, halfH, -halfD);
	meshData->Vertices[19].normal = XMFLOAT3(0.f, 1.f, 0.f);
	meshData->Vertices[19].coord = XMFLOAT2(1.f, 1.f);
	//底面
	meshData->Vertices[20].pos = XMFLOAT3(-halfW, -halfH, halfD);
	meshData->Vertices[20].normal = XMFLOAT3(0.f, -1.f, 0.f);
	meshData->Vertices[20].coord = XMFLOAT2(0.f, 1.f);
	meshData->Vertices[21].pos = XMFLOAT3(-halfW, -halfH, -halfD);
	meshData->Vertices[21].normal = XMFLOAT3(0.f, -1.f, 0.f);
	meshData->Vertices[21].coord = XMFLOAT2(0.f, 0.f);
	meshData->Vertices[22].pos = XMFLOAT3(halfW, -halfH, -halfD);
	meshData->Vertices[22].normal = XMFLOAT3(0.f, -1.f, 0.f);
	meshData->Vertices[22].coord = XMFLOAT2(1.f, 0.f);
	meshData->Vertices[23].pos = XMFLOAT3(halfW, -halfH, halfD);
	meshData->Vertices[23].normal = XMFLOAT3(0.f, -1.f, 0.f);
	meshData->Vertices[23].coord = XMFLOAT2(1.f, 1.f);

	//构建索引
	meshData->Indices[0] = 0;
	meshData->Indices[1] = 1;
	meshData->Indices[2] = 2;
	meshData->Indices[3] = 0;
	meshData->Indices[4] = 2;
	meshData->Indices[5] = 3;

	meshData->Indices[6] = 4;
	meshData->Indices[7] = 5;
	meshData->Indices[8] = 6;
	meshData->Indices[9] = 4;
	meshData->Indices[10] = 6;
	meshData->Indices[11] = 7;

	meshData->Indices[12] = 8;
	meshData->Indices[13] = 9;
	meshData->Indices[14] = 10;
	meshData->Indices[15] = 8;
	meshData->Indices[16] = 10;
	meshData->Indices[17] = 11;

	meshData->Indices[18] = 12;
	meshData->Indices[19] = 13;
	meshData->Indices[20] = 14;
	meshData->Indices[21] = 12;
	meshData->Indices[22] = 14;
	meshData->Indices[23] = 15;

	meshData->Indices[24] = 16;
	meshData->Indices[25] = 17;
	meshData->Indices[26] = 18;
	meshData->Indices[27] = 16;
	meshData->Indices[28] = 18;
	meshData->Indices[29] = 19;

	meshData->Indices[30] = 20;
	meshData->Indices[31] = 21;
	meshData->Indices[32] = 22;
	meshData->Indices[33] = 20;
	meshData->Indices[34] = 22;
	meshData->Indices[35] = 23;

	return meshData;
}

Mesh* GeometryUtility::CreateGrid(float width, float height, UINT m, UINT n)
{
	Mesh* meshData = new Mesh();
	meshData->Vertices.clear();
	meshData->Indices.clear();
	//每行顶点数、每列顶点数
	UINT nVertsRow = m + 1;
	UINT nVertsCol = n + 1;
	//起始x、z坐标
	float oX = -width * 0.5f;
	float oZ = height * 0.5f;
	//每一格坐标变化
	float dx = width / m;
	float dz = height / n;

	//顶点总数量：nVertsRow * nVertsCol
	meshData->Vertices.resize(nVertsRow * nVertsCol);

	//逐个添加顶点
	for (UINT i = 0; i < nVertsCol; ++i)
	{
		float tmpZ = oZ - dz * i;
		for (UINT j = 0; j < nVertsRow; ++j)
		{
			UINT index = nVertsRow * i + j;
			meshData->Vertices[index].pos.x = oX + dx * j;
			meshData->Vertices[index].pos.y = 0.f;
			meshData->Vertices[index].pos.z = tmpZ;

			meshData->Vertices[index].normal = XMFLOAT3(0.f, 1.f, 0.f);
			meshData->Vertices[index].coord = XMFLOAT2(dx*i, dx*j);
		}
	}

	//总格子数量:m * n
	//因此总索引数量: 6 * m * n
	UINT nIndices = m * n * 6;
	meshData->Indices.resize(nIndices);
	UINT tmp = 0;
	for (UINT i = 0; i < n; ++i)
	{
		for (UINT j = 0; j < m; ++j)
		{
			meshData->Indices[tmp] = i * nVertsRow + j;
			meshData->Indices[tmp + 1] = i * nVertsRow + j + 1;
			meshData->Indices[tmp + 2] = (i + 1) * nVertsRow + j;
			meshData->Indices[tmp + 3] = i * nVertsRow + j + 1;
			meshData->Indices[tmp + 4] = (i + 1) * nVertsRow + j + 1;
			meshData->Indices[tmp + 5] = (i + 1) * nVertsRow + j;

			tmp += 6;
		}
	}

	return meshData;
}

Mesh* GeometryUtility::CreateCylinder(float topRadius, float bottomRadius, float height, int slice, int stack)
{
	Mesh* meshData = new Mesh();
	meshData->Vertices.clear();
	meshData->Indices.clear();

	//从上到下每个stack半径变化量:dRadius
	float dRadius = (bottomRadius - topRadius) / stack;
	//每个stack高度:dHeight
	float dHeight = height / stack;

	//每个圆周上顶点数量:slice+1
	int vertsPerRow = slice + 1;
	//顶点行数:stack+1
	int nRows = stack + 1;

	//总顶点数
	int nVerts = vertsPerRow * nRows;
	//总索引数
	int nIndices = slice * stack * 6;

	meshData->Vertices.resize(nVerts);
	meshData->Indices.resize(nIndices);

	//顶部Y坐标
	float topY = height * 0.5f;

	for (int i = 0; i < nRows; ++i)
	{
		float tmpY = topY - dHeight * i;
		float tmpRadius = topRadius + i * dRadius;

		for (int j = 0; j < vertsPerRow; ++j)
		{
			float theta = XM_2PI * j / slice;
			int index = i * vertsPerRow + j;
			meshData->Vertices[index].pos = XMFLOAT3(tmpRadius*cos(theta), tmpY, tmpRadius*sin(theta));
		}
	}

	UINT tmp(0);
	for (int i = 0; i < stack; ++i)
	{
		for (int j = 0; j < slice; ++j)
		{
			meshData->Indices[tmp] = i * vertsPerRow + j;
			meshData->Indices[tmp + 1] = (i + 1) * vertsPerRow + j + 1;
			meshData->Indices[tmp + 2] = (i + 1) * vertsPerRow + j;
			meshData->Indices[tmp + 3] = i * vertsPerRow + j;
			meshData->Indices[tmp + 4] = i * vertsPerRow + j + 1;
			meshData->Indices[tmp + 5] = (i + 1) * vertsPerRow + j + 1;

			tmp += 6;
		}
	}

	return meshData;
}

void GeometryUtility::AddCylinderTopCap(float topRadius, float bottomRadius, float height, int slice, int stack,Mesh& mesh)
{
	UINT start = mesh.Vertices.size();

	for (int i = 0; i < slice + 1; ++i)
	{
		float theta = XM_2PI * i / slice;

		float x = topRadius*cosf(theta);
		float y = height * 0.5f;
		float z = topRadius*sinf(theta);

		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		mesh.Vertices.push_back(Vertex(XMFLOAT3(x, y, z), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT2(u, v)));
	}

	mesh.Vertices.push_back(Vertex(XMFLOAT3(0.f, height*0.5f, 0.f), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT2(0.5f, 0.5f)));

	UINT center = mesh.Vertices.size() - 1;
	for (int i = 0; i < slice; ++i)
	{
		mesh.Indices.push_back(center);
		mesh.Indices.push_back(start + i + 1);
		mesh.Indices.push_back(start + i);
	}
}

void GeometryUtility::AddCylinderBottomCap(float topRadius, float bottomRadius, float height, int slice, int stack,Mesh& mesh)
{
	UINT start = mesh.Vertices.size();

	for (int i = 0; i < slice + 1; ++i)
	{
		float theta = XM_2PI * i / slice;

		float x = bottomRadius*cosf(theta);
		float y = -height * 0.5f;
		float z = bottomRadius*sinf(theta);

		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		mesh.Vertices.push_back(Vertex(XMFLOAT3(x, y, z), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT2(u, v)));
	}

	mesh.Vertices.push_back(Vertex(XMFLOAT3(0.f, -height*0.5f, 0.f), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT2(0.5f, 0.5f)));

	UINT center = mesh.Vertices.size() - 1;
	for (int i = 0; i < slice; ++i)
	{
		mesh.Indices.push_back(center);
		mesh.Indices.push_back(start + i);
		mesh.Indices.push_back(start + i + 1);
	}

}

Mesh* GeometryUtility::CreateSphere(float radius, int slice, int stack)
{
	Mesh* meshData = new Mesh();
	meshData->Vertices.clear();
	meshData->Indices.clear();

	int vertsPerRow = slice + 1;
	int nRows = stack - 1;

	int nVerts = vertsPerRow * nRows + 2;
	int nIndices = (nRows - 1)*slice * 6 + slice * 6;

	meshData->Vertices.resize(nVerts);
	meshData->Indices.resize(nIndices);

	for (int i = 1; i <= nRows; ++i)
	{
		float phy = XM_PI * i / stack;
		float tmpRadius = radius * sin(phy);
		for (int j = 0; j < vertsPerRow; ++j)
		{
			float theta = XM_2PI * j / slice;
			UINT index = (i - 1)*vertsPerRow + j;

			float x = tmpRadius*cos(theta);
			float y = radius*cos(phy);
			float z = tmpRadius*sin(theta);

			//位置坐标
			meshData->Vertices[index].pos = XMFLOAT3(x, y, z);
			//法线
			XMVECTOR N = XMVectorSet(x, y, z, 0.f);
			XMStoreFloat3(&meshData->Vertices[index].normal, XMVector3Normalize(N));
			//纹理坐标
			meshData->Vertices[index].coord = XMFLOAT2(j*1.f / slice, i*1.f / stack);
		}
	}

	int size = vertsPerRow * nRows;
	//添加顶部和底部两个顶点信息
	meshData->Vertices[size].pos = XMFLOAT3(0.f, radius, 0.f);
	meshData->Vertices[size].normal = XMFLOAT3(0.f, 1.f, 0.f);
	meshData->Vertices[size].coord = XMFLOAT2(0.f, 0.f);

	meshData->Vertices[size + 1].pos = XMFLOAT3(0.f, -radius, 0.f);
	meshData->Vertices[size + 1].normal = XMFLOAT3(0.f, -1.f, 0.f);
	meshData->Vertices[size + 1].coord = XMFLOAT2(0.f, 1.f);

	UINT tmp(0);
	int start1 = 0;
	int start2 = meshData->Vertices.size() - vertsPerRow - 2;
	int top = size;
	int bottom = size + 1;
	for (int i = 0; i < slice; ++i)
	{
		meshData->Indices[tmp] = top;
		meshData->Indices[tmp + 1] = start1 + i + 1;
		meshData->Indices[tmp + 2] = start1 + i;

		tmp += 3;
	}

	for (int i = 0; i < slice; ++i)
	{
		meshData->Indices[tmp] = bottom;
		meshData->Indices[tmp + 1] = start2 + i;
		meshData->Indices[tmp + 2] = start2 + i + 1;

		tmp += 3;
	}

	for (int i = 0; i < nRows - 1; ++i)
	{
		for (int j = 0; j < slice; ++j)
		{
			meshData->Indices[tmp] = i * vertsPerRow + j;
			meshData->Indices[tmp + 1] = (i + 1) * vertsPerRow + j + 1;
			meshData->Indices[tmp + 2] = (i + 1) * vertsPerRow + j;
			meshData->Indices[tmp + 3] = i * vertsPerRow + j;
			meshData->Indices[tmp + 4] = i * vertsPerRow + j + 1;
			meshData->Indices[tmp + 5] = (i + 1) * vertsPerRow + j + 1;

			tmp += 6;
		}
	}

	return meshData;
}
