void Loader::SplitToVector3(char*  source, char*  split, DirectX::Vector3& result)
{
	char* curtok = strtok(source, split);
	if (!isdigit(*curtok))	curtok = strtok(nullptr, split);
	byte flag = 0;
	while (curtok != nullptr)
	{
		if (flag == 0)
		{
			result.x = atof(curtok);
		}
		else if (flag == 1)
		{
			result.y = atof(curtok);
		}
		else if (flag == 2)
		{
			result.z = atof(curtok);
		}
		flag++;
		curtok = strtok(nullptr, split);
	}
}