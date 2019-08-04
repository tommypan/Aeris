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

	if (path.find(".obj"))
	{
		FILE * fp = nullptr;
		fp = fopen(path.c_str(), "r");
		if (fp == nullptr)
		{
			return false;
		}

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
					int x = -0;
				}
				else if (lineBuffer[1] == 'n')
				{
					Vector3 normal;
					SplitToVector3(lineBuffer, " ", normal);
					int x = -0;
				}
				else
				{
					Vector3 pos;
					SplitToVector3(lineBuffer, " ", pos);
					int x = -0;
				}
			}
			else if (lineBuffer[0] == 'f')
			{
				char* curtok = strtok(lineBuffer, " ");
				curtok = strtok(nullptr, " ");
				byte flag = 0;
				char* curtokArra[3];
				while (flag != 3)
				{
					curtokArra[flag] = curtok;
					curtok = strtok(nullptr, " ");
					flag++;
				}

				for (int i = 0; i < 3;i++)
				{
					Vector3 pos;
					SplitToVector3(curtokArra[i], "/", pos);
					int x = -0;
				}
			

			}
		}

		fclose(fp);
	}

	return false;
}