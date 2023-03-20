#include"Sprite.hlsli"
float4 rotate(float a) // ‰ñ“]s—ñ
{
    float s = sin(a);
    float c = cos(a);
    return float4(c, -s, s, c);
}
float2 twirl(float2 uvs, float2 center, float range, float angle) {
    float d = distance(uvs, center);
    uvs -= center;
    // d = clamp(-angle/range * d + angle,0.,angle); // üŒ`•û®
    d = smoothstep(0., range, range - d) * angle;
    uvs *= rotate(d);
    uvs += center;
    return uvs;
}
Output main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	Output output;
	output.svpos = pos;

	
    output.uv =uv;
    
	return output;
}
