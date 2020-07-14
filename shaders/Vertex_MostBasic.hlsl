cbuffer CONST_DISPLACE_BUFF : register(b0) {
	float3 offset;
}

struct VS_INPUT {
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input) { // Only output is position
	VS_OUTPUT output;

	// output.pos = float4(offset.x + input.pos.x, offset.y + input.pos.y, offset.z + input.pos.z, input.pos.w);
	output.pos = float4(input.pos.x + offset.x, input.pos.y + offset.y, input.pos.z + offset.z, 1.0);
	output.texcoord = float2(input.texcoord[0], input.texcoord[1]);
	return output;
}

/* float4 main(VS_INPUT input): SV_POSITION{
	float4 pos = { offset.x, offset.y, offset.z, 0.0 };
	return float4(input.pos.x, input.pos.y, input.pos.z, 1.0) + pos;
} */