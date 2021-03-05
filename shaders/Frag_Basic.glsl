#version 440

uniform sampler2D baseTex;

/* layout(packed, binding = 2) uniform Block{
	vec2 rotation; // padded to vec4
	vec3 offset; // padded to vec4
}; */

// glsl block index 1
layout(packed) uniform SceneBlock {
	mat4 projMatrix;
};

layout(location = 0) in vec2 texcoord;

out vec4 color;

void main() {
	if (projMatrix[0][0] == 1.0) color = vec4(0.9f, 0.3f, 0.9f, 1.0f); // desired result
	else color = vec4(0.2f, 0.9f, 0.2f, 1.0f); // problem result
	// color = texture(baseTex, texcoord); // Uncomment

	if (color.a < 0.5) discard; // Quick blending fix
}
