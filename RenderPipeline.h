#pragma once
class RenderPipeline
{
public:
	RenderPipeline();
	~RenderPipeline();

	void BuildShaowMap();

	void DefferLight();

	void ForwardShade();

public:
	static RenderPipeline* GetInstance()
	{
		static RenderPipeline pipe;
		return &pipe;
	}
private:

};