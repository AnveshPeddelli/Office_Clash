cbuffer Camera: register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

struct VSInput
{
	float3 pos: POSITION;
	float4 color: COLOR;
};

struct PSInput
{
	float4 pos: SV_POSITION;
	float4 color: COLOR;
};

PSInput VSMain(VSInput input)
{
	PSInput output;

	//Camera transform + simple ortho projection
	float4 pos = float4(input.pos, 1.0f);
	
	pos = mul(pos, world);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.pos = pos;
	output.color = input.color;

	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}