#version 450 core

const float PI = 3.141592;

layout (isolines, equal_spacing, ccw) in;

layout (std140) uniform ViewBlock {
	mat4 view;
};

void main() {
	const vec4 data = gl_in[0].gl_Position;
	const vec2 pos = data.xy;
	const float rad = data.z;

	// produces circles of tess^2 vertices
	// each isoline forms a sector of the circle
	const float tess = gl_TessLevelOuter[0];
	const float tessStep = 1.0 / tess;
	const float big = gl_TessCoord.y;
	const float small = tessStep * gl_TessCoord.x;
	const float t = big + small;
	
	const float angle = 2 * PI * t;
	const vec2 offset = rad * vec2(cos(angle), sin(angle));
	gl_Position = view * vec4(pos + offset, 0.0, 1.0);
}