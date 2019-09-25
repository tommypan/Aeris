void Loader::SplitToVector3(char*  source, char*  split, XMFLOAT3& result)
{
	char* curtok = strtok(source, split);
	if (!isdigit(*curtok))	curtok = strtok(nullptr, split);
	byte flag = 0;
	while (curtok != nullptr)
	{
		if (flag == 0)
		{
			result.x = (float)atof(curtok);
		}
		else if (flag == 1)
		{
			result.y = (float)atof(curtok);
		}
		else if (flag == 2)
		{
			result.z = (float)atof(curtok);
		}
		flag++;
		curtok = strtok(nullptr, split);
	}
}

void Loader::SplitToVector2(char*  source, char*  split, XMFLOAT2& result)
{
	char* curtok = strtok(source, split);
	if (!isdigit(*curtok))	curtok = strtok(nullptr, split);
	byte flag = 0;
	while (curtok != nullptr)
	{
		if (flag == 0)
		{
			result.x = (float)atof(curtok);
		}
		else if (flag == 1)
		{
			result.y = (float)atof(curtok);
			return;
		}
		flag++;
		curtok = strtok(nullptr, split);
	}
}

void Loader::AddVertexToMesh(Vector3& index, Mesh* mesh)
{
	Vertex vet;
	vet.pos = posv[index.x - 1];
	vet.coord = texcoordv[index.y - 1];
	vet.normal = normalv[index.z - 1];
	UINT vertextIndex = mesh->Vertices.size();
	mesh->Vertices.push_back(vet);
	mesh->Indices.push_back(vertextIndex);
}