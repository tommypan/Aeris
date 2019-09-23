#include "Loader.h"
#include "Mesh.h"

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
					Vector3 texcoord;
					SplitToVector3(lineBuffer, " ", texcoord);
					Vector2 realTexcoord(texcoord.x, texcoord.y);
					mesh->Vertices[texCoordIndex].coord = realTexcoord;
					texCoordIndex++;
				}
				else if (lineBuffer[1] == 'n')
				{
					if (normalIndex < mesh->Vertices.size())
					{
						Vector3 normal;
						SplitToVector3(lineBuffer, " ", normal);
						mesh->Vertices[normalIndex].normal = normal;
						normalIndex++;
					}
				}
				else
				{
					Vertex tex;
					Vector3 pos;
					SplitToVector3(lineBuffer, " ", pos);
					tex.pos = pos;
					mesh->Vertices.push_back(tex);

					/*Vector3 normal;
					DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&pos)));
					mesh->Vertices.back().normal = normal;*/
				}
			}
			else if (lineBuffer[0] == 'f')
			{
				char* curtok = strtok(lineBuffer, " ");
				curtok = strtok(nullptr, " ");
				byte flag = 0;
				char* curtokArra[4];
				while (!(*curtok == '\n' || flag == 4))
				{
					curtokArra[flag] = curtok;
					curtok = strtok(nullptr, " ");
					flag++;
				}

				for (int i = 0; i < 3;i++)
				{
					Vector3 index;
					SplitToVector3(curtokArra[i], "/", index);
					mesh->Indices.push_back(index.x);
					if (flag == 4)
					{
						SplitToVector3(curtokArra[0], "/", index);
						mesh->Indices.push_back(index.x);

						SplitToVector3(curtokArra[2], "/", index);
						mesh->Indices.push_back(index.x);

						SplitToVector3(curtokArra[3], "/", index);
						mesh->Indices.push_back(index.x);
					}
				}
			

			}
		}

		fclose(fp);
	}

	return false;
}