void Loader::SplitToVector3(char*  source, char*  split, DirectX::Vector3& result)
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