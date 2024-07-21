shader_type spatial;

uniform vec2 map_dimensions = vec2(2.);
uniform vec2 map_dist = vec2(1.);

uniform sampler2D color_map : source_color;

varying vec2 tex_position;

void vertex() {
	// Called for every vertex the material is visible on.
	tex_position = VERTEX.xz / (map_dimensions * map_dist) + .5;
}

void fragment() {
	// Called for every pixel the material is visible on.
	ALBEDO = texture(color_map, tex_position).rgb;
}

//void light() {
	// Called for every pixel for every light affecting the material.
	// Uncomment to replace the default light processing function with this one.
//}
