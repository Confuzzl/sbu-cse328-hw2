#version 450 core

layout (vertices = 1) out;

void main() {
	gl_TessLevelOuter[0] = 1;
	gl_TessLevelOuter[1] = 3;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}