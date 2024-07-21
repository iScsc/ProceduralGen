shader_type spatial;

uniform sampler2D noise;
uniform sampler2D normal_map;

uniform float quad_size = 2.;
uniform float height_scale = .5;

varying vec2 tex_position;

void vertex() {
	// Called for every vertex the material is visible on.
	tex_position = VERTEX.xz / quad_size + .5;
	float alt = texture(noise, tex_position).r;
	VERTEX.y += height_scale * alt;
}

void fragment() {
	// Called for every pixel the material is visible on.
	NORMAL_MAP = texture(normal_map, tex_position).xyz;
	
	ALBEDO = vec3(tex_position, 0.);
	
}

//void light() {
	// Called for every pixel for every light affecting the material.
	// Uncomment to replace the default light processing function with this one.
//}
