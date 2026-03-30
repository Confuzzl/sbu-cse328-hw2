#version 450 core

//layout(std140) uniform ViewBlock {
//	mat4 view;
//};
//layout(std140) uniform ScreenBlock {
//	uvec2 screen_dimensions;
//};

uniform sampler2D sampler;

in vec2 geom_center;
in float geom_radius;
in vec2 geom_world_position;
in vec2 geom_uv_out;

out vec4 color;

//vec2 worldPosition() {
//	vec2 ndc = (gl_FragCoord.xy / screen_dimensions - 0.5) * 2.0;
//	return (inverse(view) * vec4(ndc, 0.0, 1.0)).xy;
//}

void main() {
	const vec2 diff = geom_world_position - geom_center;
//	const vec2 diff = worldPosition() - center;
	if (dot(diff, diff) > geom_radius * geom_radius)
		discard;
	color = texture(sampler, geom_uv_out);
}