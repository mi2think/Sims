struct VS_OUTPUT
{
	float4 position : POSITION;
	float3 color : COLOR;
};

float4x4 gMatWorld;

VS_OUTPUT main(float3 pos : POSITION, float3 c : COLOR)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	o.position = mul(float4(pos, 1), gMatWorld);
	o.color = c;
	return o;
}
