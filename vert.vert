#version 460

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUv;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outUv;

layout (set = 0, binding = 0) uniform vertexTranform {
	mat4 mvp;
};

void main() {
	outUv = inUv;
	outColor = vec4(inColor, 1.0f);
	gl_Position = mvp * vec4(inPos, 1.0f);
}