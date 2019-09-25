#include "Loader.h"

Loader::Loader()
{
}

Loader::~Loader()
{
}

bool Loader::Load(const std::string& path,Mesh* mesh)
{
	if (mesh == nullptr)
	{
		return false;
	}

	posv.clear();
	texcoordv.clear();
	normalv.clear();
	btnv.clear();

	if (path.find(".obj"))
	{
		FILE * fp = nullptr;
		fp = fopen(path.c_str(), "r");
		if (fp == nullptr)
		{
			return false;
		}
		int texCoordIndex = 0;
		int normalIndex = 0;
		while (!feof(fp))
		{
			char lineBuffer[256];
			fgets(lineBuffer, 256, fp);
			
			if (lineBuffer[0] == 'v')
			{
				char * splitCh = " ";
				if (lineBuffer[1] == 't')
				{
					XMFLOAT2 texcoord;
					SplitToVector2(lineBuffer, " ", texcoord);
					texcoordv.push_back(texcoord);
				}
				else if (lineBuffer[1] == 'n')
				{
					Vector3 normal;
					SplitToVector3(lineBuffer, " ", normal);
					normalv.push_back(normal);
				}
				else
				{
					XMFLOAT3 pos;
					SplitToVector3(lineBuffer, " ", pos);
					posv.push_back(pos);
					/*Vector3 normal;
					DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&pos)));
					mesh->Vertices.back().normal = normal;*/
				}
			}
			else if (lineBuffer[0] == 'f')
			{
				//f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
				char* curtok = strtok(lineBuffer, " ");
				curtok = strtok(nullptr, " ");
				byte flag = 0;
				char* curtokArra[4];
				char* apArra[4];
				while (!(*curtok == '\n' || flag == 4))
				{
					curtokArra[flag] = curtok;
					apArra[flag] = curtok;
					curtok = strtok(nullptr, " ");
					flag++;
				}

				std::vector<Vector3> fVertex;
				for (int i = 0; i < 3;i++)
				{
					Vector3 index;
					SplitToVector3(apArra[i], "/", index);
					fVertex.push_back(index);
				}
				if (flag == 4)
				{
					Vector3 index;
					SplitToVector3(apArra[3], "/", index);
					fVertex.push_back(index);
				}

				for (int i = 0; i < 3; i++)
				{
					AddVertexToMesh(fVertex[i],mesh);
				}
				if (flag == 4)
				{
					AddVertexToMesh(fVertex[0], mesh);
					AddVertexToMesh(fVertex[2], mesh);
					AddVertexToMesh(fVertex[3], mesh);
				}
			}
		}

		fclose(fp);
	}

	return false;
}