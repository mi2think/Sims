struct VS_OUTPUT
{
	float4 position : POSITION;
	float3 color : COLOR;
};

float gScale;

VS_OUTPUT main(float3 pos : POSITION, float3 c : COLOR)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	o.position = float4(pos, 1) * float4(gScale, gScale, 1, 1);
	o.color = c;
	return o;
}
