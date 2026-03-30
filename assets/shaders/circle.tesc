#version 450 core

const float RESOLUTION = 8.0;

layout (vertices = 1) out;

void main() {
	gl_TessLevelOuter[0] = RESOLUTION;
	gl_TessLevelOuter[1] = RESOLUTION;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}