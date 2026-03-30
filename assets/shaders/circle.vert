#version 450 core

layout (location = 0) in vec2 pos;
layout (location = 1) in float radius;

void main() {
	gl_Position = vec4(pos, radius, 0.0);
}