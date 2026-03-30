#version 450 core

layout (isolines, equal_spacing, ccw) in;

layout (std140) uniform ViewBlock {
	mat4 view;
};

void main() {
	const vec4 data = gl_in[0].gl_Position;
	const vec2 pos = data.xy;
	const float rad = data.z;

	const uint i = uint(gl_TessCoord.x * 3 + 0.01); // [0, 3]
	vec2 offset;
	switch (i % 3) {
	case 0: {
		offset = vec2(0, -0.75);
		break;
	}
	case 1: {
		offset = vec2(-0.25, -0.5);
		break;
	}
	case 2: {
		offset = vec2(+0.25, -0.5);
		break;
	}
	}

	gl_Position = view * vec4(pos + rad * offset, 0.0, 1.0);
}