#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout(std140) uniform ViewBlock {
	mat4 view;
};

out vec2 geom_center;
out float geom_radius;
out vec2 geom_world_position;
out vec2 geom_uv_out;

void main() {
	const vec2 center = gl_in[0].gl_Position.xy;
	const float radius = gl_in[0].gl_Position.z;
	geom_center = center;
	geom_radius = radius;
	// 0---2
	// | c |
	// 1---3
	const vec2 uv[4] = {
		vec2(0, 1), vec2(0, 0), vec2(1, 1), vec2(1, 0)
	};
	const vec2 coordinates[4] = {
		center + vec2(-radius, +radius), center + vec2(-radius, -radius),
		center + vec2(+radius, +radius), center + vec2(+radius, -radius)
	};
	for (uint i = 0; i < 4; i++) {
		geom_world_position = coordinates[i];
		geom_uv_out = uv[i];
		gl_Position = view * vec4(geom_world_position, 0, 1);
		EmitVertex();
	}
	EndPrimitive();
}