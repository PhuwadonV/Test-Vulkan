#version 460

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 inUv;

layout (location = 0) out vec4 outColor;

layout (binding = 1) uniform sampler splr;
layout (binding = 2) uniform texture2D tex;

void main() {
   outColor = texture(sampler2D(tex, splr), inUv);
}